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
include("ffi/core/gmod_node_metadata.jl")
include("ffi/core/gmod_node.jl")
include("ffi/core/gmod.jl")
include("ffi/core/location.jl")
include("ffi/core/location_group.jl")
include("ffi/core/relative_location.jl")
include("ffi/core/locations.jl")
include("ffi/core/location_builder.jl")
include("ffi/core/gmod_path.jl")
include("ffi/core/gmod_individualizable_set.jl")
include("ffi/core/local_id.jl")
include("ffi/core/local_id_builder.jl")
include("ffi/core/local_id_mqtt.jl")
include("ffi/core/universal_id.jl")
include("ffi/core/universal_id_builder.jl")
include("core/parsing_errors.jl")
include("core/metadata_tag.jl")
include("core/codebook.jl")
include("core/codebooks.jl")
include("core/imo_number.jl")
include("core/gmod_node_metadata.jl")
include("core/gmod_node.jl")
include("core/gmod.jl")
include("core/location.jl")
include("core/relative_location.jl")
include("core/locations.jl")
include("core/location_builder.jl")
include("core/gmod_path.jl")
include("core/gmod_individualizable_set.jl")
include("core/local_id.jl")
include("core/local_id_builder.jl")
include("core/local_id_mqtt.jl")
include("core/universal_id_builder.jl")
include("core/universal_id.jl")

export Vis,
    vis,
    versions,
    latest,
    codebooks,
    locations,
    gmod,
    convert_node,
    convert_path,
    convert_local_id_builder,
    convert_local_id
export codebook_name_from_prefix, codebook_name_to_prefix
export MetadataTagRef, MetadataTag, name, value, is_custom
export LocalIdRef,
    LocalId,
    LocalIdBuilderRef,
    LocalIdBuilder,
    MqttLocalId,
    naming_rule,
    from_string,
    from_string_with_errors,
    primary_item,
    secondary_item,
    is_verbose_mode,
    has_custom_tag,
    metadata_tag,
    metadata_tags,
    builder,
    quantity,
    content,
    calculation,
    state,
    command,
    tag_type,
    detail,
    is_valid,
    is_empty,
    is_empty_metadata,
    with_vis_version,
    without_vis_version,
    with_primary_item,
    without_primary_item,
    with_secondary_item,
    without_secondary_item,
    with_metadata_tag,
    without_metadata_tag,
    with_verbose_mode,
    create
export UniversalIdRef,
    UniversalId,
    UniversalIdBuilderRef,
    UniversalIdBuilder,
    naming_entity,
    imo_number,
    local_id,
    with_imo_number,
    without_imo_number,
    with_local_id,
    without_local_id
export Codebook,
    standard_values, groups, has_group, has_standard_value, validate_position, create_tag
export Codebooks, version
export ParsingErrors, has_errors, has_error_type
export VistaError, last_error, clear_error
export ImoNumber, is_valid
export GmodNodeMetadata,
    category,
    node_type,
    full_type,
    common_name,
    common_definition,
    install_substructure,
    normal_assignment_name_count,
    normal_assignment_name_at,
    normal_assignment_names
export Gmod,
    root_node,
    get_node,
    node_count,
    node_at,
    traverse,
    TraversalHandlerResult,
    TraversalStop,
    TraversalSkipSubtree,
    TraversalContinue
export GmodPath,
    GmodPathRef,
    from_short_path,
    from_short_path_with_errors,
    from_full_path,
    from_full_path_with_errors,
    node,
    is_individualizable,
    without_locations,
    normal_assignment_name,
    individualizable_set_count,
    individualizable_set_at,
    common_name_count,
    common_names,
    to_full_path_string,
    to_string_dump
export GmodIndividualizableSet, index_count, index_at, set_location, build
export GmodNodeRef,
    GmodNode,
    metadata,
    location,
    children,
    child_count,
    child_at,
    parents,
    parent_count,
    parent_at,
    product_type,
    product_selection,
    is_function_composition,
    is_mappable,
    is_product_selection,
    is_product_type,
    is_asset,
    is_leaf_node,
    is_function_node,
    is_asset_function_node,
    is_root,
    is_child,
    is_child_code
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
