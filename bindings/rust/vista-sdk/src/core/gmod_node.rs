use std::ffi::{CStr, CString};
use std::str::FromStr;

use crate::core::gmod_node_metadata::GmodNodeMetadata;
use crate::core::vis_version::VisVersion;
use crate::ffi::core::gmod_node as ffi;
use crate::ffi::core::location as ffi_location;

/// Represents a node in the Gmod (Generic Product Model) tree hierarchy
#[repr(transparent)]
pub struct GmodNode(ffi::dnv_vista_sdk_gmod_node_t);

impl GmodNode {
    pub(crate) fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_gmod_node_t) -> &'a Self {
        assert!(!ptr.is_null(), "gmod node pointer must not be NULL");
        // SAFETY: ptr is non-null and `GmodNode` is `#[repr(transparent)]` over the FFI type.
        unsafe { &*(ptr as *const GmodNode) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_gmod_node_t {
        self as *const Self as *const ffi::dnv_vista_sdk_gmod_node_t
    }

    /// VIS version this node belongs to.
    pub fn version(&self) -> VisVersion {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_node_version(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_gmod_node_version returned NULL"
        );
        // SAFETY: ptr is non-null and owned by `self` for its lifetime.
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in gmod node version");
        VisVersion::from_str(s).expect("unrecognized gmod node version string")
    }

    /// Short alphanumeric code identifying this node in the Gmod tree.
    pub fn code(&self) -> &str {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_node_code(self.as_ffi_ptr()) };
        assert!(!ptr.is_null(), "dnv_vista_sdk_gmod_node_code returned NULL");
        // SAFETY: ptr is non-null and owned by `self` for its lifetime.
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in gmod node code")
    }

    /// Location tag of this node, if any.
    pub fn location(&self) -> Option<&str> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_node_location(self.as_ffi_ptr()) };
        if ptr.is_null() {
            return None;
        }
        // SAFETY: ptr is non-null (checked above) and valid for the lifetime of `&self`.
        let value_ptr = unsafe { ffi_location::dnv_vista_sdk_location_value(ptr) };
        assert!(
            !value_ptr.is_null(),
            "dnv_vista_sdk_location_value returned NULL"
        );
        Some(
            // SAFETY: value_ptr is non-null (checked above) and owned by `self` for its lifetime.
            unsafe { CStr::from_ptr(value_ptr) }
                .to_str()
                .expect("invalid UTF-8 in location value"),
        )
    }

    /// Metadata associated with this node.
    pub fn metadata(&self) -> &GmodNodeMetadata {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_node_metadata(self.as_ffi_ptr()) };
        GmodNodeMetadata::from_ptr(ptr)
    }

    /// Number of direct children.
    pub fn child_count(&self) -> usize {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_gmod_node_child_count(self.as_ffi_ptr()) }
    }

    /// Child node at `index`, or `None` if out of bounds.
    pub fn child_at(&self, index: usize) -> Option<&GmodNode> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_node_child_at(self.as_ffi_ptr(), index) };
        if ptr.is_null() {
            return None;
        }
        Some(GmodNode::from_ptr(ptr))
    }

    /// Iterator over all direct children.
    pub fn children(&self) -> impl Iterator<Item = &GmodNode> {
        (0..self.child_count()).map(move |i| {
            self.child_at(i)
                .expect("index within count() must be valid")
        })
    }

    /// Number of direct parents.
    pub fn parent_count(&self) -> usize {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_gmod_node_parent_count(self.as_ffi_ptr()) }
    }

    /// Parent node at `index`, or `None` if out of bounds.
    pub fn parent_at(&self, index: usize) -> Option<&GmodNode> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_node_parent_at(self.as_ffi_ptr(), index) };
        if ptr.is_null() {
            return None;
        }
        Some(GmodNode::from_ptr(ptr))
    }

    /// Iterator over all direct parents.
    pub fn parents(&self) -> impl Iterator<Item = &GmodNode> {
        (0..self.parent_count()).map(move |i| {
            self.parent_at(i)
                .expect("index within count() must be valid")
        })
    }

    /// Product type child of this function node, if any.
    pub fn product_type(&self) -> Option<&GmodNode> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_node_product_type(self.as_ffi_ptr()) };
        if ptr.is_null() {
            return None;
        }
        Some(GmodNode::from_ptr(ptr))
    }

    /// Product selection child of this function node, if any.
    pub fn product_selection(&self) -> Option<&GmodNode> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_node_product_selection(self.as_ffi_ptr()) };
        if ptr.is_null() {
            return None;
        }
        Some(GmodNode::from_ptr(ptr))
    }

    /// Whether this is a function composition node.
    pub fn is_function_composition(&self) -> bool {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_gmod_node_is_function_composition(self.as_ffi_ptr()) != 0 }
    }

    /// Whether this node can appear in a path.
    pub fn is_mappable(&self) -> bool {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_gmod_node_is_mappable(self.as_ffi_ptr()) != 0 }
    }

    /// Whether this is a product selection node.
    pub fn is_product_selection(&self) -> bool {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_gmod_node_is_product_selection(self.as_ffi_ptr()) != 0 }
    }

    /// Whether this is a product type node.
    pub fn is_product_type(&self) -> bool {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_gmod_node_is_product_type(self.as_ffi_ptr()) != 0 }
    }

    /// Whether this is an asset node.
    pub fn is_asset(&self) -> bool {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_gmod_node_is_asset(self.as_ffi_ptr()) != 0 }
    }

    /// Whether this node has no children.
    pub fn is_leaf_node(&self) -> bool {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_gmod_node_is_leaf_node(self.as_ffi_ptr()) != 0 }
    }

    /// Whether this is a function node.
    pub fn is_function_node(&self) -> bool {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_gmod_node_is_function_node(self.as_ffi_ptr()) != 0 }
    }

    /// Whether this is an asset function node.
    pub fn is_asset_function_node(&self) -> bool {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_gmod_node_is_asset_function_node(self.as_ffi_ptr()) != 0 }
    }

    /// Whether this is the root of the Gmod tree.
    pub fn is_root(&self) -> bool {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_gmod_node_is_root(self.as_ffi_ptr()) != 0 }
    }

    /// Whether `node` is a direct child of this node.
    pub fn is_child(&self, node: &GmodNode) -> bool {
        // SAFETY: self and node are non-null and valid for the lifetime of `&self`/`&node`.
        unsafe { ffi::dnv_vista_sdk_gmod_node_is_child(self.as_ffi_ptr(), node.as_ffi_ptr()) != 0 }
    }

    /// Whether the node identified by `code` is a direct child of this node.
    pub fn is_child_code(&self, code: &str) -> bool {
        let c_code = CString::new(code).expect("code contains a NUL byte");
        // SAFETY: self is non-null, and c_code is a valid NUL-terminated C string.
        unsafe {
            ffi::dnv_vista_sdk_gmod_node_is_child_code(self.as_ffi_ptr(), c_code.as_ptr()) != 0
        }
    }
}

impl std::fmt::Display for GmodNode {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        // SAFETY: self is non-null and valid for the lifetime of `&self`; returns an owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_node_to_string(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_gmod_node_to_string returned NULL"
        );
        // SAFETY: ptr is non-null and owned until freed below.
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in gmod node string");
        let result = f.write_str(s);
        // SAFETY: ptr was allocated by the library and is freed exactly once.
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        result
    }
}

/// An owned `GmodNode` whose memory is managed by this binding.
pub struct OwnedGmodNode {
    pub(crate) ptr: *mut ffi::dnv_vista_sdk_gmod_node_t,
    drop_fn: unsafe extern "C" fn(*mut ffi::dnv_vista_sdk_gmod_node_t),
}

impl OwnedGmodNode {
    pub(crate) fn with_drop(
        ptr: *mut ffi::dnv_vista_sdk_gmod_node_t,
        drop_fn: unsafe extern "C" fn(*mut ffi::dnv_vista_sdk_gmod_node_t),
    ) -> Self {
        Self { ptr, drop_fn }
    }
}

impl Drop for OwnedGmodNode {
    fn drop(&mut self) {
        // SAFETY: self.ptr is owned by this `OwnedGmodNode` and freed exactly once via
        // the matching `drop_fn` supplied at construction.
        unsafe { (self.drop_fn)(self.ptr) }
    }
}

impl std::ops::Deref for OwnedGmodNode {
    type Target = GmodNode;

    fn deref(&self) -> &GmodNode {
        GmodNode::from_ptr(self.ptr)
    }
}
