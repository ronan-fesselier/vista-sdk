use std::collections::HashMap;
use std::path::PathBuf;
use std::process::Command;

fn main() {
    let manifest_dir = PathBuf::from(env!("CARGO_MANIFEST_DIR"));
    let cpp_dir = manifest_dir
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .join("cpp");
    let out_dir_raw = std::env::var("OUT_DIR").unwrap();
    let out_dir = PathBuf::from(out_dir_raw.strip_prefix(r"\\?\").unwrap_or(&out_dir_raw));
    let cmake_build_dir = out_dir.join("cmake-build");

    println!(
        "cargo:rerun-if-changed={}",
        cpp_dir.join("CMakeLists.txt").display()
    );
    println!("cargo:rerun-if-changed={}", cpp_dir.join("c-api").display());

    let vis_versions_h = cpp_dir.join("include/dnv/vista/sdk/core/VisVersions.h");
    println!("cargo:rerun-if-changed={}", vis_versions_h.display());

    let iso19848_versions_h = cpp_dir.join("include/dnv/vista/sdk/transport/ISO19848Versions.h");
    println!("cargo:rerun-if-changed={}", iso19848_versions_h.display());

    let version = extract_version(&cpp_dir.join("CMakeLists.txt"));
    println!("cargo:rustc-env=VISTA_SDK_VERSION={version}");

    let msvc_env = if cfg!(windows) {
        Some(msvc_dev_env())
    } else {
        None
    };

    let has_ninja = Command::new("ninja").arg("--version").output().is_ok();

    let mut configure = Command::new("cmake");
    if has_ninja {
        configure.arg("-G").arg("Ninja");
    }
    configure
        .arg("-S")
        .arg(&cpp_dir)
        .arg("-B")
        .arg(&cmake_build_dir)
        .arg("-DCMAKE_BUILD_TYPE=Release")
        .arg("-DDNV_VISTA_SDK_BUILD_C_API=ON");
    if let Some(env) = &msvc_env {
        configure.envs(env);
    }
    let status = configure
        .status()
        .expect("cmake not found - install cmake 3.25+");
    assert!(status.success(), "cmake configure failed");

    let mut build = Command::new("cmake");
    build
        .arg("--build")
        .arg(&cmake_build_dir)
        .arg("--target")
        .arg("dnv-vista-sdk-c")
        .arg("dnv-vista-sdk");
    if let Some(env) = &msvc_env {
        build.envs(env);
    }
    let status = build.status().expect("cmake --build failed");
    assert!(status.success(), "cmake build failed");

    let lib_dir = cmake_build_dir.join("lib");
    println!("cargo:rustc-link-search=native={}", lib_dir.display());
    println!("cargo:rustc-link-lib=static=dnv-vista-sdk-c");
    println!("cargo:rustc-link-lib=static=dnv-vista-sdk");
    if !cfg!(windows) {
        println!("cargo:rustc-link-lib=stdc++");
    }

    generate_vis_version(&vis_versions_h, &out_dir);
    generate_iso19848_version(&iso19848_versions_h, &out_dir);
}

fn msvc_dev_env() -> HashMap<String, String> {
    let vswhere =
        PathBuf::from(std::env::var("ProgramFiles(x86)").expect("ProgramFiles(x86) not set"))
            .join("Microsoft Visual Studio")
            .join("Installer")
            .join("vswhere.exe");
    let output = Command::new(&vswhere)
        .args([
            "-latest",
            "-products",
            "*",
            "-requires",
            "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
            "-property",
            "installationPath",
        ])
        .output()
        .expect("failed to run vswhere.exe - is Visual Studio installed?");
    let vs_install_path =
        String::from_utf8(output.stdout).expect("vswhere output is not valid UTF-8");
    let vs_install_path = vs_install_path.trim();
    assert!(
        !vs_install_path.is_empty(),
        "vswhere found no VS installation with the MSVC C++ toolset"
    );

    let vcvars64 = PathBuf::from(vs_install_path).join("VC/Auxiliary/Build/vcvars64.bat");
    let output = Command::new("cmd")
        .args(["/c", "call"])
        .arg(&vcvars64)
        .args([">", "nul", "&&", "set"])
        .output()
        .expect("failed to run vcvars64.bat");
    assert!(output.status.success(), "vcvars64.bat failed");

    String::from_utf8(output.stdout)
        .expect("vcvars64.bat output is not valid UTF-8")
        .lines()
        .filter_map(|line| line.split_once('='))
        .map(|(k, v)| (k.to_string(), v.to_string()))
        .collect()
}

fn generate_vis_version(vis_versions_h: &std::path::Path, out_dir: &std::path::Path) {
    let content = std::fs::read_to_string(vis_versions_h).expect("cannot read VisVersions.h");

    let variants: Vec<String> = content
        .lines()
        .filter_map(|line| {
            let trimmed = line.trim().trim_end_matches(',');
            if trimmed.starts_with('v')
                && trimmed.chars().nth(1).map_or(false, |c| c.is_ascii_digit())
            {
                Some(trimmed.to_string())
            } else {
                None
            }
        })
        .collect();

    assert!(
        !variants.is_empty(),
        "no VisVersion variants found in VisVersions.h"
    );

    let last = variants.last().unwrap().clone();

    let mut out = String::new();

    out.push_str("#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]\n");
    out.push_str("pub enum VisVersion {\n");
    for v in &variants {
        out.push_str(&format!("    {},\n", to_pascal(v)));
    }
    out.push_str("}\n\n");

    out.push_str("impl VisVersion {\n");
    out.push_str("    pub fn all() -> &'static [VisVersion] {\n");
    out.push_str("        &[\n");
    for v in &variants {
        out.push_str(&format!("            VisVersion::{},\n", to_pascal(v)));
    }
    out.push_str("        ]\n");
    out.push_str("    }\n\n");

    out.push_str(&format!(
        "    pub fn latest() -> VisVersion {{\n        VisVersion::{}\n    }}\n\n",
        to_pascal(&last)
    ));

    out.push_str("    pub fn as_str(self) -> &'static str {\n        match self {\n");
    for v in &variants {
        out.push_str(&format!(
            "            VisVersion::{} => \"{}\",\n",
            to_pascal(v),
            to_vis_str(v)
        ));
    }
    out.push_str("        }\n    }\n}\n\n");

    out.push_str("impl std::fmt::Display for VisVersion {\n");
    out.push_str("    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {\n");
    out.push_str("        f.write_str(self.as_str())\n");
    out.push_str("    }\n}\n\n");

    out.push_str("impl std::str::FromStr for VisVersion {\n");
    out.push_str("    type Err = ();\n\n");
    out.push_str("    fn from_str(s: &str) -> Result<Self, Self::Err> {\n");
    out.push_str("        match s {\n");
    for v in &variants {
        out.push_str(&format!(
            "            \"{}\" => Ok(VisVersion::{}),\n",
            to_vis_str(v),
            to_pascal(v)
        ));
    }
    out.push_str("            _ => Err(()),\n");
    out.push_str("        }\n    }\n}\n");

    std::fs::write(out_dir.join("vis_version.rs"), out).expect("cannot write vis_version.rs");
}

fn to_pascal(variant: &str) -> String {
    let mut result = String::new();
    let mut first = true;
    for ch in variant.chars() {
        if first && ch.is_alphabetic() {
            result.extend(ch.to_uppercase());
            first = false;
        } else {
            result.push(ch);
        }
    }
    result
}

fn to_vis_str(variant: &str) -> String {
    variant.trim_start_matches('v').replace('_', "-")
}

fn generate_iso19848_version(iso19848_versions_h: &std::path::Path, out_dir: &std::path::Path) {
    let content =
        std::fs::read_to_string(iso19848_versions_h).expect("cannot read ISO19848Versions.h");

    let variants: Vec<String> = content
        .lines()
        .filter_map(|line| {
            let trimmed = line.trim();
            let token = trimmed
                .split_whitespace()
                .next()
                .unwrap_or("")
                .trim_end_matches(',');
            if token.starts_with('v') && token.chars().nth(1).map_or(false, |c| c.is_ascii_digit())
            {
                Some(token.to_string())
            } else {
                None
            }
        })
        .collect();

    assert!(
        !variants.is_empty(),
        "no ISO19848Version variants found in ISO19848Versions.h"
    );

    let last = variants.last().unwrap().clone();

    let mut out = String::new();

    out.push_str("#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]\n");
    out.push_str("pub enum Iso19848Version {\n");
    for v in &variants {
        out.push_str(&format!("    {},\n", to_pascal(v)));
    }
    out.push_str("}\n\n");

    out.push_str("impl Iso19848Version {\n");
    out.push_str("    /// Returns all available ISO 19848 versions, in ascending order.\n");
    out.push_str("    pub fn all() -> &'static [Iso19848Version] {\n");
    out.push_str("        &[\n");
    for v in &variants {
        out.push_str(&format!("            Iso19848Version::{},\n", to_pascal(v)));
    }
    out.push_str("        ]\n");
    out.push_str("    }\n\n");

    out.push_str("    /// Returns the latest ISO 19848 version.\n");
    out.push_str(&format!(
        "    pub fn latest() -> Iso19848Version {{\n        Iso19848Version::{}\n    }}\n\n",
        to_pascal(&last)
    ));

    out.push_str("    /// Returns the canonical string representation (e.g. `\"v2018\"`).\n");
    out.push_str("    pub fn as_str(self) -> &'static str {\n        match self {\n");
    for v in &variants {
        out.push_str(&format!(
            "            Iso19848Version::{} => \"{}\",\n",
            to_pascal(v),
            v
        ));
    }
    out.push_str("        }\n    }\n}\n\n");

    out.push_str("impl std::fmt::Display for Iso19848Version {\n");
    out.push_str("    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {\n");
    out.push_str("        f.write_str(self.as_str())\n");
    out.push_str("    }\n}\n\n");

    out.push_str("impl std::str::FromStr for Iso19848Version {\n");
    out.push_str("    type Err = ();\n\n");
    out.push_str("    fn from_str(s: &str) -> Result<Self, Self::Err> {\n");
    out.push_str("        match s {\n");
    for v in &variants {
        out.push_str(&format!(
            "            \"{}\" => Ok(Iso19848Version::{}),\n",
            v,
            to_pascal(v)
        ));
    }
    out.push_str("            _ => Err(()),\n");
    out.push_str("        }\n    }\n}\n");

    std::fs::write(out_dir.join("iso19848_version.rs"), out)
        .expect("cannot write iso19848_version.rs");
}

fn extract_version(cmake_lists: &std::path::Path) -> String {
    let content = std::fs::read_to_string(cmake_lists).expect("cannot read cpp/CMakeLists.txt");

    let mut in_project = false;
    for line in content.lines() {
        let trimmed = line.trim();
        if trimmed.starts_with("project(") {
            in_project = true;
        }
        if in_project {
            if let Some(v) = trimmed.strip_prefix("VERSION ") {
                return v.trim().to_string();
            }
            if trimmed.contains(')') {
                in_project = false;
            }
        }
    }
    panic!("VERSION not found in project() block of cpp/CMakeLists.txt");
}
