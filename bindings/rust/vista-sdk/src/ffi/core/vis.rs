use std::ffi::c_char;

use super::codebooks::dnv_vista_sdk_codebooks_t;
use super::gmod::dnv_vista_sdk_gmod_t;
use super::gmod_node::dnv_vista_sdk_gmod_node_t;
use super::gmod_path::dnv_vista_sdk_gmod_path_t;
use super::local_id::dnv_vista_sdk_local_id_t;
use super::local_id_builder::dnv_vista_sdk_local_id_builder_t;
use super::locations::dnv_vista_sdk_locations_t;

#[repr(C)]
pub(crate) struct dnv_vista_sdk_vis_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_vis_instance() -> *const dnv_vista_sdk_vis_t;
    pub(crate) fn dnv_vista_sdk_vis_version_count(vis: *const dnv_vista_sdk_vis_t) -> usize;
    pub(crate) fn dnv_vista_sdk_vis_version_at(
        vis: *const dnv_vista_sdk_vis_t,
        index: usize,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_vis_latest(vis: *const dnv_vista_sdk_vis_t) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_vis_codebooks(
        vis: *const dnv_vista_sdk_vis_t,
        vis_version: *const c_char,
    ) -> *const dnv_vista_sdk_codebooks_t;
    pub(crate) fn dnv_vista_sdk_vis_locations(
        vis: *const dnv_vista_sdk_vis_t,
        vis_version: *const c_char,
    ) -> *const dnv_vista_sdk_locations_t;
    pub(crate) fn dnv_vista_sdk_vis_gmod(
        vis: *const dnv_vista_sdk_vis_t,
        vis_version: *const c_char,
    ) -> *const dnv_vista_sdk_gmod_t;
    pub(crate) fn dnv_vista_sdk_vis_convert_node(
        vis: *const dnv_vista_sdk_vis_t,
        source_version: *const c_char,
        source_node: *const dnv_vista_sdk_gmod_node_t,
        target_version: *const c_char,
    ) -> *mut dnv_vista_sdk_gmod_node_t;
    pub(crate) fn dnv_vista_sdk_vis_convert_path(
        vis: *const dnv_vista_sdk_vis_t,
        source_version: *const c_char,
        source_path: *const dnv_vista_sdk_gmod_path_t,
        target_version: *const c_char,
    ) -> *mut dnv_vista_sdk_gmod_path_t;
    pub(crate) fn dnv_vista_sdk_vis_convert_local_id_builder(
        vis: *const dnv_vista_sdk_vis_t,
        source_local_id: *const dnv_vista_sdk_local_id_builder_t,
        target_version: *const c_char,
    ) -> *mut dnv_vista_sdk_local_id_builder_t;
    pub(crate) fn dnv_vista_sdk_vis_convert_local_id(
        vis: *const dnv_vista_sdk_vis_t,
        source_local_id: *const dnv_vista_sdk_local_id_t,
        target_version: *const c_char,
    ) -> *mut dnv_vista_sdk_local_id_t;
}
