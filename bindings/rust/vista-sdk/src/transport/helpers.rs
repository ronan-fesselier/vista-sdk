use std::ffi::CStr;

use crate::ffi::core::common::dnv_vista_sdk_string_free;

pub(crate) unsafe fn ffi_string_to_owned(raw: *mut std::ffi::c_char) -> String {
    let s = CStr::from_ptr(raw).to_string_lossy().into_owned();
    dnv_vista_sdk_string_free(raw);
    s
}
