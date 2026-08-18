use std::ffi::{CStr, CString};
use std::os::raw::c_int;
use std::ptr;
use std::str::FromStr;

use crate::core::codebook_name::{codebook_names, CodebookName};
use crate::core::error::{last_error, VistaError};
use crate::core::gmod_path::OwnedGmodPath;
use crate::core::local_id::OwnedLocalId;
use crate::core::metadata_tag::OwnedMetadataTag;
use crate::core::parsing_errors::ParsingErrors;
use crate::core::vis_version::VisVersion;
use crate::ffi::core::local_id_builder as ffi;

/// Builder for constructing [`LocalId`](crate::core::local_id::LocalId) instances with a fluent API.
///
/// Borrowed - obtained via accessors like [`LocalId::builder`](crate::core::local_id::LocalId::builder). See
/// [`OwnedLocalIdBuilder`] for the owned variant returned by [`OwnedLocalIdBuilder::create`].
#[repr(transparent)]
pub struct LocalIdBuilder(ffi::dnv_vista_sdk_local_id_builder_t);

impl LocalIdBuilder {
    pub(crate) fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_local_id_builder_t) -> &'a Self {
        assert!(!ptr.is_null(), "local id builder pointer must not be NULL");
        unsafe { &*(ptr as *const LocalIdBuilder) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_local_id_builder_t {
        self as *const Self as *const ffi::dnv_vista_sdk_local_id_builder_t
    }

    /// ISO 19848 Annex C naming rule identifier (`"dnv-v2"`).
    pub fn naming_rule() -> &'static str {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_builder_naming_rule() };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_builder_naming_rule returned NULL"
        );
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in local id builder naming rule")
    }

    /// VIS version set on this builder, if any.
    pub fn version(&self) -> Option<VisVersion> {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_builder_version(self.as_ffi_ptr()) };
        if ptr.is_null() {
            return None;
        }
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in local id builder version");
        Some(VisVersion::from_str(s).expect("unrecognized local id builder version string"))
    }

    /// Whether verbose mode is enabled.
    pub fn is_verbose_mode(&self) -> bool {
        unsafe { ffi::dnv_vista_sdk_local_id_builder_is_verbose_mode(self.as_ffi_ptr()) != 0 }
    }

    /// Whether the builder state is complete enough to call [`build`](LocalIdBuilder::build).
    pub fn is_valid(&self) -> bool {
        unsafe { ffi::dnv_vista_sdk_local_id_builder_is_valid(self.as_ffi_ptr()) != 0 }
    }

    /// Whether no components have been set yet.
    pub fn is_empty(&self) -> bool {
        unsafe { ffi::dnv_vista_sdk_local_id_builder_is_empty(self.as_ffi_ptr()) != 0 }
    }

    /// Whether no metadata tags have been set.
    pub fn is_empty_metadata(&self) -> bool {
        unsafe { ffi::dnv_vista_sdk_local_id_builder_is_empty_metadata(self.as_ffi_ptr()) != 0 }
    }

    /// Whether any metadata tag uses a non-standard value.
    pub fn has_custom_tag(&self) -> bool {
        unsafe { ffi::dnv_vista_sdk_local_id_builder_has_custom_tag(self.as_ffi_ptr()) != 0 }
    }

    /// Primary item (Gmod path), if set.
    pub fn primary_item(&self) -> Option<OwnedGmodPath> {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_builder_primary_item(self.as_ffi_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(OwnedGmodPath::from_owned_ptr(ptr))
        }
    }

    /// Secondary item (Gmod path), if set.
    pub fn secondary_item(&self) -> Option<OwnedGmodPath> {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_builder_secondary_item(self.as_ffi_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(OwnedGmodPath::from_owned_ptr(ptr))
        }
    }

    /// Metadata tag for the given codebook slot, if set.
    pub fn metadata_tag(&self, name: CodebookName) -> Option<OwnedMetadataTag> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_builder_metadata_tag(
                self.as_ffi_ptr(),
                codebook_names::to_ffi(name),
            )
        };
        if ptr.is_null() {
            None
        } else {
            Some(OwnedMetadataTag(ptr))
        }
    }

    /// Get the Quantity metadata tag (`None` if not set)
    pub fn quantity(&self) -> Option<OwnedMetadataTag> {
        self.metadata_tag(CodebookName::Quantity)
    }

    /// Get the Content metadata tag (`None` if not set)
    pub fn content(&self) -> Option<OwnedMetadataTag> {
        self.metadata_tag(CodebookName::Content)
    }

    /// Get the Calculation metadata tag (`None` if not set)
    pub fn calculation(&self) -> Option<OwnedMetadataTag> {
        self.metadata_tag(CodebookName::Calculation)
    }

    /// Get the State metadata tag (`None` if not set)
    pub fn state(&self) -> Option<OwnedMetadataTag> {
        self.metadata_tag(CodebookName::State)
    }

    /// Get the Command metadata tag (`None` if not set)
    pub fn command(&self) -> Option<OwnedMetadataTag> {
        self.metadata_tag(CodebookName::Command)
    }

    /// Get the Type metadata tag (`None` if not set)
    pub fn r#type(&self) -> Option<OwnedMetadataTag> {
        self.metadata_tag(CodebookName::Type)
    }

    /// Get the Position metadata tag (`None` if not set)
    pub fn position(&self) -> Option<OwnedMetadataTag> {
        self.metadata_tag(CodebookName::Position)
    }

    /// Get the Detail metadata tag (`None` if not set)
    pub fn detail(&self) -> Option<OwnedMetadataTag> {
        self.metadata_tag(CodebookName::Detail)
    }

    /// Returns a new builder with the VIS version set to `vis_version`.
    pub fn with_vis_version(
        &self,
        vis_version: VisVersion,
    ) -> Result<OwnedLocalIdBuilder, VistaError> {
        let c_version = CString::new(vis_version.as_str()).expect("invalid UTF-8 in VisVersion");
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_builder_with_vis_version(
                self.as_ffi_ptr(),
                c_version.as_ptr(),
            )
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedLocalIdBuilder(ptr))
        }
    }

    /// Returns a new builder with the VIS version cleared.
    pub fn without_vis_version(&self) -> OwnedLocalIdBuilder {
        let ptr =
            unsafe { ffi::dnv_vista_sdk_local_id_builder_without_vis_version(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_builder_without_vis_version returned NULL"
        );
        OwnedLocalIdBuilder(ptr)
    }

    /// Returns a new builder with the primary item set to `path`.
    pub fn with_primary_item(
        &self,
        path: &crate::core::gmod_path::GmodPath,
    ) -> OwnedLocalIdBuilder {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_builder_with_primary_item(
                self.as_ffi_ptr(),
                path.as_ffi_ptr(),
            )
        };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_builder_with_primary_item returned NULL"
        );
        OwnedLocalIdBuilder(ptr)
    }

    /// Returns a new builder with the primary item cleared.
    pub fn without_primary_item(&self) -> OwnedLocalIdBuilder {
        let ptr =
            unsafe { ffi::dnv_vista_sdk_local_id_builder_without_primary_item(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_builder_without_primary_item returned NULL"
        );
        OwnedLocalIdBuilder(ptr)
    }

    /// Returns a new builder with the secondary item set to `path`.
    pub fn with_secondary_item(
        &self,
        path: &crate::core::gmod_path::GmodPath,
    ) -> OwnedLocalIdBuilder {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_builder_with_secondary_item(
                self.as_ffi_ptr(),
                path.as_ffi_ptr(),
            )
        };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_builder_with_secondary_item returned NULL"
        );
        OwnedLocalIdBuilder(ptr)
    }

    /// Returns a new builder with the secondary item cleared.
    pub fn without_secondary_item(&self) -> OwnedLocalIdBuilder {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_builder_without_secondary_item(self.as_ffi_ptr())
        };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_builder_without_secondary_item returned NULL"
        );
        OwnedLocalIdBuilder(ptr)
    }

    /// Returns a new builder with `tag` set in the corresponding codebook slot.
    pub fn with_metadata_tag(
        &self,
        tag: &crate::core::metadata_tag::MetadataTag,
    ) -> OwnedLocalIdBuilder {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_builder_with_metadata_tag(
                self.as_ffi_ptr(),
                tag.as_ffi_ptr(),
            )
        };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_builder_with_metadata_tag returned NULL"
        );
        OwnedLocalIdBuilder(ptr)
    }

    /// Returns a new builder with the metadata tag for `name` cleared.
    pub fn without_metadata_tag(&self, name: CodebookName) -> OwnedLocalIdBuilder {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_builder_without_metadata_tag(
                self.as_ffi_ptr(),
                codebook_names::to_ffi(name),
            )
        };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_builder_without_metadata_tag returned NULL"
        );
        OwnedLocalIdBuilder(ptr)
    }

    /// Returns a new builder with verbose mode set to `verbose`.
    pub fn with_verbose_mode(&self, verbose: bool) -> OwnedLocalIdBuilder {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_builder_with_verbose_mode(
                self.as_ffi_ptr(),
                verbose as c_int,
            )
        };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_builder_with_verbose_mode returned NULL"
        );
        OwnedLocalIdBuilder(ptr)
    }

    /// Constructs the final [`LocalId`](crate::core::local_id::LocalId). Returns `Err` if the builder state is not valid.
    pub fn build(&self) -> Result<OwnedLocalId, VistaError> {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_builder_build(self.as_ffi_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(OwnedLocalId::from_owned_ptr(ptr))
        }
    }
}

impl PartialEq for LocalIdBuilder {
    fn eq(&self, other: &Self) -> bool {
        unsafe {
            ffi::dnv_vista_sdk_local_id_builder_equals(self.as_ffi_ptr(), other.as_ffi_ptr()) != 0
        }
    }
}

impl Eq for LocalIdBuilder {}

impl std::fmt::Display for LocalIdBuilder {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_builder_to_string(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_builder_to_string returned NULL"
        );
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in local id builder string");
        let result = f.write_str(s);
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        result
    }
}

/// An owned, independently-released [`LocalIdBuilder`].
///
/// Returned by [`OwnedLocalIdBuilder::create`] and other APIs that hand back
/// a new builder value rather than a reference into existing state.
pub struct OwnedLocalIdBuilder(pub(crate) *mut ffi::dnv_vista_sdk_local_id_builder_t);

impl Drop for OwnedLocalIdBuilder {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_local_id_builder_free(self.0) }
    }
}

impl std::ops::Deref for OwnedLocalIdBuilder {
    type Target = LocalIdBuilder;

    fn deref(&self) -> &LocalIdBuilder {
        LocalIdBuilder::from_ptr(self.0)
    }
}

impl std::fmt::Display for OwnedLocalIdBuilder {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        std::fmt::Display::fmt(&**self, f)
    }
}

impl OwnedLocalIdBuilder {
    /// Create a new LocalIdBuilder for the given VIS version
    pub fn create(vis_version: VisVersion) -> Self {
        let c_version = CString::new(vis_version.as_str()).expect("invalid UTF-8 in VisVersion");
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_builder_create(c_version.as_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_builder_create returned NULL"
        );
        OwnedLocalIdBuilder(ptr)
    }

    /// Parses a LocalId string. Returns `None` on invalid input.
    pub fn from_string(local_id_str: &str) -> Option<OwnedLocalId> {
        let c_str = CString::new(local_id_str).ok()?;
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_builder_from_string(c_str.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(OwnedLocalId::from_owned_ptr(ptr))
        }
    }

    /// Parses a LocalId string, returning the result together with any parse errors.
    pub fn from_string_with_errors(local_id_str: &str) -> (Option<OwnedLocalId>, ParsingErrors) {
        let c_str = CString::new(local_id_str).expect("local_id_str contains a NUL byte");
        let mut errors_ptr = ptr::null_mut();
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_builder_from_string_with_errors(
                c_str.as_ptr(),
                &mut errors_ptr,
            )
        };
        assert!(
            !errors_ptr.is_null(),
            "dnv_vista_sdk_local_id_builder_from_string_with_errors did not set outErrors"
        );
        let local_id = if ptr.is_null() {
            None
        } else {
            Some(OwnedLocalId::from_owned_ptr(ptr))
        };
        (local_id, ParsingErrors(errors_ptr))
    }
}
