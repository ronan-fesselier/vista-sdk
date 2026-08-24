use std::ffi::{CStr, CString};
use std::ptr::NonNull;

use crate::core::error::{last_error, VistaError};
use crate::ffi::core::common::dnv_vista_sdk_string_free;
use crate::ffi::transport::datachannel::data_channel_dto as ffi;
use crate::transport::datachannel::data_channel::DataChannelListPackage;
use crate::transport::serializable_document::SerializableDocumentRefMut;

unsafe fn cstr(ptr: *const std::ffi::c_char) -> String {
    CStr::from_ptr(ptr)
        .to_str()
        .expect("invalid UTF-8")
        .to_string()
}

unsafe fn opt_cstr(ptr: *const std::ffi::c_char) -> Option<String> {
    if ptr.is_null() {
        None
    } else {
        Some(cstr(ptr))
    }
}

// ============================================================
// DtoPackage
// ============================================================

/// Owned handle to a `DataChannelListPackageDto`. Free on drop.
pub struct DtoPackage {
    ptr: NonNull<ffi::dnv_vista_sdk_dcl_dto_package_t>,
}

impl Drop for DtoPackage {
    fn drop(&mut self) {
        // SAFETY: ptr is owned and valid. Freed exactly once.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_package_free(self.ptr.as_ptr()) };
    }
}

impl DtoPackage {
    pub(crate) unsafe fn from_raw(ptr: *mut ffi::dnv_vista_sdk_dcl_dto_package_t) -> Self {
        Self {
            ptr: NonNull::new(ptr).expect("DtoPackage::from_raw called with null"),
        }
    }

    pub(crate) fn as_ptr(&self) -> *mut ffi::dnv_vista_sdk_dcl_dto_package_t {
        self.ptr.as_ptr()
    }

    /// Non-owning view of the embedded `PackageDto`.
    pub fn pkg(&mut self) -> DtoPkgRef<'_> {
        // SAFETY: ptr is valid; result is a non-owning borrow.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_package_get_pkg(self.ptr.as_ptr()) };
        DtoPkgRef {
            ptr: NonNull::new(p).expect("get_pkg returned null"),
            _phantom: std::marker::PhantomData,
        }
    }
}

// ============================================================
// DtoPkgRef
// ============================================================

/// Non-owning borrow of a `PackageDto`.
pub struct DtoPkgRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_dcl_dto_pkg_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_dcl_dto_pkg_t>,
}

impl<'a> DtoPkgRef<'a> {
    /// Non-owning view of the embedded `HeaderDto`.
    pub fn header(&mut self) -> DtoHeaderRef<'_> {
        // SAFETY: ptr is valid; result is a non-owning borrow.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_pkg_get_header(self.ptr.as_ptr()) };
        DtoHeaderRef {
            ptr: NonNull::new(p).expect("get_header returned null"),
            _phantom: std::marker::PhantomData,
        }
    }

    /// Non-owning view of the embedded `DataChannelListDto`.
    pub fn channel_list(&mut self) -> DtoChListRef<'_> {
        // SAFETY: ptr is valid; result is a non-owning borrow.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_pkg_get_channel_list(self.ptr.as_ptr()) };
        DtoChListRef {
            ptr: NonNull::new(p).expect("get_channel_list returned null"),
            _phantom: std::marker::PhantomData,
        }
    }
}

// ============================================================
// DtoHeaderRef
// ============================================================

/// Non-owning borrow of a `HeaderDto`.
pub struct DtoHeaderRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_dcl_dto_header_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_dcl_dto_header_t>,
}

impl<'a> DtoHeaderRef<'a> {
    /// Ship identifier (IMO number or alternative).
    pub fn ship_id(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            cstr(ffi::dnv_vista_sdk_dcl_dto_header_get_ship_id(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the ship identifier.
    pub fn set_ship_id(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in ship_id");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_header_set_ship_id(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Mutable view of the embedded configuration reference.
    pub fn cfg_ref(&mut self) -> DtoCfgRefRef<'_> {
        // SAFETY: ptr is valid; result is a non-owning borrow.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_header_get_cfg_ref(self.ptr.as_ptr()) };
        DtoCfgRefRef {
            ptr: NonNull::new(p).expect("get_cfg_ref returned null"),
            _phantom: std::marker::PhantomData,
        }
    }

    /// Whether version information is set.
    pub fn has_ver_info(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_header_has_ver_info(self.ptr.as_ptr()) }) != 0
    }

    /// Mutable view of the version information, or `None` if absent.
    pub fn ver_info(&mut self) -> Option<DtoVerInfoRef<'_>> {
        if !self.has_ver_info() {
            return None;
        }
        // SAFETY: ptr is valid and has_ver_info is true.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_header_get_ver_info(self.ptr.as_ptr()) };
        Some(DtoVerInfoRef {
            ptr: NonNull::new(p).expect("get_ver_info returned null"),
            _phantom: std::marker::PhantomData,
        })
    }

    /// Ensures version information exists (creating a default one if absent).
    pub fn ensure_ver_info(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_header_ensure_ver_info(self.ptr.as_ptr()) };
    }

    /// Clears the version information.
    pub fn clear_ver_info(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_header_clear_ver_info(self.ptr.as_ptr()) };
    }

    /// Whether the header carries an author.
    pub fn has_author(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_header_has_author(self.ptr.as_ptr()) }) != 0
    }

    /// Author of the Data Channel List, if set.
    pub fn author(&self) -> Option<String> {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_dcl_dto_header_get_author(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the author.
    pub fn set_author(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in author");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_header_set_author(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Clears the author.
    pub fn clear_author(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_header_clear_author(self.ptr.as_ptr()) };
    }

    /// Whether the header carries a creation date.
    pub fn has_date_created(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_header_has_date_created(self.ptr.as_ptr()) }) != 0
    }

    /// Creation date of the package, if set.
    pub fn date_created(&self) -> Option<String> {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_dcl_dto_header_get_date_created(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the creation date.
    pub fn set_date_created(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in date_created");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe {
            ffi::dnv_vista_sdk_dcl_dto_header_set_date_created(self.ptr.as_ptr(), s.as_ptr())
        };
    }

    /// Clears the creation date.
    pub fn clear_date_created(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_header_clear_date_created(self.ptr.as_ptr()) };
    }

    /// Whether the header carries custom headers.
    pub fn has_custom_headers(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_header_has_custom_headers(self.ptr.as_ptr()) }) != 0
    }

    /// Mutable view of the custom headers, or `None` if absent. Do NOT free.
    pub fn custom_headers(&mut self) -> Option<SerializableDocumentRefMut<'_>> {
        if !self.has_custom_headers() {
            return None;
        }
        // SAFETY: ptr is valid and has_custom_headers is true. The returned pointer is a
        // non-owning view into the header, valid for the lifetime of this borrow.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_header_get_custom_headers(self.ptr.as_ptr()) };
        Some(unsafe { SerializableDocumentRefMut::from_ptr_mut(p) })
    }

    /// Ensures custom headers exist (creating an empty object if absent) and returns a mutable view.
    pub fn ensure_custom_headers(&mut self) -> SerializableDocumentRefMut<'_> {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_header_ensure_custom_headers(self.ptr.as_ptr()) };
        // SAFETY: after ensure, get returned a non-null non-owning view.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_header_get_custom_headers(self.ptr.as_ptr()) };
        unsafe { SerializableDocumentRefMut::from_ptr_mut(p) }
    }

    /// Clears the custom headers.
    pub fn clear_custom_headers(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_header_clear_custom_headers(self.ptr.as_ptr()) };
    }
}

// ============================================================
// DtoCfgRefRef
// ============================================================

/// Non-owning borrow of a `ConfigurationReferenceDto`.
pub struct DtoCfgRefRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_dcl_dto_cfg_ref_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_dcl_dto_cfg_ref_t>,
}

impl<'a> DtoCfgRefRef<'a> {
    /// Identifier of the referenced configuration.
    pub fn id(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe { cstr(ffi::dnv_vista_sdk_dcl_dto_cfg_ref_get_id(self.ptr.as_ptr())) }
    }

    /// Sets the identifier of the referenced configuration.
    pub fn set_id(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in id");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_cfg_ref_set_id(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Timestamp of the referenced configuration.
    pub fn timestamp(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            cstr(ffi::dnv_vista_sdk_dcl_dto_cfg_ref_get_timestamp(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the timestamp of the referenced configuration.
    pub fn set_timestamp(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in timestamp");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_cfg_ref_set_timestamp(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Whether a version is set.
    pub fn has_version(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_cfg_ref_has_version(self.ptr.as_ptr()) }) != 0
    }

    /// Version of the referenced configuration, if set.
    pub fn version(&self) -> Option<String> {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_dcl_dto_cfg_ref_get_version(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the version of the referenced configuration.
    pub fn set_version(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in version");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_cfg_ref_set_version(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Clears the version of the referenced configuration.
    pub fn clear_version(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_cfg_ref_clear_version(self.ptr.as_ptr()) };
    }
}

// ============================================================
// DtoVerInfoRef
// ============================================================

/// Non-owning borrow of a `VersionInformationDto`.
pub struct DtoVerInfoRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_dcl_dto_ver_info_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_dcl_dto_ver_info_t>,
}

impl<'a> DtoVerInfoRef<'a> {
    /// Name of the naming scheme (e.g. `"dnv-v2"`).
    pub fn naming_rule(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            cstr(ffi::dnv_vista_sdk_dcl_dto_ver_info_get_naming_rule(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the name of the naming scheme.
    pub fn set_naming_rule(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in naming_rule");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe {
            ffi::dnv_vista_sdk_dcl_dto_ver_info_set_naming_rule(self.ptr.as_ptr(), s.as_ptr())
        };
    }

    /// Version of the naming scheme.
    pub fn naming_scheme_version(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            cstr(ffi::dnv_vista_sdk_dcl_dto_ver_info_get_naming_scheme_version(self.ptr.as_ptr()))
        }
    }

    /// Sets the version of the naming scheme.
    pub fn set_naming_scheme_version(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in naming_scheme_version");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe {
            ffi::dnv_vista_sdk_dcl_dto_ver_info_set_naming_scheme_version(
                self.ptr.as_ptr(),
                s.as_ptr(),
            )
        };
    }

    /// Whether a reference URL is set.
    pub fn has_reference_url(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_ver_info_has_reference_url(self.ptr.as_ptr()) }) != 0
    }

    /// URL documenting the naming scheme, if set.
    pub fn reference_url(&self) -> Option<String> {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_dcl_dto_ver_info_get_reference_url(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the URL documenting the naming scheme.
    pub fn set_reference_url(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in reference_url");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe {
            ffi::dnv_vista_sdk_dcl_dto_ver_info_set_reference_url(self.ptr.as_ptr(), s.as_ptr())
        };
    }

    /// Clears the reference URL.
    pub fn clear_reference_url(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_ver_info_clear_reference_url(self.ptr.as_ptr()) };
    }
}

// ============================================================
// DtoChListRef
// ============================================================

/// Non-owning borrow of a `DataChannelListDto`.
pub struct DtoChListRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_dcl_dto_ch_list_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_dcl_dto_ch_list_t>,
}

impl<'a> DtoChListRef<'a> {
    /// Number of channels in the list.
    pub fn count(&self) -> usize {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_list_count(self.ptr.as_ptr()) }
    }

    /// Mutable view of the channel at `index`, or `None` if out of range.
    pub fn at(&mut self, index: usize) -> Option<DtoChannelRef<'_>> {
        // SAFETY: ptr is valid; index bounds checked by C function (returns null).
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_list_at(self.ptr.as_ptr(), index) };
        if p.is_null() {
            return None;
        }
        Some(DtoChannelRef {
            ptr: NonNull::new(p).unwrap(),
            _phantom: std::marker::PhantomData,
        })
    }

    /// Appends a default-constructed channel and returns a mutable view.
    pub fn push(&mut self) -> DtoChannelRef<'_> {
        // SAFETY: ptr is valid; result is a non-owning borrow.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_list_push(self.ptr.as_ptr()) };
        DtoChannelRef {
            ptr: NonNull::new(p).expect("push returned null"),
            _phantom: std::marker::PhantomData,
        }
    }

    /// Removes the channel at `index`, shifting subsequent elements.
    pub fn remove(&mut self, index: usize) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_list_remove(self.ptr.as_ptr(), index) };
    }
}

// ============================================================
// DtoChannelRef
// ============================================================

/// Non-owning borrow of a `DataChannelDto`.
pub struct DtoChannelRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_dcl_dto_channel_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_dcl_dto_channel_t>,
}

impl<'a> DtoChannelRef<'a> {
    /// Mutable view of the embedded channel identifier.
    pub fn channel_id(&mut self) -> DtoChIdRef<'_> {
        // SAFETY: ptr is valid; result is a non-owning borrow.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_channel_get_id(self.ptr.as_ptr()) };
        DtoChIdRef {
            ptr: NonNull::new(p).expect("get_id returned null"),
            _phantom: std::marker::PhantomData,
        }
    }

    /// Mutable view of the embedded property.
    pub fn property(&mut self) -> DtoPropertyRef<'_> {
        // SAFETY: ptr is valid; result is a non-owning borrow.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_channel_get_property(self.ptr.as_ptr()) };
        DtoPropertyRef {
            ptr: NonNull::new(p).expect("get_property returned null"),
            _phantom: std::marker::PhantomData,
        }
    }
}

// ============================================================
// DtoChIdRef
// ============================================================

/// Non-owning borrow of a `DataChannelIdDto`.
pub struct DtoChIdRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_dcl_dto_ch_id_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_dcl_dto_ch_id_t>,
}

impl<'a> DtoChIdRef<'a> {
    /// Local ID string of the channel.
    pub fn local_id(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            cstr(ffi::dnv_vista_sdk_dcl_dto_ch_id_get_local_id(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the local ID string of the channel.
    pub fn set_local_id(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in local_id");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_id_set_local_id(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Whether a short ID is set.
    pub fn has_short_id(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_id_has_short_id(self.ptr.as_ptr()) }) != 0
    }

    /// Short ID of the channel, if set.
    pub fn short_id(&self) -> Option<String> {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_dcl_dto_ch_id_get_short_id(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the short ID of the channel.
    pub fn set_short_id(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in short_id");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_id_set_short_id(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Clears the short ID.
    pub fn clear_short_id(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_id_clear_short_id(self.ptr.as_ptr()) };
    }

    /// Mutable view of the name object, or `None` if absent.
    pub fn name_object(&mut self) -> Option<DtoNameObjRef<'_>> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_id_has_name_object(self.ptr.as_ptr()) }) == 0 {
            return None;
        }
        // SAFETY: ptr is valid and has_name_object is true.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_id_get_name_object(self.ptr.as_ptr()) };
        Some(DtoNameObjRef {
            ptr: NonNull::new(p).expect("get_name_object returned null"),
            _phantom: std::marker::PhantomData,
        })
    }

    /// Ensures a name object exists (creating a default one if absent).
    pub fn ensure_name_object(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_id_ensure_name_object(self.ptr.as_ptr()) };
    }

    /// Clears the name object.
    pub fn clear_name_object(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_id_clear_name_object(self.ptr.as_ptr()) };
    }
}

// ============================================================
// DtoNameObjRef
// ============================================================

/// Non-owning borrow of a `NameObjectDto`.
pub struct DtoNameObjRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_dcl_dto_name_obj_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_dcl_dto_name_obj_t>,
}

impl<'a> DtoNameObjRef<'a> {
    /// Name of the naming scheme (e.g. `"dnv-v2"`).
    pub fn naming_rule(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            cstr(ffi::dnv_vista_sdk_dcl_dto_name_obj_get_naming_rule(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the name of the naming scheme.
    pub fn set_naming_rule(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in naming_rule");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe {
            ffi::dnv_vista_sdk_dcl_dto_name_obj_set_naming_rule(self.ptr.as_ptr(), s.as_ptr())
        };
    }

    /// Whether custom name objects are set.
    pub fn has_custom(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_name_obj_has_custom(self.ptr.as_ptr()) }) != 0
    }

    /// Mutable view of the custom name objects, or `None` if absent.
    pub fn custom(&mut self) -> Option<SerializableDocumentRefMut<'_>> {
        if !self.has_custom() {
            return None;
        }
        // SAFETY: ptr is valid and has_custom is true. The returned pointer is a non-owning
        // view into the name object, valid for the lifetime of this borrow.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_name_obj_get_custom(self.ptr.as_ptr()) };
        Some(unsafe { SerializableDocumentRefMut::from_ptr_mut(p) })
    }

    /// Ensures custom name objects exist (creating an empty object if absent).
    pub fn ensure_custom(&mut self) -> SerializableDocumentRefMut<'_> {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_name_obj_ensure_custom(self.ptr.as_ptr()) };
        // SAFETY: after ensure, get returned a non-null non-owning view.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_name_obj_get_custom(self.ptr.as_ptr()) };
        unsafe { SerializableDocumentRefMut::from_ptr_mut(p) }
    }

    /// Clears the custom name objects.
    pub fn clear_custom(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_name_obj_clear_custom(self.ptr.as_ptr()) };
    }
}

// ============================================================
// DtoPropertyRef
// ============================================================

/// Non-owning borrow of a `PropertyDto`.
pub struct DtoPropertyRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_dcl_dto_property_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_dcl_dto_property_t>,
}

impl<'a> DtoPropertyRef<'a> {
    /// Mutable view of the embedded data channel type.
    pub fn ch_type(&mut self) -> DtoChTypeRef<'_> {
        // SAFETY: ptr is valid; result is a non-owning borrow.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_property_get_ch_type(self.ptr.as_ptr()) };
        DtoChTypeRef {
            ptr: NonNull::new(p).expect("get_ch_type returned null"),
            _phantom: std::marker::PhantomData,
        }
    }

    /// Mutable view of the embedded format.
    pub fn format(&mut self) -> DtoFormatRef<'_> {
        // SAFETY: ptr is valid; result is a non-owning borrow.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_property_get_format(self.ptr.as_ptr()) };
        DtoFormatRef {
            ptr: NonNull::new(p).expect("get_format returned null"),
            _phantom: std::marker::PhantomData,
        }
    }

    /// Mutable view of the range, or `None` if absent.
    pub fn range(&mut self) -> Option<DtoRangeRef<'_>> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_dto_property_has_range(self.ptr.as_ptr()) }) == 0 {
            return None;
        }
        // SAFETY: ptr is valid and has_range is true.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_property_get_range(self.ptr.as_ptr()) };
        Some(DtoRangeRef {
            ptr: NonNull::new(p).expect("get_range returned null"),
            _phantom: std::marker::PhantomData,
        })
    }

    /// Mutable view of the unit, or `None` if absent.
    pub fn unit(&mut self) -> Option<DtoUnitRef<'_>> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_dto_property_has_unit(self.ptr.as_ptr()) }) == 0 {
            return None;
        }
        // SAFETY: ptr is valid and has_unit is true.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_property_get_unit(self.ptr.as_ptr()) };
        Some(DtoUnitRef {
            ptr: NonNull::new(p).expect("get_unit returned null"),
            _phantom: std::marker::PhantomData,
        })
    }

    /// Ensures a range exists (creating a default one if absent) and returns a mutable view.
    pub fn ensure_range(&mut self) -> DtoRangeRef<'_> {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_property_ensure_range(self.ptr.as_ptr()) };
        // SAFETY: after ensure, get returned a non-null non-owning view.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_property_get_range(self.ptr.as_ptr()) };
        DtoRangeRef {
            ptr: NonNull::new(p).expect("get_range returned null"),
            _phantom: std::marker::PhantomData,
        }
    }

    /// Clears the range.
    pub fn clear_range(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_property_clear_range(self.ptr.as_ptr()) };
    }

    /// Ensures a unit exists (creating a default one if absent) and returns a mutable view.
    pub fn ensure_unit(&mut self) -> DtoUnitRef<'_> {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_property_ensure_unit(self.ptr.as_ptr()) };
        // SAFETY: after ensure, get returned a non-null non-owning view.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_property_get_unit(self.ptr.as_ptr()) };
        DtoUnitRef {
            ptr: NonNull::new(p).expect("get_unit returned null"),
            _phantom: std::marker::PhantomData,
        }
    }

    /// Clears the unit.
    pub fn clear_unit(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_property_clear_unit(self.ptr.as_ptr()) };
    }

    /// Whether a quality coding is set.
    pub fn has_quality_coding(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_property_has_quality_coding(self.ptr.as_ptr()) }) != 0
    }

    /// Quality coding of the data channel, if set.
    pub fn quality_coding(&self) -> Option<String> {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_dcl_dto_property_get_quality_coding(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the quality coding.
    pub fn set_quality_coding(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in quality_coding");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe {
            ffi::dnv_vista_sdk_dcl_dto_property_set_quality_coding(self.ptr.as_ptr(), s.as_ptr())
        };
    }

    /// Clears the quality coding.
    pub fn clear_quality_coding(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_property_clear_quality_coding(self.ptr.as_ptr()) };
    }

    /// Whether an alert priority is set.
    pub fn has_alert_priority(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_property_has_alert_priority(self.ptr.as_ptr()) }) != 0
    }

    /// Alert priority of the data channel, if set.
    pub fn alert_priority(&self) -> Option<String> {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_dcl_dto_property_get_alert_priority(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the alert priority.
    pub fn set_alert_priority(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in alert_priority");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe {
            ffi::dnv_vista_sdk_dcl_dto_property_set_alert_priority(self.ptr.as_ptr(), s.as_ptr())
        };
    }

    /// Clears the alert priority.
    pub fn clear_alert_priority(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_property_clear_alert_priority(self.ptr.as_ptr()) };
    }

    /// Whether a name is set.
    pub fn has_name(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_property_has_name(self.ptr.as_ptr()) }) != 0
    }

    /// Human-readable name of the data channel, if set.
    pub fn name(&self) -> Option<String> {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_dcl_dto_property_get_name(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the name.
    pub fn set_name(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in name");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_property_set_name(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Clears the name.
    pub fn clear_name(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_property_clear_name(self.ptr.as_ptr()) };
    }

    /// Whether remarks are set.
    pub fn has_remarks(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_property_has_remarks(self.ptr.as_ptr()) }) != 0
    }

    /// Remarks on the data channel, if set.
    pub fn remarks(&self) -> Option<String> {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_dcl_dto_property_get_remarks(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the remarks.
    pub fn set_remarks(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in remarks");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_property_set_remarks(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Clears the remarks.
    pub fn clear_remarks(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_property_clear_remarks(self.ptr.as_ptr()) };
    }

    /// Whether custom properties are set.
    pub fn has_custom_properties(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_property_has_custom_properties(self.ptr.as_ptr()) })
            != 0
    }

    /// Mutable view of the custom properties, or `None` if absent.
    pub fn custom_properties(&mut self) -> Option<SerializableDocumentRefMut<'_>> {
        if !self.has_custom_properties() {
            return None;
        }
        // SAFETY: ptr is valid and has_custom_properties is true. The returned pointer is a
        // non-owning view into the property, valid for the lifetime of this borrow.
        let p =
            unsafe { ffi::dnv_vista_sdk_dcl_dto_property_get_custom_properties(self.ptr.as_ptr()) };
        Some(unsafe { SerializableDocumentRefMut::from_ptr_mut(p) })
    }

    /// Ensures custom properties exist (creating an empty object if absent).
    pub fn ensure_custom_properties(&mut self) -> SerializableDocumentRefMut<'_> {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_property_ensure_custom_properties(self.ptr.as_ptr()) };
        // SAFETY: after ensure, get returned a non-null non-owning view.
        let p =
            unsafe { ffi::dnv_vista_sdk_dcl_dto_property_get_custom_properties(self.ptr.as_ptr()) };
        unsafe { SerializableDocumentRefMut::from_ptr_mut(p) }
    }

    /// Clears the custom properties.
    pub fn clear_custom_properties(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_property_clear_custom_properties(self.ptr.as_ptr()) };
    }
}

// ============================================================
// DtoChTypeRef
// ============================================================

/// Non-owning borrow of a `DataChannelTypeDto`.
pub struct DtoChTypeRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_dcl_dto_ch_type_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_dcl_dto_ch_type_t>,
}

impl<'a> DtoChTypeRef<'a> {
    /// Data channel type (e.g. `"Inst"`, `"Average"`).
    pub fn type_str(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            cstr(ffi::dnv_vista_sdk_dcl_dto_ch_type_get_type(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the data channel type.
    pub fn set_type(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in type");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_type_set_type(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Update cycle in seconds, if set.
    pub fn update_cycle(&self) -> Option<f64> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_type_has_update_cycle(self.ptr.as_ptr()) }) == 0
        {
            return None;
        }
        // SAFETY: ptr is valid and has_update_cycle is true.
        Some(unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_type_get_update_cycle(self.ptr.as_ptr()) })
    }

    /// Sets the update cycle in seconds.
    pub fn set_update_cycle(&mut self, v: f64) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_type_set_update_cycle(self.ptr.as_ptr(), v) };
    }

    /// Clears the update cycle.
    pub fn clear_update_cycle(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_type_clear_update_cycle(self.ptr.as_ptr()) };
    }

    /// Calculation period in seconds, if set.
    pub fn calculation_period(&self) -> Option<f64> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_type_has_calculation_period(self.ptr.as_ptr()) })
            == 0
        {
            return None;
        }
        // SAFETY: ptr is valid and has_calculation_period is true.
        Some(unsafe {
            ffi::dnv_vista_sdk_dcl_dto_ch_type_get_calculation_period(self.ptr.as_ptr())
        })
    }

    /// Sets the calculation period in seconds.
    pub fn set_calculation_period(&mut self, v: f64) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_type_set_calculation_period(self.ptr.as_ptr(), v) };
    }

    /// Clears the calculation period.
    pub fn clear_calculation_period(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_ch_type_clear_calculation_period(self.ptr.as_ptr()) };
    }
}

// ============================================================
// DtoFormatRef
// ============================================================

/// Non-owning borrow of a `FormatDto`.
pub struct DtoFormatRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_dcl_dto_format_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_dcl_dto_format_t>,
}

impl<'a> DtoFormatRef<'a> {
    /// Format type (e.g. `"Decimal"`, `"Integer"`).
    pub fn type_str(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            cstr(ffi::dnv_vista_sdk_dcl_dto_format_get_type(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the format type (e.g. `"Decimal"`, `"Integer"`).
    pub fn set_type(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in type");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_format_set_type(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Restriction applied to the format, if set.
    pub fn restriction(&mut self) -> Option<DtoRestrictionRef<'_>> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_dto_format_has_restriction(self.ptr.as_ptr()) }) == 0 {
            return None;
        }
        // SAFETY: ptr is valid and has_restriction is true.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_format_get_restriction(self.ptr.as_ptr()) };
        Some(DtoRestrictionRef {
            ptr: NonNull::new(p).expect("get_restriction returned null"),
            _phantom: std::marker::PhantomData,
        })
    }

    /// Ensures a restriction exists (creating a default one if absent).
    pub fn ensure_restriction(&mut self) -> DtoRestrictionRef<'_> {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_format_ensure_restriction(self.ptr.as_ptr()) };
        // SAFETY: after ensure, get returned a non-null non-owning view.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_format_get_restriction(self.ptr.as_ptr()) };
        DtoRestrictionRef {
            ptr: NonNull::new(p).expect("get_restriction returned null"),
            _phantom: std::marker::PhantomData,
        }
    }

    /// Clears the restriction.
    pub fn clear_restriction(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_format_clear_restriction(self.ptr.as_ptr()) };
    }
}

// ============================================================
// DtoRestrictionRef
// ============================================================

/// Non-owning borrow of a `RestrictionDto`.
pub struct DtoRestrictionRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_dcl_dto_restriction_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_dcl_dto_restriction_t>,
}

impl<'a> DtoRestrictionRef<'a> {
    /// Enumeration of acceptable values.
    pub fn enumeration(&self) -> Vec<String> {
        // SAFETY: ptr is valid; count and at are safe to call.
        let n =
            unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_enumeration_count(self.ptr.as_ptr()) };
        (0..n)
            .map(|i| unsafe {
                cstr(ffi::dnv_vista_sdk_dcl_dto_restriction_enumeration_at(
                    self.ptr.as_ptr(),
                    i,
                ))
            })
            .collect()
    }

    /// Sets the enumeration of acceptable values.
    pub fn set_enumeration(&mut self, values: &[&str]) {
        let cstrings: Vec<CString> = values
            .iter()
            .map(|v| CString::new(*v).expect("NUL in enumeration value"))
            .collect();
        let ptrs: Vec<*const std::ffi::c_char> = cstrings.iter().map(|c| c.as_ptr()).collect();
        // SAFETY: ptr is valid; ptrs and cstrings are valid for the call.
        unsafe {
            ffi::dnv_vista_sdk_dcl_dto_restriction_set_enumeration(
                self.ptr.as_ptr(),
                ptrs.as_ptr(),
                ptrs.len(),
            )
        };
    }

    /// Clears the enumeration.
    pub fn clear_enumeration(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_clear_enumeration(self.ptr.as_ptr()) };
    }

    /// Whether a fraction-digits restriction is set.
    pub fn has_fraction_digits(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_fraction_digits(self.ptr.as_ptr()) })
            != 0
    }

    /// Number of significant fraction digits, if set.
    pub fn fraction_digits(&self) -> Option<u32> {
        if (unsafe {
            ffi::dnv_vista_sdk_dcl_dto_restriction_has_fraction_digits(self.ptr.as_ptr())
        }) == 0
        {
            return None;
        }
        // SAFETY: ptr is valid and has_fraction_digits is true.
        Some(unsafe {
            ffi::dnv_vista_sdk_dcl_dto_restriction_get_fraction_digits(self.ptr.as_ptr())
        })
    }

    /// Sets the number of significant fraction digits.
    pub fn set_fraction_digits(&mut self, v: u32) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_set_fraction_digits(self.ptr.as_ptr(), v) };
    }

    /// Clears the fraction digits.
    pub fn clear_fraction_digits(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_clear_fraction_digits(self.ptr.as_ptr()) };
    }

    /// Whether a length restriction is set.
    pub fn has_length(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_length(self.ptr.as_ptr()) }) != 0
    }

    /// Exact number of characters allowed, if set.
    pub fn length(&self) -> Option<u32> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_length(self.ptr.as_ptr()) }) == 0 {
            return None;
        }
        // SAFETY: ptr is valid and has_length is true.
        Some(unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_get_length(self.ptr.as_ptr()) })
    }

    /// Sets the exact number of characters allowed.
    pub fn set_length(&mut self, v: u32) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_set_length(self.ptr.as_ptr(), v) };
    }

    /// Clears the length.
    pub fn clear_length(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_clear_length(self.ptr.as_ptr()) };
    }

    /// Whether a max-exclusive bound is set.
    pub fn has_max_exclusive(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_max_exclusive(self.ptr.as_ptr()) })
            != 0
    }

    /// Upper exclusive bound for numeric values, if set.
    pub fn max_exclusive(&self) -> Option<f64> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_max_exclusive(self.ptr.as_ptr()) })
            == 0
        {
            return None;
        }
        // SAFETY: ptr is valid and has_max_exclusive is true.
        Some(unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_get_max_exclusive(self.ptr.as_ptr()) })
    }

    /// Sets the upper exclusive bound for numeric values.
    pub fn set_max_exclusive(&mut self, v: f64) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_set_max_exclusive(self.ptr.as_ptr(), v) };
    }

    /// Clears the max exclusive bound.
    pub fn clear_max_exclusive(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_clear_max_exclusive(self.ptr.as_ptr()) };
    }

    /// Whether a max-inclusive bound is set.
    pub fn has_max_inclusive(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_max_inclusive(self.ptr.as_ptr()) })
            != 0
    }

    /// Upper inclusive bound for numeric values, if set.
    pub fn max_inclusive(&self) -> Option<f64> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_max_inclusive(self.ptr.as_ptr()) })
            == 0
        {
            return None;
        }
        // SAFETY: ptr is valid and has_max_inclusive is true.
        Some(unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_get_max_inclusive(self.ptr.as_ptr()) })
    }

    /// Sets the upper inclusive bound for numeric values.
    pub fn set_max_inclusive(&mut self, v: f64) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_set_max_inclusive(self.ptr.as_ptr(), v) };
    }

    /// Clears the max inclusive bound.
    pub fn clear_max_inclusive(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_clear_max_inclusive(self.ptr.as_ptr()) };
    }

    /// Whether a max-length restriction is set.
    pub fn has_max_length(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_max_length(self.ptr.as_ptr()) }) != 0
    }

    /// Maximum number of characters allowed, if set.
    pub fn max_length(&self) -> Option<u32> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_max_length(self.ptr.as_ptr()) })
            == 0
        {
            return None;
        }
        // SAFETY: ptr is valid and has_max_length is true.
        Some(unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_get_max_length(self.ptr.as_ptr()) })
    }

    /// Sets the maximum number of characters allowed.
    pub fn set_max_length(&mut self, v: u32) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_set_max_length(self.ptr.as_ptr(), v) };
    }

    /// Clears the max length.
    pub fn clear_max_length(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_clear_max_length(self.ptr.as_ptr()) };
    }

    /// Whether a min-exclusive bound is set.
    pub fn has_min_exclusive(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_min_exclusive(self.ptr.as_ptr()) })
            != 0
    }

    /// Lower exclusive bound for numeric values, if set.
    pub fn min_exclusive(&self) -> Option<f64> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_min_exclusive(self.ptr.as_ptr()) })
            == 0
        {
            return None;
        }
        // SAFETY: ptr is valid and has_min_exclusive is true.
        Some(unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_get_min_exclusive(self.ptr.as_ptr()) })
    }

    /// Sets the lower exclusive bound for numeric values.
    pub fn set_min_exclusive(&mut self, v: f64) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_set_min_exclusive(self.ptr.as_ptr(), v) };
    }

    /// Clears the min exclusive bound.
    pub fn clear_min_exclusive(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_clear_min_exclusive(self.ptr.as_ptr()) };
    }

    /// Whether a min-inclusive bound is set.
    pub fn has_min_inclusive(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_min_inclusive(self.ptr.as_ptr()) })
            != 0
    }

    /// Lower inclusive bound for numeric values, if set.
    pub fn min_inclusive(&self) -> Option<f64> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_min_inclusive(self.ptr.as_ptr()) })
            == 0
        {
            return None;
        }
        // SAFETY: ptr is valid and has_min_inclusive is true.
        Some(unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_get_min_inclusive(self.ptr.as_ptr()) })
    }

    /// Sets the lower inclusive bound for numeric values.
    pub fn set_min_inclusive(&mut self, v: f64) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_set_min_inclusive(self.ptr.as_ptr(), v) };
    }

    /// Clears the min inclusive bound.
    pub fn clear_min_inclusive(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_clear_min_inclusive(self.ptr.as_ptr()) };
    }

    /// Whether a min-length restriction is set.
    pub fn has_min_length(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_min_length(self.ptr.as_ptr()) }) != 0
    }

    /// Minimum number of characters allowed, if set.
    pub fn min_length(&self) -> Option<u32> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_min_length(self.ptr.as_ptr()) })
            == 0
        {
            return None;
        }
        // SAFETY: ptr is valid and has_min_length is true.
        Some(unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_get_min_length(self.ptr.as_ptr()) })
    }

    /// Sets the minimum number of characters allowed.
    pub fn set_min_length(&mut self, v: u32) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_set_min_length(self.ptr.as_ptr(), v) };
    }

    /// Clears the min length.
    pub fn clear_min_length(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_clear_min_length(self.ptr.as_ptr()) };
    }

    /// Whether a pattern restriction is set.
    pub fn has_pattern(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_pattern(self.ptr.as_ptr()) }) != 0
    }

    /// Regular-expression pattern, if set.
    pub fn pattern(&self) -> Option<String> {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_dcl_dto_restriction_get_pattern(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the regular-expression pattern.
    pub fn set_pattern(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in pattern");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe {
            ffi::dnv_vista_sdk_dcl_dto_restriction_set_pattern(self.ptr.as_ptr(), s.as_ptr())
        };
    }

    /// Clears the pattern.
    pub fn clear_pattern(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_clear_pattern(self.ptr.as_ptr()) };
    }

    /// Whether a total-digits restriction is set.
    pub fn has_total_digits(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_total_digits(self.ptr.as_ptr()) }) != 0
    }

    /// Exact number of digits allowed, if set.
    pub fn total_digits(&self) -> Option<u32> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_total_digits(self.ptr.as_ptr()) })
            == 0
        {
            return None;
        }
        // SAFETY: ptr is valid and has_total_digits is true.
        Some(unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_get_total_digits(self.ptr.as_ptr()) })
    }

    /// Sets the exact number of digits allowed.
    pub fn set_total_digits(&mut self, v: u32) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_set_total_digits(self.ptr.as_ptr(), v) };
    }

    /// Clears the total digits.
    pub fn clear_total_digits(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_clear_total_digits(self.ptr.as_ptr()) };
    }

    /// Whether a white-space handling mode is set.
    pub fn has_white_space(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_has_white_space(self.ptr.as_ptr()) }) != 0
    }

    /// White-space handling mode (`"Preserve"`, `"Replace"`, `"Collapse"`), if set.
    pub fn white_space(&self) -> Option<String> {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_dcl_dto_restriction_get_white_space(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the white-space handling mode.
    pub fn set_white_space(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in white_space");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe {
            ffi::dnv_vista_sdk_dcl_dto_restriction_set_white_space(self.ptr.as_ptr(), s.as_ptr())
        };
    }

    /// Clears the white-space handling mode.
    pub fn clear_white_space(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_restriction_clear_white_space(self.ptr.as_ptr()) };
    }
}

// ============================================================
// DtoRangeRef
// ============================================================

/// Non-owning borrow of a `RangeDto`.
pub struct DtoRangeRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_dcl_dto_range_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_dcl_dto_range_t>,
}

impl<'a> DtoRangeRef<'a> {
    /// Lower bound of the range.
    pub fn low(&self) -> f64 {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_range_get_low(self.ptr.as_ptr()) }
    }

    /// Upper bound of the range.
    pub fn high(&self) -> f64 {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_range_get_high(self.ptr.as_ptr()) }
    }

    /// Sets the lower bound of the range.
    pub fn set_low(&mut self, v: f64) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_range_set_low(self.ptr.as_ptr(), v) };
    }

    /// Sets the upper bound of the range.
    pub fn set_high(&mut self, v: f64) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_range_set_high(self.ptr.as_ptr(), v) };
    }
}

// ============================================================
// DtoUnitRef
// ============================================================

/// Non-owning borrow of a `UnitDto`.
pub struct DtoUnitRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_dcl_dto_unit_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_dcl_dto_unit_t>,
}

impl<'a> DtoUnitRef<'a> {
    /// Unit symbol (e.g. `"bar"`, `"degC"`).
    pub fn symbol(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            cstr(ffi::dnv_vista_sdk_dcl_dto_unit_get_symbol(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the unit symbol.
    pub fn set_symbol(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in symbol");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_unit_set_symbol(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Whether a quantity name is set.
    pub fn has_quantity_name(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_unit_has_quantity_name(self.ptr.as_ptr()) }) != 0
    }

    /// Quantity name of the measurement value, if set.
    pub fn quantity_name(&self) -> Option<String> {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_dcl_dto_unit_get_quantity_name(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the quantity name.
    pub fn set_quantity_name(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in quantity_name");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_unit_set_quantity_name(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Clears the quantity name.
    pub fn clear_quantity_name(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_unit_clear_quantity_name(self.ptr.as_ptr()) };
    }

    /// Whether custom elements are set.
    pub fn has_custom_elements(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_dcl_dto_unit_has_custom_elements(self.ptr.as_ptr()) }) != 0
    }

    /// Mutable view of the custom elements, or `None` if absent.
    pub fn custom_elements(&mut self) -> Option<SerializableDocumentRefMut<'_>> {
        if !self.has_custom_elements() {
            return None;
        }
        // SAFETY: ptr is valid and has_custom_elements is true. The returned pointer is a
        // non-owning view into the unit, valid for the lifetime of this borrow.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_unit_get_custom_elements(self.ptr.as_ptr()) };
        Some(unsafe { SerializableDocumentRefMut::from_ptr_mut(p) })
    }

    /// Ensures custom elements exist (creating an empty object if absent).
    pub fn ensure_custom_elements(&mut self) -> SerializableDocumentRefMut<'_> {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_unit_ensure_custom_elements(self.ptr.as_ptr()) };
        // SAFETY: after ensure, get returned a non-null non-owning view.
        let p = unsafe { ffi::dnv_vista_sdk_dcl_dto_unit_get_custom_elements(self.ptr.as_ptr()) };
        unsafe { SerializableDocumentRefMut::from_ptr_mut(p) }
    }

    /// Clears the custom elements.
    pub fn clear_custom_elements(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_dcl_dto_unit_clear_custom_elements(self.ptr.as_ptr()) };
    }
}

// ============================================================
// Free functions: to_dto / to_domain / from_json / to_json
// ============================================================

/// Converts a domain `DataChannelListPackage` to an owned `DtoPackage`.
pub fn to_dto(domain: &DataChannelListPackage) -> Result<DtoPackage, VistaError> {
    // SAFETY: domain is valid for the call's duration. Returns an owned pointer or NULL.
    let ptr = unsafe { ffi::dnv_vista_sdk_dcl_to_dto(domain.as_ptr()) };
    if ptr.is_null() {
        Err(last_error())
    } else {
        // SAFETY: ptr is non-null and owned.
        Ok(unsafe { DtoPackage::from_raw(ptr) })
    }
}

/// Converts an owned `DtoPackage` back to a domain `DataChannelListPackage`.
pub fn to_domain(dto: &DtoPackage) -> Result<DataChannelListPackage, VistaError> {
    // SAFETY: dto is valid for the call's duration. Returns an owned pointer or NULL.
    let ptr = unsafe { ffi::dnv_vista_sdk_dcl_to_domain(dto.as_ptr()) };
    if ptr.is_null() {
        Err(last_error())
    } else {
        // SAFETY: ptr is non-null and owned.
        Ok(unsafe { DataChannelListPackage::from_raw(ptr) })
    }
}

/// Encodes `json` as a NUL-terminated C string, or reports the failure.
fn c_json(json: &str) -> Result<CString, VistaError> {
    CString::new(json).map_err(|_| VistaError {
        kind: crate::core::error::ErrorKind::InvalidArgument,
        message: "JSON string contains a NUL byte".to_string(),
    })
}

/// Parses a `DtoPackage` from a JSON string.
pub fn from_json(json: &str) -> Result<DtoPackage, VistaError> {
    let c = c_json(json)?;
    // SAFETY: c is valid for the call's duration. Returns an owned pointer or NULL.
    let ptr = unsafe { ffi::dnv_vista_sdk_dcl_dto_from_json(c.as_ptr()) };
    if ptr.is_null() {
        Err(last_error())
    } else {
        // SAFETY: ptr is non-null and owned.
        Ok(unsafe { DtoPackage::from_raw(ptr) })
    }
}

/// Serializes a `DtoPackage` to a JSON string.
pub fn to_json(dto: &DtoPackage, pretty_print: bool) -> String {
    // SAFETY: dto is valid for the call's duration. Returns an owned pointer or NULL.
    let raw = unsafe {
        ffi::dnv_vista_sdk_dcl_dto_to_json(dto.as_ptr(), pretty_print as std::ffi::c_int)
    };
    if raw.is_null() {
        return String::new();
    }
    // SAFETY: raw is non-null (checked above) and owned until freed below. Invalid UTF-8 is
    // replaced rather than panicking.
    let s = unsafe { CStr::from_ptr(raw) }
        .to_string_lossy()
        .into_owned();
    // SAFETY: raw was allocated by the library and is freed exactly once.
    unsafe { dnv_vista_sdk_string_free(raw) };
    s
}
