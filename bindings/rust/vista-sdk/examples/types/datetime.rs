use vista_sdk::types::date_time::{DateTime, DateTimeFormat};
use vista_sdk::types::date_time_offset::DateTimeOffset;
use vista_sdk::types::time_span::TimeSpan;

fn main() {
    println!("=== vista-sdk DateTime samples ===\n");

    {
        println!("1. DateTime: Basic construction");
        println!("--------------------------------");

        let utc_now = DateTime::utc_now();
        let local_now = DateTimeOffset::now();
        let today = DateTimeOffset::today();
        let sensor_reading = DateTime::from_date_time(2026, 8, 22, 14, 30, 45);
        let epoch = DateTime::epoch();

        println!("Current UTC time:        {utc_now}");
        println!("Current LOCAL time:      {local_now}");
        println!("Today at midnight:       {today}");
        println!("Engine sensor timestamp: {sensor_reading}");
        println!("Unix epoch:              {epoch}");
        println!("Note: DateTime is always UTC - use DateTimeOffset for local time");
        println!();
    }

    {
        println!("2. DateTime: Parsing ISO 8601 strings");
        println!("--------------------------------------");

        let dt1 = DateTime::from_str("2026-08-22T14:30:45Z").expect("valid");
        let dt2 = DateTime::from_str("2026-08-22").expect("valid");

        println!("Parsed sensor timestamp \"2026-08-22T14:30:45Z\": {dt1}");
        println!("Parsed dry-dock date \"2026-08-22\":              {dt2}");

        match DateTime::from_str("2026-08-22T00:00:00Z") {
            Some(result) => {
                println!("fromString success:             true");
                println!("  Parsed value:                 {result}");
            }
            None => println!("fromString success:             false"),
        }
        println!();
    }

    {
        println!("3. DateTime: Accessing components");
        println!("----------------------------------");

        let dt = DateTime::from_date_time_millis(2026, 8, 22, 14, 30, 45, 123);
        println!(
            "Sensor timestamp: {}",
            dt.to_string_fmt(DateTimeFormat::Iso8601Precise)
        );
        println!("  Year:         {}", dt.year());
        println!("  Month:        {}", dt.month());
        println!("  Day:          {}", dt.day());
        println!("  Hour:         {}", dt.hour());
        println!("  Minute:       {}", dt.minute());
        println!("  Second:       {}", dt.second());
        println!("  Millisecond:  {}", dt.millisecond());
        println!("  Day of week:  {} (0=Sunday)", dt.day_of_week());
        println!("  Day of year:  {}", dt.day_of_year());
        println!();
    }

    {
        println!("4. DateTime: Arithmetic operations");
        println!("-----------------------------------");

        let departure = DateTime::from_date_time(2026, 8, 22, 10, 0, 0);
        println!("Departure (UTC):         {departure}");

        let eta = departure + TimeSpan::from_hours(2.5);
        println!("ETA (+2.5h transit):     {eta}");

        let delayed_departure = departure - TimeSpan::from_minutes(30.0);
        println!("Departure moved -30 min: {delayed_departure}");

        let transit_time = eta - departure;
        println!(
            "Transit time:            {} ({} hours)",
            transit_time,
            transit_time.hours()
        );
        println!();
    }

    {
        println!("5. DateTime: Comparisons");
        println!("------------------------");

        let inspection_start = DateTime::from_date_time(2026, 8, 22, 10, 0, 0);
        let inspection_end = DateTime::from_date_time(2026, 8, 22, 14, 0, 0);
        let duplicate_entry = DateTime::from_date_time(2026, 8, 22, 10, 0, 0);

        println!(
            "inspectionStart == duplicateEntry:  {}",
            inspection_start == duplicate_entry
        );
        println!(
            "inspectionStart != inspectionEnd:   {}",
            inspection_start != inspection_end
        );
        println!(
            "inspectionStart < inspectionEnd:    {}",
            inspection_start < inspection_end
        );
        println!(
            "inspectionEnd > inspectionStart:    {}",
            inspection_end > inspection_start
        );
        println!();
    }

    {
        println!("6. DateTime: Epoch timestamp conversions");
        println!("----------------------------------------");

        let dt = DateTime::from_date_time(2026, 8, 22, 0, 0, 0);
        println!("DateTime:              {dt}");
        println!("Unix seconds:          {}", dt.to_epoch_seconds());
        println!("Unix milliseconds:     {}", dt.to_epoch_millis());

        let from_epoch = DateTime::from_epoch_seconds(1_704_067_200);
        println!("From epoch 1704067200: {from_epoch}");
        println!();
    }

    {
        println!("7. DateTime: Leap year handling");
        println!("--------------------------------");

        println!("2024 is leap year:    {}", DateTime::is_leap_year(2024));
        println!("2023 is leap year:    {}", DateTime::is_leap_year(2023));
        println!("Days in Feb 2024:     {}", DateTime::days_in_month(2024, 2));
        println!("Days in Feb 2023:     {}", DateTime::days_in_month(2023, 2));

        let survey_date = DateTime::from_date_time(2024, 2, 29, 12, 0, 0);
        println!("Survey scheduled Feb 29, 2024: {survey_date}");
        println!();
    }

    {
        println!("8. TimeSpan: Creating durations");
        println!("--------------------------------");

        let voyage_duration = TimeSpan::from_days(1.5);
        let bunkering_duration = TimeSpan::from_hours(2.5);
        let port_stay = TimeSpan::from_minutes(90.0);
        let engine_warmup = TimeSpan::from_seconds(3600.0);

        println!("Voyage duration (1.5 days):    {voyage_duration}");
        println!("Bunkering duration (2.5h):     {bunkering_duration}");
        println!("Port stay (90 min):            {port_stay}");
        println!("Engine warmup (3600s):         {engine_warmup}");
        println!("Note: ISO 8601 duration format (PT = Period Time)");
        println!();
    }

    {
        println!("9. TimeSpan: Parsing ISO 8601 durations");
        println!("----------------------------------------");

        let ts1 = TimeSpan::from_str("PT1H").expect("valid");
        let ts2 = TimeSpan::from_str("PT1H30M45S").expect("valid");
        let ts3 = TimeSpan::from_str("P1DT12H").expect("valid");
        let ts4 = TimeSpan::from_str("-PT2H30M").expect("valid");

        println!("PT1H (watch rotation):         {} hours", ts1.hours());
        println!("PT1H30M45S (engine test run):  {} seconds", ts2.seconds());
        println!("P1DT12H (port turnaround):     {} hours", ts3.hours());
        println!("-PT2H30M (schedule offset):    {} minutes", ts4.minutes());
        println!();
    }

    {
        println!("10. TimeSpan: Unit conversions");
        println!("-------------------------------");

        let overhaul_duration = TimeSpan::from_hours(2.5);
        println!("Overhaul duration: 2.5 hours");
        println!("  Total days:          {}", overhaul_duration.days());
        println!("  Total hours:         {}", overhaul_duration.hours());
        println!("  Total minutes:       {}", overhaul_duration.minutes());
        println!("  Total seconds:       {}", overhaul_duration.seconds());
        println!("  Total milliseconds:  {}", overhaul_duration.millis());
        println!("  Ticks (100ns):       {}", overhaul_duration.ticks());
        println!();
    }

    {
        println!("11. TimeSpan: Arithmetic operations");
        println!("------------------------------------");

        let morning_watch = TimeSpan::from_hours(2.0);
        let handover_buffer = TimeSpan::from_minutes(30.0);

        println!("Morning watch (2h):      {morning_watch}");
        println!("Handover buffer (30m):   {handover_buffer}");
        println!(
            "watch + buffer:          {}",
            morning_watch + handover_buffer
        );
        println!(
            "watch - buffer:          {}",
            morning_watch - handover_buffer
        );
        println!("-watch:                  {}", -morning_watch);

        let full_shift =
            TimeSpan::from_days(1.0) + TimeSpan::from_hours(3.0) + TimeSpan::from_minutes(30.0);
        println!("1d + 3h + 30m shift:     {full_shift}");
        println!();
    }

    {
        println!("12. TimeSpan: Comparisons");
        println!("-------------------------");

        let short_anchorage = TimeSpan::from_minutes(30.0);
        let long_anchorage = TimeSpan::from_hours(2.0);

        println!(
            "30 min == 30 min:  {}",
            short_anchorage == TimeSpan::from_minutes(30.0)
        );
        println!("30 min < 2 hours:  {}", short_anchorage < long_anchorage);
        println!("2 hours > 30 min:  {}", long_anchorage > short_anchorage);
        println!();
    }

    {
        println!("13. DateTimeOffset: Basic construction with timezone");
        println!("-----------------------------------------------------");

        let now = DateTimeOffset::now();
        let utc_now = DateTimeOffset::utc_now();
        let yokohama = DateTimeOffset::new(
            DateTime::from_date_time(2026, 8, 22, 14, 30, 0),
            TimeSpan::from_hours(9.0),
        );
        let new_york = DateTimeOffset::new(
            DateTime::from_date_time(2026, 8, 22, 14, 30, 0),
            TimeSpan::from_hours(-5.0),
        );

        println!("Bridge local time:        {now}");
        println!("UTC time:                 {utc_now}");
        println!("Port of Yokohama (UTC+9): {yokohama}");
        println!("Port of New York (UTC-5): {new_york}");
        println!("Note: DateTimeOffset includes timezone offset");
        println!();
    }

    {
        println!("14. DateTimeOffset: Parsing with timezone offsets");
        println!("--------------------------------------------------");

        let dto1 = DateTimeOffset::from_str("2026-08-22T14:30:00+09:00").expect("valid");
        let dto2 = DateTimeOffset::from_str("2026-08-22T14:30:00-05:00").expect("valid");
        let dto3 = DateTimeOffset::from_str("2026-08-22T14:30:00Z").expect("valid");

        println!("Parsed +09:00:  {dto1}");
        println!("Parsed -05:00:  {dto2}");
        println!("Parsed Z (UTC): {dto3}");
        println!();
    }

    {
        println!("15. DateTimeOffset: Timezone conversions");
        println!("-----------------------------------------");

        let yokohama = DateTimeOffset::from_str("2026-08-22T18:00:00+09:00").expect("valid");
        println!("Arrival, Yokohama (UTC+9):  {yokohama}");

        let new_york = yokohama.to_offset(TimeSpan::from_hours(-5.0));
        println!("Same instant in New York:   {new_york}");

        let utc = yokohama.to_universal_time();
        println!("Same instant in UTC:        {utc}");

        println!(
            "\nAll represent same instant: {}",
            yokohama.utc_ticks() == new_york.utc_ticks()
        );
        println!();
    }

    {
        println!("16. DateTimeOffset: Accessing components");
        println!("-----------------------------------------");

        let dto = DateTimeOffset::new(
            DateTime::from_date_time(2026, 8, 22, 14, 30, 45),
            TimeSpan::from_hours(2.0),
        );
        println!("DateTimeOffset:     {dto}");
        println!("  Local time:       {}", dto.date_time());
        println!("  UTC time:         {}", dto.utc_date_time());
        println!("  Offset:           {}", dto.offset());
        println!("  Offset minutes:   {}", dto.total_offset_minutes());
        println!();
    }

    {
        println!("17. DateTimeOffset: Arithmetic operations");
        println!("------------------------------------------");

        let berth_available = DateTimeOffset::new(
            DateTime::from_date_time(2026, 8, 22, 10, 0, 0),
            TimeSpan::from_hours(2.0),
        );
        println!("Berth available from:  {berth_available}");

        let berth_deadline = berth_available + TimeSpan::from_hours(3.0);
        println!("Berth deadline (+3h):  {berth_deadline}");

        let yokohama_departure =
            DateTimeOffset::from_str("2026-08-22T18:00:00+09:00").expect("valid");
        let new_york_arrival =
            DateTimeOffset::from_str("2026-08-22T04:00:00-05:00").expect("valid");
        let diff = yokohama_departure - new_york_arrival;
        println!("\nYokohama departure:    {yokohama_departure}");
        println!("New York arrival:      {new_york_arrival}");
        println!("Difference (UTC):      {} (same UTC moment)", diff);
        println!();
    }

    {
        println!("18. DateTimeOffset: Comparisons (UTC-based)");
        println!("--------------------------------------------");

        let yokohama = DateTimeOffset::from_str("2026-08-22T18:00:00+09:00").expect("valid");
        let new_york = DateTimeOffset::from_str("2026-08-22T04:00:00-05:00").expect("valid");

        println!("Yokohama:                  {yokohama}");
        println!("New York:                  {new_york}");
        println!("Equal (same UTC moment):   {}", yokohama == new_york);
        println!(
            "Exact (incl offset):       {}",
            yokohama.equals_exact(new_york)
        );
        println!();
    }

    {
        println!("19. Integration: Maintenance task scheduling across timezones");
        println!("-----------------------------------------------------------------");

        let task_start = DateTime::from_date_time(2026, 8, 22, 9, 0, 0);
        let duration = TimeSpan::from_hours(2.0) + TimeSpan::from_minutes(30.0);
        let task_end = task_start + duration;

        println!("Maintenance starts (UTC): {task_start}");
        println!("Duration:                 {duration}");
        println!("Maintenance ends (UTC):   {task_end}");

        let end_utc = DateTimeOffset::new(task_end, TimeSpan::from_ticks(0));
        let end_yokohama = end_utc.to_offset(TimeSpan::from_hours(9.0));
        let end_ny = end_utc.to_offset(TimeSpan::from_hours(-5.0));
        println!("End reported to Yokohama: {end_yokohama}");
        println!("End reported to New York: {end_ny}");
        println!();
    }

    {
        println!("20. Integration: Port agent coordination across timezones");
        println!("-------------------------------------------------------------");

        let call_yokohama = DateTimeOffset::new(
            DateTime::from_date_time(2026, 8, 22, 14, 0, 0),
            TimeSpan::from_hours(9.0),
        );
        println!("Port agent call, Yokohama (UTC+9): {call_yokohama}");

        let call_ny = call_yokohama.to_offset(TimeSpan::from_hours(-5.0));
        let call_london = call_yokohama.to_offset(TimeSpan::from_hours(1.0));
        let call_sydney = call_yokohama.to_offset(TimeSpan::from_hours(10.0));

        println!(
            "Same call in New York:             {} ({}:00)",
            call_ny,
            call_ny.hour()
        );
        println!(
            "Same call in London:               {} ({}:00)",
            call_london,
            call_london.hour()
        );
        println!(
            "Same call in Sydney:               {} ({}:00)",
            call_sydney,
            call_sydney.hour()
        );
        println!();
    }
}
