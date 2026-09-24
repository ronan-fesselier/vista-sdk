using VistaSdk

println("=== VistaSdk.jl Decimal Sample ===\n")

let
    println("1. Propulsion: Main engine temperatures and pressures")
    println("------------------------------------------------------")

    cooling_water_outlet = Base.parse(Decimal, "87.3")
    cooling_water_inlet = Base.parse(Decimal, "72.1")
    delta_t = cooling_water_outlet - cooling_water_inlet

    scavenge_air_pressure = Base.parse(Decimal, "2.847")
    exhaust_gas_temp = Base.parse(Decimal, "342.6")

    println("  Cooling water outlet:  $(Base.string(cooling_water_outlet)) °C")
    println("  Cooling water inlet:   $(Base.string(cooling_water_inlet)) °C")
    println("  Delta T (exact):       $(Base.string(delta_t)) °C")
    println("  Scavenge air pressure: $(Base.string(scavenge_air_pressure)) bar")
    println("  Exhaust gas temp:      $(Base.string(exhaust_gas_temp)) °C")
    println()
end

let
    println("2. Fuel: Consumption rate and voyage budget")
    println("--------------------------------------------")

    fuel_consumption_per_day = Base.parse(Decimal, "45.72")
    voyage_days = Decimal(12)
    total_fuel = fuel_consumption_per_day * voyage_days

    fuel_density = Base.parse(Decimal, "0.8914")
    fuel_volume_m3 = total_fuel / fuel_density

    d_consumption = 45.72
    d_days = 12.0
    d_result = d_consumption * d_days

    println("  Fuel consumption:      $(Base.string(fuel_consumption_per_day)) t/day")
    println("  Voyage duration:       $(Base.string(voyage_days)) days")
    println("  Total fuel (Decimal):  $(Base.string(total_fuel)) t")
    println("  Total fuel (Float64):  $(repr(d_result))")
    println(
        "  Fuel volume:           $(Base.string(Base.round(fuel_volume_m3, 3, ToNearest))) m³",
    )
    println("  Density scale:         $(scale(fuel_density)) decimal places")
    println()
end

let
    println("3. Navigation: Speed, distance and ETA")
    println("----------------------------------------")

    speed_over_ground = Base.parse(Decimal, "14.3")
    speed_through_water = Base.parse(Decimal, "13.8")
    current_effect = speed_over_ground - speed_through_water

    distance_to_waypoint = Base.parse(Decimal, "1247.6")
    eta_hours = distance_to_waypoint / speed_over_ground
    hours_per_day = Decimal(24)
    eta_days = eta_hours / hours_per_day

    println("  Speed over ground:     $(Base.string(speed_over_ground)) kn")
    println("  Speed through water:   $(Base.string(speed_through_water)) kn")
    println("  Current effect:        $(Base.string(current_effect)) kn")
    println("  Distance to waypoint:  $(Base.string(distance_to_waypoint)) nm")
    println(
        "  ETA:                   $(Base.string(Base.round(eta_hours, 2, ToNearest))) h" *
        "  ($(Base.string(Base.round(eta_days, 2, ToNearest))) days)",
    )
    println()
end

let
    println("4. Stability: Draught, displacement and GM")
    println("-------------------------------------------")

    draught_fwd = Base.parse(Decimal, "7.843")
    draught_aft = Base.parse(Decimal, "8.126")
    mean_draught = (draught_fwd + draught_aft) / Decimal(2)
    trim = draught_aft - draught_fwd

    displacement = Base.parse(Decimal, "23847.5")
    kg = Base.parse(Decimal, "9.214")
    km = Base.parse(Decimal, "10.871")
    gm = km - kg

    println("  Forward draught:       $(Base.string(draught_fwd)) m")
    println("  Aft draught:           $(Base.string(draught_aft)) m")
    println("  Mean draught:          $(Base.string(mean_draught)) m")
    println("  Trim:                  $(Base.string(trim)) m (by stern)")
    println("  Displacement:          $(Base.string(displacement)) t")
    println("  KG:                    $(Base.string(kg)) m")
    println("  KM:                    $(Base.string(km)) m")
    println("  GM (exact):            $(Base.string(gm)) m")
    println()
end

let
    println("5. Environmental: Celsius to Kelvin and unit conversions")
    println("---------------------------------------------------------")

    celsius_to_kelvin = Base.parse(Decimal, "273.15")

    sea_water_temp_c = Base.parse(Decimal, "14.8")
    sea_water_temp_k = sea_water_temp_c + celsius_to_kelvin

    air_temp_c = Base.parse(Decimal, "-3.2")
    air_temp_k = celsius_to_kelvin + air_temp_c

    knots_to_ms = Base.parse(Decimal, "0.514444")
    wind_speed = Base.parse(Decimal, "18.5")
    wind_speed_ms = wind_speed * knots_to_ms

    bar_to_kpa = Decimal(100)
    barometric_pressure = Base.parse(Decimal, "1.0132")
    pressure_kpa = barometric_pressure * bar_to_kpa

    from_float = Decimal(273.15)
    println("  Celsius -> Kelvin offset (string): $(Base.string(celsius_to_kelvin)) K")
    println(
        "  From Float64 273.15 (exact?):      $(Base.string(from_float)) K" *
        "  (== \"273.15\": $(from_float == celsius_to_kelvin))",
    )
    println(
        "  Sea water temp:    $(Base.string(sea_water_temp_c)) °C = $(Base.string(sea_water_temp_k)) K",
    )
    println(
        "  Air temp:          $(Base.string(air_temp_c)) °C = $(Base.string(air_temp_k)) K",
    )
    println(
        "  Wind speed:        $(Base.string(wind_speed)) kn = $(Base.string(Base.round(wind_speed_ms, 3, ToNearest))) m/s",
    )
    println(
        "  Barometric press:  $(Base.string(barometric_pressure)) bar = $(Base.string(Base.round(pressure_kpa, 2, ToNearest))) kPa",
    )
    println()
end

let
    println("6. Rounding modes: Sensor quantization and reporting")
    println("------------------------------------------------------")

    shaft_rpm = Base.parse(Decimal, "97.4375")

    println("  Shaft RPM (raw):           $(Base.string(shaft_rpm)) rpm")
    println("  Trunc  (toward zero):      $(Base.string(Base.trunc(shaft_rpm))) rpm")
    println("  Floor  (toward -inf):      $(Base.string(Base.floor(shaft_rpm))) rpm")
    println("  Ceil   (toward +inf):      $(Base.string(Base.ceil(shaft_rpm))) rpm")
    println(
        "  Round 1dp (banker's):      $(Base.string(Base.round(shaft_rpm, 1, ToNearest))) rpm",
    )
    println(
        "  Round 1dp (ties-away):     $(Base.string(Base.round(shaft_rpm, 1, ToNearestTiesAway))) rpm",
    )

    tied_even = Base.parse(Decimal, "97.45")
    tied_odd = Base.parse(Decimal, "97.35")

    println()
    println("  Exact ties at 1dp - banker's rounds to even, ties-away up:")
    println(
        "    $(Base.string(tied_even))  banker's: $(Base.string(Base.round(tied_even, 1, ToNearest)))" *
        "   ties-away: $(Base.string(Base.round(tied_even, 1, ToNearestTiesAway)))" *
        "   (97.4 has even last digit, stays)",
    )
    println(
        "    $(Base.string(tied_odd))  banker's: $(Base.string(Base.round(tied_odd, 1, ToNearest)))" *
        "   ties-away: $(Base.string(Base.round(tied_odd, 1, ToNearestTiesAway)))" *
        "   (97.3 is odd, up to 97.4)",
    )

    flow_increment = Base.parse(Decimal, "0.047")
    flow_total = Base.zero(Decimal)
    for _ = 1:1000
        flow_total = flow_total + flow_increment
    end
    println(
        "  Flow total (1000 x 0.047 t): $(Base.string(flow_total)) t" *
        "  (exact: $(flow_total == Decimal(47)))",
    )
    println()
end

let
    println("7. Error handling: invalid sqrt and division by zero")
    println("--------------------------------------------------------")

    try
        sqrt(Decimal(-4.0))
        println("  FAILED: sqrt(-4.0) should have thrown")
    catch e
        println("  sqrt(-4.0) throws: $e")
    end

    try
        Decimal(10.0) / zero(Decimal)
        println("  FAILED: division by zero should have thrown")
    catch e
        println("  10.0 / 0 throws: $e")
    end
    println()
end
