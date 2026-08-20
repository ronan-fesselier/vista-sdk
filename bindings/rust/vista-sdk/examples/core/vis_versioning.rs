use vista_sdk::core::gmod_path::OwnedGmodPath;
use vista_sdk::core::local_id::OwnedLocalId;
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

fn main() {
    println!("=== vista-sdk VIS Versioning Sample ===\n");

    {
        println!("1. VIS: Available versions");
        println!("----------------------------");

        let vis = Vis::instance();

        println!("Latest VIS version: {}", vis.latest());
        println!("\nAll available versions:");
        for version in vis.versions() {
            println!("  - {}", version);
        }

        println!();
    }

    {
        println!("2. VIS::convert_node: Converting individual Gmod nodes");
        println!("-------------------------------------------------------");

        let vis = Vis::instance();
        let source_gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");

        let source_node1 = source_gmod
            .get_node("323.5")
            .expect("323.5 should exist in V3_4a");
        let target_node1 = vis.convert_node(VisVersion::V3_4a, source_node1, VisVersion::V3_6a);

        println!("Converting node '323.5' from v3.4a to v3.6a:");
        println!(
            "  Source: {} ({})",
            source_node1.code(),
            source_node1.metadata().name()
        );
        if let Ok(target_node1) = &target_node1 {
            println!(
                "  Target: {} ({})",
                target_node1.code(),
                target_node1.metadata().name()
            );
        }

        let source_node2 = source_gmod
            .get_node("412.72")
            .expect("412.72 should exist in V3_4a");
        let target_node2 = vis.convert_node(VisVersion::V3_4a, source_node2, vis.latest());

        println!("\nConverting node '412.72' from v3.4a to {}:", vis.latest());
        println!(
            "  Source: {} ({})",
            source_node2.code(),
            source_node2.metadata().name()
        );
        if let Ok(target_node2) = &target_node2 {
            println!(
                "  Target: {} ({})",
                target_node2.code(),
                target_node2.metadata().name()
            );
        }

        let source_node3 = source_gmod
            .get_node("1014.211")
            .expect("1014.211 should exist in V3_4a");
        let target_node3 = vis.convert_node(VisVersion::V3_4a, source_node3, VisVersion::V3_6a);

        println!("\nConverting node '1014.211' from v3.4a to v3.6a:");
        println!("  Source: {}", source_node3.code());
        if let Ok(target_node3) = &target_node3 {
            println!("  Target: {} (unchanged)", target_node3.code());
        }

        println!();
    }

    {
        println!("3. VIS::convert_path: Converting Gmod paths (simple code changes)");
        println!("-------------------------------------------------------------------");

        let vis = Vis::instance();
        let source_gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
        let source_locations = vis
            .locations(VisVersion::V3_4a)
            .expect("V3_4a should be valid");

        let source_path1 =
            OwnedGmodPath::from_short_path("323.51/H362.1", source_gmod, source_locations);
        if let Some(source_path1) = source_path1 {
            let target_path1 =
                vis.convert_path(VisVersion::V3_4a, &source_path1, VisVersion::V3_6a);

            println!("Converting path '323.51/H362.1' from v3.4a to v3.6a:");
            println!("  Source: {}", source_path1);
            if let Ok(target_path1) = &target_path1 {
                println!("  Target: {}", target_path1);
            }
        }

        let source_path2 = OwnedGmodPath::from_short_path(
            "511.11/C101.663i/C663.5/CS6d",
            source_gmod,
            source_locations,
        );
        if let Some(source_path2) = source_path2 {
            let target_path2 =
                vis.convert_path(VisVersion::V3_4a, &source_path2, VisVersion::V3_6a);

            println!("\nConverting path '511.11/C101.663i/C663.5/CS6d' from v3.4a to v3.6a:");
            println!("  Source: {}", source_path2);
            if let Ok(target_path2) = &target_path2 {
                println!("  Target: {}", target_path2);
            }
        }

        println!();
    }

    {
        println!("4. VIS::convert_path: Converting paths with depth changes");
        println!("-----------------------------------------------------------");

        let vis = Vis::instance();
        let source_gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
        let target_gmod = vis.gmod(VisVersion::V3_6a).expect("V3_6a should be valid");
        let source_locations = vis
            .locations(VisVersion::V3_4a)
            .expect("V3_4a should be valid");
        let target_locations = vis
            .locations(VisVersion::V3_6a)
            .expect("V3_6a should be valid");

        let source_path =
            OwnedGmodPath::from_short_path("511.331/C221", source_gmod, source_locations);

        if let Some(source_path) = source_path {
            println!("Converting path '511.331/C221' from v3.4a to v3.6a:");
            println!(
                "  Source: {} (depth: {})",
                source_path,
                source_path.length()
            );

            let target_path = vis.convert_path(VisVersion::V3_4a, &source_path, VisVersion::V3_6a);

            match target_path {
                Ok(target_path) => {
                    println!(
                        "  Target: {} (depth: {})",
                        target_path,
                        target_path.length()
                    );
                    println!(
                        "  Note: Depth changed from {} to {} nodes",
                        source_path.length(),
                        target_path.length()
                    );

                    let expected = OwnedGmodPath::from_short_path(
                        "511.31/C121.31/C221",
                        target_gmod,
                        target_locations,
                    );
                    if let Some(expected) = expected {
                        if *target_path == *expected {
                            println!("  OK: Conversion matches expected path");
                        }
                    }
                }
                Err(_) => println!("  ERROR: Conversion failed"),
            }
        }

        println!();
    }

    {
        println!("5. VIS::convert_path: Converting paths with locations");
        println!("-------------------------------------------------------");

        let vis = Vis::instance();
        let source_gmod = vis.gmod(VisVersion::V3_7a).expect("V3_7a should be valid");
        let source_locations = vis
            .locations(VisVersion::V3_7a)
            .expect("V3_7a should be valid");

        let source_path =
            OwnedGmodPath::from_short_path("691.811i-A/H101.11-1", source_gmod, source_locations);

        if let Some(source_path) = source_path {
            println!("Converting path '691.811i-A/H101.11-1' from v3.7a to v3.9a:");
            println!("  Source: {}", source_path);
            println!(
                "  Source has location: {}",
                source_path.node().location().is_some()
            );

            let target_path = vis.convert_path(VisVersion::V3_7a, &source_path, VisVersion::V3_9a);

            if let Ok(target_path) = target_path {
                println!("  Target: {}", target_path);
                println!(
                    "  Target has location: {}",
                    target_path.node().location().is_some()
                );

                if let (Some(_), Some(location)) =
                    (source_path.node().location(), target_path.node().location())
                {
                    println!("  OK: Location preserved: {}", location);
                }
            }
        }

        println!();
    }

    {
        println!("6. VIS::convert_path: Converting to latest version");
        println!("----------------------------------------------------");

        let vis = Vis::instance();
        let source_gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
        let source_locations = vis
            .locations(VisVersion::V3_4a)
            .expect("V3_4a should be valid");

        let path_strings = [
            "411.1/C101.72/I101",
            "1012.21/C1147.221/C1051.7/C101.22",
            "632.32i/S110.2/C111.42/G203.31/S90.5/C401",
        ];

        println!("Converting paths from v3.4a to latest ({}):", vis.latest());

        for path_str in path_strings {
            let source_path =
                OwnedGmodPath::from_short_path(path_str, source_gmod, source_locations);
            if let Some(source_path) = source_path {
                let target_path = vis.convert_path(VisVersion::V3_4a, &source_path, vis.latest());
                print!("  {:<45}", path_str);
                match target_path {
                    Ok(target_path) => println!(" -> {}", target_path),
                    Err(_) => println!(" -> FAILED"),
                }
            }
        }

        println!();
    }

    {
        println!("7. VIS::convert_local_id: Converting LocalIds between versions");
        println!("---------------------------------------------------------------");

        let vis = Vis::instance();

        let source_local_id_str =
            "/dnv-v2/vis-3-4a/411.1/C101/sec/411.1/C101.64i/S201/meta/cnt-condensate";
        let source_local_id = OwnedLocalId::from_string(source_local_id_str);

        if let Some(source_local_id) = source_local_id {
            println!("Source LocalId (v3.4a):");
            println!("  {}", source_local_id);

            let target_local_id = vis.convert_local_id(&source_local_id, VisVersion::V3_5a);

            if let Ok(target_local_id) = target_local_id {
                println!("\nTarget LocalId (v3.5a):");
                println!("  {}", target_local_id);
                println!("\n  Note: 'C101.64i' converted to 'C101.64'");
            }
        }

        let source_local_id_str2 =
            "/dnv-v2/vis-3-4a/411.1/C101.64i-1/S201.1/C151.2/S110/meta/cnt-hydraulic.oil/state-running";
        let source_local_id2 = OwnedLocalId::from_string(source_local_id_str2);

        if let Some(source_local_id2) = source_local_id2 {
            println!("\nSource LocalId (v3.4a):");
            println!("  {}", source_local_id2);

            let target_local_id2 = vis.convert_local_id(&source_local_id2, VisVersion::V3_9a);

            if let Ok(target_local_id2) = target_local_id2 {
                println!("\nTarget LocalId (v3.9a):");
                println!("  {}", target_local_id2);
            }
        }

        println!();
    }

    {
        println!("8. VIS: Loading multiple versions at once");
        println!("----------------------------------------------");

        let vis = Vis::instance();
        let versions = vis.versions();

        println!(
            "Loading Gmods for all versions ({} -> {}):",
            versions.first().expect("at least one version"),
            versions.last().expect("at least one version")
        );

        println!("\nUnique node count per version:");
        for version in &versions {
            let gmod = vis.gmod(*version).expect("version should be valid");
            let node_count = gmod.iter().count();
            println!("  v{}: {} nodes", version, node_count);
        }

        println!();
    }

    {
        println!("9. VIS::convert_path: Chained conversion through all versions");
        println!("---------------------------------------------------------------");

        let vis = Vis::instance();
        let versions = vis.versions();

        let start_path = "511.36/I101";
        let start_version = VisVersion::V3_4a;

        println!(
            "Tracing '{}' from {} through all versions:\n",
            start_path, start_version
        );

        let start_gmod = vis.gmod(start_version).expect("V3_4a should be valid");
        let start_locations = vis.locations(start_version).expect("V3_4a should be valid");
        let current_path = OwnedGmodPath::from_short_path(start_path, start_gmod, start_locations);

        if current_path.is_none() {
            println!("  ERROR: Failed to parse source path");
        } else {
            let mut current_path = current_path.expect("checked above");

            let col_width = versions
                .iter()
                .map(|v| v.to_string().len())
                .max()
                .unwrap_or(0);

            println!(
                "  {:<width$}: {}",
                start_version.to_string(),
                current_path,
                width = col_width
            );

            for i in 1..versions.len() {
                let source_version = versions[i - 1];
                let target_version = versions[i];

                let next_path = vis.convert_path(source_version, &current_path, target_version);
                print!(
                    "  {:<width$}: ",
                    target_version.to_string(),
                    width = col_width
                );

                match next_path {
                    Ok(next_path) => {
                        println!("{}", next_path);
                        current_path = next_path;
                    }
                    Err(_) => {
                        println!("(conversion failed)");
                        break;
                    }
                }
            }
        }

        println!();
    }
}
