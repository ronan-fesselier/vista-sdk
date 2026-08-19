use std::ffi::{c_char, c_int};

use super::imo_number::dnv_vista_sdk_imo_number_t;
use super::local_id_builder::dnv_vista_sdk_local_id_builder_t;
use super::parsing_errors::dnv_vista_sdk_parsing_errors_t;
use super::universal_id::dnv_vista_sdk_universal_id_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_universal_id_builder_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_universal_id_builder_naming_entity() -> *const c_char;
    pub(crate) fn dnv_vista_sdk_universal_id_builder_create(
        vis_version: *const c_char,
    ) -> *mut dnv_vista_sdk_universal_id_builder_t;
    pub(crate) fn dnv_vista_sdk_universal_id_builder_free(
        builder: *mut dnv_vista_sdk_universal_id_builder_t,
    );
    pub(crate) fn dnv_vista_sdk_universal_id_builder_imo_number(
        builder: *const dnv_vista_sdk_universal_id_builder_t,
    ) -> *mut dnv_vista_sdk_imo_number_t;
    pub(crate) fn dnv_vista_sdk_universal_id_builder_local_id(
        builder: *const dnv_vista_sdk_universal_id_builder_t,
    ) -> *mut dnv_vista_sdk_local_id_builder_t;
    pub(crate) fn dnv_vista_sdk_universal_id_builder_is_valid(
        builder: *const dnv_vista_sdk_universal_id_builder_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_universal_id_builder_equals(
        a: *const dnv_vista_sdk_universal_id_builder_t,
        b: *const dnv_vista_sdk_universal_id_builder_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_universal_id_builder_with_imo_number(
        builder: *const dnv_vista_sdk_universal_id_builder_t,
        imo_number: *const dnv_vista_sdk_imo_number_t,
    ) -> *mut dnv_vista_sdk_universal_id_builder_t;
    pub(crate) fn dnv_vista_sdk_universal_id_builder_without_imo_number(
        builder: *const dnv_vista_sdk_universal_id_builder_t,
    ) -> *mut dnv_vista_sdk_universal_id_builder_t;
    pub(crate) fn dnv_vista_sdk_universal_id_builder_with_local_id(
        builder: *const dnv_vista_sdk_universal_id_builder_t,
        local_id_builder: *const dnv_vista_sdk_local_id_builder_t,
    ) -> *mut dnv_vista_sdk_universal_id_builder_t;
    pub(crate) fn dnv_vista_sdk_universal_id_builder_without_local_id(
        builder: *const dnv_vista_sdk_universal_id_builder_t,
    ) -> *mut dnv_vista_sdk_universal_id_builder_t;
    pub(crate) fn dnv_vista_sdk_universal_id_builder_build(
        builder: *const dnv_vista_sdk_universal_id_builder_t,
    ) -> *mut dnv_vista_sdk_universal_id_t;
    pub(crate) fn dnv_vista_sdk_universal_id_builder_to_string(
        builder: *const dnv_vista_sdk_universal_id_builder_t,
    ) -> *mut c_char;
    pub(crate) fn dnv_vista_sdk_universal_id_builder_from_string(
        universal_id_str: *const c_char,
    ) -> *mut dnv_vista_sdk_universal_id_t;
    pub(crate) fn dnv_vista_sdk_universal_id_builder_from_string_with_errors(
        universal_id_str: *const c_char,
        out_errors: *mut *mut dnv_vista_sdk_parsing_errors_t,
    ) -> *mut dnv_vista_sdk_universal_id_t;
}
