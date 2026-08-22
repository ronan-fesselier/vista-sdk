use std::ffi::CString;
use std::fmt;
use std::ops::{Add, Sub};

use crate::ffi::types::date_time as ffi;
use crate::types::time_span::TimeSpan;

/// Format used when converting a `DateTime` or `DateTimeOffset` to a string.
#[repr(C)]
pub enum DateTimeFormat {
    Iso8601 = 0,
    Iso8601Precise,
    Iso8601PreciseTrimmed,
    Iso8601Millis,
    Iso8601Micros,
    Iso8601Extended,
    Iso8601Basic,
    Iso8601Date,
    Iso8601Time,
    UnixSeconds,
    UnixMilliseconds,
}

/// A UTC instant expressed as 100-nanosecond ticks since January 1, 0001.
#[repr(C)]
#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct DateTime {
    ticks: i64,
}

impl DateTime {
    /// Current UTC time.
    #[inline]
    pub fn utc_now() -> Self {
        // SAFETY: no arguments; returns a plain struct by value.
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_utc_now() })
    }

    /// January 1, 0001 00:00:00 UTC.
    #[inline]
    pub fn min() -> Self {
        // SAFETY: no arguments; returns a plain struct by value.
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_min() })
    }

    /// December 31, 9999 23:59:59.9999999 UTC.
    #[inline]
    pub fn max() -> Self {
        // SAFETY: no arguments; returns a plain struct by value.
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_max() })
    }

    /// January 1, 1970 00:00:00 UTC.
    #[inline]
    pub fn epoch() -> Self {
        // SAFETY: no arguments; returns a plain struct by value.
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_epoch() })
    }

    /// Constructs a `DateTime` from a raw 100-nanosecond tick count.
    #[inline]
    pub fn from_ticks(ticks: i64) -> Self {
        // SAFETY: no pointer arguments; returns a plain struct by value.
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_from_ticks(ticks) })
    }

    /// Constructs a `DateTime` from date components (UTC).
    ///
    /// Returns `DateTime::min()` if the components are invalid.
    #[inline]
    pub fn from_date(year: i32, month: i32, day: i32) -> Self {
        // SAFETY: no pointer arguments; returns a plain struct by value.
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_from_date(year, month, day) })
    }

    /// Constructs a `DateTime` from date and time components (UTC).
    ///
    /// Returns `DateTime::min()` if the components are invalid.
    #[inline]
    pub fn from_date_time(
        year: i32,
        month: i32,
        day: i32,
        hour: i32,
        minute: i32,
        second: i32,
    ) -> Self {
        // SAFETY: no pointer arguments; returns a plain struct by value.
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_from_date_time(year, month, day, hour, minute, second)
        })
    }

    /// Constructs a `DateTime` from date and time components with milliseconds (UTC).
    ///
    /// Returns `DateTime::min()` if the components are invalid.
    #[inline]
    pub fn from_date_time_millis(
        year: i32,
        month: i32,
        day: i32,
        hour: i32,
        minute: i32,
        second: i32,
        millisecond: i32,
    ) -> Self {
        // SAFETY: no pointer arguments; returns a plain struct by value.
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_from_date_time_millis(
                year,
                month,
                day,
                hour,
                minute,
                second,
                millisecond,
            )
        })
    }

    /// Parses an ISO 8601 string (e.g. `"2026-07-14T14:30:45Z"`).
    ///
    /// Returns `None` if the string is invalid.
    pub fn from_str(s: &str) -> Option<Self> {
        let c = CString::new(s).ok()?;
        let mut result = ffi::dnv_vista_sdk_date_time_t { ticks: 0 };
        // SAFETY: c is a valid NUL-terminated C string, and `result` is a valid writable pointer to a local.
        let ok = unsafe { ffi::dnv_vista_sdk_date_time_from_string(c.as_ptr(), &mut result) };
        if ok != 0 {
            Some(Self::from_ffi(result))
        } else {
            None
        }
    }

    /// Constructs a `DateTime` from Unix epoch seconds.
    #[inline]
    pub fn from_epoch_seconds(seconds: i64) -> Self {
        // SAFETY: no pointer arguments; returns a plain struct by value.
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_from_epoch_seconds(seconds) })
    }

    /// Constructs a `DateTime` from Unix epoch milliseconds.
    #[inline]
    pub fn from_epoch_millis(milliseconds: i64) -> Self {
        // SAFETY: no pointer arguments; returns a plain struct by value.
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_from_epoch_milliseconds(milliseconds)
        })
    }

    /// Raw 100-nanosecond tick count since January 1, 0001.
    #[inline]
    pub fn ticks(self) -> i64 {
        self.ticks
    }

    /// Year component.
    #[inline]
    pub fn year(self) -> i32 {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_date_time_year(self.into_ffi()) }
    }

    /// Month component (1–12).
    #[inline]
    pub fn month(self) -> i32 {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_date_time_month(self.into_ffi()) }
    }

    /// Day component (1–31).
    #[inline]
    pub fn day(self) -> i32 {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_date_time_day(self.into_ffi()) }
    }

    /// Hour component (0–23).
    #[inline]
    pub fn hour(self) -> i32 {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_date_time_hour(self.into_ffi()) }
    }

    /// Minute component (0–59).
    #[inline]
    pub fn minute(self) -> i32 {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_date_time_minute(self.into_ffi()) }
    }

    /// Second component (0–59).
    #[inline]
    pub fn second(self) -> i32 {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_date_time_second(self.into_ffi()) }
    }

    /// Millisecond component (0–999).
    #[inline]
    pub fn millisecond(self) -> i32 {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_date_time_millisecond(self.into_ffi()) }
    }

    /// Microsecond component (0–999).
    #[inline]
    pub fn microsecond(self) -> i32 {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_date_time_microsecond(self.into_ffi()) }
    }

    /// Nanosecond component in 100 ns increments (0, 100, …, 900).
    #[inline]
    pub fn nanosecond(self) -> i32 {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_date_time_nanosecond(self.into_ffi()) }
    }

    /// Day of week (0 = Sunday, 6 = Saturday).
    #[inline]
    pub fn day_of_week(self) -> i32 {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_date_time_day_of_week(self.into_ffi()) }
    }

    /// Day of year (1–366).
    #[inline]
    pub fn day_of_year(self) -> i32 {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_date_time_day_of_year(self.into_ffi()) }
    }

    /// Unix timestamp in whole seconds.
    #[inline]
    pub fn to_epoch_seconds(self) -> i64 {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_date_time_to_epoch_seconds(self.into_ffi()) }
    }

    /// Unix timestamp in milliseconds.
    #[inline]
    pub fn to_epoch_millis(self) -> i64 {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_date_time_to_epoch_milliseconds(self.into_ffi()) }
    }

    /// Date component with time set to 00:00:00.
    #[inline]
    pub fn date(self) -> Self {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_date(self.into_ffi()) })
    }

    /// Elapsed time since midnight.
    #[inline]
    pub fn time_of_day(self) -> TimeSpan {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        TimeSpan::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_time_of_day(self.into_ffi()) })
    }

    /// Returns `true` if this value falls within the representable range.
    #[inline]
    pub fn is_valid(self) -> bool {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        (unsafe { ffi::dnv_vista_sdk_date_time_is_valid(self.into_ffi()) }) != 0
    }

    /// Returns `true` if the given year is a leap year.
    #[inline]
    pub fn is_leap_year(year: i32) -> bool {
        // SAFETY: no pointer arguments.
        (unsafe { ffi::dnv_vista_sdk_date_time_is_leap_year(year) }) != 0
    }

    /// Number of days in the given month of the given year.
    #[inline]
    pub fn days_in_month(year: i32, month: i32) -> i32 {
        // SAFETY: no pointer arguments.
        unsafe { ffi::dnv_vista_sdk_date_time_days_in_month(year, month) }
    }

    /// Returns this value with the given number of days added (fractional).
    #[inline]
    pub fn add_days(self, days: f64) -> Self {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_add_days(self.into_ffi(), days) })
    }

    /// Returns this value with the given number of hours added (fractional).
    #[inline]
    pub fn add_hours(self, hours: f64) -> Self {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_add_hours(self.into_ffi(), hours) })
    }

    /// Returns this value with the given number of minutes added (fractional).
    #[inline]
    pub fn add_minutes(self, minutes: f64) -> Self {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_add_minutes(self.into_ffi(), minutes)
        })
    }

    /// Returns this value with the given number of seconds added (fractional).
    #[inline]
    pub fn add_seconds(self, seconds: f64) -> Self {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_add_seconds(self.into_ffi(), seconds)
        })
    }

    /// Returns this value with the given number of milliseconds added (fractional).
    #[inline]
    pub fn add_millis(self, milliseconds: f64) -> Self {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_add_milliseconds(self.into_ffi(), milliseconds)
        })
    }

    /// Returns this value with the given number of calendar months added.
    ///
    /// If the resulting day exceeds the days in the target month, it is clamped.
    #[inline]
    pub fn add_months(self, months: i32) -> Self {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_add_months(self.into_ffi(), months) })
    }

    /// Returns this value with the given number of calendar years added.
    #[inline]
    pub fn add_years(self, years: i32) -> Self {
        // SAFETY: no pointer arguments; self.into_ffi() is a plain struct passed by value.
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_add_years(self.into_ffi(), years) })
    }

    /// Converts this value to a string using the given format.
    pub fn to_string_fmt(self, format: DateTimeFormat) -> String {
        let ffi_format = match format {
            DateTimeFormat::Iso8601 => ffi::dnv_vista_sdk_date_time_format_t::Iso8601,
            DateTimeFormat::Iso8601Precise => ffi::dnv_vista_sdk_date_time_format_t::Iso8601Precise,
            DateTimeFormat::Iso8601PreciseTrimmed => {
                ffi::dnv_vista_sdk_date_time_format_t::Iso8601PreciseTrimmed
            }
            DateTimeFormat::Iso8601Millis => ffi::dnv_vista_sdk_date_time_format_t::Iso8601Millis,
            DateTimeFormat::Iso8601Micros => ffi::dnv_vista_sdk_date_time_format_t::Iso8601Micros,
            DateTimeFormat::Iso8601Extended => {
                ffi::dnv_vista_sdk_date_time_format_t::Iso8601Extended
            }
            DateTimeFormat::Iso8601Basic => ffi::dnv_vista_sdk_date_time_format_t::Iso8601Basic,
            DateTimeFormat::Iso8601Date => ffi::dnv_vista_sdk_date_time_format_t::Iso8601Date,
            DateTimeFormat::Iso8601Time => ffi::dnv_vista_sdk_date_time_format_t::Iso8601Time,
            DateTimeFormat::UnixSeconds => ffi::dnv_vista_sdk_date_time_format_t::UnixSeconds,
            DateTimeFormat::UnixMilliseconds => {
                ffi::dnv_vista_sdk_date_time_format_t::UnixMilliseconds
            }
        };
        // SAFETY: self.into_ffi() is a plain struct passed by value; ffi_format is a valid discriminant.
        let ptr = unsafe { ffi::dnv_vista_sdk_date_time_to_string(self.into_ffi(), ffi_format) };
        if ptr.is_null() {
            return String::new();
        }
        // SAFETY: ptr is non-null (checked above) and owned until freed below.
        let s = unsafe { std::ffi::CStr::from_ptr(ptr).to_string_lossy().into_owned() };
        // SAFETY: ptr was allocated by the library and is freed exactly once.
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        s
    }

    #[inline]
    pub(crate) fn into_ffi(self) -> ffi::dnv_vista_sdk_date_time_t {
        ffi::dnv_vista_sdk_date_time_t { ticks: self.ticks }
    }

    #[inline]
    pub(crate) fn from_ffi(ffi: ffi::dnv_vista_sdk_date_time_t) -> Self {
        Self { ticks: ffi.ticks }
    }
}

impl Add<TimeSpan> for DateTime {
    type Output = Self;
    #[inline]
    fn add(self, rhs: TimeSpan) -> Self {
        // SAFETY: no pointer arguments; self.into_ffi()/rhs.into_ffi() are plain structs passed by value.
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_add_time_span(self.into_ffi(), rhs.into_ffi())
        })
    }
}

impl Sub<TimeSpan> for DateTime {
    type Output = Self;
    #[inline]
    fn sub(self, rhs: TimeSpan) -> Self {
        // SAFETY: no pointer arguments; self.into_ffi()/rhs.into_ffi() are plain structs passed by value.
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_subtract_time_span(self.into_ffi(), rhs.into_ffi())
        })
    }
}

impl Sub for DateTime {
    type Output = TimeSpan;
    #[inline]
    fn sub(self, rhs: Self) -> TimeSpan {
        // SAFETY: no pointer arguments; self.into_ffi()/rhs.into_ffi() are plain structs passed by value.
        TimeSpan::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_subtract(self.into_ffi(), rhs.into_ffi())
        })
    }
}

impl fmt::Display for DateTime {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(&self.to_string_fmt(DateTimeFormat::Iso8601))
    }
}

impl fmt::Debug for DateTime {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "DateTime({})", self)
    }
}
