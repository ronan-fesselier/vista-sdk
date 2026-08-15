use std::ffi::CStr;

use crate::ffi::core::location as ffi_location;
use crate::ffi::core::relative_location as ffi;

/// A relative location entry: a single-character code with a name, optional
/// definition, and canonical [`Location`](crate::core::location::Location) representation.
///
/// Borrowed from a [`crate::core::locations::Locations`] instance, valid as long as it is.
#[repr(transparent)]
pub struct RelativeLocation(ffi::dnv_vista_sdk_relative_location_t);

impl RelativeLocation {
    pub(crate) fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_relative_location_t) -> &'a Self {
        assert!(!ptr.is_null(), "relative location pointer must not be NULL");
        unsafe { &*(ptr as *const RelativeLocation) }
    }

    fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_relative_location_t {
        self as *const Self as *const ffi::dnv_vista_sdk_relative_location_t
    }

    /// Returns the single-character location code.
    pub fn code(&self) -> char {
        let c = unsafe { ffi::dnv_vista_sdk_relative_location_code(self.as_ffi_ptr()) };
        c as u8 as char
    }

    /// Returns the human-readable name.
    pub fn name(&self) -> &str {
        let ptr = unsafe { ffi::dnv_vista_sdk_relative_location_name(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_relative_location_name returned NULL"
        );
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in relative location name")
    }

    /// Returns the optional definition text, or `None` if not set.
    pub fn definition(&self) -> Option<&str> {
        let ptr = unsafe { ffi::dnv_vista_sdk_relative_location_definition(self.as_ffi_ptr()) };
        if ptr.is_null() {
            return None;
        }
        Some(
            unsafe { CStr::from_ptr(ptr) }
                .to_str()
                .expect("invalid UTF-8 in relative location definition"),
        )
    }

    /// Returns the canonical location string.
    ///
    /// This is the same underlying representation as [`crate::core::location::Location::value`],
    /// but borrowed from this `RelativeLocation` rather than an owned handle.
    pub fn location_value(&self) -> &str {
        let ptr = unsafe { ffi::dnv_vista_sdk_relative_location_location(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_relative_location_location returned NULL"
        );
        let value_ptr = unsafe { ffi_location::dnv_vista_sdk_location_value(ptr) };
        assert!(
            !value_ptr.is_null(),
            "dnv_vista_sdk_location_value returned NULL"
        );
        unsafe { CStr::from_ptr(value_ptr) }
            .to_str()
            .expect("invalid UTF-8 in location value")
    }
}
