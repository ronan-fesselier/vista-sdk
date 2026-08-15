use std::ffi::c_char;

use super::codebook::dnv_vista_sdk_codebook_t;
use super::codebook_name::dnv_vista_sdk_codebook_name_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_codebooks_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_codebooks_version(
        codebooks: *const dnv_vista_sdk_codebooks_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_codebooks_at(
        codebooks: *const dnv_vista_sdk_codebooks_t,
        name: dnv_vista_sdk_codebook_name_t,
    ) -> *const dnv_vista_sdk_codebook_t;
}
