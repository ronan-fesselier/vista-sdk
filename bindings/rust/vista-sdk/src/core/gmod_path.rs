use std::ffi::{CStr, CString};
use std::ptr;
use std::str::FromStr;

use crate::core::gmod::Gmod;
use crate::core::gmod_individualizable_set::GmodIndividualizableSet;
use crate::core::gmod_node::GmodNode;
use crate::core::locations::Locations;
use crate::core::parsing_errors::ParsingErrors;
use crate::core::vis_version::VisVersion;
use crate::ffi::core::gmod_path as ffi;

/// Validated path through a Gmod tree, consisting of parent nodes leading to a target node.
///
/// Borrowed - obtained via accessors like [`crate::core::local_id::LocalId::primary_item`].
/// See [`OwnedGmodPath`] for the owned variant returned by parsing/building functions.
#[repr(transparent)]
pub struct GmodPath(ffi::dnv_vista_sdk_gmod_path_t);

impl GmodPath {
    pub(crate) fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_gmod_path_t) -> &'a Self {
        assert!(!ptr.is_null(), "gmod path pointer must not be NULL");
        unsafe { &*(ptr as *const GmodPath) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_gmod_path_t {
        self as *const Self as *const ffi::dnv_vista_sdk_gmod_path_t
    }

    /// VIS version this path belongs to.
    pub fn version(&self) -> VisVersion {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_path_version(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_gmod_path_version returned NULL"
        );
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in gmod path version");
        VisVersion::from_str(s).expect("unrecognized gmod path version string")
    }

    /// Target (leaf) node of this path.
    pub fn node(&self) -> &GmodNode {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_path_node(self.as_ffi_ptr()) };
        GmodNode::from_ptr(ptr)
    }

    /// Total number of nodes in the path (parents + target).
    pub fn length(&self) -> usize {
        unsafe { ffi::dnv_vista_sdk_gmod_path_length(self.as_ffi_ptr()) }
    }

    /// Node at `index` (depth from root), or `None` if out of bounds.
    pub fn at(&self, index: usize) -> Option<&GmodNode> {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_path_at(self.as_ffi_ptr(), index) };
        if ptr.is_null() {
            return None;
        }
        Some(GmodNode::from_ptr(ptr))
    }

    /// Iterator over all nodes from root to target.
    pub fn iter(&self) -> impl Iterator<Item = &GmodNode> {
        (0..self.length()).map(move |i| self.at(i).expect("index within length() must be valid"))
    }

    /// Iterator over all nodes except the target (i.e. the parent chain).
    pub fn parents(&self) -> impl Iterator<Item = &GmodNode> {
        self.iter().take(self.length().saturating_sub(1))
    }

    /// Whether the target node is mappable to equipment.
    pub fn is_mappable(&self) -> bool {
        unsafe { ffi::dnv_vista_sdk_gmod_path_is_mappable(self.as_ffi_ptr()) != 0 }
    }

    /// Whether any node in this path can be individualized with a location.
    pub fn is_individualizable(&self) -> bool {
        unsafe { ffi::dnv_vista_sdk_gmod_path_is_individualizable(self.as_ffi_ptr()) != 0 }
    }

    /// Returns a copy of this path with all location tags stripped.
    pub fn without_locations(&self) -> OwnedGmodPath {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_path_without_locations(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_gmod_path_without_locations returned NULL"
        );
        OwnedGmodPath(ptr)
    }

    /// Normal assignment name of the node at `node_depth`, if any.
    pub fn normal_assignment_name(&self, node_depth: usize) -> Option<String> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_path_normal_assignment_name(self.as_ffi_ptr(), node_depth)
        };
        if ptr.is_null() {
            return None;
        }
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in normal assignment name")
            .to_string();
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        Some(s)
    }

    /// Number of individualizable sets in this path.
    pub fn individualizable_set_count(&self) -> usize {
        unsafe { ffi::dnv_vista_sdk_gmod_path_individualizable_set_count(self.as_ffi_ptr()) }
    }

    /// Individualizable set at `index`, or `None` if out of bounds.
    pub fn individualizable_set_at(&self, index: usize) -> Option<GmodIndividualizableSet> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_path_individualizable_set_at(self.as_ffi_ptr(), index)
        };
        if ptr.is_null() {
            return None;
        }
        Some(GmodIndividualizableSet::from_owned_ptr(ptr))
    }

    /// Iterator over all individualizable sets in this path.
    pub fn individualizable_sets(&self) -> impl Iterator<Item = GmodIndividualizableSet> + '_ {
        (0..self.individualizable_set_count()).map(move |i| {
            self.individualizable_set_at(i)
                .expect("index within count() must be valid")
        })
    }

    /// Number of function nodes in this path that carry a common name.
    pub fn common_name_count(&self) -> usize {
        unsafe { ffi::dnv_vista_sdk_gmod_path_common_name_count(self.as_ffi_ptr()) }
    }

    /// Depth of the common-name entry at `index`, or `None` if out of bounds.
    pub fn common_name_depth_at(&self, index: usize) -> Option<usize> {
        let mut out: usize = 0;
        let ok = unsafe {
            ffi::dnv_vista_sdk_gmod_path_common_name_depth_at(self.as_ffi_ptr(), index, &mut out)
        };
        if ok != 0 {
            Some(out)
        } else {
            None
        }
    }

    /// Common name of the entry at `index`, or `None` if out of bounds.
    pub fn common_name_at(&self, index: usize) -> Option<String> {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_path_common_name_at(self.as_ffi_ptr(), index) };
        if ptr.is_null() {
            return None;
        }
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in common name")
            .to_string();
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        Some(s)
    }

    /// Iterator over `(depth, name)` pairs for all function nodes with a common name.
    pub fn common_names(&self) -> impl Iterator<Item = (usize, String)> + '_ {
        (0..self.common_name_count()).map(move |i| {
            let depth = self
                .common_name_depth_at(i)
                .expect("index within count() must be valid");
            let name = self
                .common_name_at(i)
                .expect("index within count() must be valid");
            (depth, name)
        })
    }

    /// All node codes joined by `'/'` (e.g. `"VE/411/411.1"`).
    pub fn to_full_path_string(&self) -> String {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_path_to_full_path_string(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_gmod_path_to_full_path_string returned NULL"
        );
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in gmod path full string")
            .to_string();
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        s
    }

    /// Diagnostic dump including codes, names, common names and normal assignments.
    pub fn to_string_dump(&self) -> String {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_path_to_string_dump(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_gmod_path_to_string_dump returned NULL"
        );
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in gmod path dump string")
            .to_string();
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        s
    }
}

impl PartialEq for GmodPath {
    fn eq(&self, other: &Self) -> bool {
        unsafe { ffi::dnv_vista_sdk_gmod_path_equals(self.as_ffi_ptr(), other.as_ffi_ptr()) != 0 }
    }
}

impl Eq for GmodPath {}

impl std::fmt::Display for GmodPath {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_path_to_string(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_gmod_path_to_string returned NULL"
        );
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in gmod path string");
        let result = f.write_str(s);
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        result
    }
}

impl std::fmt::Debug for GmodPath {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "GmodPath({})", self)
    }
}

/// An owned, independently-released [`GmodPath`].
///
/// Returned by parsing functions (e.g. [`OwnedGmodPath::from_short_path`]) and other
/// APIs that hand back a new path value rather than a reference into existing state.
pub struct OwnedGmodPath(pub(crate) *mut ffi::dnv_vista_sdk_gmod_path_t);

impl Drop for OwnedGmodPath {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_gmod_path_free(self.0) }
    }
}

impl std::fmt::Display for OwnedGmodPath {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        std::fmt::Display::fmt(self.as_ref(), f)
    }
}

impl AsRef<GmodPath> for OwnedGmodPath {
    fn as_ref(&self) -> &GmodPath {
        self
    }
}

impl std::ops::Deref for OwnedGmodPath {
    type Target = GmodPath;

    fn deref(&self) -> &GmodPath {
        GmodPath::from_ptr(self.0)
    }
}

impl OwnedGmodPath {
    pub(crate) fn from_owned_ptr(ptr: *mut ffi::dnv_vista_sdk_gmod_path_t) -> Self {
        OwnedGmodPath(ptr)
    }

    /// Parses a short path string for the given VIS version. Returns `None` on invalid input.
    pub fn from_short_path_version(item: &str, vis_version: VisVersion) -> Option<Self> {
        let c_item = CString::new(item).ok()?;
        let c_version = CString::new(vis_version.as_str()).expect("invalid UTF-8 in VisVersion");
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_path_from_short_path_version(
                c_item.as_ptr(),
                c_version.as_ptr(),
            )
        };
        if ptr.is_null() {
            None
        } else {
            Some(OwnedGmodPath(ptr))
        }
    }

    /// Parses a short path string using the provided Gmod and Locations. Returns `None` on invalid input.
    pub fn from_short_path(item: &str, gmod: &Gmod, locations: &Locations) -> Option<Self> {
        let c_item = CString::new(item).ok()?;
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_path_from_short_path(
                c_item.as_ptr(),
                gmod.as_ffi_ptr(),
                locations.as_ffi_ptr(),
            )
        };
        if ptr.is_null() {
            None
        } else {
            Some(OwnedGmodPath(ptr))
        }
    }

    /// Parses a short path string, returning the path (or `None`) together with any parse errors.
    pub fn from_short_path_with_errors(
        item: &str,
        gmod: &Gmod,
        locations: &Locations,
    ) -> (Option<Self>, ParsingErrors) {
        let c_item = CString::new(item).expect("item contains a NUL byte");
        let mut errors_ptr = ptr::null_mut();
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_path_from_short_path_with_errors(
                c_item.as_ptr(),
                gmod.as_ffi_ptr(),
                locations.as_ffi_ptr(),
                &mut errors_ptr,
            )
        };
        assert!(
            !errors_ptr.is_null(),
            "dnv_vista_sdk_gmod_path_from_short_path_with_errors did not set outErrors"
        );
        let path = if ptr.is_null() {
            None
        } else {
            Some(OwnedGmodPath(ptr))
        };
        (path, ParsingErrors(errors_ptr))
    }

    /// Parses a full path string (all node codes joined by `'/'`). Returns `None` on invalid input.
    pub fn from_full_path(full_path_str: &str, gmod: &Gmod, locations: &Locations) -> Option<Self> {
        let c_str = CString::new(full_path_str).ok()?;
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_path_from_full_path(
                c_str.as_ptr(),
                gmod.as_ffi_ptr(),
                locations.as_ffi_ptr(),
            )
        };
        if ptr.is_null() {
            None
        } else {
            Some(OwnedGmodPath(ptr))
        }
    }

    /// Parses a full path string, returning the path (or `None`) together with any parse errors.
    pub fn from_full_path_with_errors(
        full_path_str: &str,
        gmod: &Gmod,
        locations: &Locations,
    ) -> (Option<Self>, ParsingErrors) {
        let c_str = CString::new(full_path_str).expect("full_path_str contains a NUL byte");
        let mut errors_ptr = ptr::null_mut();
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_path_from_full_path_with_errors(
                c_str.as_ptr(),
                gmod.as_ffi_ptr(),
                locations.as_ffi_ptr(),
                &mut errors_ptr,
            )
        };
        assert!(
            !errors_ptr.is_null(),
            "dnv_vista_sdk_gmod_path_from_full_path_with_errors did not set outErrors"
        );
        let path = if ptr.is_null() {
            None
        } else {
            Some(OwnedGmodPath(ptr))
        };
        (path, ParsingErrors(errors_ptr))
    }
}
