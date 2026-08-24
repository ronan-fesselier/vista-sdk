use crate::ffi::core::local_id::dnv_vista_sdk_local_id_t;
use crate::ffi::transport::iso19848::dnv_vista_sdk_iso19848_value_t;
use crate::ffi::transport::serializable_document::dnv_vista_sdk_serializable_document_t;
use crate::ffi::transport::ship_id::dnv_vista_sdk_ship_id_t;
use crate::ffi::types::date_time_offset::dnv_vista_sdk_date_time_offset_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_restriction {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_restriction_t = dnv_vista_sdk_dcl_restriction;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_range {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_range_t = dnv_vista_sdk_dcl_range;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_format {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_format_t = dnv_vista_sdk_dcl_format;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_data_channel_type {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_data_channel_type_t = dnv_vista_sdk_dcl_data_channel_type;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_name_object {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_name_object_t = dnv_vista_sdk_dcl_name_object;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_unit {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_unit_t = dnv_vista_sdk_dcl_unit;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_property {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_property_t = dnv_vista_sdk_dcl_property;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_configuration_reference {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_configuration_reference_t =
    dnv_vista_sdk_dcl_configuration_reference;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_version_information {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_version_information_t = dnv_vista_sdk_dcl_version_information;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_channel_id {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_channel_id_t = dnv_vista_sdk_dcl_channel_id;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_header {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_header_t = dnv_vista_sdk_dcl_header;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_data_channel {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_data_channel_t = dnv_vista_sdk_dcl_data_channel;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_data_channel_list {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_data_channel_list_t = dnv_vista_sdk_dcl_data_channel_list;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_package {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_package_t = dnv_vista_sdk_dcl_package;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_dcl_list_package {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_dcl_list_package_t = dnv_vista_sdk_dcl_list_package;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) enum dnv_vista_sdk_dcl_white_space_t {
    Preserve = 0,
    Replace = 1,
    Collapse = 2,
}

extern "C" {
    // Restriction
    pub(crate) fn dnv_vista_sdk_dcl_restriction_create() -> *mut dnv_vista_sdk_dcl_restriction_t;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_free(r: *mut dnv_vista_sdk_dcl_restriction_t);
    pub(crate) fn dnv_vista_sdk_dcl_restriction_enumeration_count(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_enumeration_at(
        r: *const dnv_vista_sdk_dcl_restriction_t,
        index: usize,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_set_enumeration(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
        values: *const *const std::ffi::c_char,
        count: usize,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_clear_enumeration(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_has_fraction_digits(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_fraction_digits(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> u32;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_set_fraction_digits(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
        value: u32,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_clear_fraction_digits(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_has_length(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_length(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> u32;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_set_length(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
        value: u32,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_clear_length(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_has_max_exclusive(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_max_exclusive(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> f64;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_set_max_exclusive(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
        value: f64,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_clear_max_exclusive(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_has_max_inclusive(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_max_inclusive(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> f64;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_set_max_inclusive(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
        value: f64,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_clear_max_inclusive(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_has_max_length(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_max_length(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> u32;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_set_max_length(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
        value: u32,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_clear_max_length(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_has_min_exclusive(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_min_exclusive(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> f64;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_set_min_exclusive(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
        value: f64,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_clear_min_exclusive(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_has_min_inclusive(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_min_inclusive(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> f64;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_set_min_inclusive(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
        value: f64,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_clear_min_inclusive(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_has_min_length(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_min_length(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> u32;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_set_min_length(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
        value: u32,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_clear_min_length(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_pattern(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_set_pattern(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
        value: *const std::ffi::c_char,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_clear_pattern(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_has_total_digits(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_total_digits(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> u32;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_set_total_digits(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
        value: u32,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_clear_total_digits(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_has_white_space(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_white_space(
        r: *const dnv_vista_sdk_dcl_restriction_t,
    ) -> dnv_vista_sdk_dcl_white_space_t;
    pub(crate) fn dnv_vista_sdk_dcl_restriction_set_white_space(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
        value: dnv_vista_sdk_dcl_white_space_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_clear_white_space(
        r: *mut dnv_vista_sdk_dcl_restriction_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_restriction_validate_value(
        r: *const dnv_vista_sdk_dcl_restriction_t,
        value: *const std::ffi::c_char,
        format: *const dnv_vista_sdk_dcl_format_t,
    ) -> i32;

    // Range
    pub(crate) fn dnv_vista_sdk_dcl_range_create(
        low: f64,
        high: f64,
    ) -> *mut dnv_vista_sdk_dcl_range_t;
    pub(crate) fn dnv_vista_sdk_dcl_range_free(range: *mut dnv_vista_sdk_dcl_range_t);
    pub(crate) fn dnv_vista_sdk_dcl_range_low(range: *const dnv_vista_sdk_dcl_range_t) -> f64;
    pub(crate) fn dnv_vista_sdk_dcl_range_high(range: *const dnv_vista_sdk_dcl_range_t) -> f64;
    pub(crate) fn dnv_vista_sdk_dcl_range_set_low(range: *mut dnv_vista_sdk_dcl_range_t, low: f64);
    pub(crate) fn dnv_vista_sdk_dcl_range_set_high(
        range: *mut dnv_vista_sdk_dcl_range_t,
        high: f64,
    );

    // Format
    pub(crate) fn dnv_vista_sdk_dcl_format_create(
        type_: *const std::ffi::c_char,
    ) -> *mut dnv_vista_sdk_dcl_format_t;
    pub(crate) fn dnv_vista_sdk_dcl_format_free(format: *mut dnv_vista_sdk_dcl_format_t);
    pub(crate) fn dnv_vista_sdk_dcl_format_type(
        format: *const dnv_vista_sdk_dcl_format_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_dcl_format_set_type(
        format: *mut dnv_vista_sdk_dcl_format_t,
        type_: *const std::ffi::c_char,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_dcl_format_restriction(
        format: *const dnv_vista_sdk_dcl_format_t,
    ) -> *const dnv_vista_sdk_dcl_restriction_t;
    pub(crate) fn dnv_vista_sdk_dcl_format_set_restriction(
        format: *mut dnv_vista_sdk_dcl_format_t,
        restriction: *const dnv_vista_sdk_dcl_restriction_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_format_clear_restriction(
        format: *mut dnv_vista_sdk_dcl_format_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_format_validate_value(
        format: *const dnv_vista_sdk_dcl_format_t,
        value: *const std::ffi::c_char,
        parsed_value: *mut *mut dnv_vista_sdk_iso19848_value_t,
    ) -> i32;

    // DataChannelType
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_type_create(
        type_: *const std::ffi::c_char,
    ) -> *mut dnv_vista_sdk_dcl_data_channel_type_t;
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_type_free(
        dct: *mut dnv_vista_sdk_dcl_data_channel_type_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_type_type(
        dct: *const dnv_vista_sdk_dcl_data_channel_type_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_type_set_type(
        dct: *mut dnv_vista_sdk_dcl_data_channel_type_t,
        type_: *const std::ffi::c_char,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_type_has_update_cycle(
        dct: *const dnv_vista_sdk_dcl_data_channel_type_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_type_update_cycle(
        dct: *const dnv_vista_sdk_dcl_data_channel_type_t,
    ) -> f64;
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_type_set_update_cycle(
        dct: *mut dnv_vista_sdk_dcl_data_channel_type_t,
        value: f64,
    );
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_type_clear_update_cycle(
        dct: *mut dnv_vista_sdk_dcl_data_channel_type_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_type_has_calculation_period(
        dct: *const dnv_vista_sdk_dcl_data_channel_type_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_type_calculation_period(
        dct: *const dnv_vista_sdk_dcl_data_channel_type_t,
    ) -> f64;
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_type_set_calculation_period(
        dct: *mut dnv_vista_sdk_dcl_data_channel_type_t,
        value: f64,
    );
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_type_clear_calculation_period(
        dct: *mut dnv_vista_sdk_dcl_data_channel_type_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_type_is_alert(
        dct: *const dnv_vista_sdk_dcl_data_channel_type_t,
    ) -> i32;

    // NameObject
    pub(crate) fn dnv_vista_sdk_dcl_name_object_create_default(
    ) -> *mut dnv_vista_sdk_dcl_name_object_t;
    pub(crate) fn dnv_vista_sdk_dcl_name_object_create(
        naming_rule: *const std::ffi::c_char,
    ) -> *mut dnv_vista_sdk_dcl_name_object_t;
    pub(crate) fn dnv_vista_sdk_dcl_name_object_free(
        name_object: *mut dnv_vista_sdk_dcl_name_object_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_name_object_naming_rule(
        name_object: *const dnv_vista_sdk_dcl_name_object_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_dcl_name_object_set_naming_rule(
        name_object: *mut dnv_vista_sdk_dcl_name_object_t,
        naming_rule: *const std::ffi::c_char,
    );
    pub(crate) fn dnv_vista_sdk_dcl_name_object_custom_name_objects(
        name_object: *const dnv_vista_sdk_dcl_name_object_t,
    ) -> *const dnv_vista_sdk_serializable_document_t;
    pub(crate) fn dnv_vista_sdk_dcl_name_object_set_custom_name_objects(
        name_object: *mut dnv_vista_sdk_dcl_name_object_t,
        value: *mut dnv_vista_sdk_serializable_document_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_name_object_clear_custom_name_objects(
        name_object: *mut dnv_vista_sdk_dcl_name_object_t,
    );

    // Unit
    pub(crate) fn dnv_vista_sdk_dcl_unit_create(
        unit_symbol: *const std::ffi::c_char,
    ) -> *mut dnv_vista_sdk_dcl_unit_t;
    pub(crate) fn dnv_vista_sdk_dcl_unit_free(unit: *mut dnv_vista_sdk_dcl_unit_t);
    pub(crate) fn dnv_vista_sdk_dcl_unit_unit_symbol(
        unit: *const dnv_vista_sdk_dcl_unit_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_dcl_unit_set_unit_symbol(
        unit: *mut dnv_vista_sdk_dcl_unit_t,
        unit_symbol: *const std::ffi::c_char,
    );
    pub(crate) fn dnv_vista_sdk_dcl_unit_quantity_name(
        unit: *const dnv_vista_sdk_dcl_unit_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_dcl_unit_set_quantity_name(
        unit: *mut dnv_vista_sdk_dcl_unit_t,
        quantity_name: *const std::ffi::c_char,
    );
    pub(crate) fn dnv_vista_sdk_dcl_unit_clear_quantity_name(unit: *mut dnv_vista_sdk_dcl_unit_t);
    pub(crate) fn dnv_vista_sdk_dcl_unit_set_custom_elements(
        unit: *mut dnv_vista_sdk_dcl_unit_t,
        value: *mut dnv_vista_sdk_serializable_document_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_unit_clear_custom_elements(unit: *mut dnv_vista_sdk_dcl_unit_t);

    // Property
    pub(crate) fn dnv_vista_sdk_dcl_property_create(
        data_channel_type: *const dnv_vista_sdk_dcl_data_channel_type_t,
        format: *const dnv_vista_sdk_dcl_format_t,
    ) -> *mut dnv_vista_sdk_dcl_property_t;
    pub(crate) fn dnv_vista_sdk_dcl_property_free(property: *mut dnv_vista_sdk_dcl_property_t);
    pub(crate) fn dnv_vista_sdk_dcl_property_data_channel_type(
        property: *const dnv_vista_sdk_dcl_property_t,
    ) -> *const dnv_vista_sdk_dcl_data_channel_type_t;
    pub(crate) fn dnv_vista_sdk_dcl_property_set_data_channel_type(
        property: *mut dnv_vista_sdk_dcl_property_t,
        data_channel_type: *const dnv_vista_sdk_dcl_data_channel_type_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_property_format(
        property: *const dnv_vista_sdk_dcl_property_t,
    ) -> *const dnv_vista_sdk_dcl_format_t;
    pub(crate) fn dnv_vista_sdk_dcl_property_set_format(
        property: *mut dnv_vista_sdk_dcl_property_t,
        format: *const dnv_vista_sdk_dcl_format_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_property_range(
        property: *const dnv_vista_sdk_dcl_property_t,
    ) -> *const dnv_vista_sdk_dcl_range_t;
    pub(crate) fn dnv_vista_sdk_dcl_property_set_range(
        property: *mut dnv_vista_sdk_dcl_property_t,
        range: *const dnv_vista_sdk_dcl_range_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_property_clear_range(
        property: *mut dnv_vista_sdk_dcl_property_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_property_unit(
        property: *const dnv_vista_sdk_dcl_property_t,
    ) -> *const dnv_vista_sdk_dcl_unit_t;
    pub(crate) fn dnv_vista_sdk_dcl_property_set_unit(
        property: *mut dnv_vista_sdk_dcl_property_t,
        unit: *const dnv_vista_sdk_dcl_unit_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_property_clear_unit(
        property: *mut dnv_vista_sdk_dcl_property_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_property_quality_coding(
        property: *const dnv_vista_sdk_dcl_property_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_dcl_property_set_quality_coding(
        property: *mut dnv_vista_sdk_dcl_property_t,
        quality_coding: *const std::ffi::c_char,
    );
    pub(crate) fn dnv_vista_sdk_dcl_property_clear_quality_coding(
        property: *mut dnv_vista_sdk_dcl_property_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_property_alert_priority(
        property: *const dnv_vista_sdk_dcl_property_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_dcl_property_set_alert_priority(
        property: *mut dnv_vista_sdk_dcl_property_t,
        alert_priority: *const std::ffi::c_char,
    );
    pub(crate) fn dnv_vista_sdk_dcl_property_clear_alert_priority(
        property: *mut dnv_vista_sdk_dcl_property_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_property_name(
        property: *const dnv_vista_sdk_dcl_property_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_dcl_property_set_name(
        property: *mut dnv_vista_sdk_dcl_property_t,
        name: *const std::ffi::c_char,
    );
    pub(crate) fn dnv_vista_sdk_dcl_property_clear_name(
        property: *mut dnv_vista_sdk_dcl_property_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_property_remarks(
        property: *const dnv_vista_sdk_dcl_property_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_dcl_property_set_remarks(
        property: *mut dnv_vista_sdk_dcl_property_t,
        remarks: *const std::ffi::c_char,
    );
    pub(crate) fn dnv_vista_sdk_dcl_property_clear_remarks(
        property: *mut dnv_vista_sdk_dcl_property_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_property_set_custom_properties(
        property: *mut dnv_vista_sdk_dcl_property_t,
        value: *mut dnv_vista_sdk_serializable_document_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_property_clear_custom_properties(
        property: *mut dnv_vista_sdk_dcl_property_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_property_validate(
        property: *const dnv_vista_sdk_dcl_property_t,
    ) -> i32;

    // ConfigurationReference
    pub(crate) fn dnv_vista_sdk_dcl_configuration_reference_create(
        id: *const std::ffi::c_char,
        timestamp: dnv_vista_sdk_date_time_offset_t,
    ) -> *mut dnv_vista_sdk_dcl_configuration_reference_t;
    pub(crate) fn dnv_vista_sdk_dcl_configuration_reference_free(
        config_ref: *mut dnv_vista_sdk_dcl_configuration_reference_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_configuration_reference_id(
        config_ref: *const dnv_vista_sdk_dcl_configuration_reference_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_dcl_configuration_reference_set_id(
        config_ref: *mut dnv_vista_sdk_dcl_configuration_reference_t,
        id: *const std::ffi::c_char,
    );
    pub(crate) fn dnv_vista_sdk_dcl_configuration_reference_version(
        config_ref: *const dnv_vista_sdk_dcl_configuration_reference_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_dcl_configuration_reference_set_version(
        config_ref: *mut dnv_vista_sdk_dcl_configuration_reference_t,
        version: *const std::ffi::c_char,
    );
    pub(crate) fn dnv_vista_sdk_dcl_configuration_reference_clear_version(
        config_ref: *mut dnv_vista_sdk_dcl_configuration_reference_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_configuration_reference_timestamp(
        config_ref: *const dnv_vista_sdk_dcl_configuration_reference_t,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_dcl_configuration_reference_set_timestamp(
        config_ref: *mut dnv_vista_sdk_dcl_configuration_reference_t,
        timestamp: dnv_vista_sdk_date_time_offset_t,
    );

    // VersionInformation
    pub(crate) fn dnv_vista_sdk_dcl_version_information_create_default(
    ) -> *mut dnv_vista_sdk_dcl_version_information_t;
    pub(crate) fn dnv_vista_sdk_dcl_version_information_create(
        naming_rule: *const std::ffi::c_char,
        naming_scheme_version: *const std::ffi::c_char,
    ) -> *mut dnv_vista_sdk_dcl_version_information_t;
    pub(crate) fn dnv_vista_sdk_dcl_version_information_free(
        version_info: *mut dnv_vista_sdk_dcl_version_information_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_version_information_naming_rule(
        version_info: *const dnv_vista_sdk_dcl_version_information_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_dcl_version_information_set_naming_rule(
        version_info: *mut dnv_vista_sdk_dcl_version_information_t,
        naming_rule: *const std::ffi::c_char,
    );
    pub(crate) fn dnv_vista_sdk_dcl_version_information_naming_scheme_version(
        version_info: *const dnv_vista_sdk_dcl_version_information_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_dcl_version_information_set_naming_scheme_version(
        version_info: *mut dnv_vista_sdk_dcl_version_information_t,
        naming_scheme_version: *const std::ffi::c_char,
    );
    pub(crate) fn dnv_vista_sdk_dcl_version_information_reference_url(
        version_info: *const dnv_vista_sdk_dcl_version_information_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_dcl_version_information_set_reference_url(
        version_info: *mut dnv_vista_sdk_dcl_version_information_t,
        reference_url: *const std::ffi::c_char,
    );
    pub(crate) fn dnv_vista_sdk_dcl_version_information_clear_reference_url(
        version_info: *mut dnv_vista_sdk_dcl_version_information_t,
    );

    // DataChannelId (dcl::DataChannelId, Table 15)
    pub(crate) fn dnv_vista_sdk_dcl_channel_id_create(
        local_id: *const dnv_vista_sdk_local_id_t,
    ) -> *mut dnv_vista_sdk_dcl_channel_id_t;
    pub(crate) fn dnv_vista_sdk_dcl_channel_id_free(
        channel_id: *mut dnv_vista_sdk_dcl_channel_id_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_channel_id_local_id(
        channel_id: *const dnv_vista_sdk_dcl_channel_id_t,
    ) -> *const dnv_vista_sdk_local_id_t;
    pub(crate) fn dnv_vista_sdk_dcl_channel_id_set_local_id(
        channel_id: *mut dnv_vista_sdk_dcl_channel_id_t,
        local_id: *const dnv_vista_sdk_local_id_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_channel_id_short_id(
        channel_id: *const dnv_vista_sdk_dcl_channel_id_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_dcl_channel_id_set_short_id(
        channel_id: *mut dnv_vista_sdk_dcl_channel_id_t,
        short_id: *const std::ffi::c_char,
    );
    pub(crate) fn dnv_vista_sdk_dcl_channel_id_clear_short_id(
        channel_id: *mut dnv_vista_sdk_dcl_channel_id_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_channel_id_name_object(
        channel_id: *const dnv_vista_sdk_dcl_channel_id_t,
    ) -> *const dnv_vista_sdk_dcl_name_object_t;
    pub(crate) fn dnv_vista_sdk_dcl_channel_id_set_name_object(
        channel_id: *mut dnv_vista_sdk_dcl_channel_id_t,
        name_object: *const dnv_vista_sdk_dcl_name_object_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_channel_id_clear_name_object(
        channel_id: *mut dnv_vista_sdk_dcl_channel_id_t,
    );

    // Header
    pub(crate) fn dnv_vista_sdk_dcl_header_create(
        ship_id: *const dnv_vista_sdk_ship_id_t,
        data_channel_list_id: *const dnv_vista_sdk_dcl_configuration_reference_t,
    ) -> *mut dnv_vista_sdk_dcl_header_t;
    pub(crate) fn dnv_vista_sdk_dcl_header_free(header: *mut dnv_vista_sdk_dcl_header_t);
    pub(crate) fn dnv_vista_sdk_dcl_header_ship_id(
        header: *const dnv_vista_sdk_dcl_header_t,
    ) -> *const dnv_vista_sdk_ship_id_t;
    pub(crate) fn dnv_vista_sdk_dcl_header_set_ship_id(
        header: *mut dnv_vista_sdk_dcl_header_t,
        ship_id: *const dnv_vista_sdk_ship_id_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_header_data_channel_list_id(
        header: *const dnv_vista_sdk_dcl_header_t,
    ) -> *const dnv_vista_sdk_dcl_configuration_reference_t;
    pub(crate) fn dnv_vista_sdk_dcl_header_set_data_channel_list_id(
        header: *mut dnv_vista_sdk_dcl_header_t,
        data_channel_list_id: *const dnv_vista_sdk_dcl_configuration_reference_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_header_version_information(
        header: *const dnv_vista_sdk_dcl_header_t,
    ) -> *const dnv_vista_sdk_dcl_version_information_t;
    pub(crate) fn dnv_vista_sdk_dcl_header_set_version_information(
        header: *mut dnv_vista_sdk_dcl_header_t,
        version_information: *const dnv_vista_sdk_dcl_version_information_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_header_clear_version_information(
        header: *mut dnv_vista_sdk_dcl_header_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_header_author(
        header: *const dnv_vista_sdk_dcl_header_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_dcl_header_set_author(
        header: *mut dnv_vista_sdk_dcl_header_t,
        author: *const std::ffi::c_char,
    );
    pub(crate) fn dnv_vista_sdk_dcl_header_clear_author(header: *mut dnv_vista_sdk_dcl_header_t);
    pub(crate) fn dnv_vista_sdk_dcl_header_has_date_created(
        header: *const dnv_vista_sdk_dcl_header_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_dcl_header_date_created(
        header: *const dnv_vista_sdk_dcl_header_t,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_dcl_header_set_date_created(
        header: *mut dnv_vista_sdk_dcl_header_t,
        date_created: dnv_vista_sdk_date_time_offset_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_header_clear_date_created(
        header: *mut dnv_vista_sdk_dcl_header_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_header_custom_headers(
        header: *const dnv_vista_sdk_dcl_header_t,
    ) -> *const dnv_vista_sdk_serializable_document_t;
    pub(crate) fn dnv_vista_sdk_dcl_header_set_custom_headers(
        header: *mut dnv_vista_sdk_dcl_header_t,
        value: *mut dnv_vista_sdk_serializable_document_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_header_clear_custom_headers(
        header: *mut dnv_vista_sdk_dcl_header_t,
    );

    // DataChannel
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_create(
        channel_id: *const dnv_vista_sdk_dcl_channel_id_t,
        property: *const dnv_vista_sdk_dcl_property_t,
    ) -> *mut dnv_vista_sdk_dcl_data_channel_t;
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_free(
        data_channel: *mut dnv_vista_sdk_dcl_data_channel_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_channel_id(
        data_channel: *const dnv_vista_sdk_dcl_data_channel_t,
    ) -> *const dnv_vista_sdk_dcl_channel_id_t;
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_set_channel_id(
        data_channel: *mut dnv_vista_sdk_dcl_data_channel_t,
        channel_id: *const dnv_vista_sdk_dcl_channel_id_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_property(
        data_channel: *const dnv_vista_sdk_dcl_data_channel_t,
    ) -> *const dnv_vista_sdk_dcl_property_t;
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_set_property(
        data_channel: *mut dnv_vista_sdk_dcl_data_channel_t,
        property: *const dnv_vista_sdk_dcl_property_t,
    ) -> i32;

    // DataChannelList
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_list_create(
    ) -> *mut dnv_vista_sdk_dcl_data_channel_list_t;
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_list_free(
        list: *mut dnv_vista_sdk_dcl_data_channel_list_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_list_size(
        list: *const dnv_vista_sdk_dcl_data_channel_list_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_list_at(
        list: *const dnv_vista_sdk_dcl_data_channel_list_t,
        index: usize,
    ) -> *const dnv_vista_sdk_dcl_data_channel_t;
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_list_from_short_id(
        list: *const dnv_vista_sdk_dcl_data_channel_list_t,
        short_id: *const std::ffi::c_char,
    ) -> *const dnv_vista_sdk_dcl_data_channel_t;
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_list_from_local_id(
        list: *const dnv_vista_sdk_dcl_data_channel_list_t,
        local_id: *const dnv_vista_sdk_local_id_t,
    ) -> *const dnv_vista_sdk_dcl_data_channel_t;
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_list_add(
        list: *mut dnv_vista_sdk_dcl_data_channel_list_t,
        data_channel: *const dnv_vista_sdk_dcl_data_channel_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_list_remove(
        list: *mut dnv_vista_sdk_dcl_data_channel_list_t,
        item: *const dnv_vista_sdk_dcl_data_channel_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_dcl_data_channel_list_clear(
        list: *mut dnv_vista_sdk_dcl_data_channel_list_t,
    );

    // Package
    pub(crate) fn dnv_vista_sdk_dcl_package_create(
        header: *const dnv_vista_sdk_dcl_header_t,
        data_channel_list: *const dnv_vista_sdk_dcl_data_channel_list_t,
    ) -> *mut dnv_vista_sdk_dcl_package_t;
    pub(crate) fn dnv_vista_sdk_dcl_package_free(package: *mut dnv_vista_sdk_dcl_package_t);
    pub(crate) fn dnv_vista_sdk_dcl_package_header(
        package: *const dnv_vista_sdk_dcl_package_t,
    ) -> *const dnv_vista_sdk_dcl_header_t;
    pub(crate) fn dnv_vista_sdk_dcl_package_set_header(
        package: *mut dnv_vista_sdk_dcl_package_t,
        header: *const dnv_vista_sdk_dcl_header_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_package_data_channel_list(
        package: *const dnv_vista_sdk_dcl_package_t,
    ) -> *const dnv_vista_sdk_dcl_data_channel_list_t;
    pub(crate) fn dnv_vista_sdk_dcl_package_set_data_channel_list(
        package: *mut dnv_vista_sdk_dcl_package_t,
        data_channel_list: *const dnv_vista_sdk_dcl_data_channel_list_t,
    );

    // DataChannelListPackage
    pub(crate) fn dnv_vista_sdk_dcl_list_package_create(
        package: *const dnv_vista_sdk_dcl_package_t,
    ) -> *mut dnv_vista_sdk_dcl_list_package_t;
    pub(crate) fn dnv_vista_sdk_dcl_list_package_free(
        list_package: *mut dnv_vista_sdk_dcl_list_package_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_list_package_package(
        list_package: *const dnv_vista_sdk_dcl_list_package_t,
    ) -> *const dnv_vista_sdk_dcl_package_t;
    pub(crate) fn dnv_vista_sdk_dcl_list_package_set_package(
        list_package: *mut dnv_vista_sdk_dcl_list_package_t,
        package: *const dnv_vista_sdk_dcl_package_t,
    );
    pub(crate) fn dnv_vista_sdk_dcl_list_package_data_channel_list(
        list_package: *const dnv_vista_sdk_dcl_list_package_t,
    ) -> *const dnv_vista_sdk_dcl_data_channel_list_t;

    // JSON serialization
    pub(crate) fn dnv_vista_sdk_dcl_list_package_from_json(
        json: *const std::ffi::c_char,
    ) -> *mut dnv_vista_sdk_dcl_list_package_t;
    pub(crate) fn dnv_vista_sdk_dcl_list_package_to_json(
        package: *const dnv_vista_sdk_dcl_list_package_t,
        pretty_print: std::ffi::c_int,
    ) -> *mut std::ffi::c_char;
}
