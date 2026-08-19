use std::ffi::{CStr, CString};

use crate::core::imo_number::ImoNumber;
use crate::core::local_id::LocalIdRef;
use crate::core::universal_id_builder::UniversalIdBuilderRef;
use crate::ffi::core::universal_id as ffi;

/// Immutable representation of a VIS Universal ID.
///
/// Combines an IMO number (vessel identification) with a LocalIdRef (sensor
/// identification) to form a globally unique sensor identifier.
#[repr(transparent)]
pub struct UniversalIdRef(ffi::dnv_vista_sdk_universal_id_t);

impl UniversalIdRef {
    /// # Safety
    /// `ptr` must be non-null and valid for `'a`.
    pub(crate) unsafe fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_universal_id_t) -> &'a Self {
        // SAFETY: caller guarantees ptr is non-null and valid. `UniversalIdRef` is `#[repr(transparent)]`.
        unsafe { &*(ptr as *const UniversalIdRef) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_universal_id_t {
        self as *const Self as *const ffi::dnv_vista_sdk_universal_id_t
    }

    /// ISO 19848 Annex C naming entity (`"data.dnv.com"`).
    pub fn naming_entity() -> &'static str {
        // SAFETY: no arguments. Returns a non-null, statically-owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_naming_entity() };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_universal_id_naming_entity returned NULL"
        );
        // SAFETY: ptr is non-null and owned by the library for the program's lifetime.
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in universal id naming entity")
    }

    /// IMO vessel number associated with this UniversalIdRef.
    pub fn imo_number(&self) -> ImoNumber {
        self.builder()
            .imo_number()
            .expect("a valid UniversalId always has an IMO number")
    }

    /// Sensor identifier embedded in this UniversalIdRef.
    pub fn local_id(&self) -> &LocalIdRef {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_local_id(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_universal_id_local_id returned NULL"
        );
        // SAFETY: ptr is non-null (checked above) and valid for `self`'s lifetime.
        unsafe { LocalIdRef::from_ptr(ptr) }
    }

    /// Builder state underlying this UniversalIdRef.
    pub fn builder(&self) -> &UniversalIdBuilderRef {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_builder(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_universal_id_builder returned NULL"
        );
        // SAFETY: ptr is non-null (checked above) and valid for `self`'s lifetime.
        unsafe { UniversalIdBuilderRef::from_ptr(ptr) }
    }
}

impl PartialEq for UniversalIdRef {
    fn eq(&self, other: &Self) -> bool {
        // SAFETY: self and other are non-null and valid for the lifetime of `&self`/`&other`.
        unsafe {
            ffi::dnv_vista_sdk_universal_id_equals(self.as_ffi_ptr(), other.as_ffi_ptr()) != 0
        }
    }
}

impl Eq for UniversalIdRef {}

impl std::fmt::Display for UniversalIdRef {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        // SAFETY: self is non-null and valid for the lifetime of `&self`. Returns an owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_to_string(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_universal_id_to_string returned NULL"
        );
        // SAFETY: ptr is non-null and allocated by the library; invalid UTF-8 is replaced lossily.
        unsafe { crate::ffi::core::common::display_owned_cstr(ptr, f) }
    }
}

impl std::fmt::Debug for UniversalIdRef {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "UniversalId({})", self)
    }
}

/// An owned, independently-released [`UniversalIdRef`].
///
/// Returned by [`crate::core::universal_id_builder::UniversalIdBuilderRef::build`] and other
/// APIs that hand back a new universal id value rather than a reference into existing state.
pub struct UniversalId(pub(crate) std::ptr::NonNull<ffi::dnv_vista_sdk_universal_id_t>);

impl Drop for UniversalId {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `UniversalId` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_universal_id_free(self.0.as_ptr()) }
    }
}

impl std::ops::Deref for UniversalId {
    type Target = UniversalIdRef;

    fn deref(&self) -> &UniversalIdRef {
        // SAFETY: self.0 is NonNull, so as_ptr() is always non-null and valid for `self`'s lifetime.
        unsafe { UniversalIdRef::from_ptr(self.0.as_ptr()) }
    }
}

impl PartialEq for UniversalId {
    fn eq(&self, other: &Self) -> bool {
        **self == **other
    }
}

impl Eq for UniversalId {}

impl std::fmt::Display for UniversalId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        std::fmt::Display::fmt(&**self, f)
    }
}

impl std::fmt::Debug for UniversalId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        std::fmt::Debug::fmt(&**self, f)
    }
}

impl UniversalId {
    pub(crate) fn from_owned_ptr(ptr: *mut ffi::dnv_vista_sdk_universal_id_t) -> Self {
        UniversalId(std::ptr::NonNull::new(ptr).expect("universal id pointer must not be NULL"))
    }

    /// Parses a UniversalIdRef string. Returns `None` on invalid input.
    pub fn from_string(universal_id_str: &str) -> Option<Self> {
        let c_str = CString::new(universal_id_str).ok()?;
        // SAFETY: c_str is a valid NUL-terminated C string. Returns an owned pointer or NULL.
        let ptr = unsafe { ffi::dnv_vista_sdk_universal_id_from_string(c_str.as_ptr()) };
        std::ptr::NonNull::new(ptr).map(UniversalId)
    }
}

// SAFETY: UniversalIdRef is a borrowed view (`repr(transparent)`). Sync only, not Send.
unsafe impl Sync for UniversalIdRef {}

// SAFETY: UniversalId owns its heap-allocated C++ object exclusively.
unsafe impl Send for UniversalId {}
unsafe impl Sync for UniversalId {}
