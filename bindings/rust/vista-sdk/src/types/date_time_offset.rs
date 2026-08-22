use std::ffi::CString;
use std::fmt;
use std::ops::{Add, Sub};

use crate::ffi::types::date_time_offset as ffi;
use crate::types::date_time::{DateTime, DateTimeFormat};
use crate::types::time_span::TimeSpan;

/// A local instant paired with a UTC offset.
#[repr(C)]
#[derive(Clone, Copy)]
pub struct DateTimeOffset {
    date_time: DateTime,
    offset: TimeSpan,
}

impl DateTimeOffset {
    /// Current local time with system timezone offset.
    #[inline]
    pub fn now() -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_offset_now() })
    }

    /// Current UTC time (offset = 00:00:00).
    #[inline]
    pub fn utc_now() -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_offset_utc_now() })
    }

    /// Current local date with time set to 00:00:00.
    #[inline]
    pub fn today() -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_offset_today() })
    }

    /// January 1, 0001 00:00:00.0000000, zero offset.
    #[inline]
    pub fn min() -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_offset_min() })
    }

    /// December 31, 9999 23:59:59.9999999, zero offset.
    #[inline]
    pub fn max() -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_offset_max() })
    }

    /// January 1, 1970 00:00:00 UTC, zero offset.
    #[inline]
    pub fn epoch() -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_offset_epoch() })
    }

    /// Constructs a `DateTimeOffset` from a `DateTime` and a UTC offset.
    #[inline]
    pub fn new(date_time: DateTime, offset: TimeSpan) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_create(date_time.into_ffi(), offset.into_ffi())
        })
    }

    /// Constructs a `DateTimeOffset` from a `DateTime`, using the system's local timezone offset.
    #[inline]
    pub fn from_date_time(date_time: DateTime) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_from_date_time(date_time.into_ffi())
        })
    }

    /// Constructs a `DateTimeOffset` from ticks and an explicit UTC offset.
    #[inline]
    pub fn from_ticks(ticks: i64, offset: TimeSpan) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_from_ticks(ticks, offset.into_ffi())
        })
    }

    /// Constructs a `DateTimeOffset` from Unix epoch seconds (UTC).
    #[inline]
    pub fn from_epoch_seconds(seconds: i64) -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_offset_from_epoch_seconds(seconds) })
    }

    /// Constructs a `DateTimeOffset` from Unix epoch milliseconds (UTC).
    #[inline]
    pub fn from_epoch_millis(milliseconds: i64) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_from_epoch_milliseconds(milliseconds)
        })
    }

    /// Constructs a `DateTimeOffset` from a Windows FILETIME value.
    #[inline]
    pub fn from_filetime(filetime: i64) -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_offset_from_filetime(filetime) })
    }

    /// Parses an ISO 8601 string with timezone offset (e.g. `"2026-07-14T14:30:00+09:00"`).
    ///
    /// Returns `None` if the string is invalid.
    pub fn from_str(s: &str) -> Option<Self> {
        let c = CString::new(s).ok()?;
        let mut result = ffi::dnv_vista_sdk_date_time_offset_t {
            date_time: crate::ffi::types::date_time::dnv_vista_sdk_date_time_t { ticks: 0 },
            offset: crate::ffi::types::time_span::dnv_vista_sdk_time_span_t { ticks: 0 },
        };
        let ok =
            unsafe { ffi::dnv_vista_sdk_date_time_offset_from_string(c.as_ptr(), &mut result) };
        if ok != 0 {
            Some(Self::from_ffi(result))
        } else {
            None
        }
    }

    /// Local `DateTime` component.
    #[inline]
    pub fn date_time(self) -> DateTime {
        DateTime::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_date_time(self.into_ffi())
        })
    }

    /// UTC offset as a `TimeSpan`.
    #[inline]
    pub fn offset(self) -> TimeSpan {
        TimeSpan::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_offset_offset(self.into_ffi()) })
    }

    /// Equivalent UTC `DateTime`.
    #[inline]
    pub fn utc_date_time(self) -> DateTime {
        DateTime::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_utc_date_time(self.into_ffi())
        })
    }

    /// Equivalent local `DateTime` (system timezone).
    #[inline]
    pub fn local_date_time(self) -> DateTime {
        DateTime::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_local_date_time(self.into_ffi())
        })
    }

    /// Local time as 100 ns ticks.
    #[inline]
    pub fn ticks(self) -> i64 {
        unsafe { ffi::dnv_vista_sdk_date_time_offset_ticks(self.into_ffi()) }
    }

    /// UTC time as 100 ns ticks.
    #[inline]
    pub fn utc_ticks(self) -> i64 {
        unsafe { ffi::dnv_vista_sdk_date_time_offset_utc_ticks(self.into_ffi()) }
    }

    /// Year component.
    #[inline]
    pub fn year(self) -> i32 {
        unsafe { ffi::dnv_vista_sdk_date_time_offset_year(self.into_ffi()) }
    }

    /// Month component (1–12).
    #[inline]
    pub fn month(self) -> i32 {
        unsafe { ffi::dnv_vista_sdk_date_time_offset_month(self.into_ffi()) }
    }

    /// Day component (1–31).
    #[inline]
    pub fn day(self) -> i32 {
        unsafe { ffi::dnv_vista_sdk_date_time_offset_day(self.into_ffi()) }
    }

    /// Hour component (0–23).
    #[inline]
    pub fn hour(self) -> i32 {
        unsafe { ffi::dnv_vista_sdk_date_time_offset_hour(self.into_ffi()) }
    }

    /// Minute component (0–59).
    #[inline]
    pub fn minute(self) -> i32 {
        unsafe { ffi::dnv_vista_sdk_date_time_offset_minute(self.into_ffi()) }
    }

    /// Second component (0–59).
    #[inline]
    pub fn second(self) -> i32 {
        unsafe { ffi::dnv_vista_sdk_date_time_offset_second(self.into_ffi()) }
    }

    /// Millisecond component (0–999).
    #[inline]
    pub fn millisecond(self) -> i32 {
        unsafe { ffi::dnv_vista_sdk_date_time_offset_millisecond(self.into_ffi()) }
    }

    /// Microsecond component (0–999).
    #[inline]
    pub fn microsecond(self) -> i32 {
        unsafe { ffi::dnv_vista_sdk_date_time_offset_microsecond(self.into_ffi()) }
    }

    /// Nanosecond component in 100 ns increments (0, 100, …, 900).
    #[inline]
    pub fn nanosecond(self) -> i32 {
        unsafe { ffi::dnv_vista_sdk_date_time_offset_nanosecond(self.into_ffi()) }
    }

    /// Day of week (0 = Sunday, 6 = Saturday).
    #[inline]
    pub fn day_of_week(self) -> i32 {
        unsafe { ffi::dnv_vista_sdk_date_time_offset_day_of_week(self.into_ffi()) }
    }

    /// Day of year (1–366).
    #[inline]
    pub fn day_of_year(self) -> i32 {
        unsafe { ffi::dnv_vista_sdk_date_time_offset_day_of_year(self.into_ffi()) }
    }

    /// Total minutes offset from UTC (positive for East, negative for West).
    #[inline]
    pub fn total_offset_minutes(self) -> i32 {
        unsafe { ffi::dnv_vista_sdk_date_time_offset_total_offset_minutes(self.into_ffi()) }
    }

    /// Unix timestamp in whole seconds.
    #[inline]
    pub fn to_epoch_seconds(self) -> i64 {
        unsafe { ffi::dnv_vista_sdk_date_time_offset_to_epoch_seconds(self.into_ffi()) }
    }

    /// Unix timestamp in milliseconds.
    #[inline]
    pub fn to_epoch_millis(self) -> i64 {
        unsafe { ffi::dnv_vista_sdk_date_time_offset_to_epoch_milliseconds(self.into_ffi()) }
    }

    /// Windows FILETIME (100 ns intervals since January 1, 1601 UTC).
    #[inline]
    pub fn to_filetime(self) -> i64 {
        unsafe { ffi::dnv_vista_sdk_date_time_offset_to_filetime(self.into_ffi()) }
    }

    /// Date component with time set to 00:00:00.
    #[inline]
    pub fn date(self) -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_date_time_offset_date(self.into_ffi()) })
    }

    /// Elapsed time since midnight.
    #[inline]
    pub fn time_of_day(self) -> TimeSpan {
        TimeSpan::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_time_of_day(self.into_ffi())
        })
    }

    /// Converts to a different UTC offset, representing the same instant.
    #[inline]
    pub fn to_offset(self, new_offset: TimeSpan) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_to_offset(self.into_ffi(), new_offset.into_ffi())
        })
    }

    /// Converts to UTC (offset = 00:00:00).
    #[inline]
    pub fn to_universal_time(self) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_to_universal_time(self.into_ffi())
        })
    }

    /// Converts to local time (system timezone).
    #[inline]
    pub fn to_local_time(self) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_to_local_time(self.into_ffi())
        })
    }

    /// Returns `true` if both represent the same instant in UTC (offsets may differ).
    #[inline]
    pub fn equals(self, other: Self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_date_time_offset_equals(self.into_ffi(), other.into_ffi()) })
            != 0
    }

    /// Returns `true` if both have the same local time AND the same offset.
    #[inline]
    pub fn equals_exact(self, other: Self) -> bool {
        (unsafe {
            ffi::dnv_vista_sdk_date_time_offset_equals_exact(self.into_ffi(), other.into_ffi())
        }) != 0
    }

    /// Returns `true` if this value falls within the representable range.
    #[inline]
    pub fn is_valid(self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_date_time_offset_is_valid(self.into_ffi()) }) != 0
    }

    /// Returns this value with the given number of days added (fractional).
    #[inline]
    pub fn add_days(self, days: f64) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_add_days(self.into_ffi(), days)
        })
    }

    /// Returns this value with the given number of hours added (fractional).
    #[inline]
    pub fn add_hours(self, hours: f64) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_add_hours(self.into_ffi(), hours)
        })
    }

    /// Returns this value with the given number of minutes added (fractional).
    #[inline]
    pub fn add_minutes(self, minutes: f64) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_add_minutes(self.into_ffi(), minutes)
        })
    }

    /// Returns this value with the given number of seconds added (fractional).
    #[inline]
    pub fn add_seconds(self, seconds: f64) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_add_seconds(self.into_ffi(), seconds)
        })
    }

    /// Returns this value with the given number of milliseconds added (fractional).
    #[inline]
    pub fn add_millis(self, milliseconds: f64) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_add_milliseconds(self.into_ffi(), milliseconds)
        })
    }

    /// Returns this value with the given number of calendar months added.
    #[inline]
    pub fn add_months(self, months: i32) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_add_months(self.into_ffi(), months)
        })
    }

    /// Returns this value with the given number of calendar years added.
    #[inline]
    pub fn add_years(self, years: i32) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_add_years(self.into_ffi(), years)
        })
    }

    /// Returns this value with the given number of ticks added.
    #[inline]
    pub fn add_ticks(self, ticks: i64) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_add_ticks(self.into_ffi(), ticks)
        })
    }

    /// Converts this value to a string using the given format.
    pub fn to_string_fmt(self, format: DateTimeFormat) -> String {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_date_time_offset_to_string(
                self.into_ffi(),
                std::mem::transmute(format),
            )
        };
        if ptr.is_null() {
            return String::new();
        }
        let s = unsafe { std::ffi::CStr::from_ptr(ptr).to_string_lossy().into_owned() };
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        s
    }

    #[inline]
    pub(crate) fn into_ffi(self) -> ffi::dnv_vista_sdk_date_time_offset_t {
        ffi::dnv_vista_sdk_date_time_offset_t {
            date_time: self.date_time.into_ffi(),
            offset: self.offset.into_ffi(),
        }
    }

    #[inline]
    pub(crate) fn from_ffi(f: ffi::dnv_vista_sdk_date_time_offset_t) -> Self {
        Self {
            date_time: DateTime::from_ffi(f.date_time),
            offset: TimeSpan::from_ffi(f.offset),
        }
    }
}

impl PartialEq for DateTimeOffset {
    /// Two `DateTimeOffset` values are equal if they represent the same UTC instant.
    #[inline]
    fn eq(&self, other: &Self) -> bool {
        self.equals(*other)
    }
}

impl Eq for DateTimeOffset {}

impl PartialOrd for DateTimeOffset {
    #[inline]
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}

impl Ord for DateTimeOffset {
    #[inline]
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        let c = unsafe {
            ffi::dnv_vista_sdk_date_time_offset_compare(self.into_ffi(), other.into_ffi())
        };
        c.cmp(&0)
    }
}

impl Add<TimeSpan> for DateTimeOffset {
    type Output = Self;
    #[inline]
    fn add(self, rhs: TimeSpan) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_add_time_span(self.into_ffi(), rhs.into_ffi())
        })
    }
}

impl Sub<TimeSpan> for DateTimeOffset {
    type Output = Self;
    #[inline]
    fn sub(self, rhs: TimeSpan) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_subtract_time_span(self.into_ffi(), rhs.into_ffi())
        })
    }
}

impl Sub for DateTimeOffset {
    type Output = TimeSpan;
    #[inline]
    fn sub(self, rhs: Self) -> TimeSpan {
        TimeSpan::from_ffi(unsafe {
            ffi::dnv_vista_sdk_date_time_offset_subtract(self.into_ffi(), rhs.into_ffi())
        })
    }
}

impl fmt::Display for DateTimeOffset {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(&self.to_string_fmt(DateTimeFormat::Iso8601))
    }
}

impl fmt::Debug for DateTimeOffset {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "DateTimeOffset({})", self)
    }
}
