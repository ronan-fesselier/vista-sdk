# Vista SDK - Julia Bindings

[![Julia Build & Test Status](https://img.shields.io/github/actions/workflow/status/dnv-opensource/vista-sdk/julia-build-and-test.yml?branch=main&label=Julia+Build+%26+Test)](https://github.com/dnv-opensource/vista-sdk/actions)
[![GitHub](https://img.shields.io/github/license/dnv-opensource/vista-sdk?style=flat-square)](../../../LICENSE)

The Julia bindings for the Vista SDK, built on top of the C++ SDK and its C API. For an overview
of the SDK and its concepts, see the [main README](../../../README.md).

> **Built from source**: `deps/build.jl` builds the C++ SDK (via its C API) into a shared
> library (`libdnv-vista-sdk-c.so`/`.dylib`/`.dll`) when the package is built, and `ccall`
> loads it at runtime. No separate install step, but the shared library ships alongside the
> package rather than being statically linked into it.

## Table of Contents

- [Installation](#-installation)
- [Types](#-types)
- [Quick Start](#-quick-start)
- [Core Components](#-core-components)
- [Query API](#-query-api)
- [Advanced Usage](#-advanced-usage)
- [ISO 19848 Transport](#-iso-19848-transport)
- [Build Pipeline](#️-build-pipeline)
- [Testing](#-testing)
- [Performance](#-performance)
- [Error Handling](#️-error-handling)
- [Development](#️-development)
- [For Maintainers](#-for-maintainers)
- [Contributing](#-contributing)
- [License](#-license)
- [Links](#-links)
- [Support](#-support)

## 📦 Installation

### Supported platforms

| OS      | Architecture  | Compiler                     |
| ------- | ------------- | ---------------------------- |
| Linux   | x86_64, ARM64 | GCC 10+, Clang 10+           |
| Windows | x86_64        | MSVC 19.26+, MinGW GCC 14.2+ |

### Prerequisites

- **Julia** 1.13 or later
- **CMake** 3.25 or later
- **C++20 compiler** with concepts support:
  - GCC 10+ (tested: GCC 12.2.0, GCC 14.2.0)
  - Clang 10+ (tested: Clang 16.0.6, Clang 19.1.7)
  - MSVC 19.26+ (tested: MSVC 19.44)
  - MinGW GCC 14.2.0 (Windows)

The C++ toolchain requirements are those of the [C++ SDK](../../../cpp/README.md#prerequisites):
`deps/build.jl` configures and builds it through CMake.

### Consuming via path

```julia
import Pkg
Pkg.develop(path = "bindings/julia/VistaSdk.jl")
```

### Consuming via git

```julia
import Pkg
Pkg.add(url = "https://github.com/dnv-opensource/vista-sdk.git", subdir = "bindings/julia/VistaSdk.jl")
```

### Building from source

```bash
git clone https://github.com/dnv-opensource/vista-sdk.git
cd vista-sdk/bindings/julia/VistaSdk.jl

# Build the package (builds the C++ SDK and C API through CMake)
julia --project=. -e 'import Pkg; Pkg.build(verbose=true)'

# Run the tests
julia --project=. -e 'import Pkg; Pkg.test()'
```

`deps/build.jl` builds the C++ SDK into `deps/cmake-build` when the package is built. The
package itself only exposes safe wrappers over the C API. Its version is kept in sync with the
C++ SDK version (see the `justfile`).

## 🔢 Types

The package re-exports the SDK standalone value types. See the [types examples](examples/types/) for more usage examples.

### Decimal

Exact decimal arithmetic with no floating-point rounding, implementing XSD `xs:decimal` semantics (ISO 19848 Table 2).

```julia
using VistaSdk

fuel_consumed   = parse(Decimal, "12.450") # tonnes
price_per_tonne = parse(Decimal, "615.30") # USD

println(fuel_consumed * price_per_tonne) # Decimal(7660.485), exact
```

Output:
```
Decimal(7660.485)
```

### DateTime / DateTimeOffset

`DateTime` (UTC-only) and `DateTimeOffset` (timezone-aware) represent timestamps with 100-nanosecond precision and ISO 8601 parsing/formatting (ISO 19848 Table 8).

```julia
using VistaSdk

dto = parse(DateTimeOffset, "2026-09-13T19:11:23+02:00")
println(dto)
```

Output:
```
DateTimeOffset(2026-09-13T19:11:23+02:00)
```

### TimeSpan

Represents a duration (not a point in time) with 100-nanosecond precision, formatted per the ISO 8601 duration grammar (`P[n]DT[n]H[n]M[n]S`).

```julia
using VistaSdk

duration = from_hours(TimeSpan, 1.5)
println(duration)
```

Output:
```
TimeSpan(PT1H30M)
```

## 🚀 Quick Start

> 💡 For more complete examples, see the [examples](examples/) directory.

```julia
using VistaSdk

v       = vis()
version = latest(v)
g       = gmod(v, version)
cbs     = codebooks(v, version)
locs    = locations(v, version)

path = from_short_path("411.1/C101.31-2", g, locs)

qty_tag = create_tag(cbs[VistaSdk.Quantity], "temperature")
cnt_tag = create_tag(cbs[VistaSdk.Content], "exhaust.gas")
pos_tag = create_tag(cbs[VistaSdk.Position], "inlet")

local_id = build(
    with_metadata_tag(
        with_metadata_tag(
            with_metadata_tag(with_primary_item(create(LocalIdBuilder, version), path), qty_tag),
            cnt_tag,
        ),
        pos_tag,
    ),
)

println("LocalId: ", local_id)

verbose = with_verbose_mode(builder(local_id), true)
println("Verbose: ", verbose)
```

Output:
```
LocalId: /dnv-v2/vis-3-11a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet
Verbose: /dnv-v2/vis-3-11a/411.1/C101.31-2/~propulsion.engine/~cylinder.2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet
```

All runnable examples:

| Example                       | Description                                             |
| ----------------------------- | ------------------------------------------------------- |
| `core/codebooks`              | Look up standard values and create metadata tags        |
| `core/locations`              | Parse and build VIS locations                           |
| `core/imo_number`             | Parse and validate IMO numbers                          |
| `core/gmod`                   | Navigate the Gmod tree, look up nodes, inspect metadata |
| `core/gmod_path`              | Parse short and full Gmod paths, traverse path nodes    |
| `core/gmod_subset`            | Build asset model subsets from Gmod paths               |
| `core/gmod_traversal`         | Depth-first traversal, early stop, subtree skipping     |
| `core/local_id`               | Build and parse Local IDs, verbose mode, MQTT format    |
| `core/universal_id`           | Build and parse Universal IDs                           |
| `core/vis_versioning`         | Convert paths and Local IDs across VIS versions         |
| `query/metadata_tags_query`   | Match Local IDs by metadata tags (subset/exact)         |
| `query/gmod_path_query`       | Match Gmod paths by nodes, locations, masking           |
| `query/local_id_query`        | Match Local IDs by primary/secondary item and tags      |
| `transport/ship_id`           | Construct and parse ShipID values                       |
| `transport/iso19848`          | List channel type names, validate format types          |
| `transport/data_channel_list` | Parse and serialize DataChannelList packages            |
| `transport/time_series_data`  | Build and serialize TimeSeriesData packages             |
| `transport/sensors_data_flow` | End-to-end sensor data flow to ISO 19848                |
| `types/decimal`               | Exact decimal arithmetic                                |
| `types/datetime`              | DateTime, DateTimeOffset, TimeSpan                      |

Run any example with:

```bash
just example core/gmod
just example transport/sensors_data_flow
```

## 📚 Core Components

For a detailed overview of VIS concepts (Gmod, Codebooks, Locations, etc.), see the [main README](../../../README.md).

### Vis (Vessel Information Structure)

Singleton entry point providing lazy-loaded access to versioned VIS data.

```julia
using VistaSdk

v    = vis()
g    = gmod(v, latest(v))
locs = locations(v, latest(v))

println("Latest version    : ", latest(v))
println("Gmod nodes        : ", length(g))
println("Relative locations: ", length(locs))
```

Output:
```
Latest version    : V3_11a
Gmod nodes        : 6593
Relative locations: 13
```

### Gmod

Navigate the Generic Product Model hierarchy.

```julia
using VistaSdk

v = vis()
g = gmod(v, latest(v))

n = get_node(g, "411.1")
println("Code    : ", code(n))
println("Name    : ", name(metadata(n)))
println("Category: ", category(metadata(n)))

println("First 5 nodes:")
for node in Iterators.take(g, 5)
    println("  ", rpad(code(node), 8), " - ", name(metadata(node)))
end
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

```julia
using VistaSdk

local_id = from_string(
    LocalId,
    "/dnv-v2/vis-3-11a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas",
)

println("LocalId     : ", local_id)
println("Primary item: ", primary_item(local_id))
println("Quantity    : ", value(quantity(local_id)))
println("Content     : ", value(content(local_id)))
```

Output:
```
LocalId     : /dnv-v2/vis-3-11a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas
Primary item: 411.1/C101.31-2
Quantity    : temperature
Content     : exhaust.gas
```

`MqttLocalId` produces the MQTT-topic formatted variant (no leading `/`, underscores instead of slashes, 8 fixed slots).

```julia
using VistaSdk

universal_id = from_string(
    UniversalId,
    "data.dnv.com/IMO1234567/dnv-v2/vis-3-11a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas",
)

println("UniversalId: ", universal_id)
println("IMO        : ", imo_number(universal_id))
println("LocalId    : ", local_id(universal_id))
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

```julia
using VistaSdk

v    = vis()
cbs  = codebooks(v, latest(v))
locs = locations(v, latest(v))

qty = cbs[VistaSdk.Quantity]
tag = create_tag(qty, "temperature")

println("Tag                               : ", tag)
println("Valid standard value 'temperature': ", has_standard_value(qty, "temperature"))

for s in ["1", "1P", "2CF", "3US"]
    loc = parse(Location, locs, s)
    println("  '", s, "' -> ", loc !== nothing ? "Valid" : "Invalid")
end
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

`GmodPathQueryBuilder`, `MetadataTagsQueryBuilder`, and `LocalIdQueryBuilder` provide a fluent builder API for matching `GmodPath` / `LocalId` instances against structural and metadata criteria. Useful for filtering incoming sensor data or Local IDs against a set of rules without re-parsing them.

```julia
using VistaSdk

v    = vis()
g    = gmod(v, latest(v))
locs = locations(v, latest(v))

base_path  = from_short_path("411.1/C101", g, locs)
path_query = build(without_locations(from_path(GmodPathQueryBuilder, base_path)))

tags_query = build(with_tag(create(MetadataTagsQueryBuilder), VistaSdk.Quantity, "temperature"))

query = build(
    with_tags(with_primary_item_query(create(LocalIdQueryBuilder), path_query), tags_query),
)

a = from_string(LocalId, "/dnv-v2/vis-3-11a/411.1/C101.31-2/meta/qty-temperature")
b = from_string(LocalId, "/dnv-v2/vis-3-11a/411.1/C101.31-2/meta/qty-pressure")

println("Matches (same subtree, qty-temperature): ", is_match(query, a))
println("Matches (different quantity)           : ", is_match(query, b))
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

`from_string_with_errors(LocalId, s)` returns both the parsed value (if valid) and any accumulated errors, mirroring the C++ `LocalId::fromString` with error output parameter.

```julia
using VistaSdk

test_strings = [
    "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature",
    "/dnv-v2/INVALID/411.1/meta/qty-temperature",
    "//vis-3-4a/not-a-valid-path/meta/qty-temperature",
]

for s in test_strings
    local_id, errors = from_string_with_errors(LocalId, s)
    println("Parsing: \"", s, "\"")
    if local_id !== nothing
        println("  Success: ", local_id)
    else
        println("  Failed:")
        for (type, message) in errors
            println("  [", type, "] ", message)
        end
    end
end
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

Use `traverse(gmod, handler)` for a true depth-first walk with traversal control, or iterate `gmod` directly for unordered iteration over all nodes.

```julia
using VistaSdk

v = vis()
g = gmod(v, latest(v))

count = Ref(0)
traverse(g, (parents, node) -> begin
    count[] += 1
    TraversalContinue
end)
println("Nodes visited (DFS): ", count[])
```

Output:
```
Nodes visited (DFS): 12173432
```

## 🚢 ISO 19848 Transport

### DataChannelList

Build and serialize the ISO 19848 DataChannelList package.

```julia
using VistaSdk

v    = vis()
g    = gmod(v, latest(v))
locs = locations(v, latest(v))
cbs  = codebooks(v, latest(v))

path    = from_short_path("411.1/C101.31-2", g, locs)
qty_tag = create_tag(cbs[VistaSdk.Quantity], "temperature")
local_id = build(
    with_metadata_tag(with_primary_item(create(LocalIdBuilder, latest(v)), path), qty_tag),
)

channel_id = DclDataChannelId(local_id)
dct        = DclDataChannelType("Inst")
fmt        = DclFormat("Decimal")
prop       = DclProperty(dct, fmt)
set_range!(prop, DclRange(0.0, 600.0))
set_unit!(prop, DclUnit("°C"))
channel = DclDataChannel(channel_id, prop)

ship_id    = from_imo_number(parse(ImoNumber, "1234567"))
config_ref = DclConfigurationReference("dcl-001", parse(DateTimeOffset, "2026-09-13T19:11:23Z"))
header     = DclHeader(ship_id, config_ref)
list       = DclDataChannelList()
add!(list, channel)
pkg = DclListPackage(DclPackage(header, list))

println(dcl_to_json(pkg; pretty = true))
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

```julia
using VistaSdk

ch = from_string(TsdChannelId, "Temperature")
ts = parse(DateTimeOffset, "2026-09-13T10:00:00Z")
ds = TabularDataSet(ts, ["87.3"])
tabular = TabularData([ch], [ds])

tsd = TimeSeriesData()
set_tabular_data!(tsd, [tabular])

pkg = TimeSeriesDataPackage(TsdPackage(nothing, [tsd]))
println(tsd_to_json(pkg, true))
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

```julia
using VistaSdk

from_imo = from_string(ShipId, "IMO9074729")
from_alt = from_string(ShipId, "MMSI338234631")
println("IMO: ", from_imo)
println("Alt: ", from_alt)
```

Output:
```
IMO: IMO9074729
Alt: MMSI338234631
```

### DTO-level manipulation

The DTO is the serialization-facing representation of a package. Convert to it to patch values
and serialize directly, without rebuilding a validated domain package. `dcl_from_json` /
`dcl_to_json` work on JSON that never passes through the domain model.

```julia
using VistaSdk

ship_id    = from_string(ShipId, "IMO1234567")
config_ref = DclConfigurationReference("cfg-v1", now(DateTimeOffset))
hdr        = DclHeader(ship_id, config_ref)
dcl_pkg    = DclListPackage(DclPackage(hdr, DclDataChannelList()))

dto     = dcl_to_dto(dcl_pkg)
dto_pkg = pkg(dto)
dto_hdr = header(dto_pkg)
set_author!(dto_hdr, "export-pipeline")

custom = ensure_custom_headers!(dto_hdr)
set!(custom, "exportedBy", SerializableDocument("vista-sdk-sample"))

json_str = dcl_dto_to_json(dto, true)
println(json_str)
```

Output:
```json
{
  "Package": {
    "Header": {
      "ShipID": "IMO1234567",
      "DataChannelListID": {
        "ID": "cfg-v1",
        "TimeStamp": "2026-10-05T08:10:53+02:00"
      },
      "Author": "export-pipeline",
      "exportedBy": "vista-sdk-sample"
    },
    "DataChannelList": {
      "DataChannel": []
    }
  }
}
```

### Domain vs DTO

Two modules can produce the same JSON. Use the domain one unless you need to patch fields the
domain model does not expose.

| Module                             | Use it for                                                                      |
| ---------------------------------- | ------------------------------------------------------------------------------- |
| `dcl_from_json` / `dcl_to_json`    | The common case: read/write a package. Domain ↔ JSON in one call.               |
| `dcl_dto_from_json` / `dcl_to_dto` | Advanced: patch fields with no domain API (e.g. `author`) before serialization. |

Both expose `from_json` / `to_json`. The DTO additionally converts to and from the domain via
`dcl_to_dto` / `dcl_to_domain`.

```julia
using VistaSdk

ship_id    = from_string(ShipId, "IMO1234567")
config_ref = DclConfigurationReference("cfg-v1", now(DateTimeOffset))
hdr        = DclHeader(ship_id, config_ref)
dcl_pkg    = DclListPackage(DclPackage(hdr, DclDataChannelList()))

# The domain module reads and writes the package directly
direct = dcl_to_json(dcl_pkg; pretty = false)

# The DTO module lets you patch fields the domain model does not expose
dto     = dcl_to_dto(dcl_pkg)
dto_hdr = header(pkg(dto))
set_author!(dto_hdr, "export-pipeline")
patched = dcl_dto_to_json(dto, false)

println("author present in direct output : ", occursin("export-pipeline", direct))
println("author present in patched output: ", occursin("export-pipeline", patched))
```

Output:
```
author present in direct output : false
author present in patched output: true
```

## 🏗️ Build Pipeline

`deps/build.jl` runs automatically when the package is built (`Pkg.build`). It drives two
independent stages:

```
┌─────────────────────────────────────────────────────────────────────────────┐
│ Stage 1: C++ SDK Compilation                                                │
├─────────────────────────────────────────────────────────────────────────────┤
│                       cpp/  (C++ SDK + C API sources)                       │
│                                      ↓                                      │
│                   cmake -S cpp -B deps/cmake-build                          │
│                   -DBUILD_SHARED_LIBS=ON                                    │
│                   -DDNV_VISTA_SDK_BUILD_C_API=ON                            │
│                                      ↓                                      │
│                        cmake --build deps/cmake-build                       │
│                                      ↓                                      │
│         deps/cmake-build/lib/libdnv-vista-sdk-c.{so,dylib,dll}              │
└─────────────────────────────────────────────────────────────────────────────┘
                   ┌───────────────────┴───────────────────┐
                   ↓                                       ↓
┌─────────────────────────────────────┐ ┌─────────────────────────────────────┐
│ Stage 2A: VIS Version Codegen       │ │ Stage 2B: ISO 19848 Version Codegen │
├─────────────────────────────────────┤ ├─────────────────────────────────────┤
│          VisVersions.h              │ │        ISO19848Versions.h           │
│                ↓                    │ │               ↓                     │
│  generate_vis_version() in build.jl │ │    generate_iso19848_version()      │
│                ↓                    │ │               ↓                     │
│  vis_version_generated.jl           │ │    iso19848_version_generated.jl    │
│                                     │ │                                     │
│  Generates @enum VisVersion,        │ │  Generates @enum Iso19848Version,   │
│  Base.show Base.string, Base.parse  │ │  Base.show, Base.string, Base.parse │
└─────────────────────────────────────┘ └─────────────────────────────────────┘
                   └───────────────────┬───────────────────┘
                                       ↓
┌─────────────────────────────────────────────────────────────────────────────┐
│ Stage 3: Package Loading                                                    │
├─────────────────────────────────────────────────────────────────────────────┤
│           src/   +  include(deps/generated/vis_version_generated.jl)        │
│                  +  include(deps/generated/iso19848_version_generated.jl)   │
│                                      ↓                                      │
│                    ccall((:..., libdnv-vista-sdk-c), ...)                   │
│                                      ↓                                      │
│                         VistaSdk  (Julia module)                            │
└─────────────────────────────────────────────────────────────────────────────┘
```

## 🧪 Testing

### Running tests

```bash
julia --project=. -e 'import Pkg; Pkg.test()'
# or, via the justfile:
just test
```

Test suites (`test/runtests.jl` includes all of them):

```bash
# Core
test/core/test_codebook_name.jl
test/core/test_codebooks.jl
test/core/test_gmod.jl
test/core/test_gmod_node.jl
test/core/test_gmod_node_metadata.jl
test/core/test_gmod_path.jl
test/core/test_imo_number.jl
test/core/test_local_id.jl
test/core/test_locations.jl
test/core/test_parsing_errors.jl
test/core/test_universal_id.jl
test/core/test_vis.jl
test/core/test_vis_versioning.jl
test/core/test_vis_versions.jl

# Queries
test/query/test_gmod_path_query.jl
test/query/test_local_id_query.jl
test/query/test_metadata_tags_query.jl

# Transport
test/transport/test_iso19848.jl
test/transport/test_iso19848_version.jl
test/transport/test_serializable_document.jl
test/transport/test_ship_id.jl
test/transport/datachannel/test_data_channel.jl
test/transport/datachannel/test_data_channel_json.jl
test/transport/datachannel/test_datachannel_dto.jl
test/transport/timeseries/test_time_series_data.jl
test/transport/timeseries/test_time_series_data_channel_id.jl
test/transport/timeseries/test_time_series_data_dto.jl
test/transport/timeseries/test_time_series_data_json.jl

# Types
test/types/test_datetime.jl
test/types/test_decimal.jl
```

### Running examples

```bash
just example core/gmod
just example core/gmod_path
just example core/gmod_subset
just example core/gmod_traversal
just example core/local_id
just example core/universal_id
just example core/locations
just example core/codebooks
just example core/vis_versioning
just example core/imo_number
just example query/gmod_path_query
just example query/local_id_query
just example query/metadata_tags_query
just example transport/iso19848
just example transport/ship_id
just example transport/data_channel_list
just example transport/time_series_data
just example transport/sensors_data_flow
just example types/decimal
just example types/datetime
```

## 📈 Performance

Benchmarks use [BenchmarkTools.jl](https://github.com/JuliaCI/BenchmarkTools.jl). See the
[benchmarks README](benches/README.md).

```bash
just bench codebook_methods
just bench gmod_lookup
# ... etc, see benches/README.md for the full list
```

## ⚠️ Error Handling

The package uses three conventions depending on the nature of the failure:

| Pattern                                                        | When                                                    | Example                                                                                |
| -------------------------------------------------------------- | ------------------------------------------------------- | -------------------------------------------------------------------------------------- |
| `throw(VistaError)`                                            | Operation failed in the C layer: error kind and message | `ImoNumber(1234507)`, `parse(ImoNumber, "IM9074729")`                                  |
| `Base.parse` throws / `from_string` returns `Union{T,Nothing}` | Parse failure, with or without a diagnostic             | `parse(ImoNumber, s)` throws; `from_string(LocalId, s)` returns `nothing`              |
| `Union{T,Nothing}`                                             | Failure without diagnostic needed                       | `from_short_path(...)`, `get_node(...)` throws instead when the caller expects success |

`VistaError <: Exception` carries both a `kind::ErrorKind` and a human-readable `message::String`.
`last_error()` reads the last error recorded by the SDK on the current thread; `clear_error()`
clears it.

`from_string_with_errors` variants return `(Union{T,Nothing}, ParsingErrors)` when the caller
needs the full list of accumulated parse errors.

## 🛠️ Development

### Setup

```bash
git clone https://github.com/dnv-opensource/vista-sdk.git
cd vista-sdk/bindings/julia/VistaSdk.jl

julia --project=. -e 'import Pkg; Pkg.build(verbose=true)'
julia --project=. -e 'import Pkg; Pkg.test()'
```

### API documentation

Generate and open the full API docs locally:

```bash
just docs
```

Or serve them with live reload while editing docstrings:

```bash
just docs-live
```

### Public API surface

Every file under `src/` that is not `src/ffi/` is public API. The `src/ffi/` subtree contains
the raw `ccall` declarations and is not meant to be used directly.

Types that have both an owned variant and a borrowing variant follow the `T` / `TRef` naming
convention (`GmodNode` / `GmodNodeRef`, `GmodPath` / `GmodPathRef`, etc.). Types that only exist
as borrowing views with no owned counterpart (`Gmod`, `Codebooks`, `Locations`, etc.) carry no
`Ref` suffix: the suffix only appears when there is an owned twin.

### Project structure

```
bindings/julia/VistaSdk.jl/
├── benches/            # BenchmarkTools.jl benchmarks
├── examples/
│   ├── core/           # VIS, Gmod, LocalId, locations, codebooks
│   ├── query/          # GmodPathQuery, LocalIdQuery, MetadataTagsQuery
│   ├── transport/      # DataChannelList, TimeSeriesData, ShipId, Iso19848
│   └── types/          # Decimal, DateTime
├── src/
│   ├── core/           # Safe wrappers: Vis, Gmod, LocalId, etc.
│   ├── ffi/            # Raw ccall declarations (not public API)
│   ├── query/          # Safe wrappers: GmodPathQuery, LocalIdQuery, etc.
│   ├── transport/      # Safe wrappers: DataChannel, TimeSeriesData, etc.
│   ├── types/          # Decimal, DateTime, DateTimeOffset, TimeSpan
│   └── VistaSdk.jl
├── test/               # Test suites
├── deps/
│   └── build.jl        # CMake orchestration + version codegen
├── Project.toml
├── justfile
└── README.md
```

## 🔖 For Maintainers

### Version numbering

The package version tracks the C++ SDK version. Version management is handled via the
`justfile`: `just sync-version` reads the `VERSION` from `cpp/CMakeLists.txt` and updates
`Project.toml` automatically. CI performs this step as part of the build and release workflow.

### Creating a release

1. Bump `VERSION` in `cpp/CMakeLists.txt`, then run `just sync-version` to propagate it to `Project.toml`
2. Run `just test` to verify the full suite passes
3. Tag the commit `vX.Y.Z` and push

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes with tests
4. Run the test suite (`just test`)
5. Commit your changes (`git commit -m 'feat(julia): add amazing feature'`)
6. Push to the branch (`git push origin feature/amazing-feature`)
7. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](../../../LICENSE) file for details.

## 🔗 Links

- **Main SDK**: [github.com/dnv-opensource/vista-sdk](https://github.com/dnv-opensource/vista-sdk)
- **C++ README**: [cpp/README.md](../../../cpp/README.md)
- **C API README**: [cpp/c-api/README.md](../../../cpp/c-api/README.md)
- **Issues**: [GitHub Issues](https://github.com/dnv-opensource/vista-sdk/issues)

## 💬 Support

Open an issue on [GitHub Issues](https://github.com/dnv-opensource/vista-sdk/issues) or refer to the [main README](../../../README.md) for SDK-level documentation.
