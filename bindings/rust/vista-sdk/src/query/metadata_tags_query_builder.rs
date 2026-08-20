use std::ffi::{c_int, CString};

use crate::core::codebook_name::{codebook_names, CodebookName};
use crate::core::local_id::LocalId;
use crate::core::metadata_tag::MetadataTag;
use crate::ffi::query::metadata_tags_query_builder as ffi;
use crate::query::metadata_tags_query::MetadataTagsQuery;

/// Fluent builder for [`MetadataTagsQuery`].
///
/// Borrowed - obtained via accessors like [`MetadataTagsQuery::builder`]. See
/// [`OwnedMetadataTagsQueryBuilder`] for the owned variant returned by parsing/building functions.
#[repr(transparent)]
pub struct MetadataTagsQueryBuilder(ffi::dnv_vista_sdk_metadata_tags_query_builder_t);

impl MetadataTagsQueryBuilder {
    pub(crate) fn from_ptr<'a>(
        ptr: *const ffi::dnv_vista_sdk_metadata_tags_query_builder_t,
    ) -> &'a Self {
        assert!(
            !ptr.is_null(),
            "metadata tags query builder pointer must not be NULL"
        );
        unsafe { &*(ptr as *const MetadataTagsQueryBuilder) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_metadata_tags_query_builder_t {
        self as *const Self as *const ffi::dnv_vista_sdk_metadata_tags_query_builder_t
    }

    /// Returns a new builder requiring that codebook slot `name` equals `value`.
    pub fn with_tag(&self, name: CodebookName, value: &str) -> OwnedMetadataTagsQueryBuilder {
        let c_value = CString::new(value).expect("value contains a NUL byte");
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
        OwnedMetadataTagsQueryBuilder(ptr)
    }

    /// Returns a new builder requiring that `tag`'s codebook slot equals `tag`'s value.
    pub fn with_metadata_tag(&self, tag: &MetadataTag) -> OwnedMetadataTagsQueryBuilder {
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
        OwnedMetadataTagsQueryBuilder(ptr)
    }

    /// Returns a new builder with the allow-other-tags flag set to `allow_others`.
    ///
    /// When `false`, a [`LocalId`] must carry exactly the tags specified.
    pub fn with_allow_other_tags(&self, allow_others: bool) -> OwnedMetadataTagsQueryBuilder {
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
        OwnedMetadataTagsQueryBuilder(ptr)
    }

    /// Constructs the immutable [`MetadataTagsQuery`].
    pub fn build(&self) -> MetadataTagsQuery {
        let ptr =
            unsafe { ffi::dnv_vista_sdk_metadata_tags_query_builder_build(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_metadata_tags_query_builder_build returned NULL"
        );
        MetadataTagsQuery::from_owned_ptr(ptr)
    }
}

/// An owned, independently-released [`MetadataTagsQueryBuilder`].
pub struct OwnedMetadataTagsQueryBuilder(
    pub(crate) *mut ffi::dnv_vista_sdk_metadata_tags_query_builder_t,
);

impl Drop for OwnedMetadataTagsQueryBuilder {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_metadata_tags_query_builder_free(self.0) }
    }
}

impl std::ops::Deref for OwnedMetadataTagsQueryBuilder {
    type Target = MetadataTagsQueryBuilder;

    fn deref(&self) -> &MetadataTagsQueryBuilder {
        MetadataTagsQueryBuilder::from_ptr(self.0)
    }
}

impl OwnedMetadataTagsQueryBuilder {
    /// Creates an empty builder. With default settings, the built query matches any [`LocalId`].
    pub fn create() -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_metadata_tags_query_builder_create() };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_metadata_tags_query_builder_create returned NULL"
        );
        OwnedMetadataTagsQueryBuilder(ptr)
    }

    /// Creates a builder pre-populated with all metadata tags from `local_id`.
    pub fn from_local_id(local_id: &LocalId, allow_other_tags: bool) -> Self {
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
        OwnedMetadataTagsQueryBuilder(ptr)
    }
}
