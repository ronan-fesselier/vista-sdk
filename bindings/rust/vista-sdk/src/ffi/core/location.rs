use std::ffi::c_char;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_location_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_location_free(location: *mut dnv_vista_sdk_location_t);
    pub(crate) fn dnv_vista_sdk_location_value(
        location: *const dnv_vista_sdk_location_t,
    ) -> *const c_char;
}
