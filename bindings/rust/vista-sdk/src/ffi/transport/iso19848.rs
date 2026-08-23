use crate::ffi::types::date_time_offset::dnv_vista_sdk_date_time_offset_t;
use crate::ffi::types::decimal::dnv_vista_sdk_decimal_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_iso19848 {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_iso19848_t = dnv_vista_sdk_iso19848;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_iso19848_data_channel_type_name {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_iso19848_data_channel_type_name_t =
    dnv_vista_sdk_iso19848_data_channel_type_name;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_iso19848_data_channel_type_names {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_iso19848_data_channel_type_names_t =
    dnv_vista_sdk_iso19848_data_channel_type_names;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_iso19848_format_data_type {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_iso19848_format_data_type_t = dnv_vista_sdk_iso19848_format_data_type;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_iso19848_format_data_types {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_iso19848_format_data_types_t =
    dnv_vista_sdk_iso19848_format_data_types;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_iso19848_value {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_iso19848_value_t = dnv_vista_sdk_iso19848_value;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) enum dnv_vista_sdk_iso19848_version_t {
    V2018 = 0,
    V2024 = 1,
}

#[allow(non_camel_case_types, dead_code)]
#[repr(C)]
pub(crate) enum dnv_vista_sdk_iso19848_value_type_t {
    Decimal = 0,
    Integer = 1,
    Boolean = 2,
    String = 3,
    DateTime = 4,
}

extern "C" {
    // ISO19848 singleton
    pub(crate) fn dnv_vista_sdk_iso19848_instance() -> *const dnv_vista_sdk_iso19848_t;
    pub(crate) fn dnv_vista_sdk_iso19848_version_count(
        iso19848: *const dnv_vista_sdk_iso19848_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_iso19848_version_at(
        iso19848: *const dnv_vista_sdk_iso19848_t,
        index: usize,
    ) -> dnv_vista_sdk_iso19848_version_t;
    pub(crate) fn dnv_vista_sdk_iso19848_latest(
        iso19848: *const dnv_vista_sdk_iso19848_t,
    ) -> dnv_vista_sdk_iso19848_version_t;
    pub(crate) fn dnv_vista_sdk_iso19848_data_channel_type_names(
        iso19848: *const dnv_vista_sdk_iso19848_t,
        version: dnv_vista_sdk_iso19848_version_t,
    ) -> *mut dnv_vista_sdk_iso19848_data_channel_type_names_t;
    pub(crate) fn dnv_vista_sdk_iso19848_format_data_types(
        iso19848: *const dnv_vista_sdk_iso19848_t,
        version: dnv_vista_sdk_iso19848_version_t,
    ) -> *mut dnv_vista_sdk_iso19848_format_data_types_t;

    // DataChannelTypeName
    pub(crate) fn dnv_vista_sdk_iso19848_data_channel_type_name_free(
        type_name: *mut dnv_vista_sdk_iso19848_data_channel_type_name_t,
    );
    pub(crate) fn dnv_vista_sdk_iso19848_data_channel_type_name_type(
        type_name: *const dnv_vista_sdk_iso19848_data_channel_type_name_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_iso19848_data_channel_type_name_description(
        type_name: *const dnv_vista_sdk_iso19848_data_channel_type_name_t,
    ) -> *const std::ffi::c_char;

    // DataChannelTypeNames
    pub(crate) fn dnv_vista_sdk_iso19848_data_channel_type_names_free(
        type_names: *mut dnv_vista_sdk_iso19848_data_channel_type_names_t,
    );
    pub(crate) fn dnv_vista_sdk_iso19848_data_channel_type_names_from_string(
        type_names: *const dnv_vista_sdk_iso19848_data_channel_type_names_t,
        type_: *const std::ffi::c_char,
    ) -> *mut dnv_vista_sdk_iso19848_data_channel_type_name_t;
    pub(crate) fn dnv_vista_sdk_iso19848_data_channel_type_names_count(
        type_names: *const dnv_vista_sdk_iso19848_data_channel_type_names_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_iso19848_data_channel_type_names_at(
        type_names: *const dnv_vista_sdk_iso19848_data_channel_type_names_t,
        index: usize,
    ) -> *const dnv_vista_sdk_iso19848_data_channel_type_name_t;

    // FormatDataType
    pub(crate) fn dnv_vista_sdk_iso19848_format_data_type_free(
        format_data_type: *mut dnv_vista_sdk_iso19848_format_data_type_t,
    );
    pub(crate) fn dnv_vista_sdk_iso19848_format_data_type_type(
        format_data_type: *const dnv_vista_sdk_iso19848_format_data_type_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_iso19848_format_data_type_description(
        format_data_type: *const dnv_vista_sdk_iso19848_format_data_type_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_iso19848_format_data_type_validate(
        format_data_type: *const dnv_vista_sdk_iso19848_format_data_type_t,
        value: *const std::ffi::c_char,
        result: *mut *mut dnv_vista_sdk_iso19848_value_t,
    ) -> i32;

    // FormatDataTypes
    pub(crate) fn dnv_vista_sdk_iso19848_format_data_types_free(
        format_data_types: *mut dnv_vista_sdk_iso19848_format_data_types_t,
    );
    pub(crate) fn dnv_vista_sdk_iso19848_format_data_types_from_string(
        format_data_types: *const dnv_vista_sdk_iso19848_format_data_types_t,
        type_: *const std::ffi::c_char,
    ) -> *mut dnv_vista_sdk_iso19848_format_data_type_t;
    pub(crate) fn dnv_vista_sdk_iso19848_format_data_types_count(
        format_data_types: *const dnv_vista_sdk_iso19848_format_data_types_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_iso19848_format_data_types_at(
        format_data_types: *const dnv_vista_sdk_iso19848_format_data_types_t,
        index: usize,
    ) -> *const dnv_vista_sdk_iso19848_format_data_type_t;

    // Value
    pub(crate) fn dnv_vista_sdk_iso19848_value_from_string(
        value: *const std::ffi::c_char,
    ) -> *mut dnv_vista_sdk_iso19848_value_t;
    pub(crate) fn dnv_vista_sdk_iso19848_value_from_integer(
        value: i64,
    ) -> *mut dnv_vista_sdk_iso19848_value_t;
    pub(crate) fn dnv_vista_sdk_iso19848_value_from_boolean(
        value: i32,
    ) -> *mut dnv_vista_sdk_iso19848_value_t;
    pub(crate) fn dnv_vista_sdk_iso19848_value_from_decimal(
        value: dnv_vista_sdk_decimal_t,
    ) -> *mut dnv_vista_sdk_iso19848_value_t;
    pub(crate) fn dnv_vista_sdk_iso19848_value_from_date_time(
        value: dnv_vista_sdk_date_time_offset_t,
    ) -> *mut dnv_vista_sdk_iso19848_value_t;
    pub(crate) fn dnv_vista_sdk_iso19848_value_free(value: *mut dnv_vista_sdk_iso19848_value_t);
    pub(crate) fn dnv_vista_sdk_iso19848_value_type(
        value: *const dnv_vista_sdk_iso19848_value_t,
    ) -> dnv_vista_sdk_iso19848_value_type_t;
    pub(crate) fn dnv_vista_sdk_iso19848_value_string(
        value: *const dnv_vista_sdk_iso19848_value_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_iso19848_value_boolean(
        value: *const dnv_vista_sdk_iso19848_value_t,
        result: *mut i32,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_iso19848_value_integer(
        value: *const dnv_vista_sdk_iso19848_value_t,
        result: *mut i64,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_iso19848_value_decimal(
        value: *const dnv_vista_sdk_iso19848_value_t,
        result: *mut dnv_vista_sdk_decimal_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_iso19848_value_date_time(
        value: *const dnv_vista_sdk_iso19848_value_t,
        result: *mut dnv_vista_sdk_date_time_offset_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_iso19848_value_to_string(
        value: *const dnv_vista_sdk_iso19848_value_t,
    ) -> *mut std::ffi::c_char;
}
