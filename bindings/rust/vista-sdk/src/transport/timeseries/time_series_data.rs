use std::ffi::{CStr, CString};
use std::ptr::NonNull;

use crate::core::error::{last_error, VistaError};
use crate::ffi::transport::timeseries::data_channel_id::dnv_vista_sdk_tsd_channel_id_t;
use crate::ffi::transport::timeseries::time_series_data as ffi;
use crate::transport::datachannel::data_channel::{DataChannelListPackage, DataChannelRef};
use crate::transport::iso19848::{read_value, Value};
use crate::transport::serializable_document::{SerializableDocument, SerializableDocumentRef};
use crate::transport::ship_id::ShipId;
use crate::transport::timeseries::data_channel_id::{TsdChannelId, TsdChannelIdRef};
use crate::types::date_time_offset::DateTimeOffset;

/// ISO 19848 Table 23 - package wrapping a header and a list of [`TimeSeriesData`].
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

    /// Returns a new instance with the header set.
    #[must_use]
    pub fn with_header(self, header: &TsdHeader) -> Self {
        // SAFETY: self.0 and header are non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_tsd_package_set_header(self.0.as_ptr(), header.as_ptr()) }
        self
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

/// ISO 19848 Table 24 - header for a time series package.
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

    /// Returns a new instance with the ship ID set.
    #[must_use]
    pub fn with_ship_id(self, ship_id: &ShipId) -> Self {
        // SAFETY: self.0 and sp are non-null and valid for the call's duration.
        ship_id.with_ffi_ptr(|sp| unsafe {
            ffi::dnv_vista_sdk_tsd_header_set_ship_id(self.0.as_ptr(), sp)
        });
        self
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

    /// Returns a new instance with the time span set.
    #[must_use]
    pub fn with_time_span(self, time_span: &TsdTimeSpan) -> Self {
        // SAFETY: self.0 and time_span are non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_tsd_header_set_time_span(self.0.as_ptr(), time_span.as_ptr()) }
        self
    }

    /// Returns a new instance with the time span cleared.
    #[must_use]
    pub fn without_time_span(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_tsd_header_clear_time_span(self.0.as_ptr()) };
        self
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

    /// Returns a new instance with the date created set.
    #[must_use]
    pub fn with_date_created(self, date_created: DateTimeOffset) -> Self {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe {
            ffi::dnv_vista_sdk_tsd_header_set_date_created(self.0.as_ptr(), date_created.into_ffi())
        }
        self
    }

    /// Returns a new instance with the date created cleared.
    #[must_use]
    pub fn without_date_created(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_tsd_header_clear_date_created(self.0.as_ptr()) };
        self
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

    /// Returns a new instance with the date modified set.
    #[must_use]
    pub fn with_date_modified(self, date_modified: DateTimeOffset) -> Self {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe {
            ffi::dnv_vista_sdk_tsd_header_set_date_modified(
                self.0.as_ptr(),
                date_modified.into_ffi(),
            )
        }
        self
    }

    /// Returns a new instance with the date modified cleared.
    #[must_use]
    pub fn without_date_modified(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_tsd_header_clear_date_modified(self.0.as_ptr()) };
        self
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
                    .to_string_lossy()
                    .into_owned(),
            )
        }
    }

    /// Returns a new instance with the author set.
    #[must_use]
    pub fn with_author(self, author: &str) -> Self {
        let c = CString::new(author).expect("author contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe { ffi::dnv_vista_sdk_tsd_header_set_author(self.0.as_ptr(), c.as_ptr()) }
        self
    }

    /// Returns a new instance with the author cleared.
    #[must_use]
    pub fn without_author(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_tsd_header_clear_author(self.0.as_ptr()) };
        self
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
            // SAFETY: ptr is non-null (checked above) and valid for `self`'s lifetime.
            Some(unsafe { SerializableDocumentRef::from_ptr(ptr) })
        }
    }

    /// Returns a new instance with the system configuration list set.
    #[must_use]
    pub fn with_system_configuration(self, entries: &[&TsdConfigRef]) -> Self {
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
        self
    }

    /// Returns a new instance with the system configuration list cleared.
    #[must_use]
    pub fn without_system_configuration(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_tsd_header_clear_system_configuration(self.0.as_ptr()) };
        self
    }

    /// Returns a new instance with custom headers, consuming the document set.
    #[must_use]
    pub fn with_custom_headers(self, doc: SerializableDocument) -> Self {
        // SAFETY: self.0 is non-null, and doc.into_raw() transfers ownership of a non-null pointer.
        unsafe { ffi::dnv_vista_sdk_tsd_header_set_custom_headers(self.0.as_ptr(), doc.into_raw()) }
        self
    }

    /// Returns a new instance with custom headers cleared.
    #[must_use]
    pub fn without_custom_headers(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_tsd_header_clear_custom_headers(self.0.as_ptr()) };
        self
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

/// ISO 19848 Table 26 - configuration reference for time series.
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
            .to_string_lossy()
            .into_owned()
    }

    /// Returns a new instance with the configuration identifier set.
    #[must_use]
    pub fn with_id(self, id: &str) -> Self {
        let c = CString::new(id).expect("id contains NUL byte");
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        unsafe { ffi::dnv_vista_sdk_tsd_config_ref_set_id(self.0.as_ptr(), c.as_ptr()) }
        self
    }

    /// Returns the timestamp.
    pub fn time_stamp(&self) -> DateTimeOffset {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        DateTimeOffset::from_ffi(unsafe {
            ffi::dnv_vista_sdk_tsd_config_ref_timestamp(self.0.as_ptr())
        })
    }

    /// Returns a new instance with the timestamp set.
    #[must_use]
    pub fn with_time_stamp(self, time_stamp: DateTimeOffset) -> Self {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&mut self`.
        unsafe {
            ffi::dnv_vista_sdk_tsd_config_ref_set_timestamp(self.0.as_ptr(), time_stamp.into_ffi())
        }
        self
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
            .to_string_lossy()
            .into_owned()
    }

    /// Returns the timestamp.
    pub fn time_stamp(&self) -> DateTimeOffset {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        DateTimeOffset::from_ffi(unsafe { ffi::dnv_vista_sdk_tsd_config_ref_timestamp(self.ptr) })
    }
}

/// ISO 19848 Table 25 - interval between two timestamps.
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

    /// Returns a new instance with the start timestamp updated.
    /// Returns `Err` if `start` is after the current end.
    #[must_use = "returns Err if start is after end"]
    pub fn with_start(self, start: DateTimeOffset) -> Result<Self, VistaError> {
        use crate::core::error::ErrorKind;
        if start > self.end() {
            return Err(VistaError {
                kind: ErrorKind::InvalidArgument,
                message: "start must not be after end".to_string(),
            });
        }
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_tsd_time_span_set_start(self.0.as_ptr(), start.into_ffi()) };
        Ok(self)
    }

    /// Returns a new instance with the end timestamp updated.
    /// Returns `Err` if `end` is before the current start.
    #[must_use = "returns Err if end is before start"]
    pub fn with_end(self, end: DateTimeOffset) -> Result<Self, VistaError> {
        use crate::core::error::ErrorKind;
        if end < self.start() {
            return Err(VistaError {
                kind: ErrorKind::InvalidArgument,
                message: "end must not be before start".to_string(),
            });
        }
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_tsd_time_span_set_end(self.0.as_ptr(), end.into_ffi()) };
        Ok(self)
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

/// ISO 19848 Table 27 - one time series data block.
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

    /// Returns a new instance with the data configuration reference set.
    #[must_use]
    pub fn with_data_configuration(self, config_ref: &TsdConfigRef) -> Self {
        // SAFETY: self.0 and config_ref are non-null and valid for the call's duration.
        unsafe {
            ffi::dnv_vista_sdk_tsd_time_series_data_set_data_configuration(
                self.0.as_ptr(),
                config_ref.as_ptr(),
            )
        }
        self
    }

    /// Returns a new instance with the data configuration cleared.
    #[must_use]
    pub fn without_data_configuration(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe {
            ffi::dnv_vista_sdk_tsd_time_series_data_clear_data_configuration(self.0.as_ptr())
        };
        self
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

    /// Returns a new instance with the tabular data list set.
    #[must_use]
    pub fn with_tabular_data(self, entries: &[&TabularData]) -> Self {
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
        self
    }

    /// Returns a new instance with the tabular data list cleared.
    #[must_use]
    pub fn without_tabular_data(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_tsd_time_series_data_clear_tabular_data(self.0.as_ptr()) };
        self
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

    /// Returns a new instance with the event data set.
    #[must_use]
    pub fn with_event_data(self, event_data: &EventData) -> Self {
        // SAFETY: self.0 and event_data are non-null and valid for the call's duration.
        unsafe {
            ffi::dnv_vista_sdk_tsd_time_series_data_set_event_data(
                self.0.as_ptr(),
                event_data.as_ptr(),
            )
        }
        self
    }

    /// Returns a new instance with the event data cleared.
    #[must_use]
    pub fn without_event_data(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_tsd_time_series_data_clear_event_data(self.0.as_ptr()) };
        self
    }

    /// Returns a new instance with custom data kinds cleared.
    #[must_use]
    pub fn without_custom_data_kinds(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_tsd_time_series_data_clear_custom_data_kinds(self.0.as_ptr()) };
        self
    }

    /// Returns a new instance with custom data kinds, consuming the document set.
    #[must_use]
    pub fn with_custom_data_kinds(self, doc: SerializableDocument) -> Self {
        // SAFETY: self.0 is non-null, and doc.into_raw() transfers ownership of a non-null pointer.
        unsafe {
            ffi::dnv_vista_sdk_tsd_time_series_data_set_custom_data_kinds(
                self.0.as_ptr(),
                doc.into_raw(),
            )
        }
        self
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

/// ISO 19848 Table 28 - column-oriented tabular data.
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
    pub fn channel_id_at(&self, index: usize) -> Option<TsdChannelIdRef<'_>> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_channel_id_at(self.0.as_ptr(), index) };
        if ptr.is_null() {
            None
        } else {
            // SAFETY: ptr is non-null (checked above) and valid for `self`'s lifetime.
            Some(unsafe { TsdChannelIdRef::from_ptr(ptr) })
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
    pub fn channel_id_at(&self, index: usize) -> Option<TsdChannelIdRef<'_>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_channel_id_at(self.ptr, index) };
        if ptr.is_null() {
            None
        } else {
            // SAFETY: ptr is non-null (checked above) and valid for `self`'s lifetime.
            Some(unsafe { TsdChannelIdRef::from_ptr(ptr) })
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

/// ISO 19848 Table 29 - collection of event data sets.
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

    /// Returns a new instance with the data set list set.
    #[must_use]
    pub fn with_data_sets(self, data_sets: &[&EventDataSet]) -> Self {
        self.set_data_set_inner(data_sets);
        self
    }

    /// Returns a new instance with the data set list cleared.
    #[must_use]
    pub fn without_data_sets(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_tsd_event_data_clear_data_set(self.0.as_ptr()) };
        self
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

/// ISO 19848 Table 30 - one row of tabular data.
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
                    .to_string_lossy()
                    .into_owned()
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
                            .to_string_lossy()
                            .into_owned()
                    })
                    .collect(),
            )
        }
    }

    /// Returns a new instance with all values replaced.
    #[must_use]
    pub fn with_values(self, values: &[&str]) -> Self {
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
        };
        self
    }

    /// Returns a new instance with quality indicators cleared.
    #[must_use]
    pub fn without_quality(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_tsd_tabular_data_set_clear_quality(self.0.as_ptr()) };
        self
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
                    .to_string_lossy()
                    .into_owned()
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
                            .to_string_lossy()
                            .into_owned()
                    })
                    .collect(),
            )
        }
    }
}

/// ISO 19848 Table 31 - one event data point.
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
    pub fn channel_id(&self) -> TsdChannelIdRef<'_> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_event_data_set_data_channel_id(self.0.as_ptr()) };
        // SAFETY: ptr is non-null (always set for a valid EventDataSet) and valid for `self`'s lifetime.
        unsafe { TsdChannelIdRef::from_ptr(ptr) }
    }

    /// Returns the value.
    pub fn value(&self) -> String {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_event_data_set_value(self.0.as_ptr()) };
        // SAFETY: ptr is non-null and owned by `self` for its lifetime.
        unsafe { CStr::from_ptr(ptr) }
            .to_string_lossy()
            .into_owned()
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
                    .to_string_lossy()
                    .into_owned(),
            )
        }
    }

    /// Returns a new instance with the quality indicator cleared.
    #[must_use]
    pub fn without_quality(self) -> Self {
        // SAFETY: self.0 is non-null and valid for the call's duration.
        unsafe { ffi::dnv_vista_sdk_tsd_event_data_set_clear_quality(self.0.as_ptr()) };
        self
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
    pub fn channel_id(&self) -> TsdChannelIdRef<'_> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_event_data_set_data_channel_id(self.ptr) };
        // SAFETY: ptr is non-null (always set for a valid EventData) and valid for `self`'s lifetime.
        unsafe { TsdChannelIdRef::from_ptr(ptr) }
    }

    /// Returns the value.
    pub fn value(&self) -> String {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let ptr = unsafe { ffi::dnv_vista_sdk_tsd_event_data_set_value(self.ptr) };
        // SAFETY: ptr is non-null and owned by the referenced data set for lifetime `'a`.
        unsafe { CStr::from_ptr(ptr) }
            .to_string_lossy()
            .into_owned()
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
                    .to_string_lossy()
                    .into_owned(),
            )
        }
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

// SAFETY: all owned types below wrap NonNull pointers to heap-allocated C++ objects they own
// exclusively - no interior mutability or shared state, so both Send and Sync are safe.
unsafe impl Send for TsdTimeSpan {}
unsafe impl Sync for TsdTimeSpan {}
unsafe impl Send for TsdConfigRef {}
unsafe impl Sync for TsdConfigRef {}
unsafe impl Send for TsdHeader {}
unsafe impl Sync for TsdHeader {}
unsafe impl Send for TabularDataSet {}
unsafe impl Sync for TabularDataSet {}
unsafe impl Send for TabularData {}
unsafe impl Sync for TabularData {}
unsafe impl Send for EventDataSet {}
unsafe impl Sync for EventDataSet {}
unsafe impl Send for EventData {}
unsafe impl Sync for EventData {}
unsafe impl Send for TimeSeriesData {}
unsafe impl Sync for TimeSeriesData {}
unsafe impl Send for TsdPackage {}
unsafe impl Sync for TsdPackage {}
unsafe impl Send for TimeSeriesDataPackage {}
unsafe impl Sync for TimeSeriesDataPackage {}
