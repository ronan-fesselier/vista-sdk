use std::ffi::CStr;
use std::os::raw::c_int;
use std::str::FromStr;

use crate::core::error::{last_error, VistaError};
use crate::core::location::Location;
use crate::core::location_group::{self, LocationGroup};
use crate::core::locations::Locations;
use crate::core::vis_version::VisVersion;
use crate::ffi::core::location_builder as ffi;
use crate::ffi::core::locations as ffi_locations;

/// Immutable builder for constructing a [`Location`] one component at a time.
///
/// Every `with_*`/`without_*` method consumes `self` and returns a new, independent
/// builder. Values are validated against the VIS version's location specification.
pub struct LocationBuilder(*mut ffi::dnv_vista_sdk_location_builder_t);

impl Drop for LocationBuilder {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_location_builder_free(self.0) }
    }
}

impl LocationBuilder {
    /// Creates a new LocationBuilder for the given [`Locations`] instance.
    pub fn create(locations: &Locations) -> Self {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_location_builder_create(
                locations as *const Locations as *const ffi_locations::dnv_vista_sdk_locations_t,
            )
        };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_location_builder_create returned NULL"
        );
        LocationBuilder(ptr)
    }

    /// Returns the VIS version this builder validates against.
    pub fn version(&self) -> VisVersion {
        let ptr = unsafe { ffi::dnv_vista_sdk_location_builder_version(self.0) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_location_builder_version returned NULL"
        );
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in location builder version");
        VisVersion::from_str(s).expect("unrecognized location builder version string")
    }

    /// Returns the location number component, or `None` if not set.
    pub fn number(&self) -> Option<i32> {
        let mut out: c_int = 0;
        let ok = unsafe { ffi::dnv_vista_sdk_location_builder_number(self.0, &mut out) };
        if ok != 0 {
            Some(out as i32)
        } else {
            None
        }
    }

    /// Returns the side location component, or `None` if not set.
    pub fn side(&self) -> Option<char> {
        let mut out: i8 = 0;
        let ok = unsafe { ffi::dnv_vista_sdk_location_builder_side(self.0, &mut out) };
        if ok != 0 {
            Some(out as u8 as char)
        } else {
            None
        }
    }

    /// Returns the vertical location component, or `None` if not set.
    pub fn vertical(&self) -> Option<char> {
        let mut out: i8 = 0;
        let ok = unsafe { ffi::dnv_vista_sdk_location_builder_vertical(self.0, &mut out) };
        if ok != 0 {
            Some(out as u8 as char)
        } else {
            None
        }
    }

    /// Returns the transverse location component, or `None` if not set.
    pub fn transverse(&self) -> Option<char> {
        let mut out: i8 = 0;
        let ok = unsafe { ffi::dnv_vista_sdk_location_builder_transverse(self.0, &mut out) };
        if ok != 0 {
            Some(out as u8 as char)
        } else {
            None
        }
    }

    /// Returns the longitudinal location component, or `None` if not set.
    pub fn longitudinal(&self) -> Option<char> {
        let mut out: i8 = 0;
        let ok = unsafe { ffi::dnv_vista_sdk_location_builder_longitudinal(self.0, &mut out) };
        if ok != 0 {
            Some(out as u8 as char)
        } else {
            None
        }
    }

    /// Sets all components from an existing [`Location`].
    pub fn with_location(self, location: &Location) -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_location_builder_with_location(self.0, location.0) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_location_builder_with_location returned NULL"
        );
        LocationBuilder(ptr)
    }

    /// Sets the number component. Returns `Err` if `number` < 1.
    pub fn with_number(self, number: i32) -> Result<Self, VistaError> {
        let ptr =
            unsafe { ffi::dnv_vista_sdk_location_builder_with_number(self.0, number as c_int) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(LocationBuilder(ptr))
        }
    }

    /// Removes the number component.
    pub fn without_number(self) -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_location_builder_without_number(self.0) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_location_builder_without_number returned NULL"
        );
        LocationBuilder(ptr)
    }

    /// Sets a location component by character code, auto-detecting its group. Returns `Err` if `code` is not valid.
    pub fn with_code(self, code: char) -> Result<Self, VistaError> {
        let ptr = unsafe { ffi::dnv_vista_sdk_location_builder_with_code(self.0, code as i8) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(LocationBuilder(ptr))
        }
    }

    /// Sets the side component. Returns `Err` if `side` is not a valid side code.
    pub fn with_side(self, side: char) -> Result<Self, VistaError> {
        let ptr = unsafe { ffi::dnv_vista_sdk_location_builder_with_side(self.0, side as i8) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(LocationBuilder(ptr))
        }
    }

    /// Removes the side component.
    pub fn without_side(self) -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_location_builder_without_side(self.0) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_location_builder_without_side returned NULL"
        );
        LocationBuilder(ptr)
    }

    /// Sets the vertical component. Returns `Err` if `vertical` is not a valid vertical code.
    pub fn with_vertical(self, vertical: char) -> Result<Self, VistaError> {
        let ptr =
            unsafe { ffi::dnv_vista_sdk_location_builder_with_vertical(self.0, vertical as i8) };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(LocationBuilder(ptr))
        }
    }

    /// Removes the vertical component.
    pub fn without_vertical(self) -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_location_builder_without_vertical(self.0) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_location_builder_without_vertical returned NULL"
        );
        LocationBuilder(ptr)
    }

    /// Sets the transverse component. Returns `Err` if `transverse` is not a valid transverse code.
    pub fn with_transverse(self, transverse: char) -> Result<Self, VistaError> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_location_builder_with_transverse(self.0, transverse as i8)
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(LocationBuilder(ptr))
        }
    }

    /// Removes the transverse component.
    pub fn without_transverse(self) -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_location_builder_without_transverse(self.0) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_location_builder_without_transverse returned NULL"
        );
        LocationBuilder(ptr)
    }

    /// Sets the longitudinal component. Returns `Err` if `longitudinal` is not a valid longitudinal code.
    pub fn with_longitudinal(self, longitudinal: char) -> Result<Self, VistaError> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_location_builder_with_longitudinal(self.0, longitudinal as i8)
        };
        if ptr.is_null() {
            Err(last_error())
        } else {
            Ok(LocationBuilder(ptr))
        }
    }

    /// Removes the longitudinal component.
    pub fn without_longitudinal(self) -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_location_builder_without_longitudinal(self.0) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_location_builder_without_longitudinal returned NULL"
        );
        LocationBuilder(ptr)
    }

    /// Removes the component belonging to `group`.
    pub fn without_value(self, group: LocationGroup) -> Self {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_location_builder_without_value(self.0, location_group::to_ffi(group))
        };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_location_builder_without_value returned NULL"
        );
        LocationBuilder(ptr)
    }

    /// Builds the final [`Location`] from this builder's current state.
    pub fn build(&self) -> Location {
        let ptr = unsafe { ffi::dnv_vista_sdk_location_builder_build(self.0) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_location_builder_build returned NULL"
        );
        Location(ptr)
    }
}

impl std::fmt::Display for LocationBuilder {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let ptr = unsafe { ffi::dnv_vista_sdk_location_builder_to_string(self.0) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_location_builder_to_string returned NULL"
        );
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in location builder string");
        let result = f.write_str(s);
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        result
    }
}
