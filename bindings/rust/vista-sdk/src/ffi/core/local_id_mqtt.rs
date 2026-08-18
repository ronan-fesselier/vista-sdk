use std::ffi::c_char;

use super::codebook_name::dnv_vista_sdk_codebook_name_t;
use super::gmod_path::dnv_vista_sdk_gmod_path_t;
use super::local_id_builder::dnv_vista_sdk_local_id_builder_t;
use super::metadata_tag::dnv_vista_sdk_metadata_tag_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_local_id_mqtt_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_local_id_mqtt_create(
        builder: *const dnv_vista_sdk_local_id_builder_t,
    ) -> *mut dnv_vista_sdk_local_id_mqtt_t;
    pub(crate) fn dnv_vista_sdk_local_id_mqtt_free(local_id: *mut dnv_vista_sdk_local_id_mqtt_t);
    pub(crate) fn dnv_vista_sdk_local_id_mqtt_version(
        local_id: *const dnv_vista_sdk_local_id_mqtt_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_local_id_mqtt_primary_item(
        local_id: *const dnv_vista_sdk_local_id_mqtt_t,
    ) -> *const dnv_vista_sdk_gmod_path_t;
    pub(crate) fn dnv_vista_sdk_local_id_mqtt_secondary_item(
        local_id: *const dnv_vista_sdk_local_id_mqtt_t,
    ) -> *const dnv_vista_sdk_gmod_path_t;
    pub(crate) fn dnv_vista_sdk_local_id_mqtt_metadata_tag(
        local_id: *const dnv_vista_sdk_local_id_mqtt_t,
        name: dnv_vista_sdk_codebook_name_t,
    ) -> *const dnv_vista_sdk_metadata_tag_t;
    pub(crate) fn dnv_vista_sdk_local_id_mqtt_builder(
        local_id: *const dnv_vista_sdk_local_id_mqtt_t,
    ) -> *const dnv_vista_sdk_local_id_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_mqtt_to_string(
        local_id: *const dnv_vista_sdk_local_id_mqtt_t,
    ) -> *mut c_char;
}
