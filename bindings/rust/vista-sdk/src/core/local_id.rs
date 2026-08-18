use std::ffi::{CStr, CString};
use std::ptr;
use std::str::FromStr;

use crate::core::codebook_name::{codebook_names, CodebookName};
use crate::core::gmod_path::GmodPath;
use crate::core::local_id_builder::LocalIdBuilder;
use crate::core::metadata_tag::MetadataTag;
use crate::core::parsing_errors::ParsingErrors;
use crate::core::vis_version::VisVersion;
use crate::ffi::core::local_id as ffi;

/// Immutable representation of a VIS Local ID.
///
/// Represents a unique sensor identifier within a vessel according to the
/// dnv-v2 naming rule.
///
/// Borrowed - obtained via accessors like [`crate::core::universal_id::UniversalId::local_id`].
/// See [`OwnedLocalId`] for the owned variant returned by parsing/building functions.
#[repr(transparent)]
pub struct LocalId(ffi::dnv_vista_sdk_local_id_t);

impl LocalId {
    pub(crate) fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_local_id_t) -> &'a Self {
        assert!(!ptr.is_null(), "local id pointer must not be NULL");
        unsafe { &*(ptr as *const LocalId) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_local_id_t {
        self as *const Self as *const ffi::dnv_vista_sdk_local_id_t
    }

    /// ISO 19848 Annex C naming rule identifier (`"dnv-v2"`).
    pub fn naming_rule() -> &'static str {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_naming_rule() };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_naming_rule returned NULL"
        );
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in local id naming rule")
    }

    /// VIS version this LocalId was built for.
    pub fn version(&self) -> VisVersion {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_version(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_version returned NULL"
        );
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in local id version");
        VisVersion::from_str(s).expect("unrecognized local id version string")
    }

    /// Primary item (Gmod path) of this LocalId.
    pub fn primary_item(&self) -> &GmodPath {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_primary_item(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_primary_item returned NULL"
        );
        GmodPath::from_ptr(ptr)
    }

    /// Secondary item (Gmod path), if set.
    pub fn secondary_item(&self) -> Option<&GmodPath> {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_secondary_item(self.as_ffi_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(GmodPath::from_ptr(ptr))
        }
    }

    /// Whether verbose mode is enabled.
    pub fn is_verbose_mode(&self) -> bool {
        unsafe { ffi::dnv_vista_sdk_local_id_is_verbose_mode(self.as_ffi_ptr()) != 0 }
    }

    /// Whether any metadata tag uses a non-standard value.
    pub fn has_custom_tag(&self) -> bool {
        unsafe { ffi::dnv_vista_sdk_local_id_has_custom_tag(self.as_ffi_ptr()) != 0 }
    }

    /// Metadata tag for the given codebook slot, if set.
    pub fn metadata_tag(&self, name: CodebookName) -> Option<&MetadataTag> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_metadata_tag(
                self.as_ffi_ptr(),
                codebook_names::to_ffi(name),
            )
        };
        if ptr.is_null() {
            None
        } else {
            Some(MetadataTag::from_ptr(ptr))
        }
    }

    /// Get the Quantity metadata tag (`None` if not set)
    pub fn quantity(&self) -> Option<&MetadataTag> {
        self.metadata_tag(CodebookName::Quantity)
    }

    /// Get the Content metadata tag (`None` if not set)
    pub fn content(&self) -> Option<&MetadataTag> {
        self.metadata_tag(CodebookName::Content)
    }

    /// Get the Calculation metadata tag (`None` if not set)
    pub fn calculation(&self) -> Option<&MetadataTag> {
        self.metadata_tag(CodebookName::Calculation)
    }

    /// Get the State metadata tag (`None` if not set)
    pub fn state(&self) -> Option<&MetadataTag> {
        self.metadata_tag(CodebookName::State)
    }

    /// Get the Command metadata tag (`None` if not set)
    pub fn command(&self) -> Option<&MetadataTag> {
        self.metadata_tag(CodebookName::Command)
    }

    /// Get the Type metadata tag (`None` if not set)
    pub fn r#type(&self) -> Option<&MetadataTag> {
        self.metadata_tag(CodebookName::Type)
    }

    /// Get the Position metadata tag (`None` if not set)
    pub fn position(&self) -> Option<&MetadataTag> {
        self.metadata_tag(CodebookName::Position)
    }

    /// Get the Detail metadata tag (`None` if not set)
    pub fn detail(&self) -> Option<&MetadataTag> {
        self.metadata_tag(CodebookName::Detail)
    }

    /// All metadata tags that are set.
    pub fn metadata_tags(&self) -> Vec<&MetadataTag> {
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

    /// Builder that produced this LocalId.
    pub fn builder(&self) -> &LocalIdBuilder {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_builder(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_builder returned NULL"
        );
        LocalIdBuilder::from_ptr(ptr)
    }
}

impl PartialEq for LocalId {
    fn eq(&self, other: &Self) -> bool {
        unsafe { ffi::dnv_vista_sdk_local_id_equals(self.as_ffi_ptr(), other.as_ffi_ptr()) != 0 }
    }
}

impl Eq for LocalId {}

impl std::fmt::Display for LocalId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_to_string(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_to_string returned NULL"
        );
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in local id string");
        let result = f.write_str(s);
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        result
    }
}

impl std::fmt::Debug for LocalId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "LocalId({})", self)
    }
}

/// An owned, independently-released [`LocalId`].
pub struct OwnedLocalId(pub(crate) *mut ffi::dnv_vista_sdk_local_id_t);

impl Drop for OwnedLocalId {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_local_id_free(self.0) }
    }
}

impl std::ops::Deref for OwnedLocalId {
    type Target = LocalId;

    fn deref(&self) -> &LocalId {
        LocalId::from_ptr(self.0)
    }
}

impl PartialEq for OwnedLocalId {
    fn eq(&self, other: &Self) -> bool {
        **self == **other
    }
}

impl Eq for OwnedLocalId {}

impl std::fmt::Display for OwnedLocalId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        std::fmt::Display::fmt(&**self, f)
    }
}

impl std::fmt::Debug for OwnedLocalId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        std::fmt::Debug::fmt(&**self, f)
    }
}

impl OwnedLocalId {
    pub(crate) fn from_owned_ptr(ptr: *mut ffi::dnv_vista_sdk_local_id_t) -> Self {
        OwnedLocalId(ptr)
    }

    /// Parses a LocalId string. Returns `None` on invalid input.
    pub fn from_string(local_id_str: &str) -> Option<Self> {
        let c_str = CString::new(local_id_str).ok()?;
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_from_string(c_str.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(OwnedLocalId(ptr))
        }
    }

    /// Parses a LocalId string, returning the result together with any parse errors.
    pub fn from_string_with_errors(local_id_str: &str) -> (Option<Self>, ParsingErrors) {
        let c_str = CString::new(local_id_str).expect("local_id_str contains a NUL byte");
        let mut errors_ptr = ptr::null_mut();
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_from_string_with_errors(c_str.as_ptr(), &mut errors_ptr)
        };
        assert!(
            !errors_ptr.is_null(),
            "dnv_vista_sdk_local_id_from_string_with_errors did not set outErrors"
        );
        let local_id = if ptr.is_null() {
            None
        } else {
            Some(OwnedLocalId(ptr))
        };
        (local_id, ParsingErrors(errors_ptr))
    }
}
