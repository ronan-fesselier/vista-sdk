use std::ffi::{c_char, c_int};

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_imo_number_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_imo_number_is_valid(imo_number: c_int) -> c_int;
    pub(crate) fn dnv_vista_sdk_imo_number_create(value: c_int) -> *mut dnv_vista_sdk_imo_number_t;
    pub(crate) fn dnv_vista_sdk_imo_number_from_string(
        value: *const c_char,
    ) -> *mut dnv_vista_sdk_imo_number_t;
    pub(crate) fn dnv_vista_sdk_imo_number_create_from_string(
        value: *const c_char,
    ) -> *mut dnv_vista_sdk_imo_number_t;
    pub(crate) fn dnv_vista_sdk_imo_number_free(imo_number: *mut dnv_vista_sdk_imo_number_t);
    pub(crate) fn dnv_vista_sdk_imo_number_value(
        imo_number: *const dnv_vista_sdk_imo_number_t,
    ) -> c_int;
}
