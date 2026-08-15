use std::ffi::{c_char, c_int};

use super::codebook_name::dnv_vista_sdk_codebook_name_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_metadata_tag_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_metadata_tag_free(tag: *mut dnv_vista_sdk_metadata_tag_t);
    pub(crate) fn dnv_vista_sdk_metadata_tag_name(
        tag: *const dnv_vista_sdk_metadata_tag_t,
    ) -> dnv_vista_sdk_codebook_name_t;
    pub(crate) fn dnv_vista_sdk_metadata_tag_value(
        tag: *const dnv_vista_sdk_metadata_tag_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_metadata_tag_prefix(
        tag: *const dnv_vista_sdk_metadata_tag_t,
    ) -> c_char;
    pub(crate) fn dnv_vista_sdk_metadata_tag_is_custom(
        tag: *const dnv_vista_sdk_metadata_tag_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_metadata_tag_to_string(
        tag: *const dnv_vista_sdk_metadata_tag_t,
    ) -> *mut c_char;
}
