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
include("ffi/query/metadata_tags_query.jl")
include("ffi/query/metadata_tags_query_builder.jl")
include("ffi/query/gmod_path_query.jl")
include("ffi/query/gmod_path_query_builder.jl")
include("ffi/query/local_id_query.jl")
include("ffi/query/local_id_query_builder.jl")
include("query/metadata_tags_query.jl")
include("query/gmod_path_query.jl")
include("query/local_id_query.jl")
include("transport/iso19848_version.jl")
include("ffi/types/time_span.jl")
include("ffi/types/date_time.jl")
include("ffi/types/decimal.jl")
include("types/time_span.jl")
include("types/date_time.jl")
include("types/decimal.jl")
include("ffi/transport/iso19848.jl")
include("transport/iso19848.jl")

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
    with_tag,
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
export MetadataTagsQuery,
    MetadataTagsQueryBuilderRef,
    MetadataTagsQueryBuilder,
    from_local_id,
    with_allow_other_tags,
    is_match
export GmodPathQuery,
    GmodPathQueryBuilderRef,
    GmodPathQueryBuilder,
    from_path,
    path_with_node_all_locations,
    path_with_node_locations,
    with_any_node_before,
    with_any_node_after,
    with_node_all_locations,
    with_node_locations
export LocalIdQuery,
    LocalIdQueryBuilderRef,
    LocalIdQueryBuilder,
    with_primary_item_query,
    with_secondary_item_query,
    with_primary_item_nodes_builder,
    with_primary_item_path_builder,
    with_any_secondary_item,
    without_secondary_item,
    with_secondary_item_nodes_builder,
    with_secondary_item_path_builder,
    with_tags,
    tags_builder,
    is_match_str
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
export TimeSpan,
    from_days,
    from_hours,
    from_minutes,
    from_seconds,
    from_millis,
    from_micros,
    ticks,
    days,
    hours,
    minutes,
    seconds,
    millis,
    micros,
    nanos,
    negate,
    divide,
    ratio
export DateTimeFormat,
    Iso8601,
    Iso8601Precise,
    Iso8601PreciseTrimmed,
    Iso8601Millis,
    Iso8601Micros,
    Iso8601Extended,
    Iso8601Basic,
    Iso8601Date,
    Iso8601Time,
    UnixSeconds,
    UnixMilliseconds
export DateTime,
    from_epoch_seconds,
    from_epoch_millis,
    utc_now,
    epoch,
    is_leap_year,
    days_in_month,
    year,
    month,
    day,
    hour,
    minute,
    second,
    millisecond,
    microsecond,
    nanosecond,
    day_of_week,
    day_of_year,
    to_epoch_seconds,
    to_epoch_millis,
    date,
    time_of_day,
    add_days,
    add_hours,
    add_minutes,
    add_seconds,
    add_milliseconds,
    add_months,
    add_years,
    to_string
export DateTimeOffset,
    now,
    today,
    utc_date_time,
    local_date_time,
    utc_ticks,
    total_offset_minutes,
    to_offset,
    to_universal_time,
    to_local_time,
    to_filetime,
    from_filetime,
    equals_exact,
    date_time,
    offset
export RoundingMode,
    ToNearest, ToNearestTiesAway, ToZero, ToPositiveInfinity, ToNegativeInfinity
export Decimal, lowest, scale, decimal_places_count, total_digits_count, to_f64, to_bits
export Iso19848,
    ISO19848,
    Iso19848Value,
    DataChannelTypeName,
    DataChannelTypeNames,
    FormatDataType,
    FormatDataTypeRef,
    FormatDataTypes,
    data_channel_type_names,
    format_data_types,
    find,
    type_,
    description,
    validate,
    iso19848_value_from_string,
    iso19848_value_from_integer,
    iso19848_value_from_boolean,
    iso19848_value_from_decimal,
    iso19848_value_from_date_time,
    iso19848_value_to_string

end
