use std::ffi::{CStr, CString};

use crate::ffi::core::common as ffi_common;
use crate::ffi::core::parsing_errors as ffi;

/// A single parsing error entry, borrowed from a [`ParsingErrors`] collection.
pub struct ErrorEntry<'a> {
    pub r#type: &'a str,
    pub message: &'a str,
}

/// Immutable collection of typed error messages from parsing operations.
pub struct ParsingErrors(pub(crate) *mut ffi::dnv_vista_sdk_parsing_errors_t);

impl Drop for ParsingErrors {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `ParsingErrors` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_parsing_errors_free(self.0) }
    }
}

impl ParsingErrors {
    /// Returns the number of errors.
    pub fn count(&self) -> usize {
        // SAFETY: self.0 is non-null and valid for the lifetime of `self`.
        unsafe { ffi::dnv_vista_sdk_parsing_errors_count(self.0) }
    }

    /// Returns `true` if the collection is non-empty.
    pub fn has_errors(&self) -> bool {
        // SAFETY: self.0 is non-null and valid for the lifetime of `self`.
        unsafe { ffi::dnv_vista_sdk_parsing_errors_has_errors(self.0) != 0 }
    }

    /// Returns `true` if at least one entry with the given type is present.
    pub fn has_error_type(&self, r#type: &str) -> bool {
        let c_type = CString::new(r#type).expect("error type contains a NUL byte");
        // SAFETY: self.0 is non-null, and c_type is a valid NUL-terminated C string.
        unsafe { ffi::dnv_vista_sdk_parsing_errors_has_error_type(self.0, c_type.as_ptr()) != 0 }
    }

    /// Returns the error entry at `index`, or `None` if out of range.
    pub fn get(&self, index: usize) -> Option<ErrorEntry<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `self`.
        let type_ptr = unsafe { ffi::dnv_vista_sdk_parsing_errors_type_at(self.0, index) };
        if type_ptr.is_null() {
            return None;
        }
        // SAFETY: self.0 is non-null and valid for the lifetime of `self`.
        let message_ptr = unsafe { ffi::dnv_vista_sdk_parsing_errors_message_at(self.0, index) };
        assert!(
            !message_ptr.is_null(),
            "dnv_vista_sdk_parsing_errors_message_at returned NULL"
        );

        // SAFETY: type_ptr is non-null (checked above) and owned by `self` for its lifetime.
        let r#type = unsafe { CStr::from_ptr(type_ptr) }
            .to_str()
            .expect("invalid UTF-8 in error type");
        // SAFETY: message_ptr is non-null (checked above) and owned by `self` for its lifetime.
        let message = unsafe { CStr::from_ptr(message_ptr) }
            .to_str()
            .expect("invalid UTF-8 in error message");
        Some(ErrorEntry { r#type, message })
    }

    /// Returns an iterator over the error entries, in the order errors were added.
    pub fn iter(&self) -> impl Iterator<Item = ErrorEntry<'_>> {
        (0..self.count()).map(move |i| self.get(i).expect("index within count() must be valid"))
    }
}

impl std::fmt::Display for ParsingErrors {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        // SAFETY: self.0 is non-null and valid for the lifetime of `self`; returns an owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_parsing_errors_to_string(self.0) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_parsing_errors_to_string returned NULL"
        );
        // SAFETY: ptr is non-null and owned until freed below.
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in parsing errors string");
        let result = f.write_str(s);
        // SAFETY: ptr was allocated by the library and is freed exactly once.
        unsafe { ffi_common::dnv_vista_sdk_string_free(ptr) };
        result
    }
}
