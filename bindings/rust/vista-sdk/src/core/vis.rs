use std::ffi::{CStr, CString};
use std::str::FromStr;

use crate::core::codebooks::Codebooks;
use crate::core::error::{last_error, VistaError};
use crate::core::gmod::Gmod;
use crate::core::gmod_node::{GmodNode, GmodNodeRef};
use crate::core::gmod_path::{GmodPath, GmodPathRef};
use crate::core::local_id::{LocalId, LocalIdRef};
use crate::core::local_id_builder::{LocalIdBuilder, LocalIdBuilderRef};
use crate::core::locations::Locations;
use crate::core::vis_version::VisVersion;
use crate::ffi::core::vis as ffi;

/// Central entry point for the Vista SDK.
///
/// Wraps a borrowed pointer to the C++ singleton, valid for the lifetime of the program.
pub struct Vis(*const ffi::dnv_vista_sdk_vis_t);

// SAFETY: the wrapped pointer targets a C++ singleton with a lifetime that is `'static`
// for the running program. All methods only read through it, so sharing across threads
// is sound as long as the underlying VIS singleton is itself thread-safe (it is: it is
// lazily initialized once via C++11 static-local magic statics, and every method below
// is a read-only query into version-scoped, immutable data).
unsafe impl Send for Vis {}
unsafe impl Sync for Vis {}

impl Vis {
    /// Returns the VIS singleton instance.
    pub fn instance() -> Vis {
        // SAFETY: no arguments. Returns a non-null pointer valid for the program's lifetime.
        Vis(unsafe { ffi::dnv_vista_sdk_vis_instance() })
    }

    /// Returns all available VIS versions, in ascending order.
    pub fn versions(&self) -> Vec<VisVersion> {
        // SAFETY: self.0 is non-null and valid for the program's lifetime.
        let count = unsafe { ffi::dnv_vista_sdk_vis_version_count(self.0) };
        (0..count)
            .filter_map(|i| {
                // SAFETY: self.0 is non-null and valid for the program's lifetime.
                let ptr = unsafe { ffi::dnv_vista_sdk_vis_version_at(self.0, i) };
                if ptr.is_null() {
                    return None;
                }
                // SAFETY: ptr is non-null and owned by the library for the program's lifetime.
                let s = unsafe { CStr::from_ptr(ptr) }.to_str().ok()?;
                VisVersion::from_str(s).ok()
            })
            .collect()
    }

    /// Returns the latest VIS version.
    pub fn latest(&self) -> VisVersion {
        // SAFETY: self.0 is non-null and valid for the program's lifetime.
        let ptr = unsafe { ffi::dnv_vista_sdk_vis_latest(self.0) };
        assert!(!ptr.is_null(), "dnv_vista_sdk_vis_latest returned NULL");
        // SAFETY: ptr is non-null and owned by the library for the program's lifetime.
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in latest version");
        VisVersion::from_str(s).expect("unrecognized latest version string")
    }

    /// Returns the codebooks for `version`.
    pub fn codebooks(&self, version: VisVersion) -> Result<&Codebooks, VistaError> {
        let s = CString::new(version.as_str()).expect("invalid UTF-8 in VisVersion");
        // SAFETY: self.0 is non-null, and s is a valid NUL-terminated C string.
        let ptr = unsafe { ffi::dnv_vista_sdk_vis_codebooks(self.0, s.as_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            // SAFETY: ptr is non-null (checked above) and valid for `self`'s lifetime.
            Ok(unsafe { Codebooks::from_ptr(ptr) })
        }
    }

    /// Returns the locations for `version`.
    pub fn locations(&self, version: VisVersion) -> Result<&Locations, VistaError> {
        let s = CString::new(version.as_str()).expect("invalid UTF-8 in VisVersion");
        // SAFETY: self.0 is non-null, and s is a valid NUL-terminated C string.
        let ptr = unsafe { ffi::dnv_vista_sdk_vis_locations(self.0, s.as_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            // SAFETY: ptr is non-null (checked above) and valid for `self`'s lifetime.
            Ok(unsafe { Locations::from_ptr(ptr) })
        }
    }

    /// Returns the Gmod for `version`.
    pub fn gmod(&self, version: VisVersion) -> Result<&Gmod, VistaError> {
        let s = CString::new(version.as_str()).expect("invalid UTF-8 in VisVersion");
        // SAFETY: self.0 is non-null, and s is a valid NUL-terminated C string.
        let ptr = unsafe { ffi::dnv_vista_sdk_vis_gmod(self.0, s.as_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            // SAFETY: ptr is non-null (checked above) and valid for `self`'s lifetime.
            Ok(unsafe { Gmod::from_ptr(ptr) })
        }
    }

    /// Converts a Gmod node from `source_version` to `target_version`.
    pub fn convert_node(
        &self,
        source_version: VisVersion,
        source_node: &GmodNodeRef,
        target_version: VisVersion,
    ) -> Result<GmodNode, VistaError> {
        let source_version_c =
            CString::new(source_version.as_str()).expect("invalid UTF-8 in VisVersion");
        let target_version_c =
            CString::new(target_version.as_str()).expect("invalid UTF-8 in VisVersion");
        // SAFETY: self.0 and source_node are non-null. Source_version_c and target_version_c
        // are valid NUL-terminated C strings.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_vis_convert_node(
                self.0,
                source_version_c.as_ptr(),
                source_node.as_ffi_ptr(),
                target_version_c.as_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(GmodNode::from_owned_ptr(ptr))
        }
    }

    /// Converts a Gmod path from `source_version` to `target_version`.
    pub fn convert_path(
        &self,
        source_version: VisVersion,
        source_path: &GmodPathRef,
        target_version: VisVersion,
    ) -> Result<GmodPath, VistaError> {
        let source_version_c =
            CString::new(source_version.as_str()).expect("invalid UTF-8 in VisVersion");
        let target_version_c =
            CString::new(target_version.as_str()).expect("invalid UTF-8 in VisVersion");
        // SAFETY: self.0 and source_path are non-null. Source_version_c and target_version_c
        // are valid NUL-terminated C strings.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_vis_convert_path(
                self.0,
                source_version_c.as_ptr(),
                source_path.as_ffi_ptr(),
                target_version_c.as_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(GmodPath::from_owned_ptr(ptr))
        }
    }

    /// Converts a LocalIdBuilderRef to `target_version`.
    pub fn convert_local_id_builder(
        &self,
        source_local_id: &LocalIdBuilderRef,
        target_version: VisVersion,
    ) -> Result<LocalIdBuilder, VistaError> {
        let target_version_c =
            CString::new(target_version.as_str()).expect("invalid UTF-8 in VisVersion");
        // SAFETY: self.0 and source_local_id are non-null, and target_version_c is a valid
        // NUL-terminated C string.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_vis_convert_local_id_builder(
                self.0,
                source_local_id.as_ffi_ptr(),
                target_version_c.as_ptr(),
            )
        };
        std::ptr::NonNull::new(ptr)
            .map(LocalIdBuilder)
            .ok_or_else(last_error)
    }

    /// Converts a LocalIdRef to `target_version`.
    pub fn convert_local_id(
        &self,
        source_local_id: &LocalIdRef,
        target_version: VisVersion,
    ) -> Result<LocalId, VistaError> {
        let target_version_c =
            CString::new(target_version.as_str()).expect("invalid UTF-8 in VisVersion");
        // SAFETY: self.0 and source_local_id are non-null, and target_version_c is a valid
        // NUL-terminated C string.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_vis_convert_local_id(
                self.0,
                source_local_id.as_ffi_ptr(),
                target_version_c.as_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(LocalId::from_owned_ptr(ptr))
        }
    }
}
