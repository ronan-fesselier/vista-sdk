use std::ffi::{c_char, c_int};

use super::gmod_node::dnv_vista_sdk_gmod_node_t;
use super::gmod_path::dnv_vista_sdk_gmod_path_t;
use super::location::dnv_vista_sdk_location_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_gmod_individualizable_set_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_gmod_individualizable_set_create(
        node_indices: *const c_int,
        node_indices_count: usize,
        source_path: *const dnv_vista_sdk_gmod_path_t,
    ) -> *mut dnv_vista_sdk_gmod_individualizable_set_t;
    pub(crate) fn dnv_vista_sdk_gmod_individualizable_set_free(
        set: *mut dnv_vista_sdk_gmod_individualizable_set_t,
    );
    pub(crate) fn dnv_vista_sdk_gmod_individualizable_set_build(
        set: *mut dnv_vista_sdk_gmod_individualizable_set_t,
    ) -> *mut dnv_vista_sdk_gmod_path_t;
    pub(crate) fn dnv_vista_sdk_gmod_individualizable_set_node_count(
        set: *const dnv_vista_sdk_gmod_individualizable_set_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_gmod_individualizable_set_node_at(
        set: *const dnv_vista_sdk_gmod_individualizable_set_t,
        index: usize,
    ) -> *mut dnv_vista_sdk_gmod_node_t;
    pub(crate) fn dnv_vista_sdk_gmod_individualizable_set_node_free(
        node: *mut dnv_vista_sdk_gmod_node_t,
    );
    pub(crate) fn dnv_vista_sdk_gmod_individualizable_set_index_count(
        set: *const dnv_vista_sdk_gmod_individualizable_set_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_gmod_individualizable_set_index_at(
        set: *const dnv_vista_sdk_gmod_individualizable_set_t,
        position: usize,
        out_index: *mut c_int,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_gmod_individualizable_set_location(
        set: *const dnv_vista_sdk_gmod_individualizable_set_t,
    ) -> *mut dnv_vista_sdk_location_t;
    pub(crate) fn dnv_vista_sdk_gmod_individualizable_set_to_string(
        set: *const dnv_vista_sdk_gmod_individualizable_set_t,
    ) -> *mut c_char;
}
