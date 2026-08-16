use std::ffi::{c_char, c_int};

use super::gmod_node_metadata::dnv_vista_sdk_gmod_node_metadata_t;
use super::location::dnv_vista_sdk_location_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_gmod_node_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_gmod_node_free(node: *mut dnv_vista_sdk_gmod_node_t);
    pub(crate) fn dnv_vista_sdk_gmod_node_version(
        node: *const dnv_vista_sdk_gmod_node_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_gmod_node_code(
        node: *const dnv_vista_sdk_gmod_node_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_gmod_node_location(
        node: *const dnv_vista_sdk_gmod_node_t,
    ) -> *const dnv_vista_sdk_location_t;
    pub(crate) fn dnv_vista_sdk_gmod_node_metadata(
        node: *const dnv_vista_sdk_gmod_node_t,
    ) -> *const dnv_vista_sdk_gmod_node_metadata_t;
    pub(crate) fn dnv_vista_sdk_gmod_node_child_count(
        node: *const dnv_vista_sdk_gmod_node_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_gmod_node_child_at(
        node: *const dnv_vista_sdk_gmod_node_t,
        index: usize,
    ) -> *const dnv_vista_sdk_gmod_node_t;
    pub(crate) fn dnv_vista_sdk_gmod_node_parent_count(
        node: *const dnv_vista_sdk_gmod_node_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_gmod_node_parent_at(
        node: *const dnv_vista_sdk_gmod_node_t,
        index: usize,
    ) -> *const dnv_vista_sdk_gmod_node_t;
    pub(crate) fn dnv_vista_sdk_gmod_node_product_type(
        node: *const dnv_vista_sdk_gmod_node_t,
    ) -> *const dnv_vista_sdk_gmod_node_t;
    pub(crate) fn dnv_vista_sdk_gmod_node_product_selection(
        node: *const dnv_vista_sdk_gmod_node_t,
    ) -> *const dnv_vista_sdk_gmod_node_t;
    pub(crate) fn dnv_vista_sdk_gmod_node_is_function_composition(
        node: *const dnv_vista_sdk_gmod_node_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_gmod_node_is_mappable(
        node: *const dnv_vista_sdk_gmod_node_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_gmod_node_is_product_selection(
        node: *const dnv_vista_sdk_gmod_node_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_gmod_node_is_product_type(
        node: *const dnv_vista_sdk_gmod_node_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_gmod_node_is_asset(node: *const dnv_vista_sdk_gmod_node_t)
        -> c_int;
    pub(crate) fn dnv_vista_sdk_gmod_node_is_leaf_node(
        node: *const dnv_vista_sdk_gmod_node_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_gmod_node_is_function_node(
        node: *const dnv_vista_sdk_gmod_node_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_gmod_node_is_asset_function_node(
        node: *const dnv_vista_sdk_gmod_node_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_gmod_node_is_root(node: *const dnv_vista_sdk_gmod_node_t) -> c_int;
    pub(crate) fn dnv_vista_sdk_gmod_node_is_child(
        node: *const dnv_vista_sdk_gmod_node_t,
        other: *const dnv_vista_sdk_gmod_node_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_gmod_node_is_child_code(
        node: *const dnv_vista_sdk_gmod_node_t,
        code: *const c_char,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_gmod_node_to_string(
        node: *const dnv_vista_sdk_gmod_node_t,
    ) -> *mut c_char;
}
