//! Temporal and numeric value types: `DateTime`, `DateTimeOffset`, `TimeSpan`, `Decimal`.

/// UTC instant with 100-nanosecond precision.
pub mod date_time;
/// UTC instant paired with a fixed timezone offset.
pub mod date_time_offset;
/// Fixed-point decimal number with up to 28 significant digits.
pub mod decimal;
/// Duration with 100-nanosecond precision.
pub mod time_span;
