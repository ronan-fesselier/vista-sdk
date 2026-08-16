use std::ffi::{c_char, c_int};

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_gmod_node_metadata_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_gmod_node_metadata_category(
        metadata: *const dnv_vista_sdk_gmod_node_metadata_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_gmod_node_metadata_type(
        metadata: *const dnv_vista_sdk_gmod_node_metadata_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_gmod_node_metadata_full_type(
        metadata: *const dnv_vista_sdk_gmod_node_metadata_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_gmod_node_metadata_name(
        metadata: *const dnv_vista_sdk_gmod_node_metadata_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_gmod_node_metadata_common_name(
        metadata: *const dnv_vista_sdk_gmod_node_metadata_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_gmod_node_metadata_definition(
        metadata: *const dnv_vista_sdk_gmod_node_metadata_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_gmod_node_metadata_common_definition(
        metadata: *const dnv_vista_sdk_gmod_node_metadata_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_gmod_node_metadata_install_substructure(
        metadata: *const dnv_vista_sdk_gmod_node_metadata_t,
        out_value: *mut c_int,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_count(
        metadata: *const dnv_vista_sdk_gmod_node_metadata_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_key_at(
        metadata: *const dnv_vista_sdk_gmod_node_metadata_t,
        index: usize,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_value_at(
        metadata: *const dnv_vista_sdk_gmod_node_metadata_t,
        index: usize,
    ) -> *const c_char;
}
