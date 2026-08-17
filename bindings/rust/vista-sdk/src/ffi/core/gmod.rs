use std::ffi::c_char;

use super::gmod_node::dnv_vista_sdk_gmod_node_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_gmod_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_gmod_version(gmod: *const dnv_vista_sdk_gmod_t) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_gmod_root_node(
        gmod: *const dnv_vista_sdk_gmod_t,
    ) -> *const dnv_vista_sdk_gmod_node_t;
    pub(crate) fn dnv_vista_sdk_gmod_get_node(
        gmod: *const dnv_vista_sdk_gmod_t,
        code: *const c_char,
    ) -> *const dnv_vista_sdk_gmod_node_t;
    pub(crate) fn dnv_vista_sdk_gmod_node_count(gmod: *const dnv_vista_sdk_gmod_t) -> usize;
    pub(crate) fn dnv_vista_sdk_gmod_node_at(
        gmod: *const dnv_vista_sdk_gmod_t,
        index: usize,
    ) -> *const dnv_vista_sdk_gmod_node_t;
}
