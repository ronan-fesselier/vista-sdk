use std::ffi::{c_char, c_int};

use super::codebook_name::dnv_vista_sdk_codebook_name_t;
use super::gmod_path::dnv_vista_sdk_gmod_path_t;
use super::local_id_builder::dnv_vista_sdk_local_id_builder_t;
use super::metadata_tag::dnv_vista_sdk_metadata_tag_t;
use super::parsing_errors::dnv_vista_sdk_parsing_errors_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_local_id_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_local_id_naming_rule() -> *const c_char;
    pub(crate) fn dnv_vista_sdk_local_id_free(local_id: *mut dnv_vista_sdk_local_id_t);
    pub(crate) fn dnv_vista_sdk_local_id_from_string(
        local_id_str: *const c_char,
    ) -> *mut dnv_vista_sdk_local_id_t;
    pub(crate) fn dnv_vista_sdk_local_id_from_string_with_errors(
        local_id_str: *const c_char,
        out_errors: *mut *mut dnv_vista_sdk_parsing_errors_t,
    ) -> *mut dnv_vista_sdk_local_id_t;
    pub(crate) fn dnv_vista_sdk_local_id_version(
        local_id: *const dnv_vista_sdk_local_id_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_local_id_primary_item(
        local_id: *const dnv_vista_sdk_local_id_t,
    ) -> *const dnv_vista_sdk_gmod_path_t;
    pub(crate) fn dnv_vista_sdk_local_id_secondary_item(
        local_id: *const dnv_vista_sdk_local_id_t,
    ) -> *const dnv_vista_sdk_gmod_path_t;
    pub(crate) fn dnv_vista_sdk_local_id_is_verbose_mode(
        local_id: *const dnv_vista_sdk_local_id_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_local_id_has_custom_tag(
        local_id: *const dnv_vista_sdk_local_id_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_local_id_equals(
        a: *const dnv_vista_sdk_local_id_t,
        b: *const dnv_vista_sdk_local_id_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_local_id_metadata_tag(
        local_id: *const dnv_vista_sdk_local_id_t,
        name: dnv_vista_sdk_codebook_name_t,
    ) -> *const dnv_vista_sdk_metadata_tag_t;
    pub(crate) fn dnv_vista_sdk_local_id_builder(
        local_id: *const dnv_vista_sdk_local_id_t,
    ) -> *const dnv_vista_sdk_local_id_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_to_string(
        local_id: *const dnv_vista_sdk_local_id_t,
    ) -> *mut c_char;
}
