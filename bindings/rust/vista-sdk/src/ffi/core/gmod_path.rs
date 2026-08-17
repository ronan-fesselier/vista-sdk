use std::ffi::{c_char, c_int};

use super::gmod::dnv_vista_sdk_gmod_t;
use super::gmod_individualizable_set::dnv_vista_sdk_gmod_individualizable_set_t;
use super::gmod_node::dnv_vista_sdk_gmod_node_t;
use super::locations::dnv_vista_sdk_locations_t;
use super::parsing_errors::dnv_vista_sdk_parsing_errors_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_gmod_path_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_gmod_path_free(path: *mut dnv_vista_sdk_gmod_path_t);
    pub(crate) fn dnv_vista_sdk_gmod_path_from_short_path_version(
        item: *const c_char,
        vis_version: *const c_char,
    ) -> *mut dnv_vista_sdk_gmod_path_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_from_short_path(
        item: *const c_char,
        gmod: *const dnv_vista_sdk_gmod_t,
        locations: *const dnv_vista_sdk_locations_t,
    ) -> *mut dnv_vista_sdk_gmod_path_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_from_short_path_with_errors(
        item: *const c_char,
        gmod: *const dnv_vista_sdk_gmod_t,
        locations: *const dnv_vista_sdk_locations_t,
        out_errors: *mut *mut dnv_vista_sdk_parsing_errors_t,
    ) -> *mut dnv_vista_sdk_gmod_path_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_from_full_path(
        full_path_str: *const c_char,
        gmod: *const dnv_vista_sdk_gmod_t,
        locations: *const dnv_vista_sdk_locations_t,
    ) -> *mut dnv_vista_sdk_gmod_path_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_from_full_path_with_errors(
        full_path_str: *const c_char,
        gmod: *const dnv_vista_sdk_gmod_t,
        locations: *const dnv_vista_sdk_locations_t,
        out_errors: *mut *mut dnv_vista_sdk_parsing_errors_t,
    ) -> *mut dnv_vista_sdk_gmod_path_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_version(
        path: *const dnv_vista_sdk_gmod_path_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_gmod_path_node(
        path: *const dnv_vista_sdk_gmod_path_t,
    ) -> *const dnv_vista_sdk_gmod_node_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_length(path: *const dnv_vista_sdk_gmod_path_t) -> usize;
    pub(crate) fn dnv_vista_sdk_gmod_path_at(
        path: *const dnv_vista_sdk_gmod_path_t,
        index: usize,
    ) -> *const dnv_vista_sdk_gmod_node_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_equals(
        a: *const dnv_vista_sdk_gmod_path_t,
        b: *const dnv_vista_sdk_gmod_path_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_gmod_path_is_mappable(
        path: *const dnv_vista_sdk_gmod_path_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_gmod_path_is_individualizable(
        path: *const dnv_vista_sdk_gmod_path_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_gmod_path_without_locations(
        path: *const dnv_vista_sdk_gmod_path_t,
    ) -> *mut dnv_vista_sdk_gmod_path_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_normal_assignment_name(
        path: *const dnv_vista_sdk_gmod_path_t,
        node_depth: usize,
    ) -> *mut c_char;
    pub(crate) fn dnv_vista_sdk_gmod_path_individualizable_set_count(
        path: *const dnv_vista_sdk_gmod_path_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_gmod_path_individualizable_set_at(
        path: *const dnv_vista_sdk_gmod_path_t,
        index: usize,
    ) -> *mut dnv_vista_sdk_gmod_individualizable_set_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_common_name_count(
        path: *const dnv_vista_sdk_gmod_path_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_gmod_path_common_name_depth_at(
        path: *const dnv_vista_sdk_gmod_path_t,
        index: usize,
        out_depth: *mut usize,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_gmod_path_common_name_at(
        path: *const dnv_vista_sdk_gmod_path_t,
        index: usize,
    ) -> *mut c_char;
    pub(crate) fn dnv_vista_sdk_gmod_path_to_string(
        path: *const dnv_vista_sdk_gmod_path_t,
    ) -> *mut c_char;
    pub(crate) fn dnv_vista_sdk_gmod_path_to_full_path_string(
        path: *const dnv_vista_sdk_gmod_path_t,
    ) -> *mut c_char;
    pub(crate) fn dnv_vista_sdk_gmod_path_to_string_dump(
        path: *const dnv_vista_sdk_gmod_path_t,
    ) -> *mut c_char;
}
