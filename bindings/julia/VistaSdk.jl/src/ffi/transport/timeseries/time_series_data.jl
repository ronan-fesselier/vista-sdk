# TimeSpan
function ffi_tsd_time_span_create(start::_FfiDateTimeOffset, end_::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_tsd_time_span_create, VISTA_LIB),
        Ptr{Cvoid},
        (_FfiDateTimeOffset, _FfiDateTimeOffset),
        start,
        end_,
    )
end

function ffi_tsd_time_span_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_time_span_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_time_span_start(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_time_span_start, VISTA_LIB),
        _FfiDateTimeOffset,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_time_span_end(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_time_span_end, VISTA_LIB),
        _FfiDateTimeOffset,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_time_span_set_start(ptr::Ptr{Cvoid}, start::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_tsd_time_span_set_start, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, _FfiDateTimeOffset),
        ptr,
        start,
    )
end

function ffi_tsd_time_span_set_end(ptr::Ptr{Cvoid}, end_::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_tsd_time_span_set_end, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, _FfiDateTimeOffset),
        ptr,
        end_,
    )
end

# ConfigurationReference
function ffi_tsd_config_ref_create(id::Cstring, ts::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_tsd_config_ref_create, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring, _FfiDateTimeOffset),
        id,
        ts,
    )
end

function ffi_tsd_config_ref_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_config_ref_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_config_ref_id(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_config_ref_id, VISTA_LIB), Cstring, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_config_ref_set_id(ptr::Ptr{Cvoid}, id::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_config_ref_set_id, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        id,
    )
end

function ffi_tsd_config_ref_timestamp(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_config_ref_timestamp, VISTA_LIB),
        _FfiDateTimeOffset,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_config_ref_set_timestamp(ptr::Ptr{Cvoid}, ts::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_tsd_config_ref_set_timestamp, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, _FfiDateTimeOffset),
        ptr,
        ts,
    )
end

# Header
function ffi_tsd_header_create(ship_id_ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_header_create, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ship_id_ptr,
    )
end

function ffi_tsd_header_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_header_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_header_ship_id(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_header_ship_id, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_header_set_ship_id(ptr::Ptr{Cvoid}, ship_id_ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_header_set_ship_id, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        ptr,
        ship_id_ptr,
    )
end

function ffi_tsd_header_time_span(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_header_time_span, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_header_set_time_span(ptr::Ptr{Cvoid}, ts_ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_header_set_time_span, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        ptr,
        ts_ptr,
    )
end

function ffi_tsd_header_clear_time_span(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_header_clear_time_span, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_header_has_date_created(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_header_has_date_created, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_header_date_created(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_header_date_created, VISTA_LIB),
        _FfiDateTimeOffset,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_header_set_date_created(ptr::Ptr{Cvoid}, ts::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_tsd_header_set_date_created, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, _FfiDateTimeOffset),
        ptr,
        ts,
    )
end

function ffi_tsd_header_clear_date_created(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_header_clear_date_created, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_header_has_date_modified(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_header_has_date_modified, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_header_date_modified(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_header_date_modified, VISTA_LIB),
        _FfiDateTimeOffset,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_header_set_date_modified(ptr::Ptr{Cvoid}, ts::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_tsd_header_set_date_modified, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, _FfiDateTimeOffset),
        ptr,
        ts,
    )
end

function ffi_tsd_header_clear_date_modified(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_header_clear_date_modified, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_header_author(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_header_author, VISTA_LIB), Cstring, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_header_set_author(ptr::Ptr{Cvoid}, author::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_header_set_author, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        author,
    )
end

function ffi_tsd_header_clear_author(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_header_clear_author, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_header_system_configuration_count(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_header_system_configuration_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_header_system_configuration_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_header_system_configuration_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_header_set_system_configuration(
    ptr::Ptr{Cvoid},
    entries::Ptr{Ptr{Cvoid}},
    count::Csize_t,
)
    ccall(
        (:dnv_vista_sdk_tsd_header_set_system_configuration, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Ptr{Cvoid}}, Csize_t),
        ptr,
        entries,
        count,
    )
end

function ffi_tsd_header_clear_system_configuration(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_header_clear_system_configuration, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_header_custom_headers(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_header_custom_headers, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_header_set_custom_headers(ptr::Ptr{Cvoid}, doc_ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_header_set_custom_headers, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        ptr,
        doc_ptr,
    )
end

function ffi_tsd_header_clear_custom_headers(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_header_clear_custom_headers, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

# TabularDataSet
function ffi_tsd_tabular_data_set_create(
    ts::_FfiDateTimeOffset,
    values::Ptr{Cstring},
    count::Csize_t,
)
    ccall(
        (:dnv_vista_sdk_tsd_tabular_data_set_create, VISTA_LIB),
        Ptr{Cvoid},
        (_FfiDateTimeOffset, Ptr{Cstring}, Csize_t),
        ts,
        values,
        count,
    )
end

function ffi_tsd_tabular_data_set_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_tabular_data_set_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_tabular_data_set_time_stamp(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_tabular_data_set_time_stamp, VISTA_LIB),
        _FfiDateTimeOffset,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_tabular_data_set_value_count(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_tabular_data_set_value_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_tabular_data_set_value_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_tabular_data_set_value_at, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_tabular_data_set_set_values(
    ptr::Ptr{Cvoid},
    values::Ptr{Cstring},
    count::Csize_t,
)
    ccall(
        (:dnv_vista_sdk_tsd_tabular_data_set_set_values, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cstring}, Csize_t),
        ptr,
        values,
        count,
    )
end

function ffi_tsd_tabular_data_set_quality_count(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_tabular_data_set_quality_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_tabular_data_set_quality_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_tabular_data_set_quality_at, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_tabular_data_set_set_quality(
    ptr::Ptr{Cvoid},
    quality::Ptr{Cstring},
    count::Csize_t,
)
    ccall(
        (:dnv_vista_sdk_tsd_tabular_data_set_set_quality, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cstring}, Csize_t),
        ptr,
        quality,
        count,
    )
end

function ffi_tsd_tabular_data_set_clear_quality(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_tabular_data_set_clear_quality, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

# TabularData
function ffi_tsd_tabular_data_create(
    channel_ids::Ptr{Ptr{Cvoid}},
    id_count::Csize_t,
    data_sets::Ptr{Ptr{Cvoid}},
    ds_count::Csize_t,
)
    ccall(
        (:dnv_vista_sdk_tsd_tabular_data_create, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Ptr{Cvoid}}, Csize_t, Ptr{Ptr{Cvoid}}, Csize_t),
        channel_ids,
        id_count,
        data_sets,
        ds_count,
    )
end

function ffi_tsd_tabular_data_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_tabular_data_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_tabular_data_channel_id_count(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_tabular_data_channel_id_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_tabular_data_channel_id_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_tabular_data_channel_id_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_tabular_data_data_set_count(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_tabular_data_data_set_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_tabular_data_data_set_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_tabular_data_data_set_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_tabular_data_validate(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_tabular_data_validate, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

# EventDataSet
function ffi_tsd_event_data_set_create(
    ts::_FfiDateTimeOffset,
    channel_id_ptr::Ptr{Cvoid},
    value::Cstring,
)
    ccall(
        (:dnv_vista_sdk_tsd_event_data_set_create, VISTA_LIB),
        Ptr{Cvoid},
        (_FfiDateTimeOffset, Ptr{Cvoid}, Cstring),
        ts,
        channel_id_ptr,
        value,
    )
end

function ffi_tsd_event_data_set_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_event_data_set_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_event_data_set_time_stamp(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_event_data_set_time_stamp, VISTA_LIB),
        _FfiDateTimeOffset,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_event_data_set_data_channel_id(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_event_data_set_data_channel_id, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_event_data_set_value(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_event_data_set_value, VISTA_LIB), Cstring, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_event_data_set_quality(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_event_data_set_quality, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_event_data_set_set_quality(ptr::Ptr{Cvoid}, quality::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_event_data_set_set_quality, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        quality,
    )
end

function ffi_tsd_event_data_set_clear_quality(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_event_data_set_clear_quality, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

# EventData
function ffi_tsd_event_data_create()
    ccall((:dnv_vista_sdk_tsd_event_data_create, VISTA_LIB), Ptr{Cvoid}, ())
end

function ffi_tsd_event_data_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_event_data_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_event_data_data_set_count(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_event_data_data_set_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_event_data_data_set_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_event_data_data_set_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_event_data_set_data_set(
    ptr::Ptr{Cvoid},
    data_sets::Ptr{Ptr{Cvoid}},
    count::Csize_t,
)
    ccall(
        (:dnv_vista_sdk_tsd_event_data_set_data_set, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Ptr{Cvoid}}, Csize_t),
        ptr,
        data_sets,
        count,
    )
end

function ffi_tsd_event_data_clear_data_set(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_event_data_clear_data_set, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

# TimeSeriesData
function ffi_tsd_time_series_data_create()
    ccall((:dnv_vista_sdk_tsd_time_series_data_create, VISTA_LIB), Ptr{Cvoid}, ())
end

function ffi_tsd_time_series_data_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_time_series_data_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_time_series_data_data_configuration(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_time_series_data_data_configuration, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_time_series_data_set_data_configuration(
    ptr::Ptr{Cvoid},
    cfg_ptr::Ptr{Cvoid},
)
    ccall(
        (:dnv_vista_sdk_tsd_time_series_data_set_data_configuration, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        ptr,
        cfg_ptr,
    )
end

function ffi_tsd_time_series_data_clear_data_configuration(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_time_series_data_clear_data_configuration, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_time_series_data_tabular_data_count(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_time_series_data_tabular_data_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_time_series_data_tabular_data_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_time_series_data_tabular_data_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_time_series_data_set_tabular_data(
    ptr::Ptr{Cvoid},
    entries::Ptr{Ptr{Cvoid}},
    count::Csize_t,
)
    ccall(
        (:dnv_vista_sdk_tsd_time_series_data_set_tabular_data, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Ptr{Cvoid}}, Csize_t),
        ptr,
        entries,
        count,
    )
end

function ffi_tsd_time_series_data_clear_tabular_data(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_time_series_data_clear_tabular_data, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_time_series_data_event_data(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_time_series_data_event_data, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_time_series_data_set_event_data(ptr::Ptr{Cvoid}, ed_ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_time_series_data_set_event_data, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        ptr,
        ed_ptr,
    )
end

function ffi_tsd_time_series_data_clear_event_data(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_time_series_data_clear_event_data, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_time_series_data_custom_data_kinds(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_time_series_data_custom_data_kinds, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_time_series_data_set_custom_data_kinds(
    ptr::Ptr{Cvoid},
    doc_ptr::Ptr{Cvoid},
)
    ccall(
        (:dnv_vista_sdk_tsd_time_series_data_set_custom_data_kinds, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        ptr,
        doc_ptr,
    )
end

function ffi_tsd_time_series_data_clear_custom_data_kinds(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_time_series_data_clear_custom_data_kinds, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_validation_errors_add(errors_ptr::Ptr{Cvoid}, message::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_validation_errors_add, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        errors_ptr,
        message,
    )
end

function ffi_tsd_time_series_data_validate(
    ptr::Ptr{Cvoid},
    dc_package_ptr::Ptr{Cvoid},
    on_tabular::Ptr{Cvoid},
    tabular_userdata::Ptr{Cvoid},
    on_event::Ptr{Cvoid},
    event_userdata::Ptr{Cvoid},
)
    ccall(
        (:dnv_vista_sdk_tsd_time_series_data_validate, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cvoid}, Ptr{Cvoid}, Ptr{Cvoid}, Ptr{Cvoid}, Ptr{Cvoid}),
        ptr,
        dc_package_ptr,
        on_tabular,
        tabular_userdata,
        on_event,
        event_userdata,
    )
end

# Package
function ffi_tsd_package_create(
    header_ptr::Ptr{Cvoid},
    entries::Ptr{Ptr{Cvoid}},
    count::Csize_t,
)
    ccall(
        (:dnv_vista_sdk_tsd_package_create, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Ptr{Cvoid}}, Csize_t),
        header_ptr,
        entries,
        count,
    )
end

function ffi_tsd_package_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_package_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_package_header(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_package_header, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_package_set_header(ptr::Ptr{Cvoid}, header_ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_package_set_header, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        ptr,
        header_ptr,
    )
end

function ffi_tsd_package_time_series_data_count(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_package_time_series_data_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_package_time_series_data_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_package_time_series_data_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

# TimeSeriesDataPackage
function ffi_tsd_data_package_create(package_ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_data_package_create, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        package_ptr,
    )
end

function ffi_tsd_data_package_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_data_package_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_data_package_package(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_data_package_package, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

# JSON (domain level)
function ffi_tsd_data_package_from_json(json::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_data_package_from_json, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring,),
        json,
    )
end

function ffi_tsd_data_package_to_json(ptr::Ptr{Cvoid}, pretty::Cint)
    ccall(
        (:dnv_vista_sdk_tsd_data_package_to_json, VISTA_LIB),
        Ptr{UInt8},
        (Ptr{Cvoid}, Cint),
        ptr,
        pretty,
    )
end
