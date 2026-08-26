use std::ffi::{CStr, CString};
use std::ptr::NonNull;

use crate::core::local_id::LocalId;
use crate::ffi::core::common::dnv_vista_sdk_string_free;
use crate::ffi::transport::timeseries::data_channel_id as ffi;

/// Polymorphic data channel identifier for ISO 19848 time series messages.
///
/// Holds either a [`LocalId`] (full VIS identifier) or a `ShortId` (plain string),
/// never both. Constructed via [`TsdChannelId::from_string`].
pub struct TsdChannelId(NonNull<ffi::dnv_vista_sdk_tsd_channel_id_t>);

impl TsdChannelId {
    /// Parse a `TsdChannelId` from its string representation.
    ///
    /// Tries to parse `value` as a `LocalId`. On failure, stores the string verbatim as
    /// a `ShortId`. Returns `None` only when `value` is empty.
    pub fn from_string(value: &str) -> Option<Self> {
        let c = CString::new(value).expect("value contains NUL byte");
        // SAFETY: c is a valid NUL-terminated C string; returns an owned pointer or NULL.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_channel_id_from_string(c.as_ptr()) };
        NonNull::new(ptr).map(Self)
    }

    /// Returns `true` if this holds a `LocalId`.
    pub fn is_local_id(&self) -> bool {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        (unsafe { ffi::dnv_vista_sdk_tsd_channel_id_is_local_id(self.0.as_ptr()) }) != 0
    }

    /// Returns `true` if this holds a `ShortId`.
    pub fn is_short_id(&self) -> bool {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        (unsafe { ffi::dnv_vista_sdk_tsd_channel_id_is_short_id(self.0.as_ptr()) }) != 0
    }

    /// Returns the `LocalId` if this holds one, or `None` if it holds a `ShortId`.
    pub fn local_id(&self) -> Option<&LocalId> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_channel_id_local_id(self.0.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(LocalId::from_ptr(ptr))
        }
    }

    /// Returns the `ShortId` if this holds one, or `None` if it holds a `LocalId`.
    pub fn short_id(&self) -> Option<&str> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_channel_id_short_id(self.0.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(
                // SAFETY: ptr is non-null (checked above) and owned by `self` for its lifetime.
                unsafe { CStr::from_ptr(ptr) }
                    .to_str()
                    .expect("invalid UTF-8 in short_id"),
            )
        }
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_tsd_channel_id_t {
        self.0.as_ptr()
    }

    pub(crate) fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_tsd_channel_id_t) -> &'a Self {
        // SAFETY: ptr is non-null and `TsdChannelId` is a `NonNull` newtype over the FFI type.
        unsafe { &*(ptr as *const Self) }
    }

    /// Convert to string representation (`LocalId.toString()` or the `ShortId` string).
    pub fn to_string_repr(&self) -> String {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`; returns an owned pointer.
        let raw = unsafe { ffi::dnv_vista_sdk_tsd_channel_id_to_string(self.0.as_ptr()) };
        assert!(
            !raw.is_null(),
            "to_string returned null for non-null TsdChannelId"
        );
        // SAFETY: raw is non-null and owned until freed below.
        let s = unsafe { CStr::from_ptr(raw) }
            .to_str()
            .expect("invalid UTF-8")
            .to_string();
        // SAFETY: raw was allocated by the library and is freed exactly once.
        unsafe { dnv_vista_sdk_string_free(raw) };
        s
    }
}

impl PartialEq for TsdChannelId {
    fn eq(&self, other: &Self) -> bool {
        // SAFETY: self.0 and other.0 are non-null and valid for the lifetime of `&self`/`&other`.
        (unsafe { ffi::dnv_vista_sdk_tsd_channel_id_equals(self.0.as_ptr(), other.0.as_ptr()) })
            != 0
    }
}

impl Eq for TsdChannelId {}

impl std::fmt::Display for TsdChannelId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.to_string_repr())
    }
}

impl std::fmt::Debug for TsdChannelId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "TsdChannelId({})", self.to_string_repr())
    }
}

impl Drop for TsdChannelId {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `TsdChannelId` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_tsd_channel_id_free(self.0.as_ptr()) }
    }
}
