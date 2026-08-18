use std::ffi::{CStr, CString};
use std::os::raw::c_int;
use std::ptr;
use std::str::FromStr;

use crate::core::codebook_name::{codebook_names, CodebookName};
use crate::core::error::{last_error, VistaError};
use crate::core::gmod_path::GmodPath;
use crate::core::local_id::LocalId;
use crate::core::metadata_tag::MetadataTag;
use crate::core::parsing_errors::ParsingErrors;
use crate::core::vis_version::VisVersion;
use crate::ffi::core::local_id_builder as ffi;

/// Builder for constructing [`LocalIdRef`](crate::core::local_id::LocalIdRef) instances with a fluent API.
///
/// Borrowed - obtained via accessors like [`LocalIdRef::builder`](crate::core::local_id::LocalIdRef::builder). See
/// [`LocalIdBuilder`] for the owned variant returned by [`LocalIdBuilder::create`].
#[repr(transparent)]
pub struct LocalIdBuilderRef(ffi::dnv_vista_sdk_local_id_builder_t);

impl LocalIdBuilderRef {
    /// # Safety
    /// `ptr` must be non-null and valid for `'a`.
    pub(crate) unsafe fn from_ptr<'a>(
        ptr: *const ffi::dnv_vista_sdk_local_id_builder_t,
    ) -> &'a Self {
        // SAFETY: caller guarantees ptr is non-null and valid. `LocalIdBuilderRef` is `#[repr(transparent)]`.
        unsafe { &*(ptr as *const LocalIdBuilderRef) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_local_id_builder_t {
        self as *const Self as *const ffi::dnv_vista_sdk_local_id_builder_t
    }

    /// ISO 19848 Annex C naming rule identifier (`"dnv-v2"`).
    pub fn naming_rule() -> &'static str {
        // SAFETY: no arguments. Returns a non-null, statically-owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_builder_naming_rule() };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_builder_naming_rule returned NULL"
        );
        // SAFETY: ptr is non-null and owned by the library for the program's lifetime.
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in local id builder naming rule")
    }

    /// VIS version set on this builder, if any.
    pub fn version(&self) -> Option<VisVersion> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_builder_version(self.as_ffi_ptr()) };
        if ptr.is_null() {
            return None;
        }
        // SAFETY: ptr is non-null (checked above) and owned by `self` for its lifetime.
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in local id builder version");
        Some(VisVersion::from_str(s).expect("unrecognized local id builder version string"))
    }

    /// Whether verbose mode is enabled.
    pub fn is_verbose_mode(&self) -> bool {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_local_id_builder_is_verbose_mode(self.as_ffi_ptr()) != 0 }
    }

    /// Whether the builder state is complete enough to call [`build`](LocalIdBuilderRef::build).
    pub fn is_valid(&self) -> bool {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_local_id_builder_is_valid(self.as_ffi_ptr()) != 0 }
    }

    /// Whether no components have been set yet.
    pub fn is_empty(&self) -> bool {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_local_id_builder_is_empty(self.as_ffi_ptr()) != 0 }
    }

    /// Whether no metadata tags have been set.
    pub fn is_empty_metadata(&self) -> bool {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_local_id_builder_is_empty_metadata(self.as_ffi_ptr()) != 0 }
    }

    /// Whether any metadata tag uses a non-standard value.
    pub fn has_custom_tag(&self) -> bool {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_local_id_builder_has_custom_tag(self.as_ffi_ptr()) != 0 }
    }

    /// Primary item (Gmod path), if set.
    pub fn primary_item(&self) -> Option<GmodPath> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_builder_primary_item(self.as_ffi_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(GmodPath::from_owned_ptr(ptr))
        }
    }

    /// Secondary item (Gmod path), if set.
    pub fn secondary_item(&self) -> Option<GmodPath> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_builder_secondary_item(self.as_ffi_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(GmodPath::from_owned_ptr(ptr))
        }
    }

    /// Metadata tag for the given codebook slot, if set.
    pub fn metadata_tag(&self, name: CodebookName) -> Option<MetadataTag> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_builder_metadata_tag(
                self.as_ffi_ptr(),
                codebook_names::to_ffi(name),
            )
        };
        std::ptr::NonNull::new(ptr).map(MetadataTag)
    }

    /// Get the Quantity metadata tag (`None` if not set)
    pub fn quantity(&self) -> Option<MetadataTag> {
        self.metadata_tag(CodebookName::Quantity)
    }

    /// Get the Content metadata tag (`None` if not set)
    pub fn content(&self) -> Option<MetadataTag> {
        self.metadata_tag(CodebookName::Content)
    }

    /// Get the Calculation metadata tag (`None` if not set)
    pub fn calculation(&self) -> Option<MetadataTag> {
        self.metadata_tag(CodebookName::Calculation)
    }

    /// Get the State metadata tag (`None` if not set)
    pub fn state(&self) -> Option<MetadataTag> {
        self.metadata_tag(CodebookName::State)
    }

    /// Get the Command metadata tag (`None` if not set)
    pub fn command(&self) -> Option<MetadataTag> {
        self.metadata_tag(CodebookName::Command)
    }

    /// Get the Type metadata tag (`None` if not set)
    pub fn r#type(&self) -> Option<MetadataTag> {
        self.metadata_tag(CodebookName::Type)
    }

    /// Get the Position metadata tag (`None` if not set)
    pub fn position(&self) -> Option<MetadataTag> {
        self.metadata_tag(CodebookName::Position)
    }

    /// Get the Detail metadata tag (`None` if not set)
    pub fn detail(&self) -> Option<MetadataTag> {
        self.metadata_tag(CodebookName::Detail)
    }

    /// Returns a new builder with the VIS version set to `vis_version`.
    pub fn with_vis_version(&self, vis_version: VisVersion) -> Result<LocalIdBuilder, VistaError> {
        let c_version = CString::new(vis_version.as_str()).expect("invalid UTF-8 in VisVersion");
        // SAFETY: self and c_version are non-null/valid for the call's duration. Returns an owned pointer or NULL.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_builder_with_vis_version(
                self.as_ffi_ptr(),
                c_version.as_ptr(),
            )
        };
        std::ptr::NonNull::new(ptr)
            .map(LocalIdBuilder)
            .ok_or_else(last_error)
    }

    /// Returns a new builder with the VIS version cleared.
    pub fn without_vis_version(&self) -> LocalIdBuilder {
        // SAFETY: self is non-null and valid for the lifetime of `&self`. Returns an owned pointer.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_local_id_builder_without_vis_version(self.as_ffi_ptr()) };
        LocalIdBuilder(
            std::ptr::NonNull::new(ptr)
                .expect("dnv_vista_sdk_local_id_builder_without_vis_version returned NULL"),
        )
    }

    /// Returns a new builder with the primary item set to `path`.
    pub fn with_primary_item(&self, path: &crate::core::gmod_path::GmodPathRef) -> LocalIdBuilder {
        // SAFETY: self and path are non-null and valid for the call's duration. Returns an owned pointer.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_builder_with_primary_item(
                self.as_ffi_ptr(),
                path.as_ffi_ptr(),
            )
        };
        LocalIdBuilder(
            std::ptr::NonNull::new(ptr)
                .expect("dnv_vista_sdk_local_id_builder_with_primary_item returned NULL"),
        )
    }

    /// Returns a new builder with the primary item cleared.
    pub fn without_primary_item(&self) -> LocalIdBuilder {
        // SAFETY: self is non-null and valid for the lifetime of `&self`. Returns an owned pointer.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_local_id_builder_without_primary_item(self.as_ffi_ptr()) };
        LocalIdBuilder(
            std::ptr::NonNull::new(ptr)
                .expect("dnv_vista_sdk_local_id_builder_without_primary_item returned NULL"),
        )
    }

    /// Returns a new builder with the secondary item set to `path`.
    pub fn with_secondary_item(
        &self,
        path: &crate::core::gmod_path::GmodPathRef,
    ) -> LocalIdBuilder {
        // SAFETY: self and path are non-null and valid for the call's duration. Returns an owned pointer.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_builder_with_secondary_item(
                self.as_ffi_ptr(),
                path.as_ffi_ptr(),
            )
        };
        LocalIdBuilder(
            std::ptr::NonNull::new(ptr)
                .expect("dnv_vista_sdk_local_id_builder_with_secondary_item returned NULL"),
        )
    }

    /// Returns a new builder with the secondary item cleared.
    pub fn without_secondary_item(&self) -> LocalIdBuilder {
        // SAFETY: self is non-null and valid for the lifetime of `&self`. Returns an owned pointer.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_builder_without_secondary_item(self.as_ffi_ptr())
        };
        LocalIdBuilder(
            std::ptr::NonNull::new(ptr)
                .expect("dnv_vista_sdk_local_id_builder_without_secondary_item returned NULL"),
        )
    }

    /// Returns a new builder with `tag` set in the corresponding codebook slot.
    pub fn with_metadata_tag(
        &self,
        tag: &crate::core::metadata_tag::MetadataTagRef,
    ) -> LocalIdBuilder {
        // SAFETY: self and tag are non-null and valid for the call's duration. Returns an owned pointer.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_builder_with_metadata_tag(
                self.as_ffi_ptr(),
                tag.as_ffi_ptr(),
            )
        };
        LocalIdBuilder(
            std::ptr::NonNull::new(ptr)
                .expect("dnv_vista_sdk_local_id_builder_with_metadata_tag returned NULL"),
        )
    }

    /// Returns a new builder with the metadata tag for `name` cleared.
    pub fn without_metadata_tag(&self, name: CodebookName) -> LocalIdBuilder {
        // SAFETY: self is non-null and valid for the lifetime of `&self`. Returns an owned pointer.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_builder_without_metadata_tag(
                self.as_ffi_ptr(),
                codebook_names::to_ffi(name),
            )
        };
        LocalIdBuilder(
            std::ptr::NonNull::new(ptr)
                .expect("dnv_vista_sdk_local_id_builder_without_metadata_tag returned NULL"),
        )
    }

    /// Returns a new builder with verbose mode set to `verbose`.
    pub fn with_verbose_mode(&self, verbose: bool) -> LocalIdBuilder {
        // SAFETY: self is non-null and valid for the lifetime of `&self`. Returns an owned pointer.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_builder_with_verbose_mode(
                self.as_ffi_ptr(),
                verbose as c_int,
            )
        };
        LocalIdBuilder(
            std::ptr::NonNull::new(ptr)
                .expect("dnv_vista_sdk_local_id_builder_with_verbose_mode returned NULL"),
        )
    }

    /// Constructs the final [`LocalIdRef`](crate::core::local_id::LocalIdRef). Returns `Err` if the builder state is not valid.
    pub fn build(&self) -> Result<LocalId, VistaError> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`. Returns an owned pointer.or NULL.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_builder_build(self.as_ffi_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(LocalId::from_owned_ptr(ptr))
        }
    }
}

impl PartialEq for LocalIdBuilderRef {
    fn eq(&self, other: &Self) -> bool {
        // SAFETY: self and other are non-null and valid for the lifetime of `&self`/`&other`.
        unsafe {
            ffi::dnv_vista_sdk_local_id_builder_equals(self.as_ffi_ptr(), other.as_ffi_ptr()) != 0
        }
    }
}

impl Eq for LocalIdBuilderRef {}

impl std::fmt::Display for LocalIdBuilderRef {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        // SAFETY: self is non-null and valid for the lifetime of `&self`. Returns an owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_builder_to_string(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_builder_to_string returned NULL"
        );
        // SAFETY: ptr is non-null and allocated by the library; invalid UTF-8 is replaced lossily.
        unsafe { crate::ffi::core::common::display_owned_cstr(ptr, f) }
    }
}

/// An owned, independently-released [`LocalIdBuilderRef`].
///
/// Returned by [`LocalIdBuilder::create`] and other APIs that hand back
/// a new builder value rather than a reference into existing state.
pub struct LocalIdBuilder(pub(crate) std::ptr::NonNull<ffi::dnv_vista_sdk_local_id_builder_t>);

impl Drop for LocalIdBuilder {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `LocalIdBuilder` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_local_id_builder_free(self.0.as_ptr()) }
    }
}

impl std::ops::Deref for LocalIdBuilder {
    type Target = LocalIdBuilderRef;

    fn deref(&self) -> &LocalIdBuilderRef {
        // SAFETY: self.0 is NonNull, so as_ptr() is always non-null and valid for `self`'s lifetime.
        unsafe { LocalIdBuilderRef::from_ptr(self.0.as_ptr()) }
    }
}

impl std::fmt::Display for LocalIdBuilder {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        std::fmt::Display::fmt(&**self, f)
    }
}

impl LocalIdBuilder {
    /// Create a new LocalIdBuilderRef for the given VIS version
    pub fn create(vis_version: VisVersion) -> Self {
        let c_version = CString::new(vis_version.as_str()).expect("invalid UTF-8 in VisVersion");
        // SAFETY: c_version is a valid NUL-terminated C string. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_builder_create(c_version.as_ptr()) };
        LocalIdBuilder(
            std::ptr::NonNull::new(ptr)
                .expect("dnv_vista_sdk_local_id_builder_create returned NULL"),
        )
    }

    /// Parses a LocalIdRef string. Returns `None` on invalid input.
    pub fn from_string(local_id_str: &str) -> Option<LocalId> {
        let c_str = CString::new(local_id_str).ok()?;
        // SAFETY: c_str is a valid NUL-terminated C string. Returns an owned pointer or NULL.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_builder_from_string(c_str.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(LocalId::from_owned_ptr(ptr))
        }
    }

    /// Parses a LocalIdRef string, returning the result together with any parse errors.
    pub fn from_string_with_errors(local_id_str: &str) -> (Option<LocalId>, ParsingErrors) {
        let c_str = CString::new(local_id_str).expect("local_id_str contains a NUL byte");
        let mut errors_ptr = ptr::null_mut();
        // SAFETY: c_str is a valid NUL-terminated C string, and `errors_ptr` is a valid
        // writable pointer to a local that the callee always sets.
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
            Some(LocalId::from_owned_ptr(ptr))
        };
        (
            local_id,
            ParsingErrors(std::ptr::NonNull::new(errors_ptr).expect("errors_ptr must not be NULL")),
        )
    }
}

// SAFETY: LocalIdBuilderRef is a borrowed view (`repr(transparent)`). Sync only, not Send.
unsafe impl Sync for LocalIdBuilderRef {}

// SAFETY: LocalIdBuilder owns its heap-allocated C++ object exclusively.
unsafe impl Send for LocalIdBuilder {}
unsafe impl Sync for LocalIdBuilder {}
