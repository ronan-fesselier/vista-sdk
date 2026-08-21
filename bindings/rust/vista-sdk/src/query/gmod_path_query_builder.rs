use std::ffi::CString;
use std::ptr::NonNull;

use crate::core::error::{last_error, VistaError};
use crate::core::gmod_node::GmodNodeRef;
use crate::core::gmod_path::GmodPathRef;
use crate::core::location::Location;
use crate::ffi::query::gmod_path_query_builder as ffi;
use crate::query::gmod_path_query::GmodPathQuery;

/// Builder for constructing [`GmodPathQuery`] instances with a fluent API.
///
/// Two variants exist internally:
/// - **Path** - created via [`GmodPathQueryBuilder::from_path`], starts from an existing
///   [`GmodPathRef`] and allows node-level overrides and location masking.
/// - **Nodes** - created via [`GmodPathQueryBuilder::create`], built from scratch by
///   adding individual nodes.
///
/// Path-only methods ([`Self::path`], [`Self::path_with_node_all_locations`], [`Self::path_with_node_locations`],
/// [`Self::with_any_node_before`], [`Self::with_any_node_after`], [`Self::without_locations`]) return
/// `Err` when called on a Nodes-variant handle, and vice versa for Nodes-only methods
/// ([`Self::with_node_all_locations`], [`Self::with_node_locations`]).
///
/// Borrowed - obtained via accessors. See [`GmodPathQueryBuilder`] for the owned variant.
#[repr(transparent)]
pub struct GmodPathQueryBuilderRef(ffi::dnv_vista_sdk_gmod_path_query_builder_t);

impl GmodPathQueryBuilderRef {
    /// # Safety
    /// `ptr` must be non-null and valid for `'a`.
    pub(crate) unsafe fn from_ptr<'a>(
        ptr: *const ffi::dnv_vista_sdk_gmod_path_query_builder_t,
    ) -> &'a Self {
        // SAFETY: caller guarantees ptr is non-null and valid. `GmodPathQueryBuilderRef` is `#[repr(transparent)]`.
        unsafe { &*(ptr as *const GmodPathQueryBuilderRef) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_gmod_path_query_builder_t {
        self as *const Self as *const ffi::dnv_vista_sdk_gmod_path_query_builder_t
    }

    /// Base path this builder was created from (Path variant only).
    pub fn path(&self) -> Option<&GmodPathRef> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_path_query_builder_path(self.as_ffi_ptr()) };
        if ptr.is_null() {
            None
        } else {
            // SAFETY: ptr is non-null (checked above) and valid for `self`'s lifetime.
            Some(unsafe { GmodPathRef::from_ptr(ptr) })
        }
    }

    /// Returns a new builder with the node selected by `code` configured to match
    /// any location individualization (Path variant only).
    pub fn path_with_node_all_locations(
        &self,
        code: &str,
        match_all_locations: bool,
    ) -> Result<GmodPathQueryBuilder, VistaError> {
        let c_code = CString::new(code).expect("code contains a NUL byte");
        // SAFETY: self is non-null, and c_code is a valid NUL-terminated C string.
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
            Ok(GmodPathQueryBuilder(
                NonNull::new(ptr).expect("returned NULL"),
            ))
        }
    }

    /// Returns a new builder with the node selected by `code` configured to match
    /// the given locations (Path variant only).
    pub fn path_with_node_locations(
        &self,
        code: &str,
        locations: &[&Location],
    ) -> Result<GmodPathQueryBuilder, VistaError> {
        let c_code = CString::new(code).expect("code contains a NUL byte");
        let ptrs: Vec<*const crate::ffi::core::location::dnv_vista_sdk_location_t> =
            locations.iter().map(|l| l.as_ffi_ptr()).collect();
        // SAFETY: self is non-null, c_code is a valid NUL-terminated C string, and ptrs.as_ptr()
        // is valid for ptrs.len() elements.
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
            Ok(GmodPathQueryBuilder(
                NonNull::new(ptr).expect("returned NULL"),
            ))
        }
    }

    /// Returns a new builder with all nodes before `code` in the path ignored
    /// (Path variant only).
    pub fn with_any_node_before(&self, code: &str) -> Result<GmodPathQueryBuilder, VistaError> {
        let c_code = CString::new(code).expect("code contains a NUL byte");
        // SAFETY: self is non-null, and c_code is a valid NUL-terminated C string.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_path_query_builder_with_any_node_before(
                self.as_ffi_ptr(),
                c_code.as_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(GmodPathQueryBuilder(
                NonNull::new(ptr).expect("returned NULL"),
            ))
        }
    }

    /// Returns a new builder with all nodes after `code` in the path ignored
    /// (Path variant only).
    pub fn with_any_node_after(&self, code: &str) -> Result<GmodPathQueryBuilder, VistaError> {
        let c_code = CString::new(code).expect("code contains a NUL byte");
        // SAFETY: self is non-null, and c_code is a valid NUL-terminated C string.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_path_query_builder_with_any_node_after(
                self.as_ffi_ptr(),
                c_code.as_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(GmodPathQueryBuilder(
                NonNull::new(ptr).expect("returned NULL"),
            ))
        }
    }

    /// Returns a new builder with all location individualizations ignored
    /// (Path variant only).
    pub fn without_locations(&self) -> Result<GmodPathQueryBuilder, VistaError> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_path_query_builder_without_locations(self.as_ffi_ptr())
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(GmodPathQueryBuilder(
                NonNull::new(ptr).expect("returned NULL"),
            ))
        }
    }

    /// Returns a new builder with `node` added, configured to match any location
    /// individualization (Nodes variant only).
    pub fn with_node_all_locations(
        &self,
        node: &GmodNodeRef,
        match_all_locations: bool,
    ) -> Result<GmodPathQueryBuilder, VistaError> {
        // SAFETY: self and node are non-null and valid for the call's duration.
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
            Ok(GmodPathQueryBuilder(
                NonNull::new(ptr).expect("returned NULL"),
            ))
        }
    }

    /// Returns a new builder with `node` added, configured to match the given locations
    /// (Nodes variant only).
    pub fn with_node_locations(
        &self,
        node: &GmodNodeRef,
        locations: &[&Location],
    ) -> Result<GmodPathQueryBuilder, VistaError> {
        let ptrs: Vec<*const crate::ffi::core::location::dnv_vista_sdk_location_t> =
            locations.iter().map(|l| l.as_ffi_ptr()).collect();
        // SAFETY: self and node are non-null, and ptrs.as_ptr() is valid for ptrs.len() elements.
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
            Ok(GmodPathQueryBuilder(
                NonNull::new(ptr).expect("returned NULL"),
            ))
        }
    }

    /// Constructs the immutable [`GmodPathQuery`].
    pub fn build(&self) -> GmodPathQuery {
        // SAFETY: self is non-null and valid for the lifetime of `&self`. Returns an owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_path_query_builder_build(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_gmod_path_query_builder_build returned NULL"
        );
        GmodPathQuery::from_owned_ptr(ptr)
    }
}

/// An owned, independently-released [`GmodPathQueryBuilderRef`].
pub struct GmodPathQueryBuilder(pub(crate) NonNull<ffi::dnv_vista_sdk_gmod_path_query_builder_t>);

impl Drop for GmodPathQueryBuilder {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `GmodPathQueryBuilder` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_gmod_path_query_builder_free(self.0.as_ptr()) }
    }
}

impl std::ops::Deref for GmodPathQueryBuilder {
    type Target = GmodPathQueryBuilderRef;

    fn deref(&self) -> &GmodPathQueryBuilderRef {
        // SAFETY: self.0 is NonNull, so as_ptr() is always non-null and valid for `self`'s lifetime.
        unsafe { GmodPathQueryBuilderRef::from_ptr(self.0.as_ptr()) }
    }
}

impl GmodPathQueryBuilder {
    /// Creates an empty Nodes-variant builder.
    pub fn create() -> Self {
        // SAFETY: no arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_path_query_builder_create() };
        GmodPathQueryBuilder(
            NonNull::new(ptr).expect("dnv_vista_sdk_gmod_path_query_builder_create returned NULL"),
        )
    }

    /// Creates a Path-variant builder from an existing [`GmodPathRef`].
    pub fn from_path(path: &GmodPathRef) -> Result<Self, VistaError> {
        // SAFETY: path is non-null and valid for the call's duration. Returns an owned pointer or NULL.
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_path_query_builder_from(path.as_ffi_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(GmodPathQueryBuilder(
                NonNull::new(ptr).expect("from_path returned NULL"),
            ))
        }
    }
}

// SAFETY: GmodPathQueryBuilderRef is a borrowed view (`repr(transparent)`). Sync only, not Send.
unsafe impl Sync for GmodPathQueryBuilderRef {}

// SAFETY: GmodPathQueryBuilder owns its heap-allocated C++ object exclusively.
unsafe impl Send for GmodPathQueryBuilder {}
unsafe impl Sync for GmodPathQueryBuilder {}
