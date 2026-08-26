use std::ffi::{CStr, CString};
use std::ptr::NonNull;

use crate::core::error::{last_error, VistaError};
use crate::ffi::core::common::dnv_vista_sdk_string_free;
use crate::ffi::transport::timeseries::time_series_data_dto as ffi;
use crate::transport::serializable_document::SerializableDocumentRefMut;
use crate::transport::timeseries::time_series_data::TimeSeriesDataPackage;

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
// TsdDtoPackage
// ============================================================

/// Owned handle to a `TimeSeriesDataPackageDto`. Free on drop.
pub struct TsdDtoPackage {
    ptr: NonNull<ffi::dnv_vista_sdk_tsd_dto_package_t>,
}

impl Drop for TsdDtoPackage {
    fn drop(&mut self) {
        // SAFETY: ptr is owned and valid. Freed exactly once.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_package_free(self.ptr.as_ptr()) };
    }
}

impl TsdDtoPackage {
    pub(crate) unsafe fn from_raw(ptr: *mut ffi::dnv_vista_sdk_tsd_dto_package_t) -> Self {
        Self {
            ptr: NonNull::new(ptr).expect("TsdDtoPackage::from_raw called with null"),
        }
    }

    pub(crate) fn as_ptr(&self) -> *mut ffi::dnv_vista_sdk_tsd_dto_package_t {
        self.ptr.as_ptr()
    }

    /// Non-owning view of the embedded `PackageDto`.
    pub fn pkg(&mut self) -> TsdDtoPkgRef<'_> {
        // SAFETY: ptr is valid; result is a non-owning borrow.
        let p = unsafe { ffi::dnv_vista_sdk_tsd_dto_package_get_pkg(self.ptr.as_ptr()) };
        TsdDtoPkgRef {
            ptr: NonNull::new(p).expect("get_pkg returned null"),
            _phantom: std::marker::PhantomData,
        }
    }
}

// ============================================================
// TsdDtoPkgRef
// ============================================================

/// Non-owning borrow of a `PackageDto`.
pub struct TsdDtoPkgRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_tsd_dto_pkg_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_tsd_dto_pkg_t>,
}

impl<'a> TsdDtoPkgRef<'a> {
    /// Whether the optional `HeaderDto` is set.
    pub fn has_header(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_tsd_dto_pkg_has_header(self.ptr.as_ptr()) }) != 0
    }

    /// Non-owning view of the optional `HeaderDto`, or `None` if absent.
    pub fn header(&mut self) -> Option<TsdDtoHeaderRef<'_>> {
        if !self.has_header() {
            return None;
        }
        // SAFETY: ptr is valid and has_header is true.
        let p = unsafe { ffi::dnv_vista_sdk_tsd_dto_pkg_get_header(self.ptr.as_ptr()) };
        Some(TsdDtoHeaderRef {
            ptr: NonNull::new(p).expect("get_header returned null"),
            _phantom: std::marker::PhantomData,
        })
    }

    /// Ensures a `HeaderDto` exists (creating a default one if absent).
    pub fn ensure_header(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_pkg_ensure_header(self.ptr.as_ptr()) };
    }

    /// Clears the `HeaderDto`.
    pub fn clear_header(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_pkg_clear_header(self.ptr.as_ptr()) };
    }

    /// Number of `TimeSeriesDataDto` entries.
    pub fn tsd_count(&self) -> usize {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_pkg_tsd_count(self.ptr.as_ptr()) }
    }

    /// Mutable view of the `TimeSeriesDataDto` at `index`, or `None` if out of range.
    pub fn tsd_at(&mut self, index: usize) -> Option<TsdDtoTsdRef<'_>> {
        // SAFETY: ptr is valid; index bounds checked by C function (returns null).
        let p = unsafe { ffi::dnv_vista_sdk_tsd_dto_pkg_tsd_at(self.ptr.as_ptr(), index) };
        if p.is_null() {
            return None;
        }
        Some(TsdDtoTsdRef {
            ptr: NonNull::new(p).unwrap(),
            _phantom: std::marker::PhantomData,
        })
    }

    /// Appends a default-constructed `TimeSeriesDataDto` and returns a mutable view.
    pub fn tsd_push(&mut self) -> TsdDtoTsdRef<'_> {
        // SAFETY: ptr is valid; result is a non-owning borrow.
        let p = unsafe { ffi::dnv_vista_sdk_tsd_dto_pkg_tsd_push(self.ptr.as_ptr()) };
        TsdDtoTsdRef {
            ptr: NonNull::new(p).expect("tsd_push returned null"),
            _phantom: std::marker::PhantomData,
        }
    }

    /// Removes the `TimeSeriesDataDto` at `index`, shifting subsequent elements.
    pub fn tsd_remove(&mut self, index: usize) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_pkg_tsd_remove(self.ptr.as_ptr(), index) };
    }
}

// ============================================================
// TsdDtoHeaderRef
// ============================================================

/// Non-owning borrow of a `HeaderDto`.
pub struct TsdDtoHeaderRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_tsd_dto_header_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_tsd_dto_header_t>,
}

impl<'a> TsdDtoHeaderRef<'a> {
    /// Ship identifier (IMO number or alternative).
    pub fn ship_id(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            cstr(ffi::dnv_vista_sdk_tsd_dto_header_get_ship_id(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the ship identifier.
    pub fn set_ship_id(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in ship_id");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_header_set_ship_id(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Whether a `TimeSpanDto` is set.
    pub fn has_time_span(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_tsd_dto_header_has_time_span(self.ptr.as_ptr()) }) != 0
    }

    /// Mutable view of the `TimeSpanDto`, or `None` if absent.
    pub fn time_span(&mut self) -> Option<TsdDtoTimeSpanRef<'_>> {
        if !self.has_time_span() {
            return None;
        }
        // SAFETY: ptr is valid and has_time_span is true.
        let p = unsafe { ffi::dnv_vista_sdk_tsd_dto_header_get_time_span(self.ptr.as_ptr()) };
        Some(TsdDtoTimeSpanRef {
            ptr: NonNull::new(p).expect("get_time_span returned null"),
            _phantom: std::marker::PhantomData,
        })
    }

    /// Ensures a `TimeSpanDto` exists (creating a default one if absent).
    pub fn ensure_time_span(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_header_ensure_time_span(self.ptr.as_ptr()) };
    }

    /// Clears the `TimeSpanDto`.
    pub fn clear_time_span(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_header_clear_time_span(self.ptr.as_ptr()) };
    }

    /// Whether a creation date is set.
    pub fn has_date_created(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_tsd_dto_header_has_date_created(self.ptr.as_ptr()) }) != 0
    }

    /// Creation date of the package, if set.
    pub fn date_created(&self) -> Option<String> {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_tsd_dto_header_get_date_created(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the creation date.
    pub fn set_date_created(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in date_created");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe {
            ffi::dnv_vista_sdk_tsd_dto_header_set_date_created(self.ptr.as_ptr(), s.as_ptr())
        };
    }

    /// Clears the creation date.
    pub fn clear_date_created(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_header_clear_date_created(self.ptr.as_ptr()) };
    }

    /// Whether a modification date is set.
    pub fn has_date_modified(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_tsd_dto_header_has_date_modified(self.ptr.as_ptr()) }) != 0
    }

    /// Modification date of the package, if set.
    pub fn date_modified(&self) -> Option<String> {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_tsd_dto_header_get_date_modified(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the modification date.
    pub fn set_date_modified(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in date_modified");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe {
            ffi::dnv_vista_sdk_tsd_dto_header_set_date_modified(self.ptr.as_ptr(), s.as_ptr())
        };
    }

    /// Clears the modification date.
    pub fn clear_date_modified(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_header_clear_date_modified(self.ptr.as_ptr()) };
    }

    /// Whether an author is set.
    pub fn has_author(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_tsd_dto_header_has_author(self.ptr.as_ptr()) }) != 0
    }

    /// Author of the package, if set.
    pub fn author(&self) -> Option<String> {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_tsd_dto_header_get_author(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the author.
    pub fn set_author(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in author");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_header_set_author(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Clears the author.
    pub fn clear_author(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_header_clear_author(self.ptr.as_ptr()) };
    }

    /// Number of system configuration references.
    pub fn system_cfg_count(&self) -> usize {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_header_system_cfg_count(self.ptr.as_ptr()) }
    }

    /// Mutable view of the configuration reference at `index`, or `None` if out of range.
    pub fn system_cfg_at(&mut self, index: usize) -> Option<TsdDtoCfgRefRef<'_>> {
        // SAFETY: ptr is valid; index bounds checked by C function (returns null).
        let p =
            unsafe { ffi::dnv_vista_sdk_tsd_dto_header_system_cfg_at(self.ptr.as_ptr(), index) };
        if p.is_null() {
            return None;
        }
        Some(TsdDtoCfgRefRef {
            ptr: NonNull::new(p).unwrap(),
            _phantom: std::marker::PhantomData,
        })
    }

    /// Appends a default-constructed configuration reference and returns a mutable view.
    pub fn system_cfg_push(&mut self) -> TsdDtoCfgRefRef<'_> {
        // SAFETY: ptr is valid; result is a non-owning borrow.
        let p = unsafe { ffi::dnv_vista_sdk_tsd_dto_header_system_cfg_push(self.ptr.as_ptr()) };
        TsdDtoCfgRefRef {
            ptr: NonNull::new(p).expect("system_cfg_push returned null"),
            _phantom: std::marker::PhantomData,
        }
    }

    /// Removes the configuration reference at `index`, shifting subsequent elements.
    pub fn system_cfg_remove(&mut self, index: usize) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_header_system_cfg_remove(self.ptr.as_ptr(), index) };
    }

    /// Whether custom headers are set.
    pub fn has_custom_headers(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_tsd_dto_header_has_custom_headers(self.ptr.as_ptr()) }) != 0
    }

    /// Mutable view of the custom headers, or `None` if absent.
    pub fn custom_headers(&mut self) -> Option<SerializableDocumentRefMut<'_>> {
        if !self.has_custom_headers() {
            return None;
        }
        // SAFETY: ptr is valid and has_custom_headers is true.
        let p = unsafe { ffi::dnv_vista_sdk_tsd_dto_header_get_custom_headers(self.ptr.as_ptr()) };
        Some(unsafe { SerializableDocumentRefMut::from_ptr_mut(p) })
    }

    /// Ensures custom headers exist (creating an empty object if absent) and returns a mutable view.
    pub fn ensure_custom_headers(&mut self) -> SerializableDocumentRefMut<'_> {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_header_ensure_custom_headers(self.ptr.as_ptr()) };
        // SAFETY: after ensure, get returns a non-null non-owning view.
        let p = unsafe { ffi::dnv_vista_sdk_tsd_dto_header_get_custom_headers(self.ptr.as_ptr()) };
        unsafe { SerializableDocumentRefMut::from_ptr_mut(p) }
    }

    /// Clears the custom headers.
    pub fn clear_custom_headers(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_header_clear_custom_headers(self.ptr.as_ptr()) };
    }
}

// ============================================================
// TsdDtoTimeSpanRef
// ============================================================

/// Non-owning borrow of a `TimeSpanDto`.
pub struct TsdDtoTimeSpanRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_tsd_dto_time_span_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_tsd_dto_time_span_t>,
}

impl<'a> TsdDtoTimeSpanRef<'a> {
    /// Start of the time span (ISO 8601 string).
    pub fn start(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            cstr(ffi::dnv_vista_sdk_tsd_dto_time_span_get_start(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the start of the time span.
    pub fn set_start(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in start");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_time_span_set_start(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// End of the time span (ISO 8601 string).
    pub fn end(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            cstr(ffi::dnv_vista_sdk_tsd_dto_time_span_get_end(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the end of the time span.
    pub fn set_end(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in end");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_time_span_set_end(self.ptr.as_ptr(), s.as_ptr()) };
    }
}

// ============================================================
// TsdDtoCfgRefRef
// ============================================================

/// Non-owning borrow of a `ConfigurationReferenceDto`.
pub struct TsdDtoCfgRefRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_tsd_dto_cfg_ref_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_tsd_dto_cfg_ref_t>,
}

impl<'a> TsdDtoCfgRefRef<'a> {
    /// Identifier of the referenced configuration.
    pub fn id(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe { cstr(ffi::dnv_vista_sdk_tsd_dto_cfg_ref_get_id(self.ptr.as_ptr())) }
    }

    /// Sets the identifier of the referenced configuration.
    pub fn set_id(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in id");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_cfg_ref_set_id(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Timestamp of the referenced configuration.
    pub fn timestamp(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            cstr(ffi::dnv_vista_sdk_tsd_dto_cfg_ref_get_timestamp(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the timestamp of the referenced configuration.
    pub fn set_timestamp(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in timestamp");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_cfg_ref_set_timestamp(self.ptr.as_ptr(), s.as_ptr()) };
    }
}

// ============================================================
// TsdDtoTsdRef
// ============================================================

/// Non-owning borrow of a `TimeSeriesDataDto`.
pub struct TsdDtoTsdRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_tsd_dto_tsd_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_tsd_dto_tsd_t>,
}

impl<'a> TsdDtoTsdRef<'a> {
    /// Whether a data configuration reference is set.
    pub fn has_data_cfg(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_tsd_dto_tsd_has_data_cfg(self.ptr.as_ptr()) }) != 0
    }

    /// Mutable view of the data configuration reference, or `None` if absent.
    pub fn data_cfg(&mut self) -> Option<TsdDtoCfgRefRef<'_>> {
        if !self.has_data_cfg() {
            return None;
        }
        // SAFETY: ptr is valid and has_data_cfg is true.
        let p = unsafe { ffi::dnv_vista_sdk_tsd_dto_tsd_get_data_cfg(self.ptr.as_ptr()) };
        Some(TsdDtoCfgRefRef {
            ptr: NonNull::new(p).expect("get_data_cfg returned null"),
            _phantom: std::marker::PhantomData,
        })
    }

    /// Ensures a data configuration reference exists (creating a default one if absent).
    pub fn ensure_data_cfg(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_tsd_ensure_data_cfg(self.ptr.as_ptr()) };
    }

    /// Clears the data configuration reference.
    pub fn clear_data_cfg(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_tsd_clear_data_cfg(self.ptr.as_ptr()) };
    }

    /// Number of `TabularDataDto` entries.
    pub fn tabular_count(&self) -> usize {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_tsd_tabular_count(self.ptr.as_ptr()) }
    }

    /// Mutable view of the `TabularDataDto` at `index`, or `None` if out of range.
    pub fn tabular_at(&mut self, index: usize) -> Option<TsdDtoTabularRef<'_>> {
        // SAFETY: ptr is valid; index bounds checked by C function (returns null).
        let p = unsafe { ffi::dnv_vista_sdk_tsd_dto_tsd_tabular_at(self.ptr.as_ptr(), index) };
        if p.is_null() {
            return None;
        }
        Some(TsdDtoTabularRef {
            ptr: NonNull::new(p).unwrap(),
            _phantom: std::marker::PhantomData,
        })
    }

    /// Appends a default-constructed `TabularDataDto` and returns a mutable view.
    pub fn tabular_push(&mut self) -> TsdDtoTabularRef<'_> {
        // SAFETY: ptr is valid; result is a non-owning borrow.
        let p = unsafe { ffi::dnv_vista_sdk_tsd_dto_tsd_tabular_push(self.ptr.as_ptr()) };
        TsdDtoTabularRef {
            ptr: NonNull::new(p).expect("tabular_push returned null"),
            _phantom: std::marker::PhantomData,
        }
    }

    /// Removes the `TabularDataDto` at `index`, shifting subsequent elements.
    pub fn tabular_remove(&mut self, index: usize) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_tsd_tabular_remove(self.ptr.as_ptr(), index) };
    }

    /// Whether event data is set.
    pub fn has_event(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_tsd_dto_tsd_has_event(self.ptr.as_ptr()) }) != 0
    }

    /// Mutable view of the `EventDataDto`, or `None` if absent.
    pub fn event(&mut self) -> Option<TsdDtoEventRef<'_>> {
        if !self.has_event() {
            return None;
        }
        // SAFETY: ptr is valid and has_event is true.
        let p = unsafe { ffi::dnv_vista_sdk_tsd_dto_tsd_get_event(self.ptr.as_ptr()) };
        Some(TsdDtoEventRef {
            ptr: NonNull::new(p).expect("get_event returned null"),
            _phantom: std::marker::PhantomData,
        })
    }

    /// Ensures event data exists (creating a default one if absent).
    pub fn ensure_event(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_tsd_ensure_event(self.ptr.as_ptr()) };
    }

    /// Clears the event data.
    pub fn clear_event(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_tsd_clear_event(self.ptr.as_ptr()) };
    }

    /// Whether custom data kinds are set.
    pub fn has_custom_data_kinds(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_tsd_dto_tsd_has_custom_data_kinds(self.ptr.as_ptr()) }) != 0
    }

    /// Mutable view of the custom data kinds, or `None` if absent.
    pub fn custom_data_kinds(&mut self) -> Option<SerializableDocumentRefMut<'_>> {
        if !self.has_custom_data_kinds() {
            return None;
        }
        // SAFETY: ptr is valid and has_custom_data_kinds is true.
        let p = unsafe { ffi::dnv_vista_sdk_tsd_dto_tsd_get_custom_data_kinds(self.ptr.as_ptr()) };
        Some(unsafe { SerializableDocumentRefMut::from_ptr_mut(p) })
    }

    /// Ensures custom data kinds exist (creating an empty object if absent) and returns a mutable view.
    pub fn ensure_custom_data_kinds(&mut self) -> SerializableDocumentRefMut<'_> {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_tsd_ensure_custom_data_kinds(self.ptr.as_ptr()) };
        // SAFETY: after ensure, get returns a non-null non-owning view.
        let p = unsafe { ffi::dnv_vista_sdk_tsd_dto_tsd_get_custom_data_kinds(self.ptr.as_ptr()) };
        unsafe { SerializableDocumentRefMut::from_ptr_mut(p) }
    }

    /// Clears the custom data kinds.
    pub fn clear_custom_data_kinds(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_tsd_clear_custom_data_kinds(self.ptr.as_ptr()) };
    }
}

// ============================================================
// TsdDtoTabularRef
// ============================================================

/// Non-owning borrow of a `TabularDataDto`.
pub struct TsdDtoTabularRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_tsd_dto_tabular_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_tsd_dto_tabular_t>,
}

impl<'a> TsdDtoTabularRef<'a> {
    /// Number of channel ID strings.
    pub fn channel_id_count(&self) -> usize {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_tabular_channel_id_count(self.ptr.as_ptr()) }
    }

    /// Channel ID string at `index`, or `None` if out of range.
    pub fn channel_id_at(&self, index: usize) -> Option<String> {
        // SAFETY: ptr is valid; index bounds checked by C function (returns null).
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_tsd_dto_tabular_channel_id_at(
                self.ptr.as_ptr(),
                index,
            ))
        }
    }

    /// Replaces all channel IDs with the given slice.
    pub fn set_channel_ids(&mut self, ids: &[&str]) {
        let cstrings: Vec<CString> = ids
            .iter()
            .map(|v| CString::new(*v).expect("NUL in channel_id"))
            .collect();
        let ptrs: Vec<*const std::ffi::c_char> = cstrings.iter().map(|c| c.as_ptr()).collect();
        // SAFETY: ptr is valid; ptrs and cstrings are valid for the call.
        unsafe {
            ffi::dnv_vista_sdk_tsd_dto_tabular_set_channel_ids(
                self.ptr.as_ptr(),
                ptrs.as_ptr(),
                ptrs.len(),
            )
        };
    }

    /// Clears all channel IDs.
    pub fn clear_channel_ids(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_tabular_clear_channel_ids(self.ptr.as_ptr()) };
    }

    /// Number of `TabularDataSetDto` entries.
    pub fn data_set_count(&self) -> usize {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_tabular_data_set_count(self.ptr.as_ptr()) }
    }

    /// Mutable view of the `TabularDataSetDto` at `index`, or `None` if out of range.
    pub fn data_set_at(&mut self, index: usize) -> Option<TsdDtoTabSetRef<'_>> {
        // SAFETY: ptr is valid; index bounds checked by C function (returns null).
        let p = unsafe { ffi::dnv_vista_sdk_tsd_dto_tabular_data_set_at(self.ptr.as_ptr(), index) };
        if p.is_null() {
            return None;
        }
        Some(TsdDtoTabSetRef {
            ptr: NonNull::new(p).unwrap(),
            _phantom: std::marker::PhantomData,
        })
    }

    /// Appends a default-constructed `TabularDataSetDto` and returns a mutable view.
    pub fn data_set_push(&mut self) -> TsdDtoTabSetRef<'_> {
        // SAFETY: ptr is valid; result is a non-owning borrow.
        let p = unsafe { ffi::dnv_vista_sdk_tsd_dto_tabular_data_set_push(self.ptr.as_ptr()) };
        TsdDtoTabSetRef {
            ptr: NonNull::new(p).expect("data_set_push returned null"),
            _phantom: std::marker::PhantomData,
        }
    }

    /// Removes the `TabularDataSetDto` at `index`, shifting subsequent elements.
    pub fn data_set_remove(&mut self, index: usize) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_tabular_data_set_remove(self.ptr.as_ptr(), index) };
    }
}

// ============================================================
// TsdDtoTabSetRef
// ============================================================

/// Non-owning borrow of a `TabularDataSetDto`.
pub struct TsdDtoTabSetRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_tsd_dto_tab_set_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_tsd_dto_tab_set_t>,
}

impl<'a> TsdDtoTabSetRef<'a> {
    /// Timestamp of the data set (ISO 8601 string).
    pub fn timestamp(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            cstr(ffi::dnv_vista_sdk_tsd_dto_tab_set_get_timestamp(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the timestamp.
    pub fn set_timestamp(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in timestamp");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_tab_set_set_timestamp(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Number of value strings.
    pub fn value_count(&self) -> usize {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_tab_set_value_count(self.ptr.as_ptr()) }
    }

    /// Value string at `index`, or `None` if out of range.
    pub fn value_at(&self, index: usize) -> Option<String> {
        // SAFETY: ptr is valid; index bounds checked by C function (returns null).
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_tsd_dto_tab_set_value_at(
                self.ptr.as_ptr(),
                index,
            ))
        }
    }

    /// Replaces all values with the given slice.
    pub fn set_values(&mut self, values: &[&str]) {
        let cstrings: Vec<CString> = values
            .iter()
            .map(|v| CString::new(*v).expect("NUL in value"))
            .collect();
        let ptrs: Vec<*const std::ffi::c_char> = cstrings.iter().map(|c| c.as_ptr()).collect();
        // SAFETY: ptr is valid; ptrs and cstrings are valid for the call.
        unsafe {
            ffi::dnv_vista_sdk_tsd_dto_tab_set_set_values(
                self.ptr.as_ptr(),
                ptrs.as_ptr(),
                ptrs.len(),
            )
        };
    }

    /// Number of quality strings, or 0 if absent.
    pub fn quality_count(&self) -> usize {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_tab_set_quality_count(self.ptr.as_ptr()) }
    }

    /// Quality string at `index`, or `None` if absent or out of range.
    pub fn quality_at(&self, index: usize) -> Option<String> {
        // SAFETY: ptr is valid; index bounds checked by C function (returns null).
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_tsd_dto_tab_set_quality_at(
                self.ptr.as_ptr(),
                index,
            ))
        }
    }

    /// Replaces all quality codes with the given slice.
    pub fn set_quality(&mut self, quality: &[&str]) {
        let cstrings: Vec<CString> = quality
            .iter()
            .map(|v| CString::new(*v).expect("NUL in quality"))
            .collect();
        let ptrs: Vec<*const std::ffi::c_char> = cstrings.iter().map(|c| c.as_ptr()).collect();
        // SAFETY: ptr is valid; ptrs and cstrings are valid for the call.
        unsafe {
            ffi::dnv_vista_sdk_tsd_dto_tab_set_set_quality(
                self.ptr.as_ptr(),
                ptrs.as_ptr(),
                ptrs.len(),
            )
        };
    }

    /// Clears all quality codes.
    pub fn clear_quality(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_tab_set_clear_quality(self.ptr.as_ptr()) };
    }
}

// ============================================================
// TsdDtoEventRef
// ============================================================

/// Non-owning borrow of an `EventDataDto`.
pub struct TsdDtoEventRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_tsd_dto_event_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_tsd_dto_event_t>,
}

impl<'a> TsdDtoEventRef<'a> {
    /// Number of `EventDataSetDto` entries.
    pub fn data_set_count(&self) -> usize {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_event_data_set_count(self.ptr.as_ptr()) }
    }

    /// Mutable view of the `EventDataSetDto` at `index`, or `None` if out of range.
    pub fn data_set_at(&mut self, index: usize) -> Option<TsdDtoEventSetRef<'_>> {
        // SAFETY: ptr is valid; index bounds checked by C function (returns null).
        let p = unsafe { ffi::dnv_vista_sdk_tsd_dto_event_data_set_at(self.ptr.as_ptr(), index) };
        if p.is_null() {
            return None;
        }
        Some(TsdDtoEventSetRef {
            ptr: NonNull::new(p).unwrap(),
            _phantom: std::marker::PhantomData,
        })
    }

    /// Appends a default-constructed `EventDataSetDto` and returns a mutable view.
    pub fn data_set_push(&mut self) -> TsdDtoEventSetRef<'_> {
        // SAFETY: ptr is valid; result is a non-owning borrow.
        let p = unsafe { ffi::dnv_vista_sdk_tsd_dto_event_data_set_push(self.ptr.as_ptr()) };
        TsdDtoEventSetRef {
            ptr: NonNull::new(p).expect("data_set_push returned null"),
            _phantom: std::marker::PhantomData,
        }
    }

    /// Removes the `EventDataSetDto` at `index`, shifting subsequent elements.
    pub fn data_set_remove(&mut self, index: usize) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_event_data_set_remove(self.ptr.as_ptr(), index) };
    }
}

// ============================================================
// TsdDtoEventSetRef
// ============================================================

/// Non-owning borrow of an `EventDataSetDto`.
pub struct TsdDtoEventSetRef<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_tsd_dto_event_set_t>,
    _phantom: std::marker::PhantomData<&'a mut ffi::dnv_vista_sdk_tsd_dto_event_set_t>,
}

impl<'a> TsdDtoEventSetRef<'a> {
    /// Timestamp of the event (ISO 8601 string).
    pub fn timestamp(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            cstr(ffi::dnv_vista_sdk_tsd_dto_event_set_get_timestamp(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the timestamp.
    pub fn set_timestamp(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in timestamp");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe {
            ffi::dnv_vista_sdk_tsd_dto_event_set_set_timestamp(self.ptr.as_ptr(), s.as_ptr())
        };
    }

    /// Data channel identifier.
    pub fn data_channel_id(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            cstr(ffi::dnv_vista_sdk_tsd_dto_event_set_get_data_channel_id(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the data channel identifier.
    pub fn set_data_channel_id(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in data_channel_id");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe {
            ffi::dnv_vista_sdk_tsd_dto_event_set_set_data_channel_id(self.ptr.as_ptr(), s.as_ptr())
        };
    }

    /// Value of the event.
    pub fn value(&self) -> String {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            cstr(ffi::dnv_vista_sdk_tsd_dto_event_set_get_value(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the value of the event.
    pub fn set_value(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in value");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_event_set_set_value(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Whether a quality code is set.
    pub fn has_quality(&self) -> bool {
        // SAFETY: ptr is valid.
        (unsafe { ffi::dnv_vista_sdk_tsd_dto_event_set_has_quality(self.ptr.as_ptr()) }) != 0
    }

    /// Quality code of the event, if set.
    pub fn quality(&self) -> Option<String> {
        // SAFETY: ptr is valid; returned pointer is borrowed from it.
        unsafe {
            opt_cstr(ffi::dnv_vista_sdk_tsd_dto_event_set_get_quality(
                self.ptr.as_ptr(),
            ))
        }
    }

    /// Sets the quality code.
    pub fn set_quality(&mut self, v: &str) {
        let s = CString::new(v).expect("NUL in quality");
        // SAFETY: ptr is valid; s is valid for the call.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_event_set_set_quality(self.ptr.as_ptr(), s.as_ptr()) };
    }

    /// Clears the quality code.
    pub fn clear_quality(&mut self) {
        // SAFETY: ptr is valid.
        unsafe { ffi::dnv_vista_sdk_tsd_dto_event_set_clear_quality(self.ptr.as_ptr()) };
    }
}

// ============================================================
// Free functions: to_dto / to_domain / from_json / to_json
// ============================================================

/// Converts a domain `TimeSeriesDataPackage` to an owned `TsdDtoPackage`.
pub fn to_dto(domain: &TimeSeriesDataPackage) -> Result<TsdDtoPackage, VistaError> {
    // SAFETY: domain is valid for the call's duration. Returns an owned pointer or NULL.
    let ptr = unsafe { ffi::dnv_vista_sdk_tsd_to_dto(domain.as_ptr()) };
    if ptr.is_null() {
        Err(last_error())
    } else {
        // SAFETY: ptr is non-null and owned.
        Ok(unsafe { TsdDtoPackage::from_raw(ptr) })
    }
}

/// Converts an owned `TsdDtoPackage` back to a domain `TimeSeriesDataPackage`.
pub fn to_domain(dto: &TsdDtoPackage) -> Result<TimeSeriesDataPackage, VistaError> {
    // SAFETY: dto is valid for the call's duration. Returns an owned pointer or NULL.
    let ptr = unsafe { ffi::dnv_vista_sdk_tsd_to_domain(dto.as_ptr()) };
    if ptr.is_null() {
        Err(last_error())
    } else {
        // SAFETY: ptr is non-null and owned.
        Ok(unsafe { TimeSeriesDataPackage::from_raw(ptr) })
    }
}

fn c_json(json: &str) -> Result<CString, VistaError> {
    CString::new(json).map_err(|_| VistaError {
        kind: crate::core::error::ErrorKind::InvalidArgument,
        message: "JSON string contains a NUL byte".to_string(),
    })
}

/// Parses a `TsdDtoPackage` from a JSON string.
pub fn from_json(json: &str) -> Result<TsdDtoPackage, VistaError> {
    let c = c_json(json)?;
    // SAFETY: c is valid for the call's duration. Returns an owned pointer or NULL.
    let ptr = unsafe { ffi::dnv_vista_sdk_tsd_dto_from_json(c.as_ptr()) };
    if ptr.is_null() {
        Err(last_error())
    } else {
        // SAFETY: ptr is non-null and owned.
        Ok(unsafe { TsdDtoPackage::from_raw(ptr) })
    }
}

/// Serializes a `TsdDtoPackage` to a JSON string.
pub fn to_json(dto: &TsdDtoPackage, pretty_print: bool) -> String {
    // SAFETY: dto is valid for the call's duration. Returns an owned pointer or NULL.
    let raw = unsafe {
        ffi::dnv_vista_sdk_tsd_dto_to_json(dto.as_ptr(), pretty_print as std::ffi::c_int)
    };
    if raw.is_null() {
        return String::new();
    }
    // SAFETY: raw is non-null and owned until freed below.
    let s = unsafe { CStr::from_ptr(raw) }
        .to_string_lossy()
        .into_owned();
    // SAFETY: raw was allocated by the library and is freed exactly once.
    unsafe { dnv_vista_sdk_string_free(raw) };
    s
}
