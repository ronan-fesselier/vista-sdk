use std::ffi::CString;

use crate::core::error::{last_error, VistaError};
use crate::ffi::core::imo_number as ffi;

/// An International Maritime Organization (IMO) number: a unique seven-digit
/// identifier assigned to maritime vessels, with a check digit for validation.
#[derive(Clone, Copy, PartialEq, Eq, Hash)]
pub struct ImoNumber(pub(crate) u32);

impl ImoNumber {
    /// Checks if an integer value represents a valid IMO number
    /// (7-digit structure and checksum), without constructing one.
    pub fn is_valid(imo_number: i32) -> bool {
        unsafe { ffi::dnv_vista_sdk_imo_number_is_valid(imo_number) != 0 }
    }

    /// Constructs an IMO number from an integer. Returns `Err` if `value` fails checksum validation.
    pub fn create(value: i32) -> Result<Self, VistaError> {
        let ptr = unsafe { ffi::dnv_vista_sdk_imo_number_create(value) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            let v = unsafe { ffi::dnv_vista_sdk_imo_number_value(ptr) };
            unsafe { ffi::dnv_vista_sdk_imo_number_free(ptr) };
            Ok(ImoNumber(v as u32))
        }
    }

    /// Parses an IMO number from a string, with or without the `"IMO"` prefix
    /// (e.g. `"IMO9074729"` or `"9074729"`). Returns `None` on invalid format or checksum.
    pub fn from_str(value: &str) -> Option<Self> {
        let c_value = CString::new(value).ok()?;
        let ptr = unsafe { ffi::dnv_vista_sdk_imo_number_from_string(c_value.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            let v = unsafe { ffi::dnv_vista_sdk_imo_number_value(ptr) };
            unsafe { ffi::dnv_vista_sdk_imo_number_free(ptr) };
            Some(ImoNumber(v as u32))
        }
    }

    /// Parses an IMO number from a string. Returns `Err` if the format or checksum is invalid.
    pub fn create_from_str(value: &str) -> Result<Self, VistaError> {
        let c_value = CString::new(value).map_err(|_| VistaError {
            kind: crate::core::error::ErrorKind::InvalidArgument,
            message: "value contains a NUL byte".to_string(),
        })?;
        let ptr = unsafe { ffi::dnv_vista_sdk_imo_number_create_from_string(c_value.as_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            let v = unsafe { ffi::dnv_vista_sdk_imo_number_value(ptr) };
            unsafe { ffi::dnv_vista_sdk_imo_number_free(ptr) };
            Ok(ImoNumber(v as u32))
        }
    }

    /// Raw seven-digit integer value (e.g. `9074729`).
    pub fn value(&self) -> u32 {
        self.0
    }
}

impl std::fmt::Display for ImoNumber {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "IMO{}", self.0)
    }
}

impl std::fmt::Debug for ImoNumber {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "ImoNumber({})", self)
    }
}
