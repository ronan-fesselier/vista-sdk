use std::ptr::NonNull;

use crate::core::local_id::LocalIdRef;
use crate::ffi::query::metadata_tags_query as ffi;
use crate::query::metadata_tags_query_builder::MetadataTagsQueryBuilderRef;

/// Immutable query for matching LocalIdRef metadata tags.
///
/// Created via [`crate::query::metadata_tags_query_builder::MetadataTagsQueryBuilder`].
pub struct MetadataTagsQuery(pub(crate) NonNull<ffi::dnv_vista_sdk_metadata_tags_query_t>);

impl Drop for MetadataTagsQuery {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `MetadataTagsQuery` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_metadata_tags_query_free(self.0.as_ptr()) }
    }
}

impl MetadataTagsQuery {
    pub(crate) fn from_owned_ptr(ptr: *mut ffi::dnv_vista_sdk_metadata_tags_query_t) -> Self {
        MetadataTagsQuery(NonNull::new(ptr).expect("metadata_tags_query pointer must not be NULL"))
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_metadata_tags_query_t {
        self.0.as_ptr()
    }

    /// Whether `local_id` satisfies all constraints in this query.
    pub fn r#match(&self, local_id: &LocalIdRef) -> bool {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe {
            ffi::dnv_vista_sdk_metadata_tags_query_match(self.0.as_ptr(), local_id.as_ffi_ptr())
                != 0
        }
    }

    /// Builder state underlying this query.
    pub fn builder(&self) -> &MetadataTagsQueryBuilderRef {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_metadata_tags_query_builder(self.0.as_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_metadata_tags_query_builder returned NULL"
        );
        // SAFETY: ptr is non-null (checked above) and valid for `self`'s lifetime.
        unsafe { MetadataTagsQueryBuilderRef::from_ptr(ptr) }
    }
}

// SAFETY: MetadataTagsQuery wraps a NonNull pointer to a heap-allocated C++ object with no shared
// mutable aliasing outside this type. Transferring or sharing ownership across threads is sound.
unsafe impl Send for MetadataTagsQuery {}
unsafe impl Sync for MetadataTagsQuery {}
