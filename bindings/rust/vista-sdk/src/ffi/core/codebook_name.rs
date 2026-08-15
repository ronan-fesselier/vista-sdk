use std::ffi::{c_char, c_int};

#[allow(non_camel_case_types)]
#[repr(u8)]
pub(crate) enum dnv_vista_sdk_codebook_name_t {
    Quantity = 1,
    Content,
    Calculation,
    State,
    Command,
    Type,
    FunctionalServices,
    MaintenanceCategory,
    ActivityType,
    Position,
    Detail,
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_codebook_names_from_prefix(
        prefix: *const c_char,
        out_name: *mut dnv_vista_sdk_codebook_name_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_codebook_names_to_prefix(
        name: dnv_vista_sdk_codebook_name_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_codebook_names_to_string(
        name: dnv_vista_sdk_codebook_name_t,
    ) -> *const c_char;
}
