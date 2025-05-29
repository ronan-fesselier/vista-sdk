# Vista SDK - C++ Implementation

This directory contains the C++ implementation of the Vista SDK, a native library port of the master C# version for working with the DNV Vessel Information Structure (VIS) standard, ISO 19847/19848, and related functionality.

## Status

> [!NOTE]
> This C++ SDK is currently under active development, aiming for feature parity and API alignment with the established C# SDK.
> While core functionalities are being implemented and refined, some features might be missing or under further development.

## Progress Towards C# Parity

-   **Implemented:**

    -   Core `VIS` entry point and caching mechanism for loaded resources.
    -   `Gmod` loading, parsing (from DTOs), and `GmodNode` representation with full traversal capabilities.
    -   `GmodPath`: Complete parsing implementation including validation, individualization sets, and path construction. Supports both simple "VE" paths and complex paths with location individualization.
    -   `Codebooks` loading, parsing (from DTOs), `Codebook` representation, and `MetadataTag` lookup/creation.
    -   `Locations` loading and parsing (from DTOs) with comprehensive location parsing and validation.
    -   `LocationBuilder` with full immutable builder pattern, fluent interface, and comprehensive validation.
    -   `LocalId` parsing and immutable builder pattern (`LocalIdBuilder`) with full validation.
    -   `LocalIdBuilder` with comprehensive segment validation, metadata handling, and immutable fluent interface.
    -   `GmodVersioning` logic for converting GMOD IDs between versions with complete path transformation support.
    -   `GmodTraversal` utility for tree traversal operations with customizable handlers.
    -   Internal Data Transfer Objects (DTOs) for robust resource deserialization.
    -   Embedded resource handling, including decompression of gzipped JSON data.
    -   `ChdDictionary` (Perfect Hashing utility) - Includes input validation for empty/duplicate keys.
        _Note: The binary format of the dictionary is incompatible with the C# version due to differences in hash inputs and internal caching._
    -   `ParsingErrors` mechanism for collecting and reporting issues during parsing operations.
    -   `ImoNumber` with full validation and parsing capabilities.
    -   Complete `VisVersion` enumeration and extension methods.
    -   `UniversalIdBuilder`: Complete implementation with parsing, building, and validation capabilities.
    -   `UniversalId`: Full implementation with parsing, construction, and string representation.

-   **Partially Implemented / In Progress:**

    -   _None currently_

-   **Not Yet Implemented:**
    -   `GmodPathQuery`
    -   `LocalIdQuery`
    -   `LocalIdQueryBuilder`
    -   `MetadataTagsQuery`
    -   MQTT specifics
    -   Benchmarking suite.

## Core Features Implemented

-   **VIS Entry Point:** Central access via `dnv::vista::sdk::VIS` with comprehensive resource caching.
-   **Gmod:** Complete representation and parsing of the Generic Product Model (`dnv::vista::sdk::Gmod`, `dnv::vista::sdk::GmodNode`) with tree traversal capabilities.
-   **GmodPath:** Full path parsing, validation, and construction with support for individualization sets and location-based paths (`dnv::vista::sdk::GmodPath`).
-   **GmodTraversal:** Utility for traversing Gmod trees with customizable handler functions (`dnv::vista::sdk::GmodTraversal`).
-   **Codebooks:** Complete handling of metadata tags and standard values (`dnv::vista::sdk::Codebooks`, `dnv::vista::sdk::Codebook`, `dnv::vista::sdk::MetadataTag`).
-   **Locations:** Full representation, parsing, and building of standard locations with validation (`dnv::vista::sdk::Locations`, `dnv::vista::sdk::LocationBuilder`) with immutable fluent interface.
-   **Location Builder:** Complete building and validation of location objects (`dnv::vista::sdk::LocationBuilder`) with immutable fluent interface.
-   **Local ID:** Complete building, parsing, and representation of Local IDs (`dnv::vista::sdk::LocalId`, `dnv::vista::sdk::LocalIdBuilder`) with comprehensive validation.
-   **Universal ID:** Complete building, parsing, and representation of Universal IDs (`dnv::vista::sdk::UniversalId`, `dnv::vista::sdk::UniversalIdBuilder`) with full validation and string conversion.
-   **IMO Numbers:** Full parsing and validation of IMO numbers (`dnv::vista::sdk::ImoNumber`).
-   **Gmod Versioning:** Complete conversion logic between different Gmod versions (`dnv::vista::sdk::GmodVersioning`) with path transformation support.
-   **Data Transfer Objects (DTOs):** Comprehensive internal structures for loading data from resources (`dnv::vista::sdk::GmodDto`, `dnv::vista::sdk::CodebooksDto`, `dnv::vista::sdk::LocationsDto`, etc.).
-   **Resource Loading:** Complete loading system for gzipped JSON resources embedded in the library (`dnv::vista::sdk::EmbeddedResource`).
-   **Perfect Hashing:** Utility for efficient string lookups (`dnv::vista::sdk::ChdDictionary`) with thread-local caching.
-   **Error Handling:** Comprehensive mechanism for accumulating parsing errors (`dnv::vista::sdk::ParsingErrors`, `dnv::vista::sdk::LocalIdParsingErrorBuilder`, `dnv::vista::sdk::LocationParsingErrorBuilder`).
-   **Version Management:** Complete VIS version enumeration and utilities (`dnv::vista::sdk::VisVersion`, `dnv::vista::sdk::VisVersionExtensions`).

## Architecture and Design Patterns

### Core Interfaces

The C++ SDK follows a clear interface-based architecture:

-   **`ILocalId`:** Abstract interface for LocalId implementations with template-based design for type safety.
-   **`ILocalIdBuilder`:** Abstract interface for LocalIdBuilder implementations with fluent API design.
-   **`IUniversalId`:** Abstract interface for UniversalId implementations with immutable design patterns.
-   **`IUniversalIdBuilder`:** Complete interface for UniversalId builders with comprehensive validation.

### Template-Based Design

The SDK leverages C++20 templates and concepts extensively:

-   Template-based visitor patterns for location parsing.
-   Concept-based constraints for type safety.
-   Template specializations for performance optimization.

### Memory Management

Memory management patterns:

-   RAII principles throughout the codebase.
-   Smart pointers (`std::unique_ptr`, `std::shared_ptr`) for automatic memory management.
-   Careful ownership tracking in complex objects like `GmodPath`.
-   Thread-local storage for performance-critical caching in `ChdDictionary`.

## Comparison with C# SDK

This C++ implementation follows the same core design principles (Immutability, Builder Pattern, VIS entry point) as the C# SDK but utilizes C++ language features and standard libraries.

| Feature             | C++ Implementation                                                                        | C# Implementation                                          | Notes                                                                     |
| :------------------ | :---------------------------------------------------------------------------------------- | :--------------------------------------------------------- | :------------------------------------------------------------------------ |
| **Language**        | C++20                                                                                     | C# (.NET)                                                  | Leverages modern C++ features including concepts, ranges, and coroutines. |
| **Immutability**    | `const` correctness, returning new builder instances, immutable by design                 | `readonly`, records (`struct`/`class`), `with` expressions |                                                                           |
| **Optional Values** | `std::optional`                                                                           | Nullable types (`?`)                                       | Standard C++ approach with comprehensive null-safety patterns.            |
| **Collections**     | `std::vector`, `std::unordered_map`, `std::deque`, `std::array`                           | `List<T>`, `Dictionary<K,V>`, `T[]`, `FrozenDictionary`    | Uses STL containers with performance optimizations.                       |
| **String Handling** | `std::string`, `std::string_view`, `std::stringstream`, extensive view usage              | `string`, `ReadOnlySpan<char>`, `StringBuilder`            | C++ version uses string_view extensively for performance.                 |
| **Error Handling**  | `std::exception` hierarchy, comprehensive `ParsingErrors` system, `std::invalid_argument` | `.NET` exceptions, `ParsingErrors` struct                  |
| **Hashing (CHD)**   | FNV1a/CRC32 (SSE4.2), thread-local cache, comprehensive input validation                  | FNV1a/CRC32 (SSE4.2)                                       | **Incompatible binary formats** due to different hash inputs.             |
| **Logging**         | `spdlog` with extensive diagnostic logging throughout                                     | Minimal built-in logging                                   |
| **Build System**    | CMake (`FetchContent`) with comprehensive dependency management                           | .NET SDK (MSBuild/NuGet)                                   | CMake configuration with cross-platform support.                          |
| **Dependencies**    | `nlohmann/json`, `spdlog`, `zlib`, `fmt`, `gtest`, `libcpuid`                             | NuGet packages                                             | Managed via `FetchContent` with version pinning.                          |
| **Path Parsing**    | Validation with root node checking and structure validation                               | Basic validation                                           |

## Features

### Perfect Hashing (ChdDictionary)

The C++ implementation includes a perfect hashing system (as well as c# version):

-   Thread-local caching for performance optimization.
-   Comprehensive input validation (empty keys, duplicates).
-   SSE4.2 accelerated hashing when available.
-   **Note:** Binary format incompatible with C# version due to different hash inputs and caching strategy.

### Path Parsing and Validation

`GmodPath` implementation:

-   Root node validation ensuring paths start from valid root nodes.
-   Complex individualization set handling for location-based paths.
-   Comprehensive parent-child relationship validation.
-   Support for both simple and complex path structures.

### Memory-Efficient Design

-   Extensive use of `std::string_view` to avoid unnecessary string copies.
-   Smart pointer usage for automatic memory management.
-   Thread-local storage for performance-critical components.

## API Patterns

-   **Immutability:** Core domain objects (`LocalId`, `GmodPath`, `MetadataTag`, etc.) are immutable once created.
-   **Builder Pattern:** Objects like `LocalId` are constructed using an immutable builder (`LocalIdBuilder`) where modification methods return new builder instances.
-   **Interface-Based Design:** Clear separation of concerns through abstract interfaces (`ILocalId`, `ILocalIdBuilder`).
-   **Template-Based Safety:** Extensive use of C++20 templates and concepts for type safety and performance.

## Performance Characteristics

-   **Memory Efficiency:** Careful memory management with RAII and smart pointers.
-   **CPU Optimization:** SSE4.2 accelerated operations where available.
-   **Caching Strategy:** Thread-local caching in performance-critical paths.
-   **String Optimization:** Extensive use of `string_view` to minimize allocations.

## Testing Infrastructure

Comprehensive test coverage using GoogleTest:

-   **Unit Tests:** Complete coverage of core functionality.
-   **Integration Tests:** End-to-end testing of complex scenarios.
-   **Performance Tests:** Validation of performance characteristics.
-   **Parameterized Tests:** Extensive test matrices for edge cases.

Test suites include:

-   `CodebookTests` and `CodebooksTests`
-   `GmodPathTests`
-   `GmodTests`
-   `GmodVersioningTests`
-   `ImoNumberTests`
-   `ISOStringTests`
-   `LocalIdTests`
-   `LocationsTests`
-   `UniversalIdTests`

## TODO List

-   **Query Systems:**

    -   Implement `GmodPathQuery` for advanced path querying and filtering capabilities.
    -   Implement `LocalIdQuery` for querying and filtering LocalId collections.
    -   Implement `LocalIdQueryBuilder` with fluent interface for building complex LocalId queries.
    -   Implement `MetadataTagsQuery` for advanced metadata tag searching and filtering.

-   **Cross-Platform Alignment:**

    -   Evaluate hash algorithm alignment between C++ and C# implementations for scenarios requiring cross-platform hash consistency.

-   **Testing:**

    -   Implement MQTT-related tests (`Test_LocalId_Mqtt`, `Test_LocalId_Mqtt_Invalid`, `Test_LocalIdBuilder_Mqtt`) when MQTT functionality is added.

-   **Performance & Optimization:**

    -   Investigate C++20 heterogeneous lookup for `std::unordered_map` instances.
    -   Further optimize string handling to minimize temporary allocations.
    -   Consider configurable hash functions in `ChdDictionary`.

-   **Benchmarking:**

    -   Develop a comprehensive benchmarking suite comparable to the C# SDK.

-   **Documentation:**

    -   Complete API documentation using Doxygen.
    -   Performance characteristics documentation.

-   **Core Implementation:**

    -   Implement MQTT-specific aspects for `LocalId` (parsing from topics, builder methods, topic validation).

## Build Requirements

-   **C++20 compliant compiler** (MSVC 2019+, GCC 10+, Clang 12+)
-   **CMake 3.20+**
-   **Dependencies automatically fetched via FetchContent:**
    -   nlohmann/json v3.12.0
    -   spdlog (latest)
    -   zlib (latest)
    -   fmt v11.0.2
    -   GoogleTest v1.16.0
    -   libcpuid (latest)

## C++ Coding Conventions

The C++ Vista SDK follows a consistent coding style throughout the codebase.

> [!NOTE]
> This represents my personal coding style preference for C++ development.
> If requested by the project owner, I can adapt the codebase to follow C# naming conventions (PascalCase for methods, etc.) to maintain consistency across language implementations.

### **Naming Conventions**

-   **Classes/Structs:** PascalCase - `LocalIdBuilder`, `GmodPath`, `UniversalId`
-   **Methods/Functions:** camelCase - `toString()`, `hashCode()`, `tryParse()`
-   **Member Variables:** `m_` prefix - `m_imoNumber`, `m_localIdBuilder`, `m_visVersion`
-   **Static Members:** `s_` prefix - `s_leafTypesSet`, `s_hashCache`
-   **Constants:** SCREAMING_SNAKE_CASE - `NUM_CODEBOOKS`, `FNV_OFFSET_BASIS`
-   **Namespaces:** Lowercase with scope resolution - `dnv::vista::sdk`, `dnv::vista::sdk::tests`
-   **Enums:** PascalCase - `CodebookName::Position`, `VisVersion::v3_8a`

### **Code Organization**

-   **Include Order:** PCH first, then main public header, then project headers
-   **Namespace Usage:** Full namespace qualification preferred over `using` statements
-   **Forward Declarations:** Extensive use to minimize compilation dependencies and speed-up recompilation

### **Method Conventions**

-   **Getters:** Simple property names (Qt style) - `imoNumber()`, `localId()`, `visVersion()`
-   **Builders:** Fluent interface with method chaining - `withImoNumber().withLocalId()`, mirroring the c# master implementation
-   **Validation/state inspection:** `isValid()`, `tryParse()`, `hasValue()` patterns
-   **Factory Methods:** Static `create()`, `parse()` methods for object construction

### **Documentation Standards**

-   **Doxygen Comments:** Comprehensive documentation using `@brief`, `@details`, `@param`, `@return`
-   **File Headers:** Standard file documentation with purpose and brief description
-   **Inline Comments (// ):** Only for developpement phase

### **Error Handling**

-   **Exceptions:** `std::invalid_argument`, `std::out_of_range` for validation failures
-   **Optional Types:** `std::optional` for nullable return values
-   **Error Accumulation:** `ParsingErrors` system for collecting multiple validation issues (based on c# master implementation)

### **Performance Considerations**

-   **String Views:** Extensive use of `std::string_view` to avoid unnecessary copies
-   **Move Semantics:** Proper use of move constructors and assignment operators
-   **Thread Safety:** Thread-local storage for performance-critical caching
-   **Template Usage:** C++20 templates and concepts for type safety and performance

### **Code Formatting**

-   **Braces:** Opening brace on same line for all constructs - methods, classes, namespaces, and control structures
-   **Indentation:** Tabs for indentation, consistent throughout
-   **Line Length:** Reasonable line lengths with proper wrapping
-   **Spacing:** Consistent spacing around operators and function parameters
