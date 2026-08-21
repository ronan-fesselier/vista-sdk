use std::ffi::CString;
use std::ptr::NonNull;

use crate::core::local_id::LocalIdRef;
use crate::ffi::query::local_id_query as ffi;

/// Immutable query for matching [`LocalIdRef`] instances.
///
/// Created via [`crate::query::local_id_query_builder::LocalIdQueryBuilder`].
pub struct LocalIdQuery(NonNull<ffi::dnv_vista_sdk_local_id_query_t>);

impl Drop for LocalIdQuery {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `LocalIdQuery` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_local_id_query_free(self.0.as_ptr()) }
    }
}

impl LocalIdQuery {
    pub(crate) fn from_owned_ptr(ptr: *mut ffi::dnv_vista_sdk_local_id_query_t) -> Self {
        LocalIdQuery(NonNull::new(ptr).expect("local_id_query pointer must not be NULL"))
    }

    /// Whether `local_id` satisfies all constraints in this query.
    pub fn match_local_id(&self, local_id: &LocalIdRef) -> bool {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe {
            ffi::dnv_vista_sdk_local_id_query_match(self.0.as_ptr(), local_id.as_ffi_ptr()) != 0
        }
    }

    /// Whether the given LocalIdRef string satisfies all constraints in this query.
    pub fn match_str(&self, local_id_str: &str) -> bool {
        let c_str = CString::new(local_id_str).expect("local_id_str contains a NUL byte");
        // SAFETY: self.0 is non-null, and c_str is a valid NUL-terminated C string.
        unsafe {
            ffi::dnv_vista_sdk_local_id_query_match_string(self.0.as_ptr(), c_str.as_ptr()) != 0
        }
    }
}

// SAFETY: LocalIdQuery wraps a NonNull pointer to a heap-allocated C++ object with no shared
// mutable aliasing outside this type. Transferring or sharing ownership across threads is sound.
unsafe impl Send for LocalIdQuery {}
unsafe impl Sync for LocalIdQuery {}
