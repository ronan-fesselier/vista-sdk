use crate::core::gmod_path::GmodPath;
use crate::ffi::query::gmod_path_query as ffi;

/// Immutable query for matching [`GmodPath`] instances.
///
/// Created via [`crate::query::gmod_path_query_builder::OwnedGmodPathQueryBuilder`].
pub struct GmodPathQuery(*mut ffi::dnv_vista_sdk_gmod_path_query_t);

impl Drop for GmodPathQuery {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_gmod_path_query_free(self.0) }
    }
}

impl GmodPathQuery {
    pub(crate) fn from_owned_ptr(ptr: *mut ffi::dnv_vista_sdk_gmod_path_query_t) -> Self {
        GmodPathQuery(ptr)
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_gmod_path_query_t {
        self.0
    }

    /// Whether `path` satisfies all constraints in this query.
    pub fn match_path(&self, path: &GmodPath) -> bool {
        unsafe { ffi::dnv_vista_sdk_gmod_path_query_match(self.0, path.as_ffi_ptr()) != 0 }
    }
}
