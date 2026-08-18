use std::ffi::{c_char, c_int};

use super::codebook_name::dnv_vista_sdk_codebook_name_t;
use super::gmod_path::dnv_vista_sdk_gmod_path_t;
use super::local_id::dnv_vista_sdk_local_id_t;
use super::metadata_tag::dnv_vista_sdk_metadata_tag_t;
use super::parsing_errors::dnv_vista_sdk_parsing_errors_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_local_id_builder_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_local_id_builder_naming_rule() -> *const c_char;
    pub(crate) fn dnv_vista_sdk_local_id_builder_create(
        vis_version: *const c_char,
    ) -> *mut dnv_vista_sdk_local_id_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_builder_free(
        builder: *mut dnv_vista_sdk_local_id_builder_t,
    );
    pub(crate) fn dnv_vista_sdk_local_id_builder_version(
        builder: *const dnv_vista_sdk_local_id_builder_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_local_id_builder_is_verbose_mode(
        builder: *const dnv_vista_sdk_local_id_builder_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_local_id_builder_is_valid(
        builder: *const dnv_vista_sdk_local_id_builder_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_local_id_builder_is_empty(
        builder: *const dnv_vista_sdk_local_id_builder_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_local_id_builder_is_empty_metadata(
        builder: *const dnv_vista_sdk_local_id_builder_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_local_id_builder_has_custom_tag(
        builder: *const dnv_vista_sdk_local_id_builder_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_local_id_builder_equals(
        a: *const dnv_vista_sdk_local_id_builder_t,
        b: *const dnv_vista_sdk_local_id_builder_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_local_id_builder_primary_item(
        builder: *const dnv_vista_sdk_local_id_builder_t,
    ) -> *mut dnv_vista_sdk_gmod_path_t;
    pub(crate) fn dnv_vista_sdk_local_id_builder_secondary_item(
        builder: *const dnv_vista_sdk_local_id_builder_t,
    ) -> *mut dnv_vista_sdk_gmod_path_t;
    pub(crate) fn dnv_vista_sdk_local_id_builder_metadata_tag(
        builder: *const dnv_vista_sdk_local_id_builder_t,
        name: dnv_vista_sdk_codebook_name_t,
    ) -> *mut dnv_vista_sdk_metadata_tag_t;
    pub(crate) fn dnv_vista_sdk_local_id_builder_with_vis_version(
        builder: *const dnv_vista_sdk_local_id_builder_t,
        vis_version: *const c_char,
    ) -> *mut dnv_vista_sdk_local_id_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_builder_without_vis_version(
        builder: *const dnv_vista_sdk_local_id_builder_t,
    ) -> *mut dnv_vista_sdk_local_id_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_builder_with_primary_item(
        builder: *const dnv_vista_sdk_local_id_builder_t,
        path: *const dnv_vista_sdk_gmod_path_t,
    ) -> *mut dnv_vista_sdk_local_id_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_builder_without_primary_item(
        builder: *const dnv_vista_sdk_local_id_builder_t,
    ) -> *mut dnv_vista_sdk_local_id_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_builder_with_secondary_item(
        builder: *const dnv_vista_sdk_local_id_builder_t,
        path: *const dnv_vista_sdk_gmod_path_t,
    ) -> *mut dnv_vista_sdk_local_id_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_builder_without_secondary_item(
        builder: *const dnv_vista_sdk_local_id_builder_t,
    ) -> *mut dnv_vista_sdk_local_id_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_builder_with_metadata_tag(
        builder: *const dnv_vista_sdk_local_id_builder_t,
        tag: *const dnv_vista_sdk_metadata_tag_t,
    ) -> *mut dnv_vista_sdk_local_id_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_builder_without_metadata_tag(
        builder: *const dnv_vista_sdk_local_id_builder_t,
        name: dnv_vista_sdk_codebook_name_t,
    ) -> *mut dnv_vista_sdk_local_id_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_builder_with_verbose_mode(
        builder: *const dnv_vista_sdk_local_id_builder_t,
        verbose: c_int,
    ) -> *mut dnv_vista_sdk_local_id_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_builder_build(
        builder: *const dnv_vista_sdk_local_id_builder_t,
    ) -> *mut dnv_vista_sdk_local_id_t;
    pub(crate) fn dnv_vista_sdk_local_id_builder_to_string(
        builder: *const dnv_vista_sdk_local_id_builder_t,
    ) -> *mut c_char;
    pub(crate) fn dnv_vista_sdk_local_id_builder_from_string(
        local_id_str: *const c_char,
    ) -> *mut dnv_vista_sdk_local_id_t;
    pub(crate) fn dnv_vista_sdk_local_id_builder_from_string_with_errors(
        local_id_str: *const c_char,
        out_errors: *mut *mut dnv_vista_sdk_parsing_errors_t,
    ) -> *mut dnv_vista_sdk_local_id_t;
}
