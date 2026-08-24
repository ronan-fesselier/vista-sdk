use std::ffi::{CStr, CString};

use crate::core::error::{last_error, VistaError};
use crate::ffi::core::common::dnv_vista_sdk_string_free;
use crate::ffi::transport::datachannel::data_channel as ffi;
use crate::transport::datachannel::data_channel::DataChannelListPackage;

/// Parses a [`DataChannelListPackage`] from a JSON string.
pub fn from_json(json: &str) -> Result<DataChannelListPackage, VistaError> {
    let c = CString::new(json).expect("json contains NUL byte");
    let ptr = unsafe { ffi::dnv_vista_sdk_dcl_list_package_from_json(c.as_ptr()) };
    if ptr.is_null() {
        Err(last_error())
    } else {
        Ok(unsafe { DataChannelListPackage::from_raw(ptr) })
    }
}

/// Serializes a [`DataChannelListPackage`] to a JSON string.
pub fn to_json(package: &DataChannelListPackage, pretty_print: bool) -> String {
    let raw = unsafe {
        ffi::dnv_vista_sdk_dcl_list_package_to_json(
            package.as_ptr(),
            pretty_print as std::ffi::c_int,
        )
    };
    assert!(!raw.is_null(), "to_json returned null for non-null package");
    let s = unsafe { CStr::from_ptr(raw) }
        .to_str()
        .expect("invalid UTF-8 in JSON output")
        .to_string();
    unsafe { dnv_vista_sdk_string_free(raw) };
    s
}
