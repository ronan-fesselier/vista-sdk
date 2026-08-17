use std::ffi::{CStr, CString};
use std::str::FromStr;

use crate::core::codebooks::Codebooks;
use crate::core::error::{last_error, VistaError};
use crate::core::gmod::Gmod;
use crate::core::locations::Locations;
use crate::core::vis_version::VisVersion;
use crate::ffi::core::vis as ffi;

/// Central entry point for the Vista SDK.
///
/// Wraps a borrowed pointer to the C++ singleton, valid for the lifetime of the program.
pub struct Vis(*const ffi::dnv_vista_sdk_vis_t);

unsafe impl Send for Vis {}
unsafe impl Sync for Vis {}

impl Vis {
    /// Returns the VIS singleton instance.
    pub fn instance() -> Vis {
        Vis(unsafe { ffi::dnv_vista_sdk_vis_instance() })
    }

    /// Returns all available VIS versions, in ascending order.
    pub fn versions(&self) -> Vec<VisVersion> {
        let count = unsafe { ffi::dnv_vista_sdk_vis_version_count(self.0) };
        (0..count)
            .filter_map(|i| {
                let ptr = unsafe { ffi::dnv_vista_sdk_vis_version_at(self.0, i) };
                if ptr.is_null() {
                    return None;
                }
                let s = unsafe { CStr::from_ptr(ptr) }.to_str().ok()?;
                VisVersion::from_str(s).ok()
            })
            .collect()
    }

    /// Returns the latest VIS version.
    pub fn latest(&self) -> VisVersion {
        let ptr = unsafe { ffi::dnv_vista_sdk_vis_latest(self.0) };
        assert!(!ptr.is_null(), "dnv_vista_sdk_vis_latest returned NULL");
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in latest version");
        VisVersion::from_str(s).expect("unrecognized latest version string")
    }

    /// Returns the codebooks for `version`.
    pub fn codebooks(&self, version: VisVersion) -> Result<&Codebooks, VistaError> {
        let s = CString::new(version.as_str()).expect("invalid UTF-8 in VisVersion");
        let ptr = unsafe { ffi::dnv_vista_sdk_vis_codebooks(self.0, s.as_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(Codebooks::from_ptr(ptr))
        }
    }

    /// Returns the locations for `version`.
    pub fn locations(&self, version: VisVersion) -> Result<&Locations, VistaError> {
        let s = CString::new(version.as_str()).expect("invalid UTF-8 in VisVersion");
        let ptr = unsafe { ffi::dnv_vista_sdk_vis_locations(self.0, s.as_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(Locations::from_ptr(ptr))
        }
    }

    /// Returns the Gmod for `version`.
    pub fn gmod(&self, version: VisVersion) -> Result<&Gmod, VistaError> {
        let s = CString::new(version.as_str()).expect("invalid UTF-8 in VisVersion");
        let ptr = unsafe { ffi::dnv_vista_sdk_vis_gmod(self.0, s.as_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(Gmod::from_ptr(ptr))
        }
    }
}
