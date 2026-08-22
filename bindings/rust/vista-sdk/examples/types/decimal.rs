use vista_sdk::types::decimal::{Decimal, RoundingMode};

fn main() {
    println!("=== vista-sdk Decimal type samples ===\n");

    {
        println!("1. Propulsion: Main engine temperatures and pressures");
        println!("------------------------------------------------------");

        let cooling_water_outlet = Decimal::from_str("87.3").expect("valid");
        let cooling_water_inlet = Decimal::from_str("72.1").expect("valid");
        let delta_t = cooling_water_outlet - cooling_water_inlet;

        let scavenge_air_pressure = Decimal::from_str("2.847").expect("valid");
        let exhaust_gas_temp = Decimal::from_str("342.6").expect("valid");

        println!("Cooling water outlet:  {} °C", cooling_water_outlet);
        println!("Cooling water inlet:   {} °C", cooling_water_inlet);
        println!("Delta T (exact):       {} °C", delta_t);
        println!("Scavenge air pressure: {} bar", scavenge_air_pressure);
        println!("Exhaust gas temp:      {} °C", exhaust_gas_temp);
        println!();
    }

    {
        println!("2. Fuel: Consumption rate and voyage budget");
        println!("---------------------------------------------");

        let fuel_consumption_per_day = Decimal::from_str("45.72").expect("valid");
        let voyage_days = Decimal::from_i64(12);
        let total_fuel = fuel_consumption_per_day * voyage_days;

        let fuel_density = Decimal::from_str("0.8914").expect("valid");
        let fuel_volume_m3 = total_fuel / fuel_density;

        let d_consumption = 45.72_f64;
        let d_days = 12.0_f64;
        let d_result = d_consumption * d_days;

        println!("Fuel consumption:      {} t/day", fuel_consumption_per_day);
        println!("Voyage duration:       {} days", voyage_days);
        println!("Total fuel (Decimal):  {} t", total_fuel);
        println!("Total fuel (double):   {:.17}", d_result);
        println!(
            "Fuel volume:           {} m³",
            fuel_volume_m3.round(3, RoundingMode::ToNearest)
        );
        println!(
            "Density scale:         {} decimal places",
            fuel_density.scale()
        );
        println!();
    }

    {
        println!("3. Navigation: Speed, distance and ETA");
        println!("-----------------------------------------");

        let speed_over_ground = Decimal::from_str("14.3").expect("valid");
        let speed_through_water = Decimal::from_str("13.8").expect("valid");
        let current_effect = speed_over_ground - speed_through_water;

        let distance_to_waypoint = Decimal::from_str("1247.6").expect("valid");
        let eta_hours = distance_to_waypoint / speed_over_ground;
        let hours_per_day = Decimal::from_i64(24);
        let eta_days = eta_hours / hours_per_day;

        println!("Speed over ground:     {} kn", speed_over_ground);
        println!("Speed through water:   {} kn", speed_through_water);
        println!("Current effect:        {} kn", current_effect);
        println!("Distance to waypoint:  {} nm", distance_to_waypoint);
        println!(
            "ETA:                   {} h  ({} days)",
            eta_hours.round(2, RoundingMode::ToNearest),
            eta_days.round(2, RoundingMode::ToNearest)
        );
        println!();
    }

    {
        println!("4. Stability: Draught, displacement and GM");
        println!("--------------------------------------------");

        let draught_fwd = Decimal::from_str("7.843").expect("valid");
        let draught_aft = Decimal::from_str("8.126").expect("valid");
        let mean_draught = (draught_fwd + draught_aft) / Decimal::from_i64(2);
        let trim = draught_aft - draught_fwd;

        let displacement = Decimal::from_str("23847.5").expect("valid");
        let kg = Decimal::from_str("9.214").expect("valid");
        let km = Decimal::from_str("10.871").expect("valid");
        let gm = km - kg;

        println!("Forward draught:       {} m", draught_fwd);
        println!("Aft draught:           {} m", draught_aft);
        println!("Mean draught:          {} m", mean_draught);
        println!("Trim:                  {} m (by stern)", trim);
        println!("Displacement:          {} t", displacement);
        println!("KG:                    {} m", kg);
        println!("KM:                    {} m", km);
        println!("GM (exact):            {} m", gm);
        println!();
    }

    {
        println!("5. Environmental: Celsius to Kelvin and unit conversions");
        println!("----------------------------------------------------------");

        let celsius_to_kelvin = Decimal::from_str("273.15").expect("valid");

        let sea_water_temp_c = Decimal::from_str("14.8").expect("valid");
        let sea_water_temp_k = sea_water_temp_c + celsius_to_kelvin;

        let air_temp_c = Decimal::from_str("-3.2").expect("valid");
        let air_temp_k = celsius_to_kelvin + air_temp_c;

        let knots_to_ms = Decimal::from_str("0.514444").expect("valid");
        let wind_speed = Decimal::from_str("18.5").expect("valid");
        let wind_speed_ms = wind_speed * knots_to_ms;

        let bar_to_kpa = Decimal::from_i64(100);
        let barometric_pressure = Decimal::from_str("1.0132").expect("valid");
        let pressure_kpa = barometric_pressure * bar_to_kpa;

        let celsius_offset_float = 273.15_f32;
        let from_float = Decimal::from_f64(celsius_offset_float as f64);
        println!(
            "Celsius -> Kelvin offset (string):  {} K",
            celsius_to_kelvin
        );
        println!(
            "From float 273.15f (exact?):        {} K  (== \"273.15\": {})",
            from_float,
            from_float == celsius_to_kelvin
        );
        println!(
            "Sea water temp:    {} °C = {} K",
            sea_water_temp_c, sea_water_temp_k
        );
        println!("Air temp:          {} °C = {} K", air_temp_c, air_temp_k);
        println!(
            "Wind speed:        {} kn = {} m/s",
            wind_speed,
            wind_speed_ms.round(3, RoundingMode::ToNearest)
        );
        println!(
            "Barometric press:  {} bar = {} kPa",
            barometric_pressure,
            pressure_kpa.round(2, RoundingMode::ToNearest)
        );
        println!();
    }

    {
        println!("6. Rounding modes: Sensor quantization and reporting");
        println!("-------------------------------------------------------");

        let shaft_rpm = Decimal::from_str("97.4375").expect("valid");

        println!("Shaft RPM (raw):             {} rpm", shaft_rpm);
        println!("Trunc  (floor abs):          {} rpm", shaft_rpm.trunc());
        println!("Floor  (toward -inf):        {} rpm", shaft_rpm.floor());
        println!("Ceil   (toward +inf):        {} rpm", shaft_rpm.ceil());
        println!(
            "Round  1dp (banker's):       {} rpm",
            shaft_rpm.round(1, RoundingMode::ToNearest)
        );
        println!(
            "Round  1dp (ties-away):      {} rpm",
            shaft_rpm.round(1, RoundingMode::ToNearestTiesAway)
        );

        let tied_even = Decimal::from_str("97.45").expect("valid");
        let tied_odd = Decimal::from_str("97.35").expect("valid");

        println!();
        println!("Exact ties at 1dp - banker's rounds to even, ties-away up:");
        println!(
            "  {}  banker's: {}   ties-away: {}   (97.4 has even last digit, stays)",
            tied_even,
            tied_even.round(1, RoundingMode::ToNearest),
            tied_even.round(1, RoundingMode::ToNearestTiesAway)
        );
        println!(
            "  {}  banker's: {}   ties-away: {}   (97.3 is odd, up to 97.4)",
            tied_odd,
            tied_odd.round(1, RoundingMode::ToNearest),
            tied_odd.round(1, RoundingMode::ToNearestTiesAway)
        );

        let flow_increment = Decimal::from_str("0.047").expect("valid");
        let mut flow_total = Decimal::zero();
        for _ in 0..1000 {
            flow_total += flow_increment;
        }
        println!(
            "Flow total (1000 x 0.047 t): {} t  (exact: {})",
            flow_total,
            flow_total == Decimal::from_i64(47)
        );
        println!();
    }
}
