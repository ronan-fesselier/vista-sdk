use std::ffi::CStr;
use std::os::raw::c_int;

use crate::core::error::{clear_error, last_error, ErrorKind, VistaError};
use crate::core::gmod_node::OwnedGmodNode;
use crate::core::gmod_path::{GmodPath, OwnedGmodPath};
use crate::core::location::Location;
use crate::ffi::core::gmod_individualizable_set as ffi;

/// Contiguous sequence of nodes in a [`GmodPath`] that can be individualized with a location.
pub struct GmodIndividualizableSet(*mut ffi::dnv_vista_sdk_gmod_individualizable_set_t);

impl Drop for GmodIndividualizableSet {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_gmod_individualizable_set_free(self.0) }
    }
}

impl GmodIndividualizableSet {
    pub(crate) fn from_owned_ptr(ptr: *mut ffi::dnv_vista_sdk_gmod_individualizable_set_t) -> Self {
        GmodIndividualizableSet(ptr)
    }

    /// Creates an individualizable set from `node_indices` within `source_path`. Returns `Err` if validation fails.
    pub fn create(node_indices: &[i32], source_path: &GmodPath) -> Result<Self, VistaError> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_individualizable_set_create(
                node_indices.as_ptr(),
                node_indices.len(),
                source_path.as_ffi_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(GmodIndividualizableSet(ptr))
        }
    }

    /// Consumes this set and returns the individualized path. Returns `Err` if already consumed.
    pub fn build(self) -> Result<OwnedGmodPath, VistaError> {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_individualizable_set_build(self.0) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedGmodPath::from_owned_ptr(ptr))
        }
    }

    /// Number of nodes in this set. Returns `Err` if the set has already been built.
    pub fn node_count(&self) -> Result<usize, VistaError> {
        clear_error();
        let count = unsafe { ffi::dnv_vista_sdk_gmod_individualizable_set_node_count(self.0) };
        let error = last_error();
        if error.kind != ErrorKind::None {
            Err(error)
        } else {
            Ok(count)
        }
    }

    /// Node at `index`. Returns `Err` if the set has already been built or `index` is out of bounds.
    pub fn node_at(&self, index: usize) -> Result<OwnedGmodNode, VistaError> {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_individualizable_set_node_at(self.0, index) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedGmodNode::with_drop(
                ptr,
                ffi::dnv_vista_sdk_gmod_individualizable_set_node_free,
            ))
        }
    }

    /// Number of node indices in this set.
    pub fn index_count(&self) -> usize {
        unsafe { ffi::dnv_vista_sdk_gmod_individualizable_set_index_count(self.0) }
    }

    /// Node index at `position`, or `None` if out of bounds.
    pub fn index_at(&self, position: usize) -> Option<i32> {
        let mut out: c_int = 0;
        let ok = unsafe {
            ffi::dnv_vista_sdk_gmod_individualizable_set_index_at(self.0, position, &mut out)
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
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_individualizable_set_location(self.0) };
        if !ptr.is_null() {
            return Ok(Some(Location(ptr)));
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
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_individualizable_set_to_string(self.0) };
        if ptr.is_null() {
            return Err(last_error());
        }
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in gmod individualizable set string");
        let result = s.to_string();
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        Ok(result)
    }
}
