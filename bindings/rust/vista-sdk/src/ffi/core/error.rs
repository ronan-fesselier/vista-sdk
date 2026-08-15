use std::ffi::c_char;

#[allow(non_camel_case_types, dead_code)]
#[repr(u32)]
pub(crate) enum dnv_vista_sdk_error_kind_t {
    None = 0,
    InvalidArgument,
    OutOfRange,
    Domain,
    Overflow,
    Runtime,
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_last_error_message() -> *const c_char;
    pub(crate) fn dnv_vista_sdk_last_error_kind() -> dnv_vista_sdk_error_kind_t;
    pub(crate) fn dnv_vista_sdk_clear_error();
}
