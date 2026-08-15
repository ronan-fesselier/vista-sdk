use std::path::PathBuf;

fn main() {
    let out_dir_raw = std::env::var("OUT_DIR").unwrap();
    let out_dir = PathBuf::from(out_dir_raw.strip_prefix(r"\\?\").unwrap_or(&out_dir_raw));
    let _ = out_dir;

    #[cfg(feature = "vendored")]
    {
        use std::collections::HashMap;
        use std::process::Command;

        let cpp_dir = {
            let local = PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("cpp");
            if local.join("CMakeLists.txt").is_file() {
                local
            } else {
                PathBuf::from(env!("CARGO_MANIFEST_DIR"))
                    .parent()
                    .unwrap()
                    .parent()
                    .unwrap()
                    .parent()
                    .unwrap()
                    .join("cpp")
            }
        };

        let cmake_build_dir = PathBuf::from(env!("CARGO_MANIFEST_DIR"))
            .join("target")
            .join("cmake-build");

        println!(
            "cargo:rerun-if-changed={}",
            cpp_dir.join("CMakeLists.txt").display()
        );
        emit_rerun_if_changed_dir(&cpp_dir.join("c-api"));

        let msvc_env: Option<HashMap<String, String>> = if cfg!(windows) {
            Some(msvc_dev_env())
        } else {
            None
        };

        let has_ninja = Command::new("ninja").arg("--version").output().is_ok();

        let mut configure = Command::new("cmake");
        if has_ninja {
            configure.arg("-G").arg("Ninja");
        }
        println!("cargo:rerun-if-env-changed=VISTA_SDK_CMAKE_ARGS");
        configure
            .arg("-S")
            .arg(&cpp_dir)
            .arg("-B")
            .arg(&cmake_build_dir)
            .arg("-DCMAKE_BUILD_TYPE=Release")
            .arg("-DDNV_VISTA_SDK_BUILD_C_API=ON");
        if let Ok(extra) = std::env::var("VISTA_SDK_CMAKE_ARGS") {
            for arg in extra.split_whitespace() {
                configure.arg(arg);
            }
        }
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
    }

    #[cfg(not(feature = "vendored"))]
    {
        let lib_dir = std::env::var("VISTA_SDK_LIB_DIR").unwrap_or_else(|_| {
            panic!(
                "vista-sdk: prebuilt libraries not found.\n\
                 Either:\n\
                 - build with the `vendored` feature to compile from source (requires the monorepo \
                 and cmake 3.25+): `cargo build --features vendored`\n\
                 - set VISTA_SDK_LIB_DIR to the directory containing `libdnv-vista-sdk-c.a` and \
                 `libdnv-vista-sdk.a`"
            )
        });
        println!("cargo:rerun-if-env-changed=VISTA_SDK_LIB_DIR");
        println!("cargo:rustc-link-search=native={lib_dir}");
    }

    println!("cargo:rustc-link-lib=static=dnv-vista-sdk-c");
    println!("cargo:rustc-link-lib=static=dnv-vista-sdk");
    if !cfg!(windows) {
        println!("cargo:rustc-link-lib=stdc++");
    }
}

#[cfg(feature = "vendored")]
fn emit_rerun_if_changed_dir(dir: &std::path::Path) {
    let Ok(entries) = std::fs::read_dir(dir) else {
        return;
    };
    for entry in entries.flatten() {
        let path = entry.path();
        if path.is_dir() {
            emit_rerun_if_changed_dir(&path);
        } else {
            println!("cargo:rerun-if-changed={}", path.display());
        }
    }
}

#[cfg(feature = "vendored")]
fn msvc_dev_env() -> std::collections::HashMap<String, String> {
    use std::process::Command;

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
