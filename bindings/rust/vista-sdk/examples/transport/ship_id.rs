//! Demonstrates usage of the vista-sdk ShipId API.
//!
//! Shows how to construct, parse, and inspect ShipId values, which identify a vessel
//! in ISO 19848 transport packages either by IMO number or by an alternative identifier.

use vista_sdk::core::imo_number::ImoNumber;
use vista_sdk::transport::ship_id::ShipId;

fn main() {
    println!("=== vista-sdk ShipId Sample ===\n");

    {
        println!("1. ShipId: Constructing from an IMO number");
        println!("----------------------------------------------");

        if let Some(imo) = ImoNumber::from_str("9074729") {
            let ship_id = ShipId::from_imo_number(imo);

            println!("  is_imo_number: {}", ship_id.is_imo_number());
            println!("  is_other_id  : {}", ship_id.is_other_id());
            println!("  to_string()  : {}", ship_id);
        }
        println!();
    }

    {
        println!("2. ShipId: Constructing from an alternative identifier");
        println!("-----------------------------------------------------------");

        let ship_id = ShipId::from_other_id("VESSEL-XYZ-789").expect("non-empty string");

        println!("  is_imo_number: {}", ship_id.is_imo_number());
        println!("  is_other_id  : {}", ship_id.is_other_id());
        println!("  to_string()  : {}", ship_id);
        println!();
    }

    {
        println!("3. ShipId::from_string: Parsing header ShipID values");
        println!("--------------------------------------------------------");

        let test_values = [
            "IMO9074729",     // valid IMO number, with prefix
            "imo9074729",     // valid IMO number, case-insensitive prefix
            "IMO1234568",     // "IMO" prefix but failing checksum -> falls back to alternative id
            "VESSEL-ABC-123", // plain alternative identifier
            "",               // empty -> None
        ];

        for value in &test_values {
            print!("  '{value}'\n");
            match ShipId::from_string(value) {
                None => println!("    -> (invalid, empty input)"),
                Some(ship_id) if ship_id.is_imo_number() => {
                    println!("    -> IMO number: {ship_id}");
                }
                Some(ship_id) => {
                    println!("    -> Alternative id: {ship_id}");
                }
            }
        }
        println!();
    }

    {
        println!("4. ShipId: Pattern matching on the identifier kind");
        println!("------------------------------------------------------");

        let mut ship_ids: Vec<ShipId> = Vec::new();
        if let Some(imo) = ImoNumber::from_str("9785811") {
            ship_ids.push(ShipId::from_imo_number(imo));
        }
        ship_ids.push(ShipId::from_other_id("BARGE-042").expect("non-empty string"));

        for ship_id in &ship_ids {
            let description = match ship_id {
                ShipId::Imo(imo) => format!("IMO number  : {imo}"),
                ShipId::Other(id) => format!("Alternative : {id}"),
            };
            println!("  {description}");
        }
        println!();
    }

    {
        println!("5. ShipId: Equality comparison");
        println!("----------------------------------");

        let imo1 = ImoNumber::from_str("9074729").expect("9074729 is valid");
        let imo2 = ImoNumber::from_str("9074729").expect("9074729 is valid");
        let imo3 = ImoNumber::from_str("9785811").expect("9785811 is valid");

        let ship_id_a = ShipId::from_imo_number(imo1);
        let ship_id_b = ShipId::from_imo_number(imo2);
        let ship_id_c = ShipId::from_imo_number(imo3);

        println!("  IMO9074729 == IMO9074729 : {}", ship_id_a == ship_id_b);
        println!("  IMO9074729 == IMO9785811 : {}", ship_id_a == ship_id_c);
        println!();
    }
}
