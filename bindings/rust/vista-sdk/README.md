# Vista SDK - Rust Bindings

The Rust bindings for the Vista SDK, built on top of the C++ SDK and its C API. For an overview
of the SDK and its concepts, see the [main README](https://github.com/dnv-opensource/vista-sdk/blob/main/README.md).

> **Statically linked**: the crate builds and links the C++ SDK (via its C API) at compile time.
> No runtime dependency, no separate library to install.

## 📦 Installation

This crate is not published to a registry yet. Consume it from source via a path or git
dependency.

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

The crate re-exports the SDK standalone value types. `Decimal` (exact decimal arithmetic, no
floating-point rounding), `DateTime` and `DateTimeOffset` (UTC and timezone-aware timestamps with
ISO 8601 support) and `TimeSpan` (durations). See the [types examples](examples/README.md#types)
for usage examples.

## 🚀 Quick Start

> 💡 For more complete examples, see the [examples](examples/) directory and
> [examples/README.md](examples/README.md).

```rust
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

fn main() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let codebooks = vis.codebooks(VisVersion::V3_4a).expect("V3_4a should be valid");

    let node = gmod.get_node("411.1").expect("411.1 should exist");
    println!("Node: {} - {}", node.code(), node.metadata().name());

    let qty = codebooks[vista_sdk::core::codebook_name::CodebookName::Quantity]
        .create_tag("temperature")
        .expect("temperature should be a standard quantity");
    println!("Tag: {}", qty);
}
```

Run it:

```bash
cargo run --example core_gmod
```

## 📚 Core Components

### Vis (Vessel Information Structure)

Access the VIS reference libraries (Gmod, Codebooks, Locations) and convert
identifiers across VIS versions.

- `Vis::instance()` - the VIS singleton
- `versions()` / `latest()` - available and latest VIS versions
- `gmod(version)`, `codebooks(version)`, `locations(version)` - versioned libraries
- `convert_node`, `convert_path`, `convert_local_id` - cross-version conversion

### Gmod

Navigate the Generic Product Model hierarchy.

- `root_node()`, `get_node(code)`, `node_at(index)`, `iter()`
- Node metadata: code, name, common name, normal assignment

### LocalId / UniversalId

Parse and build the ISO 19848 `dnv-v2` identifiers.

- `LocalId::from_string` / `OwnedLocalIdBuilder` - parse and build Local IDs
- `UniversalId::from_string` / `OwnedUniversalIdBuilder` - parse and build Universal IDs
- `MqttLocalId` - MQTT-topic formatted Local IDs

### Locations / Codebooks

Look up and validate VIS locations and codebook standard values, and create
standard or custom metadata tags.

## 🔎 Query API

Match identifiers without hand-writing predicates.

- `OwnedGmodPathQueryBuilder` - match `GmodPath` by nodes, locations, and before/after masking
- `OwnedMetadataTagsQueryBuilder` - match Local IDs by metadata tags (subset/exact)
- `OwnedLocalIdQueryBuilder` - combine primary/secondary item and tag constraints

> **Note on `with_tags`**: the C API does not expose the C++ `configure`-callback overloads (no
> closures across the FFI boundary). Chained `with_tags` calls therefore **replace** the previous
> tag query rather than accumulating it.

## 🚢 ISO 19848 Transport

Work with the ISO 19848 message types.

- `ShipId` - IMO number or alternative ship identifier
- `DataChannelList` - JSON (de)serialization of the channel list package
- `TimeSeriesData` - JSON (de)serialization of time series packages
- `Iso19848` - data channel type names and format data types, with value validation

## 🔧 Build Options

The crate delegates compilation of the C++ SDK to CMake through `build.rs`. To build the C++ side
with the SDK's optional features, see the
[C++ build options](https://github.com/dnv-opensource/vista-sdk/blob/main/cpp/README.md#-build-options).

## 🧪 Testing

```bash
# Run all tests
cargo test

# Run a single test suite
cargo test --test test_local_id

# Run the examples (one target per example, see Cargo.toml)
cargo run --example core_gmod
cargo run --example query_local_id_query
cargo run --example transport_ship_id
```

## 📈 Performance

Benchmarks use [criterion](https://github.com/bheisler/criterion.rs). See the
[benchmarks README](benches/README.md).

```bash
cargo bench
```

## ⚠️ Error Handling

Fallible operations return [`Result`](https://doc.rust-lang.org/std/result/) with a `VistaError`,
carrying the error kind and message reported by the C API (`last_error()`). Parsing helpers that
mirror the C++ API return `Option` where the C++ side returns `std::optional`.

## 📄 License

MIT, see the [LICENSE](https://github.com/dnv-opensource/vista-sdk/blob/main/LICENSE).
