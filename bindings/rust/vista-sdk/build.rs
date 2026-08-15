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
