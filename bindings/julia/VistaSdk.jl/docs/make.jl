using Documenter
using VistaSdk

using Pkg
version = Pkg.project().version

makedocs(
    sitename = "VistaSdk.jl v$version",
    modules = [VistaSdk],
    pages = ["Home" => "index.md"],
    format = Documenter.HTML(size_threshold = 600 * 1024, size_threshold_warn = 400 * 1024),
)
