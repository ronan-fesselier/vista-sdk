use std::ptr::NonNull;

use crate::core::gmod_path::GmodPathRef;
use crate::ffi::query::gmod_path_query as ffi;

/// Immutable query for matching [`GmodPathRef`] instances.
///
/// Created via [`crate::query::gmod_path_query_builder::GmodPathQueryBuilder`].
pub struct GmodPathQuery(NonNull<ffi::dnv_vista_sdk_gmod_path_query_t>);

impl Drop for GmodPathQuery {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `GmodPathQuery` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_gmod_path_query_free(self.0.as_ptr()) }
    }
}

impl GmodPathQuery {
    pub(crate) fn from_owned_ptr(ptr: *mut ffi::dnv_vista_sdk_gmod_path_query_t) -> Self {
        GmodPathQuery(NonNull::new(ptr).expect("gmod_path_query pointer must not be NULL"))
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_gmod_path_query_t {
        self.0.as_ptr()
    }

    /// Whether `path` satisfies all constraints in this query.
    pub fn match_path(&self, path: &GmodPathRef) -> bool {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_gmod_path_query_match(self.0.as_ptr(), path.as_ffi_ptr()) != 0 }
    }
}

// SAFETY: GmodPathQuery wraps a NonNull pointer to a heap-allocated C++ object with no shared
// mutable aliasing outside this type. Transferring or sharing ownership across threads is sound.
unsafe impl Send for GmodPathQuery {}
unsafe impl Sync for GmodPathQuery {}
