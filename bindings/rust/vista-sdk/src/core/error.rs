use std::ffi::CStr;

use crate::ffi::core::error as ffi;

/// Category of error returned by the Vista SDK.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ErrorKind {
    /// No error.
    None,
    /// Invalid argument passed to a function.
    InvalidArgument,
    /// Requested version or element not available.
    OutOfRange,
    /// Domain constraint violated.
    Domain,
    /// Arithmetic overflow.
    Overflow,
    /// Runtime failure.
    Runtime,
}

fn from_ffi_kind(kind: ffi::dnv_vista_sdk_error_kind_t) -> ErrorKind {
    match kind {
        ffi::dnv_vista_sdk_error_kind_t::None => ErrorKind::None,
        ffi::dnv_vista_sdk_error_kind_t::InvalidArgument => ErrorKind::InvalidArgument,
        ffi::dnv_vista_sdk_error_kind_t::OutOfRange => ErrorKind::OutOfRange,
        ffi::dnv_vista_sdk_error_kind_t::Domain => ErrorKind::Domain,
        ffi::dnv_vista_sdk_error_kind_t::Overflow => ErrorKind::Overflow,
        ffi::dnv_vista_sdk_error_kind_t::Runtime => ErrorKind::Runtime,
    }
}

/// Error returned by Vista SDK operations that can fail.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct VistaError {
    /// Category of the error.
    pub kind: ErrorKind,
    /// Human-readable error message.
    pub message: String,
}

impl std::fmt::Display for VistaError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(&self.message)
    }
}

impl std::error::Error for VistaError {}

/// Returns the last error recorded by the SDK on the current thread.
pub fn last_error() -> VistaError {
    let kind = unsafe { ffi::dnv_vista_sdk_last_error_kind() };
    let ptr = unsafe { ffi::dnv_vista_sdk_last_error_message() };
    let message = if ptr.is_null() {
        String::new()
    } else {
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .unwrap_or("")
            .to_string()
    };
    VistaError {
        kind: from_ffi_kind(kind),
        message,
    }
}

/// Clears the last error recorded by the SDK on the current thread.
pub fn clear_error() {
    unsafe { ffi::dnv_vista_sdk_clear_error() }
}
