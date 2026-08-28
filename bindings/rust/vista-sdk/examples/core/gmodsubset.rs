use std::collections::BTreeMap;

use vista_sdk::core::gmod::Gmod;
use vista_sdk::core::gmod_path::{GmodPath, OwnedGmodPath};
use vista_sdk::core::local_id::OwnedLocalId;
use vista_sdk::core::locations::Locations;
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

struct AssetNode {
    path: Option<String>,
    code: String,
    name: String,
    common_name: Option<String>,
    display_name: String,
    category: String,
    type_: String,
    location: Option<String>,
    depth: usize,
    children: Vec<usize>,
    parent: Option<usize>,
}

struct AssetModel {
    vis_version: VisVersion,
    nodes: Vec<AssetNode>,
    node_map: BTreeMap<String, usize>,
    nodes_by_code: BTreeMap<String, Vec<usize>>,
    root: Option<usize>,
}

impl AssetModel {
    fn new(vis_version: VisVersion) -> Self {
        Self {
            vis_version,
            nodes: Vec::new(),
            node_map: BTreeMap::new(),
            nodes_by_code: BTreeMap::new(),
            root: None,
        }
    }

    fn add_path(&mut self, path: &GmodPath, gmod: &Gmod, locations: &Locations) {
        let all_nodes: Vec<_> = path.iter().collect();

        let mut node_id = String::new();

        for (i, node) in all_nodes.iter().enumerate() {
            let depth = i;
            let prev_id = if node_id.is_empty() {
                None
            } else {
                Some(node_id.clone())
            };

            if !node_id.is_empty() {
                node_id.push('/');
            }
            node_id.push_str(&format!("{}", node));

            if self.node_map.contains_key(&node_id) {
                continue;
            }

            let meta = node.metadata();

            let (sub_path_string, display_name) = if depth == 0 {
                (None, meta.name().to_string())
            } else {
                let sub_path = OwnedGmodPath::from_full_path(&node_id, gmod, locations);
                let display_name = sub_path
                    .as_ref()
                    .and_then(|p| p.common_names().last().map(|(_, name)| name))
                    .unwrap_or_else(|| meta.name().to_string());
                let path_str = sub_path
                    .as_ref()
                    .map(|p| format!("{}", p))
                    .unwrap_or_else(|| format!("{}", node));
                (Some(path_str), display_name)
            };

            let asset_node = AssetNode {
                path: sub_path_string,
                code: node.code().to_string(),
                name: meta.name().to_string(),
                common_name: meta.common_name().map(|s| s.to_string()),
                display_name,
                category: meta.category().to_string(),
                type_: meta.r#type().to_string(),
                location: node.location().map(|s| s.to_string()),
                depth,
                children: Vec::new(),
                parent: None,
            };

            let idx = self.nodes.len();
            self.nodes.push(asset_node);
            self.nodes_by_code
                .entry(node.code().to_string())
                .or_default()
                .push(idx);

            if depth == 0 {
                if self.root.is_none() {
                    self.root = Some(idx);
                }
                self.node_map.insert(node_id.clone(), idx);
                continue;
            }

            if let Some(ref pid) = prev_id {
                if let Some(&parent_idx) = self.node_map.get(pid) {
                    self.nodes[parent_idx].children.push(idx);
                    self.nodes[idx].parent = Some(parent_idx);
                }
            }

            self.node_map.insert(node_id.clone(), idx);
        }
    }

    fn from_paths(
        vis_version: VisVersion,
        paths: &[OwnedGmodPath],
        gmod: &Gmod,
        locations: &Locations,
    ) -> Self {
        let mut model = Self::new(vis_version);
        for path in paths {
            model.add_path(path, gmod, locations);
        }
        model
    }

    fn from_path_strings(vis_version: VisVersion, path_strings: &[&str]) -> Self {
        let vis = Vis::instance();
        let gmod = vis.gmod(vis_version).expect("valid vis version");
        let locations = vis.locations(vis_version).expect("valid vis version");

        let mut paths = Vec::new();
        for &path_str in path_strings {
            match OwnedGmodPath::from_short_path(path_str, gmod, locations) {
                Some(path) => paths.push(path),
                None => println!("  Warning: Could not parse path '{}'", path_str),
            }
        }
        Self::from_paths(vis_version, &paths, gmod, locations)
    }

    fn nodes_by_code(&self, code: &str) -> &[usize] {
        self.nodes_by_code
            .get(code)
            .map(|v| v.as_slice())
            .unwrap_or(&[])
    }

    fn node_count(&self) -> usize {
        self.nodes.len()
    }

    fn max_depth(&self) -> usize {
        self.nodes.iter().map(|n| n.depth).max().unwrap_or(0)
    }

    fn to_json(&self) -> String {
        match self.root {
            Some(idx) => self.node_to_json(idx, 0),
            None => "{}".to_string(),
        }
    }

    fn node_to_json(&self, idx: usize, indent: usize) -> String {
        let node = &self.nodes[idx];
        let pad = "  ".repeat(indent);
        let pad2 = "  ".repeat(indent + 1);

        let escape = |s: &str| -> String {
            let mut out = String::with_capacity(s.len() + 2);
            out.push('"');
            for c in s.chars() {
                if c == '"' || c == '\\' {
                    out.push('\\');
                }
                out.push(c);
            }
            out.push('"');
            out
        };

        let mut children_with_path: Vec<usize> = node
            .children
            .iter()
            .copied()
            .filter(|&ci| self.nodes[ci].path.is_some())
            .collect();
        children_with_path.sort_by(|&a, &b| {
            self.nodes[a]
                .path
                .as_deref()
                .unwrap_or("")
                .cmp(self.nodes[b].path.as_deref().unwrap_or(""))
        });

        let mut o = String::new();
        o.push_str("{\n");
        o.push_str(&format!(
            "{}\"path\": {},\n",
            pad2,
            escape(node.path.as_deref().unwrap_or(""))
        ));
        o.push_str(&format!("{}\"code\": {},\n", pad2, escape(&node.code)));
        o.push_str(&format!("{}\"name\": {},\n", pad2, escape(&node.name)));
        match &node.common_name {
            Some(cn) => o.push_str(&format!("{}\"commonName\": {},\n", pad2, escape(cn))),
            None => o.push_str(&format!("{}\"commonName\": null,\n", pad2)),
        }
        o.push_str(&format!(
            "{}\"displayName\": {},\n",
            pad2,
            escape(&node.display_name)
        ));
        o.push_str(&format!(
            "{}\"category\": {},\n",
            pad2,
            escape(&node.category)
        ));
        o.push_str(&format!("{}\"type\": {},\n", pad2, escape(&node.type_)));
        match &node.location {
            Some(loc) => o.push_str(&format!("{}\"location\": {},\n", pad2, escape(loc))),
            None => o.push_str(&format!("{}\"location\": null,\n", pad2)),
        }

        o.push_str(&format!("{}\"children\": [", pad2));
        if children_with_path.is_empty() {
            o.push_str("]\n");
        } else {
            o.push('\n');
            for (i, &ci) in children_with_path.iter().enumerate() {
                o.push_str(&format!("{}  ", pad2));
                o.push_str(&self.node_to_json(ci, indent + 2));
                if i + 1 < children_with_path.len() {
                    o.push(',');
                }
                o.push('\n');
            }
            o.push_str(&format!("{}]\n", pad2));
        }
        o.push_str(&pad);
        o.push('}');
        o
    }

    fn print_tree(&self) {
        if let Some(root) = self.root {
            self.print_node(root, "", true, true);
        }
    }

    fn print_node(&self, idx: usize, prefix: &str, is_last: bool, is_root: bool) {
        let node = &self.nodes[idx];

        if !is_root {
            print!("{}{}", prefix, if is_last { "└─ " } else { "├─ " });
        }
        print!("{}", node.code);
        if let Some(ref loc) = node.location {
            print!(" [{}]", loc);
        }
        println!(": {}", node.display_name);

        let mut sorted_children: Vec<usize> = node
            .children
            .iter()
            .copied()
            .filter(|&ci| self.nodes[ci].path.is_some())
            .collect();
        sorted_children.sort_by(|&a, &b| {
            self.nodes[a]
                .path
                .as_deref()
                .unwrap_or("")
                .cmp(self.nodes[b].path.as_deref().unwrap_or(""))
        });

        for (i, &ci) in sorted_children.iter().enumerate() {
            let child_is_last = i == sorted_children.len() - 1;
            let new_prefix = if is_root {
                prefix.to_string()
            } else {
                format!("{}{}", prefix, if is_last { "   " } else { "│  " })
            };
            self.print_node(ci, &new_prefix, child_is_last, false);
        }
    }
}

fn main() {
    println!("=== vista-sdk GmodSubset Sample ===\n");

    {
        println!("1. AssetModel: Building from equipment paths (dual-engine vessel)");
        println!("--------------------------------------------------------------------");

        let vis = Vis::instance();
        let vis_version = vis.latest();

        let asset_paths: &[&str] = &[
            "411.1-P/C101.31-1",
            "411.1-P/C101.31-2",
            "411.1-P/C101.31-3",
            "411.1-P/C101.31-4",
            "411.1-P/C101.31-5",
            "411.1-P/C101.31-6",
            "411.1-P/C101.63/S206",
            "411.1-S/C101.31-1",
            "411.1-S/C101.31-2",
            "411.1-S/C101.31-3",
            "411.1-S/C101.31-4",
            "411.1-S/C101.31-5",
            "411.1-S/C101.31-6",
            "411.1-S/C101.63/S206",
            "511.11-1/C101",
            "511.11-2/C101",
            "621.21/S90",
        ];

        println!("Defining {} equipment paths", asset_paths.len());
        println!("(Each path implicitly includes all parent nodes)\n");

        let model = AssetModel::from_path_strings(vis_version, asset_paths);

        println!("Built model with {} total nodes", model.node_count());
        println!("Maximum depth   : {}\n", model.max_depth());

        let engines = model.nodes_by_code("C101");
        println!("Lookup: All Engines (C101)");
        println!(
            "Found {} instances of C101 (Internal combustion engine):",
            engines.len()
        );
        for &idx in engines {
            if let Some(ref p) = model.nodes[idx].path {
                println!("  - {}", p);
            }
        }

        println!("\nLookup: All Cylinders (C101.31)");
        let cylinders = model.nodes_by_code("C101.31");
        println!("Found {} instances of C101.31 (Cylinder):", cylinders.len());
        for (count, &idx) in cylinders.iter().enumerate() {
            if count >= 6 {
                break;
            }
            if let Some(ref p) = model.nodes[idx].path {
                println!("  - {}", p);
            }
        }
        if cylinders.len() > 6 {
            println!("  ... and {} more", cylinders.len() - 6);
        }

        println!("\nAll available codes in model:");
        let codes: Vec<&str> = model.nodes_by_code.keys().map(|s| s.as_str()).collect();
        println!("{} unique codes: [{}]", codes.len(), codes.join(", "));

        println!("\nModel Metadata:");
        println!("VIS Version   : {}", model.vis_version);
        println!("Total nodes   : {}", model.node_count());
        println!("Maximum depth : {}\n", model.max_depth());

        println!("Asset Model Tree:\n");
        model.print_tree();

        println!();
    }

    {
        println!("2. AssetModel: Building from LocalIds (runtime flow)");
        println!("-------------------------------------------------------");

        let vis = Vis::instance();
        let vis_version = vis.latest();

        let local_id_strings: &[&str] = &[
            "/dnv-v2/vis-3-4a/411.1-1/C101.31-1/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
            "/dnv-v2/vis-3-4a/411.1-1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
            "/dnv-v2/vis-3-4a/411.1-1/C101.31-3/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
            "/dnv-v2/vis-3-4a/411.1-2/C101.31-1/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
            "/dnv-v2/vis-3-4a/411.1-2/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
            "/dnv-v2/vis-3-4a/511.11-1/C101/meta/qty-revolution",
        ];

        println!(
            "Received {} LocalIds from data source\n",
            local_id_strings.len()
        );

        let mut paths: Vec<OwnedGmodPath> = Vec::new();
        let gmod = vis.gmod(vis_version).expect("valid vis version");
        let locations = vis.locations(vis_version).expect("valid vis version");

        for &lid_str in local_id_strings {
            if let Some(local_id) = OwnedLocalId::from_string(lid_str) {
                let full = local_id.primary_item().to_full_path_string();
                if let Some(p) = OwnedGmodPath::from_full_path(&full, gmod, locations) {
                    paths.push(p);
                }
                if let Some(secondary) = local_id.secondary_item() {
                    let full2 = secondary.to_full_path_string();
                    if let Some(p) = OwnedGmodPath::from_full_path(&full2, gmod, locations) {
                        paths.push(p);
                    }
                }
            }
        }

        println!("Extracted {} GmodPaths from LocalIds", paths.len());

        let model = AssetModel::from_paths(vis_version, &paths, gmod, locations);

        println!("Built model with {} nodes", model.node_count());
        println!("Maximum depth   : {}\n", model.max_depth());

        println!("Asset Model Tree (derived from LocalIds):\n");
        model.print_tree();

        println!();
    }

    {
        println!("3. AssetModel: JSON export for visualization");
        println!("-----------------------------------------------");

        let vis = Vis::instance();
        let vis_version = vis.latest();

        let asset_paths: &[&str] = &["411.1/C101.31-1", "411.1/C101.31-2"];

        let model = AssetModel::from_path_strings(vis_version, asset_paths);

        println!("JSON output (for D3.js, visualization tools, etc.):\n");
        println!("{}", model.to_json());

        println!();
    }
}
