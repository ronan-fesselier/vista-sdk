use std::ffi::{c_void, CStr, CString};
use std::str::FromStr;

use crate::core::error::{last_error, VistaError};
use crate::core::gmod_node::GmodNode;
use crate::core::vis_version::VisVersion;
use crate::ffi::core::gmod as ffi;
use crate::ffi::core::gmod::dnv_vista_sdk_traversal_result_t;
use crate::ffi::core::gmod_node::dnv_vista_sdk_gmod_node_t;

/// Controls depth-first traversal of the Gmod tree.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum TraversalHandlerResult {
    /// Stop traversal immediately.
    Stop,
    /// Skip children of the current node but continue elsewhere.
    SkipSubtree,
    /// Continue normal traversal.
    Continue,
}

/// Generic Product Model container for a specific VIS version.
///
/// Borrowed from the VIS singleton, valid for the lifetime of the program.
#[repr(transparent)]
pub struct Gmod(ffi::dnv_vista_sdk_gmod_t);

impl Gmod {
    pub(crate) fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_gmod_t) -> &'a Self {
        assert!(!ptr.is_null(), "dnv_vista_sdk_vis_gmod returned NULL");
        // SAFETY: ptr is non-null and `Gmod` is `#[repr(transparent)]` over the FFI type.
        unsafe { &*(ptr as *const Gmod) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_gmod_t {
        self as *const Self as *const ffi::dnv_vista_sdk_gmod_t
    }

    /// VIS version this Gmod was loaded for.
    pub fn version(&self) -> VisVersion {
        // SAFETY: self.as_ffi_ptr() is non-null and valid for the lifetime of `self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_version(self.as_ffi_ptr()) };
        assert!(!ptr.is_null(), "dnv_vista_sdk_gmod_version returned NULL");
        // SAFETY: ptr is non-null and owned by the library for the program's lifetime.
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in gmod version");
        VisVersion::from_str(s).expect("unrecognized gmod version string")
    }

    /// Root of the Gmod tree (code `"VE"`).
    pub fn root_node(&self) -> &GmodNode {
        // SAFETY: self.as_ffi_ptr() is non-null and valid for the lifetime of `self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_root_node(self.as_ffi_ptr()) };
        GmodNode::from_ptr(ptr)
    }

    /// Returns the node identified by `code`, or `Err` if not found.
    pub fn get_node(&self, code: &str) -> Result<&GmodNode, VistaError> {
        let c_code = CString::new(code).map_err(|_| VistaError {
            kind: crate::core::error::ErrorKind::InvalidArgument,
            message: "code contains a NUL byte".to_string(),
        })?;
        // SAFETY: self.as_ffi_ptr() is non-null and c_code is a valid NUL-terminated C string.
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_get_node(self.as_ffi_ptr(), c_code.as_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(GmodNode::from_ptr(ptr))
        }
    }

    /// Total number of nodes.
    pub fn node_count(&self) -> usize {
        // SAFETY: self.as_ffi_ptr() is non-null and valid for the lifetime of `self`.
        unsafe { ffi::dnv_vista_sdk_gmod_node_count(self.as_ffi_ptr()) }
    }

    /// Node at `index` in unspecified iteration order, or `None` if out of bounds.
    pub fn node_at(&self, index: usize) -> Option<&GmodNode> {
        // SAFETY: self.as_ffi_ptr() is non-null and valid for the lifetime of `self`.
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

    /// Traverse the Gmod tree depth-first from the root.
    ///
    /// `handler` receives the current ancestor path (root first) and the current node.
    /// Return [`TraversalHandlerResult::Stop`] to abort early,
    /// [`TraversalHandlerResult::SkipSubtree`] to skip children,
    /// or [`TraversalHandlerResult::Continue`] for normal traversal.
    ///
    /// `max_traversal_occurrence` limits how many times a node may appear in the current
    /// path before the traversal of that subtree is cut. The standard default is `1`.
    ///
    /// Returns `true` if traversal completed, `false` if stopped early.
    pub fn traverse<F>(&self, mut handler: F, max_traversal_occurrence: i32) -> bool
    where
        F: FnMut(&[&GmodNode], &GmodNode) -> TraversalHandlerResult,
    {
        // SAFETY: called by the C API with `userdata` as passed below (a live `*mut F`)
        // and `parents`/`node` as non-null pointers valid for the call's duration.
        unsafe extern "C" fn trampoline<F>(
            parents: *const *const dnv_vista_sdk_gmod_node_t,
            parent_count: usize,
            node: *const dnv_vista_sdk_gmod_node_t,
            userdata: *mut c_void,
        ) -> dnv_vista_sdk_traversal_result_t
        where
            F: FnMut(&[&GmodNode], &GmodNode) -> TraversalHandlerResult,
        {
            let cb = &mut *(userdata as *mut F);
            let parent_nodes: Vec<&GmodNode> = if parent_count == 0 {
                Vec::new()
            } else {
                std::slice::from_raw_parts(parents, parent_count)
                    .iter()
                    .map(|p| GmodNode::from_ptr(*p))
                    .collect()
            };
            let current = GmodNode::from_ptr(node);
            match cb(&parent_nodes, current) {
                TraversalHandlerResult::Stop => dnv_vista_sdk_traversal_result_t::Stop,
                TraversalHandlerResult::SkipSubtree => {
                    dnv_vista_sdk_traversal_result_t::SkipSubtree
                }
                TraversalHandlerResult::Continue => dnv_vista_sdk_traversal_result_t::Continue,
            }
        }

        let userdata = &mut handler as *mut F as *mut c_void;

        // SAFETY: self.as_ffi_ptr() is non-null, `trampoline::<F>` matches the expected
        // signature, and `userdata` outlives the call (borrowed from `handler` on the stack).
        unsafe {
            ffi::dnv_vista_sdk_gmod_traverse(
                self.as_ffi_ptr(),
                trampoline::<F>,
                max_traversal_occurrence,
                userdata,
            ) != 0
        }
    }
}
