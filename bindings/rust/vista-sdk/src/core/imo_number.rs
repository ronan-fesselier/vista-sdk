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
        // SAFETY: pure function, no pointer arguments.
        unsafe { ffi::dnv_vista_sdk_imo_number_is_valid(imo_number) != 0 }
    }

    /// Constructs an IMO number from an integer. Returns `Err` if `value` fails checksum validation.
    pub fn create(value: i32) -> Result<Self, VistaError> {
        // SAFETY: no pointer arguments. Returns an owned pointer or NULL.
        let ptr = unsafe { ffi::dnv_vista_sdk_imo_number_create(value) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            // SAFETY: ptr is non-null, freed exactly once right after reading the value.
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

impl std::str::FromStr for ImoNumber {
    type Err = VistaError;
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let c = CString::new(s).map_err(|_| VistaError {
            kind: crate::core::error::ErrorKind::InvalidArgument,
            message: "value contains a NUL byte".to_string(),
        })?;
        // SAFETY: c is a valid NUL-terminated C string. Returns an owned pointer or NULL.
        let ptr = unsafe { ffi::dnv_vista_sdk_imo_number_from_string(c.as_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            // SAFETY: ptr is non-null, freed exactly once right after reading the value.
            let v = unsafe { ffi::dnv_vista_sdk_imo_number_value(ptr) };
            unsafe { ffi::dnv_vista_sdk_imo_number_free(ptr) };
            Ok(ImoNumber(v as u32))
        }
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
