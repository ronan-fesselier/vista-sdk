use crate::ffi::core::location_group as ffi;

/// Enumeration of relative location groups (number, side, vertical, transverse, longitudinal).
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u8)]
pub enum LocationGroup {
    Number = 0,
    Side,
    Vertical,
    Transverse,
    Longitudinal,
}

pub(crate) fn to_ffi(group: LocationGroup) -> ffi::dnv_vista_sdk_location_group_t {
    match group {
        LocationGroup::Number => ffi::dnv_vista_sdk_location_group_t::Number,
        LocationGroup::Side => ffi::dnv_vista_sdk_location_group_t::Side,
        LocationGroup::Vertical => ffi::dnv_vista_sdk_location_group_t::Vertical,
        LocationGroup::Transverse => ffi::dnv_vista_sdk_location_group_t::Transverse,
        LocationGroup::Longitudinal => ffi::dnv_vista_sdk_location_group_t::Longitudinal,
    }
}

#[cfg(test)]
mod discriminant_sync {
    use super::*;

    #[test]
    fn location_group_discriminants_match_ffi() {
        assert_eq!(
            LocationGroup::Number as u8,
            ffi::dnv_vista_sdk_location_group_t::Number as u8
        );
        assert_eq!(
            LocationGroup::Side as u8,
            ffi::dnv_vista_sdk_location_group_t::Side as u8
        );
        assert_eq!(
            LocationGroup::Vertical as u8,
            ffi::dnv_vista_sdk_location_group_t::Vertical as u8
        );
        assert_eq!(
            LocationGroup::Transverse as u8,
            ffi::dnv_vista_sdk_location_group_t::Transverse as u8
        );
        assert_eq!(
            LocationGroup::Longitudinal as u8,
            ffi::dnv_vista_sdk_location_group_t::Longitudinal as u8
        );
    }
}
