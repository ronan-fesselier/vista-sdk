use std::ffi::{CStr, CString};

use crate::ffi::core::common::dnv_vista_sdk_string_free;
use crate::ffi::transport::iso19848 as ffi;
use crate::transport::iso19848_version::Iso19848Version;
use crate::types::date_time_offset::DateTimeOffset;
use crate::types::decimal::Decimal;

/// A data channel type name entry from the ISO 19848 specification.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct DataChannelTypeName {
    /// The type identifier string (e.g. `"Inst"`, `"Mean"`).
    pub type_: String,
    /// Human-readable description.
    pub description: String,
}

/// A collection of data channel type names for a specific ISO 19848 version.
pub struct DataChannelTypeNames(*mut ffi::dnv_vista_sdk_iso19848_data_channel_type_names_t);

impl Drop for DataChannelTypeNames {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `DataChannelTypeNames` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_iso19848_data_channel_type_names_free(self.0) }
    }
}

impl DataChannelTypeNames {
    /// Returns the number of data channel type names in this collection.
    pub fn len(&self) -> usize {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_iso19848_data_channel_type_names_count(self.0) }
    }

    /// Returns `true` if the collection contains no entries.
    pub fn is_empty(&self) -> bool {
        self.len() == 0
    }

    /// Looks up a data channel type name by its type identifier. Returns `None` if not found.
    pub fn find(&self, type_: &str) -> Option<DataChannelTypeName> {
        let c = CString::new(type_).ok()?;
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_iso19848_data_channel_type_names_from_string(self.0, c.as_ptr())
        };
        if ptr.is_null() {
            return None;
        }
        // SAFETY: ptr is non-null and owned until freed below.
        let result = unsafe { read_data_channel_type_name(ptr) };
        // SAFETY: ptr was allocated by the library and is freed exactly once.
        unsafe { ffi::dnv_vista_sdk_iso19848_data_channel_type_name_free(ptr) };
        Some(result)
    }

    /// Returns the entry at `index`. Returns `None` if `index` is out of range.
    pub fn at(&self, index: usize) -> Option<DataChannelTypeName> {
        if index >= self.len() {
            return None;
        }
        // SAFETY: self.0 is non-null and index is in bounds (checked above).
        let ptr = unsafe { ffi::dnv_vista_sdk_iso19848_data_channel_type_names_at(self.0, index) };
        if ptr.is_null() {
            return None;
        }
        // SAFETY: ptr is non-null and owned by `self` for the call's duration.
        Some(unsafe { read_data_channel_type_name(ptr) })
    }

    /// Returns an iterator over all entries.
    pub fn iter(&self) -> impl Iterator<Item = DataChannelTypeName> + '_ {
        (0..self.len()).filter_map(move |i| self.at(i))
    }
}

/// SAFETY: caller must ensure `ptr` is non-null and valid for the duration of the call.
unsafe fn read_data_channel_type_name(
    ptr: *const ffi::dnv_vista_sdk_iso19848_data_channel_type_name_t,
) -> DataChannelTypeName {
    let type_ = CStr::from_ptr(ffi::dnv_vista_sdk_iso19848_data_channel_type_name_type(ptr))
        .to_string_lossy()
        .into_owned();
    let description =
        CStr::from_ptr(ffi::dnv_vista_sdk_iso19848_data_channel_type_name_description(ptr))
            .to_string_lossy()
            .into_owned();
    DataChannelTypeName { type_, description }
}

/// A discriminated union holding one ISO 19848 data value.
#[derive(Debug, Clone, PartialEq)]
pub enum Value {
    /// A fixed-point decimal value.
    Decimal(Decimal),
    /// A 64-bit signed integer value.
    Integer(i64),
    /// A boolean value.
    Boolean(bool),
    /// A UTF-8 string value.
    String(String),
    /// A date-time-with-offset value.
    DateTime(DateTimeOffset),
}

impl Value {
    /// Constructs a `Value` from a string (the string variant).
    pub fn from_string(value: &str) -> Option<Self> {
        let c = CString::new(value).ok()?;
        // SAFETY: c is a valid NUL-terminated C string. Returns an owned pointer or NULL.
        let ptr = unsafe { ffi::dnv_vista_sdk_iso19848_value_from_string(c.as_ptr()) };
        if ptr.is_null() {
            return None;
        }
        // SAFETY: ptr is non-null and owned until freed below.
        let result = unsafe { read_value(ptr) };
        // SAFETY: ptr was allocated by the library and is freed exactly once.
        unsafe { ffi::dnv_vista_sdk_iso19848_value_free(ptr) };
        Some(result)
    }

    /// Constructs a `Value` holding a 64-bit integer.
    pub fn from_integer(value: i64) -> Self {
        // SAFETY: no pointer arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_iso19848_value_from_integer(value) };
        // SAFETY: ptr is non-null and owned until freed below.
        let result = unsafe { read_value(ptr) };
        // SAFETY: ptr was allocated by the library and is freed exactly once.
        unsafe { ffi::dnv_vista_sdk_iso19848_value_free(ptr) };
        result
    }

    /// Constructs a `Value` holding a boolean.
    pub fn from_boolean(value: bool) -> Self {
        // SAFETY: no pointer arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_iso19848_value_from_boolean(value as i32) };
        // SAFETY: ptr is non-null and owned until freed below.
        let result = unsafe { read_value(ptr) };
        // SAFETY: ptr was allocated by the library and is freed exactly once.
        unsafe { ffi::dnv_vista_sdk_iso19848_value_free(ptr) };
        result
    }

    /// Constructs a `Value` holding a `Decimal`.
    pub fn from_decimal(value: Decimal) -> Self {
        // SAFETY: no pointer arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_iso19848_value_from_decimal(value.into_ffi()) };
        // SAFETY: ptr is non-null and owned until freed below.
        let result = unsafe { read_value(ptr) };
        // SAFETY: ptr was allocated by the library and is freed exactly once.
        unsafe { ffi::dnv_vista_sdk_iso19848_value_free(ptr) };
        result
    }

    /// Constructs a `Value` holding a `DateTimeOffset`.
    pub fn from_date_time(value: DateTimeOffset) -> Self {
        // SAFETY: no pointer arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_iso19848_value_from_date_time(value.into_ffi()) };
        // SAFETY: ptr is non-null and owned until freed below.
        let result = unsafe { read_value(ptr) };
        // SAFETY: ptr was allocated by the library and is freed exactly once.
        unsafe { ffi::dnv_vista_sdk_iso19848_value_free(ptr) };
        result
    }

    /// Returns the string representation of this value.
    pub fn to_string_value(&self) -> String {
        let ptr = match self {
            Value::String(s) => return s.clone(),
            // SAFETY: no pointer arguments. Returns a non-null owned pointer.
            Value::Integer(i) => unsafe { ffi::dnv_vista_sdk_iso19848_value_from_integer(*i) },
            // SAFETY: no pointer arguments. Returns a non-null owned pointer.
            Value::Boolean(b) => unsafe {
                ffi::dnv_vista_sdk_iso19848_value_from_boolean(*b as i32)
            },
            // SAFETY: no pointer arguments. Returns a non-null owned pointer.
            Value::Decimal(d) => unsafe {
                ffi::dnv_vista_sdk_iso19848_value_from_decimal(d.into_ffi())
            },
            // SAFETY: no pointer arguments. Returns a non-null owned pointer.
            Value::DateTime(dt) => unsafe {
                ffi::dnv_vista_sdk_iso19848_value_from_date_time(dt.into_ffi())
            },
        };
        // SAFETY: ptr is non-null and owned until freed below.
        let s_ptr = unsafe { ffi::dnv_vista_sdk_iso19848_value_to_string(ptr) };
        // SAFETY: ptr was allocated by the library and is freed exactly once.
        unsafe { ffi::dnv_vista_sdk_iso19848_value_free(ptr) };
        // SAFETY: s_ptr is non-null and owned until freed below.
        let result = unsafe { CStr::from_ptr(s_ptr).to_string_lossy().into_owned() };
        // SAFETY: s_ptr was allocated by the library and is freed exactly once.
        unsafe { dnv_vista_sdk_string_free(s_ptr) };
        result
    }
}

impl std::fmt::Display for Value {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(&self.to_string_value())
    }
}

/// SAFETY: caller must ensure `ptr` is non-null and valid for the duration of the call.
pub(crate) unsafe fn read_value(ptr: *const ffi::dnv_vista_sdk_iso19848_value_t) -> Value {
    match ffi::dnv_vista_sdk_iso19848_value_type(ptr) {
        ffi::dnv_vista_sdk_iso19848_value_type_t::Decimal => {
            let mut raw = std::mem::zeroed();
            ffi::dnv_vista_sdk_iso19848_value_decimal(ptr, &mut raw);
            Value::Decimal(Decimal::from_ffi(raw))
        }
        ffi::dnv_vista_sdk_iso19848_value_type_t::Integer => {
            let mut v: i64 = 0;
            ffi::dnv_vista_sdk_iso19848_value_integer(ptr, &mut v);
            Value::Integer(v)
        }
        ffi::dnv_vista_sdk_iso19848_value_type_t::Boolean => {
            let mut v: i32 = 0;
            ffi::dnv_vista_sdk_iso19848_value_boolean(ptr, &mut v);
            Value::Boolean(v != 0)
        }
        ffi::dnv_vista_sdk_iso19848_value_type_t::String => {
            let s = CStr::from_ptr(ffi::dnv_vista_sdk_iso19848_value_string(ptr))
                .to_string_lossy()
                .into_owned();
            Value::String(s)
        }
        ffi::dnv_vista_sdk_iso19848_value_type_t::DateTime => {
            let mut raw = std::mem::zeroed();
            ffi::dnv_vista_sdk_iso19848_value_date_time(ptr, &mut raw);
            Value::DateTime(DateTimeOffset::from_ffi(raw))
        }
    }
}

/// A format data type entry from the ISO 19848 specification.
pub struct FormatDataType(*mut ffi::dnv_vista_sdk_iso19848_format_data_type_t);

impl Drop for FormatDataType {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `FormatDataType` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_iso19848_format_data_type_free(self.0) }
    }
}

impl FormatDataType {
    /// The type identifier string (e.g. `"Numeric"`, `"Discrete"`, `"DateTime"`).
    pub fn type_(&self) -> &str {
        // SAFETY: self.0 is non-null and owned by `self` for its lifetime.
        unsafe {
            CStr::from_ptr(ffi::dnv_vista_sdk_iso19848_format_data_type_type(self.0))
                .to_str()
                .unwrap_or("")
        }
    }

    /// Human-readable description of this format data type.
    pub fn description(&self) -> &str {
        // SAFETY: self.0 is non-null and owned by `self` for its lifetime.
        unsafe {
            CStr::from_ptr(ffi::dnv_vista_sdk_iso19848_format_data_type_description(
                self.0,
            ))
            .to_str()
            .unwrap_or("")
        }
    }

    /// Validates `value` against this format type and returns the parsed `Value` on success.
    pub fn validate(&self, value: &str) -> Option<Value> {
        let c = CString::new(value).ok()?;
        let mut result_ptr: *mut ffi::dnv_vista_sdk_iso19848_value_t = std::ptr::null_mut();
        // SAFETY: self.0 is non-null, c is a valid NUL-terminated C string, and `result_ptr`
        // is a valid writable pointer to a local.
        let ok = unsafe {
            ffi::dnv_vista_sdk_iso19848_format_data_type_validate(
                self.0,
                c.as_ptr(),
                &mut result_ptr,
            )
        };
        if ok == 0 || result_ptr.is_null() {
            return None;
        }
        // SAFETY: result_ptr is non-null (checked above) and owned until freed below.
        let result = unsafe { read_value(result_ptr) };
        // SAFETY: result_ptr was allocated by the library and is freed exactly once.
        unsafe { ffi::dnv_vista_sdk_iso19848_value_free(result_ptr) };
        Some(result)
    }
}

/// A collection of format data types for a specific ISO 19848 version.
pub struct FormatDataTypes(*mut ffi::dnv_vista_sdk_iso19848_format_data_types_t);

impl Drop for FormatDataTypes {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `FormatDataTypes` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_iso19848_format_data_types_free(self.0) }
    }
}

impl FormatDataTypes {
    /// Returns the number of format data types in this collection.
    pub fn len(&self) -> usize {
        // SAFETY: self.0 is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_iso19848_format_data_types_count(self.0) }
    }

    /// Returns `true` if the collection contains no entries.
    pub fn is_empty(&self) -> bool {
        self.len() == 0
    }

    /// Looks up a format data type by its type identifier. Returns `None` if not found.
    pub fn find(&self, type_: &str) -> Option<FormatDataType> {
        let c = CString::new(type_).ok()?;
        // SAFETY: self.0 is non-null, and c is a valid NUL-terminated C string.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_iso19848_format_data_types_from_string(self.0, c.as_ptr())
        };
        if ptr.is_null() {
            None
        } else {
            Some(FormatDataType(ptr))
        }
    }

    /// Returns the entry at `index`. Returns `None` if `index` is out of range.
    pub fn at(&self, index: usize) -> Option<FormatDataTypeRef<'_>> {
        if index >= self.len() {
            return None;
        }
        // SAFETY: self.0 is non-null and index is in bounds (checked above).
        let ptr = unsafe { ffi::dnv_vista_sdk_iso19848_format_data_types_at(self.0, index) };
        if ptr.is_null() {
            None
        } else {
            Some(FormatDataTypeRef(ptr, std::marker::PhantomData))
        }
    }

    /// Returns an iterator over borrowed views of all entries.
    pub fn iter(&self) -> impl Iterator<Item = FormatDataTypeRef<'_>> {
        (0..self.len()).filter_map(move |i| self.at(i))
    }
}

/// A borrowed view of a `FormatDataType` entry. Valid as long as the parent `FormatDataTypes` is alive.
pub struct FormatDataTypeRef<'a>(
    *const ffi::dnv_vista_sdk_iso19848_format_data_type_t,
    std::marker::PhantomData<&'a FormatDataTypes>,
);

impl<'a> FormatDataTypeRef<'a> {
    /// The type identifier string (e.g. `"Numeric"`, `"Discrete"`, `"DateTime"`).
    pub fn type_(&self) -> &str {
        // SAFETY: self.0 is non-null and valid for lifetime `'a`.
        unsafe {
            CStr::from_ptr(ffi::dnv_vista_sdk_iso19848_format_data_type_type(self.0))
                .to_str()
                .unwrap_or("")
        }
    }

    /// Human-readable description of this format data type.
    pub fn description(&self) -> &str {
        // SAFETY: self.0 is non-null and valid for lifetime `'a`.
        unsafe {
            CStr::from_ptr(ffi::dnv_vista_sdk_iso19848_format_data_type_description(
                self.0,
            ))
            .to_str()
            .unwrap_or("")
        }
    }

    /// Validates `value` against this format type and returns the parsed `Value` on success.
    pub fn validate(&self, value: &str) -> Option<Value> {
        let c = CString::new(value).ok()?;
        let mut result_ptr: *mut ffi::dnv_vista_sdk_iso19848_value_t = std::ptr::null_mut();
        // SAFETY: self.0 is non-null, c is a valid NUL-terminated C string, and `result_ptr`
        // is a valid writable pointer to a local.
        let ok = unsafe {
            ffi::dnv_vista_sdk_iso19848_format_data_type_validate(
                self.0,
                c.as_ptr(),
                &mut result_ptr,
            )
        };
        if ok == 0 || result_ptr.is_null() {
            return None;
        }
        // SAFETY: result_ptr is non-null (checked above) and owned until freed below.
        let result = unsafe { read_value(result_ptr) };
        // SAFETY: result_ptr was allocated by the library and is freed exactly once.
        unsafe { ffi::dnv_vista_sdk_iso19848_value_free(result_ptr) };
        Some(result)
    }
}

/// The ISO 19848 singleton - entry point for accessing versioned type definitions.
pub struct Iso19848(*const ffi::dnv_vista_sdk_iso19848_t);

impl Iso19848 {
    /// Returns the global ISO 19848 singleton instance.
    pub fn instance() -> Self {
        // SAFETY: no arguments. Returns a non-null pointer valid for the program's lifetime.
        Iso19848(unsafe { ffi::dnv_vista_sdk_iso19848_instance() })
    }

    /// Returns all available ISO 19848 versions.
    pub fn versions(&self) -> Vec<Iso19848Version> {
        // SAFETY: self.0 is non-null and valid for the program's lifetime.
        let count = unsafe { ffi::dnv_vista_sdk_iso19848_version_count(self.0) };
        (0..count)
            .map(|i| {
                // SAFETY: self.0 is non-null, and i is in [0, count).
                let v = unsafe { ffi::dnv_vista_sdk_iso19848_version_at(self.0, i) };
                ffi_version_to_rust(v)
            })
            .collect()
    }

    /// Returns the latest available ISO 19848 version.
    pub fn latest(&self) -> Iso19848Version {
        // SAFETY: self.0 is non-null and valid for the program's lifetime.
        ffi_version_to_rust(unsafe { ffi::dnv_vista_sdk_iso19848_latest(self.0) })
    }

    /// Returns the data channel type names for `version`. Returns `None` if unavailable.
    pub fn data_channel_type_names(
        &self,
        version: Iso19848Version,
    ) -> Option<DataChannelTypeNames> {
        // SAFETY: self.0 is non-null and valid for the program's lifetime.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_iso19848_data_channel_type_names(
                self.0,
                rust_version_to_ffi(version),
            )
        };
        if ptr.is_null() {
            None
        } else {
            Some(DataChannelTypeNames(ptr))
        }
    }

    /// Returns the format data types for `version`. Returns `None` if unavailable.
    pub fn format_data_types(&self, version: Iso19848Version) -> Option<FormatDataTypes> {
        // SAFETY: self.0 is non-null and valid for the program's lifetime.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_iso19848_format_data_types(self.0, rust_version_to_ffi(version))
        };
        if ptr.is_null() {
            None
        } else {
            Some(FormatDataTypes(ptr))
        }
    }
}

fn ffi_version_to_rust(v: ffi::dnv_vista_sdk_iso19848_version_t) -> Iso19848Version {
    match v {
        ffi::dnv_vista_sdk_iso19848_version_t::V2018 => Iso19848Version::V2018,
        ffi::dnv_vista_sdk_iso19848_version_t::V2024 => Iso19848Version::V2024,
    }
}

fn rust_version_to_ffi(v: Iso19848Version) -> ffi::dnv_vista_sdk_iso19848_version_t {
    match v {
        Iso19848Version::V2018 => ffi::dnv_vista_sdk_iso19848_version_t::V2018,
        Iso19848Version::V2024 => ffi::dnv_vista_sdk_iso19848_version_t::V2024,
    }
}
