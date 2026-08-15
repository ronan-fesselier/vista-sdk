use std::ffi::CStr;

use crate::ffi::core::location as ffi;

/// A validated, canonical VIS location (e.g. `"1PS"`).
pub struct Location(pub(crate) std::ptr::NonNull<ffi::dnv_vista_sdk_location_t>);

impl Drop for Location {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `Location` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_location_free(self.0.as_ptr()) }
    }
}

impl Location {
    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_location_t {
        self.0.as_ptr()
    }

    /// Returns the canonical location string.
    pub fn value(&self) -> &str {
        // SAFETY: self.0 is non-null and valid for the lifetime of `self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_location_value(self.0.as_ptr()) };
        assert!(!ptr.is_null(), "dnv_vista_sdk_location_value returned NULL");
        // SAFETY: ptr is non-null and owned by `self` for its lifetime.
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in location value")
    }
}

impl std::fmt::Display for Location {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(self.value())
    }
}
