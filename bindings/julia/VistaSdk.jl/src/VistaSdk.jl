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
include("ffi/transport/ship_id.jl")
include("transport/ship_id.jl")
include("ffi/transport/serializable_document.jl")
include("ffi/transport/datachannel/data_channel_dto.jl")
include("transport/serializable_document.jl")
include("ffi/transport/datachannel/data_channel.jl")
include("transport/datachannel/data_channel.jl")
include("transport/datachannel/data_channel_dto.jl")
include("transport/datachannel/data_channel_json.jl")
include("ffi/transport/timeseries/data_channel_id.jl")
include("transport/timeseries/data_channel_id.jl")
include("ffi/transport/timeseries/time_series_data.jl")
include("transport/timeseries/time_series_data.jl")
include("ffi/transport/timeseries/time_series_data_dto.jl")
include("transport/timeseries/time_series_data_dto.jl")
include("transport/timeseries/time_series_data_json.jl")

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
export ShipId,
    ShipIdImo,
    ShipIdOther,
    from_imo_number,
    from_other_id,
    is_imo_number,
    is_other_id,
    other_id
export DocumentKind,
    DocNull, DocBoolean, DocInteger, DocDouble, DocString, DocArray, DocObject
export SerializableDocument,
    SerializableDocumentRef,
    sd_array,
    sd_object,
    kind,
    is_null,
    is_boolean,
    is_integer,
    is_double,
    is_string,
    is_array,
    is_object,
    as_boolean,
    as_integer,
    as_double,
    as_string,
    array_size,
    array_at,
    push_back!,
    object_size,
    object_key_at,
    object_value_at,
    has_key,
    set!
export WhiteSpace, Preserve, Replace, Collapse
export DclRestriction,
    DclRestrictionRef,
    enumeration_count,
    enumeration_at,
    set_enumeration!,
    clear_enumeration!,
    fraction_digits,
    set_fraction_digits!,
    clear_fraction_digits!,
    length_,
    set_length!,
    clear_length!,
    max_exclusive,
    set_max_exclusive!,
    clear_max_exclusive!,
    max_inclusive,
    set_max_inclusive!,
    clear_max_inclusive!,
    max_length,
    set_max_length!,
    clear_max_length!,
    min_exclusive,
    set_min_exclusive!,
    clear_min_exclusive!,
    min_inclusive,
    set_min_inclusive!,
    clear_min_inclusive!,
    min_length,
    set_min_length!,
    clear_min_length!,
    pattern,
    set_pattern!,
    clear_pattern!,
    total_digits,
    set_total_digits!,
    clear_total_digits!,
    white_space,
    set_white_space!,
    clear_white_space!,
    validate_value
export DclRange, DclRangeRef, low, high, set_low!, set_high!
export DclFormat,
    DclFormatRef, set_type_!, restriction, set_restriction!, clear_restriction!
export DclDataChannelType,
    DclDataChannelTypeRef,
    update_cycle,
    set_update_cycle!,
    clear_update_cycle!,
    calculation_period,
    set_calculation_period!,
    clear_calculation_period!,
    is_alert
export DclNameObject,
    DclNameObjectRef,
    naming_rule,
    set_naming_rule!,
    custom_name_objects,
    set_custom_name_objects!,
    clear_custom_name_objects!
export DclUnit,
    DclUnitRef,
    unit_symbol,
    set_unit_symbol!,
    quantity_name,
    set_quantity_name!,
    clear_quantity_name!,
    set_custom_elements!,
    clear_custom_elements!
export DclProperty,
    DclPropertyRef,
    data_channel_type,
    set_data_channel_type!,
    format,
    set_format!,
    range,
    set_range!,
    clear_range!,
    unit,
    set_unit!,
    clear_unit!,
    quality_coding,
    set_quality_coding!,
    clear_quality_coding!,
    alert_priority,
    set_alert_priority!,
    clear_alert_priority!,
    name,
    set_name!,
    clear_name!,
    remarks,
    set_remarks!,
    clear_remarks!,
    custom_properties,
    set_custom_properties!,
    clear_custom_properties!
export DclConfigurationReference,
    DclConfigurationReferenceRef,
    id,
    set_id!,
    version,
    set_version!,
    clear_version!,
    timestamp,
    set_timestamp!
export DclVersionInformation,
    DclVersionInformationRef,
    naming_scheme_version,
    set_naming_scheme_version!,
    reference_url,
    set_reference_url!,
    clear_reference_url!
export DclDataChannelId,
    DclDataChannelIdRef,
    local_id_string,
    short_id,
    set_short_id!,
    clear_short_id!,
    set_local_id!,
    name_object,
    set_name_object!,
    clear_name_object!
export DclHeader,
    DclHeaderRef,
    ship_id,
    set_ship_id!,
    data_channel_list_id,
    set_data_channel_list_id!,
    version_information,
    set_version_information!,
    clear_version_information!,
    author,
    set_author!,
    clear_author!,
    date_created,
    set_date_created!,
    clear_date_created!,
    custom_headers,
    set_custom_headers!,
    clear_custom_headers!
export DclDataChannel,
    DclDataChannelRef, channel_id, set_channel_id!, property, set_property!
export DclDataChannelList,
    DclDataChannelListRef, add!, remove!, from_short_id, from_local_id
export DclPackage,
    DclPackageRef, header, set_header!, data_channel_list, set_data_channel_list!
export DclListPackage, set_package!, package, dcl_from_json, dcl_to_json
export TsdChannelId, TsdChannelIdRef, is_local_id, is_short_id
export TsdTimeSpan, TsdTimeSpanRef, start_time, end_time, set_start!, set_end!
export TsdConfigRef, TsdConfigRefRef, config_id, set_config_id!, time_stamp, set_time_stamp!
export TsdHeader,
    ship_id,
    set_ship_id!,
    time_span,
    set_time_span!,
    clear_time_span!,
    date_created,
    set_date_created!,
    clear_date_created!,
    date_modified,
    set_date_modified!,
    clear_date_modified!,
    author,
    set_author!,
    clear_author!,
    system_configuration_count,
    system_configuration_at,
    set_system_configuration!,
    clear_system_configuration!,
    custom_headers,
    set_custom_headers!,
    clear_custom_headers!
export TabularDataSet,
    TabularDataSetRef, values, set_values!, quality, set_quality!, clear_quality!
export TabularData,
    TabularDataRef, channel_id_count, channel_id_at, data_set_count, data_set_at
export EventDataSet, EventDataSetRef, channel_id, value, set_quality!, clear_quality!
export EventData,
    EventDataRef, data_set_count, data_set_at, set_data_sets!, clear_data_sets!
export TimeSeriesData,
    TimeSeriesDataRef,
    data_configuration,
    set_data_configuration!,
    clear_data_configuration!,
    tabular_data_count,
    tabular_data_at,
    set_tabular_data!,
    clear_tabular_data!,
    event_data,
    set_event_data!,
    clear_event_data!,
    custom_data_kinds,
    set_custom_data_kinds!,
    clear_custom_data_kinds!,
    validate
export ValidationResult, is_valid, errors
export TsdPackage,
    has_header,
    set_header!,
    time_series_data_count,
    time_series_data_is_empty,
    time_series_data_at
export TimeSeriesDataPackage, is_empty
export tsd_from_json, tsd_to_json
export tsd_dto_from_json, tsd_dto_to_json, tsd_to_dto, tsd_to_domain
export TsdDtoPackage, pkg
export TsdDtoPkgRef,
    header, ensure_header!, clear_header!, tsd_count, tsd_at, tsd_push!, tsd_remove!
export TsdDtoHeaderRef,
    ship_id,
    set_ship_id!,
    time_span,
    ensure_time_span!,
    clear_time_span!,
    date_created,
    set_date_created!,
    clear_date_created!,
    date_modified,
    set_date_modified!,
    clear_date_modified!,
    author,
    set_author!,
    clear_author!,
    system_cfg_count,
    system_cfg_at,
    system_cfg_push!,
    system_cfg_remove!,
    custom_headers,
    ensure_custom_headers!,
    clear_custom_headers!
export TsdDtoTimeSpanRef, start_time, set_start_time!, end_time, set_end_time!
export TsdDtoCfgRefRef, cfg_id, set_cfg_id!, timestamp, set_timestamp!
export TsdDtoTsdRef,
    data_cfg,
    ensure_data_cfg!,
    clear_data_cfg!,
    tabular_count,
    tabular_at,
    tabular_push!,
    tabular_remove!,
    event,
    ensure_event!,
    clear_event!,
    custom_data_kinds,
    ensure_custom_data_kinds!,
    clear_custom_data_kinds!
export TsdDtoTabularRef,
    number_of_data_set,
    set_number_of_data_set!,
    clear_number_of_data_set!,
    number_of_data_channel,
    set_number_of_data_channel!,
    clear_number_of_data_channel!,
    channel_ids,
    set_channel_ids!,
    clear_channel_ids!,
    data_set_count,
    data_set_at,
    data_set_push!,
    data_set_remove!
export TsdDtoTabSetRef,
    timestamp, set_timestamp!, values, set_values!, quality, set_quality!, clear_quality!
export TsdDtoEventRef,
    number_of_data_set,
    set_number_of_data_set!,
    clear_number_of_data_set!,
    data_set_count,
    data_set_at,
    data_set_push!,
    data_set_remove!
export TsdDtoEventSetRef,
    timestamp,
    set_timestamp!,
    channel_id,
    set_channel_id!,
    value,
    set_value!,
    quality,
    set_quality!,
    clear_quality!
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
export dcl_dto_from_json, dcl_dto_to_json, dcl_to_dto, dcl_to_domain
export DclDtoPackage,
    DclDtoPkgRef,
    DclDtoHeaderRef,
    DclDtoCfgRefRef,
    DclDtoVersionInfoRef,
    DclDtoChannelListRef,
    DclDtoChannelRef,
    DclDtoChannelIdRef,
    DclDtoNameObjectRef,
    DclDtoPropertyRef,
    DclDtoChannelTypeRef,
    DclDtoFormatRef,
    DclDtoRestrictionRef,
    DclDtoRangeRef,
    DclDtoUnitRef,
    pkg,
    channel_list,
    cfg_ref,
    version_info,
    ensure_version_info!,
    clear_version_info!,
    naming_rule,
    set_naming_rule!,
    naming_scheme_version,
    set_naming_scheme_version!,
    reference_url,
    set_reference_url!,
    clear_reference_url!,
    cfg_id,
    set_cfg_id!,
    ship_id,
    set_ship_id!,
    author,
    set_author!,
    clear_author!,
    date_created,
    set_date_created!,
    clear_date_created!,
    channel_id,
    name_object,
    ensure_name_object!,
    clear_name_object!,
    format_type,
    set_format_type!,
    restriction,
    ensure_restriction!,
    clear_restriction!,
    channel_type,
    set_channel_type!,
    update_cycle,
    set_update_cycle!,
    clear_update_cycle!,
    calculation_period,
    set_calculation_period!,
    clear_calculation_period!,
    dcl_range,
    ensure_dcl_range!,
    clear_dcl_range!,
    unit,
    ensure_unit!,
    clear_unit!,
    quality_coding,
    set_quality_coding!,
    clear_quality_coding!,
    alert_priority,
    set_alert_priority!,
    clear_alert_priority!,
    remarks,
    set_remarks!,
    clear_remarks!,
    set_low!,
    set_high!,
    symbol,
    set_symbol!,
    quantity_name,
    set_quantity_name!,
    clear_quantity_name!

end
