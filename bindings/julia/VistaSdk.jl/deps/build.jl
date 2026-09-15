const PACKAGE_DIR = dirname(@__DIR__)

function find_cpp_dir()
    vendored = joinpath(PACKAGE_DIR, "cpp")
    isfile(joinpath(vendored, "CMakeLists.txt")) && return vendored
    abspath(joinpath(PACKAGE_DIR, "..", "..", "..", "cpp"))
end

function generate_vis_version(vis_versions_h::String, out_dir::String)
    src = read(vis_versions_h, String)
    versions = unique([m[1] for m in eachmatch(r"\bv(\d+_\d+[a-z]?)\b", src)])
    versions = [replace(v, "_" => "-") for v in versions]

    mkpath(out_dir)
    open(joinpath(out_dir, "vis_version_generated.jl"), "w") do io
        println(io, "# generated - do not edit")
        print(
            io,
            "\"\"\"\n    VisVersion\n\nA released version of the DNV Vessel Information Structure (VIS).\n\n# Variants\n",
        )
        for v in versions
            id = replace(v, "-" => "_")
            println(io, "- `V" * id * "` : `\"" * v * "\"`")
        end
        println(io, "\"\"\"")
        println(io, "@enum VisVersion begin")
        for v in versions
            id = replace(v, "-" => "_")
            println(io, "    V" * id)
        end
        println(io, "end")
        println(io)
        print(io, "export VisVersion")
        for v in versions
            id = replace(v, "-" => "_")
            print(io, ", V" * id)
        end
        println(io)
        println(io)
        println(io, "const _VIS_VERSION_STRINGS = Dict{VisVersion,String}(")
        for v in versions
            id = replace(v, "-" => "_")
            println(io, "    V" * id * " => " * repr(v) * ",")
        end
        println(io, ")")
        println(io)
        println(io, "Base.show(io::IO, v::VisVersion) = print(io, _VIS_VERSION_STRINGS[v])")
        println(io, "Base.string(v::VisVersion) = _VIS_VERSION_STRINGS[v]")
        println(io)
        println(io, "function Base.parse(::Type{VisVersion}, s::AbstractString)")
        println(io, "    for (k, str) in _VIS_VERSION_STRINGS")
        println(io, "        str == s && return k")
        println(io, "    end")
        println(io, "    throw(ArgumentError(\"unknown VisVersion: \\\"\" * s * \"\\\"\"))")
        println(io, "end")
    end
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

vis_versions_h =
    joinpath(CPP_DIR, "include", "dnv", "vista", "sdk", "core", "VisVersions.h")
generate_vis_version(vis_versions_h, joinpath(PACKAGE_DIR, "deps", "generated"))
