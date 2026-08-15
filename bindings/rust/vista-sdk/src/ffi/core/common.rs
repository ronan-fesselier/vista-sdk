use std::ffi::c_char;

extern "C" {
    pub(crate) fn dnv_vista_sdk_string_free(str: *mut c_char);
}
