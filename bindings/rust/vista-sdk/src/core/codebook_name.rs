use std::ffi::{CStr, CString};

use crate::ffi::core::codebook_name as ffi;

/// Enumeration of VIS codebook types.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
#[repr(u8)]
pub enum CodebookName {
    Quantity = 1,
    Content,
    Calculation,
    State,
    Command,
    Type,
    FunctionalServices,
    MaintenanceCategory,
    ActivityType,
    Position,
    Detail,
}

/// Utility functions for converting between `CodebookName` values and string prefixes.
pub mod codebook_names {
    use super::*;

    /// Converts a string prefix to a `CodebookName`.
    ///
    /// Returns `None` if the prefix is not recognized.
    pub fn from_prefix(prefix: &str) -> Option<CodebookName> {
        let c_prefix = CString::new(prefix).ok()?;
        let mut out = ffi::dnv_vista_sdk_codebook_name_t::Quantity;
        let ok =
            unsafe { ffi::dnv_vista_sdk_codebook_names_from_prefix(c_prefix.as_ptr(), &mut out) };
        if ok == 0 {
            return None;
        }
        Some(from_ffi(out))
    }

    /// Converts a `CodebookName` to its string prefix (e.g. `"qty"` for `Quantity`).
    pub fn to_prefix(name: CodebookName) -> &'static str {
        let ptr = unsafe { ffi::dnv_vista_sdk_codebook_names_to_prefix(to_ffi(name)) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_codebook_names_to_prefix returned NULL"
        );
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in codebook prefix")
    }

    pub(crate) fn to_string(name: CodebookName) -> &'static str {
        let ptr = unsafe { ffi::dnv_vista_sdk_codebook_names_to_string(to_ffi(name)) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_codebook_names_to_string returned NULL"
        );
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in codebook name")
    }

    pub(crate) fn to_ffi(name: CodebookName) -> ffi::dnv_vista_sdk_codebook_name_t {
        match name {
            CodebookName::Quantity => ffi::dnv_vista_sdk_codebook_name_t::Quantity,
            CodebookName::Content => ffi::dnv_vista_sdk_codebook_name_t::Content,
            CodebookName::Calculation => ffi::dnv_vista_sdk_codebook_name_t::Calculation,
            CodebookName::State => ffi::dnv_vista_sdk_codebook_name_t::State,
            CodebookName::Command => ffi::dnv_vista_sdk_codebook_name_t::Command,
            CodebookName::Type => ffi::dnv_vista_sdk_codebook_name_t::Type,
            CodebookName::FunctionalServices => {
                ffi::dnv_vista_sdk_codebook_name_t::FunctionalServices
            }
            CodebookName::MaintenanceCategory => {
                ffi::dnv_vista_sdk_codebook_name_t::MaintenanceCategory
            }
            CodebookName::ActivityType => ffi::dnv_vista_sdk_codebook_name_t::ActivityType,
            CodebookName::Position => ffi::dnv_vista_sdk_codebook_name_t::Position,
            CodebookName::Detail => ffi::dnv_vista_sdk_codebook_name_t::Detail,
        }
    }

    pub(crate) fn from_ffi(name: ffi::dnv_vista_sdk_codebook_name_t) -> CodebookName {
        match name {
            ffi::dnv_vista_sdk_codebook_name_t::Quantity => CodebookName::Quantity,
            ffi::dnv_vista_sdk_codebook_name_t::Content => CodebookName::Content,
            ffi::dnv_vista_sdk_codebook_name_t::Calculation => CodebookName::Calculation,
            ffi::dnv_vista_sdk_codebook_name_t::State => CodebookName::State,
            ffi::dnv_vista_sdk_codebook_name_t::Command => CodebookName::Command,
            ffi::dnv_vista_sdk_codebook_name_t::Type => CodebookName::Type,
            ffi::dnv_vista_sdk_codebook_name_t::FunctionalServices => {
                CodebookName::FunctionalServices
            }
            ffi::dnv_vista_sdk_codebook_name_t::MaintenanceCategory => {
                CodebookName::MaintenanceCategory
            }
            ffi::dnv_vista_sdk_codebook_name_t::ActivityType => CodebookName::ActivityType,
            ffi::dnv_vista_sdk_codebook_name_t::Position => CodebookName::Position,
            ffi::dnv_vista_sdk_codebook_name_t::Detail => CodebookName::Detail,
        }
    }
}

#[cfg(test)]
mod discriminant_sync {
    use super::*;

    #[test]
    fn codebook_name_discriminants_match_ffi() {
        assert_eq!(
            CodebookName::Quantity as u8,
            ffi::dnv_vista_sdk_codebook_name_t::Quantity as u8
        );
        assert_eq!(
            CodebookName::Content as u8,
            ffi::dnv_vista_sdk_codebook_name_t::Content as u8
        );
        assert_eq!(
            CodebookName::Calculation as u8,
            ffi::dnv_vista_sdk_codebook_name_t::Calculation as u8
        );
        assert_eq!(
            CodebookName::State as u8,
            ffi::dnv_vista_sdk_codebook_name_t::State as u8
        );
        assert_eq!(
            CodebookName::Command as u8,
            ffi::dnv_vista_sdk_codebook_name_t::Command as u8
        );
        assert_eq!(
            CodebookName::Type as u8,
            ffi::dnv_vista_sdk_codebook_name_t::Type as u8
        );
        assert_eq!(
            CodebookName::FunctionalServices as u8,
            ffi::dnv_vista_sdk_codebook_name_t::FunctionalServices as u8
        );
        assert_eq!(
            CodebookName::MaintenanceCategory as u8,
            ffi::dnv_vista_sdk_codebook_name_t::MaintenanceCategory as u8
        );
        assert_eq!(
            CodebookName::ActivityType as u8,
            ffi::dnv_vista_sdk_codebook_name_t::ActivityType as u8
        );
        assert_eq!(
            CodebookName::Position as u8,
            ffi::dnv_vista_sdk_codebook_name_t::Position as u8
        );
        assert_eq!(
            CodebookName::Detail as u8,
            ffi::dnv_vista_sdk_codebook_name_t::Detail as u8
        );
    }
}

impl std::fmt::Display for CodebookName {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(codebook_names::to_string(*self))
    }
}
