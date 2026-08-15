use std::ffi::CStr;

use crate::core::codebook_name::{codebook_names, CodebookName};
use crate::ffi::core::common as ffi_common;
use crate::ffi::core::metadata_tag as ffi;

/// A metadata tag combining a codebook name and a string value.
///
/// Tags appear as `"prefix-value"` (standard) or `"prefix~value"` (custom)
/// in VIS Local ID string format.
///
/// Borrowed - obtained via accessors like [`crate::core::local_id::LocalId::metadata_tag`].
/// See [`OwnedMetadataTag`] for the owned variant returned by [`crate::core::codebook::Codebook::create_tag`].
#[repr(transparent)]
pub struct MetadataTag(ffi::dnv_vista_sdk_metadata_tag_t);

impl MetadataTag {
    pub(crate) fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_metadata_tag_t) -> &'a Self {
        assert!(!ptr.is_null(), "metadata tag pointer must not be NULL");
        unsafe { &*(ptr as *const MetadataTag) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_metadata_tag_t {
        self as *const Self as *const ffi::dnv_vista_sdk_metadata_tag_t
    }

    /// Returns the codebook this tag belongs to.
    pub fn name(&self) -> CodebookName {
        let raw = unsafe { ffi::dnv_vista_sdk_metadata_tag_name(self.as_ffi_ptr()) };
        codebook_names::from_ffi(raw)
    }

    /// Returns the tag value.
    pub fn value(&self) -> &str {
        let ptr = unsafe { ffi::dnv_vista_sdk_metadata_tag_value(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_metadata_tag_value returned NULL"
        );
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in metadata tag value")
    }

    /// Returns the prefix separator character: `'-'` for standard tags, `'~'` for custom tags.
    pub fn prefix(&self) -> char {
        let c = unsafe { ffi::dnv_vista_sdk_metadata_tag_prefix(self.as_ffi_ptr()) };
        c as u8 as char
    }

    /// Returns `true` if this is a custom tag (`~` separator), `false` if standard (`-` separator).
    pub fn is_custom(&self) -> bool {
        unsafe { ffi::dnv_vista_sdk_metadata_tag_is_custom(self.as_ffi_ptr()) != 0 }
    }
}

impl std::fmt::Display for MetadataTag {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let ptr = unsafe { ffi::dnv_vista_sdk_metadata_tag_to_string(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_metadata_tag_to_string returned NULL"
        );
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in metadata tag string");
        let result = f.write_str(s);
        unsafe { ffi_common::dnv_vista_sdk_string_free(ptr) };
        result
    }
}

/// An owned, independently-released [`MetadataTag`].
///
/// Returned by [`crate::core::codebook::Codebook::create_tag`] and other APIs
/// that hand back a new tag value rather than a reference into existing state.
pub struct OwnedMetadataTag(pub(crate) *mut ffi::dnv_vista_sdk_metadata_tag_t);

impl Drop for OwnedMetadataTag {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_metadata_tag_free(self.0) }
    }
}

impl std::ops::Deref for OwnedMetadataTag {
    type Target = MetadataTag;

    fn deref(&self) -> &MetadataTag {
        MetadataTag::from_ptr(self.0)
    }
}

impl std::fmt::Display for OwnedMetadataTag {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        std::fmt::Display::fmt(&**self, f)
    }
}
