module VistaSdk

include(joinpath(@__DIR__, "..", "deps", "deps.jl"))

include("core/vis_version.jl")
include("ffi/core/vis.jl")
include("core/vis.jl")
include("ffi/core/codebook_name.jl")
include("core/codebook_name.jl")
include("ffi/core/error.jl")
include("core/error.jl")
include("ffi/core/parsing_errors.jl")
include("ffi/core/metadata_tag.jl")
include("ffi/core/codebook.jl")
include("ffi/core/codebooks.jl")
include("ffi/core/imo_number.jl")
include("ffi/core/location.jl")
include("ffi/core/location_group.jl")
include("ffi/core/relative_location.jl")
include("ffi/core/locations.jl")
include("ffi/core/location_builder.jl")
include("core/parsing_errors.jl")
include("core/metadata_tag.jl")
include("core/codebook.jl")
include("core/codebooks.jl")
include("core/imo_number.jl")
include("core/location.jl")
include("core/relative_location.jl")
include("core/locations.jl")
include("core/location_builder.jl")

export Vis, vis, versions, latest, codebooks, locations
export codebook_name_from_prefix, codebook_name_to_prefix
export MetadataTag, name, value, is_custom
export Codebook,
    standard_values, groups, has_group, has_standard_value, validate_position, create_tag
export Codebooks, version
export ParsingErrors, has_errors, has_error_type
export VistaError, last_error, clear_error
export ImoNumber, is_valid
export Location, LocationGroup
export RelativeLocation, code, definition, location_value
export Locations, group, parse_with_errors
export LocationBuilder,
    number,
    side,
    vertical,
    transverse,
    longitudinal,
    with_number,
    without_number,
    with_side,
    without_side,
    with_vertical,
    without_vertical,
    with_transverse,
    without_transverse,
    with_longitudinal,
    without_longitudinal,
    with_code,
    with_location,
    without_value,
    build

end
