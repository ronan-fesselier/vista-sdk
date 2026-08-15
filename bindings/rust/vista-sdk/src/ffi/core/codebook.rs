use std::ffi::{c_char, c_int};

use super::codebook_name::dnv_vista_sdk_codebook_name_t;
use super::metadata_tag::dnv_vista_sdk_metadata_tag_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_codebook_t {
    _opaque: [u8; 0],
}

#[allow(non_camel_case_types, dead_code)]
#[repr(u32)]
pub(crate) enum dnv_vista_sdk_position_validation_result_t {
    Invalid = 0,
    InvalidOrder = 1,
    InvalidGrouping = 2,
    Valid = 100,
    Custom = 101,
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_codebook_name(
        codebook: *const dnv_vista_sdk_codebook_t,
    ) -> dnv_vista_sdk_codebook_name_t;
    pub(crate) fn dnv_vista_sdk_codebook_standard_values_count(
        codebook: *const dnv_vista_sdk_codebook_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_codebook_standard_value_at(
        codebook: *const dnv_vista_sdk_codebook_t,
        index: usize,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_codebook_groups_count(
        codebook: *const dnv_vista_sdk_codebook_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_codebook_group_at(
        codebook: *const dnv_vista_sdk_codebook_t,
        index: usize,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_codebook_has_group(
        codebook: *const dnv_vista_sdk_codebook_t,
        group: *const c_char,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_codebook_has_standard_value(
        codebook: *const dnv_vista_sdk_codebook_t,
        value: *const c_char,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_codebook_validate_position(
        codebook: *const dnv_vista_sdk_codebook_t,
        position: *const c_char,
    ) -> dnv_vista_sdk_position_validation_result_t;
    pub(crate) fn dnv_vista_sdk_codebook_create_tag(
        codebook: *const dnv_vista_sdk_codebook_t,
        value: *const c_char,
    ) -> *mut dnv_vista_sdk_metadata_tag_t;
}
