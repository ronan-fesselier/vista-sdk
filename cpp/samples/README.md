# Vista SDK C++ - Samples

Usage examples demonstrating the Vista SDK C++ API.

Build all samples:

```bash
cmake -B build -S cpp -DDNV_VISTA_SDK_BUILD_SAMPLES=ON
cmake --build build
```

## Core VIS Components

### `codebooks`

Access VIS codebooks and metadata tag operations.

- Get codebooks for a specific VIS version
- Look up and validate standard values
- Create standard and custom metadata tags
- Validate position tags with grouping rules

```bash
./build/bin/dnv-vista-sdk-sample-core-codebooks
```

### `gmod`

Navigate the Generic Product Model hierarchy.

- Access the Gmod tree via the VIS singleton
- Look up nodes by code
- Query node metadata and common names
- Explore product types and function nodes

```bash
./build/bin/dnv-vista-sdk-sample-core-gmod
```

### `gmodpath`

Parse and work with Gmod paths.

- Parse short paths (e.g. `"411.1-1P"`)
- Parse full paths (e.g. `"VE/400a/410/411/411i/411.1-1P"`)
- Location-based individualization
- Path comparison and format conversion

```bash
./build/bin/dnv-vista-sdk-sample-core-gmodpath
```

### `gmodsubset`

Build asset models and digital twin subsets from Gmod paths.

- Construct hierarchical asset trees from `GmodPath` instances
- Perform code-based node lookups
- Extract equipment paths from Local IDs
- Export asset trees to JSON for visualization

```bash
./build/bin/dnv-vista-sdk-sample-core-gmodsubset
```

### `gmodversioning`

Convert Gmod paths and Local IDs between VIS versions.

- Convert paths from one VIS version to another
- Handle version compatibility and mapping gaps
- Convert Local IDs across versions

```bash
./build/bin/dnv-vista-sdk-sample-core-gmodversioning
```

### `locations`

Parse and validate location codes.

- Parse location strings (e.g. `"1PS"`, `"14C"`)
- Validate individual location components (side, vertical, transverse, longitudinal)
- Build locations with the `LocationBuilder` fluent API

```bash
./build/bin/dnv-vista-sdk-sample-core-locations
```

## Identifiers

### `imonumber`

IMO ship identification numbers.

- Parse IMO numbers from string and integer formats
- Validate the IMO check digit
- Format IMO numbers for display

```bash
./build/bin/dnv-vista-sdk-sample-identifiers-imonumber
```

### `localid`

Vista Local ID for sensor identification.

- Parse Local ID strings (e.g. `"/dnv-v2/vis-3-4a/411.1/meta/qty-temperature"`)
- Access ID components: naming rule, VIS version, primary item, metadata tags
- Build Local IDs with the `LocalIdBuilder` fluent API
- MQTT formatting support

```bash
./build/bin/dnv-vista-sdk-sample-identifiers-localid
```

### `universalid`

Vista Universal ID (globally unique sensor identifiers).

- Parse Universal ID strings (`data.dnv.com/{imo}{local-id}`)
- Build Universal IDs by combining an IMO number and a Local ID
- Access individual components

```bash
./build/bin/dnv-vista-sdk-sample-identifiers-universalid
```

## Queries

### `gmodpathquery`

Pattern matching for Gmod paths.

- Build path queries with `GmodPathQueryBuilder`
- Match paths with and without locations
- Node-based filtering
- Lambda-based path transformations

```bash
./build/bin/dnv-vista-sdk-sample-query-gmodpathquery
```

### `localidquery`

Composable Local ID filtering.

- Combine `GmodPathQuery` and `MetadataTagsQuery`
- Lambda-based query composition
- Filter collections of Local IDs

```bash
./build/bin/dnv-vista-sdk-sample-query-localidquery
```

### `metadatatagsquery`

Pattern matching for Local IDs by metadata tags.

- Build tag queries with `MetadataTagsQueryBuilder`
- AND semantics for multi-tag matching
- Match against codebook-validated tags

```bash
./build/bin/dnv-vista-sdk-sample-query-metadatatagsquery
```

## ISO 19848 Transport

### `datachannellist`

ISO 19848 DataChannelList domain model and JSON serialization.

- Create `DataChannel` configurations with metadata
- Build `DataChannelList` packages with headers
- Define validation rules: data types, formats, ranges, units
- Serialize and deserialize to/from JSON per ISO 19848:2024

```bash
./build/bin/dnv-vista-sdk-sample-transport-datachannellist
```

### `iso19848`

ISO 19848 standard specifications and validation.

- Access `ISO19848` singleton for standard metadata
- Validate `DataChannelList` and `TimeSeriesData` packages
- Inspect validation results and error details

```bash
./build/bin/dnv-vista-sdk-sample-transport-iso19848
```

### `sensorsdataflow`

Transform proprietary sensor readings to ISO 19848 format.

- Map raw sensor data to Vista Local IDs
- Create `TimeSeriesDataPackage` with typed data channels
- Handle multiple data formats (double, boolean)
- Serialize the result to ISO 19848 JSON

```bash
./build/bin/dnv-vista-sdk-sample-transport-sensorsdataflow
```

### `shipid`

Ship identification for ISO 19848 transport.

- Parse `ShipId` from string formats
- Validate and format ship identifiers

```bash
./build/bin/dnv-vista-sdk-sample-transport-shipid
```

### `timeseriesdata`

ISO 19848 TimeSeriesData domain model and JSON serialization.

- Create time series data points with timestamps
- Build tabular and event data packages
- Cross-validate against a `DataChannelList`
- Serialize and deserialize to/from JSON per ISO 19848:2024

```bash
./build/bin/dnv-vista-sdk-sample-transport-timeseriesdata
```

## Types

### `datetime`

`DateTime`, `DateTimeOffset`, and `TimeSpan` with full ISO 8601 support.

```bash
./build/bin/dnv-vista-sdk-sample-types-datetime
```

### `decimal`

High-precision `Decimal` type for exact decimal arithmetic without floating-point rounding errors.

```bash
./build/bin/dnv-vista-sdk-sample-types-decimal
```

## Utils

### `stringbuilder`

`StringBuilder` for efficient string construction without repeated heap allocations.

- Basic accumulation and chained `append()`
- Numeric append without intermediate allocation
- `clear()` and buffer reuse across a loop
- Stack-to-heap transition
- `view()`, `toString()`, and `operator<<`
- SDK types (`GmodPath`, `LocalIdBuilder`) writing directly into a `StringBuilder`

```bash
./build/bin/dnv-vista-sdk-sample-utils-stringbuilder
```

## C API

### `showcase`

End-to-end walkthrough of the C API covering all major modules in sequence.

- VIS: version management
- Codebooks: accessing a codebook and its standard values
- MetadataTag: creating a tag from a standard value
- ImoNumber: parsing and validating an IMO ship number
- Gmod: looking up a node and its hierarchy
- GmodPath: parsing a short path
- LocalId: parsing and inspecting a LocalId string
- LocalId MQTT: MQTT-compatible topic formatting
- UniversalId: globally unique sensor identifier
- ISO 19848: validating a value against a FormatDataType
- VIS versioning: converting a GmodPath and a LocalId between versions
- GmodPathQuery: matching a base path while ignoring locations
- LocalIdQuery: combining a primary-item path query with a tags query
- DataChannelList: building a package and serializing to JSON
- TimeSeriesData: building a package and cross-validating against the DataChannelList with business-rule callbacks (threshold alarm)

```bash
./build/bin/dnv-vista-sdk-sample-c-api-showcase
```

## See Also

- [Main SDK Documentation](../README.md)
- [Benchmark Results](../benchmarks/README.md)
