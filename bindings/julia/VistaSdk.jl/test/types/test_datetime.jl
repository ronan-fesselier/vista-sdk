using Test
using VistaSdk
import Dates

@testset "TimeSpan" begin

    @testset "from_ticks roundtrip" begin
        ts = TimeSpan(9_000_000)
        @test ticks(ts) == 9_000_000
    end

    @testset "from_hours accessors" begin
        ts = from_hours(TimeSpan, 2.5)
        @test abs(hours(ts) - 2.5) < 1e-9
        @test abs(minutes(ts) - 150.0) < 1e-6
    end

    @testset "from_minutes" begin
        ts = from_minutes(TimeSpan, 90.0)
        @test abs(hours(ts) - 1.5) < 1e-9
    end

    @testset "from_seconds" begin
        ts = from_seconds(TimeSpan, 3600.0)
        @test abs(hours(ts) - 1.0) < 1e-9
    end

    @testset "from_millis" begin
        ts = from_millis(TimeSpan, 1000.0)
        @test abs(seconds(ts) - 1.0) < 1e-9
    end

    @testset "parse ISO 8601 duration" begin
        ts = Base.parse(TimeSpan, "PT1H30M")
        @test abs(minutes(ts) - 90.0) < 1e-6
    end

    @testset "parse invalid throws" begin
        @test_throws VistaError Base.parse(TimeSpan, "not-a-duration")
    end

    @testset "addition" begin
        a = from_hours(TimeSpan, 1.0)
        b = from_minutes(TimeSpan, 30.0)
        @test abs(hours(a + b) - 1.5) < 1e-9
    end

    @testset "subtraction" begin
        a = from_hours(TimeSpan, 2.0)
        b = from_minutes(TimeSpan, 30.0)
        @test abs(hours(a - b) - 1.5) < 1e-9
    end

    @testset "negation" begin
        ts = from_hours(TimeSpan, 1.0)
        neg = -ts
        @test hours(neg) < 0.0
        @test abs(hours(neg) + 1.0) < 1e-9
    end

    @testset "multiply" begin
        ts = from_hours(TimeSpan, 1.0) * 2.0
        @test abs(hours(ts) - 2.0) < 1e-9
    end

    @testset "divide" begin
        ts = divide(from_hours(TimeSpan, 3.0), 2.0)
        @test abs(hours(ts) - 1.5) < 1e-9
    end

    @testset "ratio" begin
        a = from_hours(TimeSpan, 3.0)
        b = from_hours(TimeSpan, 1.0)
        @test abs(ratio(a, b) - 3.0) < 1e-9
    end

    @testset "ordering" begin
        short = from_minutes(TimeSpan, 30.0)
        long = from_hours(TimeSpan, 2.0)
        @test short < long
        @test long > short
        @test short == from_minutes(TimeSpan, 30.0)
    end

    @testset "show" begin
        ts = from_hours(TimeSpan, 1.5)
        @test !isempty(Base.string(ts))
    end

    @testset "Dates.Period constructor" begin
        ts = TimeSpan(Dates.Hour(2))
        @test abs(hours(ts) - 2.0) < 1e-9
    end

    @testset "convert to Dates.Millisecond" begin
        ts = from_seconds(TimeSpan, 5.0)
        ms = Base.convert(Dates.Millisecond, ts)
        @test ms == Dates.Millisecond(5000)
    end

    @testset "add Dates.Period" begin
        ts = from_hours(TimeSpan, 1.0)
        result = ts + Dates.Minute(30)
        @test abs(hours(result) - 1.5) < 1e-9
    end

end

@testset "DateTime" begin

    @testset "from_date accessors" begin
        dt = DateTime(2026, 3, 25)
        @test year(dt) == 2026
        @test month(dt) == 3
        @test day(dt) == 25
        @test hour(dt) == 0
        @test minute(dt) == 0
        @test second(dt) == 0
    end

    @testset "from_date_time accessors" begin
        dt = DateTime(2026, 6, 15, 13, 45, 59)
        @test hour(dt) == 13
        @test minute(dt) == 45
        @test second(dt) == 59
    end

    @testset "from_date_time_millis" begin
        dt = DateTime(2026, 1, 1, 0, 0, 0, 500)
        @test millisecond(dt) == 500
        @test microsecond(dt) >= 0
        @test nanosecond(dt) >= 0
    end

    @testset "invalid components return a sentinel, not an error" begin
        dt = DateTime(2026, 13, 1)
        @test dt == Base.typemin(DateTime)
    end

    @testset "ticks roundtrip" begin
        dt = DateTime(2026, 1, 15, 10, 30, 0)
        @test DateTime(ticks(dt)) == dt
    end

    @testset "epoch_millis roundtrip" begin
        ms = Int64(1_700_000_000_000)
        dt = from_epoch_millis(DateTime, ms)
        @test to_epoch_millis(dt) == ms
    end

    @testset "day_of_week and day_of_year" begin
        dt = DateTime(2026, 1, 1)
        @test day_of_year(dt) == 1
        @test 0 <= day_of_week(dt) <= 6
    end

    @testset "date strips time" begin
        dt = DateTime(2026, 5, 20, 14, 30, 45)
        d = date(dt)
        @test year(d) == 2026
        @test month(d) == 5
        @test day(d) == 20
        @test hour(d) == 0
        @test minute(d) == 0
        @test second(d) == 0
    end

    @testset "time_of_day" begin
        dt = DateTime(2026, 5, 20, 6, 0, 0)
        tod = time_of_day(dt)
        @test abs(seconds(tod) - 6.0 * 3600.0) < 1e-3
    end

    @testset "is_valid" begin
        @test is_valid(epoch(DateTime))
        @test is_valid(utc_now(DateTime))
    end

    @testset "is_leap_year" begin
        @test is_leap_year(DateTime, 2024)
        @test !is_leap_year(DateTime, 2023)
    end

    @testset "days_in_month" begin
        @test days_in_month(DateTime, 2024, 2) == 29
        @test days_in_month(DateTime, 2023, 2) == 28
    end

    @testset "add_days/hours/minutes/seconds" begin
        dt = DateTime(2026, 1, 1, 0, 0, 0)
        @test day(add_days(dt, 1.0)) == 2
        @test hour(add_hours(dt, 2.0)) == 2
        @test hour(add_minutes(dt, 90.0)) == 1
        @test hour(add_seconds(dt, 3600.0)) == 1
    end

    @testset "add_months clamps" begin
        dt = DateTime(2026, 1, 31, 0, 0, 0)
        @test month(add_months(dt, 1)) == 2
    end

    @testset "add_years leap" begin
        dt = DateTime(2024, 2, 29)
        @test year(add_years(dt, 1)) == 2025
    end

    @testset "Base.+ with TimeSpan" begin
        dt = DateTime(2026, 7, 14, 12, 0, 0)
        later = dt + from_hours(TimeSpan, 1.0)
        @test hour(later) == 13
    end

    @testset "Base.- with TimeSpan" begin
        dt = DateTime(2026, 7, 14, 12, 0, 0)
        earlier = dt - from_hours(TimeSpan, 1.0)
        @test hour(earlier) == 11
    end

    @testset "Base.- DateTime DateTime" begin
        a = DateTime(2026, 7, 14, 10, 0, 0)
        b = DateTime(2026, 7, 14, 8, 0, 0)
        diff = a - b
        @test abs(hours(diff) - 2.0) < 1e-9
    end

    @testset "ordering" begin
        a = DateTime(2026, 7, 14, 10, 0, 0)
        b = DateTime(2026, 7, 14, 14, 0, 0)
        @test a < b
        @test a == DateTime(2026, 7, 14, 10, 0, 0)
    end

    @testset "typemin < typemax" begin
        @test Base.typemin(DateTime) < Base.typemax(DateTime)
    end

    @testset "to_string_fmt" begin
        dt = DateTime(2026, 7, 14, 12, 0, 0)
        @test occursin("2026", to_string(dt, Iso8601))
        @test occursin("20260714", to_string(dt, Iso8601Basic))
        @test to_string(dt, Iso8601Date) == "2026-07-14"
    end

    @testset "show" begin
        dt = DateTime(2026, 7, 14, 0, 0, 0)
        @test !isempty(Base.string(dt))
    end

    @testset "parse valid" begin
        dt = Base.parse(DateTime, "2026-07-14T12:00:00Z")
        @test !isempty(Base.string(dt))
    end

    @testset "parse invalid throws" begin
        @test_throws VistaError Base.parse(DateTime, "not-a-date")
    end

    @testset "Dates.DateTime constructor" begin
        jdt = Dates.DateTime(2026, 7, 14, 10, 30, 45, 500)
        dt = DateTime(jdt)
        @test year(dt) == 2026
        @test month(dt) == 7
        @test day(dt) == 14
        @test hour(dt) == 10
        @test minute(dt) == 30
        @test second(dt) == 45
        @test millisecond(dt) == 500
    end

    @testset "convert to Dates.DateTime" begin
        dt = DateTime(2026, 4, 1, 9, 0, 0, 0)
        jdt = Base.convert(Dates.DateTime, dt)
        @test Dates.year(jdt) == 2026
        @test Dates.month(jdt) == 4
        @test Dates.day(jdt) == 1
        @test Dates.hour(jdt) == 9
    end

    @testset "add Dates.Period" begin
        dt = DateTime(2026, 7, 14, 10, 0, 0)
        later = dt + Dates.Hour(2)
        @test hour(later) == 12
    end

end

@testset "DateTimeOffset" begin

    @testset "utc_now is valid" begin
        dto = utc_now(DateTimeOffset)
        @test is_valid(dto)
        @test year(dto) >= 2024
    end

    @testset "from epoch_seconds roundtrip" begin
        dto = from_epoch_seconds(DateTimeOffset, 0)
        @test to_epoch_seconds(dto) == 0
    end

    @testset "from epoch_millis roundtrip" begin
        ms = Int64(1_700_000_000_000)
        dto = from_epoch_millis(DateTimeOffset, ms)
        @test to_epoch_millis(dto) == ms
    end

    @testset "accessors" begin
        dt = DateTime(2026, 7, 14, 10, 30, 0)
        ts = from_hours(TimeSpan, 2.0)
        dto = DateTimeOffset(dt, ts)
        @test year(dto) == 2026
        @test hour(dto) == 10
        @test total_offset_minutes(dto) == 120
    end

    @testset "date_time accessor" begin
        dt = DateTime(2026, 3, 1, 12, 0, 0)
        dto = DateTimeOffset(dt)
        @test date_time(dto) == dt
    end

    @testset "to_universal_time" begin
        dt = DateTime(2026, 7, 14, 10, 0, 0)
        ts = from_hours(TimeSpan, 2.0)
        dto = DateTimeOffset(dt, ts)
        utc = to_universal_time(dto)
        @test hour(utc_date_time(utc)) == 8
    end

    @testset "arithmetic with TimeSpan" begin
        dto = from_epoch_seconds(DateTimeOffset, 0)
        later = dto + from_hours(TimeSpan, 1.0)
        @test to_epoch_seconds(later) == 3600
    end

    @testset "subtract two DateTimeOffsets" begin
        a = from_epoch_seconds(DateTimeOffset, 7200)
        b = from_epoch_seconds(DateTimeOffset, 3600)
        diff = a - b
        @test abs(hours(diff) - 1.0) < 1e-9
    end

    @testset "equals vs equals_exact" begin
        dt = DateTime(2026, 1, 1, 12, 0, 0)
        dto1 = DateTimeOffset(dt, from_hours(TimeSpan, 1.0))
        dto2 = DateTimeOffset(dt, from_hours(TimeSpan, 1.0))
        @test dto1 == dto2
        @test equals_exact(dto1, dto2)
    end

    @testset "ordering" begin
        a = from_epoch_seconds(DateTimeOffset, 1000)
        b = from_epoch_seconds(DateTimeOffset, 2000)
        @test a < b
    end

    @testset "parse valid" begin
        dto = Base.parse(DateTimeOffset, "2026-07-14T12:00:00+02:00")
        @test !isempty(Base.string(dto))
    end

    @testset "parse invalid throws" begin
        @test_throws VistaError Base.parse(DateTimeOffset, "not-a-date")
    end

    @testset "Dates.DateTime constructor" begin
        jdt = Dates.DateTime(2026, 7, 14, 10, 0, 0)
        dto = DateTimeOffset(jdt, 120)
        @test year(dto) == 2026
        @test total_offset_minutes(dto) == 120
    end

    @testset "convert to Dates.DateTime" begin
        dt = DateTime(2026, 4, 1, 9, 0, 0, 0)
        dto = DateTimeOffset(dt, from_hours(TimeSpan, 2.0))
        jdt = Base.convert(Dates.DateTime, dto)
        @test Dates.year(jdt) == 2026
        @test Dates.hour(jdt) == 7
    end

end
