use std::ffi::{CStr, CString};

use crate::core::imo_number::ImoNumber;
use crate::core::local_id::LocalId;
use crate::core::universal_id_builder::UniversalIdBuilder;
use crate::ffi::core::universal_id as ffi;

/// Immutable representation of a VIS Universal ID.
///
/// Combines an IMO number (vessel identification) with a LocalId (sensor
/// identification) to form a globally unique sensor identifier.
#[repr(transparent)]
pub struct UniversalId(ffi::dnv_vista_sdk_universal_id_t);

impl UniversalId {
    pub(crate) fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_universal_id_t) -> &'a Self {
        assert!(!ptr.is_null(), "universal id pointer must not be NULL");
        unsafe { &*(ptr as *const UniversalId) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_universal_id_t {
        self as *const Self as *const ffi::dnv_vista_sdk_universal_id_t
    }

    /// ISO 19848 Annex C naming entity (`"data.dnv.com"`).
    pub fn naming_entity() -> &'static str {
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_naming_entity() };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_universal_id_naming_entity returned NULL"
        );
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in universal id naming entity")
    }

    /// IMO vessel number associated with this UniversalId.
    pub fn imo_number(&self) -> ImoNumber {
        self.builder()
            .imo_number()
            .expect("a valid UniversalId always has an IMO number")
    }

    /// Sensor identifier embedded in this UniversalId.
    pub fn local_id(&self) -> &LocalId {
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_local_id(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_universal_id_local_id returned NULL"
        );
        LocalId::from_ptr(ptr)
    }

    /// Builder state underlying this UniversalId.
    pub fn builder(&self) -> &UniversalIdBuilder {
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_builder(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_universal_id_builder returned NULL"
        );
        UniversalIdBuilder::from_ptr(ptr)
    }
}

impl PartialEq for UniversalId {
    fn eq(&self, other: &Self) -> bool {
        unsafe {
            ffi::dnv_vista_sdk_universal_id_equals(self.as_ffi_ptr(), other.as_ffi_ptr()) != 0
        }
    }
}

impl Eq for UniversalId {}

impl std::fmt::Display for UniversalId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_to_string(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_universal_id_to_string returned NULL"
        );
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in universal id string");
        let result = f.write_str(s);
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        result
    }
}

impl std::fmt::Debug for UniversalId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "UniversalId({})", self)
    }
}

/// An owned, independently-released [`UniversalId`].
///
/// Returned by [`crate::core::universal_id_builder::UniversalIdBuilder::build`] and other
/// APIs that hand back a new universal id value rather than a reference into existing state.
pub struct OwnedUniversalId(pub(crate) *mut ffi::dnv_vista_sdk_universal_id_t);

impl Drop for OwnedUniversalId {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_universal_id_free(self.0) }
    }
}

impl std::ops::Deref for OwnedUniversalId {
    type Target = UniversalId;

    fn deref(&self) -> &UniversalId {
        UniversalId::from_ptr(self.0)
    }
}

impl PartialEq for OwnedUniversalId {
    fn eq(&self, other: &Self) -> bool {
        **self == **other
    }
}

impl Eq for OwnedUniversalId {}

impl std::fmt::Display for OwnedUniversalId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        std::fmt::Display::fmt(&**self, f)
    }
}

impl std::fmt::Debug for OwnedUniversalId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        std::fmt::Debug::fmt(&**self, f)
    }
}

impl OwnedUniversalId {
    pub(crate) fn from_owned_ptr(ptr: *mut ffi::dnv_vista_sdk_universal_id_t) -> Self {
        OwnedUniversalId(ptr)
    }

    /// Parses a UniversalId string. Returns `None` on invalid input.
    pub fn from_string(universal_id_str: &str) -> Option<Self> {
        let c_str = CString::new(universal_id_str).ok()?;
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_from_string(c_str.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(OwnedUniversalId(ptr))
        }
    }
}
