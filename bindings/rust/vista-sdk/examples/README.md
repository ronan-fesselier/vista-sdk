# Vista SDK Rust - Examples

Usage examples demonstrating the Vista SDK Rust API. Each example is a Cargo example target
defined in `Cargo.toml`.

Run any example:

```bash
cargo run --example <name>
```

## Core VIS Components

### `core_codebooks`

Access VIS codebooks and metadata tag operations.

- Get codebooks for a specific VIS version
- Look up and validate standard values
- Create standard and custom metadata tags
- Validate position tags with grouping rules

```bash
cargo run --example core_codebooks
```

### `core_gmod`

Navigate the Generic Product Model hierarchy.

- Access the Gmod tree via the Vis singleton
- Look up nodes by code
- Query node metadata and common names
- Explore product types and function nodes

```bash
cargo run --example core_gmod
```

### `core_gmod_path`

Parse and work with Gmod paths.

- Parse short paths (e.g. `"411.1-1P"`)
- Parse full paths (e.g. `"VE/400a/410/411/411i/411.1-1P"`)
- Location-based individualization
- Path comparison and format conversion

```bash
cargo run --example core_gmod_path
```

### `core_gmodsubset`

Build asset models and digital twin subsets from Gmod paths.

- Construct hierarchical asset trees from `GmodPath` instances
- Perform code-based node lookups
- Extract equipment paths from Local IDs
- Export asset trees to JSON for visualization

```bash
cargo run --example core_gmodsubset
```

### `core_vis_versioning`

Convert Gmod paths and Local IDs between VIS versions.

- Convert paths from one VIS version to another
- Handle version compatibility and mapping gaps
- Convert Local IDs across versions

```bash
cargo run --example core_vis_versioning
```

### `core_imo_number`

IMO ship identification numbers.

- Parse IMO numbers from strings
- Validate the IMO check digit
- Format IMO numbers

```bash
cargo run --example core_imo_number
```

### `core_local_id`

Build and parse Local IDs.

- Parse Local IDs from strings
- Build Local IDs with the fluent builder
- Set primary and secondary items
- Add metadata tags

```bash
cargo run --example core_local_id
```

### `core_locations`

Work with VIS locations.

- Iterate location groups and relative locations
- Parse location strings
- Build locations with the fluent builder

```bash
cargo run --example core_locations
```

### `core_universal_id`

Build and parse Universal IDs.

- Parse Universal IDs from strings
- Build Universal IDs with the fluent builder
- Combine ship ID and Local ID

```bash
cargo run --example core_universal_id
```

## Queries

### `query_gmod_path_query`

Query Gmod paths by nodes, locations, and masking rules.

- Match paths ignoring locations
- Match paths containing specific nodes
- Ignore nodes before or after a selected node

```bash
cargo run --example query_gmod_path_query
```

### `query_local_id_query`

Query Local IDs by primary/secondary item and metadata tags.

- Match by primary item (exact or by query)
- Match by secondary item presence
- Combine item and tag constraints

```bash
cargo run --example query_local_id_query
```

### `query_metadata_tags_query`

Query Local IDs by their metadata tags.

- Match a single codebook tag
- Match multiple tags with AND semantics
- Subset vs exact matching

```bash
cargo run --example query_metadata_tags_query
```

## ISO 19848 Transport

### `transport_datachannellist`

Work with the ISO 19848 DataChannelList package.

- Parse a DataChannelList package from JSON
- Inspect data channels, IDs, and properties
- Serialize back to JSON

```bash
cargo run --example transport_datachannellist
```

### `transport_iso19848`

Work with the ISO 19848 primitive types.

- List data channel type names and format data types
- Parse and validate format type names
- Validate values against a format type
- Pattern match on validated values

```bash
cargo run --example transport_iso19848
```

### `transport_sensorsdataflow`

Demonstrate a sensor data flow to ISO 19848.

- Build data channels for sensor readings
- Assemble a DataChannelList package
- Serialize the package to JSON

```bash
cargo run --example transport_sensorsdataflow
```

### `transport_ship_id`

Work with the ISO 19848 `ShipID`.

- Construct from an IMO number
- Construct from an alternative identifier
- Parse header ShipID values
- Pattern match on the identifier kind

```bash
cargo run --example transport_ship_id
```

### `transport_timeseriesdata`

Work with the ISO 19848 TimeSeriesData package.

- Build time series data sets
- Serialize the package to JSON
- Parse a package from JSON

```bash
cargo run --example transport_timeseriesdata
```

## Types

### `types_datetime`

Date and time value types.

- Construct `DateTime`, `DateTimeOffset` and `TimeSpan`
- Parse and format ISO 8601 strings
- Perform arithmetic on dates and durations

```bash
cargo run --example types_datetime
```

### `types_decimal`

Exact decimal type.

- Construct `Decimal` from strings and numbers
- Perform exact arithmetic without floating-point rounding
- Compare decimals

```bash
cargo run --example types_decimal
```

## See Also

- [Crate documentation](../README.md)
- [Benchmarks](../benches/README.md)
- [Main SDK documentation](https://github.com/dnv-opensource/vista-sdk/blob/main/README.md)
