const PACKAGE_DIR = dirname(@__DIR__)

function find_cpp_dir()
    vendored = joinpath(PACKAGE_DIR, "cpp")
    isfile(joinpath(vendored, "CMakeLists.txt")) && return vendored
    abspath(joinpath(PACKAGE_DIR, "..", "..", "..", "cpp"))
end

const CPP_DIR = find_cpp_dir()
const BUILD_DIR = joinpath(PACKAGE_DIR, "deps", "cmake-build")

run(`cmake -S $CPP_DIR -B $BUILD_DIR
    -DCMAKE_BUILD_TYPE=Release
    -DBUILD_SHARED_LIBS=ON
    -DDNV_VISTA_SDK_BUILD_C_API=ON`)
run(`cmake --build $BUILD_DIR --config Release --parallel`)

ext = Sys.iswindows() ? "dll" : Sys.isapple() ? "dylib" : "so"
lib_candidates = String[]
for (root, dirs, files) in walkdir(BUILD_DIR)
    for f in files
        if endswith(f, ".$ext") && contains(f, "vista-sdk-c")
            push!(lib_candidates, joinpath(root, f))
        end
    end
end
isempty(lib_candidates) && error("could not find vista-sdk-c shared library in $BUILD_DIR")
lib_path = first(lib_candidates)

open(joinpath(PACKAGE_DIR, "deps", "deps.jl"), "w") do io
    println(io, "# generated - do not edit")
    println(io, "const VISTA_LIB = " * repr(lib_path))
end
