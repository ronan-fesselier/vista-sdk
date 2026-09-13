# Vista SDK - Rust Bindings

[![Rust Workflow Status](https://img.shields.io/github/actions/workflow/status/dnv-opensource/vista-sdk/rust-build-and-test.yml?branch=main&label=Rust+Build+%26+Test)](https://github.com/dnv-opensource/vista-sdk/actions)
[![GitHub](https://img.shields.io/github/license/dnv-opensource/vista-sdk?style=flat-square)](https://github.com/dnv-opensource/vista-sdk/blob/main/LICENSE)

The Rust bindings for the Vista SDK, built on top of the C++ SDK and its C API. For an overview
of the SDK and its concepts, see the [main README](https://github.com/dnv-opensource/vista-sdk/blob/main/README.md).

> **Statically linked** (default): the crate builds and links the C++ SDK (via its C API) at compile time.
> No runtime dependency, no separate library to install. This applies to the default `vendored` feature.

## 📦 Installation

```toml
[dependencies]
vista-sdk = "0.3"
```

Or consume it from source via a path or git dependency.

### Supported platforms

| OS      | Architecture  | Compiler                     |
| ------- | ------------- | ---------------------------- |
| Linux   | x86_64, ARM64 | GCC 10+, Clang 10+           |
| Windows | x86_64        | MSVC 19.26+, MinGW GCC 14.2+ |

### Prerequisites

- **Rust** 1.75 or later (edition 2021)
- **CMake** 3.25 or later
- **C++20 compiler** with concepts support:
  - GCC 10+ (tested: GCC 12.2.0, GCC 14.2.0)
  - Clang 10+ (tested: Clang 16.0.6, Clang 19.1.7)
  - MSVC 19.26+ (tested: MSVC 19.44)
  - MinGW GCC 14.2.0 (Windows)

The C++ toolchain requirements are those of the [C++ SDK](https://github.com/dnv-opensource/vista-sdk/blob/main/cpp/README.md#prerequisites):
the crate's `build.rs` configures and builds it through CMake.

### Consuming via path

```toml
[dependencies]
vista-sdk = { path = "bindings/rust/vista-sdk" }
```

### Consuming via git

```toml
[dependencies]
vista-sdk = { git = "https://github.com/dnv-opensource/vista-sdk.git", branch = "main" }
```

### Consuming via git submodule

```bash
git submodule add --depth 1 https://github.com/dnv-opensource/vista-sdk.git 3rdParty/vista-sdk
```

```toml
[dependencies]
vista-sdk = { path = "3rdParty/vista-sdk/bindings/rust/vista-sdk" }
```

### Building from source

```bash
git clone https://github.com/dnv-opensource/vista-sdk.git
cd vista-sdk/bindings/rust/vista-sdk

# Build the crate (builds the C++ SDK and C API through CMake)
cargo build

# Run the tests
cargo test
```

The C++ SDK is built into Cargo's `OUT_DIR` by `build.rs`. The crate itself only exposes safe
wrappers over the C API. Its version is kept in sync with the C++ SDK version (see the `justfile`).

## 🔢 Types

The crate re-exports the SDK standalone value types. See the [types examples](examples/README.md#types) for more usage examples.

### Decimal

Exact decimal arithmetic with no floating-point rounding, implementing XSD `xs:decimal` semantics (ISO 19848 Table 2).

```rust
use vista_sdk::*;

fn main() {
    let fuel_consumed   = "12.450".parse::<Decimal>().expect("valid"); // tonnes
    let price_per_tonne = "615.30".parse::<Decimal>().expect("valid"); // USD

    println!("{}", fuel_consumed * price_per_tonne); // "7660.485", exact
}
```

Output:
```
7660.485
```

### DateTime / DateTimeOffset

`DateTime` (UTC-only) and `DateTimeOffset` (timezone-aware) represent timestamps with 100-nanosecond precision and ISO 8601 parsing/formatting (ISO 19848 Table 8).

```rust
use vista_sdk::*;

fn main() {
    let dto = "2026-09-13T19:11:23+02:00".parse::<DateTimeOffset>().expect("valid");
    println!("{}", dto); // "2026-09-13T19:11:23+02:00"
}
```

Output:
```
2026-09-13T19:11:23+02:00
```

### TimeSpan

Represents a duration (not a point in time) with 100-nanosecond precision, formatted per the ISO 8601 duration grammar (`P[n]DT[n]H[n]M[n]S`).

```rust
use vista_sdk::*;

fn main() {
    let duration = TimeSpan::from_hours(1.5);
    println!("{}", duration); // "PT1H30M"
}
```

Output:
```
PT1H30M
```

## 🚀 Quick Start

> 💡 For more complete examples, see the [examples](examples/) directory and
> [examples/README.md](examples/README.md).

```rust
use vista_sdk::*;

fn main() {
    let vis       = Vis::instance();
    let version   = vis.latest();
    let gmod      = vis.gmod(version).expect("latest() should always be valid");
    let codebooks = vis.codebooks(version).expect("latest() should always be valid");
    let locations = vis.locations(version).expect("latest() should always be valid");

    let path = GmodPath::from_short_path("411.1/C101.31-2", gmod, locations)
        .expect("411.1/C101.31-2 should be a valid path");

    let qty_tag = codebooks[CodebookName::Quantity]
        .create_tag("temperature")
        .expect("temperature should be a standard quantity");
    let cnt_tag = codebooks[CodebookName::Content]
        .create_tag("exhaust.gas")
        .expect("exhaust.gas should be a standard content");
    let pos_tag = codebooks[CodebookName::Position]
        .create_tag("inlet")
        .expect("inlet should be a standard position");

    let local_id = LocalIdBuilder::create(version)
        .with_primary_item(&path)
        .with_metadata_tag(&qty_tag)
        .with_metadata_tag(&cnt_tag)
        .with_metadata_tag(&pos_tag)
        .build()
        .expect("builder with primary item and metadata tags should be valid");

    println!("LocalId: {}", local_id);

    let verbose = local_id.builder().with_verbose_mode(true);
    println!("Verbose: {}", verbose);
}
```

Output:
```
LocalId: /dnv-v2/vis-3-11a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet
Verbose: /dnv-v2/vis-3-11a/411.1/C101.31-2/~propulsion.engine/~cylinder.2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet
```

All runnable examples:

| Example                       | Description                                             |
| ----------------------------- | ------------------------------------------------------- |
| `core_gmod`                   | Navigate the Gmod tree, look up nodes, inspect metadata |
| `core_gmod_path`              | Parse short and full Gmod paths, traverse path nodes    |
| `core_gmod_subset`            | Build asset model subsets from Gmod paths               |
| `core_gmod_traversal`         | Depth-first traversal, early stop, subtree skipping     |
| `core_local_id`               | Build and parse Local IDs, verbose mode, MQTT format    |
| `core_universal_id`           | Build and parse Universal IDs                           |
| `core_locations`              | Parse and build VIS locations                           |
| `core_codebooks`              | Look up standard values and create metadata tags        |
| `core_vis_versioning`         | Convert paths and Local IDs across VIS versions         |
| `core_imo_number`             | Parse and validate IMO numbers                          |
| `query_gmod_path_query`       | Match Gmod paths by nodes, locations, masking           |
| `query_local_id_query`        | Match Local IDs by primary/secondary item and tags      |
| `query_metadata_tags_query`   | Match Local IDs by metadata tags (subset/exact)         |
| `transport_iso19848`          | List channel type names, validate format types          |
| `transport_ship_id`           | Construct and parse ShipID values                       |
| `transport_data_channel_list` | Parse and serialize DataChannelList packages            |
| `transport_time_series_data`  | Build and serialize TimeSeriesData packages             |
| `transport_sensors_data_flow` | End-to-end sensor data flow to ISO 19848                |
| `types_decimal`               | Exact decimal arithmetic                                |
| `types_datetime`              | DateTime, DateTimeOffset, TimeSpan                      |

## 📚 Core Components

For a detailed overview of VIS concepts (Gmod, Codebooks, Locations, etc.), see the [main README](https://github.com/dnv-opensource/vista-sdk/blob/main/README.md).

### Vis (Vessel Information Structure)

Singleton entry point providing thread-safe lazy-loaded access to versioned VIS data.

```rust
use vista_sdk::*;

fn main() {
    let vis       = Vis::instance();
    let gmod      = vis
        .gmod(vis.latest())
        .expect("latest() should always be valid");
    let locations = vis
        .locations(vis.latest())
        .expect("latest() should always be valid");

    println!("Latest version    : {}", vis.latest());
    println!("Gmod nodes        : {}", gmod.iter().count());
    println!(
        "Relative locations: {}",
        locations.relative_locations().count()
    );
}
```

Output:
```
Latest version    : 3-11a
Gmod nodes        : 6593
Relative locations: 13
```

### Gmod

Navigate the Generic Product Model hierarchy.

```rust
use vista_sdk::*;

fn main() {
    let vis  = Vis::instance();
    let gmod = vis
        .gmod(vis.latest())
        .expect("latest() should always be valid");

    let node = gmod.get_node("411.1").expect("411.1 should exist");
    println!("Code    : {}", node.code());
    println!("Name    : {}", node.metadata().name());
    println!("Category: {}", node.metadata().category());

    println!("First 5 nodes:");
    for n in gmod.iter().take(5) {
        println!("  {:<8} - {}", n.code(), n.metadata().name());
    }
}
```

Output:
```
Code    : 411.1
Name    : Conventional propulsion line driving
Category: ASSET FUNCTION
First 5 nodes:
  H346.1s  - Leg selection
  330      - Cargo, equipment and personnel handling by lifting
  861.341  - Survival - lifeboats, hyperbaric - embarkation - ladders
  1044     - Excessive roll predicting
  C101.311 - Cylinder head
```

### LocalId / UniversalId

Parse and build the ISO 19848 `dnv-v2` identifiers.

```rust
use vista_sdk::*;

fn main() {
    let local_id = LocalId::from_string(
        "/dnv-v2/vis-3-11a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas",
    )
    .expect("string should be valid");

    println!("LocalId     : {}", local_id);
    println!("Primary item: {}", local_id.primary_item());
    println!(
        "Quantity    : {}",
        local_id.quantity().expect("set").value()
    );
    println!("Content     : {}", local_id.content().expect("set").value());
}
```

Output:
```
LocalId     : /dnv-v2/vis-3-11a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas
Primary item: 411.1/C101.31-2
Quantity    : temperature
Content     : exhaust.gas
```

`MqttLocalId` produces the MQTT-topic formatted variant (no leading `/`, underscores instead of slashes, 8 fixed slots).

```rust
use vista_sdk::*;

fn main() {
    let universal_id = UniversalId::from_string(
        "data.dnv.com/IMO1234567/dnv-v2/vis-3-11a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas",
    )
    .expect("string should be valid");

    println!("UniversalId: {}", universal_id);
    println!("IMO        : {}", universal_id.imo_number());
    println!("LocalId    : {}", universal_id.local_id());
}
```

Output:
```
UniversalId: data.dnv.com/IMO1234567/dnv-v2/vis-3-11a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas
IMO        : IMO1234567
LocalId    : /dnv-v2/vis-3-11a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas
```

### Locations / Codebooks

Look up and validate VIS locations and codebook standard values, and create
standard or custom metadata tags.

```rust
use vista_sdk::*;

fn main() {
    let vis        = Vis::instance();
    let codebooks  = vis
        .codebooks(vis.latest())
        .expect("latest() should always be valid");
    let locations  = vis
        .locations(vis.latest())
        .expect("latest() should always be valid");

    let qty = &codebooks[CodebookName::Quantity];
    let tag = qty.create_tag("temperature").expect("standard value");

    println!("Tag                               : {}", tag);
    println!(
        "Valid standard value 'temperature': {}",
        qty.has_standard_value("temperature")
    );

    for s in &["1", "1P", "2CF", "3US"] {
        let loc = locations.from_string(s);
        println!(
            "  '{s}' -> {}",
            if loc.is_some() { "Valid" } else { "Invalid" }
        );
    }
}
```

Output:
```
Tag                               : qty-temperature
Valid standard value 'temperature': true
  '1' -> Valid
  '1P' -> Valid
  '2CF' -> Valid
  '3US' -> Invalid
```

## 🔎 Query API

`GmodPathQueryBuilder`, `MetadataTagsQueryBuilder`, and `LocalIdQueryBuilder` provide a fluent, immutable builder API for matching `GmodPath` / `LocalId` instances against structural and metadata criteria. Useful for filtering incoming sensor data or Local IDs against a set of rules without re-parsing them.

```rust
use vista_sdk::*;

fn main() {
    let vis        = Vis::instance();
    let gmod       = vis.gmod(vis.latest()).expect("valid");
    let locations  = vis.locations(vis.latest()).expect("valid");

    let base_path  = GmodPath::from_short_path("411.1/C101", gmod, locations).expect("valid");
    let path_query = GmodPathQueryBuilder::from_path(&base_path)
        .expect("valid")
        .without_locations()
        .expect("valid")
        .build();

    let tags_query = MetadataTagsQueryBuilder::create()
        .with_tag(CodebookName::Quantity, "temperature")
        .build();

    let query = LocalIdQueryBuilder::create()
        .with_primary_item_query(&path_query)
        .expect("valid")
        .with_tags(&tags_query)
        .expect("valid")
        .build();

    let a = LocalId::from_string("/dnv-v2/vis-3-11a/411.1/C101.31-2/meta/qty-temperature")
        .expect("valid");
    let b = LocalId::from_string("/dnv-v2/vis-3-11a/411.1/C101.31-2/meta/qty-pressure")
        .expect("valid");

    println!(
        "Matches (same subtree, qty-temperature): {}",
        query.match_local_id(&a)
    );
    println!(
        "Matches (different quantity)           : {}",
        query.match_local_id(&b)
    );
}
```

Output:
```
Matches (same subtree, qty-temperature): true
Matches (different quantity)           : false
```

> **Note on `with_tags`**: the C API does not expose the C++ `configure`-callback overloads (no
> closures across the FFI boundary). Use `tags_builder()` to accumulate tag constraints across multiple `with_tags` calls.

## 🔬 Advanced Usage

### Parsing errors

`LocalId::from_string_with_errors` returns both the parsed value (if valid) and any accumulated errors, mirroring the C++ `LocalId::fromString` with error output parameter.

```rust
use vista_sdk::*;

fn main() {
    let test_strings = [
        "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature",
        "/dnv-v2/INVALID/411.1/meta/qty-temperature",
        "//vis-3-4a/not-a-valid-path/meta/qty-temperature",
    ];

    for s in test_strings {
        let (local_id, errors) = LocalId::from_string_with_errors(s);
        println!("Parsing: \"{}\"", s);
        if let Some(local_id) = local_id {
            println!("  Success: {}", local_id);
        } else {
            println!("  Failed:");
            for error in errors.iter() {
                println!("  [{}] {}", error.r#type, error.message);
            }
        }
    }
}
```

Output:
```
Parsing: "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature"
  Success: /dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature
Parsing: "/dnv-v2/INVALID/411.1/meta/qty-temperature"
  Failed:
  [VisVersion] Invalid VIS version format, expected 'vis-X-Ya', got 'INVALID'
Parsing: "//vis-3-4a/not-a-valid-path/meta/qty-temperature"
  Failed:
  [NamingRule] Missing naming rule
  [VisVersion] Missing VIS version
  [PrimaryItem] Invalid or missing Primary item. Local IDs require atleast primary item and 1 metadata tag.
  [PrimaryItem] Invalid or missing '/meta' prefix after Primary item
  [Completeness] No metadata tags specified. Local IDs require atleast 1 metadata tag.
```

### Gmod traversal

Use `gmod.traverse()` for a true depth-first walk with traversal control, or `gmod.iter()` for unordered iteration over all nodes.

```rust
use vista_sdk::*;

fn main() {
    let vis  = Vis::instance();
    let gmod = vis.gmod(vis.latest()).expect("valid");

    let mut count = 0usize;
    gmod.traverse(|_parents, _node| {
        count += 1;
        TraversalHandlerResult::Continue
    }, 1);
    println!("Nodes visited (DFS): {}", count);
}
```

Output:
```
Nodes visited (DFS): 12173432
```

## 🚢 ISO 19848 Transport

### DataChannelList

Build and serialize the ISO 19848 DataChannelList package.

```rust
use vista_sdk::*;

fn main() {
    let vis = Vis::instance();
    let gmod      = vis.gmod(vis.latest()).expect("valid");
    let locations = vis.locations(vis.latest()).expect("valid");
    let codebooks = vis.codebooks(vis.latest()).expect("valid");

    let path     = GmodPath::from_short_path("411.1/C101.31-2", gmod, locations).expect("valid");
    let qty_tag  = codebooks[CodebookName::Quantity]
        .create_tag("temperature")
        .expect("valid");
    let local_id = LocalIdBuilder::create(vis.latest())
        .with_primary_item(&path)
        .with_metadata_tag(&qty_tag)
        .build()
        .expect("valid");

    let channel_id = DataChannelId::new(&local_id);
    let dct        = DataChannelType::new("Inst");
    let format     = Format::new("Decimal");
    let property   = Property::new(&dct, &format)
        .with_range(&Range::new(0.0, 600.0))
        .with_unit(&Unit::new("°C"));
    let channel = DataChannel::new(&channel_id, &property);

    let imo        = "1234567".parse::<ImoNumber>().expect("valid");
    let ship_id    = ShipId::from_imo_number(imo);
    let config_ref = ConfigurationReference::new(
        "dcl-001",
        "2026-09-13T19:11:23Z"
            .parse::<DateTimeOffset>()
            .expect("valid"),
    );
    let header   = Header::new(&ship_id, &config_ref);
    let mut list = DataChannelList::new();
    list.add(&channel);
    let pkg = DataChannelListPackage::new(&Package::new(&header, &list));

    println!("{}", data_channel_json::to_json(&pkg, true));
}

```

Output:
```json
{
  "Package": {
    "Header": {
      "ShipID": "IMO1234567",
      "DataChannelListID": {
        "ID": "dcl-001",
        "TimeStamp": "2026-09-13T19:11:23Z"
      }
    },
    "DataChannelList": {
      "DataChannel": [
        {
          "DataChannelID": {
            "LocalID": "/dnv-v2/vis-3-11a/411.1/C101.31-2/meta/qty-temperature"
          },
          "Property": {
            "DataChannelType": {
              "Type": "Inst"
            },
            "Format": {
              "Type": "Decimal"
            },
            "Range": {
              "High": 600,
              "Low": 0
            },
            "Unit": {
              "UnitSymbol": "°C"
            }
          }
        }
      ]
    }
  }
}
```

### TimeSeriesData

Build and serialize the ISO 19848 TimeSeriesData package.

```rust
use vista_sdk::*;

fn main() {
    let ch      = TsdChannelId::from_string("Temperature").expect("valid");
    let ts      = "2026-09-13T10:00:00Z"
        .parse::<DateTimeOffset>()
        .expect("valid");
    let ds      = TabularDataSet::new(ts, &["87.3"], None);
    let tabular = TabularData::new(&[&ch], &[&ds]);

    let tsd = TimeSeriesData::new().with_tabular_data(&[&tabular]);

    let pkg = TimeSeriesDataPackage::new(&TsdPackage::new(None, &[&tsd]));
    println!("{}", time_series_data_json::to_json(&pkg, true));
}
```

Output:
```json
{
  "Package": {
    "TimeSeriesData": [
      {
        "TabularData": [
          {
            "NumberOfDataSet": 1,
            "NumberOfDataChannel": 1,
            "DataChannelID": [
              "Temperature"
            ],
            "DataSet": [
              {
                "TimeStamp": "2026-09-13T10:00:00Z",
                "Value": [
                  "87.3"
                ]
              }
            ]
          }
        ]
      }
    ]
  }
}
```

### ShipId / Iso19848

```rust
use vista_sdk::*;

fn main() {
    let from_imo = ShipId::from_string("IMO9074729").expect("valid");
    let from_alt = ShipId::from_string("MMSI338234631").expect("valid");
    println!("IMO: {}", from_imo);
    println!("Alt: {}", from_alt);
}
```

Output:
```
IMO: IMO9074729
Alt: MMSI338234631
```

### DTO-level manipulation

The DTO is the serialization-facing representation of a package. Convert to it to patch values
and serialize directly, without rebuilding a validated domain package (which would require, for
example, well-formed LocalIDs). `from_json` / `to_json` work on JSON that never passes through
the domain model.

```rust
use vista_sdk::*;

fn main() {
    let ship_id    = ShipId::from_string("IMO1234567").expect("valid");
    let ts         = DateTimeOffset::utc_now();
    let config_ref = ConfigurationReference::new("cfg-v1", ts);
    let header     = Header::new(&ship_id, &config_ref);
    let pkg        = DataChannelListPackage::new(&Package::new(&header, &DataChannelList::new()));

    let mut dto = data_channel_dto::to_dto(&pkg).expect("to_dto");
    {
        let mut pkg     = dto.pkg();
        let mut header  = pkg.header();
        header.set_author("export-pipeline");

        let mut custom = header.ensure_custom_headers();
        custom.set("exportedBy", SerializableDocument::from_string("vista-sdk-sample"));
    }

    let json = data_channel_dto::to_json(&dto, true);
    println!("{}", json);
}
```

### Domain vs DTO

Two modules can produce the same JSON. Use the domain one unless you need to patch fields the
domain model does not expose.

| Module              | Use it for                                                                                                                            |
| ------------------- | ------------------------------------------------------------------------------------------------------------------------------------- |
| `data_channel_json` | The common case: read/write a package. Domain ↔ JSON in one call.                                                                     |
| `data_channel_dto`  | Advanced: patch fields with no domain API (`author`, arbitrary `customHeaders`) before serialization. Exposes `to_dto` / `to_domain`. |

Both modules expose `from_json` / `to_json`. The DTO additionally converts to and from the
domain via `to_dto` / `to_domain`, and bridges `SerializableDocument` to `serde_json::Value`
(see [Serde Integration](#-serde-integration), optional feature):

```rust
use vista_sdk::*;

fn main() {
    let ship_id    = ShipId::from_string("IMO1234567").expect("valid");
    let config_ref = ConfigurationReference::new("cfg-v1", DateTimeOffset::utc_now());
    let header     = Header::new(&ship_id, &config_ref);
    let package    = DataChannelListPackage::new(&Package::new(&header, &DataChannelList::new()));

    // The domain module reads and writes the package directly
    let direct = data_channel_json::to_json(&package, false);

    // The DTO module lets you patch fields the domain model does not expose
    let mut dto = data_channel_dto::to_dto(&package).expect("to_dto");
    {
        let mut pkg    = dto.pkg();
        let mut header = pkg.header();
        header.set_author("export-pipeline");

        let mut custom = header.ensure_custom_headers();
        custom.set("exportedBy", SerializableDocument::from_string("vista-sdk-sample"));
    }
    let patched = data_channel_dto::to_json(&dto, false);

    println!("author present in direct output : {}", direct.contains("export-pipeline"));
    println!("author present in patched output: {}", patched.contains("export-pipeline"));
}
```

Output:
```
author present in direct output : false
author present in patched output: true
```

## 🔌 Serde Integration

The `serde` feature is **opt-in**: without it the crate has no extra dependencies, so the
default build stays embeddable. Enabling it adds `serde` and `serde_json` and implements
`Serialize` / `Deserialize` on the transport packages.

```toml
[dependencies]
vista-sdk = { version = "0.3", features = ["serde"] }
serde_json = "1"
```

Only the transport **packages** implement the two traits: `DataChannelListPackage`,
`TimeSeriesDataPackage`, and `DtoPackage`. Inner types such as `Format`, `Property` or `Unit`
are not serialized on their own, because ISO 19848 defines documents rather than fragments.

```rust
use vista_sdk::*;

fn main() {
    let json = r#"{
        "Package": {
            "Header": {
                "ShipID": "IMO1234567",
                "DataChannelListID": { "ID": "test", "TimeStamp": "2024-01-01T00:00:00Z" },
                "VersionInformation": { "NamingRule": "dnv", "NamingSchemeVersion": "v2",
                                        "ReferenceURL": "https://docs.vista.dnv.com" }
            },
            "DataChannelList": { "DataChannel": [] }
        }
    }"#;

    // JSON -> domain through serde
    let package: DataChannelListPackage = serde_json::from_str(json).expect("valid document");

    // domain -> JSON through serde
    let text = serde_json::to_string_pretty(&package).expect("serializable");
    println!("{text}");
}
```

Output:
```
{
  "Package": {
    "DataChannelList": {
      "DataChannel": []
    },
    "Header": {
      "DataChannelListID": {
        "ID": "test",
        "TimeStamp": "2024-01-01T00:00:00Z"
      },
      "ShipID": "IMO1234567",
      "VersionInformation": {
        "NamingRule": "dnv",
        "NamingSchemeVersion": "v2",
        "ReferenceURL": "https://docs.vista.dnv.com"
      }
    }
  }
}
```

The JSON produced is **exactly** the SDK JSON: serialization delegates to the C++ writer, and
deserialization delegates to the C++ parser. There is no second, divergent JSON encoder.

`Deserialize` accepts both a JSON document string and an already-structured value, so
`serde_json::from_str` and `serde_json::from_value` both work. A leading UTF-8 BOM is
stripped (the C++ parser accepts one, `serde_json` does not).

## 🔧 Build Options

### Cargo features
| Feature    | Default | Description                                                                                                                            |
| ---------- | ------- | -------------------------------------------------------------------------------------------------------------------------------------- |
| `vendored` | yes     | Compile the C++ SDK and C API from source via CMake at build time                                                                      |
| `serde`    | no      | Implement `serde::Serialize` / `serde::Deserialize` on the transport packages and bridge `SerializableDocument` to `serde_json::Value` |

Without `vendored`, set `VISTA_SDK_LIB_DIR` to the directory containing the prebuilt static libraries, and `VISTA_SDK_INCLUDE_DIR` to the SDK include root.

### CMake options (passed through when `vendored` is enabled)

Set the `VISTA_SDK_CMAKE_ARGS` environment variable to pass extra CMake options to the C++ SDK build:

```sh
VISTA_SDK_CMAKE_ARGS="-DDNV_VISTA_SDK_ENABLE_SIMD=ON" cargo build
```

Or persist it in your project's `.cargo/config.toml`:

```toml
[env]
VISTA_SDK_CMAKE_ARGS = "-DDNV_VISTA_SDK_ENABLE_SIMD=ON"
```

Useful options:

| Option                      | Default | Description                                         |
| --------------------------- | ------- | --------------------------------------------------- |
| `DNV_VISTA_SDK_ENABLE_SIMD` | `OFF`   | Enable hardware CRC32-C acceleration (SSE4.2 / AVX) |
| `BUILD_SHARED_LIBS`         | `OFF`   | Build shared libraries instead of static            |

See the [C++ build options](https://github.com/dnv-opensource/vista-sdk/blob/main/cpp/README.md#-build-options) for the full list.

## 🏗️ Build Pipeline

`build.rs` runs automatically when Cargo builds the crate. It drives two independent stages before handing control back to `rustc`:

```
┌─────────────────────────────────────────────────────────────────────────────┐
│ Stage 1: C++ SDK Compilation  (vendored feature only)                       │
├─────────────────────────────────────────────────────────────────────────────┤
│                       cpp/  (C++ SDK + C API sources)                       │
│                                      ↓                                      │
│                        cmake --build  (via build.rs)                        │
│                                      ↓                                      │
│              OUT_DIR/cmake-build/lib/libdnv-vista-sdk.a                     │
│              OUT_DIR/cmake-build/lib/libdnv-vista-sdk-c.a                   │
└─────────────────────────────────────────────────────────────────────────────┘
                   ┌───────────────────┴───────────────────┐
                   ↓                                       ↓
┌─────────────────────────────────────┐ ┌─────────────────────────────────────┐
│ Stage 2A: VIS Version Codegen       │ │ Stage 2B: ISO 19848 Version Codegen │
├─────────────────────────────────────┤ ├─────────────────────────────────────┤
│          VisVersions.h              │ │        ISO19848Versions.h           │
│                ↓                    │ │               ↓                     │
│  generate_vis_version() in build.rs │ │    generate_iso19848_version()      │
│                ↓                    │ │               ↓                     │
│    OUT_DIR/vis_version.rs           │ │    OUT_DIR/iso19848_version.rs      │
│                                     │ │                                     │
│  Generates VisVersion enum, all(),  │ │  Generates Iso19848Version enum,    │
│  latest(), as_str(), Display,       │ │  all(), latest(), as_str(),         │
│  FromStr                            │ │  Display, FromStr                   │
└─────────────────────────────────────┘ └─────────────────────────────────────┘
                   └───────────────────┬───────────────────┘
                                       ↓
┌─────────────────────────────────────────────────────────────────────────────┐
│ Stage 3: Crate Compilation                                                  │
├─────────────────────────────────────────────────────────────────────────────┤
│           src/   +  include!(OUT_DIR/vis_version.rs)                        │
│                  +  include!(OUT_DIR/iso19848_version.rs)                   │
│                                      ↓                                      │
│                  cargo:rustc-link-lib=static=dnv-vista-sdk-c                │
│                  cargo:rustc-link-lib=static=dnv-vista-sdk                  │
│                                      ↓                                      │
│                         vista-sdk  (Rust crate)                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

## 🧪 Testing

### Running tests

```bash
# Run all test suites
cargo test

# Run a single suite
cargo test --test test_local_id
```

Test suites:

```bash
# Core
cargo test --test test_codebook
cargo test --test test_codebooks
cargo test --test test_codebook_name
cargo test --test test_gmod
cargo test --test test_gmod_individualizable_set.rs
cargo test --test test_gmod_node.rs
cargo test --test test_gmod_node_metadata.rs
cargo test --test test_gmod_path
cargo test --test test_imo_number
cargo test --test test_local_id
cargo test --test test_locations
cargo test --test test_location_builder
cargo test --test test_universal_id
cargo test --test test_vis
cargo test --test test_vis_versions
cargo test --test test_vis_versioning

# Queries
cargo test --test test_gmod_path_query
cargo test --test test_local_id_query
cargo test --test test_metadata_tags_query

# Transport
cargo test --test test_iso19848
cargo test --test test_iso19848_version
cargo test --test test_ship_id
cargo test --test test_data_channel
cargo test --test test_data_channel_json
cargo test --test test_time_series_data
cargo test --test test_time_series_data_json
cargo test --test test_time_series_data_channel_id
cargo test --test test_serializable_document

# Types
cargo test --test test_decimal
cargo test --test test_datetime
cargo test --test test_datetimeoffset
```

### Running examples

```bash
cargo run --example core_gmod
cargo run --example core_gmod_path
cargo run --example core_gmod_subset
cargo run --example core_gmod_traversal
cargo run --example core_local_id
cargo run --example core_universal_id
cargo run --example core_locations
cargo run --example core_codebooks
cargo run --example core_vis_versioning
cargo run --example core_imo_number
cargo run --example query_gmod_path_query
cargo run --example query_local_id_query
cargo run --example query_metadata_tags_query
cargo run --example transport_iso19848
cargo run --example transport_ship_id
cargo run --example transport_data_channel_list
cargo run --example transport_time_series_data
cargo run --example transport_sensors_data_flow
cargo run --example types_decimal
cargo run --example types_datetime
```

## 📈 Performance

Benchmarks use [criterion](https://github.com/bheisler/criterion.rs). See the
[benchmarks README](benches/README.md).

```bash
cargo bench
```

## ⚠️ Error Handling

The crate uses four distinct conventions depending on the nature of the failure:

| Pattern                               | When                                                              | Example                                                                 |
| ------------------------------------- | ----------------------------------------------------------------- | ----------------------------------------------------------------------- |
| `Result<T, VistaError>` via `FromStr` | Parse failure: error kind and message from the C layer            | `"PT1H".parse::<TimeSpan>()`, `"3.14".parse::<Decimal>()`               |
| `Result<T, VistaError>`               | Operation failed in the C layer: error kind and message           | `LocalIdBuilder::build(...)`                                            |
| `Option<T>`                           | Failure without diagnostic needed                                 | `GmodPath::from_short_path(...)`, `"9074729".parse::<ImoNumber>().ok()` |
| `panic!`                              | Programming error: caller passed input that violates the contract | `Vis::instance()` if the C library failed to initialize                 |

`.parse::<T>()` returns `Result<T, VistaError>` on all parseable types: use it when the error should propagate with `?`. `.parse::<T>().ok()` converts to `Option<T>` when a missing value is handled locally. `VistaError` carries both a `kind` and a human-readable `message`.

`from_string_with_errors` variants return `(Option<T>, ParsingErrors)` when the caller needs the full list of accumulated parse errors.

## 🛠️ Development

### Setup

```bash
git clone https://github.com/dnv-opensource/vista-sdk.git
cd vista-sdk/bindings/rust/vista-sdk

cargo build --features vendored
cargo test
```

### API documentation

Generate and open the full API docs locally:

```bash
cargo doc --features vendored --no-deps --open
```

The `--no-deps` flag limits the output to this crate only.

### Public API surface

Every module under `src/` that is not `ffi/` is public API. The `ffi/` subtree contains the raw `extern "C"` declarations and opaque type definitions and is `pub(crate)` only.

Types that have both an owned variant and a borrowing variant follow the `T` / `TRef` naming convention (`GmodNode` / `GmodNodeRef`, `GmodPath` / `GmodPathRef`, etc.). Types that only exist as borrowing views with no owned counterpart (`Gmod`, `Codebooks`, `Locations`, etc.) carry no `Ref` suffix: the suffix only appears when there is an owned twin.

### Project structure

```
bindings/rust/vista-sdk/
├── benches/            # criterion benchmarks
├── examples/
│   ├── core/           # VIS, Gmod, LocalId, locations, codebooks
│   ├── query/          # GmodPathQuery, LocalIdQuery, MetadataTagsQuery
│   ├── transport/      # DataChannelList, TimeSeriesData, ShipId, Iso19848
│   ├── types/          # Decimal, DateTime
│   └── README.md
├── src/
│   ├── core/           # Safe wrappers: Vis, Gmod, LocalId, etc.
│   ├── ffi/            # Raw extern "C" + opaque types (pub(crate))
│   ├── query/          # Safe wrappers: GmodPathQuery, LocalIdQuery, etc.
│   ├── transport/      # Safe wrappers: DataChannel, TimeSeriesData, etc.
│   ├── types/          # Decimal, DateTime, DateTimeOffset, TimeSpan
│   └── lib.rs
├── tests/              # Integration test suites
├── build.rs            # CMake orchestration + version codegen
├── Cargo.toml
└── README.md
```

## 🔖 For Maintainers

### Version numbering

The crate version tracks the C++ SDK version. Version management is handled via the `justfile`: `just sync-version` reads the `VERSION` from `cpp/CMakeLists.txt` and updates `Cargo.toml` automatically.
CI can perform this step as part of a release workflow.

### Creating a release

1. Bump `VERSION` in `cpp/CMakeLists.txt`, then run `just sync-version` to propagate it to `Cargo.toml`
2. Run `cargo test` to verify the full suite passes
3. Tag the commit `vX.Y.Z` and push

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes with tests
4. Run the test suite (`cargo test`)
5. Commit your changes (`git commit -m 'feat(rust): add amazing feature'`)
6. Push to the branch (`git push origin feature/amazing-feature`)
7. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/dnv-opensource/vista-sdk/blob/main/LICENSE) file for details.

### Development dependencies licenses

- **[criterion](https://github.com/bheisler/criterion.rs)**: Benchmarking framework (Apache-2.0 / MIT) - development only
- **[serde_json](https://github.com/serde-rs/json)**: JSON parsing in tests (MIT / Apache-2.0) - development only

## 🔗 Links

- **API docs**: [docs.rs/vista-sdk](https://docs.rs/vista-sdk)
- **Main SDK**: [github.com/dnv-opensource/vista-sdk](https://github.com/dnv-opensource/vista-sdk)
- **C++ README**: [cpp/README.md](https://github.com/dnv-opensource/vista-sdk/blob/main/cpp/README.md)
- **C API README**: [cpp/c-api/README.md](https://github.com/dnv-opensource/vista-sdk/blob/main/cpp/c-api/README.md)
- **Issues**: [GitHub Issues](https://github.com/dnv-opensource/vista-sdk/issues)

## 💬 Support

Open an issue on [GitHub Issues](https://github.com/dnv-opensource/vista-sdk/issues) or refer to the [main README](https://github.com/dnv-opensource/vista-sdk/blob/main/README.md) for SDK-level documentation.
