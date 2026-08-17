use std::ffi::{CStr, CString};
use std::str::FromStr;

use crate::core::error::{last_error, VistaError};
use crate::core::gmod_node::GmodNode;
use crate::core::vis_version::VisVersion;
use crate::ffi::core::gmod as ffi;

/// Generic Product Model container for a specific VIS version.
///
/// Borrowed from the VIS singleton, valid for the lifetime of the program.
#[repr(transparent)]
pub struct Gmod(ffi::dnv_vista_sdk_gmod_t);

impl Gmod {
    pub(crate) fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_gmod_t) -> &'a Self {
        assert!(!ptr.is_null(), "dnv_vista_sdk_vis_gmod returned NULL");
        unsafe { &*(ptr as *const Gmod) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_gmod_t {
        self as *const Self as *const ffi::dnv_vista_sdk_gmod_t
    }

    /// VIS version this Gmod was loaded for.
    pub fn version(&self) -> VisVersion {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_version(self.as_ffi_ptr()) };
        assert!(!ptr.is_null(), "dnv_vista_sdk_gmod_version returned NULL");
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in gmod version");
        VisVersion::from_str(s).expect("unrecognized gmod version string")
    }

    /// Root of the Gmod tree (code `"VE"`).
    pub fn root_node(&self) -> &GmodNode {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_root_node(self.as_ffi_ptr()) };
        GmodNode::from_ptr(ptr)
    }

    /// Returns the node identified by `code`, or `Err` if not found.
    pub fn get_node(&self, code: &str) -> Result<&GmodNode, VistaError> {
        let c_code = CString::new(code).map_err(|_| VistaError {
            kind: crate::core::error::ErrorKind::InvalidArgument,
            message: "code contains a NUL byte".to_string(),
        })?;
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_get_node(self.as_ffi_ptr(), c_code.as_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(GmodNode::from_ptr(ptr))
        }
    }

    /// Total number of nodes.
    pub fn node_count(&self) -> usize {
        unsafe { ffi::dnv_vista_sdk_gmod_node_count(self.as_ffi_ptr()) }
    }

    /// Node at `index` in unspecified iteration order, or `None` if out of bounds.
    pub fn node_at(&self, index: usize) -> Option<&GmodNode> {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_node_at(self.as_ffi_ptr(), index) };
        if ptr.is_null() {
            return None;
        }
        Some(GmodNode::from_ptr(ptr))
    }

    /// Iterator over all nodes in unspecified order.
    pub fn iter(&self) -> impl Iterator<Item = &GmodNode> {
        (0..self.node_count())
            .map(move |i| self.node_at(i).expect("index within count() must be valid"))
    }
}
