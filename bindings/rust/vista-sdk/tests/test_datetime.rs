use vista_sdk::*;

#[test]
fn time_span_from_ticks_roundtrips() {
    let ts = TimeSpan::from_ticks(9_000_000);
    assert_eq!(ts.ticks(), 9_000_000);
}

#[test]
fn time_span_from_hours_accessors() {
    let ts = TimeSpan::from_hours(2.5);
    assert!((ts.hours() - 2.5).abs() < 1e-9);
    assert!((ts.minutes() - 150.0).abs() < 1e-6);
}

#[test]
fn time_span_from_minutes() {
    let ts = TimeSpan::from_minutes(90.0);
    assert!((ts.hours() - 1.5).abs() < 1e-9);
}

#[test]
fn time_span_from_seconds() {
    let ts = TimeSpan::from_seconds(3600.0);
    assert!((ts.hours() - 1.0).abs() < 1e-9);
}

#[test]
fn time_span_from_millis() {
    let ts = TimeSpan::from_millis(1000.0);
    assert!((ts.seconds() - 1.0).abs() < 1e-9);
}

#[test]
fn time_span_from_str_iso8601() {
    let ts = "PT1H30M".parse::<TimeSpan>().expect("valid duration");
    assert!((ts.minutes() - 90.0).abs() < 1e-6);
}

#[test]
fn time_span_from_str_invalid() {
    assert!("not-a-duration".parse::<TimeSpan>().is_err());
}

#[test]
fn time_span_add() {
    let a = TimeSpan::from_hours(1.0);
    let b = TimeSpan::from_minutes(30.0);
    let sum = a + b;
    assert!((sum.hours() - 1.5).abs() < 1e-9);
}

#[test]
fn time_span_sub() {
    let a = TimeSpan::from_hours(2.0);
    let b = TimeSpan::from_minutes(30.0);
    let diff = a - b;
    assert!((diff.hours() - 1.5).abs() < 1e-9);
}

#[test]
fn time_span_neg() {
    let ts = TimeSpan::from_hours(1.0);
    let neg = -ts;
    assert!(neg.hours() < 0.0);
    assert!((neg.hours() + 1.0).abs() < 1e-9);
}

#[test]
fn time_span_multiply() {
    let ts = TimeSpan::from_hours(1.0) * 2.0;
    assert!((ts.hours() - 2.0).abs() < 1e-9);
}

#[test]
fn time_span_divide() {
    let ts = TimeSpan::from_hours(3.0).divide(2.0);
    assert!((ts.hours() - 1.5).abs() < 1e-9);
}

#[test]
fn time_span_ratio() {
    let a = TimeSpan::from_hours(3.0);
    let b = TimeSpan::from_hours(1.0);
    assert!((a.ratio(b) - 3.0).abs() < 1e-9);
}

#[test]
fn time_span_ordering() {
    let short = TimeSpan::from_minutes(30.0);
    let long = TimeSpan::from_hours(2.0);
    assert!(short < long);
    assert!(long > short);
    assert_eq!(short, TimeSpan::from_minutes(30.0));
}

#[test]
fn time_span_display() {
    let ts = TimeSpan::from_hours(1.5);
    let s = ts.to_string();
    assert!(!s.is_empty());
}

#[test]
fn date_time_epoch() {
    let epoch = DateTime::epoch();
    assert_eq!(epoch.year(), 1970);
    assert_eq!(epoch.month(), 1);
    assert_eq!(epoch.day(), 1);
    assert_eq!(epoch.to_epoch_seconds(), 0);
}

#[test]
fn date_time_from_date_time() {
    let dt = DateTime::from_date_time(2026, 7, 14, 14, 30, 45);
    assert_eq!(dt.year(), 2026);
    assert_eq!(dt.month(), 7);
    assert_eq!(dt.day(), 14);
    assert_eq!(dt.hour(), 14);
    assert_eq!(dt.minute(), 30);
    assert_eq!(dt.second(), 45);
}

#[test]
fn date_time_from_date_time_millis() {
    let dt = DateTime::from_date_time_millis(2026, 7, 14, 14, 30, 45, 123);
    assert_eq!(dt.millisecond(), 123);
}

#[test]
fn date_time_from_str_valid() {
    let dt = "2026-07-14T14:30:45Z".parse::<DateTime>().expect("valid");
    assert_eq!(dt.year(), 2026);
    assert_eq!(dt.month(), 7);
    assert_eq!(dt.day(), 14);
}

#[test]
fn date_time_from_str_invalid() {
    assert!("not-a-date".parse::<DateTime>().is_err());
}

#[test]
fn date_time_from_epoch_seconds_roundtrip() {
    let dt = DateTime::from_epoch_seconds(1_704_067_200);
    assert_eq!(dt.to_epoch_seconds(), 1_704_067_200);
}

#[test]
fn date_time_add_sub_time_span() {
    let departure = DateTime::from_date_time(2026, 7, 14, 10, 0, 0);
    let eta = departure + TimeSpan::from_hours(2.5);
    let transit = eta - departure;
    assert!((transit.hours() - 2.5).abs() < 1e-6);
}

#[test]
fn date_time_sub_time_span() {
    let dt = DateTime::from_date_time(2026, 7, 14, 12, 0, 0);
    let earlier = dt - TimeSpan::from_hours(1.0);
    assert_eq!(earlier.hour(), 11);
}

#[test]
fn date_time_ordering() {
    let a = DateTime::from_date_time(2026, 7, 14, 10, 0, 0);
    let b = DateTime::from_date_time(2026, 7, 14, 14, 0, 0);
    assert!(a < b);
    assert_eq!(a, DateTime::from_date_time(2026, 7, 14, 10, 0, 0));
}

#[test]
fn date_time_is_leap_year() {
    assert!(DateTime::is_leap_year(2024));
    assert!(!DateTime::is_leap_year(2023));
}

#[test]
fn date_time_days_in_month() {
    assert_eq!(DateTime::days_in_month(2024, 2), 29);
    assert_eq!(DateTime::days_in_month(2023, 2), 28);
}

#[test]
fn date_time_add_months() {
    let dt = DateTime::from_date_time(2026, 1, 31, 0, 0, 0);
    let next = dt.add_months(1);
    assert_eq!(next.month(), 2);
}

#[test]
fn date_time_to_string_fmt() {
    let dt = DateTime::from_date_time(2026, 7, 14, 0, 0, 0);
    let s = dt.to_string_fmt(DateTimeFormat::Iso8601Date);
    assert!(s.contains("2026-07-14"), "got: {s}");
}

#[test]
fn date_time_display() {
    let dt = DateTime::from_date_time(2026, 7, 14, 0, 0, 0);
    assert!(!dt.to_string().is_empty());
}

#[test]
fn date_time_from_str_invalid_returns_err() {
    assert!("not-a-date".parse::<DateTime>().is_err());
}

#[test]
fn date_time_from_str_valid_returns_ok() {
    let dt = "2026-07-14T12:00:00Z".parse::<DateTime>().expect("valid");
    assert!(!dt.to_string().is_empty());
}

#[test]
fn time_span_from_str_invalid_returns_err() {
    assert!("not-a-duration".parse::<TimeSpan>().is_err());
}

#[test]
fn time_span_from_str_valid_returns_ok() {
    let ts = "PT1H30M".parse::<TimeSpan>().expect("valid");
    assert!((ts.hours() - 1.5).abs() < 1e-9);
}

#[test]
fn date_time_parse_error_has_message() {
    let err = "not-a-date".parse::<DateTime>().unwrap_err();
    assert!(!err.message.is_empty());
}

#[test]
fn time_span_parse_error_has_message() {
    let err = "not-a-duration".parse::<TimeSpan>().unwrap_err();
    assert!(!err.message.is_empty());
}

#[test]
fn date_time_to_system_time_round_trip() {
    let dt = "2026-07-14T09:00:00Z".parse::<DateTime>().unwrap();
    let st = dt.to_system_time();
    let recovered = DateTime::from_system_time(st);
    assert_eq!(dt.to_epoch_seconds(), recovered.to_epoch_seconds());
}

#[test]
fn date_time_from_system_time_epoch() {
    let dt = DateTime::from_system_time(std::time::UNIX_EPOCH);
    assert_eq!(dt.to_epoch_seconds(), 0);
}

#[test]
fn date_time_to_system_time_preserves_sub_millisecond_ticks() {
    // 100 ticks == 10 microseconds, well below millisecond precision.
    let dt = DateTime::epoch() + TimeSpan::from_ticks(100);
    let st = dt.to_system_time();
    let expected = std::time::UNIX_EPOCH + std::time::Duration::from_nanos(10_000);
    assert_eq!(st, expected);
}

#[test]
fn date_time_to_system_time_before_epoch_with_fraction() {
    // 1969-12-31T23:59:59.500Z, i.e. -500ms
    let dt = DateTime::from_epoch_millis(-500);
    let st = dt.to_system_time();
    let expected = std::time::UNIX_EPOCH - std::time::Duration::from_millis(500);
    assert_eq!(st, expected);
    assert_eq!(
        DateTime::from_epoch_millis(dt.to_epoch_millis()).to_epoch_millis(),
        -500
    );
}

#[test]
fn time_span_to_duration_positive() {
    let ts = TimeSpan::from_seconds(3.5);
    let d = ts
        .to_duration()
        .expect("positive TimeSpan converts to Duration");
    assert_eq!(d.as_millis(), 3500);
}

#[test]
fn time_span_to_duration_negative_is_none() {
    let ts = TimeSpan::from_seconds(-1.0);
    assert!(ts.to_duration().is_none());
}

#[test]
fn time_span_from_duration_round_trip() {
    let d = std::time::Duration::from_millis(4500);
    let ts = TimeSpan::from_duration(d);
    assert!((ts.seconds() - 4.5).abs() < 1e-9);
    assert_eq!(ts.to_duration().unwrap(), d);
}

#[test]
fn date_time_utc_now_is_valid() {
    let now = DateTime::utc_now();
    assert!(now.is_valid());
    assert!(now.year() >= 2024);
}

#[test]
fn date_time_min_max_ordering() {
    assert!(DateTime::min() < DateTime::max());
    assert!(DateTime::min().ticks() < DateTime::max().ticks());
}

#[test]
fn date_time_from_ticks_roundtrip() {
    let dt = DateTime::from_date_time(2026, 1, 15, 10, 30, 0);
    let ticks = dt.ticks();
    assert_eq!(DateTime::from_ticks(ticks).ticks(), ticks);
}

#[test]
fn date_time_from_date_accessors() {
    let dt = DateTime::from_date(2026, 3, 25);
    assert_eq!(dt.year(), 2026);
    assert_eq!(dt.month(), 3);
    assert_eq!(dt.day(), 25);
    assert_eq!(dt.hour(), 0);
    assert_eq!(dt.minute(), 0);
    assert_eq!(dt.second(), 0);
}

#[test]
fn date_time_from_epoch_millis_roundtrip() {
    let ms = 1_700_000_000_000_i64;
    let dt = DateTime::from_epoch_millis(ms);
    assert_eq!(dt.to_epoch_millis(), ms);
}

#[test]
fn date_time_time_components() {
    let dt = DateTime::from_date_time(2026, 6, 15, 13, 45, 59);
    assert_eq!(dt.hour(), 13);
    assert_eq!(dt.minute(), 45);
    assert_eq!(dt.second(), 59);
}

#[test]
fn date_time_millisecond_microsecond_nanosecond() {
    let dt = DateTime::from_date_time_millis(2026, 1, 1, 0, 0, 0, 500);
    assert_eq!(dt.millisecond(), 500);
    assert!(dt.microsecond() >= 0);
    assert!(dt.nanosecond() >= 0);
}

#[test]
fn date_time_day_of_week_and_year() {
    let dt = DateTime::from_date(2026, 1, 1);
    assert_eq!(dt.day_of_year(), 1);
    let dow = dt.day_of_week();
    assert!((0..=6).contains(&dow));
}

#[test]
fn date_time_date_strips_time() {
    let dt = DateTime::from_date_time(2026, 5, 20, 14, 30, 45);
    let d = dt.date();
    assert_eq!(d.year(), 2026);
    assert_eq!(d.month(), 5);
    assert_eq!(d.day(), 20);
    assert_eq!(d.hour(), 0);
    assert_eq!(d.minute(), 0);
    assert_eq!(d.second(), 0);
}

#[test]
fn date_time_time_of_day() {
    let dt = DateTime::from_date_time(2026, 5, 20, 6, 0, 0);
    let tod = dt.time_of_day();
    assert!((tod.seconds() - 6.0 * 3600.0).abs() < 1e-3);
}

#[test]
fn date_time_is_valid_epoch() {
    assert!(DateTime::epoch().is_valid());
}

#[test]
fn date_time_add_days_hours_minutes_seconds() {
    let dt = DateTime::from_date_time(2026, 1, 1, 0, 0, 0);
    assert_eq!(dt.add_days(1.0).day(), 2);
    assert_eq!(dt.add_hours(2.0).hour(), 2);
    assert_eq!(dt.add_minutes(90.0).hour(), 1);
    assert_eq!(dt.add_seconds(3600.0).hour(), 1);
}

#[test]
fn date_time_add_years() {
    let dt = DateTime::from_date(2024, 2, 29);
    let next = dt.add_years(1);
    assert_eq!(next.year(), 2025);
}

#[test]
fn date_time_to_string_fmt_formats() {
    let dt = DateTime::from_date_time(2026, 7, 14, 12, 0, 0);
    let iso = dt.to_string_fmt(DateTimeFormat::Iso8601);
    assert!(iso.contains("2026"), "Iso8601: {iso}");
    let basic = dt.to_string_fmt(DateTimeFormat::Iso8601Basic);
    assert!(basic.contains("20260714"), "Iso8601Basic: {basic}");
    let date_only = dt.to_string_fmt(DateTimeFormat::Iso8601Date);
    assert_eq!(date_only, "2026-07-14");
}
