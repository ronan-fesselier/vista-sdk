use std::ffi::CStr;

use crate::core::codebook_name::{codebook_names, CodebookName};
use crate::ffi::core::common as ffi_common;
use crate::ffi::core::metadata_tag as ffi;

/// A metadata tag combining a codebook name and a string value.
///
/// Tags appear as `"prefix-value"` (standard) or `"prefix~value"` (custom)
/// in VIS Local ID string format.
///
/// Borrowed - obtained via accessors like [`crate::core::local_id::LocalIdRef::metadata_tag`].
/// See [`MetadataTag`] for the owned variant returned by [`crate::core::codebook::Codebook::create_tag`].
#[repr(transparent)]
pub struct MetadataTagRef(ffi::dnv_vista_sdk_metadata_tag_t);

impl MetadataTagRef {
    /// # Safety
    /// `ptr` must be non-null and valid for `'a`.
    pub(crate) unsafe fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_metadata_tag_t) -> &'a Self {
        // SAFETY: caller guarantees ptr is non-null and valid. `MetadataTagRef` is `#[repr(transparent)]`.
        unsafe { &*(ptr as *const MetadataTagRef) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_metadata_tag_t {
        self as *const Self as *const ffi::dnv_vista_sdk_metadata_tag_t
    }

    /// Returns the codebook this tag belongs to.
    pub fn name(&self) -> CodebookName {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let raw = unsafe { ffi::dnv_vista_sdk_metadata_tag_name(self.as_ffi_ptr()) };
        codebook_names::from_ffi(raw)
    }

    /// Returns the tag value.
    pub fn value(&self) -> &str {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_metadata_tag_value(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_metadata_tag_value returned NULL"
        );
        // SAFETY: ptr is non-null and owned by `self` for its lifetime.
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in metadata tag value")
    }

    /// Returns the prefix separator character: `'-'` for standard tags, `'~'` for custom tags.
    pub fn prefix(&self) -> char {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let c = unsafe { ffi::dnv_vista_sdk_metadata_tag_prefix(self.as_ffi_ptr()) };
        c as u8 as char
    }

    /// Returns `true` if this is a custom tag (`~` separator), `false` if standard (`-` separator).
    pub fn is_custom(&self) -> bool {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_metadata_tag_is_custom(self.as_ffi_ptr()) != 0 }
    }
}

impl std::fmt::Display for MetadataTagRef {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        // SAFETY: self is non-null and valid for the lifetime of `&self`. Returns an owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_metadata_tag_to_string(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_metadata_tag_to_string returned NULL"
        );
        // SAFETY: ptr is non-null and allocated by the library; invalid UTF-8 is replaced lossily.
        unsafe { ffi_common::display_owned_cstr(ptr, f) }
    }
}

/// An owned, independently-released [`MetadataTagRef`].
///
/// Returned by [`crate::core::codebook::Codebook::create_tag`] and other APIs
/// that hand back a new tag value rather than a reference into existing state.
pub struct MetadataTag(pub(crate) std::ptr::NonNull<ffi::dnv_vista_sdk_metadata_tag_t>);

impl Drop for MetadataTag {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `MetadataTag` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_metadata_tag_free(self.0.as_ptr()) }
    }
}

impl std::ops::Deref for MetadataTag {
    type Target = MetadataTagRef;

    fn deref(&self) -> &MetadataTagRef {
        // SAFETY: self.0 is NonNull, so as_ptr() is always non-null and valid for `self`'s lifetime.
        unsafe { MetadataTagRef::from_ptr(self.0.as_ptr()) }
    }
}

impl std::fmt::Display for MetadataTag {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        std::fmt::Display::fmt(&**self, f)
    }
}

// SAFETY: MetadataTagRef is a borrowed view (`repr(transparent)`). Sync only, not Send.
unsafe impl Sync for MetadataTagRef {}

// SAFETY: MetadataTag owns its heap-allocated C++ object exclusively.
unsafe impl Send for MetadataTag {}
unsafe impl Sync for MetadataTag {}
