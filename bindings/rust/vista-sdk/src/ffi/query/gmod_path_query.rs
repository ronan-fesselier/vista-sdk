use std::ffi::c_int;

use crate::ffi::core::gmod_path::dnv_vista_sdk_gmod_path_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_gmod_path_query_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_gmod_path_query_free(query: *mut dnv_vista_sdk_gmod_path_query_t);
    pub(crate) fn dnv_vista_sdk_gmod_path_query_match(
        query: *const dnv_vista_sdk_gmod_path_query_t,
        path: *const dnv_vista_sdk_gmod_path_t,
    ) -> c_int;
}
