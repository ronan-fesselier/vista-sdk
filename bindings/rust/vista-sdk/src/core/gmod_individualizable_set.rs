use std::ffi::CStr;
use std::os::raw::c_int;

use crate::core::error::{clear_error, last_error, ErrorKind, VistaError};
use crate::core::gmod_node::GmodNode;
use crate::core::gmod_path::{GmodPath, GmodPathRef};
use crate::core::location::Location;
use crate::ffi::core::gmod_individualizable_set as ffi;

/// Contiguous sequence of nodes in a [`GmodPathRef`] that can be individualized with a location.
pub struct GmodIndividualizableSet(
    std::ptr::NonNull<ffi::dnv_vista_sdk_gmod_individualizable_set_t>,
);

impl Drop for GmodIndividualizableSet {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `GmodIndividualizableSet` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_gmod_individualizable_set_free(self.0.as_ptr()) }
    }
}

impl GmodIndividualizableSet {
    pub(crate) fn from_owned_ptr(ptr: *mut ffi::dnv_vista_sdk_gmod_individualizable_set_t) -> Self {
        GmodIndividualizableSet(
            std::ptr::NonNull::new(ptr)
                .expect("gmod individualizable set pointer must not be NULL"),
        )
    }

    /// Creates an individualizable set from `node_indices` within `source_path`. Returns `Err` if validation fails.
    pub fn create(node_indices: &[i32], source_path: &GmodPathRef) -> Result<Self, VistaError> {
        // SAFETY: node_indices.as_ptr() is valid for node_indices.len() elements, and
        // source_path is non-null.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_individualizable_set_create(
                node_indices.as_ptr(),
                node_indices.len(),
                source_path.as_ffi_ptr(),
            )
        };
        std::ptr::NonNull::new(ptr)
            .map(GmodIndividualizableSet)
            .ok_or_else(last_error)
    }

    /// Consumes this set and returns the individualized path. Returns `Err` if already consumed.
    pub fn build(self) -> Result<GmodPath, VistaError> {
        // SAFETY: self.0 is non-null and owned by `self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_individualizable_set_build(self.0.as_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(GmodPath::from_owned_ptr(ptr))
        }
    }

    /// Number of nodes in this set. Returns `Err` if the set has already been built.
    pub fn node_count(&self) -> Result<usize, VistaError> {
        clear_error();
        // SAFETY: self.0 is non-null and valid for the lifetime of `self`.
        let count =
            unsafe { ffi::dnv_vista_sdk_gmod_individualizable_set_node_count(self.0.as_ptr()) };
        let error = last_error();
        if error.kind != ErrorKind::None {
            Err(error)
        } else {
            Ok(count)
        }
    }

    /// Node at `index`. Returns `Err` if the set has already been built or `index` is out of bounds.
    pub fn node_at(&self, index: usize) -> Result<GmodNode, VistaError> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `self`.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_gmod_individualizable_set_node_at(self.0.as_ptr(), index) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(GmodNode::from_owned_ptr(ptr))
        }
    }

    /// Number of node indices in this set.
    pub fn index_count(&self) -> usize {
        // SAFETY: self.0 is non-null and valid for the lifetime of `self`.
        unsafe { ffi::dnv_vista_sdk_gmod_individualizable_set_index_count(self.0.as_ptr()) }
    }

    /// Node index at `position`, or `None` if out of bounds.
    pub fn index_at(&self, position: usize) -> Option<i32> {
        let mut out: c_int = 0;
        // SAFETY: self.0 is non-null, and `out` is a valid writable pointer to a local.
        let ok = unsafe {
            ffi::dnv_vista_sdk_gmod_individualizable_set_index_at(
                self.0.as_ptr(),
                position,
                &mut out,
            )
        };
        if ok != 0 {
            Some(out)
        } else {
            None
        }
    }

    /// Current location assigned to this set, or `Ok(None)` if none. Returns `Err` if already built.
    pub fn location(&self) -> Result<Option<Location>, VistaError> {
        clear_error();
        // SAFETY: self.0 is non-null and valid for the lifetime of `self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_individualizable_set_location(self.0.as_ptr()) };
        if let Some(loc) = std::ptr::NonNull::new(ptr).map(Location) {
            return Ok(Some(loc));
        }
        let error = last_error();
        if error.kind != ErrorKind::None {
            Err(error)
        } else {
            Ok(None)
        }
    }

    /// Leaf node codes joined by `'/'`. Returns `Err` if already built.
    pub fn to_string(&self) -> Result<String, VistaError> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `self`. Returns an owned pointer or NULL.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_gmod_individualizable_set_to_string(self.0.as_ptr()) };
        if ptr.is_null() {
            return Err(last_error());
        }
        // SAFETY: ptr is non-null (checked above) and owned until freed below.
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in gmod individualizable set string");
        let result = s.to_string();
        // SAFETY: ptr was allocated by the library and is freed exactly once.
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        Ok(result)
    }
}

// SAFETY: GmodIndividualizableSet owns its heap-allocated C++ object exclusively.
unsafe impl Send for GmodIndividualizableSet {}
unsafe impl Sync for GmodIndividualizableSet {}
