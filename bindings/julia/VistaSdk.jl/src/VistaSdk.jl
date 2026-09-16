module VistaSdk

include(joinpath(@__DIR__, "..", "deps", "deps.jl"))

include("core/vis_version.jl")
include("ffi/core/vis.jl")
include("core/vis.jl")
include("ffi/core/codebook_name.jl")
include("core/codebook_name.jl")

export Vis, vis, versions, latest
export codebook_name_from_prefix, codebook_name_to_prefix

end
