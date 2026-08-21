use std::ffi::{c_char, c_int};

use crate::ffi::core::gmod_node::dnv_vista_sdk_gmod_node_t;
use crate::ffi::core::gmod_path::dnv_vista_sdk_gmod_path_t;
use crate::ffi::core::location::dnv_vista_sdk_location_t;

use super::gmod_path_query::dnv_vista_sdk_gmod_path_query_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_gmod_path_query_builder_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_gmod_path_query_builder_create(
    ) -> *mut dnv_vista_sdk_gmod_path_query_builder_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_query_builder_from(
        path: *const dnv_vista_sdk_gmod_path_t,
    ) -> *mut dnv_vista_sdk_gmod_path_query_builder_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_query_builder_free(
        builder: *mut dnv_vista_sdk_gmod_path_query_builder_t,
    );
    pub(crate) fn dnv_vista_sdk_gmod_path_query_builder_path(
        builder: *const dnv_vista_sdk_gmod_path_query_builder_t,
    ) -> *const dnv_vista_sdk_gmod_path_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_query_builder_path_with_node_all_locations(
        builder: *const dnv_vista_sdk_gmod_path_query_builder_t,
        code: *const c_char,
        match_all_locations: c_int,
    ) -> *mut dnv_vista_sdk_gmod_path_query_builder_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_query_builder_path_with_node_locations(
        builder: *const dnv_vista_sdk_gmod_path_query_builder_t,
        code: *const c_char,
        locations: *const *const dnv_vista_sdk_location_t,
        location_count: usize,
    ) -> *mut dnv_vista_sdk_gmod_path_query_builder_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_query_builder_with_any_node_before(
        builder: *const dnv_vista_sdk_gmod_path_query_builder_t,
        code: *const c_char,
    ) -> *mut dnv_vista_sdk_gmod_path_query_builder_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_query_builder_with_any_node_after(
        builder: *const dnv_vista_sdk_gmod_path_query_builder_t,
        code: *const c_char,
    ) -> *mut dnv_vista_sdk_gmod_path_query_builder_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_query_builder_without_locations(
        builder: *const dnv_vista_sdk_gmod_path_query_builder_t,
    ) -> *mut dnv_vista_sdk_gmod_path_query_builder_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_query_builder_with_node_all_locations(
        builder: *const dnv_vista_sdk_gmod_path_query_builder_t,
        node: *const dnv_vista_sdk_gmod_node_t,
        match_all_locations: c_int,
    ) -> *mut dnv_vista_sdk_gmod_path_query_builder_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_query_builder_with_node_locations(
        builder: *const dnv_vista_sdk_gmod_path_query_builder_t,
        node: *const dnv_vista_sdk_gmod_node_t,
        locations: *const *const dnv_vista_sdk_location_t,
        location_count: usize,
    ) -> *mut dnv_vista_sdk_gmod_path_query_builder_t;
    pub(crate) fn dnv_vista_sdk_gmod_path_query_builder_build(
        builder: *const dnv_vista_sdk_gmod_path_query_builder_t,
    ) -> *mut dnv_vista_sdk_gmod_path_query_t;
}
