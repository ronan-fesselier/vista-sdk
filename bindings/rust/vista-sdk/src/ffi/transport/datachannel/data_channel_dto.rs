use crate::ffi::transport::datachannel::data_channel::dnv_vista_sdk_dcl_list_package_t;
use crate::ffi::transport::serializable_document::dnv_vista_sdk_serializable_document_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_dto_package {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_dto_package_t = dnv_vista_sdk_dcl_dto_package;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_dto_pkg {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_dto_pkg_t = dnv_vista_sdk_dcl_dto_pkg;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_dto_header {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_dto_header_t = dnv_vista_sdk_dcl_dto_header;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_dto_cfg_ref {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_dto_cfg_ref_t = dnv_vista_sdk_dcl_dto_cfg_ref;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_dto_ver_info {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_dto_ver_info_t = dnv_vista_sdk_dcl_dto_ver_info;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_dto_ch_list {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_dto_ch_list_t = dnv_vista_sdk_dcl_dto_ch_list;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_dto_channel {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_dto_channel_t = dnv_vista_sdk_dcl_dto_channel;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_dto_ch_id {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_dto_ch_id_t = dnv_vista_sdk_dcl_dto_ch_id;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_dto_name_obj {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_dto_name_obj_t = dnv_vista_sdk_dcl_dto_name_obj;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_dto_property {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_dto_property_t = dnv_vista_sdk_dcl_dto_property;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_dto_ch_type {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_dto_ch_type_t = dnv_vista_sdk_dcl_dto_ch_type;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_dto_format {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_dto_format_t = dnv_vista_sdk_dcl_dto_format;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_dto_restriction {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_dto_restriction_t = dnv_vista_sdk_dcl_dto_restriction;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_dto_range {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_dto_range_t = dnv_vista_sdk_dcl_dto_range;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_dto_unit {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_dto_unit_t = dnv_vista_sdk_dcl_dto_unit;

extern "C" {
    // Root
    pub(crate) fn dnv_vista_sdk_dcl_to_dto(
        domain: *const dnv_vista_sdk_dcl_list_package_t,
    ) -> *mut dnv_vista_sdk_dcl_dto_package_t;

    pub(crate) fn dnv_vista_sdk_dcl_to_domain(
        dto: *const dnv_vista_sdk_dcl_dto_package_t,
    ) -> *mut dnv_vista_sdk_dcl_list_package_t;

    pub(crate) fn dnv_vista_sdk_dcl_dto_to_json(
        dto: *const dnv_vista_sdk_dcl_dto_package_t,
        pretty_print: std::ffi::c_int,
    ) -> *mut std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_from_json(
        json: *const std::ffi::c_char,
    ) -> *mut dnv_vista_sdk_dcl_dto_package_t;

    pub(crate) fn dnv_vista_sdk_dcl_dto_package_free(p: *mut dnv_vista_sdk_dcl_dto_package_t);

    pub(crate) fn dnv_vista_sdk_dcl_dto_package_get_pkg(
        p: *mut dnv_vista_sdk_dcl_dto_package_t,
    ) -> *mut dnv_vista_sdk_dcl_dto_pkg_t;

    // PackageDto
    pub(crate) fn dnv_vista_sdk_dcl_dto_pkg_get_header(
        pkg: *mut dnv_vista_sdk_dcl_dto_pkg_t,
    ) -> *mut dnv_vista_sdk_dcl_dto_header_t;

    pub(crate) fn dnv_vista_sdk_dcl_dto_pkg_get_channel_list(
        pkg: *mut dnv_vista_sdk_dcl_dto_pkg_t,
    ) -> *mut dnv_vista_sdk_dcl_dto_ch_list_t;

    // HeaderDto
    pub(crate) fn dnv_vista_sdk_dcl_dto_header_get_ship_id(
        h: *const dnv_vista_sdk_dcl_dto_header_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_header_set_ship_id(
        h: *mut dnv_vista_sdk_dcl_dto_header_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_header_get_cfg_ref(
        h: *mut dnv_vista_sdk_dcl_dto_header_t,
    ) -> *mut dnv_vista_sdk_dcl_dto_cfg_ref_t;

    pub(crate) fn dnv_vista_sdk_dcl_dto_header_has_ver_info(
        h: *const dnv_vista_sdk_dcl_dto_header_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_header_get_ver_info(
        h: *mut dnv_vista_sdk_dcl_dto_header_t,
    ) -> *mut dnv_vista_sdk_dcl_dto_ver_info_t;

    pub(crate) fn dnv_vista_sdk_dcl_dto_header_ensure_ver_info(
        h: *mut dnv_vista_sdk_dcl_dto_header_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_header_clear_ver_info(
        h: *mut dnv_vista_sdk_dcl_dto_header_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_header_has_author(
        h: *const dnv_vista_sdk_dcl_dto_header_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_header_get_author(
        h: *const dnv_vista_sdk_dcl_dto_header_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_header_set_author(
        h: *mut dnv_vista_sdk_dcl_dto_header_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_header_clear_author(h: *mut dnv_vista_sdk_dcl_dto_header_t);

    pub(crate) fn dnv_vista_sdk_dcl_dto_header_has_date_created(
        h: *const dnv_vista_sdk_dcl_dto_header_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_header_get_date_created(
        h: *const dnv_vista_sdk_dcl_dto_header_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_header_set_date_created(
        h: *mut dnv_vista_sdk_dcl_dto_header_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_header_clear_date_created(
        h: *mut dnv_vista_sdk_dcl_dto_header_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_header_has_custom_headers(
        h: *const dnv_vista_sdk_dcl_dto_header_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_header_get_custom_headers(
        h: *mut dnv_vista_sdk_dcl_dto_header_t,
    ) -> *mut dnv_vista_sdk_serializable_document_t;

    pub(crate) fn dnv_vista_sdk_dcl_dto_header_ensure_custom_headers(
        h: *mut dnv_vista_sdk_dcl_dto_header_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_header_clear_custom_headers(
        h: *mut dnv_vista_sdk_dcl_dto_header_t,
    );

    // ConfigurationReferenceDto
    pub(crate) fn dnv_vista_sdk_dcl_dto_cfg_ref_get_id(
        r: *const dnv_vista_sdk_dcl_dto_cfg_ref_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_cfg_ref_set_id(
        r: *mut dnv_vista_sdk_dcl_dto_cfg_ref_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_cfg_ref_get_timestamp(
        r: *const dnv_vista_sdk_dcl_dto_cfg_ref_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_cfg_ref_set_timestamp(
        r: *mut dnv_vista_sdk_dcl_dto_cfg_ref_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_cfg_ref_has_version(
        r: *const dnv_vista_sdk_dcl_dto_cfg_ref_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_cfg_ref_get_version(
        r: *const dnv_vista_sdk_dcl_dto_cfg_ref_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_cfg_ref_set_version(
        r: *mut dnv_vista_sdk_dcl_dto_cfg_ref_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_cfg_ref_clear_version(
        r: *mut dnv_vista_sdk_dcl_dto_cfg_ref_t,
    );

    // VersionInformationDto
    pub(crate) fn dnv_vista_sdk_dcl_dto_ver_info_get_naming_rule(
        v: *const dnv_vista_sdk_dcl_dto_ver_info_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_ver_info_set_naming_rule(
        v: *mut dnv_vista_sdk_dcl_dto_ver_info_t,
        s: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_ver_info_get_naming_scheme_version(
        v: *const dnv_vista_sdk_dcl_dto_ver_info_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_ver_info_set_naming_scheme_version(
        v: *mut dnv_vista_sdk_dcl_dto_ver_info_t,
        s: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_ver_info_has_reference_url(
        v: *const dnv_vista_sdk_dcl_dto_ver_info_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_ver_info_get_reference_url(
        v: *const dnv_vista_sdk_dcl_dto_ver_info_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_ver_info_set_reference_url(
        v: *mut dnv_vista_sdk_dcl_dto_ver_info_t,
        s: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_ver_info_clear_reference_url(
        v: *mut dnv_vista_sdk_dcl_dto_ver_info_t,
    );

    // DataChannelListDto
    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_list_count(
        list: *const dnv_vista_sdk_dcl_dto_ch_list_t,
    ) -> usize;

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_list_at(
        list: *mut dnv_vista_sdk_dcl_dto_ch_list_t,
        index: usize,
    ) -> *mut dnv_vista_sdk_dcl_dto_channel_t;

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_list_push(
        list: *mut dnv_vista_sdk_dcl_dto_ch_list_t,
    ) -> *mut dnv_vista_sdk_dcl_dto_channel_t;

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_list_remove(
        list: *mut dnv_vista_sdk_dcl_dto_ch_list_t,
        index: usize,
    );

    // DataChannelDto
    pub(crate) fn dnv_vista_sdk_dcl_dto_channel_get_id(
        ch: *mut dnv_vista_sdk_dcl_dto_channel_t,
    ) -> *mut dnv_vista_sdk_dcl_dto_ch_id_t;

    pub(crate) fn dnv_vista_sdk_dcl_dto_channel_get_property(
        ch: *mut dnv_vista_sdk_dcl_dto_channel_t,
    ) -> *mut dnv_vista_sdk_dcl_dto_property_t;

    // DataChannelIdDto
    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_id_get_local_id(
        id: *const dnv_vista_sdk_dcl_dto_ch_id_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_id_set_local_id(
        id: *mut dnv_vista_sdk_dcl_dto_ch_id_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_id_has_short_id(
        id: *const dnv_vista_sdk_dcl_dto_ch_id_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_id_get_short_id(
        id: *const dnv_vista_sdk_dcl_dto_ch_id_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_id_set_short_id(
        id: *mut dnv_vista_sdk_dcl_dto_ch_id_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_id_clear_short_id(
        id: *mut dnv_vista_sdk_dcl_dto_ch_id_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_id_has_name_object(
        id: *const dnv_vista_sdk_dcl_dto_ch_id_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_id_get_name_object(
        id: *mut dnv_vista_sdk_dcl_dto_ch_id_t,
    ) -> *mut dnv_vista_sdk_dcl_dto_name_obj_t;

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_id_ensure_name_object(
        id: *mut dnv_vista_sdk_dcl_dto_ch_id_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_id_clear_name_object(
        id: *mut dnv_vista_sdk_dcl_dto_ch_id_t,
    );

    // NameObjectDto
    pub(crate) fn dnv_vista_sdk_dcl_dto_name_obj_get_naming_rule(
        n: *const dnv_vista_sdk_dcl_dto_name_obj_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_name_obj_set_naming_rule(
        n: *mut dnv_vista_sdk_dcl_dto_name_obj_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_name_obj_has_custom(
        n: *const dnv_vista_sdk_dcl_dto_name_obj_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_name_obj_get_custom(
        n: *mut dnv_vista_sdk_dcl_dto_name_obj_t,
    ) -> *mut dnv_vista_sdk_serializable_document_t;

    pub(crate) fn dnv_vista_sdk_dcl_dto_name_obj_ensure_custom(
        n: *mut dnv_vista_sdk_dcl_dto_name_obj_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_name_obj_clear_custom(
        n: *mut dnv_vista_sdk_dcl_dto_name_obj_t,
    );

    // PropertyDto
    pub(crate) fn dnv_vista_sdk_dcl_dto_property_get_ch_type(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
    ) -> *mut dnv_vista_sdk_dcl_dto_ch_type_t;

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_get_format(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
    ) -> *mut dnv_vista_sdk_dcl_dto_format_t;

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_has_range(
        p: *const dnv_vista_sdk_dcl_dto_property_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_get_range(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
    ) -> *mut dnv_vista_sdk_dcl_dto_range_t;

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_ensure_range(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_clear_range(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_has_unit(
        p: *const dnv_vista_sdk_dcl_dto_property_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_get_unit(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
    ) -> *mut dnv_vista_sdk_dcl_dto_unit_t;

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_ensure_unit(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_clear_unit(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_has_quality_coding(
        p: *const dnv_vista_sdk_dcl_dto_property_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_get_quality_coding(
        p: *const dnv_vista_sdk_dcl_dto_property_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_set_quality_coding(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_clear_quality_coding(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_has_alert_priority(
        p: *const dnv_vista_sdk_dcl_dto_property_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_get_alert_priority(
        p: *const dnv_vista_sdk_dcl_dto_property_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_set_alert_priority(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_clear_alert_priority(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_has_name(
        p: *const dnv_vista_sdk_dcl_dto_property_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_get_name(
        p: *const dnv_vista_sdk_dcl_dto_property_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_set_name(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_clear_name(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_has_remarks(
        p: *const dnv_vista_sdk_dcl_dto_property_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_get_remarks(
        p: *const dnv_vista_sdk_dcl_dto_property_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_set_remarks(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_clear_remarks(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_has_custom_properties(
        p: *const dnv_vista_sdk_dcl_dto_property_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_get_custom_properties(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
    ) -> *mut dnv_vista_sdk_serializable_document_t;

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_ensure_custom_properties(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_property_clear_custom_properties(
        p: *mut dnv_vista_sdk_dcl_dto_property_t,
    );

    // DataChannelTypeDto
    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_type_get_type(
        t: *const dnv_vista_sdk_dcl_dto_ch_type_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_type_set_type(
        t: *mut dnv_vista_sdk_dcl_dto_ch_type_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_type_has_update_cycle(
        t: *const dnv_vista_sdk_dcl_dto_ch_type_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_type_get_update_cycle(
        t: *const dnv_vista_sdk_dcl_dto_ch_type_t,
    ) -> f64;

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_type_set_update_cycle(
        t: *mut dnv_vista_sdk_dcl_dto_ch_type_t,
        v: f64,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_type_clear_update_cycle(
        t: *mut dnv_vista_sdk_dcl_dto_ch_type_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_type_has_calculation_period(
        t: *const dnv_vista_sdk_dcl_dto_ch_type_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_type_get_calculation_period(
        t: *const dnv_vista_sdk_dcl_dto_ch_type_t,
    ) -> f64;

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_type_set_calculation_period(
        t: *mut dnv_vista_sdk_dcl_dto_ch_type_t,
        v: f64,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_ch_type_clear_calculation_period(
        t: *mut dnv_vista_sdk_dcl_dto_ch_type_t,
    );

    // FormatDto
    pub(crate) fn dnv_vista_sdk_dcl_dto_format_get_type(
        f: *const dnv_vista_sdk_dcl_dto_format_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_format_set_type(
        f: *mut dnv_vista_sdk_dcl_dto_format_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_format_has_restriction(
        f: *const dnv_vista_sdk_dcl_dto_format_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_format_get_restriction(
        f: *mut dnv_vista_sdk_dcl_dto_format_t,
    ) -> *mut dnv_vista_sdk_dcl_dto_restriction_t;

    pub(crate) fn dnv_vista_sdk_dcl_dto_format_ensure_restriction(
        f: *mut dnv_vista_sdk_dcl_dto_format_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_format_clear_restriction(
        f: *mut dnv_vista_sdk_dcl_dto_format_t,
    );

    // RestrictionDto
    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_enumeration_count(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> usize;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_enumeration_at(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
        i: usize,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_set_enumeration(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
        values: *const *const std::ffi::c_char,
        count: usize,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_clear_enumeration(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_has_fraction_digits(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_get_fraction_digits(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> u32;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_set_fraction_digits(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
        v: u32,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_clear_fraction_digits(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_has_length(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_get_length(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> u32;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_set_length(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
        v: u32,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_clear_length(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_has_max_exclusive(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_get_max_exclusive(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> f64;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_set_max_exclusive(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
        v: f64,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_clear_max_exclusive(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_has_max_inclusive(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_get_max_inclusive(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> f64;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_set_max_inclusive(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
        v: f64,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_clear_max_inclusive(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_has_max_length(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_get_max_length(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> u32;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_set_max_length(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
        v: u32,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_clear_max_length(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_has_min_exclusive(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_get_min_exclusive(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> f64;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_set_min_exclusive(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
        v: f64,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_clear_min_exclusive(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_has_min_inclusive(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_get_min_inclusive(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> f64;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_set_min_inclusive(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
        v: f64,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_clear_min_inclusive(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_has_min_length(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_get_min_length(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> u32;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_set_min_length(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
        v: u32,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_clear_min_length(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_has_pattern(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_get_pattern(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_set_pattern(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_clear_pattern(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_has_total_digits(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_get_total_digits(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> u32;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_set_total_digits(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
        v: u32,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_clear_total_digits(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_has_white_space(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_get_white_space(
        r: *const dnv_vista_sdk_dcl_dto_restriction_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_set_white_space(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_restriction_clear_white_space(
        r: *mut dnv_vista_sdk_dcl_dto_restriction_t,
    );

    // RangeDto
    pub(crate) fn dnv_vista_sdk_dcl_dto_range_get_low(
        r: *const dnv_vista_sdk_dcl_dto_range_t,
    ) -> f64;

    pub(crate) fn dnv_vista_sdk_dcl_dto_range_set_low(
        r: *mut dnv_vista_sdk_dcl_dto_range_t,
        v: f64,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_range_get_high(
        r: *const dnv_vista_sdk_dcl_dto_range_t,
    ) -> f64;

    pub(crate) fn dnv_vista_sdk_dcl_dto_range_set_high(
        r: *mut dnv_vista_sdk_dcl_dto_range_t,
        v: f64,
    );

    // UnitDto
    pub(crate) fn dnv_vista_sdk_dcl_dto_unit_get_symbol(
        u: *const dnv_vista_sdk_dcl_dto_unit_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_unit_set_symbol(
        u: *mut dnv_vista_sdk_dcl_dto_unit_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_unit_has_quantity_name(
        u: *const dnv_vista_sdk_dcl_dto_unit_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_unit_get_quantity_name(
        u: *const dnv_vista_sdk_dcl_dto_unit_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_dcl_dto_unit_set_quantity_name(
        u: *mut dnv_vista_sdk_dcl_dto_unit_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_unit_clear_quantity_name(
        u: *mut dnv_vista_sdk_dcl_dto_unit_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_unit_has_custom_elements(
        u: *const dnv_vista_sdk_dcl_dto_unit_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_dcl_dto_unit_get_custom_elements(
        u: *mut dnv_vista_sdk_dcl_dto_unit_t,
    ) -> *mut dnv_vista_sdk_serializable_document_t;

    pub(crate) fn dnv_vista_sdk_dcl_dto_unit_ensure_custom_elements(
        u: *mut dnv_vista_sdk_dcl_dto_unit_t,
    );

    pub(crate) fn dnv_vista_sdk_dcl_dto_unit_clear_custom_elements(
        u: *mut dnv_vista_sdk_dcl_dto_unit_t,
    );
}
