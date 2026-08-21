use std::ffi::CString;

use crate::core::error::{last_error, VistaError};
use crate::core::gmod_path::GmodPathRef;
use crate::core::local_id::LocalIdRef;
use crate::ffi::query::local_id_query_builder as ffi;
use crate::query::gmod_path_query::GmodPathQuery;
use crate::query::local_id_query::LocalIdQuery;
use crate::query::metadata_tags_query::MetadataTagsQuery;
use crate::query::metadata_tags_query_builder::MetadataTagsQueryBuilderRef;

/// Builder for constructing [`LocalIdQuery`] instances with a fluent API.
///
/// Starts empty (matches all LocalIds) via [`LocalIdQueryBuilder::create`], or from an
/// existing [`LocalIdRef`] via [`LocalIdQueryBuilder::from_local_id`] /
/// [`LocalIdQueryBuilder::from_string`].
///
/// The primary and secondary item queries accept either an exact [`GmodPathRef`] or a
/// [`GmodPathQuery`]. The metadata tags query is a [`MetadataTagsQuery`]. Every `with_x`
/// method returns a new, independent builder and does not mutate the receiver.
///
/// Borrowed - obtained via accessors. See [`LocalIdQueryBuilder`] for the owned variant.
#[repr(transparent)]
pub struct LocalIdQueryBuilderRef(ffi::dnv_vista_sdk_local_id_query_builder_t);

impl LocalIdQueryBuilderRef {
    pub(crate) fn from_ptr<'a>(
        ptr: *const ffi::dnv_vista_sdk_local_id_query_builder_t,
    ) -> &'a Self {
        assert!(
            !ptr.is_null(),
            "local id query builder pointer must not be NULL"
        );
        // SAFETY: ptr is non-null and `LocalIdQueryBuilderRef` is `#[repr(transparent)]` over the FFI type.
        unsafe { &*(ptr as *const LocalIdQueryBuilderRef) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_local_id_query_builder_t {
        self as *const Self as *const ffi::dnv_vista_sdk_local_id_query_builder_t
    }

    /// Returns a new builder with the primary item matched exactly.
    pub fn with_primary_item(
        &self,
        primary_item: &GmodPathRef,
    ) -> Result<LocalIdQueryBuilder, VistaError> {
        // SAFETY: self and primary_item are non-null and valid for the call's duration.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_query_builder_with_primary_item(
                self.as_ffi_ptr(),
                primary_item.as_ffi_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(LocalIdQueryBuilder(ptr))
        }
    }

    /// Returns a new builder with the primary item matched by the given query.
    pub fn with_primary_item_query(
        &self,
        primary_item: &GmodPathQuery,
    ) -> Result<LocalIdQueryBuilder, VistaError> {
        // SAFETY: self and primary_item are non-null and valid for the call's duration.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_query_builder_with_primary_item_query(
                self.as_ffi_ptr(),
                primary_item.as_ffi_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(LocalIdQueryBuilder(ptr))
        }
    }

    /// Returns a new builder with the secondary item matched exactly.
    pub fn with_secondary_item(
        &self,
        secondary_item: &GmodPathRef,
    ) -> Result<LocalIdQueryBuilder, VistaError> {
        // SAFETY: self and secondary_item are non-null and valid for the call's duration.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_query_builder_with_secondary_item(
                self.as_ffi_ptr(),
                secondary_item.as_ffi_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(LocalIdQueryBuilder(ptr))
        }
    }

    /// Returns a new builder with the secondary item matched by the given query.
    pub fn with_secondary_item_query(
        &self,
        secondary_item: &GmodPathQuery,
    ) -> Result<LocalIdQueryBuilder, VistaError> {
        // SAFETY: self and secondary_item are non-null and valid for the call's duration.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_query_builder_with_secondary_item_query(
                self.as_ffi_ptr(),
                secondary_item.as_ffi_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(LocalIdQueryBuilder(ptr))
        }
    }

    /// Returns a new builder matching any LocalIdRef regardless of secondary item presence.
    pub fn with_any_secondary_item(&self) -> Result<LocalIdQueryBuilder, VistaError> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_query_builder_with_any_secondary_item(self.as_ffi_ptr())
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(LocalIdQueryBuilder(ptr))
        }
    }

    /// Returns a new builder matching only LocalIds without a secondary item.
    pub fn without_secondary_item(&self) -> Result<LocalIdQueryBuilder, VistaError> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_query_builder_without_secondary_item(self.as_ffi_ptr())
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(LocalIdQueryBuilder(ptr))
        }
    }

    /// Returns a new builder with the given metadata tags query.
    pub fn with_tags(&self, tags: &MetadataTagsQuery) -> Result<LocalIdQueryBuilder, VistaError> {
        // SAFETY: self and tags are non-null and valid for the call's duration.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_query_builder_with_tags(
                self.as_ffi_ptr(),
                tags.as_ffi_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(LocalIdQueryBuilder(ptr))
        }
    }

    /// Returns a new builder with all location requirements removed from the
    /// primary and secondary items. Only affects Path-variant item queries.
    pub fn without_locations(&self) -> Result<LocalIdQueryBuilder, VistaError> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_query_builder_without_locations(self.as_ffi_ptr())
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(LocalIdQueryBuilder(ptr))
        }
    }

    /// Primary item path, if configured as an exact path.
    pub fn primary_item(&self) -> Option<&GmodPathRef> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_local_id_query_builder_primary_item(self.as_ffi_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(GmodPathRef::from_ptr(ptr))
        }
    }

    /// Secondary item path, if configured as an exact path.
    pub fn secondary_item(&self) -> Option<&GmodPathRef> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_local_id_query_builder_secondary_item(self.as_ffi_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(GmodPathRef::from_ptr(ptr))
        }
    }

    /// Returns the current tags query builder, or `None` if no tags query is configured.
    pub fn tags_builder(&self) -> Option<&MetadataTagsQueryBuilderRef> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_local_id_query_builder_tags_builder(self.as_ffi_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(MetadataTagsQueryBuilderRef::from_ptr(ptr))
        }
    }

    /// Constructs the immutable [`LocalIdQuery`].
    pub fn build(&self) -> LocalIdQuery {
        // SAFETY: self is non-null and valid for the lifetime of `&self`. Returns an owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_query_builder_build(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_query_builder_build returned NULL"
        );
        LocalIdQuery::from_owned_ptr(ptr)
    }
}

/// An owned, independently-released [`LocalIdQueryBuilderRef`].
pub struct LocalIdQueryBuilder(pub(crate) *mut ffi::dnv_vista_sdk_local_id_query_builder_t);

impl Drop for LocalIdQueryBuilder {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `LocalIdQueryBuilder` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_local_id_query_builder_free(self.0) }
    }
}

impl std::ops::Deref for LocalIdQueryBuilder {
    type Target = LocalIdQueryBuilderRef;

    fn deref(&self) -> &LocalIdQueryBuilderRef {
        LocalIdQueryBuilderRef::from_ptr(self.0)
    }
}

impl LocalIdQueryBuilder {
    /// Creates an empty builder, matching all LocalIds.
    pub fn create() -> Self {
        // SAFETY: no arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_query_builder_create() };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_query_builder_create returned NULL"
        );
        LocalIdQueryBuilder(ptr)
    }

    /// Creates a builder configured to match `local_id` exactly.
    pub fn from_local_id(local_id: &LocalIdRef) -> Result<Self, VistaError> {
        // SAFETY: local_id is non-null and valid for the call's duration. Returns an owned pointer or NULL.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_query_builder_from(local_id.as_ffi_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(LocalIdQueryBuilder(ptr))
        }
    }

    /// Creates a builder configured to match the given LocalIdRef string exactly.
    pub fn from_string(local_id_str: &str) -> Result<Self, VistaError> {
        let c_str = CString::new(local_id_str).expect("local_id_str contains a NUL byte");
        // SAFETY: c_str is a valid NUL-terminated C string. Returns an owned pointer or NULL.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_query_builder_from_string(c_str.as_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(LocalIdQueryBuilder(ptr))
        }
    }
}
