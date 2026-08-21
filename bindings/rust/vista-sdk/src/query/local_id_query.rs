use std::ffi::CString;

use crate::core::local_id::LocalId;
use crate::ffi::query::local_id_query as ffi;

/// Immutable query for matching [`LocalId`] instances.
///
/// Created via [`crate::query::local_id_query_builder::OwnedLocalIdQueryBuilder`].
pub struct LocalIdQuery(*mut ffi::dnv_vista_sdk_local_id_query_t);

impl Drop for LocalIdQuery {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_local_id_query_free(self.0) }
    }
}

impl LocalIdQuery {
    pub(crate) fn from_owned_ptr(ptr: *mut ffi::dnv_vista_sdk_local_id_query_t) -> Self {
        LocalIdQuery(ptr)
    }

    /// Whether `local_id` satisfies all constraints in this query.
    pub fn match_local_id(&self, local_id: &LocalId) -> bool {
        unsafe { ffi::dnv_vista_sdk_local_id_query_match(self.0, local_id.as_ffi_ptr()) != 0 }
    }

    /// Whether the given LocalId string satisfies all constraints in this query.
    pub fn match_str(&self, local_id_str: &str) -> bool {
        let c_str = CString::new(local_id_str).expect("local_id_str contains a NUL byte");
        unsafe { ffi::dnv_vista_sdk_local_id_query_match_string(self.0, c_str.as_ptr()) != 0 }
    }
}
