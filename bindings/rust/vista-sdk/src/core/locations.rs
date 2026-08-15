use std::ffi::{CStr, CString};
use std::ptr;
use std::str::FromStr;

use crate::core::location::Location;
use crate::core::location_group::{self, LocationGroup};
use crate::core::parsing_errors::ParsingErrors;
use crate::core::relative_location::RelativeLocation;
use crate::core::vis_version::VisVersion;
use crate::ffi::core::locations as ffi;

/// Container for all valid relative locations for a specific VIS version.
///
/// Borrowed from the VIS singleton, valid for the lifetime of the program.
#[repr(transparent)]
pub struct Locations(ffi::dnv_vista_sdk_locations_t);

impl Locations {
    pub(crate) fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_locations_t) -> &'a Self {
        assert!(!ptr.is_null(), "dnv_vista_sdk_vis_locations returned NULL");
        // SAFETY: ptr is non-null and `Locations` is `#[repr(transparent)]` over the FFI type.
        unsafe { &*(ptr as *const Locations) }
    }

    pub(crate) fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_locations_t {
        self as *const Self as *const ffi::dnv_vista_sdk_locations_t
    }

    /// Returns the VIS version these locations belong to.
    pub fn version(&self) -> VisVersion {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_locations_version(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_locations_version returned NULL"
        );
        // SAFETY: ptr is non-null and owned by the library for the program's lifetime.
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in locations version");
        VisVersion::from_str(s).expect("unrecognized locations version string")
    }

    /// Returns the number of relative locations.
    pub fn relative_location_count(&self) -> usize {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_locations_relative_location_count(self.as_ffi_ptr()) }
    }

    /// Returns the relative location at `index`, or `None` if out of range.
    pub fn relative_location_at(&self, index: usize) -> Option<&RelativeLocation> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_locations_relative_location_at(self.as_ffi_ptr(), index) };
        if ptr.is_null() {
            return None;
        }
        Some(RelativeLocation::from_ptr(ptr))
    }

    /// Returns an iterator over all relative locations.
    pub fn relative_locations(&self) -> impl Iterator<Item = &RelativeLocation> {
        (0..self.relative_location_count()).map(move |i| {
            self.relative_location_at(i)
                .expect("index within count() must be valid")
        })
    }

    /// Returns the number of relative locations belonging to `group`.
    pub fn group_count(&self, group: LocationGroup) -> usize {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        unsafe {
            ffi::dnv_vista_sdk_locations_group_count(
                self.as_ffi_ptr(),
                location_group::to_ffi(group),
            )
        }
    }

    /// Returns the relative location at `index` within `group`, or `None` if out of range.
    pub fn group_at(&self, group: LocationGroup, index: usize) -> Option<&RelativeLocation> {
        // SAFETY: self is non-null and valid for the lifetime of `&self`.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_locations_group_at(
                self.as_ffi_ptr(),
                location_group::to_ffi(group),
                index,
            )
        };
        if ptr.is_null() {
            return None;
        }
        Some(RelativeLocation::from_ptr(ptr))
    }

    /// Returns an iterator over all relative locations belonging to `group`.
    pub fn group(&self, group: LocationGroup) -> impl Iterator<Item = &RelativeLocation> {
        (0..self.group_count(group)).map(move |i| {
            self.group_at(group, i)
                .expect("index within count() must be valid")
        })
    }

    /// Parses a location string (e.g. `"1PS"`).
    ///
    /// Returns `None` if the string is not a valid location.
    pub fn from_string(&self, location_str: &str) -> Option<Location> {
        let c_str = CString::new(location_str).ok()?;
        // SAFETY: self is non-null, and c_str is a valid NUL-terminated C string.
        let ptr =
            unsafe { ffi::dnv_vista_sdk_locations_from_string(self.as_ffi_ptr(), c_str.as_ptr()) };
        if ptr.is_null() {
            return None;
        }
        Some(Location(ptr))
    }

    /// Parses a location string, collecting detailed error information on failure.
    ///
    /// Returns the parsed [`Location`] (or `None` if invalid) alongside the
    /// [`ParsingErrors`] describing what went wrong (empty on success).
    pub fn from_string_with_errors(&self, location_str: &str) -> (Option<Location>, ParsingErrors) {
        let c_str = CString::new(location_str).expect("location string contains a NUL byte");
        let mut errors_ptr = ptr::null_mut();
        // SAFETY: self is non-null, c_str is a valid NUL-terminated C string, and
        // errors_ptr is a valid writable pointer to a local.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_locations_from_string_with_errors(
                self.as_ffi_ptr(),
                c_str.as_ptr(),
                &mut errors_ptr,
            )
        };
        assert!(
            !errors_ptr.is_null(),
            "dnv_vista_sdk_locations_from_string_with_errors did not set outErrors"
        );
        let location = if ptr.is_null() {
            None
        } else {
            Some(Location(ptr))
        };
        (location, ParsingErrors(errors_ptr))
    }
}
