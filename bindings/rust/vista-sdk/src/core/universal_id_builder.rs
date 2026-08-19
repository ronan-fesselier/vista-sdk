use std::ffi::{CStr, CString};
use std::ptr;

use crate::core::error::{last_error, VistaError};
use crate::core::imo_number::ImoNumber;
use crate::core::local_id_builder::{LocalIdBuilder, LocalIdBuilderRef};
use crate::core::parsing_errors::ParsingErrors;
use crate::core::universal_id::UniversalId;
use crate::core::vis_version::VisVersion;
use crate::ffi::core::universal_id_builder as ffi;

/// Builder for constructing [`UniversalIdRef`](crate::core::universal_id::UniversalIdRef) instances with a fluent API.
///
/// Borrowed - obtained via accessors like [`UniversalIdRef::builder`](crate::core::universal_id::UniversalIdRef::builder). See
/// [`UniversalIdBuilder`] for the owned variant returned by [`UniversalIdBuilder::create`].
#[repr(transparent)]
pub struct UniversalIdBuilderRef(ffi::dnv_vista_sdk_universal_id_builder_t);

impl UniversalIdBuilderRef {
    pub(crate) fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_universal_id_builder_t) -> &'a Self {
        assert!(
            !ptr.is_null(),
            "universal id builder pointer must not be NULL"
        );
        // SAFETY: ptr is non-null and `UniversalIdBuilderRef` is `#[repr(transparent)]` over the FFI type.
        unsafe { &*(ptr as *const UniversalIdBuilderRef) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_universal_id_builder_t {
        self as *const Self as *const ffi::dnv_vista_sdk_universal_id_builder_t
    }

    /// ISO 19848 Annex C naming entity (`"data.dnv.com"`).
    pub fn naming_entity() -> &'static str {
        // SAFETY: no arguments. Returns a non-null, statically-owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_builder_naming_entity() };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_universal_id_builder_naming_entity returned NULL"
        );
        // SAFETY: ptr is non-null and owned by the library for the program's lifetime.
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in universal id builder naming entity")
    }

    /// IMO vessel number, if set.
    pub fn imo_number(&self) -> Option<ImoNumber> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_builder_imo_number(self.as_ffi_ptr()) };
        if ptr.is_null() {
            None
        } else {
            // SAFETY: ptr is non-null (checked above), and freed exactly once right after reading the value.
            let v = unsafe { crate::ffi::core::imo_number::dnv_vista_sdk_imo_number_value(ptr) };
            unsafe { crate::ffi::core::imo_number::dnv_vista_sdk_imo_number_free(ptr) };
            Some(ImoNumber(v as u32))
        }
    }

    /// Sensor identifier builder, if set.
    pub fn local_id(&self) -> Option<LocalIdBuilder> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_builder_local_id(self.as_ffi_ptr()) };
        std::ptr::NonNull::new(ptr).map(LocalIdBuilder)
    }

    /// Whether the builder state is complete enough to call [`build`](UniversalIdBuilderRef::build).
    pub fn is_valid(&self) -> bool {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_universal_id_builder_is_valid(self.as_ffi_ptr()) != 0 }
    }

    /// Returns a new builder with the IMO number set to `imo_number`.
    pub fn with_imo_number(&self, imo_number: &ImoNumber) -> UniversalIdBuilder {
        // SAFETY: no pointer arguments. Returns an owned pointer.
        let imo_ptr = unsafe {
            crate::ffi::core::imo_number::dnv_vista_sdk_imo_number_create(imo_number.value() as i32)
        };
        assert!(
            !imo_ptr.is_null(),
            "dnv_vista_sdk_imo_number_create returned NULL"
        );
        // SAFETY: self and imo_ptr are non-null and valid for the call's duration.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_universal_id_builder_with_imo_number(self.as_ffi_ptr(), imo_ptr)
        };
        // SAFETY: imo_ptr was allocated above and is freed exactly once.
        unsafe { crate::ffi::core::imo_number::dnv_vista_sdk_imo_number_free(imo_ptr) };
        UniversalIdBuilder(
            std::ptr::NonNull::new(ptr)
                .expect("dnv_vista_sdk_universal_id_builder_with_imo_number returned NULL"),
        )
    }

    /// Returns a new builder with the IMO number cleared.
    pub fn without_imo_number(&self) -> UniversalIdBuilder {
        // SAFETY: self is non-null and valid for the lifetime of `&self`. Returns an owned pointer.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_universal_id_builder_without_imo_number(self.as_ffi_ptr())
        };
        UniversalIdBuilder(
            std::ptr::NonNull::new(ptr)
                .expect("dnv_vista_sdk_universal_id_builder_without_imo_number returned NULL"),
        )
    }

    /// Returns a new builder with the sensor identifier set to `local_id_builder`.
    pub fn with_local_id(&self, local_id_builder: &LocalIdBuilderRef) -> UniversalIdBuilder {
        // SAFETY: self and local_id_builder are non-null and valid for the call's duration.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_universal_id_builder_with_local_id(
                self.as_ffi_ptr(),
                local_id_builder.as_ffi_ptr(),
            )
        };
        UniversalIdBuilder(
            std::ptr::NonNull::new(ptr)
                .expect("dnv_vista_sdk_universal_id_builder_with_local_id returned NULL"),
        )
    }

    /// Returns a new builder with the sensor identifier cleared.
    pub fn without_local_id(&self) -> UniversalIdBuilder {
        // SAFETY: self is non-null and valid for the lifetime of `&self`. Returns an owned pointer.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_universal_id_builder_without_local_id(self.as_ffi_ptr()) };
        UniversalIdBuilder(
            std::ptr::NonNull::new(ptr)
                .expect("dnv_vista_sdk_universal_id_builder_without_local_id returned NULL"),
        )
    }

    /// Constructs the final [`UniversalIdRef`](crate::core::universal_id::UniversalIdRef). Returns `Err` if the builder state is not valid.
    pub fn build(&self) -> Result<UniversalId, VistaError> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`. Returns an owned pointer.or NULL.
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_builder_build(self.as_ffi_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(UniversalId::from_owned_ptr(ptr))
        }
    }
}

impl PartialEq for UniversalIdBuilderRef {
    fn eq(&self, other: &Self) -> bool {
        // SAFETY: self and other are non-null and valid for the lifetime of `&self`/`&other`.
        unsafe {
            ffi::dnv_vista_sdk_universal_id_builder_equals(self.as_ffi_ptr(), other.as_ffi_ptr())
                != 0
        }
    }
}

impl Eq for UniversalIdBuilderRef {}

impl std::fmt::Display for UniversalIdBuilderRef {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        // SAFETY: self is non-null and valid for the lifetime of `&self`. Returns an owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_builder_to_string(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_universal_id_builder_to_string returned NULL"
        );
        // SAFETY: ptr is non-null and owned until freed below.
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in universal id builder string");
        let result = f.write_str(s);
        // SAFETY: ptr was allocated by the library and is freed exactly once.
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        result
    }
}

/// An owned, independently-released [`UniversalIdBuilderRef`].
pub struct UniversalIdBuilder(
    pub(crate) std::ptr::NonNull<ffi::dnv_vista_sdk_universal_id_builder_t>,
);

impl Drop for UniversalIdBuilder {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `UniversalIdBuilder` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_universal_id_builder_free(self.0.as_ptr()) }
    }
}

impl std::ops::Deref for UniversalIdBuilder {
    type Target = UniversalIdBuilderRef;

    fn deref(&self) -> &UniversalIdBuilderRef {
        UniversalIdBuilderRef::from_ptr(self.0.as_ptr())
    }
}

impl PartialEq for UniversalIdBuilder {
    fn eq(&self, other: &Self) -> bool {
        **self == **other
    }
}

impl Eq for UniversalIdBuilder {}

impl std::fmt::Display for UniversalIdBuilder {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        std::fmt::Display::fmt(&**self, f)
    }
}

impl UniversalIdBuilder {
    /// Creates a new UniversalIdBuilderRef for the given VIS version.
    pub fn create(vis_version: VisVersion) -> Self {
        let c_version = CString::new(vis_version.as_str()).expect("invalid UTF-8 in VisVersion");
        // SAFETY: c_version is a valid NUL-terminated C string. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_builder_create(c_version.as_ptr()) };
        UniversalIdBuilder(
            std::ptr::NonNull::new(ptr)
                .expect("dnv_vista_sdk_universal_id_builder_create returned NULL"),
        )
    }

    /// Parses a UniversalIdRef string. Returns `None` on invalid input.
    pub fn from_string(universal_id_str: &str) -> Option<UniversalId> {
        let c_str = CString::new(universal_id_str).ok()?;
        // SAFETY: c_str is a valid NUL-terminated C string. Returns an owned pointer or NULL.
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_builder_from_string(c_str.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(UniversalId::from_owned_ptr(ptr))
        }
    }

    /// Parses a UniversalIdRef string, returning the result together with any parse errors.
    pub fn from_string_with_errors(universal_id_str: &str) -> (Option<UniversalId>, ParsingErrors) {
        let c_str = CString::new(universal_id_str).expect("universal_id_str contains a NUL byte");
        let mut errors_ptr = ptr::null_mut();
        // SAFETY: c_str is a valid NUL-terminated C string, and `errors_ptr` is a valid
        // writable pointer to a local that the callee always sets.
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
            Some(UniversalId::from_owned_ptr(ptr))
        };
        (universal_id, ParsingErrors(errors_ptr))
    }
}
