use std::ffi::CStr;
use std::ops::Index;
use std::str::FromStr;

use crate::core::codebook::Codebook;
use crate::core::codebook_name::{codebook_names, CodebookName};
use crate::core::vis_version::VisVersion;
use crate::ffi::core::codebooks as ffi;

/// Container for all VIS codebooks for a specific version.
///
/// Borrowed from the VIS singleton, valid for the lifetime of the program.
#[repr(transparent)]
pub struct Codebooks(ffi::dnv_vista_sdk_codebooks_t);

impl Codebooks {
    pub(crate) fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_codebooks_t) -> &'a Self {
        assert!(!ptr.is_null(), "dnv_vista_sdk_vis_codebooks returned NULL");
        unsafe { &*(ptr as *const Codebooks) }
    }

    /// Returns the VIS version these codebooks belong to.
    pub fn version(&self) -> VisVersion {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_codebooks_version(
                self as *const Self as *const ffi::dnv_vista_sdk_codebooks_t,
            )
        };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_codebooks_version returned NULL"
        );
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in codebooks version");
        VisVersion::from_str(s).expect("unrecognized codebooks version string")
    }
}

impl Index<CodebookName> for Codebooks {
    type Output = Codebook;

    /// Returns the codebook for `name`.
    fn index(&self, name: CodebookName) -> &Codebook {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_codebooks_at(
                self as *const Self as *const ffi::dnv_vista_sdk_codebooks_t,
                codebook_names::to_ffi(name),
            )
        };
        Codebook::from_ptr(ptr)
    }
}
