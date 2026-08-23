use std::ffi::{CStr, CString};

use crate::core::error::{last_error, VistaError};
use crate::core::imo_number::ImoNumber;
use crate::ffi::core::imo_number as imo_ffi;
use crate::ffi::transport::ship_id as ffi;
use crate::ffi::transport::ship_id::dnv_vista_sdk_ship_id_t;

/// ISO 19848 ship identifier: either a validated IMO number or an alternative string.
#[derive(Clone, PartialEq, Eq, Debug)]
pub enum ShipId {
    /// IMO vessel number (preferred, validated identifier).
    Imo(ImoNumber),
    /// Alternative identifier for vessels without an IMO number.
    Other(String),
}

impl ShipId {
    /// Constructs a ShipId from an IMO number.
    pub fn from_imo_number(imo_number: ImoNumber) -> Self {
        ShipId::Imo(imo_number)
    }

    /// Constructs a ShipId from an alternative identifier string.
    /// Returns `Err` if `other_id` is empty.
    pub fn from_other_id(other_id: &str) -> Result<Self, VistaError> {
        let c_str = CString::new(other_id).expect("other_id contains a NUL byte");
        let ptr = unsafe { ffi::dnv_vista_sdk_ship_id_from_other_id(c_str.as_ptr()) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            unsafe { ffi::dnv_vista_sdk_ship_id_free(ptr) };
            Ok(ShipId::Other(other_id.to_string()))
        }
    }

    /// Parses a ShipId from its string representation.
    ///
    /// Strings starting with `"IMO"` (case-insensitive) followed by a valid IMO number
    /// produce `ShipId::Imo`; all other non-empty strings produce `ShipId::Other`.
    /// Returns `None` if `value` is empty.
    pub fn from_string(value: &str) -> Option<Self> {
        let c_str = CString::new(value).ok()?;
        let ptr = unsafe { ffi::dnv_vista_sdk_ship_id_from_string(c_str.as_ptr()) };
        if ptr.is_null() {
            return None;
        }
        let result = if unsafe { ffi::dnv_vista_sdk_ship_id_is_imo_number(ptr) } != 0 {
            let imo_ptr = unsafe { ffi::dnv_vista_sdk_ship_id_imo_number(ptr) };
            let v = unsafe { imo_ffi::dnv_vista_sdk_imo_number_value(imo_ptr) };
            unsafe { imo_ffi::dnv_vista_sdk_imo_number_free(imo_ptr) };
            ShipId::Imo(ImoNumber(v as u32))
        } else {
            let raw = unsafe { ffi::dnv_vista_sdk_ship_id_other_id(ptr) };
            let s = unsafe { CStr::from_ptr(raw) }
                .to_str()
                .expect("invalid UTF-8 in ShipId other_id")
                .to_string();
            ShipId::Other(s)
        };
        unsafe { ffi::dnv_vista_sdk_ship_id_free(ptr) };
        Some(result)
    }

    /// Returns the IMO number if this is a `ShipId::Imo`, otherwise `None`.
    pub fn imo_number(&self) -> Option<ImoNumber> {
        match self {
            ShipId::Imo(imo) => Some(*imo),
            ShipId::Other(_) => None,
        }
    }

    /// Returns the alternative identifier if this is a `ShipId::Other`, otherwise `None`.
    pub fn other_id(&self) -> Option<&str> {
        match self {
            ShipId::Imo(_) => None,
            ShipId::Other(s) => Some(s.as_str()),
        }
    }

    /// Returns `true` if this ShipId holds an IMO number.
    pub fn is_imo_number(&self) -> bool {
        matches!(self, ShipId::Imo(_))
    }

    /// Returns `true` if this ShipId holds an alternative identifier.
    pub fn is_other_id(&self) -> bool {
        matches!(self, ShipId::Other(_))
    }

    /// Constructs a `ShipId` from a borrowed C pointer returned by the C API.
    pub(crate) fn from_ffi_ptr(ptr: *const dnv_vista_sdk_ship_id_t) -> Self {
        if unsafe { ffi::dnv_vista_sdk_ship_id_is_imo_number(ptr) } != 0 {
            let imo_ptr = unsafe { ffi::dnv_vista_sdk_ship_id_imo_number(ptr) };
            let v = unsafe { imo_ffi::dnv_vista_sdk_imo_number_value(imo_ptr) };
            unsafe { imo_ffi::dnv_vista_sdk_imo_number_free(imo_ptr) };
            ShipId::Imo(ImoNumber(v as u32))
        } else {
            let raw = unsafe { ffi::dnv_vista_sdk_ship_id_other_id(ptr) };
            let s = unsafe { CStr::from_ptr(raw) }
                .to_str()
                .expect("invalid UTF-8 in ShipId other_id")
                .to_string();
            ShipId::Other(s)
        }
    }

    pub(crate) fn with_ffi_ptr<F, R>(&self, f: F) -> R
    where
        F: FnOnce(*const dnv_vista_sdk_ship_id_t) -> R,
    {
        let ptr = match self {
            ShipId::Imo(imo) => {
                let imo_ptr =
                    unsafe { imo_ffi::dnv_vista_sdk_imo_number_create(imo.value() as i32) };
                let ship_ptr = unsafe { ffi::dnv_vista_sdk_ship_id_from_imo_number(imo_ptr) };
                unsafe { imo_ffi::dnv_vista_sdk_imo_number_free(imo_ptr) };
                ship_ptr
            }
            ShipId::Other(s) => {
                let c = CString::new(s.as_str()).expect("ShipId::Other contains NUL byte");
                unsafe { ffi::dnv_vista_sdk_ship_id_from_other_id(c.as_ptr()) }
            }
        };
        let result = f(ptr);
        unsafe { ffi::dnv_vista_sdk_ship_id_free(ptr) };
        result
    }
}

impl std::fmt::Display for ShipId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            ShipId::Imo(imo) => write!(f, "{}", imo),
            ShipId::Other(s) => f.write_str(s),
        }
    }
}
