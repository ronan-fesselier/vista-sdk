use std::ffi::c_char;

use super::location::dnv_vista_sdk_location_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_relative_location_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_relative_location_code(
        relative_location: *const dnv_vista_sdk_relative_location_t,
    ) -> c_char;
    pub(crate) fn dnv_vista_sdk_relative_location_name(
        relative_location: *const dnv_vista_sdk_relative_location_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_relative_location_definition(
        relative_location: *const dnv_vista_sdk_relative_location_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_relative_location_location(
        relative_location: *const dnv_vista_sdk_relative_location_t,
    ) -> *const dnv_vista_sdk_location_t;
}
