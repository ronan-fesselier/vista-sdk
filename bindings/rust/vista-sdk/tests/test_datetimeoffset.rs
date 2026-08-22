use vista_sdk::types::date_time::DateTime;
use vista_sdk::types::date_time::DateTimeFormat;
use vista_sdk::types::date_time_offset::DateTimeOffset;
use vista_sdk::types::time_span::TimeSpan;

#[test]
fn date_time_offset_epoch() {
    let epoch = DateTimeOffset::epoch();
    assert_eq!(epoch.year(), 1970);
    assert_eq!(epoch.to_epoch_seconds(), 0);
    assert_eq!(epoch.total_offset_minutes(), 0);
}

#[test]
fn date_time_offset_new() {
    let dt = DateTime::from_date_time(2026, 7, 14, 14, 30, 0);
    let offset = TimeSpan::from_hours(9.0);
    let dto = DateTimeOffset::new(dt, offset);
    assert_eq!(dto.year(), 2026);
    assert_eq!(dto.hour(), 14);
    assert_eq!(dto.total_offset_minutes(), 540);
}

#[test]
fn date_time_offset_from_str_valid() {
    let dto = "2026-07-14T14:30:00+09:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    assert_eq!(dto.year(), 2026);
    assert_eq!(dto.total_offset_minutes(), 540);
}

#[test]
fn date_time_offset_from_str_invalid() {
    assert!("not-a-date".parse::<DateTimeOffset>().is_err());
}

#[test]
fn date_time_offset_utc_ticks_equal_for_same_instant() {
    let yokohama = "2026-07-14T18:00:00+09:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    let new_york = "2026-07-14T04:00:00-05:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    assert_eq!(yokohama.utc_ticks(), new_york.utc_ticks());
}

#[test]
fn date_time_offset_equals_same_instant_different_offset() {
    let yokohama = "2026-07-14T18:00:00+09:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    let new_york = "2026-07-14T04:00:00-05:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    assert_eq!(yokohama, new_york);
}

#[test]
fn date_time_offset_equals_exact_requires_same_offset() {
    let yokohama = "2026-07-14T18:00:00+09:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    let new_york = "2026-07-14T04:00:00-05:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    assert!(!yokohama.equals_exact(new_york));
    assert!(yokohama.equals_exact(yokohama));
}

#[test]
fn date_time_offset_to_offset() {
    let yokohama = "2026-07-14T18:00:00+09:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    let as_utc = yokohama.to_offset(TimeSpan::from_ticks(0));
    assert_eq!(as_utc.total_offset_minutes(), 0);
    assert_eq!(as_utc.hour(), 9);
}

#[test]
fn date_time_offset_add_sub_time_span() {
    let dto = "2026-07-14T10:00:00+02:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    let later = dto + TimeSpan::from_hours(3.0);
    assert_eq!(later.hour(), 13);
    let earlier = later - TimeSpan::from_hours(3.0);
    assert_eq!(earlier.hour(), 10);
}

#[test]
fn date_time_offset_sub_two_offsets() {
    let a = "2026-07-14T12:00:00+00:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    let b = "2026-07-14T10:00:00+00:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    let diff = a - b;
    assert!((diff.hours() - 2.0).abs() < 1e-6);
}

#[test]
fn date_time_offset_ordering() {
    let earlier = "2026-07-14T10:00:00+00:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    let later = "2026-07-14T14:00:00+00:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    assert!(earlier < later);
}

#[test]
fn date_time_offset_display() {
    let dto = "2026-07-14T14:30:00+02:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    assert!(!dto.to_string().is_empty());
}

#[test]
fn now_utc_now_today_are_valid() {
    assert!(DateTimeOffset::now().is_valid());
    assert!(DateTimeOffset::utc_now().is_valid());
    assert!(DateTimeOffset::today().is_valid());
}

#[test]
fn min_max_are_valid() {
    assert!(DateTimeOffset::min().is_valid());
    assert!(DateTimeOffset::max().is_valid());
    assert!(DateTimeOffset::min() < DateTimeOffset::max());
}

#[test]
fn from_ticks_round_trip() {
    let dto = "2026-07-14T12:00:00+00:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    let restored = DateTimeOffset::from_ticks(dto.ticks(), TimeSpan::from_ticks(0));
    assert_eq!(dto, restored);
}

#[test]
fn from_epoch_seconds_round_trip() {
    let seconds = 1_700_000_000i64;
    assert_eq!(
        DateTimeOffset::from_epoch_seconds(seconds).to_epoch_seconds(),
        seconds
    );
}

#[test]
fn from_epoch_millis_round_trip() {
    let millis = 1_700_000_000_000i64;
    assert_eq!(
        DateTimeOffset::from_epoch_millis(millis).to_epoch_millis(),
        millis
    );
}

#[test]
fn from_filetime_round_trip() {
    let dto = "2026-07-14T12:00:00+00:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    assert_eq!(dto, DateTimeOffset::from_filetime(dto.to_filetime()));
}

#[test]
fn from_date_time_preserves_components() {
    let dt = DateTime::from_date_time(2026, 7, 14, 10, 30, 0);
    let dto = DateTimeOffset::from_date_time(dt);
    assert_eq!(dto.year(), 2026);
    assert_eq!(dto.month(), 7);
    assert_eq!(dto.day(), 14);
    assert_eq!(dto.hour(), 10);
    assert_eq!(dto.minute(), 30);
    assert_eq!(dto.second(), 0);
}

#[test]
fn date_time_and_offset_accessors() {
    let dto = DateTimeOffset::new(
        DateTime::from_date_time(2026, 7, 14, 14, 30, 0),
        TimeSpan::from_hours(2.0),
    );
    assert_eq!(dto.date_time().year(), 2026);
    assert!((dto.offset().hours() - 2.0).abs() < 1e-6);
}

#[test]
fn utc_date_time_adjusts_for_offset() {
    let dto = "2026-07-14T14:00:00+02:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    assert_eq!(dto.utc_date_time().hour(), 12);
}

#[test]
fn date_strips_time() {
    let dto = "2026-07-14T14:30:45+02:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    let d = dto.date();
    assert_eq!(d.hour(), 0);
    assert_eq!(d.minute(), 0);
    assert_eq!(d.second(), 0);
}

#[test]
fn time_of_day_returns_elapsed_since_midnight() {
    let dto = "2026-07-14T14:30:00+00:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    assert!((dto.time_of_day().hours() - 14.5).abs() < 1e-6);
}

#[test]
fn to_universal_time_has_zero_offset() {
    let dto = "2026-07-14T18:00:00+09:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    let utc = dto.to_universal_time();
    assert_eq!(utc.total_offset_minutes(), 0);
    assert_eq!(utc.hour(), 9);
}

#[test]
fn add_arithmetic() {
    let dto = "2026-07-14T10:00:00+00:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    assert_eq!(dto.add_hours(2.0).hour(), 12);
    assert_eq!(dto.add_minutes(90.0).minute(), 30);
    assert_eq!(dto.add_seconds(3600.0).hour(), 11);
    assert_eq!(dto.add_millis(3_600_000.0).hour(), 11);
    assert_eq!(dto.add_days(1.0).day(), 15);
    assert_eq!(dto.add_months(1).month(), 8);
    assert_eq!(dto.add_years(1).year(), 2027);
}

#[test]
fn add_ticks_advances_time() {
    let dto = "2026-07-14T10:00:00+00:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    assert_eq!(dto.add_ticks(36_000_000_000i64).hour(), 11);
}

#[test]
fn to_string_fmt_iso8601() {
    let dto = "2026-07-14T10:00:00+00:00"
        .parse::<DateTimeOffset>()
        .expect("valid");
    let s = dto.to_string_fmt(DateTimeFormat::Iso8601);
    assert!(!s.is_empty());
    assert!(s.contains("2026"));
}

#[test]
fn parse_error_has_message() {
    let err = "not-a-date".parse::<DateTimeOffset>().unwrap_err();
    assert!(!err.message.is_empty());
}
