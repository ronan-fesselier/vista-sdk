use std::ffi::{CStr, CString};
use std::ptr;

use crate::core::error::{last_error, VistaError};
use crate::core::imo_number::ImoNumber;
use crate::core::local_id_builder::{LocalIdBuilder, OwnedLocalIdBuilder};
use crate::core::parsing_errors::ParsingErrors;
use crate::core::universal_id::OwnedUniversalId;
use crate::core::vis_version::VisVersion;
use crate::ffi::core::universal_id_builder as ffi;

/// Builder for constructing [`UniversalId`](crate::core::universal_id::UniversalId) instances with a fluent API.
///
/// Borrowed - obtained via accessors like [`UniversalId::builder`](crate::core::universal_id::UniversalId::builder). See
/// [`OwnedUniversalIdBuilder`] for the owned variant returned by [`OwnedUniversalIdBuilder::create`].
#[repr(transparent)]
pub struct UniversalIdBuilder(ffi::dnv_vista_sdk_universal_id_builder_t);

impl UniversalIdBuilder {
    pub(crate) fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_universal_id_builder_t) -> &'a Self {
        assert!(
            !ptr.is_null(),
            "universal id builder pointer must not be NULL"
        );
        unsafe { &*(ptr as *const UniversalIdBuilder) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_universal_id_builder_t {
        self as *const Self as *const ffi::dnv_vista_sdk_universal_id_builder_t
    }

    /// ISO 19848 Annex C naming entity (`"data.dnv.com"`).
    pub fn naming_entity() -> &'static str {
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_builder_naming_entity() };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_universal_id_builder_naming_entity returned NULL"
        );
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in universal id builder naming entity")
    }

    /// IMO vessel number, if set.
    pub fn imo_number(&self) -> Option<ImoNumber> {
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_builder_imo_number(self.as_ffi_ptr()) };
        if ptr.is_null() {
            None
        } else {
            let v = unsafe { crate::ffi::core::imo_number::dnv_vista_sdk_imo_number_value(ptr) };
            unsafe { crate::ffi::core::imo_number::dnv_vista_sdk_imo_number_free(ptr) };
            Some(ImoNumber(v as u32))
        }
    }

    /// Sensor identifier builder, if set.
    pub fn local_id(&self) -> Option<OwnedLocalIdBuilder> {
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_builder_local_id(self.as_ffi_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(OwnedLocalIdBuilder(ptr))
        }
    }

    /// Whether the builder state is complete enough to call [`build`](UniversalIdBuilder::build).
    pub fn is_valid(&self) -> bool {
        unsafe { ffi::dnv_vista_sdk_universal_id_builder_is_valid(self.as_ffi_ptr()) != 0 }
    }

    /// Returns a new builder with the IMO number set to `imo_number`.
    pub fn with_imo_number(&self, imo_number: &ImoNumber) -> OwnedUniversalIdBuilder {
        let imo_ptr = unsafe {
            crate::ffi::core::imo_number::dnv_vista_sdk_imo_number_create(imo_number.value() as i32)
        };
        assert!(
            !imo_ptr.is_null(),
            "dnv_vista_sdk_imo_number_create returned NULL"
        );
        let ptr = unsafe {
            ffi::dnv_vista_sdk_universal_id_builder_with_imo_number(self.as_ffi_ptr(), imo_ptr)
        };
        unsafe { crate::ffi::core::imo_number::dnv_vista_sdk_imo_number_free(imo_ptr) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_universal_id_builder_with_imo_number returned NULL"
        );
        OwnedUniversalIdBuilder(ptr)
    }

    /// Returns a new builder with the IMO number cleared.
    pub fn without_imo_number(&self) -> OwnedUniversalIdBuilder {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_universal_id_builder_without_imo_number(self.as_ffi_ptr())
        };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_universal_id_builder_without_imo_number returned NULL"
        );
        OwnedUniversalIdBuilder(ptr)
    }

    /// Returns a new builder with the sensor identifier set to `local_id_builder`.
    pub fn with_local_id(&self, local_id_builder: &LocalIdBuilder) -> OwnedUniversalIdBuilder {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_universal_id_builder_with_local_id(
                self.as_ffi_ptr(),
                local_id_builder.as_ffi_ptr(),
            )
        };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_universal_id_builder_with_local_id returned NULL"
        );
        OwnedUniversalIdBuilder(ptr)
    }

    /// Returns a new builder with the sensor identifier cleared.
    pub fn without_local_id(&self) -> OwnedUniversalIdBuilder {
        let ptr =
            unsafe { ffi::dnv_vista_sdk_universal_id_builder_without_local_id(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_universal_id_builder_without_local_id returned NULL"
        );
        OwnedUniversalIdBuilder(ptr)
    }

    /// Constructs the final [`UniversalId`](crate::core::universal_id::UniversalId). Returns `Err` if the builder state is not valid.
    pub fn build(&self) -> Result<OwnedUniversalId, VistaError> {
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_builder_build(self.as_ffi_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedUniversalId::from_owned_ptr(ptr))
        }
    }
}

impl PartialEq for UniversalIdBuilder {
    fn eq(&self, other: &Self) -> bool {
        unsafe {
            ffi::dnv_vista_sdk_universal_id_builder_equals(self.as_ffi_ptr(), other.as_ffi_ptr())
                != 0
        }
    }
}

impl Eq for UniversalIdBuilder {}

impl std::fmt::Display for UniversalIdBuilder {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_builder_to_string(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_universal_id_builder_to_string returned NULL"
        );
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in universal id builder string");
        let result = f.write_str(s);
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        result
    }
}

/// An owned, independently-released [`UniversalIdBuilder`].
pub struct OwnedUniversalIdBuilder(pub(crate) *mut ffi::dnv_vista_sdk_universal_id_builder_t);

impl Drop for OwnedUniversalIdBuilder {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_universal_id_builder_free(self.0) }
    }
}

impl std::ops::Deref for OwnedUniversalIdBuilder {
    type Target = UniversalIdBuilder;

    fn deref(&self) -> &UniversalIdBuilder {
        UniversalIdBuilder::from_ptr(self.0)
    }
}

impl PartialEq for OwnedUniversalIdBuilder {
    fn eq(&self, other: &Self) -> bool {
        **self == **other
    }
}

impl Eq for OwnedUniversalIdBuilder {}

impl std::fmt::Display for OwnedUniversalIdBuilder {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        std::fmt::Display::fmt(&**self, f)
    }
}

impl OwnedUniversalIdBuilder {
    /// Creates a new UniversalIdBuilder for the given VIS version.
    pub fn create(vis_version: VisVersion) -> Self {
        let c_version = CString::new(vis_version.as_str()).expect("invalid UTF-8 in VisVersion");
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_builder_create(c_version.as_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_universal_id_builder_create returned NULL"
        );
        OwnedUniversalIdBuilder(ptr)
    }

    /// Parses a UniversalId string. Returns `None` on invalid input.
    pub fn from_string(universal_id_str: &str) -> Option<OwnedUniversalId> {
        let c_str = CString::new(universal_id_str).ok()?;
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_builder_from_string(c_str.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(OwnedUniversalId::from_owned_ptr(ptr))
        }
    }

    /// Parses a UniversalId string, returning the result together with any parse errors.
    pub fn from_string_with_errors(
        universal_id_str: &str,
    ) -> (Option<OwnedUniversalId>, ParsingErrors) {
        let c_str = CString::new(universal_id_str).expect("universal_id_str contains a NUL byte");
        let mut errors_ptr = ptr::null_mut();
        let ptr = unsafe {
            ffi::dnv_vista_sdk_universal_id_builder_from_string_with_errors(
                c_str.as_ptr(),
                &mut errors_ptr,
            )
        };
        assert!(
            !errors_ptr.is_null(),
            "dnv_vista_sdk_universal_id_builder_from_string_with_errors did not set outErrors"
        );
        let universal_id = if ptr.is_null() {
            None
        } else {
            Some(OwnedUniversalId::from_owned_ptr(ptr))
        };
        (universal_id, ParsingErrors(errors_ptr))
    }
}
