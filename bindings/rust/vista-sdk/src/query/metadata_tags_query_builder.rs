use std::ffi::{c_int, CString};

use crate::core::codebook_name::{codebook_names, CodebookName};
use crate::core::local_id::LocalIdRef;
use crate::core::metadata_tag::MetadataTagRef;
use crate::ffi::query::metadata_tags_query_builder as ffi;
use crate::query::metadata_tags_query::MetadataTagsQuery;

/// Fluent builder for [`MetadataTagsQuery`].
///
/// Borrowed - obtained via accessors like [`MetadataTagsQuery::builder`]. See
/// [`MetadataTagsQueryBuilder`] for the owned variant returned by parsing/building functions.
#[repr(transparent)]
pub struct MetadataTagsQueryBuilderRef(ffi::dnv_vista_sdk_metadata_tags_query_builder_t);

impl MetadataTagsQueryBuilderRef {
    pub(crate) fn from_ptr<'a>(
        ptr: *const ffi::dnv_vista_sdk_metadata_tags_query_builder_t,
    ) -> &'a Self {
        assert!(
            !ptr.is_null(),
            "metadata tags query builder pointer must not be NULL"
        );
        // SAFETY: ptr is non-null and `MetadataTagsQueryBuilderRef` is `#[repr(transparent)]` over the FFI type.
        unsafe { &*(ptr as *const MetadataTagsQueryBuilderRef) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_metadata_tags_query_builder_t {
        self as *const Self as *const ffi::dnv_vista_sdk_metadata_tags_query_builder_t
    }

    /// Returns a new builder requiring that codebook slot `name` equals `value`.
    pub fn with_tag(&self, name: CodebookName, value: &str) -> MetadataTagsQueryBuilder {
        let c_value = CString::new(value).expect("value contains a NUL byte");
        // SAFETY: self is non-null, and c_value is a valid NUL-terminated C string.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_metadata_tags_query_builder_with_tag(
                self.as_ffi_ptr(),
                codebook_names::to_ffi(name),
                c_value.as_ptr(),
            )
        };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_metadata_tags_query_builder_with_tag returned NULL"
        );
        MetadataTagsQueryBuilder(ptr)
    }

    /// Returns a new builder requiring that `tag`'s codebook slot equals `tag`'s value.
    pub fn with_metadata_tag(&self, tag: &MetadataTagRef) -> MetadataTagsQueryBuilder {
        // SAFETY: self and tag are non-null and valid for the call's duration.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_metadata_tags_query_builder_with_metadata_tag(
                self.as_ffi_ptr(),
                tag.as_ffi_ptr(),
            )
        };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_metadata_tags_query_builder_with_metadata_tag returned NULL"
        );
        MetadataTagsQueryBuilder(ptr)
    }

    /// Returns a new builder with the allow-other-tags flag set to `allow_others`.
    ///
    /// When `false`, a [`LocalIdRef`] must carry exactly the tags specified.
    pub fn with_allow_other_tags(&self, allow_others: bool) -> MetadataTagsQueryBuilder {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_metadata_tags_query_builder_with_allow_other_tags(
                self.as_ffi_ptr(),
                allow_others as c_int,
            )
        };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_metadata_tags_query_builder_with_allow_other_tags returned NULL"
        );
        MetadataTagsQueryBuilder(ptr)
    }

    /// Constructs the immutable [`MetadataTagsQuery`].
    pub fn build(&self) -> MetadataTagsQuery {
        // SAFETY: self is non-null and valid for the lifetime of `&self`. Returns an owned pointer.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_metadata_tags_query_builder_build(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_metadata_tags_query_builder_build returned NULL"
        );
        MetadataTagsQuery::from_owned_ptr(ptr)
    }
}

/// An owned, independently-released [`MetadataTagsQueryBuilderRef`].
pub struct MetadataTagsQueryBuilder(
    pub(crate) *mut ffi::dnv_vista_sdk_metadata_tags_query_builder_t,
);

impl Drop for MetadataTagsQueryBuilder {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `MetadataTagsQueryBuilder` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_metadata_tags_query_builder_free(self.0) }
    }
}

impl std::ops::Deref for MetadataTagsQueryBuilder {
    type Target = MetadataTagsQueryBuilderRef;

    fn deref(&self) -> &MetadataTagsQueryBuilderRef {
        MetadataTagsQueryBuilderRef::from_ptr(self.0)
    }
}

impl MetadataTagsQueryBuilder {
    /// Creates an empty builder. With default settings, the built query matches any [`LocalIdRef`].
    pub fn create() -> Self {
        // SAFETY: no arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_metadata_tags_query_builder_create() };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_metadata_tags_query_builder_create returned NULL"
        );
        MetadataTagsQueryBuilder(ptr)
    }

    /// Creates a builder pre-populated with all metadata tags from `local_id`.
    pub fn from_local_id(local_id: &LocalIdRef, allow_other_tags: bool) -> Self {
        // SAFETY: local_id is non-null and valid for the call's duration. Returns a non-null owned pointer.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_metadata_tags_query_builder_from(
                local_id.as_ffi_ptr(),
                allow_other_tags as c_int,
            )
        };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_metadata_tags_query_builder_from returned NULL"
        );
        MetadataTagsQueryBuilder(ptr)
    }
}
