using VistaSdk
import Dates

println("=== VistaSdk.jl DateTime Sample ===\n")

let
    println("1. DateTime: Basic construction")
    println("--------------------------------")

    utc = utc_now(DateTime)
    local_now = now(DateTimeOffset)
    today_dto = today(DateTimeOffset)
    sensor = DateTime(2026, 8, 22, 14, 30, 45)
    ep = epoch(DateTime)

    println("  Current UTC time:        $(Base.string(utc))")
    println("  Current LOCAL time:      $(Base.string(local_now))")
    println("  Today at midnight:       $(Base.string(today_dto))")
    println("  Engine sensor timestamp: $(Base.string(sensor))")
    println("  Unix epoch:              $(Base.string(ep))")
    println()
end

let
    println("2. DateTime: Parsing ISO 8601 strings")
    println("--------------------------------------")

    dt1 = Base.parse(DateTime, "2026-08-22T14:30:45Z")
    dt2 = Base.parse(DateTime, "2026-08-22")
    println("  Parsed \"2026-08-22T14:30:45Z\": $(Base.string(dt1))")
    println("  Parsed \"2026-08-22\":           $(Base.string(dt2))")

    try
        dt3 = Base.parse(DateTime, "2026-08-22T00:00:00Z")
        println("  from_string success: true  => $(Base.string(dt3))")
    catch
        println("  from_string success: false")
    end
    println()
end

let
    println("3. DateTime: Accessing components")
    println("----------------------------------")

    dt = DateTime(2026, 8, 22, 14, 30, 45, 123)
    println("  Sensor timestamp: $(to_string(dt, Iso8601Precise))")
    println("  Year:         $(year(dt))")
    println("  Month:        $(month(dt))")
    println("  Day:          $(day(dt))")
    println("  Hour:         $(hour(dt))")
    println("  Minute:       $(minute(dt))")
    println("  Second:       $(second(dt))")
    println("  Millisecond:  $(millisecond(dt))")
    println("  Day of week:  $(day_of_week(dt)) (0=Sunday)")
    println("  Day of year:  $(day_of_year(dt))")
    println()
end

let
    println("4. DateTime: Arithmetic operations")
    println("-----------------------------------")

    departure = DateTime(2026, 8, 22, 10, 0, 0)
    println("  Departure (UTC):         $(Base.string(departure))")

    eta = departure + from_hours(TimeSpan, 2.5)
    println("  ETA (+2.5h transit):     $(Base.string(eta))")

    early = departure - from_minutes(TimeSpan, 30.0)
    println("  Departure moved -30 min: $(Base.string(early))")

    transit = eta - departure
    println("  Transit time:            $(Base.string(transit)) ($(hours(transit)) hours)")
    println()
end

let
    println("5. DateTime: Comparisons")
    println("------------------------")

    a = DateTime(2026, 8, 22, 10, 0, 0)
    b = DateTime(2026, 8, 22, 14, 0, 0)
    dup = DateTime(2026, 8, 22, 10, 0, 0)

    println("  a == dup:  $(a == dup)")
    println("  a != b:    $(a != b)")
    println("  a < b:     $(a < b)")
    println("  b > a:     $(b > a)")
    println()
end

let
    println("6. DateTime: Epoch timestamp conversions")
    println("-----------------------------------------")

    dt = DateTime(2026, 8, 22, 0, 0, 0)
    println("  DateTime:              $(Base.string(dt))")
    println("  Unix seconds:          $(to_epoch_seconds(dt))")
    println("  Unix milliseconds:     $(to_epoch_millis(dt))")

    from_ep = from_epoch_seconds(DateTime, 1_704_067_200)
    println("  From epoch 1704067200: $(Base.string(from_ep))")
    println()
end

let
    println("7. DateTime: Leap year handling")
    println("--------------------------------")

    println("  2024 is leap year:  $(is_leap_year(DateTime, 2024))")
    println("  2023 is leap year:  $(is_leap_year(DateTime, 2023))")
    println("  Days in Feb 2024:   $(days_in_month(DateTime, 2024, 2))")
    println("  Days in Feb 2023:   $(days_in_month(DateTime, 2023, 2))")

    survey = DateTime(2024, 2, 29, 12, 0, 0)
    println("  Survey Feb 29, 2024: $(Base.string(survey))")
    println()
end

let
    println("8. TimeSpan: Creating durations")
    println("--------------------------------")

    voyage = from_days(TimeSpan, 1.5)
    bunkering = from_hours(TimeSpan, 2.5)
    port_stay = from_minutes(TimeSpan, 90.0)
    warmup = from_seconds(TimeSpan, 3600.0)

    println("  Voyage (1.5 days):    $(Base.string(voyage))")
    println("  Bunkering (2.5h):     $(Base.string(bunkering))")
    println("  Port stay (90 min):   $(Base.string(port_stay))")
    println("  Engine warmup (3600s):$(Base.string(warmup))")
    println()
end

let
    println("9. TimeSpan: Parsing ISO 8601 durations")
    println("----------------------------------------")

    ts1 = Base.parse(TimeSpan, "PT1H")
    ts2 = Base.parse(TimeSpan, "PT1H30M45S")
    ts3 = Base.parse(TimeSpan, "P1DT12H")
    ts4 = Base.parse(TimeSpan, "-PT2H30M")

    println("  PT1H (watch rotation):         $(hours(ts1)) hours")
    println("  PT1H30M45S (engine test run):  $(seconds(ts2)) seconds")
    println("  P1DT12H (port turnaround):     $(hours(ts3)) hours")
    println("  -PT2H30M (schedule offset):    $(minutes(ts4)) minutes")
    println()
end

let
    println("10. TimeSpan: Unit conversions")
    println("-------------------------------")

    ts = from_hours(TimeSpan, 2.5)
    println("  Overhaul duration: 2.5 hours")
    println("  Total days:         $(days(ts))")
    println("  Total hours:        $(hours(ts))")
    println("  Total minutes:      $(minutes(ts))")
    println("  Total seconds:      $(seconds(ts))")
    println("  Total milliseconds: $(millis(ts))")
    println("  Ticks (100ns):      $(ticks(ts))")
    println()
end

let
    println("11. TimeSpan: Arithmetic operations")
    println("------------------------------------")

    watch = from_hours(TimeSpan, 2.0)
    buffer = from_minutes(TimeSpan, 30.0)

    println("  Morning watch (2h):    $(Base.string(watch))")
    println("  Handover buffer (30m): $(Base.string(buffer))")
    println("  watch + buffer:        $(Base.string(watch + buffer))")
    println("  watch - buffer:        $(Base.string(watch - buffer))")
    println("  -watch:                $(Base.string(-watch))")

    shift =
        from_days(TimeSpan, 1.0) + from_hours(TimeSpan, 3.0) + from_minutes(TimeSpan, 30.0)
    println("  1d + 3h + 30m shift:   $(Base.string(shift))")
    println()
end

let
    println("12. TimeSpan: Dates.Period interop")
    println("------------------------------------")

    ts = TimeSpan(Dates.Hour(2))
    println("  TimeSpan from Dates.Hour(2): $(hours(ts)) hours")

    ts2 = from_hours(TimeSpan, 1.5)
    ms = Base.convert(Dates.Millisecond, ts2)
    println("  1.5h as Dates.Millisecond:   $ms")

    dt = DateTime(2026, 8, 22, 10, 0, 0)
    later = dt + Dates.Hour(3)
    println("  DateTime + Dates.Hour(3):    $(Base.string(later))")
    println()
end

let
    println("13. DateTimeOffset: Construction with timezone")
    println("-----------------------------------------------")

    utc = utc_now(DateTimeOffset)
    yokohama = DateTimeOffset(DateTime(2026, 8, 22, 14, 30, 0), from_hours(TimeSpan, 9.0))
    new_york = DateTimeOffset(DateTime(2026, 8, 22, 14, 30, 0), from_hours(TimeSpan, -5.0))

    println("  UTC now:                  $(Base.string(utc))")
    println("  Port of Yokohama (UTC+9): $(Base.string(yokohama))")
    println("  Port of New York (UTC-5): $(Base.string(new_york))")
    println()
end

let
    println("14. DateTimeOffset: Parsing with timezone offsets")
    println("--------------------------------------------------")

    dto1 = Base.parse(DateTimeOffset, "2026-08-22T14:30:00+09:00")
    dto2 = Base.parse(DateTimeOffset, "2026-08-22T14:30:00-05:00")
    dto3 = Base.parse(DateTimeOffset, "2026-08-22T14:30:00Z")

    println("  Parsed +09:00:  $(Base.string(dto1))")
    println("  Parsed -05:00:  $(Base.string(dto2))")
    println("  Parsed Z (UTC): $(Base.string(dto3))")
    println()
end

let
    println("15. DateTimeOffset: Timezone conversions")
    println("-----------------------------------------")

    yokohama = Base.parse(DateTimeOffset, "2026-08-22T18:00:00+09:00")
    new_york = to_offset(yokohama, from_hours(TimeSpan, -5.0))
    utc = to_universal_time(yokohama)

    println("  Arrival, Yokohama (UTC+9): $(Base.string(yokohama))")
    println("  Same instant in New York:  $(Base.string(new_york))")
    println("  Same instant in UTC:       $(Base.string(utc))")
    println("  All same UTC instant:      $(utc_ticks(yokohama) == utc_ticks(new_york))")
    println()
end

let
    println("16. DateTimeOffset: Accessing components")
    println("-----------------------------------------")

    dto = DateTimeOffset(DateTime(2026, 8, 22, 14, 30, 45), from_hours(TimeSpan, 2.0))
    println("  DateTimeOffset: $(Base.string(dto))")
    println("  UTC time:       $(Base.string(utc_date_time(dto)))")
    println("  Offset:         $(Base.string(offset(dto)))")
    println("  Offset minutes: $(total_offset_minutes(dto))")
    println()
end

let
    println("17. DateTimeOffset: Arithmetic operations")
    println("------------------------------------------")

    berth = DateTimeOffset(DateTime(2026, 8, 22, 10, 0, 0), from_hours(TimeSpan, 2.0))
    deadline = berth + from_hours(TimeSpan, 3.0)
    println("  Berth available from:  $(Base.string(berth))")
    println("  Berth deadline (+3h):  $(Base.string(deadline))")

    dep = Base.parse(DateTimeOffset, "2026-08-22T18:00:00+09:00")
    arr = Base.parse(DateTimeOffset, "2026-08-22T04:00:00-05:00")
    diff = dep - arr
    println("  Yokohama departure:    $(Base.string(dep))")
    println("  New York arrival:      $(Base.string(arr))")
    println("  Difference (UTC):      $(Base.string(diff)) (same UTC moment)")
    println()
end

let
    println("18. DateTimeOffset: Comparisons (UTC-based)")
    println("--------------------------------------------")

    yokohama = Base.parse(DateTimeOffset, "2026-08-22T18:00:00+09:00")
    new_york = Base.parse(DateTimeOffset, "2026-08-22T04:00:00-05:00")

    println("  Yokohama: $(Base.string(yokohama))")
    println("  New York: $(Base.string(new_york))")
    println("  Equal (same UTC moment): $(yokohama == new_york)")
    println("  Exact (incl offset):     $(equals_exact(yokohama, new_york))")
    println()
end

let
    println("19. Dates stdlib interop")
    println("------------------------")

    jdt = Dates.DateTime(2026, 8, 22, 14, 30, 45, 500)
    dt = DateTime(jdt)
    println("  Dates.DateTime -> DateTime: $(Base.string(dt))")

    back = Base.convert(Dates.DateTime, dt)
    println("  DateTime -> Dates.DateTime: $back")

    dto = DateTimeOffset(jdt, 120)
    println("  Dates.DateTime + offset 120min -> DateTimeOffset: $(Base.string(dto))")
    println()
end
