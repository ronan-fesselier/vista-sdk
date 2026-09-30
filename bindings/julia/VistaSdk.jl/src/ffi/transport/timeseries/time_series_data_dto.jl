function ffi_tsd_to_dto(domain_ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_to_dto, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), domain_ptr)
end

function ffi_tsd_to_domain(dto_ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_to_domain, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), dto_ptr)
end

function ffi_tsd_dto_to_json(dto_ptr::Ptr{Cvoid}, pretty_print::Cint)
    ccall(
        (:dnv_vista_sdk_tsd_dto_to_json, VISTA_LIB),
        Ptr{UInt8},
        (Ptr{Cvoid}, Cint),
        dto_ptr,
        pretty_print,
    )
end

function ffi_tsd_dto_from_json(json::Cstring)
    ccall((:dnv_vista_sdk_tsd_dto_from_json, VISTA_LIB), Ptr{Cvoid}, (Cstring,), json)
end

function ffi_tsd_dto_package_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_dto_package_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_dto_package_get_pkg(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_package_get_pkg, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_pkg_has_header(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_dto_pkg_has_header, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_dto_pkg_get_header(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_pkg_get_header, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_pkg_ensure_header(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_dto_pkg_ensure_header, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_dto_pkg_clear_header(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_dto_pkg_clear_header, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_dto_pkg_tsd_count(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_dto_pkg_tsd_count, VISTA_LIB), Csize_t, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_dto_pkg_tsd_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_dto_pkg_tsd_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_dto_pkg_tsd_push(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_dto_pkg_tsd_push, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_dto_pkg_tsd_remove(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_dto_pkg_tsd_remove, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_dto_header_get_ship_id(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_get_ship_id, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_header_set_ship_id(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_set_ship_id, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_tsd_dto_header_has_time_span(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_has_time_span, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_header_get_time_span(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_get_time_span, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_header_ensure_time_span(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_ensure_time_span, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_header_clear_time_span(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_clear_time_span, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_header_has_date_created(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_has_date_created, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_header_get_date_created(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_get_date_created, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_header_set_date_created(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_set_date_created, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_tsd_dto_header_clear_date_created(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_clear_date_created, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_header_has_date_modified(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_has_date_modified, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_header_get_date_modified(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_get_date_modified, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_header_set_date_modified(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_set_date_modified, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_tsd_dto_header_clear_date_modified(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_clear_date_modified, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_header_has_author(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_dto_header_has_author, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_dto_header_get_author(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_get_author, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_header_set_author(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_set_author, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_tsd_dto_header_clear_author(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_clear_author, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_header_system_cfg_count(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_system_cfg_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_header_system_cfg_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_system_cfg_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_dto_header_system_cfg_push(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_system_cfg_push, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_header_system_cfg_remove(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_system_cfg_remove, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_dto_header_has_custom_headers(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_has_custom_headers, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_header_get_custom_headers(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_get_custom_headers, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_header_ensure_custom_headers(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_ensure_custom_headers, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_header_clear_custom_headers(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_header_clear_custom_headers, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_time_span_get_start(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_time_span_get_start, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_time_span_set_start(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_dto_time_span_set_start, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_tsd_dto_time_span_get_end(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_time_span_get_end, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_time_span_set_end(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_dto_time_span_set_end, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_tsd_dto_cfg_ref_get_id(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_dto_cfg_ref_get_id, VISTA_LIB), Cstring, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_dto_cfg_ref_set_id(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_dto_cfg_ref_set_id, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_tsd_dto_cfg_ref_get_timestamp(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_cfg_ref_get_timestamp, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_cfg_ref_set_timestamp(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_dto_cfg_ref_set_timestamp, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_tsd_dto_tsd_has_data_cfg(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_dto_tsd_has_data_cfg, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_dto_tsd_get_data_cfg(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tsd_get_data_cfg, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tsd_ensure_data_cfg(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tsd_ensure_data_cfg, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tsd_clear_data_cfg(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_dto_tsd_clear_data_cfg, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_dto_tsd_tabular_count(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tsd_tabular_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tsd_tabular_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_dto_tsd_tabular_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_dto_tsd_tabular_push(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tsd_tabular_push, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tsd_tabular_remove(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_dto_tsd_tabular_remove, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_dto_tsd_has_event(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_dto_tsd_has_event, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_dto_tsd_get_event(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_dto_tsd_get_event, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_dto_tsd_ensure_event(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_dto_tsd_ensure_event, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_dto_tsd_clear_event(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_dto_tsd_clear_event, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_dto_tsd_has_custom_data_kinds(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tsd_has_custom_data_kinds, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tsd_get_custom_data_kinds(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tsd_get_custom_data_kinds, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tsd_ensure_custom_data_kinds(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tsd_ensure_custom_data_kinds, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tsd_clear_custom_data_kinds(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tsd_clear_custom_data_kinds, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tabular_has_number_of_data_set(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tabular_has_number_of_data_set, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tabular_get_number_of_data_set(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tabular_get_number_of_data_set, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tabular_set_number_of_data_set(ptr::Ptr{Cvoid}, v::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_dto_tabular_set_number_of_data_set, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Csize_t),
        ptr,
        v,
    )
end

function ffi_tsd_dto_tabular_clear_number_of_data_set(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tabular_clear_number_of_data_set, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tabular_has_number_of_data_channel(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tabular_has_number_of_data_channel, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tabular_get_number_of_data_channel(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tabular_get_number_of_data_channel, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tabular_set_number_of_data_channel(ptr::Ptr{Cvoid}, v::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_dto_tabular_set_number_of_data_channel, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Csize_t),
        ptr,
        v,
    )
end

function ffi_tsd_dto_tabular_clear_number_of_data_channel(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tabular_clear_number_of_data_channel, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tabular_channel_id_count(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tabular_channel_id_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tabular_channel_id_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_dto_tabular_channel_id_at, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_dto_tabular_set_channel_ids(
    ptr::Ptr{Cvoid},
    ids::Ptr{Cstring},
    count::Csize_t,
)
    ccall(
        (:dnv_vista_sdk_tsd_dto_tabular_set_channel_ids, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cstring}, Csize_t),
        ptr,
        ids,
        count,
    )
end

function ffi_tsd_dto_tabular_clear_channel_ids(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tabular_clear_channel_ids, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tabular_data_set_count(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tabular_data_set_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tabular_data_set_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_dto_tabular_data_set_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_dto_tabular_data_set_push(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tabular_data_set_push, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tabular_data_set_remove(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_dto_tabular_data_set_remove, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_dto_tab_set_get_timestamp(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tab_set_get_timestamp, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tab_set_set_timestamp(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_dto_tab_set_set_timestamp, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_tsd_dto_tab_set_value_count(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tab_set_value_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tab_set_value_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_dto_tab_set_value_at, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_dto_tab_set_set_values(ptr::Ptr{Cvoid}, vals::Ptr{Cstring}, count::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_dto_tab_set_set_values, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cstring}, Csize_t),
        ptr,
        vals,
        count,
    )
end

function ffi_tsd_dto_tab_set_quality_count(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tab_set_quality_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_tab_set_quality_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_dto_tab_set_quality_at, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_dto_tab_set_set_quality(ptr::Ptr{Cvoid}, q::Ptr{Cstring}, count::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_dto_tab_set_set_quality, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cstring}, Csize_t),
        ptr,
        q,
        count,
    )
end

function ffi_tsd_dto_tab_set_clear_quality(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_tab_set_clear_quality, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_event_has_number_of_data_set(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_event_has_number_of_data_set, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_event_get_number_of_data_set(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_event_get_number_of_data_set, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_event_set_number_of_data_set(ptr::Ptr{Cvoid}, v::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_dto_event_set_number_of_data_set, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Csize_t),
        ptr,
        v,
    )
end

function ffi_tsd_dto_event_clear_number_of_data_set(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_event_clear_number_of_data_set, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_event_data_set_count(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_event_data_set_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_event_data_set_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_dto_event_data_set_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_dto_event_data_set_push(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_event_data_set_push, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_event_data_set_remove(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_tsd_dto_event_data_set_remove, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_tsd_dto_event_set_get_timestamp(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_event_set_get_timestamp, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_event_set_set_timestamp(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_dto_event_set_set_timestamp, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_tsd_dto_event_set_get_data_channel_id(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_event_set_get_data_channel_id, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_event_set_set_data_channel_id(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_dto_event_set_set_data_channel_id, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_tsd_dto_event_set_get_value(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_event_set_get_value, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_event_set_set_value(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_dto_event_set_set_value, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_tsd_dto_event_set_has_quality(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_event_set_has_quality, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_event_set_get_quality(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_event_set_get_quality, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_dto_event_set_set_quality(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_dto_event_set_set_quality, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_tsd_dto_event_set_clear_quality(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_dto_event_set_clear_quality, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end
