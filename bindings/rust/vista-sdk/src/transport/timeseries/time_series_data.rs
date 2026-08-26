use std::ffi::{CStr, CString};
use std::ptr::NonNull;

use crate::core::error::{last_error, VistaError};
use crate::ffi::transport::timeseries::data_channel_id::dnv_vista_sdk_tsd_channel_id_t;
use crate::ffi::transport::timeseries::time_series_data as ffi;
use crate::transport::datachannel::data_channel::{DataChannelListPackage, DataChannelRef};
use crate::transport::iso19848::{read_value, Value};
use crate::transport::serializable_document::{SerializableDocument, SerializableDocumentRef};
use crate::transport::ship_id::ShipId;
use crate::transport::timeseries::data_channel_id::TsdChannelId;
use crate::types::date_time_offset::DateTimeOffset;

/// Interval between two timestamps (Table 25).
pub struct TsdTimeSpan(NonNull<ffi::dnv_vista_sdk_tsd_time_span_t>);

impl TsdTimeSpan {
    /// Constructs a `TsdTimeSpan`. Returns an error if `start` is after `end`.
    pub fn new(start: DateTimeOffset, end: DateTimeOffset) -> Result<Self, VistaError> {
        // SAFETY: no pointer arguments. Returns an owned pointer or NULL.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_tsd_time_span_create(start.into_ffi(), end.into_ffi()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(Self(NonNull::new(ptr).unwrap()))
        }
    }

    /// Start timestamp.
    pub fn start(&self) -> DateTimeOffset {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        DateTimeOffset::from_ffi(unsafe { ffi::dnv_vista_sdk_tsd_time_span_start(self.0.as_ptr()) })
    }

    /// End timestamp.
    pub fn end(&self) -> DateTimeOffset {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        DateTimeOffset::from_ffi(unsafe { ffi::dnv_vista_sdk_tsd_time_span_end(self.0.as_ptr()) })
    }

    /// Sets the start timestamp. Fails (sets last error) if `start` is after the current end.
    pub fn set_start(&mut self, start: DateTimeOffset) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_tsd_time_span_set_start(self.0.as_ptr(), start.into_ffi()) }
    }

    /// Sets the end timestamp. Fails (sets last error) if `end` is before the current start.
    pub fn set_end(&mut self, end: DateTimeOffset) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_tsd_time_span_set_end(self.0.as_ptr(), end.into_ffi()) }
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_tsd_time_span_t {
        self.0.as_ptr()
    }
}

impl Drop for TsdTimeSpan {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `TsdTimeSpan` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_tsd_time_span_free(self.0.as_ptr()) }
    }
}

/// Configuration reference for time series (Table 26).
pub struct TsdConfigRef(NonNull<ffi::dnv_vista_sdk_tsd_config_ref_t>);

impl TsdConfigRef {
    /// Constructs a `TsdConfigRef`.
    pub fn new(id: &str, time_stamp: DateTimeOffset) -> Self {
        let c = CString::new(id).expect("id contains NUL byte");
        // SAFETY: c is a valid NUL-terminated C string. Returns an owned pointer.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_tsd_config_ref_create(c.as_ptr(), time_stamp.into_ffi()) };
        Self(NonNull::new(ptr).expect("tsd_config_ref_create returned null"))
    }

    /// Returns the configuration identifier.
    pub fn id(&self) -> String {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_config_ref_id(self.0.as_ptr()) };
        // SAFETY: ptr is non-null and owned by `self` for its lifetime.
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8")
            .to_string()
    }

    /// Sets the configuration identifier.
    pub fn set_id(&mut self, id: &str) {
        let c = CString::new(id).expect("id contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe { ffi::dnv_vista_sdk_tsd_config_ref_set_id(self.0.as_ptr(), c.as_ptr()) }
    }

    /// Returns the timestamp.
    pub fn time_stamp(&self) -> DateTimeOffset {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        DateTimeOffset::from_ffi(unsafe {
            ffi::dnv_vista_sdk_tsd_config_ref_timestamp(self.0.as_ptr())
        })
    }

    /// Sets the timestamp.
    pub fn set_time_stamp(&mut self, time_stamp: DateTimeOffset) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe {
            ffi::dnv_vista_sdk_tsd_config_ref_set_timestamp(self.0.as_ptr(), time_stamp.into_ffi())
        }
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_tsd_config_ref_t {
        self.0.as_ptr()
    }
}

impl Drop for TsdConfigRef {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `TsdConfigRef` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_tsd_config_ref_free(self.0.as_ptr()) }
    }
}

/// Borrowed view of a [`TsdTimeSpan`] obtained via [`TsdHeader::time_span`].
pub struct TsdTimeSpanRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_tsd_time_span_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_tsd_time_span_t>,
}

impl<'a> TsdTimeSpanRef<'a> {
    /// Start timestamp.
    pub fn start(&self) -> DateTimeOffset {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        DateTimeOffset::from_ffi(unsafe { ffi::dnv_vista_sdk_tsd_time_span_start(self.ptr) })
    }

    /// End timestamp.
    pub fn end(&self) -> DateTimeOffset {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        DateTimeOffset::from_ffi(unsafe { ffi::dnv_vista_sdk_tsd_time_span_end(self.ptr) })
    }
}

/// Borrowed view of a [`TsdConfigRef`] obtained via [`TsdHeader::system_configuration_at`].
pub struct TsdConfigRefRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_tsd_config_ref_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_tsd_config_ref_t>,
}

impl<'a> TsdConfigRefRef<'a> {
    /// Returns the configuration identifier.
    pub fn id(&self) -> String {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_config_ref_id(self.ptr) };
        // SAFETY: ptr is non-null and owned by the referenced config ref for lifetime `'a`.
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8")
            .to_string()
    }

    /// Returns the timestamp.
    pub fn time_stamp(&self) -> DateTimeOffset {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        DateTimeOffset::from_ffi(unsafe { ffi::dnv_vista_sdk_tsd_config_ref_timestamp(self.ptr) })
    }
}

/// Header for a time series package (Table 24).
pub struct TsdHeader(NonNull<ffi::dnv_vista_sdk_tsd_header_t>);

impl TsdHeader {
    /// Constructs a `TsdHeader` from a `ShipId`.
    pub fn new(ship_id: &ShipId) -> Self {
        // SAFETY: sp is non-null and valid for the call's duration. Returns a non-null owned pointer.
        let ptr = ship_id.with_ffi_ptr(|sp| unsafe { ffi::dnv_vista_sdk_tsd_header_create(sp) });
        Self(NonNull::new(ptr).expect("tsd_header_create returned null"))
    }

    /// Returns the ship ID.
    pub fn ship_id(&self) -> ShipId {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_header_ship_id(self.0.as_ptr()) };
        ShipId::from_ffi_ptr(ptr)
    }

    /// Sets the ship ID.
    pub fn set_ship_id(&mut self, ship_id: &ShipId) {
        // SAFETY: self.0 and sp are non-null and valid for the call's duration.
        ship_id.with_ffi_ptr(|sp| unsafe {
            ffi::dnv_vista_sdk_tsd_header_set_ship_id(self.0.as_ptr(), sp)
        })
    }

    /// Returns the time span if set.
    pub fn time_span(&self) -> Option<TsdTimeSpanRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_header_time_span(self.0.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(TsdTimeSpanRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Sets the time span.
    pub fn set_time_span(&mut self, time_span: &TsdTimeSpan) {
        // SAFETY: self.0 and time_span are non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_tsd_header_set_time_span(self.0.as_ptr(), time_span.as_ptr()) }
    }

    /// Clears the time span.
    pub fn clear_time_span(&mut self) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_tsd_header_clear_time_span(self.0.as_ptr()) }
    }

    /// Returns the date created, if set.
    pub fn date_created(&self) -> Option<DateTimeOffset> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        if (unsafe { ffi::dnv_vista_sdk_tsd_header_has_date_created(self.0.as_ptr()) }) != 0 {
            Some(DateTimeOffset::from_ffi(unsafe {
                // SAFETY: self.0 is non-null and known (checked above) to have a date created.
                ffi::dnv_vista_sdk_tsd_header_date_created(self.0.as_ptr())
            }))
        } else {
            None
        }
    }

    /// Sets the date created.
    pub fn set_date_created(&mut self, date_created: DateTimeOffset) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe {
            ffi::dnv_vista_sdk_tsd_header_set_date_created(self.0.as_ptr(), date_created.into_ffi())
        }
    }

    /// Clears the date created.
    pub fn clear_date_created(&mut self) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_tsd_header_clear_date_created(self.0.as_ptr()) }
    }

    /// Returns the date modified, if set.
    pub fn date_modified(&self) -> Option<DateTimeOffset> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        if (unsafe { ffi::dnv_vista_sdk_tsd_header_has_date_modified(self.0.as_ptr()) }) != 0 {
            Some(DateTimeOffset::from_ffi(unsafe {
                // SAFETY: self.0 is non-null and known (checked above) to have a date modified.
                ffi::dnv_vista_sdk_tsd_header_date_modified(self.0.as_ptr())
            }))
        } else {
            None
        }
    }

    /// Sets the date modified.
    pub fn set_date_modified(&mut self, date_modified: DateTimeOffset) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe {
            ffi::dnv_vista_sdk_tsd_header_set_date_modified(
                self.0.as_ptr(),
                date_modified.into_ffi(),
            )
        }
    }

    /// Clears the date modified.
    pub fn clear_date_modified(&mut self) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_tsd_header_clear_date_modified(self.0.as_ptr()) }
    }

    /// Returns the author, if set.
    pub fn author(&self) -> Option<String> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_header_author(self.0.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(
                // SAFETY: ptr is non-null (checked above) and owned by `self` for its lifetime.
                unsafe { CStr::from_ptr(ptr) }
                    .to_str()
                    .expect("invalid UTF-8")
                    .to_string(),
            )
        }
    }

    /// Sets the author.
    pub fn set_author(&mut self, author: &str) {
        let c = CString::new(author).expect("author contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe { ffi::dnv_vista_sdk_tsd_header_set_author(self.0.as_ptr(), c.as_ptr()) }
    }

    /// Clears the author.
    pub fn clear_author(&mut self) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_tsd_header_clear_author(self.0.as_ptr()) }
    }

    /// Returns the number of system configuration entries.
    pub fn system_configuration_count(&self) -> usize {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_tsd_header_system_configuration_count(self.0.as_ptr()) }
    }

    /// Returns the configuration reference at `index`, or `None` if out of bounds.
    pub fn system_configuration_at(&self, index: usize) -> Option<TsdConfigRefRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_tsd_header_system_configuration_at(self.0.as_ptr(), index)
        };
        if ptr.is_null() {
            None
        } else {
            Some(TsdConfigRefRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Returns a borrowed view of the custom headers if set.
    pub fn custom_headers(&self) -> Option<SerializableDocumentRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_header_custom_headers(self.0.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(SerializableDocumentRef::from_ptr(ptr))
        }
    }

    /// Sets the system configuration list.
    pub fn set_system_configuration(&mut self, entries: &[&TsdConfigRef]) {
        let ptrs: Vec<*const ffi::dnv_vista_sdk_tsd_config_ref_t> =
            entries.iter().map(|e| e.as_ptr()).collect();
        // SAFETY: self.0 is non-null, and ptrs.as_ptr() is valid for ptrs.len() elements.
        unsafe {
            ffi::dnv_vista_sdk_tsd_header_set_system_configuration(
                self.0.as_ptr(),
                ptrs.as_ptr(),
                ptrs.len(),
            )
        }
    }

    /// Clears the system configuration list.
    pub fn clear_system_configuration(&mut self) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_tsd_header_clear_system_configuration(self.0.as_ptr()) }
    }

    /// Sets custom headers, consuming the document.
    pub fn set_custom_headers(&mut self, doc: SerializableDocument) {
        // SAFETY: self.0 is non-null, and doc.into_raw() transfers ownership of a non-null pointer.
        unsafe { ffi::dnv_vista_sdk_tsd_header_set_custom_headers(self.0.as_ptr(), doc.into_raw()) }
    }

    /// Clears custom headers.
    pub fn clear_custom_headers(&mut self) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_tsd_header_clear_custom_headers(self.0.as_ptr()) }
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_tsd_header_t {
        self.0.as_ptr()
    }
}

impl Drop for TsdHeader {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `TsdHeader` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_tsd_header_free(self.0.as_ptr()) }
    }
}

/// One row of tabular data (Table 30).
pub struct TabularDataSet(NonNull<ffi::dnv_vista_sdk_tsd_tabular_data_set_t>);

impl TabularDataSet {
    /// Constructs a `TabularDataSet` with optional quality indicators.
    pub fn new(time_stamp: DateTimeOffset, values: &[&str], quality: Option<&[&str]>) -> Self {
        let c_values: Vec<CString> = values
            .iter()
            .map(|v| CString::new(*v).expect("value contains NUL byte"))
            .collect();
        let ptrs: Vec<*const std::ffi::c_char> = c_values.iter().map(|s| s.as_ptr()).collect();
        // SAFETY: ptrs.as_ptr() is valid for ptrs.len() elements, each a valid NUL-terminated
        // C string owned by c_values. Returns a non-null owned pointer.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_tsd_tabular_data_set_create(
                time_stamp.into_ffi(),
                ptrs.as_ptr(),
                ptrs.len(),
            )
        };
        let result = Self(NonNull::new(ptr).expect("tabular_data_set_create returned null"));
        if let Some(q) = quality {
            let c_quality: Vec<CString> = q
                .iter()
                .map(|v| CString::new(*v).expect("quality contains NUL byte"))
                .collect();
            let qptrs: Vec<*const std::ffi::c_char> =
                c_quality.iter().map(|s| s.as_ptr()).collect();
            // SAFETY: result.0 is non-null, and qptrs.as_ptr() is valid for qptrs.len() elements.
            unsafe {
                ffi::dnv_vista_sdk_tsd_tabular_data_set_set_quality(
                    result.0.as_ptr(),
                    qptrs.as_ptr(),
                    qptrs.len(),
                )
            }
        }
        result
    }

    /// Returns the timestamp.
    pub fn time_stamp(&self) -> DateTimeOffset {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        DateTimeOffset::from_ffi(unsafe {
            ffi::dnv_vista_sdk_tsd_tabular_data_set_time_stamp(self.0.as_ptr())
        })
    }

    /// Returns all values.
    pub fn values(&self) -> Vec<String> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let n = unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_set_value_count(self.0.as_ptr()) };
        (0..n)
            .map(|i| {
                // SAFETY: self.0 is non-null, and i is in [0, n).
                let ptr =
                    unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_set_value_at(self.0.as_ptr(), i) };
                // SAFETY: ptr is non-null and owned by `self` for its lifetime.
                unsafe { CStr::from_ptr(ptr) }
                    .to_str()
                    .expect("invalid UTF-8")
                    .to_string()
            })
            .collect()
    }

    /// Returns quality indicators if set.
    pub fn quality(&self) -> Option<Vec<String>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let n = unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_set_quality_count(self.0.as_ptr()) };
        if n == 0 {
            None
        } else {
            Some(
                (0..n)
                    .map(|i| {
                        // SAFETY: self.0 is non-null, and i is in [0, n).
                        let ptr = unsafe {
                            ffi::dnv_vista_sdk_tsd_tabular_data_set_quality_at(self.0.as_ptr(), i)
                        };
                        // SAFETY: ptr is non-null and owned by `self` for its lifetime.
                        unsafe { CStr::from_ptr(ptr) }
                            .to_str()
                            .expect("invalid UTF-8")
                            .to_string()
                    })
                    .collect(),
            )
        }
    }

    /// Replaces all values.
    pub fn set_values(&mut self, values: &[&str]) {
        let c_values: Vec<std::ffi::CString> = values
            .iter()
            .map(|v| std::ffi::CString::new(*v).expect("value contains NUL byte"))
            .collect();
        let ptrs: Vec<*const std::ffi::c_char> = c_values.iter().map(|s| s.as_ptr()).collect();
        // SAFETY: self.0 is non-null, and ptrs.as_ptr() is valid for ptrs.len() elements.
        unsafe {
            ffi::dnv_vista_sdk_tsd_tabular_data_set_set_values(
                self.0.as_ptr(),
                ptrs.as_ptr(),
                ptrs.len(),
            )
        }
    }

    /// Clears quality indicators.
    pub fn clear_quality(&mut self) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_set_clear_quality(self.0.as_ptr()) }
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_tsd_tabular_data_set_t {
        self.0.as_ptr()
    }
}

impl Drop for TabularDataSet {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `TabularDataSet` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_set_free(self.0.as_ptr()) }
    }
}

/// Borrowed view of a [`TabularDataSet`] obtained via [`TabularData::data_set_at`].
pub struct TabularDataSetRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_tsd_tabular_data_set_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_tsd_tabular_data_set_t>,
}

impl<'a> TabularDataSetRef<'a> {
    /// Returns the timestamp.
    pub fn time_stamp(&self) -> DateTimeOffset {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        DateTimeOffset::from_ffi(unsafe {
            ffi::dnv_vista_sdk_tsd_tabular_data_set_time_stamp(self.ptr)
        })
    }

    /// Returns all values.
    pub fn values(&self) -> Vec<String> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let n = unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_set_value_count(self.ptr) };
        (0..n)
            .map(|i| {
                // SAFETY: self.ptr is non-null, and i is in [0, n).
                let ptr = unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_set_value_at(self.ptr, i) };
                // SAFETY: ptr is non-null and owned by the referenced data set for lifetime `'a`.
                unsafe { CStr::from_ptr(ptr) }
                    .to_str()
                    .expect("invalid UTF-8")
                    .to_string()
            })
            .collect()
    }

    /// Returns quality indicators if set.
    pub fn quality(&self) -> Option<Vec<String>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let n = unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_set_quality_count(self.ptr) };
        if n == 0 {
            None
        } else {
            Some(
                (0..n)
                    .map(|i| {
                        // SAFETY: self.ptr is non-null, and i is in [0, n).
                        let ptr = unsafe {
                            ffi::dnv_vista_sdk_tsd_tabular_data_set_quality_at(self.ptr, i)
                        };
                        // SAFETY: ptr is non-null and owned by the referenced data set for lifetime `'a`.
                        unsafe { CStr::from_ptr(ptr) }
                            .to_str()
                            .expect("invalid UTF-8")
                            .to_string()
                    })
                    .collect(),
            )
        }
    }
}

/// Borrowed view of an [`EventDataSet`] obtained via [`EventData::data_set_at`].
pub struct EventDataSetRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_tsd_event_data_set_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_tsd_event_data_set_t>,
}

impl<'a> EventDataSetRef<'a> {
    /// Returns the timestamp.
    pub fn time_stamp(&self) -> DateTimeOffset {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        DateTimeOffset::from_ffi(unsafe {
            ffi::dnv_vista_sdk_tsd_event_data_set_time_stamp(self.ptr)
        })
    }

    /// Returns the channel ID.
    pub fn channel_id(&self) -> &TsdChannelId {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_event_data_set_data_channel_id(self.ptr) };
        TsdChannelId::from_ptr(ptr)
    }

    /// Returns the value.
    pub fn value(&self) -> String {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_event_data_set_value(self.ptr) };
        // SAFETY: ptr is non-null and owned by the referenced data set for lifetime `'a`.
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8")
            .to_string()
    }

    /// Returns the quality indicator, if set.
    pub fn quality(&self) -> Option<String> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_event_data_set_quality(self.ptr) };
        if ptr.is_null() {
            None
        } else {
            Some(
                // SAFETY: ptr is non-null (checked above) and owned by the referenced data set for lifetime `'a`.
                unsafe { CStr::from_ptr(ptr) }
                    .to_str()
                    .expect("invalid UTF-8")
                    .to_string(),
            )
        }
    }
}

/// Borrowed view of a [`TabularData`] obtained via [`TimeSeriesData::tabular_data_at`].
pub struct TabularDataRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_tsd_tabular_data_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_tsd_tabular_data_t>,
}

impl<'a> TabularDataRef<'a> {
    /// Returns the number of data channel IDs.
    pub fn channel_id_count(&self) -> usize {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_channel_id_count(self.ptr) }
    }

    /// Returns the channel ID at `index`, or `None` if out of bounds.
    pub fn channel_id_at(&self, index: usize) -> Option<&TsdChannelId> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_channel_id_at(self.ptr, index) };
        if ptr.is_null() {
            None
        } else {
            Some(TsdChannelId::from_ptr(ptr))
        }
    }

    /// Returns the number of data sets.
    pub fn data_set_count(&self) -> usize {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_data_set_count(self.ptr) }
    }

    /// Returns a borrowed view of the data set at `index`, or `None` if out of bounds.
    pub fn data_set_at(&self, index: usize) -> Option<TabularDataSetRef<'_>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_data_set_at(self.ptr, index) };
        if ptr.is_null() {
            None
        } else {
            Some(TabularDataSetRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }
}

/// Borrowed view of an [`EventData`] obtained via [`TimeSeriesData::event_data`].
pub struct EventDataRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_tsd_event_data_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_tsd_event_data_t>,
}

impl<'a> EventDataRef<'a> {
    /// Returns the number of data sets.
    pub fn data_set_count(&self) -> usize {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { ffi::dnv_vista_sdk_tsd_event_data_data_set_count(self.ptr) }
    }

    /// Returns a borrowed view of the data set at `index`, or `None` if out of bounds.
    pub fn data_set_at(&self, index: usize) -> Option<EventDataSetRef<'_>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_event_data_data_set_at(self.ptr, index) };
        if ptr.is_null() {
            None
        } else {
            Some(EventDataSetRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }
}

/// Borrowed view of a [`TimeSeriesData`] obtained via [`TsdPackage::time_series_data_at`].
pub struct TimeSeriesDataRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_tsd_time_series_data_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_tsd_time_series_data_t>,
}

impl<'a> TimeSeriesDataRef<'a> {
    /// Returns the data configuration reference if set.
    pub fn data_configuration(&self) -> Option<TsdConfigRefRef<'_>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_time_series_data_data_configuration(self.ptr) };
        if ptr.is_null() {
            None
        } else {
            Some(TsdConfigRefRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Returns the number of tabular data entries.
    pub fn tabular_data_count(&self) -> usize {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { ffi::dnv_vista_sdk_tsd_time_series_data_tabular_data_count(self.ptr) }
    }

    /// Returns a borrowed view of the tabular data at `index`, or `None` if out of bounds.
    pub fn tabular_data_at(&self, index: usize) -> Option<TabularDataRef<'_>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_tsd_time_series_data_tabular_data_at(self.ptr, index) };
        if ptr.is_null() {
            None
        } else {
            Some(TabularDataRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Returns a borrowed view of the event data if set.
    pub fn event_data(&self) -> Option<EventDataRef<'_>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_time_series_data_event_data(self.ptr) };
        if ptr.is_null() {
            None
        } else {
            Some(EventDataRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }
}

/// Column-oriented tabular data (Table 28).
pub struct TabularData(NonNull<ffi::dnv_vista_sdk_tsd_tabular_data_t>);

impl TabularData {
    /// Constructs a `TabularData` from channel IDs and data sets.
    pub fn new(channel_ids: &[&TsdChannelId], data_sets: &[&TabularDataSet]) -> Self {
        let id_ptrs: Vec<*const dnv_vista_sdk_tsd_channel_id_t> =
            channel_ids.iter().map(|id| id.as_ptr()).collect();
        let ds_ptrs: Vec<*const ffi::dnv_vista_sdk_tsd_tabular_data_set_t> =
            data_sets.iter().map(|ds| ds.as_ptr()).collect();
        // SAFETY: id_ptrs.as_ptr() and ds_ptrs.as_ptr() are each valid for their respective
        // lengths. Returns a non-null owned pointer.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_tsd_tabular_data_create(
                id_ptrs.as_ptr(),
                id_ptrs.len(),
                ds_ptrs.as_ptr(),
                ds_ptrs.len(),
            )
        };
        Self(NonNull::new(ptr).expect("tabular_data_create returned null"))
    }

    /// Returns the number of data channel IDs.
    pub fn channel_id_count(&self) -> usize {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_channel_id_count(self.0.as_ptr()) }
    }

    /// Returns the number of data sets.
    pub fn data_set_count(&self) -> usize {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_data_set_count(self.0.as_ptr()) }
    }

    /// Returns the channel ID at `index`, or `None` if out of bounds.
    pub fn channel_id_at(&self, index: usize) -> Option<&TsdChannelId> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_channel_id_at(self.0.as_ptr(), index) };
        if ptr.is_null() {
            None
        } else {
            Some(TsdChannelId::from_ptr(ptr))
        }
    }

    /// Returns a borrowed view of the data set at `index`, or `None` if out of bounds.
    pub fn data_set_at(&self, index: usize) -> Option<TabularDataSetRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_data_set_at(self.0.as_ptr(), index) };
        if ptr.is_null() {
            None
        } else {
            Some(TabularDataSetRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Validates that channel count matches values-per-row in each data set.
    pub fn validate(&self) -> bool {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        (unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_validate(self.0.as_ptr()) }) != 0
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_tsd_tabular_data_t {
        self.0.as_ptr()
    }
}

impl Drop for TabularData {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `TabularData` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_free(self.0.as_ptr()) }
    }
}

/// One event data point (Table 31).
pub struct EventDataSet(NonNull<ffi::dnv_vista_sdk_tsd_event_data_set_t>);

impl EventDataSet {
    /// Constructs an `EventDataSet` with optional quality indicator.
    pub fn new(
        time_stamp: DateTimeOffset,
        channel_id: &TsdChannelId,
        value: &str,
        quality: Option<&str>,
    ) -> Self {
        let c_value = CString::new(value).expect("value contains NUL byte");
        // SAFETY: channel_id and c_value are non-null/valid for the call's duration. Returns a non-null owned pointer.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_tsd_event_data_set_create(
                time_stamp.into_ffi(),
                channel_id.as_ptr(),
                c_value.as_ptr(),
            )
        };
        let result = Self(NonNull::new(ptr).expect("event_data_set_create returned null"));
        if let Some(q) = quality {
            let c_q = CString::new(q).expect("quality contains NUL byte");
            // SAFETY: result.0 is non-null, and c_q is a valid NUL-terminated C string.
            unsafe {
                ffi::dnv_vista_sdk_tsd_event_data_set_set_quality(result.0.as_ptr(), c_q.as_ptr())
            }
        }
        result
    }

    /// Returns the timestamp.
    pub fn time_stamp(&self) -> DateTimeOffset {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        DateTimeOffset::from_ffi(unsafe {
            ffi::dnv_vista_sdk_tsd_event_data_set_time_stamp(self.0.as_ptr())
        })
    }

    /// Returns the channel ID.
    pub fn channel_id(&self) -> &TsdChannelId {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_event_data_set_data_channel_id(self.0.as_ptr()) };
        TsdChannelId::from_ptr(ptr)
    }

    /// Returns the value.
    pub fn value(&self) -> String {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_event_data_set_value(self.0.as_ptr()) };
        // SAFETY: ptr is non-null and owned by `self` for its lifetime.
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8")
            .to_string()
    }

    /// Returns the quality indicator, if set.
    pub fn quality(&self) -> Option<String> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_event_data_set_quality(self.0.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(
                // SAFETY: ptr is non-null (checked above) and owned by `self` for its lifetime.
                unsafe { CStr::from_ptr(ptr) }
                    .to_str()
                    .expect("invalid UTF-8")
                    .to_string(),
            )
        }
    }

    /// Clears the quality indicator.
    pub fn clear_quality(&mut self) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_tsd_event_data_set_clear_quality(self.0.as_ptr()) }
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_tsd_event_data_set_t {
        self.0.as_ptr()
    }
}

impl Drop for EventDataSet {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `EventDataSet` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_tsd_event_data_set_free(self.0.as_ptr()) }
    }
}

/// Collection of event data sets (Table 29).
pub struct EventData(NonNull<ffi::dnv_vista_sdk_tsd_event_data_t>);

impl EventData {
    /// Constructs an `EventData` with an optional list of data sets.
    pub fn new(data_sets: Option<&[&EventDataSet]>) -> Self {
        // SAFETY: no arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_event_data_create() };
        let result = Self(NonNull::new(ptr).expect("event_data_create returned null"));
        if let Some(sets) = data_sets {
            result.set_data_set_inner(sets);
        }
        result
    }

    fn set_data_set_inner(&self, data_sets: &[&EventDataSet]) {
        let ptrs: Vec<*const ffi::dnv_vista_sdk_tsd_event_data_set_t> =
            data_sets.iter().map(|ds| ds.as_ptr()).collect();
        // SAFETY: self.0 is non-null, and ptrs.as_ptr() is valid for ptrs.len() elements.
        unsafe {
            ffi::dnv_vista_sdk_tsd_event_data_set_data_set(
                self.0.as_ptr(),
                ptrs.as_ptr(),
                ptrs.len(),
            )
        }
    }

    /// Returns the number of data sets.
    pub fn data_set_count(&self) -> usize {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_tsd_event_data_data_set_count(self.0.as_ptr()) }
    }

    /// Returns a borrowed view of the data set at `index`, or `None` if out of bounds.
    pub fn data_set_at(&self, index: usize) -> Option<EventDataSetRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_event_data_data_set_at(self.0.as_ptr(), index) };
        if ptr.is_null() {
            None
        } else {
            Some(EventDataSetRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Sets the data set list.
    pub fn set_data_sets(&mut self, data_sets: &[&EventDataSet]) {
        self.set_data_set_inner(data_sets);
    }

    /// Clears the data set list.
    pub fn clear_data_sets(&mut self) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_tsd_event_data_clear_data_set(self.0.as_ptr()) }
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_tsd_event_data_t {
        self.0.as_ptr()
    }
}

impl Drop for EventData {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `EventData` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_tsd_event_data_free(self.0.as_ptr()) }
    }
}

/// Aggregated result of a [`TimeSeriesData::validate`] call.
pub struct ValidationResult {
    valid: bool,
    errors: Vec<String>,
}

impl ValidationResult {
    /// Returns `true` if all data points passed validation.
    pub fn is_valid(&self) -> bool {
        self.valid
    }

    /// Returns all error messages collected across every data point.
    pub fn errors(&self) -> &[String] {
        &self.errors
    }
}

impl std::ops::Not for ValidationResult {
    type Output = bool;
    fn not(self) -> bool {
        !self.valid
    }
}

/// Result returned by a [`ValidateCallback`].
pub enum ValidateResult {
    /// Data point is valid.
    Ok,
    /// Data point is invalid. Each string is one error message.
    Invalid(Vec<String>),
}

/// User-provided callback for per-data-point custom validation.
///
/// Receives the timestamp, the resolved [`DataChannelRef`], the parsed [`Value`], and the
/// optional quality string. Return [`ValidateResult::Ok`] to accept the data point, or
/// [`ValidateResult::Invalid`] with one or more messages to reject it.
pub type ValidateCallback =
    dyn Fn(DateTimeOffset, DataChannelRef<'_>, Value, Option<&str>) -> ValidateResult;

struct CallbackState<'a> {
    callback: &'a ValidateCallback,
    errors: Vec<String>,
}

// SAFETY: called by the C API with `userdata` pointing to a live `CallbackState` borrowed from
// a local in `validate`. All other pointer arguments are valid for the duration of the call.
unsafe extern "C" fn trampoline(
    time_stamp: crate::ffi::types::date_time_offset::dnv_vista_sdk_date_time_offset_t,
    data_channel: *const crate::ffi::transport::datachannel::data_channel::dnv_vista_sdk_dcl_data_channel_t,
    value: *const crate::ffi::transport::iso19848::dnv_vista_sdk_iso19848_value_t,
    quality: *const std::ffi::c_char,
    out_errors: *mut ffi::dnv_vista_sdk_tsd_validation_errors_t,
    userdata: *mut std::ffi::c_void,
) -> std::ffi::c_int {
    let state = &mut *(userdata as *mut CallbackState<'_>);
    let ts = DateTimeOffset::from_ffi(time_stamp);
    // SAFETY: data_channel and value are non-null and valid for the call's duration per C API contract.
    let dc = DataChannelRef::from_ptr(data_channel);
    let val = read_value(value);
    let qual = if quality.is_null() {
        None
    } else {
        // SAFETY: quality is non-null and NUL-terminated for the call's duration.
        Some(CStr::from_ptr(quality).to_str().unwrap_or(""))
    };
    match (state.callback)(ts, dc, val, qual) {
        ValidateResult::Ok => 1,
        ValidateResult::Invalid(msgs) => {
            for msg in &msgs {
                if !out_errors.is_null() {
                    let c_msg = CString::new(msg.as_str()).unwrap_or_default();
                    // SAFETY: out_errors is non-null (checked above) and valid for the call's duration.
                    ffi::dnv_vista_sdk_tsd_validation_errors_add(out_errors, c_msg.as_ptr());
                }
            }
            state.errors.extend(msgs);
            0
        }
    }
}

/// One time series data block (Table 27).
pub struct TimeSeriesData(NonNull<ffi::dnv_vista_sdk_tsd_time_series_data_t>);

impl TimeSeriesData {
    /// Constructs an empty `TimeSeriesData`.
    pub fn new() -> Self {
        // SAFETY: no arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_time_series_data_create() };
        Self(NonNull::new(ptr).expect("time_series_data_create returned null"))
    }

    /// Returns the data configuration reference if set.
    pub fn data_configuration(&self) -> Option<TsdConfigRefRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_tsd_time_series_data_data_configuration(self.0.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(TsdConfigRefRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Sets the data configuration reference.
    pub fn set_data_configuration(&mut self, config_ref: &TsdConfigRef) {
        // SAFETY: self.0 and config_ref are non-null and valid for the call's duration.
        unsafe {
            ffi::dnv_vista_sdk_tsd_time_series_data_set_data_configuration(
                self.0.as_ptr(),
                config_ref.as_ptr(),
            )
        }
    }

    /// Clears the data configuration.
    pub fn clear_data_configuration(&mut self) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_tsd_time_series_data_clear_data_configuration(self.0.as_ptr()) }
    }

    /// Returns the number of tabular data entries.
    pub fn tabular_data_count(&self) -> usize {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_tsd_time_series_data_tabular_data_count(self.0.as_ptr()) }
    }

    /// Returns a borrowed view of the tabular data at `index`, or `None` if out of bounds.
    pub fn tabular_data_at(&self, index: usize) -> Option<TabularDataRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_tsd_time_series_data_tabular_data_at(self.0.as_ptr(), index)
        };
        if ptr.is_null() {
            None
        } else {
            Some(TabularDataRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Sets the tabular data list.
    pub fn set_tabular_data(&mut self, entries: &[&TabularData]) {
        let ptrs: Vec<*const ffi::dnv_vista_sdk_tsd_tabular_data_t> =
            entries.iter().map(|e| e.as_ptr()).collect();
        // SAFETY: self.0 is non-null, and ptrs.as_ptr() is valid for ptrs.len() elements.
        unsafe {
            ffi::dnv_vista_sdk_tsd_time_series_data_set_tabular_data(
                self.0.as_ptr(),
                ptrs.as_ptr(),
                ptrs.len(),
            )
        }
    }

    /// Clears the tabular data list.
    pub fn clear_tabular_data(&mut self) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_tsd_time_series_data_clear_tabular_data(self.0.as_ptr()) }
    }

    /// Returns a borrowed view of the event data if set.
    pub fn event_data(&self) -> Option<EventDataRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_time_series_data_event_data(self.0.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(EventDataRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Sets the event data.
    pub fn set_event_data(&mut self, event_data: &EventData) {
        // SAFETY: self.0 and event_data are non-null and valid for the call's duration.
        unsafe {
            ffi::dnv_vista_sdk_tsd_time_series_data_set_event_data(
                self.0.as_ptr(),
                event_data.as_ptr(),
            )
        }
    }

    /// Clears the event data.
    pub fn clear_event_data(&mut self) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_tsd_time_series_data_clear_event_data(self.0.as_ptr()) }
    }

    /// Clears custom data kinds.
    pub fn clear_custom_data_kinds(&mut self) {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe { ffi::dnv_vista_sdk_tsd_time_series_data_clear_custom_data_kinds(self.0.as_ptr()) }
    }

    /// Sets custom data kinds, consuming the document.
    pub fn set_custom_data_kinds(&mut self, doc: SerializableDocument) {
        // SAFETY: self.0 is non-null, and doc.into_raw() transfers ownership of a non-null pointer.
        unsafe {
            ffi::dnv_vista_sdk_tsd_time_series_data_set_custom_data_kinds(
                self.0.as_ptr(),
                doc.into_raw(),
            )
        }
    }

    /// Validates this data against a `DataChannelListPackage`.
    ///
    /// `on_tabular` and `on_event` are called once per data point with its timestamp, the
    /// resolved [`DataChannelRef`], the parsed [`Value`], and the optional quality string.
    /// Return [`ValidateResult::Ok`] to accept the point, or [`ValidateResult::Invalid`]
    /// with one or more messages to reject it.
    pub fn validate(
        &self,
        dc_package: &DataChannelListPackage,
        on_tabular: &ValidateCallback,
        on_event: &ValidateCallback,
    ) -> ValidationResult {
        let mut tabular_state = CallbackState {
            callback: on_tabular,
            errors: Vec::new(),
        };
        let mut event_state = CallbackState {
            callback: on_event,
            errors: Vec::new(),
        };

        // SAFETY: self.0 and dc_package are non-null, `trampoline` matches the expected
        // signature, and each userdata pointer outlives the call (borrowed from a local).
        let valid = (unsafe {
            ffi::dnv_vista_sdk_tsd_time_series_data_validate(
                self.0.as_ptr(),
                dc_package.as_ptr(),
                trampoline,
                &mut tabular_state as *mut _ as *mut std::ffi::c_void,
                trampoline,
                &mut event_state as *mut _ as *mut std::ffi::c_void,
            )
        }) != 0;

        let mut errors: Vec<String> = tabular_state.errors;
        errors.extend(event_state.errors);

        if !valid && errors.is_empty() {
            errors.push(last_error().message);
        }

        ValidationResult { valid, errors }
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_tsd_time_series_data_t {
        self.0.as_ptr()
    }
}

impl Default for TimeSeriesData {
    fn default() -> Self {
        Self::new()
    }
}

impl Drop for TimeSeriesData {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `TimeSeriesData` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_tsd_time_series_data_free(self.0.as_ptr()) }
    }
}

/// Package wrapping a header and a list of `TimeSeriesData` (Table 23).
pub struct TsdPackage(NonNull<ffi::dnv_vista_sdk_tsd_package_t>);

impl TsdPackage {
    /// Constructs a `TsdPackage`.
    pub fn new(header: Option<&TsdHeader>, time_series_data: &[&TimeSeriesData]) -> Self {
        let header_ptr = header.map(|h| h.as_ptr()).unwrap_or(std::ptr::null());
        let ptrs: Vec<*const ffi::dnv_vista_sdk_tsd_time_series_data_t> =
            time_series_data.iter().map(|d| d.as_ptr()).collect();
        // SAFETY: header_ptr is either null or valid for the call's duration, and ptrs.as_ptr()
        // is valid for ptrs.len() elements. Returns a non-null owned pointer.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_tsd_package_create(header_ptr, ptrs.as_ptr(), ptrs.len()) };
        Self(NonNull::new(ptr).expect("tsd_package_create returned null"))
    }

    /// Returns the number of `TimeSeriesData` entries.
    pub fn time_series_data_count(&self) -> usize {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_tsd_package_time_series_data_count(self.0.as_ptr()) }
    }

    /// Returns `true` if there are no `TimeSeriesData` entries.
    pub fn time_series_data_is_empty(&self) -> bool {
        self.time_series_data_count() == 0
    }

    /// Returns a borrowed view of the `TimeSeriesData` at `index`, or `None` if out of bounds.
    pub fn time_series_data_at(&self, index: usize) -> Option<TimeSeriesDataRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_tsd_package_time_series_data_at(self.0.as_ptr(), index) };
        if ptr.is_null() {
            None
        } else {
            Some(TimeSeriesDataRef {
                ptr,
                _marker: std::marker::PhantomData,
            })
        }
    }

    /// Sets the header.
    pub fn set_header(&mut self, header: &TsdHeader) {
        // SAFETY: self.0 and header are non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_tsd_package_set_header(self.0.as_ptr(), header.as_ptr()) }
    }

    /// Returns `true` if a header is set.
    pub fn has_header(&self) -> bool {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        !(unsafe { ffi::dnv_vista_sdk_tsd_package_header(self.0.as_ptr()) }).is_null()
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_tsd_package_t {
        self.0.as_ptr()
    }
}

impl Drop for TsdPackage {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `TsdPackage` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_tsd_package_free(self.0.as_ptr()) }
    }
}

/// Top-level time series data package.
pub struct TimeSeriesDataPackage(NonNull<ffi::dnv_vista_sdk_tsd_data_package_t>);

impl TimeSeriesDataPackage {
    /// Constructs a `TimeSeriesDataPackage` from a `TsdPackage`.
    pub fn new(package: &TsdPackage) -> Self {
        // SAFETY: package is non-null and valid for the call's duration. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_data_package_create(package.as_ptr()) };
        Self(NonNull::new(ptr).expect("tsd_data_package_create returned null"))
    }

    /// Returns the inner `TsdPackage` count (convenience accessor).
    pub fn time_series_data_count(&self) -> usize {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let pkg_ptr = unsafe { ffi::dnv_vista_sdk_tsd_data_package_package(self.0.as_ptr()) };
        if pkg_ptr.is_null() {
            0
        } else {
            // SAFETY: pkg_ptr is non-null (checked above) and owned by `self` for its lifetime.
            unsafe { ffi::dnv_vista_sdk_tsd_package_time_series_data_count(pkg_ptr) }
        }
    }

    /// Returns `true` if the package contains no `TimeSeriesData` entries.
    pub fn is_empty(&self) -> bool {
        self.time_series_data_count() == 0
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_tsd_data_package_t {
        self.0.as_ptr()
    }

    /// Constructs a `TimeSeriesDataPackage` from an owned raw pointer returned by the C API.
    ///
    /// # Safety
    /// `ptr` must be a non-null pointer returned by `dnv_vista_sdk_tsd_data_package_from_json`
    /// or another C API constructor that transfers ownership.
    pub(crate) unsafe fn from_raw(ptr: *mut ffi::dnv_vista_sdk_tsd_data_package_t) -> Self {
        Self(NonNull::new(ptr).expect("from_raw called with null pointer"))
    }
}

impl Drop for TimeSeriesDataPackage {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `TimeSeriesDataPackage` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_tsd_data_package_free(self.0.as_ptr()) }
    }
}
