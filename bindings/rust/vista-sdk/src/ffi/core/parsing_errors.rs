use std::ffi::{c_char, c_int};

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_parsing_errors_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_parsing_errors_free(errors: *mut dnv_vista_sdk_parsing_errors_t);
    pub(crate) fn dnv_vista_sdk_parsing_errors_count(
        errors: *const dnv_vista_sdk_parsing_errors_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_parsing_errors_has_errors(
        errors: *const dnv_vista_sdk_parsing_errors_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_parsing_errors_has_error_type(
        errors: *const dnv_vista_sdk_parsing_errors_t,
        r#type: *const c_char,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_parsing_errors_type_at(
        errors: *const dnv_vista_sdk_parsing_errors_t,
        index: usize,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_parsing_errors_message_at(
        errors: *const dnv_vista_sdk_parsing_errors_t,
        index: usize,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_parsing_errors_to_string(
        errors: *const dnv_vista_sdk_parsing_errors_t,
    ) -> *mut c_char;
}
