use std::ffi::{CStr, CString};
use std::ptr;
use std::str::FromStr;

use crate::core::codebook_name::{codebook_names, CodebookName};
use crate::core::gmod_path::GmodPathRef;
use crate::core::local_id_builder::LocalIdBuilderRef;
use crate::core::metadata_tag::MetadataTagRef;
use crate::core::parsing_errors::ParsingErrors;
use crate::core::vis_version::VisVersion;
use crate::ffi::core::local_id as ffi;

/// Immutable representation of a VIS Local ID.
///
/// Represents a unique sensor identifier within a vessel according to the
/// dnv-v2 naming rule.
///
/// Borrowed - obtained via accessors like [`crate::core::universal_id::UniversalIdRef::local_id`].
/// See [`LocalId`] for the owned variant returned by parsing/building functions.
#[repr(transparent)]
pub struct LocalIdRef(ffi::dnv_vista_sdk_local_id_t);

impl LocalIdRef {
    pub(crate) fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_local_id_t) -> &'a Self {
        assert!(!ptr.is_null(), "local id pointer must not be NULL");
        // SAFETY: ptr is non-null and `LocalIdRef` is `#[repr(transparent)]` over the FFI type.
        unsafe { &*(ptr as *const LocalIdRef) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_local_id_t {
        self as *const Self as *const ffi::dnv_vista_sdk_local_id_t
    }

    /// ISO 19848 Annex C naming rule identifier (`"dnv-v2"`).
    pub fn naming_rule() -> &'static str {
        // SAFETY: no arguments. Returns a non-null, statically-owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_naming_rule() };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_naming_rule returned NULL"
        );
        // SAFETY: ptr is non-null and owned by the library for the program's lifetime.
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in local id naming rule")
    }

    /// VIS version this LocalIdRef was built for.
    pub fn version(&self) -> VisVersion {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_version(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_version returned NULL"
        );
        // SAFETY: ptr is non-null and owned by `self` for its lifetime.
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in local id version");
        VisVersion::from_str(s).expect("unrecognized local id version string")
    }

    /// Primary item (Gmod path) of this LocalIdRef.
    pub fn primary_item(&self) -> &GmodPathRef {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_primary_item(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_primary_item returned NULL"
        );
        GmodPathRef::from_ptr(ptr)
    }

    /// Secondary item (Gmod path), if set.
    pub fn secondary_item(&self) -> Option<&GmodPathRef> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_secondary_item(self.as_ffi_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(GmodPathRef::from_ptr(ptr))
        }
    }

    /// Whether verbose mode is enabled.
    pub fn is_verbose_mode(&self) -> bool {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_local_id_is_verbose_mode(self.as_ffi_ptr()) != 0 }
    }

    /// Whether any metadata tag uses a non-standard value.
    pub fn has_custom_tag(&self) -> bool {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_local_id_has_custom_tag(self.as_ffi_ptr()) != 0 }
    }

    /// Metadata tag for the given codebook slot, if set.
    pub fn metadata_tag(&self, name: CodebookName) -> Option<&MetadataTagRef> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_metadata_tag(
                self.as_ffi_ptr(),
                codebook_names::to_ffi(name),
            )
        };
        if ptr.is_null() {
            None
        } else {
            Some(MetadataTagRef::from_ptr(ptr))
        }
    }

    /// Get the Quantity metadata tag (`None` if not set)
    pub fn quantity(&self) -> Option<&MetadataTagRef> {
        self.metadata_tag(CodebookName::Quantity)
    }

    /// Get the Content metadata tag (`None` if not set)
    pub fn content(&self) -> Option<&MetadataTagRef> {
        self.metadata_tag(CodebookName::Content)
    }

    /// Get the Calculation metadata tag (`None` if not set)
    pub fn calculation(&self) -> Option<&MetadataTagRef> {
        self.metadata_tag(CodebookName::Calculation)
    }

    /// Get the State metadata tag (`None` if not set)
    pub fn state(&self) -> Option<&MetadataTagRef> {
        self.metadata_tag(CodebookName::State)
    }

    /// Get the Command metadata tag (`None` if not set)
    pub fn command(&self) -> Option<&MetadataTagRef> {
        self.metadata_tag(CodebookName::Command)
    }

    /// Get the Type metadata tag (`None` if not set)
    pub fn r#type(&self) -> Option<&MetadataTagRef> {
        self.metadata_tag(CodebookName::Type)
    }

    /// Get the Position metadata tag (`None` if not set)
    pub fn position(&self) -> Option<&MetadataTagRef> {
        self.metadata_tag(CodebookName::Position)
    }

    /// Get the Detail metadata tag (`None` if not set)
    pub fn detail(&self) -> Option<&MetadataTagRef> {
        self.metadata_tag(CodebookName::Detail)
    }

    /// All metadata tags that are set.
    pub fn metadata_tags(&self) -> Vec<&MetadataTagRef> {
        [
            CodebookName::Quantity,
            CodebookName::Content,
            CodebookName::Calculation,
            CodebookName::State,
            CodebookName::Command,
            CodebookName::Type,
            CodebookName::Position,
            CodebookName::Detail,
        ]
        .into_iter()
        .filter_map(|name| self.metadata_tag(name))
        .collect()
    }

    /// Builder that produced this LocalIdRef.
    pub fn builder(&self) -> &LocalIdBuilderRef {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_builder(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_builder returned NULL"
        );
        LocalIdBuilderRef::from_ptr(ptr)
    }
}

impl PartialEq for LocalIdRef {
    fn eq(&self, other: &Self) -> bool {
        // SAFETY: self and other are non-null and valid for the lifetime of `&self`/`&other`.
        unsafe { ffi::dnv_vista_sdk_local_id_equals(self.as_ffi_ptr(), other.as_ffi_ptr()) != 0 }
    }
}

impl Eq for LocalIdRef {}

impl std::fmt::Display for LocalIdRef {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        // SAFETY: self is non-null and valid for the lifetime of `&self`. Returns an owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_to_string(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_to_string returned NULL"
        );
        // SAFETY: ptr is non-null and owned until freed below.
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in local id string");
        let result = f.write_str(s);
        // SAFETY: ptr was allocated by the library and is freed exactly once.
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        result
    }
}

impl std::fmt::Debug for LocalIdRef {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "LocalId({})", self)
    }
}

/// An owned, independently-released [`LocalIdRef`].
pub struct LocalId(pub(crate) std::ptr::NonNull<ffi::dnv_vista_sdk_local_id_t>);

impl Drop for LocalId {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `LocalId` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_local_id_free(self.0.as_ptr()) }
    }
}

impl std::ops::Deref for LocalId {
    type Target = LocalIdRef;

    fn deref(&self) -> &LocalIdRef {
        LocalIdRef::from_ptr(self.0.as_ptr())
    }
}

impl PartialEq for LocalId {
    fn eq(&self, other: &Self) -> bool {
        **self == **other
    }
}

impl Eq for LocalId {}

impl std::fmt::Display for LocalId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        std::fmt::Display::fmt(&**self, f)
    }
}

impl std::fmt::Debug for LocalId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        std::fmt::Debug::fmt(&**self, f)
    }
}

impl LocalId {
    pub(crate) fn from_owned_ptr(ptr: *mut ffi::dnv_vista_sdk_local_id_t) -> Self {
        LocalId(std::ptr::NonNull::new(ptr).expect("local_id pointer must not be NULL"))
    }

    /// Parses a LocalIdRef string. Returns `None` on invalid input.
    pub fn from_string(local_id_str: &str) -> Option<Self> {
        let c_str = CString::new(local_id_str).ok()?;
        // SAFETY: c_str is a valid NUL-terminated C string. Returns an owned pointer or NULL.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_from_string(c_str.as_ptr()) };
        Some(LocalId(std::ptr::NonNull::new(ptr)?))
    }

    /// Parses a LocalIdRef string, returning the result together with any parse errors.
    pub fn from_string_with_errors(local_id_str: &str) -> (Option<Self>, ParsingErrors) {
        let c_str = CString::new(local_id_str).expect("local_id_str contains a NUL byte");
        let mut errors_ptr = ptr::null_mut();
        // SAFETY: c_str is a valid NUL-terminated C string, and `errors_ptr` is a valid
        // writable pointer to a local that the callee always sets.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_from_string_with_errors(c_str.as_ptr(), &mut errors_ptr)
        };
        assert!(
            !errors_ptr.is_null(),
            "dnv_vista_sdk_local_id_from_string_with_errors did not set outErrors"
        );
        let local_id = std::ptr::NonNull::new(ptr).map(LocalId);
        (local_id, ParsingErrors(errors_ptr))
    }
}
