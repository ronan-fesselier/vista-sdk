use std::ffi::CString;

use crate::core::error::{last_error, VistaError};
use crate::core::gmod_path::GmodPath;
use crate::core::local_id::LocalId;
use crate::ffi::query::local_id_query_builder as ffi;
use crate::query::gmod_path_query::GmodPathQuery;
use crate::query::local_id_query::LocalIdQuery;
use crate::query::metadata_tags_query::MetadataTagsQuery;

/// Builder for constructing [`LocalIdQuery`] instances with a fluent API.
///
/// Starts empty (matches all LocalIds) via [`OwnedLocalIdQueryBuilder::create`], or from an
/// existing [`LocalId`] via [`OwnedLocalIdQueryBuilder::from_local_id`] /
/// [`OwnedLocalIdQueryBuilder::from_string`].
///
/// The primary and secondary item queries accept either an exact [`GmodPath`] or a
/// [`GmodPathQuery`]; the metadata tags query is a [`MetadataTagsQuery`]. Every `with_x`
/// method returns a new, independent builder and does not mutate the receiver.
///
/// Borrowed - obtained via accessors. See [`OwnedLocalIdQueryBuilder`] for the owned variant.
#[repr(transparent)]
pub struct LocalIdQueryBuilder(ffi::dnv_vista_sdk_local_id_query_builder_t);

impl LocalIdQueryBuilder {
    pub(crate) fn from_ptr<'a>(
        ptr: *const ffi::dnv_vista_sdk_local_id_query_builder_t,
    ) -> &'a Self {
        assert!(
            !ptr.is_null(),
            "local id query builder pointer must not be NULL"
        );
        unsafe { &*(ptr as *const LocalIdQueryBuilder) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_local_id_query_builder_t {
        self as *const Self as *const ffi::dnv_vista_sdk_local_id_query_builder_t
    }

    /// Returns a new builder with the primary item matched exactly.
    pub fn with_primary_item(
        &self,
        primary_item: &GmodPath,
    ) -> Result<OwnedLocalIdQueryBuilder, VistaError> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_query_builder_with_primary_item(
                self.as_ffi_ptr(),
                primary_item.as_ffi_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedLocalIdQueryBuilder(ptr))
        }
    }

    /// Returns a new builder with the primary item matched by the given query.
    pub fn with_primary_item_query(
        &self,
        primary_item: &GmodPathQuery,
    ) -> Result<OwnedLocalIdQueryBuilder, VistaError> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_query_builder_with_primary_item_query(
                self.as_ffi_ptr(),
                primary_item.as_ffi_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedLocalIdQueryBuilder(ptr))
        }
    }

    /// Returns a new builder with the secondary item matched exactly.
    pub fn with_secondary_item(
        &self,
        secondary_item: &GmodPath,
    ) -> Result<OwnedLocalIdQueryBuilder, VistaError> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_query_builder_with_secondary_item(
                self.as_ffi_ptr(),
                secondary_item.as_ffi_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedLocalIdQueryBuilder(ptr))
        }
    }

    /// Returns a new builder with the secondary item matched by the given query.
    pub fn with_secondary_item_query(
        &self,
        secondary_item: &GmodPathQuery,
    ) -> Result<OwnedLocalIdQueryBuilder, VistaError> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_query_builder_with_secondary_item_query(
                self.as_ffi_ptr(),
                secondary_item.as_ffi_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedLocalIdQueryBuilder(ptr))
        }
    }

    /// Returns a new builder matching any LocalId regardless of secondary item presence.
    pub fn with_any_secondary_item(&self) -> Result<OwnedLocalIdQueryBuilder, VistaError> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_query_builder_with_any_secondary_item(self.as_ffi_ptr())
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedLocalIdQueryBuilder(ptr))
        }
    }

    /// Returns a new builder matching only LocalIds without a secondary item.
    pub fn without_secondary_item(&self) -> Result<OwnedLocalIdQueryBuilder, VistaError> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_query_builder_without_secondary_item(self.as_ffi_ptr())
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedLocalIdQueryBuilder(ptr))
        }
    }

    /// Returns a new builder with the given metadata tags query.
    pub fn with_tags(
        &self,
        tags: &MetadataTagsQuery,
    ) -> Result<OwnedLocalIdQueryBuilder, VistaError> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_query_builder_with_tags(
                self.as_ffi_ptr(),
                tags.as_ffi_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedLocalIdQueryBuilder(ptr))
        }
    }

    /// Returns a new builder with all location requirements removed from the
    /// primary and secondary items. Only affects Path-variant item queries.
    pub fn without_locations(&self) -> Result<OwnedLocalIdQueryBuilder, VistaError> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_query_builder_without_locations(self.as_ffi_ptr())
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedLocalIdQueryBuilder(ptr))
        }
    }

    /// Primary item path, if configured as an exact path.
    pub fn primary_item(&self) -> Option<&GmodPath> {
        let ptr =
            unsafe { ffi::dnv_vista_sdk_local_id_query_builder_primary_item(self.as_ffi_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(GmodPath::from_ptr(ptr))
        }
    }

    /// Secondary item path, if configured as an exact path.
    pub fn secondary_item(&self) -> Option<&GmodPath> {
        let ptr =
            unsafe { ffi::dnv_vista_sdk_local_id_query_builder_secondary_item(self.as_ffi_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(GmodPath::from_ptr(ptr))
        }
    }

    /// Constructs the immutable [`LocalIdQuery`].
    pub fn build(&self) -> LocalIdQuery {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_query_builder_build(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_query_builder_build returned NULL"
        );
        LocalIdQuery::from_owned_ptr(ptr)
    }
}

/// An owned, independently-released [`LocalIdQueryBuilder`].
pub struct OwnedLocalIdQueryBuilder(pub(crate) *mut ffi::dnv_vista_sdk_local_id_query_builder_t);

impl Drop for OwnedLocalIdQueryBuilder {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_local_id_query_builder_free(self.0) }
    }
}

impl std::ops::Deref for OwnedLocalIdQueryBuilder {
    type Target = LocalIdQueryBuilder;

    fn deref(&self) -> &LocalIdQueryBuilder {
        LocalIdQueryBuilder::from_ptr(self.0)
    }
}

impl OwnedLocalIdQueryBuilder {
    /// Creates an empty builder, matching all LocalIds.
    pub fn create() -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_query_builder_create() };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_query_builder_create returned NULL"
        );
        OwnedLocalIdQueryBuilder(ptr)
    }

    /// Creates a builder configured to match `local_id` exactly.
    pub fn from_local_id(local_id: &LocalId) -> Result<Self, VistaError> {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_query_builder_from(local_id.as_ffi_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedLocalIdQueryBuilder(ptr))
        }
    }

    /// Creates a builder configured to match the given LocalId string exactly.
    pub fn from_string(local_id_str: &str) -> Result<Self, VistaError> {
        let c_str = CString::new(local_id_str).expect("local_id_str contains a NUL byte");
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_query_builder_from_string(c_str.as_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedLocalIdQueryBuilder(ptr))
        }
    }
}
