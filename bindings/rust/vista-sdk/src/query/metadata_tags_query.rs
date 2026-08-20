use crate::core::local_id::LocalId;
use crate::ffi::query::metadata_tags_query as ffi;
use crate::query::metadata_tags_query_builder::MetadataTagsQueryBuilder;

/// Immutable query for matching LocalId metadata tags.
///
/// Created via [`crate::query::metadata_tags_query_builder::OwnedMetadataTagsQueryBuilder`].
pub struct MetadataTagsQuery(pub(crate) *mut ffi::dnv_vista_sdk_metadata_tags_query_t);

impl Drop for MetadataTagsQuery {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `MetadataTagsQuery` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_metadata_tags_query_free(self.0) }
    }
}

impl MetadataTagsQuery {
    pub(crate) fn from_owned_ptr(ptr: *mut ffi::dnv_vista_sdk_metadata_tags_query_t) -> Self {
        MetadataTagsQuery(ptr)
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_metadata_tags_query_t {
        self.0
    }

    /// Whether `local_id` satisfies all constraints in this query.
    pub fn r#match(&self, local_id: &LocalId) -> bool {
        // SAFETY: self.0 and local_id are non-null and valid for the lifetime of `&self`/`&local_id`.
        unsafe { ffi::dnv_vista_sdk_metadata_tags_query_match(self.0, local_id.as_ffi_ptr()) != 0 }
    }

    /// Builder state underlying this query.
    pub fn builder(&self) -> &MetadataTagsQueryBuilder {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_metadata_tags_query_builder(self.0) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_metadata_tags_query_builder returned NULL"
        );
        MetadataTagsQueryBuilder::from_ptr(ptr)
    }
}
