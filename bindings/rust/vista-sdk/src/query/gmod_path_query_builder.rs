use std::ffi::CString;

use crate::core::error::{last_error, VistaError};
use crate::core::gmod_node::GmodNode;
use crate::core::gmod_path::GmodPath;
use crate::core::location::Location;
use crate::ffi::query::gmod_path_query_builder as ffi;
use crate::query::gmod_path_query::GmodPathQuery;

/// Builder for constructing [`GmodPathQuery`] instances with a fluent API.
///
/// Two variants exist internally:
/// - **Path** - created via [`OwnedGmodPathQueryBuilder::from_path`], starts from an existing
///   [`GmodPath`] and allows node-level overrides and location masking.
/// - **Nodes** - created via [`OwnedGmodPathQueryBuilder::create`], built from scratch by
///   adding individual nodes.
///
/// Path-only methods ([`Self::path`], [`Self::path_with_node_all_locations`], [`Self::path_with_node_locations`],
/// [`Self::with_any_node_before`], [`Self::with_any_node_after`], [`Self::without_locations`]) return
/// `Err` when called on a Nodes-variant handle, and vice versa for Nodes-only methods
/// ([`Self::with_node_all_locations`], [`Self::with_node_locations`]).
///
/// Borrowed - obtained via accessors. See [`OwnedGmodPathQueryBuilder`] for the owned variant.
#[repr(transparent)]
pub struct GmodPathQueryBuilder(ffi::dnv_vista_sdk_gmod_path_query_builder_t);

impl GmodPathQueryBuilder {
    pub(crate) fn from_ptr<'a>(
        ptr: *const ffi::dnv_vista_sdk_gmod_path_query_builder_t,
    ) -> &'a Self {
        assert!(
            !ptr.is_null(),
            "gmod path query builder pointer must not be NULL"
        );
        unsafe { &*(ptr as *const GmodPathQueryBuilder) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_gmod_path_query_builder_t {
        self as *const Self as *const ffi::dnv_vista_sdk_gmod_path_query_builder_t
    }

    /// Base path this builder was created from (Path variant only).
    pub fn path(&self) -> Option<&GmodPath> {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_path_query_builder_path(self.as_ffi_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(GmodPath::from_ptr(ptr))
        }
    }

    /// Returns a new builder with the node selected by `code` configured to match
    /// any location individualization (Path variant only).
    pub fn path_with_node_all_locations(
        &self,
        code: &str,
        match_all_locations: bool,
    ) -> Result<OwnedGmodPathQueryBuilder, VistaError> {
        let c_code = CString::new(code).expect("code contains a NUL byte");
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_path_query_builder_path_with_node_all_locations(
                self.as_ffi_ptr(),
                c_code.as_ptr(),
                match_all_locations as std::ffi::c_int,
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedGmodPathQueryBuilder(ptr))
        }
    }

    /// Returns a new builder with the node selected by `code` configured to match
    /// the given locations (Path variant only).
    pub fn path_with_node_locations(
        &self,
        code: &str,
        locations: &[&Location],
    ) -> Result<OwnedGmodPathQueryBuilder, VistaError> {
        let c_code = CString::new(code).expect("code contains a NUL byte");
        let ptrs: Vec<*const crate::ffi::core::location::dnv_vista_sdk_location_t> =
            locations.iter().map(|l| l.as_ffi_ptr()).collect();
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_path_query_builder_path_with_node_locations(
                self.as_ffi_ptr(),
                c_code.as_ptr(),
                ptrs.as_ptr(),
                ptrs.len(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedGmodPathQueryBuilder(ptr))
        }
    }

    /// Returns a new builder with all nodes before `code` in the path ignored
    /// (Path variant only).
    pub fn with_any_node_before(
        &self,
        code: &str,
    ) -> Result<OwnedGmodPathQueryBuilder, VistaError> {
        let c_code = CString::new(code).expect("code contains a NUL byte");
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_path_query_builder_with_any_node_before(
                self.as_ffi_ptr(),
                c_code.as_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedGmodPathQueryBuilder(ptr))
        }
    }

    /// Returns a new builder with all nodes after `code` in the path ignored
    /// (Path variant only).
    pub fn with_any_node_after(&self, code: &str) -> Result<OwnedGmodPathQueryBuilder, VistaError> {
        let c_code = CString::new(code).expect("code contains a NUL byte");
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_path_query_builder_with_any_node_after(
                self.as_ffi_ptr(),
                c_code.as_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedGmodPathQueryBuilder(ptr))
        }
    }

    /// Returns a new builder with all location individualizations ignored
    /// (Path variant only).
    pub fn without_locations(&self) -> Result<OwnedGmodPathQueryBuilder, VistaError> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_path_query_builder_without_locations(self.as_ffi_ptr())
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedGmodPathQueryBuilder(ptr))
        }
    }

    /// Returns a new builder with `node` added, configured to match any location
    /// individualization (Nodes variant only).
    pub fn with_node_all_locations(
        &self,
        node: &GmodNode,
        match_all_locations: bool,
    ) -> Result<OwnedGmodPathQueryBuilder, VistaError> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_path_query_builder_with_node_all_locations(
                self.as_ffi_ptr(),
                node.as_ffi_ptr(),
                match_all_locations as std::ffi::c_int,
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedGmodPathQueryBuilder(ptr))
        }
    }

    /// Returns a new builder with `node` added, configured to match the given locations
    /// (Nodes variant only).
    pub fn with_node_locations(
        &self,
        node: &GmodNode,
        locations: &[&Location],
    ) -> Result<OwnedGmodPathQueryBuilder, VistaError> {
        let ptrs: Vec<*const crate::ffi::core::location::dnv_vista_sdk_location_t> =
            locations.iter().map(|l| l.as_ffi_ptr()).collect();
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_path_query_builder_with_node_locations(
                self.as_ffi_ptr(),
                node.as_ffi_ptr(),
                ptrs.as_ptr(),
                ptrs.len(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedGmodPathQueryBuilder(ptr))
        }
    }

    /// Constructs the immutable [`GmodPathQuery`].
    pub fn build(&self) -> GmodPathQuery {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_path_query_builder_build(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_gmod_path_query_builder_build returned NULL"
        );
        GmodPathQuery::from_owned_ptr(ptr)
    }
}

/// An owned, independently-released [`GmodPathQueryBuilder`].
pub struct OwnedGmodPathQueryBuilder(pub(crate) *mut ffi::dnv_vista_sdk_gmod_path_query_builder_t);

impl Drop for OwnedGmodPathQueryBuilder {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_gmod_path_query_builder_free(self.0) }
    }
}

impl std::ops::Deref for OwnedGmodPathQueryBuilder {
    type Target = GmodPathQueryBuilder;

    fn deref(&self) -> &GmodPathQueryBuilder {
        GmodPathQueryBuilder::from_ptr(self.0)
    }
}

impl OwnedGmodPathQueryBuilder {
    /// Creates an empty Nodes-variant builder.
    pub fn create() -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_path_query_builder_create() };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_gmod_path_query_builder_create returned NULL"
        );
        OwnedGmodPathQueryBuilder(ptr)
    }

    /// Creates a Path-variant builder from an existing [`GmodPath`].
    pub fn from_path(path: &GmodPath) -> Result<Self, VistaError> {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_path_query_builder_from(path.as_ffi_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedGmodPathQueryBuilder(ptr))
        }
    }
}
