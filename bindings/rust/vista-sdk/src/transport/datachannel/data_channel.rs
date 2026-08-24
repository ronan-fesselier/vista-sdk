use std::ffi::{CStr, CString};
use std::ptr::NonNull;

use crate::core::error::{last_error, VistaError};
use crate::core::local_id::LocalIdRef;
use crate::ffi::core::common::dnv_vista_sdk_string_free;
use crate::ffi::core::local_id as local_id_ffi;
use crate::ffi::transport::datachannel::data_channel as ffi;
use crate::transport::serializable_document::{SerializableDocument, SerializableDocumentRef};
use crate::transport::ship_id::ShipId;
use crate::types::date_time_offset::DateTimeOffset;

/// XML Schema white-space normalization mode (ISO 19848 Table 19).
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum WhiteSpace {
    /// Preserve all white-space characters as-is.
    Preserve,
    /// Replace each white-space character with a single space.
    Replace,
    /// Collapse consecutive white-space into a single space and strip leading/trailing spaces.
    Collapse,
}

fn white_space_to_ffi(ws: WhiteSpace) -> ffi::dnv_vista_sdk_dcl_white_space_t {
    match ws {
        WhiteSpace::Preserve => ffi::dnv_vista_sdk_dcl_white_space_t::Preserve,
        WhiteSpace::Replace => ffi::dnv_vista_sdk_dcl_white_space_t::Replace,
        WhiteSpace::Collapse => ffi::dnv_vista_sdk_dcl_white_space_t::Collapse,
    }
}

fn white_space_from_ffi(ws: ffi::dnv_vista_sdk_dcl_white_space_t) -> WhiteSpace {
    match ws {
        ffi::dnv_vista_sdk_dcl_white_space_t::Preserve => WhiteSpace::Preserve,
        ffi::dnv_vista_sdk_dcl_white_space_t::Replace => WhiteSpace::Replace,
        ffi::dnv_vista_sdk_dcl_white_space_t::Collapse => WhiteSpace::Collapse,
    }
}

/// SAFETY: caller must ensure `ptr` is either null or a valid NUL-terminated C string
/// owned for the duration of the call.
unsafe fn opt_str(ptr: *const std::ffi::c_char) -> Option<String> {
    if ptr.is_null() {
        None
    } else {
        Some(CStr::from_ptr(ptr).to_string_lossy().into_owned())
    }
}

/// SAFETY: caller must ensure `ptr` is non-null, a valid NUL-terminated C string owned
/// for the duration of the call.
unsafe fn req_str(ptr: *const std::ffi::c_char) -> String {
    CStr::from_ptr(ptr).to_string_lossy().into_owned()
}

/// ISO 19848 DataChannelList package - top-level container for transmission.
pub struct DataChannelListPackage(NonNull<ffi::dnv_vista_sdk_dcl_list_package_t>);

impl DataChannelListPackage {
    /// Creates a DataChannelList package from the given package.
    pub fn new(package: &Package) -> Self {
        // SAFETY: package is non-null and valid for the call's duration. Returns an owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_list_package_create(package.as_ptr()) };
        Self(NonNull::new(ptr).expect("list_package_create returned null"))
    }

    /// Returns a new list package with the inner package set.
    #[must_use]
    pub fn with_package(self, package: &Package) -> Self {
        // SAFETY: self.0 and package are non-null and valid for the call's duration.
        unsafe {
            ffi::dnv_vista_sdk_dcl_list_package_set_package(self.0.as_ptr(), package.as_ptr())
        };
        self
    }

    /// Returns `true` if this list package has an inner package set.
    pub fn has_package(&self) -> bool {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        !(unsafe { ffi::dnv_vista_sdk_dcl_list_package_package(self.0.as_ptr()) }).is_null()
    }

    /// Returns a borrowed view of the data channel list, or `None` if not set.
    pub fn data_channel_list(&self) -> Option<DataChannelListRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_list_package_data_channel_list(self.0.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(DataChannelListRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Returns a borrowed view of the inner package, or `None` if not set.
    pub fn package(&self) -> Option<PackageRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_list_package_package(self.0.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(PackageRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_list_package_t {
        self.0.as_ptr()
    }

    /// Constructs a [`DataChannelListPackage`] from an owned raw pointer.
    ///
    /// # Safety
    /// `ptr` must be a non-null pointer returned by `dnv_vista_sdk_dcl_list_package_from_json`
    /// or another C API constructor that transfers ownership.
    pub(crate) unsafe fn from_raw(ptr: *mut ffi::dnv_vista_sdk_dcl_list_package_t) -> Self {
        Self(NonNull::new(ptr).expect("from_raw called with null pointer"))
    }
}

impl Drop for DataChannelListPackage {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `DataChannelListPackage` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_dcl_list_package_free(self.0.as_ptr()) };
    }
}

/// ISO 19848 Table 9 - package combining a [`Header`] and a [`DataChannelList`].
pub struct Package(NonNull<ffi::dnv_vista_sdk_dcl_package_t>);

impl Package {
    /// Creates a package from the given header and data channel list.
    pub fn new(header: &Header, data_channel_list: &DataChannelList) -> Self {
        // SAFETY: header and data_channel_list are non-null and valid for the call's duration.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_dcl_package_create(header.as_ptr(), data_channel_list.as_ptr())
        };
        Self(NonNull::new(ptr).expect("package_create returned null"))
    }

    /// Returns a new package with the header set.
    #[must_use]
    pub fn with_header(self, header: &Header) -> Self {
        // SAFETY: self.0 and header are non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_package_set_header(self.0.as_ptr(), header.as_ptr()) };
        self
    }

    /// Returns a new package with the data channel list set.
    #[must_use]
    pub fn with_data_channel_list(self, data_channel_list: &DataChannelList) -> Self {
        // SAFETY: self.0 and data_channel_list are non-null and valid for the call's duration.
        unsafe {
            ffi::dnv_vista_sdk_dcl_package_set_data_channel_list(
                self.0.as_ptr(),
                data_channel_list.as_ptr(),
            )
        };
        self
    }

    /// Returns `true` if this package has a header set.
    pub fn has_header(&self) -> bool {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        !(unsafe { ffi::dnv_vista_sdk_dcl_package_header(self.0.as_ptr()) }).is_null()
    }

    /// Returns `true` if this package has a data channel list set.
    pub fn has_data_channel_list(&self) -> bool {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        !(unsafe { ffi::dnv_vista_sdk_dcl_package_data_channel_list(self.0.as_ptr()) }).is_null()
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_package_t {
        self.0.as_ptr()
    }
}

impl Drop for Package {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `Package` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_dcl_package_free(self.0.as_ptr()) };
    }
}

/// Borrowed view of a [`Package`] obtained via [`DataChannelListPackage::package`].
pub struct PackageRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_package_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_package_t>,
}

impl<'a> PackageRef<'a> {
    /// Returns a borrowed view of the header, or `None` if not set.
    pub fn header(&self) -> Option<HeaderRef<'_>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_package_header(self.ptr) };
        if ptr.is_null() {
            None
        } else {
            Some(HeaderRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }
}

/// ISO 19848 Table 10 - header of a DataChannelList package.
pub struct Header(NonNull<ffi::dnv_vista_sdk_dcl_header_t>);

impl Header {
    /// Creates a header with the given ship identifier and data channel list ID.
    pub fn new(ship_id: &ShipId, data_channel_list_id: &ConfigurationReference) -> Self {
        // SAFETY: ship_ptr and data_channel_list_id are non-null and valid for the call's duration.
        let ptr = ship_id.with_ffi_ptr(|ship_ptr| unsafe {
            ffi::dnv_vista_sdk_dcl_header_create(ship_ptr, data_channel_list_id.as_ptr())
        });
        Self(NonNull::new(ptr).expect("header_create returned null"))
    }

    /// Returns a new header with the ship identifier set.
    #[must_use]
    pub fn with_ship_id(self, ship_id: &ShipId) -> Self {
        // SAFETY: self.0 and ship_ptr are non-null and valid for the call's duration.
        ship_id.with_ffi_ptr(|ship_ptr| unsafe {
            ffi::dnv_vista_sdk_dcl_header_set_ship_id(self.0.as_ptr(), ship_ptr)
        });
        self
    }

    /// Returns a new header with the data channel list ID set.
    #[must_use]
    pub fn with_data_channel_list_id(self, data_channel_list_id: &ConfigurationReference) -> Self {
        // SAFETY: self.0 and data_channel_list_id are non-null and valid for the call's duration.
        unsafe {
            ffi::dnv_vista_sdk_dcl_header_set_data_channel_list_id(
                self.0.as_ptr(),
                data_channel_list_id.as_ptr(),
            )
        };
        self
    }

    /// Returns a new header with the version information set.
    #[must_use]
    pub fn with_version_information(self, version_information: &VersionInformation) -> Self {
        // SAFETY: self.0 and version_information are non-null and valid for the call's duration.
        unsafe {
            ffi::dnv_vista_sdk_dcl_header_set_version_information(
                self.0.as_ptr(),
                version_information.as_ptr(),
            )
        };
        self
    }

    /// Returns a new instance with the version information cleared.
    #[must_use]
    pub fn without_version_information(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_header_clear_version_information(self.0.as_ptr()) };
        self
    }

    /// Returns the author string if set.
    pub fn author(&self) -> Option<String> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { opt_str(ffi::dnv_vista_sdk_dcl_header_author(self.0.as_ptr())) }
    }

    /// Returns a new header with the author string set.
    #[must_use]
    pub fn with_author(self, author: &str) -> Self {
        let c = CString::new(author).expect("author contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe { ffi::dnv_vista_sdk_dcl_header_set_author(self.0.as_ptr(), c.as_ptr()) };
        self
    }

    /// Returns a new instance with the author string cleared.
    #[must_use]
    pub fn without_author(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_header_clear_author(self.0.as_ptr()) };
        self
    }

    /// Returns the date-created timestamp if set.
    pub fn date_created(&self) -> Option<DateTimeOffset> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        if (unsafe { ffi::dnv_vista_sdk_dcl_header_has_date_created(self.0.as_ptr()) }) != 0 {
            Some(DateTimeOffset::from_ffi(unsafe {
                // SAFETY: self.0 is non-null and known (checked above) to have this field.
                ffi::dnv_vista_sdk_dcl_header_date_created(self.0.as_ptr())
            }))
        } else {
            None
        }
    }

    /// Returns a new header with the date-created timestamp set.
    #[must_use]
    pub fn with_date_created(self, date_created: DateTimeOffset) -> Self {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe {
            ffi::dnv_vista_sdk_dcl_header_set_date_created(self.0.as_ptr(), date_created.into_ffi())
        };
        self
    }

    /// Returns a new instance with the date-created timestamp cleared.
    #[must_use]
    pub fn without_date_created(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_header_clear_date_created(self.0.as_ptr()) };
        self
    }

    /// Returns a new header with the custom headers set, transferring ownership of `doc`.
    #[must_use]
    pub fn with_custom_headers(self, doc: SerializableDocument) -> Self {
        let raw = doc.into_raw();
        // SAFETY: self.0 is non-null, and raw transfers ownership of a non-null pointer.
        unsafe { ffi::dnv_vista_sdk_dcl_header_set_custom_headers(self.0.as_ptr(), raw) };
        self
    }

    /// Returns a new instance with the custom headers cleared.
    #[must_use]
    pub fn without_custom_headers(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_header_clear_custom_headers(self.0.as_ptr()) };
        self
    }

    /// Returns the stored ship ID, or `None` if absent.
    pub fn ship_id(&self) -> Option<ShipId> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let stored = unsafe { ffi::dnv_vista_sdk_dcl_header_ship_id(self.0.as_ptr()) };
        if stored.is_null() {
            return None;
        }
        Some(ShipId::from_ffi_ptr(stored))
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_header_t {
        self.0.as_ptr()
    }
}

impl Drop for Header {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `Header` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_dcl_header_free(self.0.as_ptr()) };
    }
}

/// Borrowed view of a [`Header`] obtained via [`PackageRef::header`].
pub struct HeaderRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_header_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_header_t>,
}

impl<'a> HeaderRef<'a> {
    /// Returns `true` if the ship ID is set.
    pub fn ship_id_is_some(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        !(unsafe { ffi::dnv_vista_sdk_dcl_header_ship_id(self.ptr) }).is_null()
    }

    /// Returns a borrowed view of the data channel list configuration reference.
    pub fn data_channel_list_id(&self) -> ConfigurationReferenceRef<'_> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_header_data_channel_list_id(self.ptr) };
        ConfigurationReferenceRef {
            ptr,
            _marker: std::marker::PhantomData,
        }
    }

    /// Returns a borrowed view of the version information if set.
    pub fn version_information(&self) -> Option<VersionInformationRef<'_>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_header_version_information(self.ptr) };
        if ptr.is_null() {
            None
        } else {
            Some(VersionInformationRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Returns a borrowed view of the custom headers if set.
    pub fn custom_headers(&self) -> Option<SerializableDocumentRef<'_>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_header_custom_headers(self.ptr) };
        if ptr.is_null() {
            None
        } else {
            // SAFETY: ptr is non-null (checked above) and valid for `self`'s lifetime.
            Some(unsafe { SerializableDocumentRef::from_ptr(ptr) })
        }
    }
}

/// ISO 19848 Table 11 - reference to a named configuration version.
pub struct ConfigurationReference(NonNull<ffi::dnv_vista_sdk_dcl_configuration_reference_t>);

impl ConfigurationReference {
    /// Creates a configuration reference with the given id and timestamp.
    pub fn new(id: &str, timestamp: DateTimeOffset) -> Self {
        let c = CString::new(id).expect("id contains NUL byte");
        // SAFETY: c is a valid NUL-terminated C string. Returns an owned pointer.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_dcl_configuration_reference_create(c.as_ptr(), timestamp.into_ffi())
        };
        Self(NonNull::new(ptr).expect("configuration_reference_create returned null"))
    }

    /// Returns the configuration identifier.
    pub fn id(&self) -> String {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe {
            req_str(ffi::dnv_vista_sdk_dcl_configuration_reference_id(
                self.0.as_ptr(),
            ))
        }
    }

    /// Returns a new configuration reference with the identifier set.
    #[must_use]
    pub fn with_id(self, id: &str) -> Self {
        let c = CString::new(id).expect("id contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe {
            ffi::dnv_vista_sdk_dcl_configuration_reference_set_id(self.0.as_ptr(), c.as_ptr())
        };
        self
    }

    /// Returns the version string if set.
    pub fn version(&self) -> Option<String> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe {
            opt_str(ffi::dnv_vista_sdk_dcl_configuration_reference_version(
                self.0.as_ptr(),
            ))
        }
    }

    /// Returns a new configuration reference with the version string set.
    #[must_use]
    pub fn with_version(self, version: &str) -> Self {
        let c = CString::new(version).expect("version contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe {
            ffi::dnv_vista_sdk_dcl_configuration_reference_set_version(self.0.as_ptr(), c.as_ptr())
        };
        self
    }

    /// Returns a new instance with the version string cleared.
    #[must_use]
    pub fn without_version(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_configuration_reference_clear_version(self.0.as_ptr()) };
        self
    }

    /// Returns the timestamp.
    pub fn timestamp(&self) -> DateTimeOffset {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        DateTimeOffset::from_ffi(unsafe {
            ffi::dnv_vista_sdk_dcl_configuration_reference_timestamp(self.0.as_ptr())
        })
    }

    /// Returns a new configuration reference with the timestamp set.
    #[must_use]
    pub fn with_timestamp(self, timestamp: DateTimeOffset) -> Self {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe {
            ffi::dnv_vista_sdk_dcl_configuration_reference_set_timestamp(
                self.0.as_ptr(),
                timestamp.into_ffi(),
            )
        };
        self
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_configuration_reference_t {
        self.0.as_ptr()
    }
}

impl Drop for ConfigurationReference {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `ConfigurationReference` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_dcl_configuration_reference_free(self.0.as_ptr()) };
    }
}

/// Borrowed view of a [`ConfigurationReference`] obtained via [`HeaderRef::data_channel_list_id`].
pub struct ConfigurationReferenceRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_configuration_reference_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_configuration_reference_t>,
}

impl<'a> ConfigurationReferenceRef<'a> {
    /// Returns the identifier string.
    pub fn id(&self) -> String {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { req_str(ffi::dnv_vista_sdk_dcl_configuration_reference_id(self.ptr)) }
    }

    /// Returns the timestamp.
    pub fn timestamp(&self) -> DateTimeOffset {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        DateTimeOffset::from_ffi(unsafe {
            ffi::dnv_vista_sdk_dcl_configuration_reference_timestamp(self.ptr)
        })
    }
}

/// ISO 19848 Table 12 - naming rule and scheme version for a data channel list.
pub struct VersionInformation(NonNull<ffi::dnv_vista_sdk_dcl_version_information_t>);

impl VersionInformation {
    /// Creates version information with no fields set.
    pub fn new() -> Self {
        // SAFETY: no arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_version_information_create_default() };
        Self(NonNull::new(ptr).expect("version_information_create_default returned null"))
    }

    /// Creates version information with the given naming rule and scheme version.
    pub fn with_fields(naming_rule: &str, naming_scheme_version: &str) -> Self {
        let nr = CString::new(naming_rule).expect("naming_rule contains NUL byte");
        let nsv =
            CString::new(naming_scheme_version).expect("naming_scheme_version contains NUL byte");
        // SAFETY: nr and nsv are valid NUL-terminated C strings. Returns an owned pointer.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_dcl_version_information_create(nr.as_ptr(), nsv.as_ptr()) };
        Self(NonNull::new(ptr).expect("version_information_create returned null"))
    }

    /// Returns the naming rule.
    pub fn naming_rule(&self) -> String {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe {
            req_str(ffi::dnv_vista_sdk_dcl_version_information_naming_rule(
                self.0.as_ptr(),
            ))
        }
    }

    /// Returns a new version information with the naming rule set.
    #[must_use]
    pub fn with_naming_rule(self, naming_rule: &str) -> Self {
        let c = CString::new(naming_rule).expect("naming_rule contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe {
            ffi::dnv_vista_sdk_dcl_version_information_set_naming_rule(self.0.as_ptr(), c.as_ptr())
        };
        self
    }

    /// Returns the naming scheme version.
    pub fn naming_scheme_version(&self) -> String {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe {
            req_str(
                ffi::dnv_vista_sdk_dcl_version_information_naming_scheme_version(self.0.as_ptr()),
            )
        }
    }

    /// Returns a new version information with the naming scheme version set.
    #[must_use]
    pub fn with_naming_scheme_version(self, naming_scheme_version: &str) -> Self {
        let c =
            CString::new(naming_scheme_version).expect("naming_scheme_version contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe {
            ffi::dnv_vista_sdk_dcl_version_information_set_naming_scheme_version(
                self.0.as_ptr(),
                c.as_ptr(),
            )
        };
        self
    }

    /// Returns the reference URL if set.
    pub fn reference_url(&self) -> Option<String> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe {
            opt_str(ffi::dnv_vista_sdk_dcl_version_information_reference_url(
                self.0.as_ptr(),
            ))
        }
    }

    /// Returns a new version information with the reference URL set.
    #[must_use]
    pub fn with_reference_url(self, reference_url: &str) -> Self {
        let c = CString::new(reference_url).expect("reference_url contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe {
            ffi::dnv_vista_sdk_dcl_version_information_set_reference_url(
                self.0.as_ptr(),
                c.as_ptr(),
            )
        };
        self
    }

    /// Returns a new instance with the reference URL cleared.
    #[must_use]
    pub fn without_reference_url(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_version_information_clear_reference_url(self.0.as_ptr()) };
        self
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_version_information_t {
        self.0.as_ptr()
    }
}

impl Default for VersionInformation {
    fn default() -> Self {
        Self::new()
    }
}

impl Drop for VersionInformation {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `VersionInformation` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_dcl_version_information_free(self.0.as_ptr()) };
    }
}

/// Borrowed view of a [`VersionInformation`] obtained via [`HeaderRef::version_information`].
pub struct VersionInformationRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_version_information_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_version_information_t>,
}

impl<'a> VersionInformationRef<'a> {
    /// Returns the naming rule string.
    pub fn naming_rule(&self) -> String {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe {
            req_str(ffi::dnv_vista_sdk_dcl_version_information_naming_rule(
                self.ptr,
            ))
        }
    }

    /// Returns the naming scheme version if set.
    pub fn naming_scheme_version(&self) -> Option<String> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe {
            opt_str(ffi::dnv_vista_sdk_dcl_version_information_naming_scheme_version(self.ptr))
        }
    }

    /// Returns the reference URL if set.
    pub fn reference_url(&self) -> Option<String> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe {
            opt_str(ffi::dnv_vista_sdk_dcl_version_information_reference_url(
                self.ptr,
            ))
        }
    }
}

/// ISO 19848 Table 13 - ordered, indexed collection of [`DataChannel`] entries.
pub struct DataChannelList(NonNull<ffi::dnv_vista_sdk_dcl_data_channel_list_t>);

impl DataChannelList {
    /// Creates an empty data channel list.
    pub fn new() -> Self {
        // SAFETY: no arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_data_channel_list_create() };
        Self(NonNull::new(ptr).expect("data_channel_list_create returned null"))
    }

    /// Returns the number of data channels in the list.
    pub fn len(&self) -> usize {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_dcl_data_channel_list_size(self.0.as_ptr()) }
    }

    /// Returns `true` if the list contains no data channels.
    pub fn is_empty(&self) -> bool {
        self.len() == 0
    }

    /// Returns a borrowed view of the data channel at `index`, or `None` if out of range.
    pub fn at(&self, index: usize) -> Option<DataChannelRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_data_channel_list_at(self.0.as_ptr(), index) };
        if ptr.is_null() {
            None
        } else {
            Some(DataChannelRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Returns a borrowed view of the data channel with the given short ID, or `None`.
    pub fn from_short_id(&self, short_id: &str) -> Option<DataChannelRef<'_>> {
        let c = CString::new(short_id).expect("short_id contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_list_from_short_id(self.0.as_ptr(), c.as_ptr())
        };
        if ptr.is_null() {
            None
        } else {
            Some(DataChannelRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Returns a borrowed view of the data channel with the given Local ID, or `None`.
    pub fn from_local_id(&self, local_id: &LocalIdRef) -> Option<DataChannelRef<'_>> {
        // SAFETY: self.0 and local_id are non-null and valid for the call's duration.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_list_from_local_id(
                self.0.as_ptr(),
                local_id.as_ffi_ptr(),
            )
        };
        if ptr.is_null() {
            None
        } else {
            Some(DataChannelRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Adds a data channel to the list.  Returns `false` if a duplicate key already exists.
    pub fn add(&mut self, data_channel: &DataChannel) -> bool {
        // SAFETY: self.0 and data_channel are non-null and valid for the call's duration.
        (unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_list_add(self.0.as_ptr(), data_channel.as_ptr())
        }) != 0
    }

    /// Removes a data channel from the list.  Returns `false` if not found.
    pub fn remove(&mut self, data_channel: &DataChannel) -> bool {
        // SAFETY: self.0 and data_channel are non-null and valid for the call's duration.
        (unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_list_remove(self.0.as_ptr(), data_channel.as_ptr())
        }) != 0
    }

    /// Removes all data channels from the list.
    pub fn clear(&mut self) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_dcl_data_channel_list_clear(self.0.as_ptr()) };
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_data_channel_list_t {
        self.0.as_ptr()
    }
}

impl Default for DataChannelList {
    fn default() -> Self {
        Self::new()
    }
}

impl Drop for DataChannelList {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `DataChannelList` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_dcl_data_channel_list_free(self.0.as_ptr()) };
    }
}

/// Borrowed view of a [`DataChannelList`] obtained via [`DataChannelListPackage::data_channel_list`].
pub struct DataChannelListRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_data_channel_list_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_data_channel_list_t>,
}

impl<'a> DataChannelListRef<'a> {
    /// Returns the number of data channels in the list.
    pub fn len(&self) -> usize {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { ffi::dnv_vista_sdk_dcl_data_channel_list_size(self.ptr) }
    }

    /// Returns `true` if the list contains no data channels.
    pub fn is_empty(&self) -> bool {
        self.len() == 0
    }

    /// Returns a borrowed view of the data channel at `index`, or `None` if out of range.
    pub fn at(&self, index: usize) -> Option<DataChannelRef<'_>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_data_channel_list_at(self.ptr, index) };
        if ptr.is_null() {
            None
        } else {
            Some(DataChannelRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Returns a borrowed view of the data channel with the given short ID, or `None` if not found.
    pub fn from_short_id(&self, short_id: &str) -> Option<DataChannelRef<'_>> {
        let c = std::ffi::CString::new(short_id).expect("short_id contains NUL byte");
        // SAFETY: self.ptr is non-null, and c is a valid NUL-terminated C string.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_dcl_data_channel_list_from_short_id(self.ptr, c.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(DataChannelRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Returns a borrowed view of the data channel with the given Local ID, or `None` if not found.
    pub fn from_local_id(&self, local_id: &LocalIdRef) -> Option<DataChannelRef<'_>> {
        // SAFETY: self.ptr and local_id are non-null and valid for the call's duration.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_list_from_local_id(self.ptr, local_id.as_ffi_ptr())
        };
        if ptr.is_null() {
            None
        } else {
            Some(DataChannelRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }
}

/// ISO 19848 Table 14 - a single data channel with its identifier and properties.
pub struct DataChannel(NonNull<ffi::dnv_vista_sdk_dcl_data_channel_t>);

impl DataChannel {
    /// Creates a data channel from the given identifier and property.
    ///
    /// # Panics
    /// Panics if `property` fails validation. Use [`DataChannel::try_new`] for fallible construction.
    pub fn new(channel_id: &DataChannelId, property: &Property) -> Self {
        Self::try_new(channel_id, property)
            .expect("data_channel_create returned null: property validation failed")
    }

    /// Creates a data channel from the given identifier and property.
    /// Returns `Err` if `property` fails validation (e.g. Decimal without Range/Unit).
    pub fn try_new(channel_id: &DataChannelId, property: &Property) -> Result<Self, VistaError> {
        // SAFETY: channel_id and property are non-null and valid for the call's duration.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_create(channel_id.as_ptr(), property.as_ptr())
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(Self(
                NonNull::new(ptr).expect("data_channel_create returned non-null"),
            ))
        }
    }

    /// Returns a new data channel with the channel identifier set.
    #[must_use]
    pub fn with_channel_id(self, channel_id: &DataChannelId) -> Self {
        // SAFETY: self.0 and channel_id are non-null and valid for the call's duration.
        unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_set_channel_id(self.0.as_ptr(), channel_id.as_ptr())
        };
        self
    }

    /// Returns a new data channel with the property set.
    /// Returns `Err` if validation of the new property fails (C++ throws `std::invalid_argument`).
    pub fn with_property(self, property: &Property) -> Result<Self, VistaError> {
        // SAFETY: self.0 and property are non-null and valid for the call's duration.
        let ok = unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_set_property(self.0.as_ptr(), property.as_ptr())
        } != 0;
        if ok {
            Ok(self)
        } else {
            Err(last_error())
        }
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_data_channel_t {
        self.0.as_ptr()
    }
}

impl Drop for DataChannel {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `DataChannel` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_dcl_data_channel_free(self.0.as_ptr()) };
    }
}

/// Borrowed view of a [`DataChannel`] obtained from [`DataChannelList`] accessor methods.
pub struct DataChannelRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_data_channel_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_data_channel_t>,
}

impl<'a> DataChannelRef<'a> {
    /// # Safety
    /// `ptr` must be non-null and valid for lifetime `'a`.
    pub(crate) unsafe fn from_ptr(ptr: *const ffi::dnv_vista_sdk_dcl_data_channel_t) -> Self {
        Self {
            ptr,
            _marker: std::marker::PhantomData,
        }
    }

    /// Returns a borrowed view of the channel identifier.
    pub fn channel_id(&self) -> DataChannelIdRef<'_> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_data_channel_channel_id(self.ptr) };
        DataChannelIdRef {
            ptr,
            _marker: std::marker::PhantomData,
        }
    }

    /// Returns a borrowed view of the property.
    pub fn property(&self) -> PropertyRef<'_> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_data_channel_property(self.ptr) };
        PropertyRef {
            ptr,
            _marker: std::marker::PhantomData,
        }
    }
}

/// ISO 19848 Table 15 - unique identifier for a data channel.
pub struct DataChannelId(NonNull<ffi::dnv_vista_sdk_dcl_channel_id_t>);

impl DataChannelId {
    /// Creates a data channel identifier from the given Local ID.
    pub fn new(local_id: &LocalIdRef) -> Self {
        // SAFETY: local_id is non-null and valid for the call's duration. Returns an owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_channel_id_create(local_id.as_ffi_ptr()) };
        Self(NonNull::new(ptr).expect("channel_id_create returned null"))
    }

    /// Returns a new data channel identifier with the Local ID set.
    #[must_use]
    pub fn with_local_id(self, local_id: &LocalIdRef) -> Self {
        // SAFETY: self.0 and local_id are non-null and valid for the call's duration.
        unsafe {
            ffi::dnv_vista_sdk_dcl_channel_id_set_local_id(self.0.as_ptr(), local_id.as_ffi_ptr())
        };
        self
    }

    /// Returns the short identifier if set.
    pub fn short_id(&self) -> Option<String> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { opt_str(ffi::dnv_vista_sdk_dcl_channel_id_short_id(self.0.as_ptr())) }
    }

    /// Returns a new data channel identifier with the short identifier set.
    #[must_use]
    pub fn with_short_id(self, short_id: &str) -> Self {
        let c = CString::new(short_id).expect("short_id contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe { ffi::dnv_vista_sdk_dcl_channel_id_set_short_id(self.0.as_ptr(), c.as_ptr()) };
        self
    }

    /// Returns a new instance with the short identifier cleared.
    #[must_use]
    pub fn without_short_id(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_channel_id_clear_short_id(self.0.as_ptr()) };
        self
    }

    /// Returns a new data channel identifier with the name object set.
    #[must_use]
    pub fn with_name_object(self, name_object: &NameObject) -> Self {
        // SAFETY: self.0 and name_object are non-null and valid for the call's duration.
        unsafe {
            ffi::dnv_vista_sdk_dcl_channel_id_set_name_object(self.0.as_ptr(), name_object.as_ptr())
        };
        self
    }

    /// Returns a new instance with the name object cleared.
    #[must_use]
    pub fn without_name_object(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_channel_id_clear_name_object(self.0.as_ptr()) };
        self
    }

    /// Returns a borrowed view of the name object if set.
    pub fn name_object(&self) -> Option<NameObjectRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_channel_id_name_object(self.0.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(NameObjectRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Returns the string representation of the stored Local ID, or `None` if not set.
    pub fn local_id_string(&self) -> Option<String> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let local_id_ptr = unsafe { ffi::dnv_vista_sdk_dcl_channel_id_local_id(self.0.as_ptr()) };
        if local_id_ptr.is_null() {
            return None;
        }
        // SAFETY: local_id_ptr is non-null (checked above) and valid for `self`'s lifetime. Returns an owned pointer or NULL.
        let s_ptr = unsafe { local_id_ffi::dnv_vista_sdk_local_id_to_string(local_id_ptr) };
        if s_ptr.is_null() {
            return None;
        }
        // SAFETY: s_ptr is non-null (checked above) and owned until freed below.
        let s = unsafe { CStr::from_ptr(s_ptr) }
            .to_string_lossy()
            .into_owned();
        // SAFETY: s_ptr was allocated by the library and is freed exactly once.
        unsafe { dnv_vista_sdk_string_free(s_ptr) };
        Some(s)
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_channel_id_t {
        self.0.as_ptr()
    }
}

impl Drop for DataChannelId {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `DataChannelId` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_dcl_channel_id_free(self.0.as_ptr()) };
    }
}

/// Borrowed view of a [`DataChannelId`] obtained via [`DataChannelRef::channel_id`].
pub struct DataChannelIdRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_channel_id_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_channel_id_t>,
}

impl<'a> DataChannelIdRef<'a> {
    /// Returns the short identifier if set.
    pub fn short_id(&self) -> Option<String> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { opt_str(ffi::dnv_vista_sdk_dcl_channel_id_short_id(self.ptr)) }
    }

    /// Returns the string representation of the local id if set.
    pub fn local_id_string(&self) -> Option<String> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let local_id_ptr = unsafe { ffi::dnv_vista_sdk_dcl_channel_id_local_id(self.ptr) };
        if local_id_ptr.is_null() {
            return None;
        }
        // SAFETY: local_id_ptr is non-null (checked above) and valid for lifetime `'a`. Returns an owned pointer or NULL.
        let s_ptr = unsafe { local_id_ffi::dnv_vista_sdk_local_id_to_string(local_id_ptr) };
        if s_ptr.is_null() {
            return None;
        }
        // SAFETY: s_ptr is non-null (checked above) and owned until freed below.
        let s = unsafe { std::ffi::CStr::from_ptr(s_ptr) }
            .to_string_lossy()
            .into_owned();
        // SAFETY: s_ptr was allocated by the library and is freed exactly once.
        unsafe { dnv_vista_sdk_string_free(s_ptr) };
        Some(s)
    }

    /// Returns a borrowed view of the name object if set.
    pub fn name_object(&self) -> Option<NameObjectRef<'_>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_channel_id_name_object(self.ptr) };
        if ptr.is_null() {
            None
        } else {
            Some(NameObjectRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }
}

/// ISO 19848 Table 16 - properties describing a data channel's type, format, range, and unit.
pub struct Property(NonNull<ffi::dnv_vista_sdk_dcl_property_t>);

impl Property {
    /// Creates a property with the required data channel type and format.
    pub fn new(data_channel_type: &DataChannelType, format: &Format) -> Self {
        // SAFETY: data_channel_type and format are non-null and valid for the call's duration.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_dcl_property_create(data_channel_type.as_ptr(), format.as_ptr())
        };
        Self(NonNull::new(ptr).expect("property_create returned null"))
    }

    /// Returns a new property with the data channel type set.
    #[must_use]
    pub fn with_data_channel_type(self, data_channel_type: &DataChannelType) -> Self {
        // SAFETY: self.0 and data_channel_type are non-null and valid for the call's duration.
        unsafe {
            ffi::dnv_vista_sdk_dcl_property_set_data_channel_type(
                self.0.as_ptr(),
                data_channel_type.as_ptr(),
            )
        };
        self
    }

    /// Returns a new property with the format set.
    #[must_use]
    pub fn with_format(self, format: &Format) -> Self {
        // SAFETY: self.0 and format are non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_property_set_format(self.0.as_ptr(), format.as_ptr()) };
        self
    }

    /// Returns a new property with the range set.
    #[must_use]
    pub fn with_range(self, range: &Range) -> Self {
        // SAFETY: self.0 and range are non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_property_set_range(self.0.as_ptr(), range.as_ptr()) };
        self
    }

    /// Returns a new instance with the range cleared.
    #[must_use]
    pub fn without_range(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_property_clear_range(self.0.as_ptr()) };
        self
    }

    /// Returns a new property with the unit set.
    #[must_use]
    pub fn with_unit(self, unit: &Unit) -> Self {
        // SAFETY: self.0 and unit are non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_property_set_unit(self.0.as_ptr(), unit.as_ptr()) };
        self
    }

    /// Returns a new instance with the unit cleared.
    #[must_use]
    pub fn without_unit(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_property_clear_unit(self.0.as_ptr()) };
        self
    }

    /// Returns the quality coding string if set.
    pub fn quality_coding(&self) -> Option<String> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe {
            opt_str(ffi::dnv_vista_sdk_dcl_property_quality_coding(
                self.0.as_ptr(),
            ))
        }
    }

    /// Returns a new property with the quality coding string set.
    #[must_use]
    pub fn with_quality_coding(self, quality_coding: &str) -> Self {
        let c = CString::new(quality_coding).expect("quality_coding contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe { ffi::dnv_vista_sdk_dcl_property_set_quality_coding(self.0.as_ptr(), c.as_ptr()) };
        self
    }

    /// Returns a new instance with the quality coding string cleared.
    #[must_use]
    pub fn without_quality_coding(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_property_clear_quality_coding(self.0.as_ptr()) };
        self
    }

    /// Returns the alert priority string if set.
    pub fn alert_priority(&self) -> Option<String> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe {
            opt_str(ffi::dnv_vista_sdk_dcl_property_alert_priority(
                self.0.as_ptr(),
            ))
        }
    }

    /// Returns a new property with the alert priority string set.
    #[must_use]
    pub fn with_alert_priority(self, alert_priority: &str) -> Self {
        let c = CString::new(alert_priority).expect("alert_priority contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe { ffi::dnv_vista_sdk_dcl_property_set_alert_priority(self.0.as_ptr(), c.as_ptr()) };
        self
    }

    /// Returns a new instance with the alert priority string cleared.
    #[must_use]
    pub fn without_alert_priority(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_property_clear_alert_priority(self.0.as_ptr()) };
        self
    }

    /// Returns the name string if set.
    pub fn name(&self) -> Option<String> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { opt_str(ffi::dnv_vista_sdk_dcl_property_name(self.0.as_ptr())) }
    }

    /// Returns a new property with the name string set.
    #[must_use]
    pub fn with_name(self, name: &str) -> Self {
        let c = CString::new(name).expect("name contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe { ffi::dnv_vista_sdk_dcl_property_set_name(self.0.as_ptr(), c.as_ptr()) };
        self
    }

    /// Returns a new instance with the name string cleared.
    #[must_use]
    pub fn without_name(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_property_clear_name(self.0.as_ptr()) };
        self
    }

    /// Returns the remarks string if set.
    pub fn remarks(&self) -> Option<String> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { opt_str(ffi::dnv_vista_sdk_dcl_property_remarks(self.0.as_ptr())) }
    }

    /// Returns a new property with the remarks string set.
    #[must_use]
    pub fn with_remarks(self, remarks: &str) -> Self {
        let c = CString::new(remarks).expect("remarks contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe { ffi::dnv_vista_sdk_dcl_property_set_remarks(self.0.as_ptr(), c.as_ptr()) };
        self
    }

    /// Returns a new instance with the remarks string cleared.
    #[must_use]
    pub fn without_remarks(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_property_clear_remarks(self.0.as_ptr()) };
        self
    }

    /// Returns a new property with the custom properties set, transferring ownership of `doc`.
    #[must_use]
    pub fn with_custom_properties(self, doc: SerializableDocument) -> Self {
        let raw = doc.into_raw();
        // SAFETY: self.0 is non-null, and raw transfers ownership of a non-null pointer.
        unsafe { ffi::dnv_vista_sdk_dcl_property_set_custom_properties(self.0.as_ptr(), raw) };
        self
    }

    /// Returns a new instance with the custom properties cleared.
    #[must_use]
    pub fn without_custom_properties(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_property_clear_custom_properties(self.0.as_ptr()) };
        self
    }

    /// Returns a borrowed view of the data channel type.
    pub fn data_channel_type(&self) -> DataChannelTypeRef<'_> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_property_data_channel_type(self.0.as_ptr()) };
        DataChannelTypeRef {
            ptr,
            _marker: std::marker::PhantomData,
        }
    }

    /// Returns a borrowed view of the format.
    pub fn format(&self) -> FormatRef<'_> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_property_format(self.0.as_ptr()) };
        FormatRef {
            ptr,
            _marker: std::marker::PhantomData,
        }
    }

    /// Returns a borrowed view of the range if set.
    pub fn range(&self) -> Option<RangeRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_property_range(self.0.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(RangeRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Returns a borrowed view of the unit if set.
    pub fn unit(&self) -> Option<UnitRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_property_unit(self.0.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(UnitRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Returns a borrowed view of the custom properties if set.
    pub fn custom_properties(&self) -> Option<SerializableDocumentRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_property_custom_properties(self.0.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(unsafe { SerializableDocumentRef::from_ptr(ptr) })
        }
    }

    /// Returns `true` if this property is consistent and complete.
    pub fn validate(&self) -> bool {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        (unsafe { ffi::dnv_vista_sdk_dcl_property_validate(self.0.as_ptr()) }) != 0
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_property_t {
        self.0.as_ptr()
    }
}

impl Drop for Property {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `Property` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_dcl_property_free(self.0.as_ptr()) };
    }
}

/// Borrowed view of a [`Property`] obtained via [`DataChannelRef::property`].
pub struct PropertyRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_property_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_property_t>,
}

impl<'a> PropertyRef<'a> {
    /// Returns the name string if set.
    pub fn name(&self) -> Option<String> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { opt_str(ffi::dnv_vista_sdk_dcl_property_name(self.ptr)) }
    }

    /// Returns the remarks string if set.
    pub fn remarks(&self) -> Option<String> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { opt_str(ffi::dnv_vista_sdk_dcl_property_remarks(self.ptr)) }
    }

    /// Returns a borrowed view of the data channel type.
    pub fn data_channel_type(&self) -> DataChannelTypeRef<'_> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_property_data_channel_type(self.ptr) };
        DataChannelTypeRef {
            ptr,
            _marker: std::marker::PhantomData,
        }
    }

    /// Returns a borrowed view of the format.
    pub fn format(&self) -> FormatRef<'_> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_property_format(self.ptr) };
        FormatRef {
            ptr,
            _marker: std::marker::PhantomData,
        }
    }

    /// Returns a borrowed view of the range if set.
    pub fn range(&self) -> Option<RangeRef<'_>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_property_range(self.ptr) };
        if ptr.is_null() {
            None
        } else {
            Some(RangeRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Returns a borrowed view of the unit if set.
    pub fn unit(&self) -> Option<UnitRef<'_>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_property_unit(self.ptr) };
        if ptr.is_null() {
            None
        } else {
            Some(UnitRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }
}

/// ISO 19848 Table 17 - data channel type (e.g. `"Inst"`, `"Calc"`, `"ManualInput"`).
pub struct DataChannelType(NonNull<ffi::dnv_vista_sdk_dcl_data_channel_type_t>);

impl DataChannelType {
    /// Creates a data channel type with the given type string.
    pub fn new(type_: &str) -> Self {
        let c = CString::new(type_).expect("type contains NUL byte");
        // SAFETY: c is a valid NUL-terminated C string. Returns an owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_data_channel_type_create(c.as_ptr()) };
        Self(NonNull::new(ptr).expect("data_channel_type_create returned null"))
    }

    /// Returns the type string.
    pub fn type_(&self) -> String {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe {
            req_str(ffi::dnv_vista_sdk_dcl_data_channel_type_type(
                self.0.as_ptr(),
            ))
        }
    }

    /// Returns a new data channel type with the type string updated.
    /// Returns `Err` if the value is empty or invalid.
    pub fn with_type(self, type_: &str) -> Result<Self, VistaError> {
        let c = CString::new(type_).expect("type contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        let ok = unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_type_set_type(self.0.as_ptr(), c.as_ptr())
        } != 0;
        if ok {
            Ok(self)
        } else {
            Err(crate::core::error::last_error())
        }
    }

    /// Returns the update cycle in seconds if set.
    pub fn update_cycle(&self) -> Option<f64> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        if (unsafe { ffi::dnv_vista_sdk_dcl_data_channel_type_has_update_cycle(self.0.as_ptr()) })
            != 0
        {
            // SAFETY: self.0 is non-null and known (checked above) to have this field.
            Some(unsafe { ffi::dnv_vista_sdk_dcl_data_channel_type_update_cycle(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Returns a new data channel type with the update cycle set.
    #[must_use]
    pub fn with_update_cycle(self, value: f64) -> Self {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_type_set_update_cycle(self.0.as_ptr(), value)
        };
        self
    }

    /// Returns a new instance with the update cycle cleared.
    #[must_use]
    pub fn without_update_cycle(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_data_channel_type_clear_update_cycle(self.0.as_ptr()) };
        self
    }

    /// Returns the calculation period in seconds if set.
    pub fn calculation_period(&self) -> Option<f64> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        if (unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_type_has_calculation_period(self.0.as_ptr())
        }) != 0
        {
            Some(unsafe {
                // SAFETY: self.0 is non-null and known (checked above) to have this field.
                ffi::dnv_vista_sdk_dcl_data_channel_type_calculation_period(self.0.as_ptr())
            })
        } else {
            None
        }
    }

    /// Returns a new data channel type with the calculation period set.
    #[must_use]
    pub fn with_calculation_period(self, value: f64) -> Self {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_type_set_calculation_period(self.0.as_ptr(), value)
        };
        self
    }

    /// Returns a new instance with the calculation period cleared.
    #[must_use]
    pub fn without_calculation_period(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_type_clear_calculation_period(self.0.as_ptr())
        };
        self
    }

    /// Returns `true` if this channel type represents an alarm/alert.
    pub fn is_alert(&self) -> bool {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        (unsafe { ffi::dnv_vista_sdk_dcl_data_channel_type_is_alert(self.0.as_ptr()) }) != 0
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_data_channel_type_t {
        self.0.as_ptr()
    }
}

impl Drop for DataChannelType {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `DataChannelType` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_dcl_data_channel_type_free(self.0.as_ptr()) };
    }
}

/// Borrowed view of a [`DataChannelType`] obtained via [`PropertyRef::data_channel_type`].
pub struct DataChannelTypeRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_data_channel_type_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_data_channel_type_t>,
}

impl<'a> DataChannelTypeRef<'a> {
    /// Returns the type string.
    pub fn type_(&self) -> String {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { req_str(ffi::dnv_vista_sdk_dcl_data_channel_type_type(self.ptr)) }
    }

    /// Returns the update cycle in seconds if set.
    pub fn update_cycle(&self) -> Option<f64> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        if (unsafe { ffi::dnv_vista_sdk_dcl_data_channel_type_has_update_cycle(self.ptr) }) != 0 {
            // SAFETY: self.ptr is non-null and known (checked above) to have this field.
            Some(unsafe { ffi::dnv_vista_sdk_dcl_data_channel_type_update_cycle(self.ptr) })
        } else {
            None
        }
    }
}

/// ISO 19848 Table 18 - data type and optional restriction for a data channel.
pub struct Format(NonNull<ffi::dnv_vista_sdk_dcl_format_t>);

impl Format {
    /// Creates a format with the given type string (e.g. `"Numeric"`, `"String"`).
    pub fn new(type_: &str) -> Self {
        let c = CString::new(type_).expect("type contains NUL byte");
        // SAFETY: c is a valid NUL-terminated C string. Returns an owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_format_create(c.as_ptr()) };
        Self(NonNull::new(ptr).expect("format_create returned null"))
    }

    /// Returns the format type string.
    pub fn type_(&self) -> String {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { req_str(ffi::dnv_vista_sdk_dcl_format_type(self.0.as_ptr())) }
    }

    /// Returns a new format with the type string updated.
    /// Returns `Err` if the value is empty or invalid.
    pub fn with_type(self, type_: &str) -> Result<Self, VistaError> {
        let c = CString::new(type_).expect("type contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        let ok =
            unsafe { ffi::dnv_vista_sdk_dcl_format_set_type(self.0.as_ptr(), c.as_ptr()) } != 0;
        if ok {
            Ok(self)
        } else {
            Err(crate::core::error::last_error())
        }
    }

    /// Returns the restriction if one is set.
    pub fn restriction(&self) -> Option<RestrictionRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_format_restriction(self.0.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(RestrictionRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Returns a new format with the restriction set (copied from `restriction`).
    #[must_use]
    pub fn with_restriction(self, restriction: &Restriction) -> Self {
        // SAFETY: self.0 and restriction are non-null and valid for the call's duration.
        unsafe {
            ffi::dnv_vista_sdk_dcl_format_set_restriction(self.0.as_ptr(), restriction.as_ptr())
        };
        self
    }

    /// Returns a new instance with the restriction cleared.
    #[must_use]
    pub fn without_restriction(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_format_clear_restriction(self.0.as_ptr()) };
        self
    }

    /// Returns `true` if `value` is valid according to this format.
    pub fn validate_value(&self, value: &str) -> bool {
        let c = CString::new(value).expect("value contains NUL byte");
        // SAFETY: self.0 is non-null, c is a valid NUL-terminated C string, and passing
        // NULL for the optional out-value pointer is accepted by the C API.
        (unsafe {
            ffi::dnv_vista_sdk_dcl_format_validate_value(
                self.0.as_ptr(),
                c.as_ptr(),
                std::ptr::null_mut(),
            )
        }) != 0
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_format_t {
        self.0.as_ptr()
    }
}

impl Drop for Format {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `Format` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_dcl_format_free(self.0.as_ptr()) };
    }
}

/// Borrowed view of a [`Format`] obtained via [`PropertyRef::format`].
pub struct FormatRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_format_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_format_t>,
}

impl<'a> FormatRef<'a> {
    /// Returns the format type string.
    pub fn type_(&self) -> String {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { req_str(ffi::dnv_vista_sdk_dcl_format_type(self.ptr)) }
    }

    /// Returns a borrowed view of the restriction if one is set.
    pub fn restriction(&self) -> Option<RestrictionRef<'_>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_format_restriction(self.ptr) };
        if ptr.is_null() {
            None
        } else {
            Some(RestrictionRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }
}

/// ISO 19848 Table 19 - value restriction for a data channel format.
pub struct Restriction(NonNull<ffi::dnv_vista_sdk_dcl_restriction_t>);

impl Restriction {
    /// Creates a new restriction with no constraints.
    pub fn new() -> Self {
        // SAFETY: no arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_restriction_create() };
        Self(NonNull::new(ptr).expect("restriction_create returned null"))
    }

    /// Returns the number of enumeration values, or 0 if none are set.
    pub fn enumeration_count(&self) -> usize {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_enumeration_count(self.0.as_ptr()) }
    }

    /// Returns the enumeration value at `index`, or `None` if out of range.
    pub fn enumeration_at(&self, index: usize) -> Option<String> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_dcl_restriction_enumeration_at(self.0.as_ptr(), index) };
        if ptr.is_null() {
            None
        } else {
            // SAFETY: ptr is non-null (checked above) and owned by `self` for its lifetime.
            Some(unsafe { req_str(ptr) })
        }
    }

    /// Returns a new restriction with the allowed enumeration values set.
    #[must_use]
    pub fn with_enumeration(self, values: &[&str]) -> Self {
        let cstrings: Vec<CString> = values
            .iter()
            .map(|s| CString::new(*s).expect("enumeration value contains NUL byte"))
            .collect();
        let ptrs: Vec<*const std::ffi::c_char> = cstrings.iter().map(|s| s.as_ptr()).collect();
        // SAFETY: self.0 is non-null, and ptrs.as_ptr() is valid for ptrs.len() elements.
        unsafe {
            ffi::dnv_vista_sdk_dcl_restriction_set_enumeration(
                self.0.as_ptr(),
                ptrs.as_ptr(),
                ptrs.len(),
            )
        };
        self
    }

    /// Returns a new instance with all enumeration values cleared.
    #[must_use]
    pub fn without_enumeration(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_enumeration(self.0.as_ptr()) };
        self
    }

    /// Returns the fraction-digits constraint if set.
    pub fn fraction_digits(&self) -> Option<u32> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_fraction_digits(self.0.as_ptr()) }) != 0
        {
            // SAFETY: self.0 is non-null and known (checked above) to have this field.
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_fraction_digits(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Returns a new restriction with the fraction-digits constraint set.
    #[must_use]
    pub fn with_fraction_digits(self, value: u32) -> Self {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_fraction_digits(self.0.as_ptr(), value) };
        self
    }

    /// Returns a new instance with the fraction-digits constraint cleared.
    #[must_use]
    pub fn without_fraction_digits(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_fraction_digits(self.0.as_ptr()) };
        self
    }

    /// Returns the length constraint if set.
    pub fn length(&self) -> Option<u32> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_length(self.0.as_ptr()) }) != 0 {
            // SAFETY: self.0 is non-null and known (checked above) to have this field.
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_length(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Returns a new restriction with the length constraint set.
    #[must_use]
    pub fn with_length(self, value: u32) -> Self {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_length(self.0.as_ptr(), value) };
        self
    }

    /// Returns a new instance with the length constraint cleared.
    #[must_use]
    pub fn without_length(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_length(self.0.as_ptr()) };
        self
    }

    /// Returns the max-exclusive constraint if set.
    pub fn max_exclusive(&self) -> Option<f64> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_max_exclusive(self.0.as_ptr()) }) != 0 {
            // SAFETY: self.0 is non-null and known (checked above) to have this field.
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_max_exclusive(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Returns a new restriction with the max-exclusive constraint set.
    #[must_use]
    pub fn with_max_exclusive(self, value: f64) -> Self {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_max_exclusive(self.0.as_ptr(), value) };
        self
    }

    /// Returns a new instance with the max-exclusive constraint cleared.
    #[must_use]
    pub fn without_max_exclusive(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_max_exclusive(self.0.as_ptr()) };
        self
    }

    /// Returns the max-inclusive constraint if set.
    pub fn max_inclusive(&self) -> Option<f64> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_max_inclusive(self.0.as_ptr()) }) != 0 {
            // SAFETY: self.0 is non-null and known (checked above) to have this field.
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_max_inclusive(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Returns a new restriction with the max-inclusive constraint set.
    #[must_use]
    pub fn with_max_inclusive(self, value: f64) -> Self {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_max_inclusive(self.0.as_ptr(), value) };
        self
    }

    /// Returns a new instance with the max-inclusive constraint cleared.
    #[must_use]
    pub fn without_max_inclusive(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_max_inclusive(self.0.as_ptr()) };
        self
    }

    /// Returns the max-length constraint if set.
    pub fn max_length(&self) -> Option<u32> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_max_length(self.0.as_ptr()) }) != 0 {
            // SAFETY: self.0 is non-null and known (checked above) to have this field.
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_max_length(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Returns a new restriction with the max-length constraint set.
    #[must_use]
    pub fn with_max_length(self, value: u32) -> Self {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_max_length(self.0.as_ptr(), value) };
        self
    }

    /// Returns a new instance with the max-length constraint cleared.
    #[must_use]
    pub fn without_max_length(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_max_length(self.0.as_ptr()) };
        self
    }

    /// Returns the min-exclusive constraint if set.
    pub fn min_exclusive(&self) -> Option<f64> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_min_exclusive(self.0.as_ptr()) }) != 0 {
            // SAFETY: self.0 is non-null and known (checked above) to have this field.
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_min_exclusive(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Returns a new restriction with the min-exclusive constraint set.
    #[must_use]
    pub fn with_min_exclusive(self, value: f64) -> Self {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_min_exclusive(self.0.as_ptr(), value) };
        self
    }

    /// Returns a new instance with the min-exclusive constraint cleared.
    #[must_use]
    pub fn without_min_exclusive(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_min_exclusive(self.0.as_ptr()) };
        self
    }

    /// Returns the min-inclusive constraint if set.
    pub fn min_inclusive(&self) -> Option<f64> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_min_inclusive(self.0.as_ptr()) }) != 0 {
            // SAFETY: self.0 is non-null and known (checked above) to have this field.
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_min_inclusive(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Returns a new restriction with the min-inclusive constraint set.
    #[must_use]
    pub fn with_min_inclusive(self, value: f64) -> Self {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_min_inclusive(self.0.as_ptr(), value) };
        self
    }

    /// Returns a new instance with the min-inclusive constraint cleared.
    #[must_use]
    pub fn without_min_inclusive(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_min_inclusive(self.0.as_ptr()) };
        self
    }

    /// Returns the min-length constraint if set.
    pub fn min_length(&self) -> Option<u32> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_min_length(self.0.as_ptr()) }) != 0 {
            // SAFETY: self.0 is non-null and known (checked above) to have this field.
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_min_length(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Returns a new restriction with the min-length constraint set.
    #[must_use]
    pub fn with_min_length(self, value: u32) -> Self {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_min_length(self.0.as_ptr(), value) };
        self
    }

    /// Returns a new instance with the min-length constraint cleared.
    #[must_use]
    pub fn without_min_length(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_min_length(self.0.as_ptr()) };
        self
    }

    /// Returns the pattern constraint if set.
    pub fn pattern(&self) -> Option<String> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_restriction_pattern(self.0.as_ptr()) };
        // SAFETY: ptr is either null or owned by `self` for its lifetime.
        unsafe { opt_str(ptr) }
    }

    /// Returns a new restriction with the pattern constraint set.
    #[must_use]
    pub fn with_pattern(self, value: &str) -> Self {
        let c = CString::new(value).expect("pattern contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_pattern(self.0.as_ptr(), c.as_ptr()) };
        self
    }

    /// Returns a new instance with the pattern constraint cleared.
    #[must_use]
    pub fn without_pattern(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_pattern(self.0.as_ptr()) };
        self
    }

    /// Returns the total-digits constraint if set.
    pub fn total_digits(&self) -> Option<u32> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_total_digits(self.0.as_ptr()) }) != 0 {
            // SAFETY: self.0 is non-null and known (checked above) to have this field.
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_total_digits(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Returns a new restriction with the total-digits constraint set.
    /// Returns `Err` if `value` is zero.
    pub fn with_total_digits(self, value: u32) -> Result<Self, VistaError> {
        use crate::core::error::ErrorKind;
        if value == 0 {
            return Err(VistaError {
                kind: ErrorKind::InvalidArgument,
                message: "total_digits must be greater than zero".to_string(),
            });
        }
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_total_digits(self.0.as_ptr(), value) };
        Ok(self)
    }

    /// Returns a new instance with the total-digits constraint cleared.
    #[must_use]
    pub fn without_total_digits(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_total_digits(self.0.as_ptr()) };
        self
    }

    /// Returns the white-space constraint if set.
    pub fn white_space(&self) -> Option<WhiteSpace> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_white_space(self.0.as_ptr()) }) != 0 {
            Some(white_space_from_ffi(unsafe {
                // SAFETY: self.0 is non-null and known (checked above) to have this field.
                ffi::dnv_vista_sdk_dcl_restriction_white_space(self.0.as_ptr())
            }))
        } else {
            None
        }
    }

    /// Returns a new restriction with the white-space constraint set.
    #[must_use]
    pub fn with_white_space(self, value: WhiteSpace) -> Self {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe {
            ffi::dnv_vista_sdk_dcl_restriction_set_white_space(
                self.0.as_ptr(),
                white_space_to_ffi(value),
            )
        };
        self
    }

    /// Returns a new instance with the white-space constraint cleared.
    #[must_use]
    pub fn without_white_space(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_white_space(self.0.as_ptr()) };
        self
    }

    /// Returns `true` if `value` satisfies all constraints of this restriction.
    pub fn validate_value(&self, value: &str, format: &Format) -> bool {
        let c = CString::new(value).expect("value contains NUL byte");
        // SAFETY: self.0, c, and format.0 are non-null/valid for the call's duration.
        (unsafe {
            ffi::dnv_vista_sdk_dcl_restriction_validate_value(
                self.0.as_ptr(),
                c.as_ptr(),
                format.0.as_ptr(),
            )
        }) != 0
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_restriction_t {
        self.0.as_ptr()
    }
}

impl Default for Restriction {
    fn default() -> Self {
        Self::new()
    }
}

impl Drop for Restriction {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `Restriction` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_free(self.0.as_ptr()) };
    }
}

/// Borrowed view of a [`Restriction`] obtained via [`Format::restriction`].
pub struct RestrictionRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_restriction_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_restriction_t>,
}

impl<'a> RestrictionRef<'a> {
    /// Returns the fraction-digits constraint if set.
    pub fn fraction_digits(&self) -> Option<u32> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_fraction_digits(self.ptr) }) != 0 {
            // SAFETY: self.ptr is non-null and known (checked above) to have this field.
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_fraction_digits(self.ptr) })
        } else {
            None
        }
    }

    /// Returns the white-space constraint if set.
    pub fn white_space(&self) -> Option<WhiteSpace> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_white_space(self.ptr) }) != 0 {
            Some(white_space_from_ffi(unsafe {
                // SAFETY: self.ptr is non-null and known (checked above) to have this field.
                ffi::dnv_vista_sdk_dcl_restriction_white_space(self.ptr)
            }))
        } else {
            None
        }
    }

    /// Returns the pattern constraint if set.
    pub fn pattern(&self) -> Option<String> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_restriction_pattern(self.ptr) };
        // SAFETY: ptr is either null or owned by the referenced restriction for lifetime `'a`.
        unsafe { opt_str(ptr) }
    }

    /// Returns the enumeration count.
    pub fn enumeration_count(&self) -> usize {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_enumeration_count(self.ptr) }
    }

    /// Returns the enumeration value at `index`, or `None` if out of range.
    pub fn enumeration_at(&self, index: usize) -> Option<String> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_restriction_enumeration_at(self.ptr, index) };
        if ptr.is_null() {
            None
        } else {
            // SAFETY: ptr is non-null (checked above) and owned by the referenced restriction for lifetime `'a`.
            Some(unsafe { req_str(ptr) })
        }
    }

    /// Returns the total-digits constraint if set.
    pub fn total_digits(&self) -> Option<u32> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_total_digits(self.ptr) }) != 0 {
            // SAFETY: self.ptr is non-null and known (checked above) to have this field.
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_total_digits(self.ptr) })
        } else {
            None
        }
    }
}

/// ISO 19848 Table 20 - numeric range [low, high] for a data channel.
pub struct Range(NonNull<ffi::dnv_vista_sdk_dcl_range_t>);

impl Range {
    /// Creates a range with the given lower and upper bounds.
    ///
    /// # Panics
    /// Panics if `low >= high`. Use [`Range::try_new`] for fallible construction.
    pub fn new(low: f64, high: f64) -> Self {
        Self::try_new(low, high).expect("range_create returned null: low must be less than high")
    }

    /// Creates a range with the given lower and upper bounds.
    /// Returns `Err` if `low >= high`.
    pub fn try_new(low: f64, high: f64) -> Result<Self, VistaError> {
        // SAFETY: no pointer arguments. Returns an owned pointer or NULL.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_range_create(low, high) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(Self(
                NonNull::new(ptr).expect("range_create returned non-null"),
            ))
        }
    }

    /// Returns the lower bound.
    pub fn low(&self) -> f64 {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_dcl_range_low(self.0.as_ptr()) }
    }

    /// Returns the upper bound.
    pub fn high(&self) -> f64 {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_dcl_range_high(self.0.as_ptr()) }
    }

    /// Returns a new range with the lower bound updated.
    /// Returns `Err` if the new `low > self.high()`.
    pub fn with_low(self, low: f64) -> Result<Self, VistaError> {
        use crate::core::error::ErrorKind;
        if low > self.high() {
            return Err(VistaError {
                kind: ErrorKind::InvalidArgument,
                message: "low must be less than high".to_string(),
            });
        }
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_dcl_range_set_low(self.0.as_ptr(), low) };
        Ok(self)
    }

    /// Returns a new range with the upper bound updated.
    /// Returns `Err` if the new `high < self.low()`.
    pub fn with_high(self, high: f64) -> Result<Self, VistaError> {
        use crate::core::error::ErrorKind;
        if high < self.low() {
            return Err(VistaError {
                kind: ErrorKind::InvalidArgument,
                message: "high must be greater than low".to_string(),
            });
        }
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_dcl_range_set_high(self.0.as_ptr(), high) };
        Ok(self)
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_range_t {
        self.0.as_ptr()
    }
}

impl Drop for Range {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `Range` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_dcl_range_free(self.0.as_ptr()) };
    }
}

/// Borrowed view of a [`Range`] obtained via [`PropertyRef::range`].
pub struct RangeRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_range_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_range_t>,
}

impl<'a> RangeRef<'a> {
    /// Returns the lower bound.
    pub fn low(&self) -> f64 {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { ffi::dnv_vista_sdk_dcl_range_low(self.ptr) }
    }

    /// Returns the upper bound.
    pub fn high(&self) -> f64 {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { ffi::dnv_vista_sdk_dcl_range_high(self.ptr) }
    }
}

/// ISO 19848 Table 21 - unit of measurement for a data channel.
pub struct Unit(NonNull<ffi::dnv_vista_sdk_dcl_unit_t>);

impl Unit {
    /// Creates a unit with the given symbol (e.g. `"°C"`, `"kW"`).
    pub fn new(unit_symbol: &str) -> Self {
        let c = CString::new(unit_symbol).expect("unit_symbol contains NUL byte");
        // SAFETY: c is a valid NUL-terminated C string. Returns an owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_unit_create(c.as_ptr()) };
        Self(NonNull::new(ptr).expect("unit_create returned null"))
    }

    /// Returns the unit symbol.
    pub fn unit_symbol(&self) -> String {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { req_str(ffi::dnv_vista_sdk_dcl_unit_unit_symbol(self.0.as_ptr())) }
    }

    /// Returns a new unit with the unit symbol set.
    #[must_use]
    pub fn with_unit_symbol(self, unit_symbol: &str) -> Self {
        let c = CString::new(unit_symbol).expect("unit_symbol contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe { ffi::dnv_vista_sdk_dcl_unit_set_unit_symbol(self.0.as_ptr(), c.as_ptr()) };
        self
    }

    /// Returns the quantity name if set (e.g. `"Temperature"`).
    pub fn quantity_name(&self) -> Option<String> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { opt_str(ffi::dnv_vista_sdk_dcl_unit_quantity_name(self.0.as_ptr())) }
    }

    /// Returns a new unit with the quantity name set.
    #[must_use]
    pub fn with_quantity_name(self, quantity_name: &str) -> Self {
        let c = CString::new(quantity_name).expect("quantity_name contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe { ffi::dnv_vista_sdk_dcl_unit_set_quantity_name(self.0.as_ptr(), c.as_ptr()) };
        self
    }

    /// Returns a new instance with the quantity name cleared.
    #[must_use]
    pub fn without_quantity_name(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_unit_clear_quantity_name(self.0.as_ptr()) };
        self
    }

    /// Returns a new unit with the custom elements set, transferring ownership of `doc`.
    #[must_use]
    pub fn with_custom_elements(self, doc: SerializableDocument) -> Self {
        let raw = doc.into_raw();
        // SAFETY: self.0 is non-null, and raw transfers ownership of a non-null pointer.
        unsafe { ffi::dnv_vista_sdk_dcl_unit_set_custom_elements(self.0.as_ptr(), raw) };
        self
    }

    /// Returns a new instance with the custom elements cleared.
    #[must_use]
    pub fn without_custom_elements(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_unit_clear_custom_elements(self.0.as_ptr()) };
        self
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_unit_t {
        self.0.as_ptr()
    }
}

impl Drop for Unit {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `Unit` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_dcl_unit_free(self.0.as_ptr()) };
    }
}

/// Borrowed view of a [`Unit`] obtained via [`PropertyRef::unit`].
pub struct UnitRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_unit_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_unit_t>,
}

impl<'a> UnitRef<'a> {
    /// Returns the unit symbol.
    pub fn unit_symbol(&self) -> String {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { req_str(ffi::dnv_vista_sdk_dcl_unit_unit_symbol(self.ptr)) }
    }

    /// Returns the quantity name if set.
    pub fn quantity_name(&self) -> Option<String> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { opt_str(ffi::dnv_vista_sdk_dcl_unit_quantity_name(self.ptr)) }
    }
}

/// ISO 19848 Table 22 - naming rule and optional custom name objects.
pub struct NameObject(NonNull<ffi::dnv_vista_sdk_dcl_name_object_t>);

impl NameObject {
    /// Creates a name object with no naming rule.
    pub fn new() -> Self {
        // SAFETY: no arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_name_object_create_default() };
        Self(NonNull::new(ptr).expect("name_object_create_default returned null"))
    }

    /// Creates a name object with the given naming rule.
    pub fn new_with_naming_rule(naming_rule: &str) -> Self {
        let c = CString::new(naming_rule).expect("naming_rule contains NUL byte");
        // SAFETY: c is a valid NUL-terminated C string. Returns an owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_name_object_create(c.as_ptr()) };
        Self(NonNull::new(ptr).expect("name_object_create returned null"))
    }

    /// Returns the naming rule string.
    pub fn naming_rule(&self) -> String {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe {
            req_str(ffi::dnv_vista_sdk_dcl_name_object_naming_rule(
                self.0.as_ptr(),
            ))
        }
    }

    /// Returns a new name object with the naming rule string set.
    #[must_use]
    pub fn with_naming_rule(self, naming_rule: &str) -> Self {
        let c = CString::new(naming_rule).expect("naming_rule contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe { ffi::dnv_vista_sdk_dcl_name_object_set_naming_rule(self.0.as_ptr(), c.as_ptr()) };
        self
    }

    /// Returns a new name object with the custom name objects set, transferring ownership of `doc`.
    #[must_use]
    pub fn with_custom_name_objects(self, doc: SerializableDocument) -> Self {
        let raw = doc.into_raw();
        // SAFETY: self.0 is non-null, and raw transfers ownership of a non-null pointer.
        unsafe { ffi::dnv_vista_sdk_dcl_name_object_set_custom_name_objects(self.0.as_ptr(), raw) };
        self
    }

    /// Returns a new instance with the custom name objects cleared.
    #[must_use]
    pub fn without_custom_name_objects(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_dcl_name_object_clear_custom_name_objects(self.0.as_ptr()) };
        self
    }

    /// Returns a borrowed view of the custom name objects if set.
    pub fn custom_name_objects(&self) -> Option<SerializableDocumentRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_dcl_name_object_custom_name_objects(self.0.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            // SAFETY: ptr is non-null (checked above) and valid for `self`'s lifetime.
            Some(unsafe { SerializableDocumentRef::from_ptr(ptr) })
        }
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_name_object_t {
        self.0.as_ptr()
    }
}

impl Default for NameObject {
    fn default() -> Self {
        Self::new()
    }
}

impl Drop for NameObject {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `NameObject` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_dcl_name_object_free(self.0.as_ptr()) };
    }
}

/// Borrowed view of a [`NameObject`] obtained via [`DataChannelIdRef::name_object`].
pub struct NameObjectRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_name_object_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_name_object_t>,
}

impl<'a> NameObjectRef<'a> {
    /// Returns the naming rule string.
    pub fn naming_rule(&self) -> String {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { req_str(ffi::dnv_vista_sdk_dcl_name_object_naming_rule(self.ptr)) }
    }
}

// SAFETY: all owned types below wrap NonNull pointers to heap-allocated C++ objects they own
// exclusively - no interior mutability or shared state, so both Send and Sync are safe.
unsafe impl Send for Restriction {}
unsafe impl Sync for Restriction {}
unsafe impl Send for Range {}
unsafe impl Sync for Range {}
unsafe impl Send for Format {}
unsafe impl Sync for Format {}
unsafe impl Send for DataChannelType {}
unsafe impl Sync for DataChannelType {}
unsafe impl Send for NameObject {}
unsafe impl Sync for NameObject {}
unsafe impl Send for Unit {}
unsafe impl Sync for Unit {}
unsafe impl Send for Property {}
unsafe impl Sync for Property {}
unsafe impl Send for ConfigurationReference {}
unsafe impl Sync for ConfigurationReference {}
unsafe impl Send for VersionInformation {}
unsafe impl Sync for VersionInformation {}
unsafe impl Send for DataChannelId {}
unsafe impl Sync for DataChannelId {}
unsafe impl Send for Header {}
unsafe impl Sync for Header {}
unsafe impl Send for DataChannel {}
unsafe impl Sync for DataChannel {}
unsafe impl Send for DataChannelList {}
unsafe impl Sync for DataChannelList {}
unsafe impl Send for Package {}
unsafe impl Sync for Package {}
unsafe impl Send for DataChannelListPackage {}
unsafe impl Sync for DataChannelListPackage {}

#[cfg(test)]
mod discriminant_sync {
    use super::*;

    #[test]
    fn white_space_discriminants_match_ffi() {
        assert_eq!(
            WhiteSpace::Preserve as u32,
            ffi::dnv_vista_sdk_dcl_white_space_t::Preserve as u32
        );
        assert_eq!(
            WhiteSpace::Replace as u32,
            ffi::dnv_vista_sdk_dcl_white_space_t::Replace as u32
        );
        assert_eq!(
            WhiteSpace::Collapse as u32,
            ffi::dnv_vista_sdk_dcl_white_space_t::Collapse as u32
        );
    }
}
