function ffi_dcl_to_dto(domain_ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_to_dto, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), domain_ptr)
end

function ffi_dcl_to_domain(dto_ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_to_domain, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), dto_ptr)
end

function ffi_dcl_dto_to_json(dto_ptr::Ptr{Cvoid}, pretty_print::Cint)
    ccall(
        (:dnv_vista_sdk_dcl_dto_to_json, VISTA_LIB),
        Ptr{UInt8},
        (Ptr{Cvoid}, Cint),
        dto_ptr,
        pretty_print,
    )
end

function ffi_dcl_dto_from_json(json::Cstring)
    ccall((:dnv_vista_sdk_dcl_dto_from_json, VISTA_LIB), Ptr{Cvoid}, (Cstring,), json)
end

function ffi_dcl_dto_package_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_dto_package_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_dcl_dto_package_get_pkg(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_package_get_pkg, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_pkg_get_header(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_pkg_get_header, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_pkg_get_channel_list(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_pkg_get_channel_list, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_header_get_ship_id(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_header_get_ship_id, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_header_set_ship_id(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_header_set_ship_id, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_header_get_cfg_ref(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_header_get_cfg_ref, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_header_has_ver_info(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_dto_header_has_ver_info, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_dcl_dto_header_get_ver_info(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_header_get_ver_info, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_header_ensure_ver_info(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_header_ensure_ver_info, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_header_clear_ver_info(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_header_clear_ver_info, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_header_has_author(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_dto_header_has_author, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_dcl_dto_header_get_author(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_header_get_author, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_header_set_author(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_header_set_author, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_header_clear_author(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_header_clear_author, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_header_has_date_created(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_header_has_date_created, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_header_get_date_created(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_header_get_date_created, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_header_set_date_created(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_header_set_date_created, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_header_clear_date_created(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_header_clear_date_created, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_header_has_custom_headers(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_header_has_custom_headers, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_header_get_custom_headers(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_header_get_custom_headers, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_header_ensure_custom_headers(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_header_ensure_custom_headers, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_header_clear_custom_headers(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_header_clear_custom_headers, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_cfg_ref_get_id(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_dto_cfg_ref_get_id, VISTA_LIB), Cstring, (Ptr{Cvoid},), ptr)
end

function ffi_dcl_dto_cfg_ref_set_id(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_cfg_ref_set_id, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_cfg_ref_get_timestamp(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_cfg_ref_get_timestamp, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_cfg_ref_set_timestamp(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_cfg_ref_set_timestamp, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_cfg_ref_has_version(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_dto_cfg_ref_has_version, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_dcl_dto_cfg_ref_get_version(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_cfg_ref_get_version, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_cfg_ref_set_version(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_cfg_ref_set_version, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_cfg_ref_clear_version(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_cfg_ref_clear_version, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ver_info_get_naming_rule(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_ver_info_get_naming_rule, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ver_info_set_naming_rule(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_ver_info_set_naming_rule, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_ver_info_get_naming_scheme_version(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_ver_info_get_naming_scheme_version, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ver_info_set_naming_scheme_version(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_ver_info_set_naming_scheme_version, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_ver_info_has_reference_url(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_ver_info_has_reference_url, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ver_info_get_reference_url(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_ver_info_get_reference_url, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ver_info_set_reference_url(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_ver_info_set_reference_url, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_ver_info_clear_reference_url(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_ver_info_clear_reference_url, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ch_list_count(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_dto_ch_list_count, VISTA_LIB), Csize_t, (Ptr{Cvoid},), ptr)
end

function ffi_dcl_dto_ch_list_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_list_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_dcl_dto_ch_list_push(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_dto_ch_list_push, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), ptr)
end

function ffi_dcl_dto_ch_list_remove(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_list_remove, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_dcl_dto_channel_get_id(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_channel_get_id, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_channel_get_property(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_channel_get_property, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ch_id_get_local_id(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_id_get_local_id, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ch_id_set_local_id(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_id_set_local_id, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_ch_id_has_short_id(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_dto_ch_id_has_short_id, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_dcl_dto_ch_id_get_short_id(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_id_get_short_id, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ch_id_set_short_id(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_id_set_short_id, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_ch_id_clear_short_id(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_id_clear_short_id, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ch_id_has_name_object(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_id_has_name_object, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ch_id_get_name_object(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_id_get_name_object, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ch_id_ensure_name_object(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_id_ensure_name_object, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ch_id_clear_name_object(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_id_clear_name_object, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_name_obj_get_naming_rule(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_name_obj_get_naming_rule, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_name_obj_set_naming_rule(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_name_obj_set_naming_rule, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_name_obj_has_custom(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_dto_name_obj_has_custom, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_dcl_dto_name_obj_get_custom(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_name_obj_get_custom, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_name_obj_ensure_custom(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_name_obj_ensure_custom, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_name_obj_clear_custom(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_name_obj_clear_custom, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_get_ch_type(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_get_ch_type, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_get_format(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_get_format, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_has_range(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_dto_property_has_range, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_dcl_dto_property_get_range(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_get_range, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_ensure_range(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_ensure_range, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_clear_range(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_clear_range, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_has_unit(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_dto_property_has_unit, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_dcl_dto_property_get_unit(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_get_unit, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_ensure_unit(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_ensure_unit, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_clear_unit(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_clear_unit, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_has_quality_coding(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_has_quality_coding, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_get_quality_coding(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_get_quality_coding, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_set_quality_coding(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_set_quality_coding, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_property_clear_quality_coding(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_clear_quality_coding, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_has_alert_priority(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_has_alert_priority, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_get_alert_priority(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_get_alert_priority, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_set_alert_priority(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_set_alert_priority, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_property_clear_alert_priority(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_clear_alert_priority, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_has_name(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_dto_property_has_name, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_dcl_dto_property_get_name(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_get_name, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_set_name(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_set_name, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_property_clear_name(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_clear_name, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_has_remarks(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_has_remarks, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_get_remarks(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_get_remarks, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_set_remarks(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_set_remarks, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_property_clear_remarks(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_clear_remarks, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_has_custom_properties(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_has_custom_properties, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_get_custom_properties(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_get_custom_properties, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_ensure_custom_properties(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_ensure_custom_properties, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_property_clear_custom_properties(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_property_clear_custom_properties, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ch_type_get_type(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_dto_ch_type_get_type, VISTA_LIB), Cstring, (Ptr{Cvoid},), ptr)
end

function ffi_dcl_dto_ch_type_set_type(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_type_set_type, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_ch_type_has_update_cycle(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_type_has_update_cycle, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ch_type_get_update_cycle(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_type_get_update_cycle, VISTA_LIB),
        Cdouble,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ch_type_set_update_cycle(ptr::Ptr{Cvoid}, v::Cdouble)
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_type_set_update_cycle, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cdouble),
        ptr,
        v,
    )
end

function ffi_dcl_dto_ch_type_clear_update_cycle(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_type_clear_update_cycle, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ch_type_has_calculation_period(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_type_has_calculation_period, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ch_type_get_calculation_period(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_type_get_calculation_period, VISTA_LIB),
        Cdouble,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_ch_type_set_calculation_period(ptr::Ptr{Cvoid}, v::Cdouble)
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_type_set_calculation_period, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cdouble),
        ptr,
        v,
    )
end

function ffi_dcl_dto_ch_type_clear_calculation_period(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_ch_type_clear_calculation_period, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_format_get_type(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_dto_format_get_type, VISTA_LIB), Cstring, (Ptr{Cvoid},), ptr)
end

function ffi_dcl_dto_format_set_type(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_format_set_type, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_format_has_restriction(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_format_has_restriction, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_format_get_restriction(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_format_get_restriction, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_format_ensure_restriction(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_format_ensure_restriction, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_format_clear_restriction(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_format_clear_restriction, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_enumeration_count(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_enumeration_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_enumeration_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_enumeration_at, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_dcl_dto_restriction_set_enumeration(
    ptr::Ptr{Cvoid},
    values::Ptr{Cstring},
    count::Csize_t,
)
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_set_enumeration, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cstring}, Csize_t),
        ptr,
        values,
        count,
    )
end

function ffi_dcl_dto_restriction_clear_enumeration(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_clear_enumeration, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_has_fraction_digits(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_has_fraction_digits, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_get_fraction_digits(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_get_fraction_digits, VISTA_LIB),
        UInt32,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_set_fraction_digits(ptr::Ptr{Cvoid}, v::UInt32)
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_set_fraction_digits, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, UInt32),
        ptr,
        v,
    )
end

function ffi_dcl_dto_restriction_clear_fraction_digits(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_clear_fraction_digits, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_has_length(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_has_length, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_get_length(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_get_length, VISTA_LIB),
        UInt32,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_set_length(ptr::Ptr{Cvoid}, v::UInt32)
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_set_length, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, UInt32),
        ptr,
        v,
    )
end

function ffi_dcl_dto_restriction_clear_length(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_clear_length, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_has_max_exclusive(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_has_max_exclusive, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_get_max_exclusive(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_get_max_exclusive, VISTA_LIB),
        Cdouble,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_set_max_exclusive(ptr::Ptr{Cvoid}, v::Cdouble)
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_set_max_exclusive, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cdouble),
        ptr,
        v,
    )
end

function ffi_dcl_dto_restriction_clear_max_exclusive(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_clear_max_exclusive, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_has_max_inclusive(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_has_max_inclusive, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_get_max_inclusive(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_get_max_inclusive, VISTA_LIB),
        Cdouble,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_set_max_inclusive(ptr::Ptr{Cvoid}, v::Cdouble)
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_set_max_inclusive, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cdouble),
        ptr,
        v,
    )
end

function ffi_dcl_dto_restriction_clear_max_inclusive(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_clear_max_inclusive, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_has_max_length(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_has_max_length, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_get_max_length(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_get_max_length, VISTA_LIB),
        UInt32,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_set_max_length(ptr::Ptr{Cvoid}, v::UInt32)
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_set_max_length, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, UInt32),
        ptr,
        v,
    )
end

function ffi_dcl_dto_restriction_clear_max_length(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_clear_max_length, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_has_min_exclusive(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_has_min_exclusive, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_get_min_exclusive(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_get_min_exclusive, VISTA_LIB),
        Cdouble,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_set_min_exclusive(ptr::Ptr{Cvoid}, v::Cdouble)
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_set_min_exclusive, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cdouble),
        ptr,
        v,
    )
end

function ffi_dcl_dto_restriction_clear_min_exclusive(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_clear_min_exclusive, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_has_min_inclusive(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_has_min_inclusive, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_get_min_inclusive(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_get_min_inclusive, VISTA_LIB),
        Cdouble,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_set_min_inclusive(ptr::Ptr{Cvoid}, v::Cdouble)
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_set_min_inclusive, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cdouble),
        ptr,
        v,
    )
end

function ffi_dcl_dto_restriction_clear_min_inclusive(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_clear_min_inclusive, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_has_min_length(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_has_min_length, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_get_min_length(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_get_min_length, VISTA_LIB),
        UInt32,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_set_min_length(ptr::Ptr{Cvoid}, v::UInt32)
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_set_min_length, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, UInt32),
        ptr,
        v,
    )
end

function ffi_dcl_dto_restriction_clear_min_length(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_clear_min_length, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_has_pattern(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_has_pattern, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_get_pattern(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_get_pattern, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_set_pattern(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_set_pattern, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_restriction_clear_pattern(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_clear_pattern, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_has_total_digits(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_has_total_digits, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_get_total_digits(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_get_total_digits, VISTA_LIB),
        UInt32,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_set_total_digits(ptr::Ptr{Cvoid}, v::UInt32)
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_set_total_digits, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, UInt32),
        ptr,
        v,
    )
end

function ffi_dcl_dto_restriction_clear_total_digits(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_clear_total_digits, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_has_white_space(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_has_white_space, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_get_white_space(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_get_white_space, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_restriction_set_white_space(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_set_white_space, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_restriction_clear_white_space(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_restriction_clear_white_space, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_range_get_low(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_dto_range_get_low, VISTA_LIB), Cdouble, (Ptr{Cvoid},), ptr)
end

function ffi_dcl_dto_range_set_low(ptr::Ptr{Cvoid}, v::Cdouble)
    ccall(
        (:dnv_vista_sdk_dcl_dto_range_set_low, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cdouble),
        ptr,
        v,
    )
end

function ffi_dcl_dto_range_get_high(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_dto_range_get_high, VISTA_LIB), Cdouble, (Ptr{Cvoid},), ptr)
end

function ffi_dcl_dto_range_set_high(ptr::Ptr{Cvoid}, v::Cdouble)
    ccall(
        (:dnv_vista_sdk_dcl_dto_range_set_high, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cdouble),
        ptr,
        v,
    )
end

function ffi_dcl_dto_unit_get_symbol(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_dto_unit_get_symbol, VISTA_LIB), Cstring, (Ptr{Cvoid},), ptr)
end

function ffi_dcl_dto_unit_set_symbol(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_unit_set_symbol, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_unit_has_quantity_name(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_unit_has_quantity_name, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_unit_get_quantity_name(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_unit_get_quantity_name, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_unit_set_quantity_name(ptr::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_dto_unit_set_quantity_name, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        ptr,
        v,
    )
end

function ffi_dcl_dto_unit_clear_quantity_name(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_unit_clear_quantity_name, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_unit_has_custom_elements(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_unit_has_custom_elements, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_unit_get_custom_elements(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_unit_get_custom_elements, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_unit_ensure_custom_elements(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_unit_ensure_custom_elements, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_dcl_dto_unit_clear_custom_elements(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_dto_unit_clear_custom_elements, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end
