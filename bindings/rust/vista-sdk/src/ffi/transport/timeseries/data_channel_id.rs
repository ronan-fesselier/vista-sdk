use crate::ffi::core::local_id::dnv_vista_sdk_local_id_t;

#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_channel_id_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_tsd_channel_id_from_string(
        value: *const std::ffi::c_char,
    ) -> *mut dnv_vista_sdk_tsd_channel_id_t;

    pub(crate) fn dnv_vista_sdk_tsd_channel_id_free(
        channel_id: *mut dnv_vista_sdk_tsd_channel_id_t,
    );

    pub(crate) fn dnv_vista_sdk_tsd_channel_id_equals(
        a: *const dnv_vista_sdk_tsd_channel_id_t,
        b: *const dnv_vista_sdk_tsd_channel_id_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_tsd_channel_id_is_local_id(
        channel_id: *const dnv_vista_sdk_tsd_channel_id_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_tsd_channel_id_is_short_id(
        channel_id: *const dnv_vista_sdk_tsd_channel_id_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_tsd_channel_id_local_id(
        channel_id: *const dnv_vista_sdk_tsd_channel_id_t,
    ) -> *const dnv_vista_sdk_local_id_t;

    pub(crate) fn dnv_vista_sdk_tsd_channel_id_short_id(
        channel_id: *const dnv_vista_sdk_tsd_channel_id_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_tsd_channel_id_to_string(
        channel_id: *const dnv_vista_sdk_tsd_channel_id_t,
    ) -> *mut std::ffi::c_char;
}
