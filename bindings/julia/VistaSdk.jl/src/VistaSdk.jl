module VistaSdk

include(joinpath(@__DIR__, "..", "deps", "deps.jl"))

include("core/vis_version.jl")
include("ffi/core/vis.jl")
include("core/vis.jl")
include("ffi/core/codebook_name.jl")
include("core/codebook_name.jl")
include("ffi/core/metadata_tag.jl")
include("ffi/core/codebook.jl")
include("ffi/core/codebooks.jl")
include("core/metadata_tag.jl")
include("core/codebook.jl")
include("core/codebooks.jl")

export Vis, vis, versions, latest
export codebook_name_from_prefix, codebook_name_to_prefix
export MetadataTag, name, value, is_custom
export Codebook,
    standard_values, groups, has_group, has_standard_value, validate_position, create_tag
export Codebooks, version

end
