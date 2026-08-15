use std::ffi::{CStr, CString};

use crate::core::codebook_name::{codebook_names, CodebookName};
use crate::core::metadata_tag::OwnedMetadataTag;
use crate::ffi::core::codebook as ffi;

/// Result of validating a position string against the Position codebook.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum PositionValidationResult {
    /// Invalid format, whitespace, or non-ISO characters.
    Invalid,
    /// Numbers not at end or not alphabetically sorted.
    InvalidOrder,
    /// Duplicate groups (except the default group).
    InvalidGrouping,
    /// Standard value, number, or valid composite position.
    Valid,
    /// Custom value not in the standard codebook.
    Custom,
}

/// A VIS codebook containing standard values and validation logic.
///
/// Borrowed from the parent [`Codebooks`](crate::core::codebooks::Codebooks), valid as long as the parent is valid.
#[repr(transparent)]
pub struct Codebook(ffi::dnv_vista_sdk_codebook_t);

impl Codebook {
    pub(crate) fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_codebook_t) -> &'a Self {
        assert!(!ptr.is_null(), "dnv_vista_sdk_codebooks_at returned NULL");
        unsafe { &*(ptr as *const Codebook) }
    }

    /// Returns the name of this codebook.
    pub fn name(&self) -> CodebookName {
        let raw = unsafe {
            ffi::dnv_vista_sdk_codebook_name(
                self as *const Self as *const ffi::dnv_vista_sdk_codebook_t,
            )
        };
        codebook_names::from_ffi(raw)
    }

    /// Returns the number of standard values in this codebook.
    pub fn standard_values_count(&self) -> usize {
        unsafe {
            ffi::dnv_vista_sdk_codebook_standard_values_count(
                self as *const Self as *const ffi::dnv_vista_sdk_codebook_t,
            )
        }
    }

    /// Returns the standard value at `index`, or `None` if out of range.
    pub fn standard_value_at(&self, index: usize) -> Option<&str> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_codebook_standard_value_at(
                self as *const Self as *const ffi::dnv_vista_sdk_codebook_t,
                index,
            )
        };
        if ptr.is_null() {
            return None;
        }
        Some(
            unsafe { CStr::from_ptr(ptr) }
                .to_str()
                .expect("invalid UTF-8 in standard value"),
        )
    }

    /// Returns the number of groups in this codebook.
    pub fn groups_count(&self) -> usize {
        unsafe {
            ffi::dnv_vista_sdk_codebook_groups_count(
                self as *const Self as *const ffi::dnv_vista_sdk_codebook_t,
            )
        }
    }

    /// Returns the group name at `index`, or `None` if out of range.
    pub fn group_at(&self, index: usize) -> Option<&str> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_codebook_group_at(
                self as *const Self as *const ffi::dnv_vista_sdk_codebook_t,
                index,
            )
        };
        if ptr.is_null() {
            return None;
        }
        Some(
            unsafe { CStr::from_ptr(ptr) }
                .to_str()
                .expect("invalid UTF-8 in group name"),
        )
    }

    /// Returns `true` if `group` exists in this codebook.
    pub fn has_group(&self, group: &str) -> bool {
        let Ok(c_group) = CString::new(group) else {
            return false;
        };
        unsafe {
            ffi::dnv_vista_sdk_codebook_has_group(
                self as *const Self as *const ffi::dnv_vista_sdk_codebook_t,
                c_group.as_ptr(),
            ) != 0
        }
    }

    /// Validates a position string according to VIS position rules.
    ///
    /// Only meaningful for the Position codebook.
    pub fn validate_position(&self, position: &str) -> PositionValidationResult {
        let Ok(c_pos) = CString::new(position) else {
            return PositionValidationResult::Invalid;
        };
        let raw = unsafe {
            ffi::dnv_vista_sdk_codebook_validate_position(
                self as *const Self as *const ffi::dnv_vista_sdk_codebook_t,
                c_pos.as_ptr(),
            )
        };
        match raw {
            ffi::dnv_vista_sdk_position_validation_result_t::Invalid => {
                PositionValidationResult::Invalid
            }
            ffi::dnv_vista_sdk_position_validation_result_t::InvalidOrder => {
                PositionValidationResult::InvalidOrder
            }
            ffi::dnv_vista_sdk_position_validation_result_t::InvalidGrouping => {
                PositionValidationResult::InvalidGrouping
            }
            ffi::dnv_vista_sdk_position_validation_result_t::Valid => {
                PositionValidationResult::Valid
            }
            ffi::dnv_vista_sdk_position_validation_result_t::Custom => {
                PositionValidationResult::Custom
            }
        }
    }

    /// Returns `true` if `value` is a standard value in this codebook.
    pub fn has_standard_value(&self, value: &str) -> bool {
        let Ok(c_value) = CString::new(value) else {
            return false;
        };
        unsafe {
            ffi::dnv_vista_sdk_codebook_has_standard_value(
                self as *const Self as *const ffi::dnv_vista_sdk_codebook_t,
                c_value.as_ptr(),
            ) != 0
        }
    }

    /// Creates a metadata tag for `value`, validating it against this codebook.
    ///
    /// Returns `None` if the value is invalid for this codebook.
    pub fn create_tag(&self, value: &str) -> Option<OwnedMetadataTag> {
        let c_value = CString::new(value).ok()?;
        let ptr = unsafe {
            ffi::dnv_vista_sdk_codebook_create_tag(
                self as *const Self as *const ffi::dnv_vista_sdk_codebook_t,
                c_value.as_ptr(),
            )
        };
        if ptr.is_null() {
            None
        } else {
            Some(OwnedMetadataTag(ptr))
        }
    }
}
