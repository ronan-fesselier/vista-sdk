use std::ffi::{CStr, CString};
use std::ptr::NonNull;

use crate::core::error::{last_error, VistaError};
use crate::core::imo_number::ImoNumber;
use crate::core::local_id::LocalId;
use crate::ffi::core::common::dnv_vista_sdk_string_free;
use crate::ffi::core::imo_number as imo_ffi;
use crate::ffi::core::local_id as local_id_ffi;
use crate::ffi::transport::datachannel::data_channel as ffi;
use crate::ffi::transport::ship_id as ship_id_ffi;
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

unsafe fn opt_str(ptr: *const std::ffi::c_char) -> Option<String> {
    if ptr.is_null() {
        None
    } else {
        Some(
            CStr::from_ptr(ptr)
                .to_str()
                .expect("invalid UTF-8")
                .to_string(),
        )
    }
}

unsafe fn req_str(ptr: *const std::ffi::c_char) -> String {
    CStr::from_ptr(ptr)
        .to_str()
        .expect("invalid UTF-8")
        .to_string()
}

fn read_ship_id_from_ffi(ptr: *const ship_id_ffi::dnv_vista_sdk_ship_id_t) -> ShipId {
    if (unsafe { ship_id_ffi::dnv_vista_sdk_ship_id_is_imo_number(ptr) }) != 0 {
        let imo_ptr = unsafe { ship_id_ffi::dnv_vista_sdk_ship_id_imo_number(ptr) };
        let v = unsafe { imo_ffi::dnv_vista_sdk_imo_number_value(imo_ptr) };
        unsafe { imo_ffi::dnv_vista_sdk_imo_number_free(imo_ptr) };
        ShipId::Imo(ImoNumber(v as u32))
    } else {
        let raw = unsafe { ship_id_ffi::dnv_vista_sdk_ship_id_other_id(ptr) };
        let s = unsafe { CStr::from_ptr(raw) }
            .to_str()
            .expect("invalid UTF-8")
            .to_string();
        ShipId::Other(s)
    }
}

/// ISO 19848 Table 19 - value restriction for a data channel format.
pub struct Restriction(NonNull<ffi::dnv_vista_sdk_dcl_restriction_t>);

impl Restriction {
    /// Creates a new restriction with no constraints.
    pub fn new() -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_restriction_create() };
        Self(NonNull::new(ptr).expect("restriction_create returned null"))
    }

    /// Returns the number of enumeration values, or 0 if none are set.
    pub fn enumeration_count(&self) -> usize {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_enumeration_count(self.0.as_ptr()) }
    }

    /// Returns the enumeration value at `index`, or `None` if out of range.
    pub fn enumeration_at(&self, index: usize) -> Option<String> {
        let ptr =
            unsafe { ffi::dnv_vista_sdk_dcl_restriction_enumeration_at(self.0.as_ptr(), index) };
        if ptr.is_null() {
            None
        } else {
            Some(unsafe { req_str(ptr) })
        }
    }

    /// Sets the allowed enumeration values.
    pub fn set_enumeration(&mut self, values: &[&str]) {
        let cstrings: Vec<CString> = values
            .iter()
            .map(|s| CString::new(*s).expect("enumeration value contains NUL byte"))
            .collect();
        let ptrs: Vec<*const std::ffi::c_char> = cstrings.iter().map(|s| s.as_ptr()).collect();
        unsafe {
            ffi::dnv_vista_sdk_dcl_restriction_set_enumeration(
                self.0.as_ptr(),
                ptrs.as_ptr(),
                ptrs.len(),
            )
        };
    }

    /// Clears all enumeration values.
    pub fn clear_enumeration(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_enumeration(self.0.as_ptr()) };
    }

    /// Returns the fraction-digits constraint if set.
    pub fn fraction_digits(&self) -> Option<u32> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_fraction_digits(self.0.as_ptr()) }) != 0
        {
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_fraction_digits(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Sets the fraction-digits constraint.
    pub fn set_fraction_digits(&mut self, value: u32) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_fraction_digits(self.0.as_ptr(), value) };
    }

    /// Clears the fraction-digits constraint.
    pub fn clear_fraction_digits(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_fraction_digits(self.0.as_ptr()) };
    }

    /// Returns the length constraint if set.
    pub fn length(&self) -> Option<u32> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_length(self.0.as_ptr()) }) != 0 {
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_length(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Sets the length constraint.
    pub fn set_length(&mut self, value: u32) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_length(self.0.as_ptr(), value) };
    }

    /// Clears the length constraint.
    pub fn clear_length(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_length(self.0.as_ptr()) };
    }

    /// Returns the max-exclusive constraint if set.
    pub fn max_exclusive(&self) -> Option<f64> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_max_exclusive(self.0.as_ptr()) }) != 0 {
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_max_exclusive(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Sets the max-exclusive constraint.
    pub fn set_max_exclusive(&mut self, value: f64) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_max_exclusive(self.0.as_ptr(), value) };
    }

    /// Clears the max-exclusive constraint.
    pub fn clear_max_exclusive(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_max_exclusive(self.0.as_ptr()) };
    }

    /// Returns the max-inclusive constraint if set.
    pub fn max_inclusive(&self) -> Option<f64> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_max_inclusive(self.0.as_ptr()) }) != 0 {
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_max_inclusive(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Sets the max-inclusive constraint.
    pub fn set_max_inclusive(&mut self, value: f64) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_max_inclusive(self.0.as_ptr(), value) };
    }

    /// Clears the max-inclusive constraint.
    pub fn clear_max_inclusive(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_max_inclusive(self.0.as_ptr()) };
    }

    /// Returns the max-length constraint if set.
    pub fn max_length(&self) -> Option<u32> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_max_length(self.0.as_ptr()) }) != 0 {
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_max_length(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Sets the max-length constraint.
    pub fn set_max_length(&mut self, value: u32) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_max_length(self.0.as_ptr(), value) };
    }

    /// Clears the max-length constraint.
    pub fn clear_max_length(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_max_length(self.0.as_ptr()) };
    }

    /// Returns the min-exclusive constraint if set.
    pub fn min_exclusive(&self) -> Option<f64> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_min_exclusive(self.0.as_ptr()) }) != 0 {
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_min_exclusive(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Sets the min-exclusive constraint.
    pub fn set_min_exclusive(&mut self, value: f64) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_min_exclusive(self.0.as_ptr(), value) };
    }

    /// Clears the min-exclusive constraint.
    pub fn clear_min_exclusive(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_min_exclusive(self.0.as_ptr()) };
    }

    /// Returns the min-inclusive constraint if set.
    pub fn min_inclusive(&self) -> Option<f64> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_min_inclusive(self.0.as_ptr()) }) != 0 {
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_min_inclusive(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Sets the min-inclusive constraint.
    pub fn set_min_inclusive(&mut self, value: f64) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_min_inclusive(self.0.as_ptr(), value) };
    }

    /// Clears the min-inclusive constraint.
    pub fn clear_min_inclusive(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_min_inclusive(self.0.as_ptr()) };
    }

    /// Returns the min-length constraint if set.
    pub fn min_length(&self) -> Option<u32> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_min_length(self.0.as_ptr()) }) != 0 {
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_min_length(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Sets the min-length constraint.
    pub fn set_min_length(&mut self, value: u32) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_min_length(self.0.as_ptr(), value) };
    }

    /// Clears the min-length constraint.
    pub fn clear_min_length(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_min_length(self.0.as_ptr()) };
    }

    /// Returns the pattern constraint if set.
    pub fn pattern(&self) -> Option<String> {
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_restriction_pattern(self.0.as_ptr()) };
        unsafe { opt_str(ptr) }
    }

    /// Sets the pattern constraint.
    pub fn set_pattern(&mut self, value: &str) {
        let c = CString::new(value).expect("pattern contains NUL byte");
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_pattern(self.0.as_ptr(), c.as_ptr()) };
    }

    /// Clears the pattern constraint.
    pub fn clear_pattern(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_pattern(self.0.as_ptr()) };
    }

    /// Returns the total-digits constraint if set.
    pub fn total_digits(&self) -> Option<u32> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_total_digits(self.0.as_ptr()) }) != 0 {
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_total_digits(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Sets the total-digits constraint.
    pub fn set_total_digits(&mut self, value: u32) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_set_total_digits(self.0.as_ptr(), value) };
    }

    /// Clears the total-digits constraint.
    pub fn clear_total_digits(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_total_digits(self.0.as_ptr()) };
    }

    /// Returns the white-space constraint if set.
    pub fn white_space(&self) -> Option<WhiteSpace> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_white_space(self.0.as_ptr()) }) != 0 {
            Some(white_space_from_ffi(unsafe {
                ffi::dnv_vista_sdk_dcl_restriction_white_space(self.0.as_ptr())
            }))
        } else {
            None
        }
    }

    /// Sets the white-space constraint.
    pub fn set_white_space(&mut self, value: WhiteSpace) {
        unsafe {
            ffi::dnv_vista_sdk_dcl_restriction_set_white_space(
                self.0.as_ptr(),
                white_space_to_ffi(value),
            )
        };
    }

    /// Clears the white-space constraint.
    pub fn clear_white_space(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_clear_white_space(self.0.as_ptr()) };
    }

    /// Returns `true` if `value` satisfies all constraints of this restriction.
    pub fn validate_value(&self, value: &str, format: &Format) -> bool {
        let c = CString::new(value).expect("value contains NUL byte");
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
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_free(self.0.as_ptr()) };
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
        unsafe { ffi::dnv_vista_sdk_dcl_range_low(self.0.as_ptr()) }
    }

    /// Returns the upper bound.
    pub fn high(&self) -> f64 {
        unsafe { ffi::dnv_vista_sdk_dcl_range_high(self.0.as_ptr()) }
    }

    /// Sets the lower bound.
    pub fn set_low(&mut self, low: f64) {
        unsafe { ffi::dnv_vista_sdk_dcl_range_set_low(self.0.as_ptr(), low) };
    }

    /// Sets the upper bound.
    pub fn set_high(&mut self, high: f64) {
        unsafe { ffi::dnv_vista_sdk_dcl_range_set_high(self.0.as_ptr(), high) };
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_range_t {
        self.0.as_ptr()
    }
}

impl Drop for Range {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_range_free(self.0.as_ptr()) };
    }
}

/// ISO 19848 Table 18 - data type and optional restriction for a data channel.
pub struct Format(NonNull<ffi::dnv_vista_sdk_dcl_format_t>);

impl Format {
    /// Creates a format with the given type string (e.g. `"Numeric"`, `"String"`).
    pub fn new(type_: &str) -> Self {
        let c = CString::new(type_).expect("type contains NUL byte");
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_format_create(c.as_ptr()) };
        Self(NonNull::new(ptr).expect("format_create returned null"))
    }

    /// Returns the format type string.
    pub fn type_(&self) -> String {
        unsafe { req_str(ffi::dnv_vista_sdk_dcl_format_type(self.0.as_ptr())) }
    }

    /// Sets the format type string. Returns `false` if the value is empty or invalid.
    pub fn set_type(&mut self, type_: &str) -> bool {
        let c = CString::new(type_).expect("type contains NUL byte");
        (unsafe { ffi::dnv_vista_sdk_dcl_format_set_type(self.0.as_ptr(), c.as_ptr()) }) != 0
    }

    /// Returns the restriction if one is set.
    pub fn restriction(&self) -> Option<RestrictionRef<'_>> {
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

    /// Sets the restriction by copying it from `restriction`.
    pub fn set_restriction(&mut self, restriction: &Restriction) {
        unsafe {
            ffi::dnv_vista_sdk_dcl_format_set_restriction(self.0.as_ptr(), restriction.as_ptr())
        };
    }

    /// Clears the restriction.
    pub fn clear_restriction(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_format_clear_restriction(self.0.as_ptr()) };
    }

    /// Returns `true` if `value` is valid according to this format.
    pub fn validate_value(&self, value: &str) -> bool {
        let c = CString::new(value).expect("value contains NUL byte");
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
        unsafe { ffi::dnv_vista_sdk_dcl_format_free(self.0.as_ptr()) };
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
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_fraction_digits(self.ptr) }) != 0 {
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_fraction_digits(self.ptr) })
        } else {
            None
        }
    }

    /// Returns the white-space constraint if set.
    pub fn white_space(&self) -> Option<WhiteSpace> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_white_space(self.ptr) }) != 0 {
            Some(white_space_from_ffi(unsafe {
                ffi::dnv_vista_sdk_dcl_restriction_white_space(self.ptr)
            }))
        } else {
            None
        }
    }

    /// Returns the pattern constraint if set.
    pub fn pattern(&self) -> Option<String> {
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_restriction_pattern(self.ptr) };
        unsafe { opt_str(ptr) }
    }

    /// Returns the enumeration count.
    pub fn enumeration_count(&self) -> usize {
        unsafe { ffi::dnv_vista_sdk_dcl_restriction_enumeration_count(self.ptr) }
    }

    /// Returns the enumeration value at `index`, or `None` if out of range.
    pub fn enumeration_at(&self, index: usize) -> Option<String> {
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_restriction_enumeration_at(self.ptr, index) };
        if ptr.is_null() {
            None
        } else {
            Some(unsafe { req_str(ptr) })
        }
    }

    /// Returns the total-digits constraint if set.
    pub fn total_digits(&self) -> Option<u32> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_restriction_has_total_digits(self.ptr) }) != 0 {
            Some(unsafe { ffi::dnv_vista_sdk_dcl_restriction_total_digits(self.ptr) })
        } else {
            None
        }
    }
}

/// ISO 19848 Table 17 - data channel type (e.g. `"Inst"`, `"Calc"`, `"ManualInput"`).
pub struct DataChannelType(NonNull<ffi::dnv_vista_sdk_dcl_data_channel_type_t>);

impl DataChannelType {
    /// Creates a data channel type with the given type string.
    pub fn new(type_: &str) -> Self {
        let c = CString::new(type_).expect("type contains NUL byte");
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_data_channel_type_create(c.as_ptr()) };
        Self(NonNull::new(ptr).expect("data_channel_type_create returned null"))
    }

    /// Returns the type string.
    pub fn type_(&self) -> String {
        unsafe {
            req_str(ffi::dnv_vista_sdk_dcl_data_channel_type_type(
                self.0.as_ptr(),
            ))
        }
    }

    /// Sets the type string. Returns `false` if the value is empty or invalid.
    pub fn set_type(&mut self, type_: &str) -> bool {
        let c = CString::new(type_).expect("type contains NUL byte");
        (unsafe { ffi::dnv_vista_sdk_dcl_data_channel_type_set_type(self.0.as_ptr(), c.as_ptr()) })
            != 0
    }

    /// Returns the update cycle in seconds if set.
    pub fn update_cycle(&self) -> Option<f64> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_data_channel_type_has_update_cycle(self.0.as_ptr()) })
            != 0
        {
            Some(unsafe { ffi::dnv_vista_sdk_dcl_data_channel_type_update_cycle(self.0.as_ptr()) })
        } else {
            None
        }
    }

    /// Sets the update cycle.
    pub fn set_update_cycle(&mut self, value: f64) {
        unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_type_set_update_cycle(self.0.as_ptr(), value)
        };
    }

    /// Clears the update cycle.
    pub fn clear_update_cycle(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_data_channel_type_clear_update_cycle(self.0.as_ptr()) };
    }

    /// Returns the calculation period in seconds if set.
    pub fn calculation_period(&self) -> Option<f64> {
        if (unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_type_has_calculation_period(self.0.as_ptr())
        }) != 0
        {
            Some(unsafe {
                ffi::dnv_vista_sdk_dcl_data_channel_type_calculation_period(self.0.as_ptr())
            })
        } else {
            None
        }
    }

    /// Sets the calculation period.
    pub fn set_calculation_period(&mut self, value: f64) {
        unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_type_set_calculation_period(self.0.as_ptr(), value)
        };
    }

    /// Clears the calculation period.
    pub fn clear_calculation_period(&mut self) {
        unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_type_clear_calculation_period(self.0.as_ptr())
        };
    }

    /// Returns `true` if this channel type represents an alarm/alert.
    pub fn is_alert(&self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_dcl_data_channel_type_is_alert(self.0.as_ptr()) }) != 0
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_data_channel_type_t {
        self.0.as_ptr()
    }
}

impl Drop for DataChannelType {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_data_channel_type_free(self.0.as_ptr()) };
    }
}

/// ISO 19848 Table 22 - naming rule and optional custom name objects.
pub struct NameObject(NonNull<ffi::dnv_vista_sdk_dcl_name_object_t>);

impl NameObject {
    /// Creates a name object with no naming rule.
    pub fn new() -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_name_object_create_default() };
        Self(NonNull::new(ptr).expect("name_object_create_default returned null"))
    }

    /// Creates a name object with the given naming rule.
    pub fn with_naming_rule(naming_rule: &str) -> Self {
        let c = CString::new(naming_rule).expect("naming_rule contains NUL byte");
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_name_object_create(c.as_ptr()) };
        Self(NonNull::new(ptr).expect("name_object_create returned null"))
    }

    /// Returns the naming rule string.
    pub fn naming_rule(&self) -> String {
        unsafe {
            req_str(ffi::dnv_vista_sdk_dcl_name_object_naming_rule(
                self.0.as_ptr(),
            ))
        }
    }

    /// Sets the naming rule string.
    pub fn set_naming_rule(&mut self, naming_rule: &str) {
        let c = CString::new(naming_rule).expect("naming_rule contains NUL byte");
        unsafe { ffi::dnv_vista_sdk_dcl_name_object_set_naming_rule(self.0.as_ptr(), c.as_ptr()) };
    }

    /// Sets the custom name objects, transferring ownership of `doc`.
    pub fn set_custom_name_objects(&mut self, doc: SerializableDocument) {
        let raw = doc.into_raw();
        unsafe { ffi::dnv_vista_sdk_dcl_name_object_set_custom_name_objects(self.0.as_ptr(), raw) };
    }

    /// Clears the custom name objects.
    pub fn clear_custom_name_objects(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_name_object_clear_custom_name_objects(self.0.as_ptr()) };
    }

    /// Returns a borrowed view of the custom name objects if set.
    pub fn custom_name_objects(&self) -> Option<SerializableDocumentRef<'_>> {
        let ptr =
            unsafe { ffi::dnv_vista_sdk_dcl_name_object_custom_name_objects(self.0.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(SerializableDocumentRef::from_ptr(ptr))
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
        unsafe { ffi::dnv_vista_sdk_dcl_name_object_free(self.0.as_ptr()) };
    }
}

/// ISO 19848 Table 21 - unit of measurement for a data channel.
pub struct Unit(NonNull<ffi::dnv_vista_sdk_dcl_unit_t>);

impl Unit {
    /// Creates a unit with the given symbol (e.g. `"°C"`, `"kW"`).
    pub fn new(unit_symbol: &str) -> Self {
        let c = CString::new(unit_symbol).expect("unit_symbol contains NUL byte");
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_unit_create(c.as_ptr()) };
        Self(NonNull::new(ptr).expect("unit_create returned null"))
    }

    /// Returns the unit symbol.
    pub fn unit_symbol(&self) -> String {
        unsafe { req_str(ffi::dnv_vista_sdk_dcl_unit_unit_symbol(self.0.as_ptr())) }
    }

    /// Sets the unit symbol.
    pub fn set_unit_symbol(&mut self, unit_symbol: &str) {
        let c = CString::new(unit_symbol).expect("unit_symbol contains NUL byte");
        unsafe { ffi::dnv_vista_sdk_dcl_unit_set_unit_symbol(self.0.as_ptr(), c.as_ptr()) };
    }

    /// Returns the quantity name if set (e.g. `"Temperature"`).
    pub fn quantity_name(&self) -> Option<String> {
        unsafe { opt_str(ffi::dnv_vista_sdk_dcl_unit_quantity_name(self.0.as_ptr())) }
    }

    /// Sets the quantity name.
    pub fn set_quantity_name(&mut self, quantity_name: &str) {
        let c = CString::new(quantity_name).expect("quantity_name contains NUL byte");
        unsafe { ffi::dnv_vista_sdk_dcl_unit_set_quantity_name(self.0.as_ptr(), c.as_ptr()) };
    }

    /// Clears the quantity name.
    pub fn clear_quantity_name(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_unit_clear_quantity_name(self.0.as_ptr()) };
    }

    /// Sets the custom elements, transferring ownership of `doc`.
    pub fn set_custom_elements(&mut self, doc: SerializableDocument) {
        let raw = doc.into_raw();
        unsafe { ffi::dnv_vista_sdk_dcl_unit_set_custom_elements(self.0.as_ptr(), raw) };
    }

    /// Clears the custom elements.
    pub fn clear_custom_elements(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_unit_clear_custom_elements(self.0.as_ptr()) };
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_unit_t {
        self.0.as_ptr()
    }
}

impl Drop for Unit {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_unit_free(self.0.as_ptr()) };
    }
}

/// ISO 19848 Table 16 - properties describing a data channel's type, format, range, and unit.
pub struct Property(NonNull<ffi::dnv_vista_sdk_dcl_property_t>);

impl Property {
    /// Creates a property with the required data channel type and format.
    pub fn new(data_channel_type: &DataChannelType, format: &Format) -> Self {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_dcl_property_create(data_channel_type.as_ptr(), format.as_ptr())
        };
        Self(NonNull::new(ptr).expect("property_create returned null"))
    }

    /// Sets the data channel type.
    pub fn set_data_channel_type(&mut self, data_channel_type: &DataChannelType) {
        unsafe {
            ffi::dnv_vista_sdk_dcl_property_set_data_channel_type(
                self.0.as_ptr(),
                data_channel_type.as_ptr(),
            )
        };
    }

    /// Sets the format.
    pub fn set_format(&mut self, format: &Format) {
        unsafe { ffi::dnv_vista_sdk_dcl_property_set_format(self.0.as_ptr(), format.as_ptr()) };
    }

    /// Sets the range.
    pub fn set_range(&mut self, range: &Range) {
        unsafe { ffi::dnv_vista_sdk_dcl_property_set_range(self.0.as_ptr(), range.as_ptr()) };
    }

    /// Clears the range.
    pub fn clear_range(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_property_clear_range(self.0.as_ptr()) };
    }

    /// Sets the unit.
    pub fn set_unit(&mut self, unit: &Unit) {
        unsafe { ffi::dnv_vista_sdk_dcl_property_set_unit(self.0.as_ptr(), unit.as_ptr()) };
    }

    /// Clears the unit.
    pub fn clear_unit(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_property_clear_unit(self.0.as_ptr()) };
    }

    /// Returns the quality coding string if set.
    pub fn quality_coding(&self) -> Option<String> {
        unsafe {
            opt_str(ffi::dnv_vista_sdk_dcl_property_quality_coding(
                self.0.as_ptr(),
            ))
        }
    }

    /// Sets the quality coding string.
    pub fn set_quality_coding(&mut self, quality_coding: &str) {
        let c = CString::new(quality_coding).expect("quality_coding contains NUL byte");
        unsafe { ffi::dnv_vista_sdk_dcl_property_set_quality_coding(self.0.as_ptr(), c.as_ptr()) };
    }

    /// Clears the quality coding string.
    pub fn clear_quality_coding(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_property_clear_quality_coding(self.0.as_ptr()) };
    }

    /// Returns the alert priority string if set.
    pub fn alert_priority(&self) -> Option<String> {
        unsafe {
            opt_str(ffi::dnv_vista_sdk_dcl_property_alert_priority(
                self.0.as_ptr(),
            ))
        }
    }

    /// Sets the alert priority string.
    pub fn set_alert_priority(&mut self, alert_priority: &str) {
        let c = CString::new(alert_priority).expect("alert_priority contains NUL byte");
        unsafe { ffi::dnv_vista_sdk_dcl_property_set_alert_priority(self.0.as_ptr(), c.as_ptr()) };
    }

    /// Clears the alert priority string.
    pub fn clear_alert_priority(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_property_clear_alert_priority(self.0.as_ptr()) };
    }

    /// Returns the name string if set.
    pub fn name(&self) -> Option<String> {
        unsafe { opt_str(ffi::dnv_vista_sdk_dcl_property_name(self.0.as_ptr())) }
    }

    /// Sets the name string.
    pub fn set_name(&mut self, name: &str) {
        let c = CString::new(name).expect("name contains NUL byte");
        unsafe { ffi::dnv_vista_sdk_dcl_property_set_name(self.0.as_ptr(), c.as_ptr()) };
    }

    /// Clears the name string.
    pub fn clear_name(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_property_clear_name(self.0.as_ptr()) };
    }

    /// Returns the remarks string if set.
    pub fn remarks(&self) -> Option<String> {
        unsafe { opt_str(ffi::dnv_vista_sdk_dcl_property_remarks(self.0.as_ptr())) }
    }

    /// Sets the remarks string.
    pub fn set_remarks(&mut self, remarks: &str) {
        let c = CString::new(remarks).expect("remarks contains NUL byte");
        unsafe { ffi::dnv_vista_sdk_dcl_property_set_remarks(self.0.as_ptr(), c.as_ptr()) };
    }

    /// Clears the remarks string.
    pub fn clear_remarks(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_property_clear_remarks(self.0.as_ptr()) };
    }

    /// Sets the custom properties, transferring ownership of `doc`.
    pub fn set_custom_properties(&mut self, doc: SerializableDocument) {
        let raw = doc.into_raw();
        unsafe { ffi::dnv_vista_sdk_dcl_property_set_custom_properties(self.0.as_ptr(), raw) };
    }

    /// Clears the custom properties.
    pub fn clear_custom_properties(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_property_clear_custom_properties(self.0.as_ptr()) };
    }

    /// Returns `true` if this property is consistent and complete.
    pub fn validate(&self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_dcl_property_validate(self.0.as_ptr()) }) != 0
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_property_t {
        self.0.as_ptr()
    }
}

impl Drop for Property {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_property_free(self.0.as_ptr()) };
    }
}

/// ISO 19848 Table 11 - reference to a named configuration version.
pub struct ConfigurationReference(NonNull<ffi::dnv_vista_sdk_dcl_configuration_reference_t>);

impl ConfigurationReference {
    /// Creates a configuration reference with the given id and timestamp.
    pub fn new(id: &str, timestamp: DateTimeOffset) -> Self {
        let c = CString::new(id).expect("id contains NUL byte");
        let ptr = unsafe {
            ffi::dnv_vista_sdk_dcl_configuration_reference_create(c.as_ptr(), timestamp.into_ffi())
        };
        Self(NonNull::new(ptr).expect("configuration_reference_create returned null"))
    }

    /// Returns the configuration identifier.
    pub fn id(&self) -> String {
        unsafe {
            req_str(ffi::dnv_vista_sdk_dcl_configuration_reference_id(
                self.0.as_ptr(),
            ))
        }
    }

    /// Sets the configuration identifier.
    pub fn set_id(&mut self, id: &str) {
        let c = CString::new(id).expect("id contains NUL byte");
        unsafe {
            ffi::dnv_vista_sdk_dcl_configuration_reference_set_id(self.0.as_ptr(), c.as_ptr())
        };
    }

    /// Returns the version string if set.
    pub fn version(&self) -> Option<String> {
        unsafe {
            opt_str(ffi::dnv_vista_sdk_dcl_configuration_reference_version(
                self.0.as_ptr(),
            ))
        }
    }

    /// Sets the version string.
    pub fn set_version(&mut self, version: &str) {
        let c = CString::new(version).expect("version contains NUL byte");
        unsafe {
            ffi::dnv_vista_sdk_dcl_configuration_reference_set_version(self.0.as_ptr(), c.as_ptr())
        };
    }

    /// Clears the version string.
    pub fn clear_version(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_configuration_reference_clear_version(self.0.as_ptr()) };
    }

    /// Returns the timestamp.
    pub fn timestamp(&self) -> DateTimeOffset {
        DateTimeOffset::from_ffi(unsafe {
            ffi::dnv_vista_sdk_dcl_configuration_reference_timestamp(self.0.as_ptr())
        })
    }

    /// Sets the timestamp.
    pub fn set_timestamp(&mut self, timestamp: DateTimeOffset) {
        unsafe {
            ffi::dnv_vista_sdk_dcl_configuration_reference_set_timestamp(
                self.0.as_ptr(),
                timestamp.into_ffi(),
            )
        };
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_configuration_reference_t {
        self.0.as_ptr()
    }
}

impl Drop for ConfigurationReference {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_configuration_reference_free(self.0.as_ptr()) };
    }
}

/// ISO 19848 Table 12 - naming rule and scheme version for a data channel list.
pub struct VersionInformation(NonNull<ffi::dnv_vista_sdk_dcl_version_information_t>);

impl VersionInformation {
    /// Creates version information with no fields set.
    pub fn new() -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_version_information_create_default() };
        Self(NonNull::new(ptr).expect("version_information_create_default returned null"))
    }

    /// Creates version information with the given naming rule and scheme version.
    pub fn with_fields(naming_rule: &str, naming_scheme_version: &str) -> Self {
        let nr = CString::new(naming_rule).expect("naming_rule contains NUL byte");
        let nsv =
            CString::new(naming_scheme_version).expect("naming_scheme_version contains NUL byte");
        let ptr =
            unsafe { ffi::dnv_vista_sdk_dcl_version_information_create(nr.as_ptr(), nsv.as_ptr()) };
        Self(NonNull::new(ptr).expect("version_information_create returned null"))
    }

    /// Returns the naming rule.
    pub fn naming_rule(&self) -> String {
        unsafe {
            req_str(ffi::dnv_vista_sdk_dcl_version_information_naming_rule(
                self.0.as_ptr(),
            ))
        }
    }

    /// Sets the naming rule.
    pub fn set_naming_rule(&mut self, naming_rule: &str) {
        let c = CString::new(naming_rule).expect("naming_rule contains NUL byte");
        unsafe {
            ffi::dnv_vista_sdk_dcl_version_information_set_naming_rule(self.0.as_ptr(), c.as_ptr())
        };
    }

    /// Returns the naming scheme version.
    pub fn naming_scheme_version(&self) -> String {
        unsafe {
            req_str(
                ffi::dnv_vista_sdk_dcl_version_information_naming_scheme_version(self.0.as_ptr()),
            )
        }
    }

    /// Sets the naming scheme version.
    pub fn set_naming_scheme_version(&mut self, naming_scheme_version: &str) {
        let c =
            CString::new(naming_scheme_version).expect("naming_scheme_version contains NUL byte");
        unsafe {
            ffi::dnv_vista_sdk_dcl_version_information_set_naming_scheme_version(
                self.0.as_ptr(),
                c.as_ptr(),
            )
        };
    }

    /// Returns the reference URL if set.
    pub fn reference_url(&self) -> Option<String> {
        unsafe {
            opt_str(ffi::dnv_vista_sdk_dcl_version_information_reference_url(
                self.0.as_ptr(),
            ))
        }
    }

    /// Sets the reference URL.
    pub fn set_reference_url(&mut self, reference_url: &str) {
        let c = CString::new(reference_url).expect("reference_url contains NUL byte");
        unsafe {
            ffi::dnv_vista_sdk_dcl_version_information_set_reference_url(
                self.0.as_ptr(),
                c.as_ptr(),
            )
        };
    }

    /// Clears the reference URL.
    pub fn clear_reference_url(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_version_information_clear_reference_url(self.0.as_ptr()) };
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
        unsafe { ffi::dnv_vista_sdk_dcl_version_information_free(self.0.as_ptr()) };
    }
}

/// ISO 19848 Table 15 - unique identifier for a data channel.
pub struct DataChannelId(NonNull<ffi::dnv_vista_sdk_dcl_channel_id_t>);

impl DataChannelId {
    /// Creates a data channel identifier from the given Local ID.
    pub fn new(local_id: &LocalId) -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_channel_id_create(local_id.as_ffi_ptr()) };
        Self(NonNull::new(ptr).expect("channel_id_create returned null"))
    }

    /// Sets the Local ID.
    pub fn set_local_id(&mut self, local_id: &LocalId) {
        unsafe {
            ffi::dnv_vista_sdk_dcl_channel_id_set_local_id(self.0.as_ptr(), local_id.as_ffi_ptr())
        };
    }

    /// Returns the short identifier if set.
    pub fn short_id(&self) -> Option<String> {
        unsafe { opt_str(ffi::dnv_vista_sdk_dcl_channel_id_short_id(self.0.as_ptr())) }
    }

    /// Sets the short identifier.
    pub fn set_short_id(&mut self, short_id: &str) {
        let c = CString::new(short_id).expect("short_id contains NUL byte");
        unsafe { ffi::dnv_vista_sdk_dcl_channel_id_set_short_id(self.0.as_ptr(), c.as_ptr()) };
    }

    /// Clears the short identifier.
    pub fn clear_short_id(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_channel_id_clear_short_id(self.0.as_ptr()) };
    }

    /// Sets the name object.
    pub fn set_name_object(&mut self, name_object: &NameObject) {
        unsafe {
            ffi::dnv_vista_sdk_dcl_channel_id_set_name_object(self.0.as_ptr(), name_object.as_ptr())
        };
    }

    /// Clears the name object.
    pub fn clear_name_object(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_channel_id_clear_name_object(self.0.as_ptr()) };
    }

    /// Returns the string representation of the stored Local ID, or `None` if not set.
    pub fn local_id_string(&self) -> Option<String> {
        let local_id_ptr = unsafe { ffi::dnv_vista_sdk_dcl_channel_id_local_id(self.0.as_ptr()) };
        if local_id_ptr.is_null() {
            return None;
        }
        let s_ptr = unsafe { local_id_ffi::dnv_vista_sdk_local_id_to_string(local_id_ptr) };
        if s_ptr.is_null() {
            return None;
        }
        let s = unsafe { CStr::from_ptr(s_ptr) }
            .to_str()
            .expect("invalid UTF-8")
            .to_string();
        unsafe { dnv_vista_sdk_string_free(s_ptr) };
        Some(s)
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_channel_id_t {
        self.0.as_ptr()
    }
}

impl Drop for DataChannelId {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_channel_id_free(self.0.as_ptr()) };
    }
}

/// ISO 19848 Table 10 - header of a DataChannelList package.
pub struct Header(NonNull<ffi::dnv_vista_sdk_dcl_header_t>);

impl Header {
    /// Creates a header with the given ship identifier and data channel list ID.
    pub fn new(ship_id: &ShipId, data_channel_list_id: &ConfigurationReference) -> Self {
        let ptr = ship_id.with_ffi_ptr(|ship_ptr| unsafe {
            ffi::dnv_vista_sdk_dcl_header_create(ship_ptr, data_channel_list_id.as_ptr())
        });
        Self(NonNull::new(ptr).expect("header_create returned null"))
    }

    /// Sets the ship identifier.
    pub fn set_ship_id(&mut self, ship_id: &ShipId) {
        ship_id.with_ffi_ptr(|ship_ptr| unsafe {
            ffi::dnv_vista_sdk_dcl_header_set_ship_id(self.0.as_ptr(), ship_ptr)
        });
    }

    /// Sets the data channel list ID.
    pub fn set_data_channel_list_id(&mut self, data_channel_list_id: &ConfigurationReference) {
        unsafe {
            ffi::dnv_vista_sdk_dcl_header_set_data_channel_list_id(
                self.0.as_ptr(),
                data_channel_list_id.as_ptr(),
            )
        };
    }

    /// Sets the version information.
    pub fn set_version_information(&mut self, version_information: &VersionInformation) {
        unsafe {
            ffi::dnv_vista_sdk_dcl_header_set_version_information(
                self.0.as_ptr(),
                version_information.as_ptr(),
            )
        };
    }

    /// Clears the version information.
    pub fn clear_version_information(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_header_clear_version_information(self.0.as_ptr()) };
    }

    /// Returns the author string if set.
    pub fn author(&self) -> Option<String> {
        unsafe { opt_str(ffi::dnv_vista_sdk_dcl_header_author(self.0.as_ptr())) }
    }

    /// Sets the author string.
    pub fn set_author(&mut self, author: &str) {
        let c = CString::new(author).expect("author contains NUL byte");
        unsafe { ffi::dnv_vista_sdk_dcl_header_set_author(self.0.as_ptr(), c.as_ptr()) };
    }

    /// Clears the author string.
    pub fn clear_author(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_header_clear_author(self.0.as_ptr()) };
    }

    /// Returns the date-created timestamp if set.
    pub fn date_created(&self) -> Option<DateTimeOffset> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_header_has_date_created(self.0.as_ptr()) }) != 0 {
            Some(DateTimeOffset::from_ffi(unsafe {
                ffi::dnv_vista_sdk_dcl_header_date_created(self.0.as_ptr())
            }))
        } else {
            None
        }
    }

    /// Sets the date-created timestamp.
    pub fn set_date_created(&mut self, date_created: DateTimeOffset) {
        unsafe {
            ffi::dnv_vista_sdk_dcl_header_set_date_created(self.0.as_ptr(), date_created.into_ffi())
        };
    }

    /// Clears the date-created timestamp.
    pub fn clear_date_created(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_header_clear_date_created(self.0.as_ptr()) };
    }

    /// Sets the custom headers, transferring ownership of `doc`.
    pub fn set_custom_headers(&mut self, doc: SerializableDocument) {
        let raw = doc.into_raw();
        unsafe { ffi::dnv_vista_sdk_dcl_header_set_custom_headers(self.0.as_ptr(), raw) };
    }

    /// Clears the custom headers.
    pub fn clear_custom_headers(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_header_clear_custom_headers(self.0.as_ptr()) };
    }

    /// Returns `true` if the stored ship ID equals `ship_id`.
    pub fn ship_id_equals(&self, ship_id: &ShipId) -> bool {
        let stored = unsafe { ffi::dnv_vista_sdk_dcl_header_ship_id(self.0.as_ptr()) };
        if stored.is_null() {
            return false;
        }
        let stored_ship_id = read_ship_id_from_ffi(stored);
        stored_ship_id == *ship_id
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_header_t {
        self.0.as_ptr()
    }
}

impl Drop for Header {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_header_free(self.0.as_ptr()) };
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

    /// Sets the channel identifier.
    pub fn set_channel_id(&mut self, channel_id: &DataChannelId) {
        unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_set_channel_id(self.0.as_ptr(), channel_id.as_ptr())
        };
    }

    /// Sets the property.  Returns `false` if validation of the new property fails.
    pub fn set_property(&mut self, property: &Property) -> bool {
        (unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_set_property(self.0.as_ptr(), property.as_ptr())
        }) != 0
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_data_channel_t {
        self.0.as_ptr()
    }
}

impl Drop for DataChannel {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_data_channel_free(self.0.as_ptr()) };
    }
}

/// ISO 19848 Table 13 - ordered, indexed collection of [`DataChannel`] entries.
pub struct DataChannelList(NonNull<ffi::dnv_vista_sdk_dcl_data_channel_list_t>);

impl DataChannelList {
    /// Creates an empty data channel list.
    pub fn new() -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_data_channel_list_create() };
        Self(NonNull::new(ptr).expect("data_channel_list_create returned null"))
    }

    /// Returns the number of data channels in the list.
    pub fn len(&self) -> usize {
        unsafe { ffi::dnv_vista_sdk_dcl_data_channel_list_size(self.0.as_ptr()) }
    }

    /// Returns `true` if the list contains no data channels.
    pub fn is_empty(&self) -> bool {
        self.len() == 0
    }

    /// Returns a borrowed view of the data channel at `index`, or `None` if out of range.
    pub fn at(&self, index: usize) -> Option<DataChannelRef<'_>> {
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
    pub fn from_local_id(&self, local_id: &LocalId) -> Option<DataChannelRef<'_>> {
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
        (unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_list_add(self.0.as_ptr(), data_channel.as_ptr())
        }) != 0
    }

    /// Removes a data channel from the list.  Returns `false` if not found.
    pub fn remove(&mut self, data_channel: &DataChannel) -> bool {
        (unsafe {
            ffi::dnv_vista_sdk_dcl_data_channel_list_remove(self.0.as_ptr(), data_channel.as_ptr())
        }) != 0
    }

    /// Removes all data channels from the list.
    pub fn clear(&mut self) {
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
        unsafe { ffi::dnv_vista_sdk_dcl_data_channel_list_free(self.0.as_ptr()) };
    }
}

/// Borrowed view of a [`DataChannel`] obtained from [`DataChannelList`] accessor methods.
pub struct DataChannelRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_data_channel_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_data_channel_t>,
}

impl<'a> DataChannelRef<'a> {
    /// Returns a borrowed view of the channel identifier.
    pub fn channel_id(&self) -> DataChannelIdRef<'_> {
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_data_channel_channel_id(self.ptr) };
        DataChannelIdRef {
            ptr,
            _marker: std::marker::PhantomData,
        }
    }

    /// Returns a borrowed view of the property.
    pub fn property(&self) -> PropertyRef<'_> {
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_data_channel_property(self.ptr) };
        PropertyRef {
            ptr,
            _marker: std::marker::PhantomData,
        }
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
        unsafe { opt_str(ffi::dnv_vista_sdk_dcl_channel_id_short_id(self.ptr)) }
    }

    /// Returns the string representation of the local id if set.
    pub fn local_id_string(&self) -> Option<String> {
        let local_id_ptr = unsafe { ffi::dnv_vista_sdk_dcl_channel_id_local_id(self.ptr) };
        if local_id_ptr.is_null() {
            return None;
        }
        let s_ptr = unsafe { local_id_ffi::dnv_vista_sdk_local_id_to_string(local_id_ptr) };
        if s_ptr.is_null() {
            return None;
        }
        let s = unsafe {
            std::ffi::CStr::from_ptr(s_ptr)
                .to_str()
                .expect("invalid UTF-8")
                .to_owned()
        };
        unsafe { dnv_vista_sdk_string_free(s_ptr) };
        Some(s)
    }

    /// Returns a borrowed view of the name object if set.
    pub fn name_object(&self) -> Option<NameObjectRef<'_>> {
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

/// Borrowed view of a [`NameObject`] obtained via [`DataChannelIdRef::name_object`].
pub struct NameObjectRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_name_object_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_name_object_t>,
}

impl<'a> NameObjectRef<'a> {
    /// Returns the naming rule string.
    pub fn naming_rule(&self) -> String {
        unsafe { req_str(ffi::dnv_vista_sdk_dcl_name_object_naming_rule(self.ptr)) }
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
        unsafe { opt_str(ffi::dnv_vista_sdk_dcl_property_name(self.ptr)) }
    }

    /// Returns the remarks string if set.
    pub fn remarks(&self) -> Option<String> {
        unsafe { opt_str(ffi::dnv_vista_sdk_dcl_property_remarks(self.ptr)) }
    }

    /// Returns a borrowed view of the data channel type.
    pub fn data_channel_type(&self) -> DataChannelTypeRef<'_> {
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_property_data_channel_type(self.ptr) };
        DataChannelTypeRef {
            ptr,
            _marker: std::marker::PhantomData,
        }
    }

    /// Returns a borrowed view of the format.
    pub fn format(&self) -> FormatRef<'_> {
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_property_format(self.ptr) };
        FormatRef {
            ptr,
            _marker: std::marker::PhantomData,
        }
    }

    /// Returns a borrowed view of the range if set.
    pub fn range(&self) -> Option<RangeRef<'_>> {
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

/// Borrowed view of a [`DataChannelType`] obtained via [`PropertyRef::data_channel_type`].
pub struct DataChannelTypeRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_data_channel_type_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_data_channel_type_t>,
}

impl<'a> DataChannelTypeRef<'a> {
    /// Returns the type string.
    pub fn type_(&self) -> String {
        unsafe { req_str(ffi::dnv_vista_sdk_dcl_data_channel_type_type(self.ptr)) }
    }

    /// Returns the update cycle in seconds if set.
    pub fn update_cycle(&self) -> Option<f64> {
        if (unsafe { ffi::dnv_vista_sdk_dcl_data_channel_type_has_update_cycle(self.ptr) }) != 0 {
            Some(unsafe { ffi::dnv_vista_sdk_dcl_data_channel_type_update_cycle(self.ptr) })
        } else {
            None
        }
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
        unsafe { req_str(ffi::dnv_vista_sdk_dcl_format_type(self.ptr)) }
    }

    /// Returns a borrowed view of the restriction if one is set.
    pub fn restriction(&self) -> Option<RestrictionRef<'_>> {
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

/// Borrowed view of a [`Range`] obtained via [`PropertyRef::range`].
pub struct RangeRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_range_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_range_t>,
}

impl<'a> RangeRef<'a> {
    /// Returns the lower bound.
    pub fn low(&self) -> f64 {
        unsafe { ffi::dnv_vista_sdk_dcl_range_low(self.ptr) }
    }

    /// Returns the upper bound.
    pub fn high(&self) -> f64 {
        unsafe { ffi::dnv_vista_sdk_dcl_range_high(self.ptr) }
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
        unsafe { req_str(ffi::dnv_vista_sdk_dcl_unit_unit_symbol(self.ptr)) }
    }

    /// Returns the quantity name if set.
    pub fn quantity_name(&self) -> Option<String> {
        unsafe { opt_str(ffi::dnv_vista_sdk_dcl_unit_quantity_name(self.ptr)) }
    }
}

/// ISO 19848 Table 9 - package combining a [`Header`] and a [`DataChannelList`].
pub struct Package(NonNull<ffi::dnv_vista_sdk_dcl_package_t>);

impl Package {
    /// Creates a package from the given header and data channel list.
    pub fn new(header: &Header, data_channel_list: &DataChannelList) -> Self {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_dcl_package_create(header.as_ptr(), data_channel_list.as_ptr())
        };
        Self(NonNull::new(ptr).expect("package_create returned null"))
    }

    /// Sets the header.
    pub fn set_header(&mut self, header: &Header) {
        unsafe { ffi::dnv_vista_sdk_dcl_package_set_header(self.0.as_ptr(), header.as_ptr()) };
    }

    /// Sets the data channel list.
    pub fn set_data_channel_list(&mut self, data_channel_list: &DataChannelList) {
        unsafe {
            ffi::dnv_vista_sdk_dcl_package_set_data_channel_list(
                self.0.as_ptr(),
                data_channel_list.as_ptr(),
            )
        };
    }

    /// Returns `true` if this package has a header set.
    pub fn has_header(&self) -> bool {
        !(unsafe { ffi::dnv_vista_sdk_dcl_package_header(self.0.as_ptr()) }).is_null()
    }

    /// Returns `true` if this package has a data channel list set.
    pub fn has_data_channel_list(&self) -> bool {
        !(unsafe { ffi::dnv_vista_sdk_dcl_package_data_channel_list(self.0.as_ptr()) }).is_null()
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::dnv_vista_sdk_dcl_package_t {
        self.0.as_ptr()
    }
}

impl Drop for Package {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_package_free(self.0.as_ptr()) };
    }
}

/// ISO 19848 DataChannelList package - top-level container for transmission.
pub struct DataChannelListPackage(NonNull<ffi::dnv_vista_sdk_dcl_list_package_t>);

impl DataChannelListPackage {
    /// Creates a DataChannelList package from the given package.
    pub fn new(package: &Package) -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_list_package_create(package.as_ptr()) };
        Self(NonNull::new(ptr).expect("list_package_create returned null"))
    }

    /// Sets the inner package.
    pub fn set_package(&mut self, package: &Package) {
        unsafe {
            ffi::dnv_vista_sdk_dcl_list_package_set_package(self.0.as_ptr(), package.as_ptr())
        };
    }

    /// Returns `true` if this list package has an inner package set.
    pub fn has_package(&self) -> bool {
        !(unsafe { ffi::dnv_vista_sdk_dcl_list_package_package(self.0.as_ptr()) }).is_null()
    }

    /// Returns a borrowed view of the data channel list, or `None` if not set.
    pub fn data_channel_list(&self) -> Option<DataChannelListRef<'_>> {
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
    pub(crate) unsafe fn from_raw(ptr: *mut ffi::dnv_vista_sdk_dcl_list_package_t) -> Self {
        Self(NonNull::new(ptr).expect("from_raw called with null pointer"))
    }
}

impl Drop for DataChannelListPackage {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_dcl_list_package_free(self.0.as_ptr()) };
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

/// Borrowed view of a [`Header`] obtained via [`PackageRef::header`].
pub struct HeaderRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_dcl_header_t,
    _marker: std::marker::PhantomData<&'a ffi::dnv_vista_sdk_dcl_header_t>,
}

impl<'a> HeaderRef<'a> {
    /// Returns `true` if the ship ID is set.
    pub fn ship_id_is_some(&self) -> bool {
        !(unsafe { ffi::dnv_vista_sdk_dcl_header_ship_id(self.ptr) }).is_null()
    }

    /// Returns a borrowed view of the data channel list configuration reference.
    pub fn data_channel_list_id(&self) -> ConfigurationReferenceRef<'_> {
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_header_data_channel_list_id(self.ptr) };
        ConfigurationReferenceRef {
            ptr,
            _marker: std::marker::PhantomData,
        }
    }

    /// Returns a borrowed view of the version information if set.
    pub fn version_information(&self) -> Option<VersionInformationRef<'_>> {
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
        let ptr = unsafe { ffi::dnv_vista_sdk_dcl_header_custom_headers(self.ptr) };
        if ptr.is_null() {
            None
        } else {
            Some(SerializableDocumentRef::from_ptr(ptr))
        }
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
        unsafe { req_str(ffi::dnv_vista_sdk_dcl_configuration_reference_id(self.ptr)) }
    }

    /// Returns the timestamp.
    pub fn timestamp(&self) -> DateTimeOffset {
        DateTimeOffset::from_ffi(unsafe {
            ffi::dnv_vista_sdk_dcl_configuration_reference_timestamp(self.ptr)
        })
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
        unsafe {
            req_str(ffi::dnv_vista_sdk_dcl_version_information_naming_rule(
                self.ptr,
            ))
        }
    }

    /// Returns the naming scheme version if set.
    pub fn naming_scheme_version(&self) -> Option<String> {
        unsafe {
            opt_str(ffi::dnv_vista_sdk_dcl_version_information_naming_scheme_version(self.ptr))
        }
    }

    /// Returns the reference URL if set.
    pub fn reference_url(&self) -> Option<String> {
        unsafe {
            opt_str(ffi::dnv_vista_sdk_dcl_version_information_reference_url(
                self.ptr,
            ))
        }
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
        unsafe { ffi::dnv_vista_sdk_dcl_data_channel_list_size(self.ptr) }
    }

    /// Returns `true` if the list contains no data channels.
    pub fn is_empty(&self) -> bool {
        self.len() == 0
    }

    /// Returns a borrowed view of the data channel at `index`, or `None` if out of range.
    pub fn at(&self, index: usize) -> Option<DataChannelRef<'_>> {
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
}
