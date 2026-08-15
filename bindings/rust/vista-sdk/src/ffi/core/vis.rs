use std::ffi::c_char;

use super::codebooks::dnv_vista_sdk_codebooks_t;

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
}
