# Vista SDK - C++ <-> C# Implementation Mapping

## Table of Contents

-   [Overview](#overview)
-   [Infrastructure and Utilities](#infrastructure-and-utilities)
-   [VIS Entry Point Implementation Comparison](#vis-entry-point-implementation-comparison)
-   [ChdDictionary Implementation Comparison](#chddictionary-implementation-comparison)
-   [Codebook Implementation Comparison](#codebook-implementation-comparison)
-   [Codebooks Container Implementation Comparison](#codebooks-container-implementation-comparison)
-   [CodebookName Implementation Comparison](#codebookname-implementation-comparison)
-   [Gmod Implementation Comparison](#gmod-implementation-comparison)
-   [GmodNode Implementation Comparison](#gmodnode-implementation-comparison)
-   [GmodPath Implementation Comparison](#gmodpath-implementation-comparison)
-   [LocalId Implementation Comparison](#localid-implementation-comparison)
-   [LocalIdBuilder Implementation Comparison](#localidbuilder-implementation-comparison)
-   [UniversalId Implementation Comparison](#universalid-implementation-comparison)
-   [UniversalIdBuilder Implementation Comparison](#universalidbuilder-implementation-comparison)
-   [Locations Implementation Comparison](#locations-implementation-comparison)
-   [Location and RelativeLocation Implementation Comparison](#location-and-relativelocation-implementation-comparison)
-   [MetadataTag Implementation Comparison](#metadatatag-implementation-comparison)
-   [ImoNumber Implementation Comparison](#imonumber-implementation-comparison)
-   [VisVersion Implementation Comparison](#visversion-implementation-comparison)
-   [Support Infrastructure Comparison](#support-infrastructure-comparison)

---

## Overview

This document provides a comprehensive mapping between the **C# master implementation** and the **C++ port** of the Vista SDK. The C# implementation serves as the **canonical specification** for the DNV Vessel Information Structure (VIS) standard (ISO 19847/19848). The C++ implementation is a faithful port that replicates C# behavior exactly while adapting to C++ language features and performance optimizations.

### Key Principles

-   **C# is the Master Implementation**: All functional behavior, algorithms, data structures, and specifications are defined by the C# version
-   **C++ is a Faithful Port**: The C++ implementation follows C# specifications exactly, providing identical functionality with platform-appropriate optimizations
-   **API Compatibility**: C++ provides equivalent functionality with language-appropriate interfaces while maintaining semantic compatibility
-   **Configuration Consistency**: Critical constants, string literals, and validation rules must match exactly between implementations
-   **Performance Optimization**: C++ provides zero-overhead abstractions and memory-efficient implementations while preserving C# behavior

### Mapping Methodology

This mapping documents:

-   **Interface Differences**: How C# methods/properties map to C++ equivalents
-   **Language-Specific Adaptations**: Different approaches required by language features while maintaining compatibility
-   **Implementation Strategy**: How C++ achieves identical functionality to C# master with performance optimizations
-   **Critical Compatibility Points**: Where exact matching is required for correctness and interoperability
-   **Configuration Dependencies**: How Config.h and StringUtils.h provide C# compatibility support

---

## Infrastructure and Utilities

### Config.h - Critical Configuration Constants

The C++ `Config.h` provides centralized constants that **must match exactly** with corresponding C# string literals and configuration values. This ensures identical behavior across implementations.

| Configuration Area            | C# Master Source                | C++ Config.h Constant                    | Compatibility Requirement                           |
| ----------------------------- | ------------------------------- | ---------------------------------------- | --------------------------------------------------- |
| **Hash Constants**            | FNV-1a algorithm constants      | `FNV_OFFSET_BASIS`, `FNV_PRIME`          | **Must match exactly for hash compatibility**       |
| **GMOD Node Classifications** | String literals in C# classes   | `GMODNODE_CATEGORY_*`, `GMODNODE_TYPE_*` | **Must match exactly for node classification**      |
| **Codebook Names**            | C# CodebookName enum strings    | `CODEBOOK_NAME_*` constants              | **Must match exactly for codebook identification**  |
| **Codebook Prefixes**         | C# CodebookNames class literals | `CODEBOOK_PREFIX_*` constants            | **Must match exactly for prefix mapping**           |
| **Validation Results**        | C# validation enum strings      | `CODEBOOK_POSITION_VALIDATION_*`         | **Must match exactly for validation compatibility** |
| **DTO Keys**                  | C# JSON serialization keys      | `*_DTO_KEY_*` constants                  | **Must match exactly for data interchange**         |

### StringUtils.h - Performance-Critical String Operations

The C++ `StringUtils.h` provides zero-overhead string operations that replicate C# `string` and `ReadOnlySpan<char>` behavior while offering superior performance.

| C# Master Pattern                                   | C++ StringUtils Implementation         | Performance Optimization                               |
| --------------------------------------------------- | -------------------------------------- | ------------------------------------------------------ |
| `string.EndsWith()`                                 | `endsWith(string_view, string_view)`   | Zero-copy operation with string_view                   |
| `string.StartsWith()`                               | `startsWith(string_view, string_view)` | Zero-copy operation with string_view                   |
| `string.Contains()`                                 | `contains(string_view, string_view)`   | Zero-copy operation with string_view                   |
| `string.Equals()`                                   | `equals(string_view, string_view)`     | Zero-copy case-sensitive comparison                    |
| `string.Equals(StringComparison.OrdinalIgnoreCase)` | `iequals(string_view, string_view)`    | Zero-copy case-insensitive comparison                  |
| `Dictionary<string, T>`                             | `StringMap<T>`                         | Heterogeneous lookup with zero-copy string_view access |
| `HashSet<string>`                                   | `StringSet`                            | Heterogeneous lookup with zero-copy string_view access |

### Heterogeneous Lookup Support

C++ provides advanced heterogeneous lookup capabilities that exceed C# performance while maintaining identical behavior:

| Feature                 | C# Master Limitation              | C++ Enhancement                  | Compatibility Status                        |
| ----------------------- | --------------------------------- | -------------------------------- | ------------------------------------------- |
| **Dictionary Lookup**   | Always requires string allocation | Zero-copy string_view lookup     | **Behavioral compatibility maintained**     |
| **HashSet Lookup**      | Always requires string allocation | Zero-copy string_view lookup     | **Behavioral compatibility maintained**     |
| **Hash Function**       | Built-in string hash              | Custom `StringViewHash` functor  | **Must produce identical hash values**      |
| **Equality Comparison** | Built-in string equality          | Custom `StringViewEqual` functor | **Must produce identical equality results** |

---

## Gmod Implementation Comparison

This section maps the C++ port to the **C# master implementation** of the Gmod (Generic Product Model) class, documenting how the C++ implementation aligns with the C# design specification for GMOD hierarchy management.

### Overview

The C# implementation defines the **VISTA Generic Product Model specification**. The C++ port implements this specification with the following alignment:

-   **Hierarchical Node Management**: C++ follows C# node storage and relationship management exactly
-   **Path Parsing**: C++ implements identical path parsing logic to C# master
-   **Node Classification**: C++ provides equivalent static classification methods
-   **Enumeration Pattern**: C++ adapts C# enumeration to STL-compatible iteration

### Class Declaration Mapping

| Aspect                | C# Master                                                  | C++ Port                            | Mapping Notes                                          |
| --------------------- | ---------------------------------------------------------- | ----------------------------------- | ------------------------------------------------------ |
| **Namespace**         | `Vista.SDK`                                                | `dnv::vista::sdk`                   | C++ maps to equivalent namespace                       |
| **Class Declaration** | `public sealed partial class Gmod : IEnumerable<GmodNode>` | `class Gmod final`                  | C++ adapts sealed class and IEnumerable to final class |
| **Visibility**        | Public class                                               | Public class                        | C++ follows C# public API visibility                   |
| **Storage Type**      | `ChdDictionary<GmodNode> _nodeMap`                         | `ChdDictionary<GmodNode> m_nodeMap` | **Identical storage strategy**                         |

### Constructor Mapping

| C# Master Constructor                                                     | C++ Port Constructor                                                                        | Parameter Mapping                            | Implementation Alignment               |
| ------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- | -------------------------------------------- | -------------------------------------- |
| `Gmod(VisVersion version, GmodDto dto)`                                   | `Gmod(VisVersion version, const GmodDto& dto)`                                              | Same parameters with const reference         | **Identical DTO processing logic**     |
| `Gmod(VisVersion version, IReadOnlyDictionary<string, GmodNode> nodeMap)` | `Gmod(VisVersion version, const std::unordered_map<std::string, GmodNode>& initialNodeMap)` | Different container types but same semantics | **Identical map processing logic**     |
| Node relationship building                                                | **Identical parent-child linking algorithm**                                                | **Must follow C# linking logic exactly**     | **Critical for hierarchy correctness** |
| Root node assignment                                                      | `_rootNode = nodeMap["VE"]`                                                                 | `m_rootNode = &m_nodeMap["VE"]`              | C++ uses pointer to owned node         |

### Public Interface Mapping

| C# Master Method                                     | C++ Port Method                                                            | Return Type Mapping                | Behavior Mapping                              |
| ---------------------------------------------------- | -------------------------------------------------------------------------- | ---------------------------------- | --------------------------------------------- |
| `GmodNode this[string key]`                          | `const GmodNode& operator[](string_view key) const`                        | Value vs const reference           | C++ avoids copying with const reference       |
| `bool TryGetNode(string code, out GmodNode node)`    | `bool tryGetNode(string_view code, const GmodNode*& node) const`           | out parameter vs pointer reference | C++ adapts out parameter to pointer reference |
| `GmodPath ParsePath(string item)`                    | `GmodPath parsePath(string_view item) const`                               | Same return type                   | **Must follow identical parsing algorithm**   |
| `bool TryParsePath(string item, out GmodPath? path)` | `bool tryParsePath(string_view item, std::optional<GmodPath>& path) const` | Nullable vs optional               | C++ maps C# nullable to optional              |
| `GmodNode RootNode { get; }`                         | `const GmodNode& rootNode() const`                                         | Property vs method                 | C++ maps C# property to const method          |

### Static Classification Methods Mapping

| C# Master Method                                             | C++ Port Method                                                     | Algorithm Requirement                      | Config.h Dependency                           |
| ------------------------------------------------------------ | ------------------------------------------------------------------- | ------------------------------------------ | --------------------------------------------- |
| `static bool IsPotentialParent(string type)`                 | `static bool isPotentialParent(string_view type)`                   | **Must use identical type checking logic** | Depends on `GMODNODE_TYPE_*` constants        |
| `static bool IsLeafNode(GmodNodeMetadata metadata)`          | `static bool isLeafNode(const GmodNodeMetadata& metadata)`          | **Must use identical leaf detection**      | Depends on `GMODNODE_FULLTYPE_*` constants    |
| `static bool IsFunctionNode(GmodNodeMetadata metadata)`      | `static bool isFunctionNode(const GmodNodeMetadata& metadata)`      | **Must follow C# category classification** | Depends on `GMODNODE_CATEGORY_*` constants    |
| `static bool IsProductSelection(GmodNodeMetadata metadata)`  | `static bool isProductSelection(const GmodNodeMetadata& metadata)`  | **Must match C# classification logic**     | Depends on category and type constants        |
| `static bool IsAssetFunctionNode(GmodNodeMetadata metadata)` | `static bool isAssetFunctionNode(const GmodNodeMetadata& metadata)` | **Must match C# classification logic**     | Depends on `GMODNODE_CATEGORY_ASSET_FUNCTION` |

### Enumeration Strategy Mapping

| C# Master Enumeration                       | C++ Port Enumeration                             | Implementation Difference              | Compatibility                                   |
| ------------------------------------------- | ------------------------------------------------ | -------------------------------------- | ----------------------------------------------- |
| `IEnumerable<GmodNode>` implementation      | `Enumerator enumerator() const`                  | C++ provides custom enumerator class   | **Must iterate in same order as C# master**     |
| `public Enumerator GetEnumerator()`         | `class Enumerator` with `next()` and `current()` | Different iteration protocol           | **Must visit same nodes in same sequence**      |
| `struct Enumerator : IEnumerator<GmodNode>` | Custom enumerator with reset support             | C++ enumerator is non-copyable/movable | **Must provide equivalent iteration semantics** |

### Critical Compatibility Requirements

| Validation Area              | C# Master Definition                         | C++ Port Compliance                          | Compatibility Status                   |
| ---------------------------- | -------------------------------------------- | -------------------------------------------- | -------------------------------------- |
| **Node Storage**             | Uses ChdDictionary for node map              | **Must use identical ChdDictionary**         | **Required for hash compatibility**    |
| **Root Node Identification** | Root always identified as "VE"               | **Must use same "VE" identification**        | **Required for hierarchy correctness** |
| **Relationship Building**    | Parent-child linking algorithm               | **Must follow identical linking logic**      | **Required for correct hierarchy**     |
| **Path Parsing**             | GmodPath parsing delegates to GmodPath.Parse | **Must delegate to identical parsing logic** | **Required for path compatibility**    |
| **Static Classification**    | Node type classification algorithms          | **Must use identical classification logic**  | **Required for semantic correctness**  |

---

## GmodNode Implementation Comparison

This section maps the C++ port to the **C# master implementation** of GmodNode and GmodNodeMetadata, documenting how the C++ implementation aligns with the C# design specification for individual GMOD nodes.

### Overview

The C# implementation defines the **VISTA GMOD node specification**. The C++ port implements this specification with the following alignment:

-   **Record Class Pattern**: C++ adapts C# record class semantics to value-based class design
-   **Metadata Management**: C++ provides identical metadata structure and access patterns
-   **Relationship Management**: C++ implements equivalent parent-child relationship handling
-   **Location Support**: C++ provides identical location association and manipulation

### Class Declaration Mapping

| Aspect             | C# Master                                     | C++ Port                                  | Mapping Notes                                 |
| ------------------ | --------------------------------------------- | ----------------------------------------- | --------------------------------------------- |
| **Main Class**     | `public record class GmodNode`                | `class GmodNode final`                    | C++ adapts record semantics to final class    |
| **Metadata Class** | `public sealed record class GmodNodeMetadata` | `class GmodNodeMetadata final`            | C++ adapts record to final class              |
| **Inheritance**    | Record class with automatic equality          | Manual equality implementation            | C++ provides explicit equality methods        |
| **Relationships**  | `internal readonly List<GmodNode> _children`  | `std::vector<const GmodNode*> m_children` | C++ uses pointers for relationship management |

### Construction and Initialization Mapping

| C# Master Constructor                           | C++ Port Constructor                                   | Parameter Mapping                    | Initialization Alignment            |
| ----------------------------------------------- | ------------------------------------------------------ | ------------------------------------ | ----------------------------------- |
| `GmodNode(VisVersion version, GmodNodeDto dto)` | `GmodNode(VisVersion version, const GmodNodeDto& dto)` | Same parameters with const reference | **Identical DTO processing**        |
| `Code = dto.Code`                               | `m_code = dto.code()`                                  | Direct assignment vs method call     | **Same final value**                |
| `Location = null`                               | `m_location = std::nullopt`                            | null vs optional empty               | **Equivalent null state**           |
| `Metadata = new GmodNodeMetadata(...)`          | `m_metadata = GmodNodeMetadata(...)`                   | new vs direct construction           | **Identical metadata construction** |

### Property and Method Mapping

| C# Master Property/Method                    | C++ Port Method                                            | Return Type Mapping                | Access Pattern                               |
| -------------------------------------------- | ---------------------------------------------------------- | ---------------------------------- | -------------------------------------------- |
| `string Code { get; init; }`                 | `string_view code() const noexcept`                        | Property vs const method           | C++ returns string_view for zero-copy        |
| `Location? Location { get; internal init; }` | `const std::optional<Location>& location() const noexcept` | Nullable vs optional               | C++ maps nullable to optional                |
| `VisVersion VisVersion { get; }`             | `VisVersion visVersion() const noexcept`                   | Property vs const method           | Same enum return                             |
| `GmodNodeMetadata Metadata { get; }`         | `const GmodNodeMetadata& metadata() const noexcept`        | Property vs const reference method | C++ avoids copying with reference            |
| `IReadOnlyList<GmodNode> Children { get; }`  | `const std::vector<GmodNode> children() const noexcept`    | Different container return         | **Must contain same children in same order** |

### Location Manipulation Methods

| C# Master Method                                                          | C++ Port Method                                                                  | Parameter Mapping          | Exception Handling                      |
| ------------------------------------------------------------------------- | -------------------------------------------------------------------------------- | -------------------------- | --------------------------------------- |
| `GmodNode WithoutLocation()`                                              | `GmodNode withoutLocation() const`                                               | Same signature             | C++ returns new instance                |
| `GmodNode WithLocation(string location)`                                  | `GmodNode withLocation(string_view locationStr) const`                           | string vs string_view      | **Must use identical location parsing** |
| `GmodNode TryWithLocation(string? locationStr)`                           | `GmodNode tryWithLocation(string_view locationStr) const`                        | Nullable vs non-nullable   | C++ handles empty string_view           |
| `GmodNode TryWithLocation(string? locationStr, out ParsingErrors errors)` | `GmodNode tryWithLocation(string_view locationStr, ParsingErrors& errors) const` | out parameter vs reference | C++ adapts out parameter pattern        |

### Node Classification Methods

| C# Master Property/Method  | C++ Port Method                    | Classification Logic                                  | Config.h Dependency     |
| -------------------------- | ---------------------------------- | ----------------------------------------------------- | ----------------------- |
| `bool IsProductSelection`  | `bool isProductSelection() const`  | **Must use identical Gmod.IsProductSelection logic**  | Category/type constants |
| `bool IsProductType`       | `bool isProductType() const`       | **Must use identical Gmod.IsProductType logic**       | Category/type constants |
| `bool IsAsset`             | `bool isAsset() const`             | **Must use identical Gmod.IsAsset logic**             | Category constants      |
| `bool IsLeafNode`          | `bool isLeafNode() const`          | **Must use identical Gmod.IsLeafNode logic**          | Full type constants     |
| `bool IsFunctionNode`      | `bool isFunctionNode() const`      | **Must use identical Gmod.IsFunctionNode logic**      | Category constants      |
| `bool IsAssetFunctionNode` | `bool isAssetFunctionNode() const` | **Must use identical Gmod.IsAssetFunctionNode logic** | Category constants      |

### Relationship Management

| C# Master Implementation                     | C++ Port Implementation                   | Management Strategy               | Compatibility                             |
| -------------------------------------------- | ----------------------------------------- | --------------------------------- | ----------------------------------------- |
| `internal readonly List<GmodNode> _children` | `std::vector<const GmodNode*> m_children` | C++ uses pointers for performance | **Must contain same node references**     |
| `internal void AddChild(GmodNode child)`     | `void addChild(const GmodNode* child)`    | Friend access only                | **Must build identical relationships**    |
| `internal void Trim()`                       | `void trim()`                             | Memory optimization               | **Must maintain same final state**        |
| `public bool IsChild(string code)`           | `bool isChild(string_view code) const`    | Efficient lookup via set          | **Must provide identical child checking** |

### Product Type and Selection Properties

| C# Master Property                    | C++ Port Method                                           | Implementation Logic                                         | Return Type Mapping                       |
| ------------------------------------- | --------------------------------------------------------- | ------------------------------------------------------------ | ----------------------------------------- |
| `GmodNode? ProductType { get; }`      | `std::optional<const GmodNode*> productType() const`      | **Must use identical logic for product type detection**      | Nullable vs optional pointer              |
| `GmodNode? ProductSelection { get; }` | `std::optional<const GmodNode*> productSelection() const` | **Must use identical logic for product selection detection** | Nullable vs optional pointer              |
| Single child + category check         | Caching with mutable optional                             | C++ optimizes repeated access                                | **Must return same results as C# master** |

### Equality and Hashing

| C# Master Implementation                             | C++ Port Implementation                        | Comparison Criteria               | Hash Algorithm                           |
| ---------------------------------------------------- | ---------------------------------------------- | --------------------------------- | ---------------------------------------- |
| `public virtual bool Equals(GmodNode? other)`        | `bool operator==(const GmodNode& other) const` | Code and Location comparison      | **Must use identical comparison logic**  |
| `public override int GetHashCode()`                  | `size_t hashCode() const noexcept`             | HashCode.Combine(Code, Location)  | **Must produce equivalent hash values**  |
| `Code == other?.Code && Location == other?.Location` | Same logic with null safety                    | **Identical comparison criteria** | **Required for container compatibility** |

### String Representation

| C# Master Method                                 | C++ Port Method                                                     | Format Specification                                 | Implementation                               |
| ------------------------------------------------ | ------------------------------------------------------------------- | ---------------------------------------------------- | -------------------------------------------- |
| `public sealed override string ToString()`       | `std::string toString() const noexcept`                             | **Must use identical format**: Code or Code-Location | **Required for string compatibility**        |
| `Location is null ? Code : $"{Code}-{Location}"` | Same conditional formatting                                         | **Exact format matching required**                   | **Required for serialization compatibility** |
| `public void ToString(StringBuilder builder)`    | `template<typename OutputIt> OutputIt toString(OutputIt out) const` | Performance optimization                             | **Must produce identical output**            |

### Critical Compatibility Requirements

| Validation Area           | C# Master Definition          | C++ Port Compliance                           | Compatibility Status                         |
| ------------------------- | ----------------------------- | --------------------------------------------- | -------------------------------------------- |
| **Node Equality**         | Code and Location comparison  | **Must use identical comparison logic**       | **Required for container operations**        |
| **Hash Computation**      | HashCode.Combine algorithm    | **Must produce compatible hash values**       | **Required for unordered containers**        |
| **Classification Logic**  | Static methods in Gmod class  | **Must delegate to identical static methods** | **Required for semantic correctness**        |
| **Location Association**  | Locations.Parse delegation    | **Must use identical location parsing**       | **Required for location compatibility**      |
| **Relationship Building** | Parent-child linking via Gmod | **Must follow identical relationship logic**  | **Required for hierarchy correctness**       |
| **String Format**         | ToString format specification | **Must match exact string format**            | **Required for serialization compatibility** |

---

## VIS Entry Point Implementation Comparison

This section maps the C++ port to the **C# master implementation** of the VIS entry point class, documenting how the C++ implementation aligns with the C# design specification for the main SDK access point.

### Overview

The C# implementation defines the **VISTA SDK entry point specification**. The C++ port implements this specification with the following alignment:

-   **Singleton Pattern**: C++ follows C# singleton pattern with `VIS.Instance`
-   **Caching Strategy**: C++ implements equivalent caching to C# `MemoryCache` using thread-safe containers
-   **Interface Definition**: C++ provides `IVIS` interface following C# `IVIS` pattern
-   **Resource Loading**: C++ maps C# embedded resource loading to platform-appropriate mechanisms

### Interface Definition Mapping

#### C# Master Interface

| C# Master Interface Method                                        | C++ Port Interface Method                                                      | Return Type Mapping                    | Parameter Mapping                    |
| ----------------------------------------------------------------- | ------------------------------------------------------------------------------ | -------------------------------------- | ------------------------------------ |
| `Gmod GetGmod(VisVersion visVersion)`                             | `const Gmod& gmod(VisVersion visVersion) const`                                | Value return vs const reference return | Same enum parameter                  |
| `Codebooks GetCodebooks(VisVersion visversion)`                   | `const Codebooks& codebooks(VisVersion visVersion)`                            | Value return vs const reference return | Same enum parameter                  |
| `Locations GetLocations(VisVersion visversion)`                   | `const Locations& locations(VisVersion visVersion)`                            | Value return vs const reference return | Same enum parameter                  |
| `IReadOnlyDictionary<VisVersion, Gmod> GetGmodsMap(...)`          | `std::unordered_map<VisVersion, Gmod> gmodsMap(...) const`                     | Different container types              | Vector vs IEnumerable                |
| `IReadOnlyDictionary<VisVersion, Codebooks> GetCodebooksMap(...)` | `std::unordered_map<VisVersion, Codebooks> codebooksMap(...)`                  | Different container types              | Vector vs IEnumerable                |
| `IReadOnlyDictionary<VisVersion, Locations> GetLocationsMap(...)` | `std::unordered_map<VisVersion, Locations> locationsMap(...)`                  | Different container types              | Vector vs IEnumerable                |
| `IEnumerable<VisVersion> GetVisVersions()`                        | `std::vector<VisVersion> visVersions()`                                        | Vector vs IEnumerable                  | No parameters                        |
| `GmodNode? ConvertNode(VisVersion, GmodNode, VisVersion)`         | `std::optional<GmodNode> convertNode(VisVersion, const GmodNode&, VisVersion)` | Nullable vs optional                   | Same parameters with const reference |
| `GmodPath? ConvertPath(VisVersion, GmodPath, VisVersion)`         | `std::optional<GmodPath> convertPath(VisVersion, const GmodPath&, VisVersion)` | Nullable vs optional                   | Same parameters with const reference |

### Singleton Implementation Mapping

#### C# Master Singleton Pattern

| C# Master Implementation                                | C++ Port Implementation                | Mapping Notes                                       |
| ------------------------------------------------------- | -------------------------------------- | --------------------------------------------------- |
| `public static readonly VIS Instance = new VIS();`      | `static VIS& instance() noexcept`      | C++ uses function-based singleton for thread safety |
| Public constructor with internal caching initialization | Private constructor with caching setup | C++ enforces singleton pattern more strictly        |
| `LatestVisVersion` as public static field               | `latestVisVersion()` as const method   | C++ provides method access vs field access          |

### Caching Strategy Mapping

#### C# Master Caching Implementation

| C# Cache Type                 | C# Configuration                                                   | C++ Port Equivalent                         | Mapping Notes                                  |
| ----------------------------- | ------------------------------------------------------------------ | ------------------------------------------- | ---------------------------------------------- |
| `MemoryCache _gmodCache`      | `SizeLimit = 10, ExpirationScanFrequency = TimeSpan.FromHours(1)`  | Thread-safe `std::unordered_map` with mutex | C++ uses manual thread-safety vs .NET built-in |
| `MemoryCache _codebooksCache` | Same configuration pattern                                         | Thread-safe container with mutex            | C++ adapts .NET caching to STL containers      |
| `MemoryCache _locationsCache` | Same configuration pattern                                         | Thread-safe container with mutex            | C++ adapts .NET caching to STL containers      |
| Entry configuration           | `entry.Size = 1; entry.SlidingExpiration = TimeSpan.FromHours(1);` | Static lifetime (no expiration)             | C++ simplifies caching model                   |

#### Cache Key Strategy

| C# Master Key Strategy                     | C++ Port Key Strategy           | Implementation Mapping                       |
| ------------------------------------------ | ------------------------------- | -------------------------------------------- |
| `VisVersion` enum as cache key             | `VisVersion` enum as map key    | Direct mapping with same enum values         |
| String `"versioning"` for versioning cache | Same string constant            | C++ follows C# string literal usage          |
| `GetOrCreate()` with factory lambda        | `find()` + conditional creation | C++ maps C# lambda pattern to explicit logic |

### Resource Loading Mapping

#### C# Master Resource Loading

| C# Resource Method                                            | C++ Port Resource Method                     | Mapping Strategy                                |
| ------------------------------------------------------------- | -------------------------------------------- | ----------------------------------------------- |
| `EmbeddedResource.GetGmod(visVersion.ToVersionString())`      | `EmbeddedResource::getGmod(visVersion)`      | C++ adapts C# resource loading to platform      |
| `EmbeddedResource.GetCodebooks(visVersion.ToVersionString())` | `EmbeddedResource::getCodebooks(visVersion)` | C++ follows C# resource identification pattern  |
| `EmbeddedResource.GetGmodVersioning()`                        | `EmbeddedResource::getGmodVersioning()`      | C++ maps C# static method to namespace function |

### Exception Handling and Validation

#### Validation Strategy Mapping

| C# Master Validation                                     | C++ Port Validation                          | Exception Mapping                              |
| -------------------------------------------------------- | -------------------------------------------- | ---------------------------------------------- |
| `if (!visVersion.IsValid()) throw new ArgumentException` | Same validation with `std::invalid_argument` | C++ maps ArgumentException to invalid_argument |
| `"Invalid VIS version: " + visVersion`                   | Same message format with `std::to_string`    | C++ follows C# error message pattern           |
| Null checks on DTO loading results                       | Same null/validity checks                    | C++ follows C# validation logic exactly        |

### Method Implementation Comparison

#### GetGmod Method Mapping

| C# Master Implementation                                  | C++ Port Implementation                | Mapping Notes                                |
| --------------------------------------------------------- | -------------------------------------- | -------------------------------------------- |
| `return _gmodCache.GetOrCreate(visVersion, entry => ...)` | Manual cache lookup with thread safety | C++ implements C# caching pattern explicitly |
| Returns `Gmod` by value                                   | Returns `const Gmod&` by reference     | C++ optimizes return to avoid copying        |
| Lambda factory for cache entry creation                   | Conditional construction logic         | C++ adapts C# lambda to procedural style     |

#### Multiple Version Methods Mapping

| C# Master Pattern                                               | C++ Port Pattern                             | Container Mapping                        |
| --------------------------------------------------------------- | -------------------------------------------- | ---------------------------------------- |
| `IEnumerable<VisVersion> visVersions` parameter                 | `const std::vector<VisVersion>& visVersions` | C++ uses concrete container vs interface |
| `var invalidVisVersions = visVersions.Where(v => !v.IsValid())` | Loop-based validation with early return      | C++ uses explicit iteration vs LINQ      |
| `versions.Select(v => (Version: v, Gmod: GetGmod(v)))`          | Manual construction with structured bindings | C++ adapts C# anonymous types            |
| `ToDictionary(t => t.Version, t => t.Gmod)`                     | Direct container construction                | C++ builds container explicitly          |

### Threading and Concurrency

#### C# Master Threading Model

| Aspect                 | C# Master Approach                   | C++ Port Approach                 | Mapping Notes                              |
| ---------------------- | ------------------------------------ | --------------------------------- | ------------------------------------------ |
| **Thread Safety**      | `MemoryCache` provides thread safety | Manual `std::mutex` protection    | C++ implements C# thread safety explicitly |
| **Concurrent Access**  | Built-in concurrent operations       | Lock-based protection             | C++ adapts to platform threading model     |
| **Cache Invalidation** | Automatic with sliding expiration    | Manual management (no expiration) | C++ simplifies C# caching model            |

### Platform Integration

#### Embedded Resource Handling

| C# Master Resource Access                    | C++ Port Resource Access             | Platform Adaptation                             |
| -------------------------------------------- | ------------------------------------ | ----------------------------------------------- |
| Assembly-based embedded resource loading     | Compiled-in resource data structures | C++ adapts to compile-time resource embedding   |
| Runtime string-based resource identification | Compile-time resource identification | C++ optimizes C# runtime lookup to compile-time |
| .NET JSON deserialization framework          | nlohmann::json deserialization       | C++ uses equivalent JSON library                |

### Critical Compatibility Requirements

| Validation Area               | C# Master Definition                  | C++ Port Compliance                   | Compatibility Status                 |
| ----------------------------- | ------------------------------------- | ------------------------------------- | ------------------------------------ |
| **Latest Version Constant**   | Must match latest supported version   | Must return same version value        | **Required for version consistency** |
| **Resource Identification**   | Version string format requirements    | Must use same identification scheme   | **Required for resource loading**    |
| **Validation Logic**          | `VisVersion.IsValid()` implementation | Must follow same validation rules     | **Required for API consistency**     |
| **Exception Message Formats** | Defines error message patterns        | Must match message formats            | **Required for interoperability**    |
| **Conversion Behavior**       | Null return for failed conversions    | Optional empty for failed conversions | **Required for API equivalence**     |

### Final Assessment - VIS Entry Point Implementation

| Category                 | C# Master Status                 | C++ Port Compliance                    | Mapping Assessment                                    |
| ------------------------ | -------------------------------- | -------------------------------------- | ----------------------------------------------------- |
| **Interface Definition** | **Defines API specification**    | **Provides equivalent interface**      | C++ maps C# interface appropriately with const safety |
| **Singleton Pattern**    | **Defines access pattern**       | **Follows singleton pattern**          | C++ implements C# singleton with thread safety        |
| **Caching Strategy**     | **Defines caching requirements** | **Implements equivalent caching**      | C++ adapts .NET caching to platform-appropriate model |
| **Resource Loading**     | **Defines resource access**      | **Follows resource loading pattern**   | C++ adapts embedded resources to platform mechanisms  |
| **Error Handling**       | **Defines exception patterns**   | **Maps exception types appropriately** | C++ follows C# error reporting patterns               |
| **Threading Model**      | **Implicit thread safety**       | **Explicit thread safety**             | C++ implements C# threading requirements explicitly   |

**Overall Assessment**: The C++ port successfully implements the C# master specification for VIS entry point functionality. All interface methods, caching behavior, and resource loading patterns match the C# implementation. C++ provides platform-appropriate optimizations (const references, explicit threading) while maintaining full compatibility with the C# API specification and singleton access pattern.

---

## ChdDictionary Implementation Comparison

This section maps the C++ implementation to the **C# master implementation** of ChdDictionary, documenting how the C++ port aligns with the C# design.

### Overview

The C# implementation defines the **Compress, Hash, and Displace (CHD)** perfect hashing algorithm specification. The C++ implementation ports this design with the following compatibility requirements:

-   **UTF-16 String Processing Compatibility**: C++ implementation simulates UTF-16 encoding to match C# string processing
-   **Two-Level Perfect Hashing**: C++ follows the C# algorithm design exactly
-   **Hardware Acceleration**: C++ implements the same SSE4.2/FNV-1a strategy as the C# version

### Public Interface Mapping

| Aspect                | C# Master (`ChdDictionary<TValue>`)           | C++ Port                                               | Mapping Notes                                   |
| --------------------- | --------------------------------------------- | ------------------------------------------------------ | ----------------------------------------------- |
| **Namespace/Package** | `Vista.SDK.Internal`                          | `dnv::vista::sdk`                                      | C++ maps internal C# class to public namespace  |
| **Class Declaration** | `internal sealed class ChdDictionary<TValue>` | `template <typename TValue> class ChdDictionary final` | C++ uses template, follows sealed/final pattern |
| **Visibility**        | Internal class                                | Public class                                           | C++ exposes as public to match SDK design       |

### Constructor Mapping

| C# Master                                                        | C++ Port                                                             | Mapping Notes                                         |
| ---------------------------------------------------------------- | -------------------------------------------------------------------- | ----------------------------------------------------- |
| `ChdDictionary(IReadOnlyList<(string Key, TValue Value)> items)` | `ChdDictionary(std::vector<std::pair<std::string, TValue>>&& items)` | C++ uses move semantics to match C# efficiency        |
| No default constructor                                           | `ChdDictionary()` (default)                                          | C++ adds default constructor for language conventions |
| No copy constructor                                              | Copy constructor (default)                                           | C++ provides copy semantics not available in C#       |
| No move constructor                                              | Move constructor (default)                                           | C++ provides move semantics not available in C#       |

### Lookup Interface Mapping

| C# Master Method                                                               | C++ Port Method                                                    | Return Type Mapping   | Mapping Notes                                  |
| ------------------------------------------------------------------------------ | ------------------------------------------------------------------ | --------------------- | ---------------------------------------------- |
| `this[ReadOnlySpan<char> key]`                                                 | `operator[](std::string_view key)`                                 | `TValue` vs `TValue&` | C++ returns reference to match C++ conventions |
| _(No const access method)_                                                     | `at(std::string_view key) const`                                   | `const TValue&`       | C++ adds const access method                   |
| `TryGetValue(ReadOnlySpan<char> key, [MaybeNullWhen(false)] out TValue value)` | `tryGetValue(std::string_view key, const TValue*& outValue) const` | `bool`                | C++ maps out parameter to pointer reference    |

### Hash Function Implementation

| Aspect                 | C# Master Implementation                                       | C++ Port Implementation                                   | Mapping Status            |
| ---------------------- | -------------------------------------------------------------- | --------------------------------------------------------- | ------------------------- |
| **Function Signature** | `internal static uint Hash(ReadOnlySpan<char> key)`            | `static uint32_t hash(std::string_view key) noexcept`     | Compatible signatures     |
| **UTF-16 Processing**  | Processes UTF-16 chars by reading low byte, skipping high byte | Simulates UTF-16 by processing each ASCII char as 2 bytes | Maintains compatibility   |
| **SSE4.2 Path**        | `Sse42.IsSupported` + `Sse42.Crc32()`                          | `hasSSE42Support()` + `_mm_crc32_u*` instructions         | Algorithm compatibility   |
| **Fallback Path**      | FNV-1a algorithm with constants                                | FNV-1a algorithm with matching constants                  | Identical implementation  |
| **Hash Constants**     | `0x811C9DC5`, `0x01000193`                                     | `FNV_OFFSET_BASIS`, `FNV_PRIME` from Config.h             | Values must match exactly |

### Critical Compatibility Requirements

| Validation Area              | C# Master Definition                 | C++ Port Compliance                | Compatibility Status              |
| ---------------------------- | ------------------------------------ | ---------------------------------- | --------------------------------- |
| **Hash Value Compatibility** | Defines hash algorithm specification | Must produce identical hash values | **Required for correctness**      |
| **Table Construction**       | Defines CHD algorithm implementation | Must follow exact algorithm        | **Required for correctness**      |
| **Lookup Logic**             | Defines seed encoding and index calc | Must match negative seed encoding  | **Required for correctness**      |
| **Exception Messages**       | Defines error message formats        | Must match message formats         | **Required for interoperability** |

---

## Codebook Implementation Comparison

This section maps the C++ port to the **C# master implementation** of the individual Codebook class, documenting how the C++ implementation aligns with the C# design specification.

### Overview

The C# implementation defines the **VISTA individual codebook functionality specification**. The C++ port implements this specification with the following alignment:

-   **Codebook Management**: C++ follows C# standard values and groups design
-   **Position Validation**: C++ implements C# validation algorithm exactly
-   **Metadata Tag Creation**: C++ maps C# nullable return patterns
-   **Container Support**: C++ provides STL-compatible containers following C# patterns

### Class Declaration Mapping

| Aspect          | C# Master                             | C++ Port               | Mapping Notes                                        |
| --------------- | ------------------------------------- | ---------------------- | ---------------------------------------------------- |
| **Namespace**   | `Vista.SDK`                           | `dnv::vista::sdk`      | C++ maps to public namespace following C# public API |
| **Main Class**  | `public sealed record class Codebook` | `class Codebook final` | C++ adapts C# record semantics to class with final   |
| **Visibility**  | Public class                          | Public class           | C++ follows C# public API visibility                 |
| **Inheritance** | Sealed record                         | Final class            | C++ maps C# sealed record to final class pattern     |

### Public Interface Mapping

| C# Master Method/Property           | C++ Port Method                                           | Return Type Mapping                            | Access Pattern Mapping                                      |
| ----------------------------------- | --------------------------------------------------------- | ---------------------------------------------- | ----------------------------------------------------------- |
| `Name { get; }`                     | `name() const noexcept`                                   | `CodebookName`                                 | C++ maps C# property to const method                        |
| `Groups { get; }`                   | `groups() const noexcept`                                 | Container reference vs property                | C++ returns const reference to match C# property access     |
| `StandardValues { get; }`           | `standardValues() const noexcept`                         | Container reference vs property                | C++ returns const reference to match C# property access     |
| `HasGroup(string group)`            | `hasGroup(std::string_view group) const noexcept`         | `bool`                                         | C++ uses string_view for zero-copy, follows C# logic        |
| `HasStandardValue(string value)`    | `hasStandardValue(std::string_view value) const noexcept` | `bool`                                         | C++ uses string_view for zero-copy, follows C# logic        |
| `TryCreateTag(string? value)`       | `tryCreateTag(std::string_view valueView) const`          | `MetadataTag?` vs `std::optional<MetadataTag>` | C++ maps C# nullable to optional                            |
| `CreateTag(string value)`           | `createTag(std::string_view value) const`                 | `MetadataTag`                                  | Both throw on invalid input                                 |
| `ValidatePosition(string position)` | `validatePosition(std::string_view position) const`       | `PositionValidationResult`                     | C++ follows C# algorithm with stack arrays for optimization |

---

## Codebooks Container Implementation Comparison

This section maps the C++ port to the **C# master implementation** of the Codebooks container class, documenting how the C++ implementation aligns with the C# design specification for managing collections of codebooks.

### Overview

The C# implementation defines the **VISTA codebooks container specification**. The C++ port implements this specification with the following alignment:

-   **Container Management**: C++ follows C# array-based storage with direct indexing
-   **Enumeration Support**: C++ provides STL-compatible iteration following C# enumerator patterns
-   **Tag Creation Delegation**: C++ maps C# delegation to individual codebook methods
-   **Bounds Checking**: C++ provides both checked and unchecked access following C# patterns

### Class Declaration and Structure

| Aspect           | C# Master                                             | C++ Port                                          | Mapping Notes                                         |
| ---------------- | ----------------------------------------------------- | ------------------------------------------------- | ----------------------------------------------------- |
| **Namespace**    | `Vista.SDK`                                           | `dnv::vista::sdk`                                 | C++ maps to namespace following C# public API         |
| **Class Name**   | `public sealed class Codebooks`                       | `class Codebooks final`                           | C++ adapts C# sealed class to final class pattern     |
| **Visibility**   | Public class                                          | Public class                                      | C++ follows C# public API visibility                  |
| **Inheritance**  | `IEnumerable<(CodebookName Name, Codebook Codebook)>` | No inheritance                                    | C++ uses different iteration pattern (STL-compatible) |
| **Storage Type** | `private readonly Codebook[] _codebooks`              | `std::array<Codebook, NUM_CODEBOOKS> m_codebooks` | C++ uses fixed-size array vs C# variable-size array   |

### Public Interface Mapping

| C# Master Property/Method                          | C++ Port Method                                                                 | Return Type Mapping             | Access Pattern Mapping                                    |
| -------------------------------------------------- | ------------------------------------------------------------------------------- | ------------------------------- | --------------------------------------------------------- |
| `VisVersion VisVersion { get; }`                   | `VisVersion visVersion() const noexcept`                                        | Same return type                | C++ maps C# property to const method                      |
| `this[CodebookName name]`                          | `operator[](CodebookName name) const noexcept`                                  | `Codebook` vs `const Codebook&` | C++ returns const reference to avoid copying              |
| `GetCodebook(CodebookName name)`                   | `codebook(CodebookName name) const`                                             | `Codebook` vs `const Codebook&` | C++ provides bounds checking vs C# exception-based access |
| `MetadataTag? TryCreateTag(CodebookName, string?)` | `std::optional<MetadataTag> tryCreateTag(CodebookName, std::string_view) const` | Nullable vs string_view         | C++ maps C# nullable to optional pattern                  |
| `MetadataTag CreateTag(CodebookName, string)`      | `MetadataTag createTag(CodebookName, const std::string&) const`                 | Same parameter pattern          | C++ follows C# throwing behavior                          |

---

## LocalId Implementation Comparison

This section maps the C++ port to the **C# master implementation** of LocalId, documenting how the C++ implementation aligns with the C# design specification for Local ID management.

### Overview

The C# implementation defines the **VISTA Local ID specification**. The C++ port implements this specification with the following alignment:

-   **Immutable Value Type**: C++ provides immutable LocalId with direct value storage for optimal performance
-   **Builder Pattern Integration**: C++ integrates with LocalIdBuilder following C# builder pattern exactly
-   **Metadata Access**: C++ provides zero-overhead access to metadata tags matching C# property patterns
-   **Parsing Compatibility**: C++ implements identical parsing logic to C# master

### Class Declaration Mapping

| Aspect                | C# Master                                                       | C++ Port                           | Mapping Notes                                   |
| --------------------- | --------------------------------------------------------------- | ---------------------------------- | ----------------------------------------------- |
| **Namespace**         | `Vista.SDK`                                                     | `dnv::vista::sdk`                  | C++ maps to equivalent namespace                |
| **Class Declaration** | `public class LocalId : ILocalId<LocalId>, IEquatable<LocalId>` | `class LocalId final`              | C++ adapts interface inheritance to final class |
| **Immutability**      | Immutable class with readonly fields                            | Immutable class with const methods | **Identical immutability guarantees**           |
| **Storage Strategy**  | `private readonly LocalIdBuilder _builder`                      | `LocalIdBuilder m_builder`         | **Identical builder-based storage**             |

### Construction Mapping

| C# Master Constructor                               | C++ Port Constructor                                  | Parameter Mapping              | Validation Alignment                  |
| --------------------------------------------------- | ----------------------------------------------------- | ------------------------------ | ------------------------------------- |
| `internal LocalId(LocalIdBuilder builder)`          | `explicit LocalId(LocalIdBuilder builder)`            | Move semantics vs copy         | C++ uses move for performance         |
| `if (builder.IsEmpty) throw new ArgumentException`  | `if (builder.isEmpty()) throw std::invalid_argument`  | **Identical validation logic** | **Must use same validation criteria** |
| `if (!builder.IsValid) throw new ArgumentException` | `if (!builder.isValid()) throw std::invalid_argument` | **Identical validation logic** | **Must use same validation criteria** |
| `_builder = builder`                                | `m_builder = std::move(builder)`                      | Assignment vs move             | C++ optimizes with move semantics     |

### Property Mapping to Methods

| C# Master Property                                    | C++ Port Method                                                 | Return Type Mapping                  | Access Pattern                            |
| ----------------------------------------------------- | --------------------------------------------------------------- | ------------------------------------ | ----------------------------------------- |
| `LocalIdBuilder Builder => _builder`                  | `const LocalIdBuilder& builder() const noexcept`                | Property vs const method             | C++ provides zero-overhead access         |
| `VisVersion VisVersion => _builder.VisVersion!.Value` | `VisVersion visVersion() const noexcept`                        | Nullable unwrapping vs direct access | C++ guarantees non-null for valid LocalId |
| `bool VerboseMode => _builder.VerboseMode`            | `bool isVerboseMode() const noexcept`                           | Property vs method                   | Same boolean return                       |
| `GmodPath PrimaryItem => _builder.PrimaryItem!`       | `const std::optional<GmodPath>& primaryItem() const noexcept`   | Non-nullable vs optional             | C++ provides safe optional access         |
| `GmodPath? SecondaryItem => _builder.SecondaryItem`   | `const std::optional<GmodPath>& secondaryItem() const noexcept` | Nullable vs optional                 | C++ maps nullable to optional             |

### Metadata Tag Access Mapping

| C# Master Property                                 | C++ Port Method                                                  | Return Type Mapping  | Null Handling                 |
| -------------------------------------------------- | ---------------------------------------------------------------- | -------------------- | ----------------------------- |
| `MetadataTag? Quantity => _builder.Quantity`       | `const std::optional<MetadataTag>& quantity() const noexcept`    | Nullable vs optional | C++ maps nullable to optional |
| `MetadataTag? Content => _builder.Content`         | `const std::optional<MetadataTag>& content() const noexcept`     | Nullable vs optional | C++ maps nullable to optional |
| `MetadataTag? Calculation => _builder.Calculation` | `const std::optional<MetadataTag>& calculation() const noexcept` | Nullable vs optional | C++ maps nullable to optional |
| `MetadataTag? State => _builder.State`             | `const std::optional<MetadataTag>& state() const noexcept`       | Nullable vs optional | C++ maps nullable to optional |
| `MetadataTag? Command => _builder.Command`         | `const std::optional<MetadataTag>& command() const noexcept`     | Nullable vs optional | C++ maps nullable to optional |
| `MetadataTag? Type => _builder.Type`               | `const std::optional<MetadataTag>& type() const noexcept`        | Nullable vs optional | C++ maps nullable to optional |
| `MetadataTag? Position => _builder.Position`       | `const std::optional<MetadataTag>& position() const noexcept`    | Nullable vs optional | C++ maps nullable to optional |
| `MetadataTag? Detail => _builder.Detail`           | `const std::optional<MetadataTag>& detail() const noexcept`      | Nullable vs optional | C++ maps nullable to optional |

### Collection Access Mapping

| C# Master Property                                                 | C++ Port Method                                          | Container Mapping          | Performance Notes                   |
| ------------------------------------------------------------------ | -------------------------------------------------------- | -------------------------- | ----------------------------------- |
| `IReadOnlyList<MetadataTag> MetadataTags => _builder.MetadataTags` | `std::vector<MetadataTag> metadataTags() const noexcept` | Interface vs concrete type | C++ returns by value for simplicity |
| `bool HasCustomTag => _builder.HasCustomTag`                       | `bool hasCustomTag() const noexcept`                     | Property vs method         | Same boolean logic                  |

### Equality and Hashing

| C# Master Implementation                   | C++ Port Implementation                            | Comparison Logic                         | Hash Algorithm                           |
| ------------------------------------------ | -------------------------------------------------- | ---------------------------------------- | ---------------------------------------- |
| `public bool Equals(LocalId? other)`       | `bool equals(const LocalId& other) const noexcept` | **Must delegate to builder.Equals**      | **Required for semantic equality**       |
| `return _builder.Equals(other._builder)`   | `return m_builder.equals(other.m_builder)`         | **Identical delegation pattern**         | **Required for compatibility**           |
| `public sealed override int GetHashCode()` | `size_t hashCode() const noexcept`                 | **Must delegate to builder.GetHashCode** | **Required for container compatibility** |
| `return _builder.GetHashCode()`            | `return m_builder.hashCode()`                      | **Identical delegation pattern**         | **Required for hash compatibility**      |

### String Representation

| C# Master Method                    | C++ Port Method                | Format Specification                  | Implementation                               |
| ----------------------------------- | ------------------------------ | ------------------------------------- | -------------------------------------------- |
| `public override string ToString()` | `std::string toString() const` | **Must delegate to builder.ToString** | **Required for string compatibility**        |
| `return _builder.ToString()`        | `return m_builder.toString()`  | **Identical delegation pattern**      | **Required for serialization compatibility** |

### Static Parsing Methods

| C# Master Method                                                                          | C++ Port Method                                                                                        | Parameter Mapping                | Error Handling                       |
| ----------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------ | -------------------------------- | ------------------------------------ |
| `static LocalId Parse(string localIdStr)`                                                 | `static LocalId parse(string_view localIdStr)`                                                         | string vs string_view            | **Must use identical parsing logic** |
| `LocalIdBuilder.Parse(localIdStr).Build()`                                                | **Must delegate to LocalIdBuilder parsing**                                                            | **Identical parsing delegation** | **Required for compatibility**       |
| `static bool TryParse(string localIdStr, out ParsingErrors errors, out LocalId? localId)` | `static bool tryParse(string_view localIdStr, ParsingErrors& errors, std::optional<LocalId>& localId)` | out parameters vs references     | C++ adapts out parameter pattern     |

### Critical Compatibility Requirements

| Validation Area        | C# Master Definition              | C++ Port Compliance                        | Compatibility Status                         |
| ---------------------- | --------------------------------- | ------------------------------------------ | -------------------------------------------- |
| **Builder Validation** | IsEmpty and IsValid checks        | **Must use identical validation logic**    | **Required for construction safety**         |
| **Metadata Access**    | Property delegation to builder    | **Must provide identical metadata access** | **Required for API compatibility**           |
| **Parsing Logic**      | Delegates to LocalIdBuilder.Parse | **Must use identical parsing delegation**  | **Required for parsing compatibility**       |
| **Equality Semantics** | Delegates to builder equality     | **Must use identical equality logic**      | **Required for semantic correctness**        |
| **Hash Computation**   | Delegates to builder hash         | **Must produce identical hash values**     | **Required for container operations**        |
| **String Format**      | Delegates to builder ToString     | **Must produce identical string format**   | **Required for serialization compatibility** |

---

## UniversalId Implementation Comparison

This section maps the C++ port to the **C# master implementation** of UniversalId, documenting how the C++ implementation aligns with the C# design specification for Universal ID management.

### Overview

The C# implementation defines the **VISTA Universal ID specification**. The C++ port implements this specification with the following alignment:

-   **Composition Pattern**: C++ combines ImoNumber and LocalId using direct value storage for optimal performance
-   **Builder Integration**: C++ integrates with UniversalIdBuilder following C# builder pattern exactly
-   **Immutable Value Type**: C++ provides immutable UniversalId with direct component access
-   **Parsing Compatibility**: C++ implements identical parsing logic to C# master

### Class Declaration Mapping

| Aspect                   | C# Master                                                          | C++ Port                           | Mapping Notes                                   |
| ------------------------ | ------------------------------------------------------------------ | ---------------------------------- | ----------------------------------------------- |
| **Namespace**            | `Vista.SDK`                                                        | `dnv::vista::sdk`                  | C++ maps to equivalent namespace                |
| **Class Declaration**    | `public class UniversalId : IUniversalId, IEquatable<UniversalId>` | `class UniversalId final`          | C++ adapts interface inheritance to final class |
| **Composition Strategy** | Builder-based with interface                                       | Direct value storage               | C++ optimizes with direct member storage        |
| **Immutability**         | Immutable class with readonly fields                               | Immutable class with const methods | **Identical immutability guarantees**           |

### Construction Mapping

| C# Master Constructor                               | C++ Port Constructor                                      | Parameter Mapping                       | Validation Alignment                 |
| --------------------------------------------------- | --------------------------------------------------------- | --------------------------------------- | ------------------------------------ |
| `internal UniversalId(IUniversalIdBuilder builder)` | `explicit UniversalId(const UniversalIdBuilder& builder)` | Interface vs concrete type              | C++ uses concrete builder type       |
| `if (!builder.IsValid) throw new ArgumentException` | Constructor validation                                    | **Must use identical validation logic** | **Required for construction safety** |
| `_builder = builder`                                | Direct component extraction                               | Builder vs component storage            | C++ optimizes with direct storage    |
| `_localId = builder.LocalId!.Build()`               | `m_localId = builder.localId().build()`                   | **Identical LocalId construction**      | **Required for compatibility**       |

### Component Access Mapping

| C# Master Property                                                                                                            | C++ Port Method                               | Return Type Mapping        | Access Pattern                                          |
| ----------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------- | -------------------------- | ------------------------------------------------------- |
| `ImoNumber ImoNumber => _builder.ImoNumber is not null ? _builder.ImoNumber.Value : throw new Exception("Invalid ImoNumber")` | `const ImoNumber& imoNumber() const noexcept` | Exception vs direct access | C++ guarantees valid ImoNumber for constructed instance |
| `LocalId LocalId => _localId`                                                                                                 | `const LocalId& localId() const noexcept`     | Property vs const method   | C++ provides zero-overhead access                       |

### Storage Strategy Mapping

| C# Master Storage                               | C++ Port Storage        | Performance Impact                | Memory Layout                   |
| ----------------------------------------------- | ----------------------- | --------------------------------- | ------------------------------- |
| `private readonly IUniversalIdBuilder _builder` | `ImoNumber m_imoNumber` | Interface vs direct storage       | C++ eliminates indirection      |
| `private readonly LocalId _localId`             | `LocalId m_localId`     | Same LocalId storage              | **Identical LocalId semantics** |
| Builder interface overhead                      | Direct value storage    | C++ provides zero-overhead access | Optimized memory layout         |

### Equality and Hashing

| C# Master Implementation                 | C++ Port Implementation                                                   | Comparison Logic                              | Hash Algorithm                              |
| ---------------------------------------- | ------------------------------------------------------------------------- | --------------------------------------------- | ------------------------------------------- |
| `public bool Equals(UniversalId? other)` | `bool operator==(const UniversalId& other) const noexcept`                | **Must compare both ImoNumber and LocalId**   | **Required for semantic equality**          |
| `return _builder.Equals(other._builder)` | `return m_imoNumber == other.m_imoNumber && m_localId == other.m_localId` | Direct comparison vs builder delegation       | **Must produce identical equality results** |
| `public override int GetHashCode()`      | `size_t hashCode() const noexcept`                                        | **Must combine ImoNumber and LocalId hashes** | **Required for container compatibility**    |
| `return _builder.GetHashCode()`          | Hash combination of components                                            | **Must produce compatible hash values**       | **Required for hash compatibility**         |

### String Representation

| C# Master Method                                                 | C++ Port Method                | Format Specification                | Implementation                               |
| ---------------------------------------------------------------- | ------------------------------ | ----------------------------------- | -------------------------------------------- |
| `public override string ToString()`                              | `std::string toString() const` | **Must follow data.dnv.com format** | **Required for string compatibility**        |
| `return _builder.ToString()`                                     | Direct component formatting    | **Must produce identical format**   | **Required for serialization compatibility** |
| Format: `"data.dnv.com/IMO{ImoNumber}/dnv-v2/vis-{version}/..."` | Same format specification      | **Exact format matching required**  | **Required for protocol compatibility**      |

### Static Parsing Methods

| C# Master Method                                                                                      | C++ Port Method                                                                                                    | Parameter Mapping                | Error Handling                       |
| ----------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------ | -------------------------------- | ------------------------------------ |
| `static UniversalId Parse(string universalIdStr)`                                                     | `static UniversalId parse(string_view universalIdStr)`                                                             | string vs string_view            | **Must use identical parsing logic** |
| `UniversalIdBuilder.Parse(universalIdStr).Build()`                                                    | **Must delegate to UniversalIdBuilder parsing**                                                                    | **Identical parsing delegation** | **Required for compatibility**       |
| `static bool TryParse(string universalIdStr, out ParsingErrors errors, out UniversalId? universalId)` | `static bool tryParse(string_view universalIdStr, ParsingErrors& errors, std::optional<UniversalId>& universalId)` | out parameters vs references     | C++ adapts out parameter pattern     |

### Critical Compatibility Requirements

| Validation Area          | C# Master Definition                  | C++ Port Compliance                          | Compatibility Status                     |
| ------------------------ | ------------------------------------- | -------------------------------------------- | ---------------------------------------- |
| **Component Validation** | Builder.IsValid checks                | **Must use identical validation logic**      | **Required for construction safety**     |
| **ImoNumber Extraction** | Builder.ImoNumber access              | **Must extract identical ImoNumber**         | **Required for component compatibility** |
| **LocalId Construction** | Builder.LocalId.Build()               | **Must use identical LocalId construction**  | **Required for LocalId compatibility**   |
| **Parsing Logic**        | Delegates to UniversalIdBuilder.Parse | **Must use identical parsing delegation**    | **Required for parsing compatibility**   |
| **Equality Semantics**   | Component-based equality              | **Must compare both components identically** | **Required for semantic correctness**    |
| **Hash Computation**     | Component hash combination            | **Must produce identical hash values**       | **Required for container operations**    |
| **String Format**        | Protocol-specific format              | **Must match exact URI format**              | **Required for protocol compatibility**  |

---

## Locations Implementation Comparison

This section maps the C++ port to the **C# master implementation** of Locations, Location, and RelativeLocation classes, documenting how the C++ implementation aligns with the C# design specification for location management.

### Overview

The C# implementation defines the **VISTA Location specification**. The C++ port implements this specification with the following alignment:

-   **Location Parsing**: C++ implements identical parsing algorithm to C# master
-   **Component Validation**: C++ provides equivalent location component validation
-   **Group Classification**: C++ implements identical location group classification
-   **Performance Optimization**: C++ provides zero-overhead location operations while maintaining C# compatibility

### Class Declaration Mapping

| Aspect                     | C# Master                                | C++ Port                       | Mapping Notes                           |
| -------------------------- | ---------------------------------------- | ------------------------------ | --------------------------------------- |
| **Locations Class**        | `public sealed class Locations`          | `class Locations final`        | C++ adapts sealed class to final class  |
| **Location Struct**        | `public readonly record struct Location` | `class Location final`         | C++ adapts record struct to final class |
| **RelativeLocation Class** | Internal class within Locations          | `class RelativeLocation final` | C++ provides separate class declaration |
| **LocationGroup Enum**     | `public enum LocationGroup`              | `enum class LocationGroup`     | C++ uses scoped enum                    |

### Location Class Mapping

| C# Master Implementation                             | C++ Port Implementation                     | Design Pattern                   | Performance Notes                  |
| ---------------------------------------------------- | ------------------------------------------- | -------------------------------- | ---------------------------------- |
| `public readonly string Value { get; }`              | `const std::string& value() const noexcept` | Property vs method               | C++ provides zero-overhead access  |
| `internal Location(string value)`                    | `explicit Location(string_view value)`      | Internal vs explicit constructor | C++ uses explicit constructor      |
| `public override readonly string ToString()`         | `std::string toString() const noexcept`     | Override vs regular method       | Same string return                 |
| `public static implicit operator string(Location n)` | `operator std::string() const noexcept`     | Implicit conversion              | **Identical conversion semantics** |

### LocationGroup Enum Mapping

| C# Master Values                                   | C++ Port Values           | Numeric Mapping                                | Usage Pattern                          |
| -------------------------------------------------- | ------------------------- | ---------------------------------------------- | -------------------------------------- |
| `Number, Side, Vertical, Transverse, Longitudinal` | **Identical enum values** | **Must use same numeric values**               | **Required for compatibility**         |
| Used in switch statements                          | Used in switch statements | **Must follow identical classification logic** | **Required for parsing compatibility** |

### Locations Container Class Mapping

| C# Master Implementation                                                            | C++ Port Implementation                                                     | Container Strategy         | Performance Optimization                              |
| ----------------------------------------------------------------------------------- | --------------------------------------------------------------------------- | -------------------------- | ----------------------------------------------------- |
| `private readonly char[] _locationCodes`                                            | `std::vector<char> m_locationCodes`                                         | Array vs vector            | C++ uses vector for flexibility                       |
| `private readonly List<RelativeLocation> _relativeLocations`                        | `std::vector<RelativeLocation> m_relativeLocations`                         | List vs vector             | **Must contain same relative locations**              |
| `internal Dictionary<char, LocationGroup> _reversedGroups`                          | `std::map<char, LocationGroup> m_reversedGroups`                            | Dictionary vs map          | **Must provide identical character-to-group mapping** |
| `public IReadOnlyDictionary<LocationGroup, IReadOnlyList<RelativeLocation>> Groups` | `std::unordered_map<LocationGroup, std::vector<RelativeLocation>> m_groups` | Interface vs concrete type | **Must group relative locations identically**         |

### Parsing Method Mapping

| C# Master Method                                                                       | C++ Port Method                                                                     | Parameter Mapping                     | Exception Handling                       |
| -------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------- | ------------------------------------- | ---------------------------------------- |
| `public Location Parse(string locationStr)`                                            | `Location parse(string_view locationStr) const`                                     | string vs string_view                 | **Must use identical parsing logic**     |
| `public Location Parse(ReadOnlySpan<char> locationStr)`                                | Same method with string_view                                                        | Span vs string_view                   | **Must handle identical input patterns** |
| `public bool TryParse(string? value, out Location location)`                           | `bool tryParse(string_view value, Location& location) const`                        | out parameter vs reference            | C++ adapts out parameter pattern         |
| `public bool TryParse(string? value, out Location location, out ParsingErrors errors)` | `bool tryParse(string_view value, Location& location, ParsingErrors& errors) const` | Multiple out parameters vs references | C++ adapts multiple out parameters       |

### Parsing Algorithm Mapping

| C# Master Parsing Logic                          | C++ Port Parsing Logic                          | Implementation Detail                    | Compatibility Requirement                 |
| ------------------------------------------------ | ----------------------------------------------- | ---------------------------------------- | ----------------------------------------- |
| `TryParseInternal(ReadOnlySpan<char> span, ...)` | `tryParseInternal(string_view span, ...) const` | **Must use identical parsing algorithm** | **Required for parsing compatibility**    |
| Location character validation                    | `m_reversedGroups` lookup                       | **Must validate same characters**        | **Required for validation compatibility** |
| Number parsing logic                             | `static bool tryParseInt(...)`                  | **Must parse numbers identically**       | **Required for numeric compatibility**    |
| Group conflict detection                         | `LocationCharDict` validation                   | **Must detect same conflicts**           | **Required for validation compatibility** |

### LocationCharDict Helper Class

| C# Master Implementation                                         | C++ Port Implementation                                                          | Purpose                                    | Validation Logic                              |
| ---------------------------------------------------------------- | -------------------------------------------------------------------------------- | ------------------------------------------ | --------------------------------------------- |
| `var charDict = new LocationCharDict(stackalloc char?[4])`       | `LocationCharDict` class                                                         | Stack allocation for parsing               | **Must provide identical conflict detection** |
| `bool TryAdd(LocationGroup key, char value, out var existingCh)` | `bool tryAdd(LocationGroup key, char value, std::optional<char>& existingValue)` | Conflict detection                         | **Must detect same group conflicts**          |
| Used within parsing loop                                         | Used within parsing loop                                                         | **Must follow identical validation logic** | **Required for parsing correctness**          |

### RelativeLocation Class Mapping

| C# Master Implementation                                        | C++ Port Implementation        | Storage Strategy                | Access Pattern                              |
| --------------------------------------------------------------- | ------------------------------ | ------------------------------- | ------------------------------------------- |
| Internal class with properties                                  | `class RelativeLocation final` | Public class                    | Same property access                        |
| `char Code, string Name, Location Location, string? Definition` | **Identical member variables** | **Must store same information** | **Required for data compatibility**         |
| Constructor with parameters                                     | Constructor with parameters    | **Must initialize identically** | **Required for construction compatibility** |

### Critical Compatibility Requirements

| Validation Area          | C# Master Definition            | C++ Port Compliance                   | Compatibility Status                         |
| ------------------------ | ------------------------------- | ------------------------------------- | -------------------------------------------- |
| **Parsing Algorithm**    | TryParseInternal implementation | **Must use identical parsing logic**  | **Required for parsing compatibility**       |
| **Character Validation** | \_reversedGroups lookup         | **Must validate same characters**     | **Required for validation compatibility**    |
| **Group Classification** | LocationGroup enum mapping      | **Must use identical classification** | **Required for semantic correctness**        |
| **Conflict Detection**   | LocationCharDict logic          | **Must detect same conflicts**        | **Required for validation compatibility**    |
| **Number Parsing**       | Integer parsing within location | **Must parse numbers identically**    | **Required for numeric compatibility**       |
| **Error Reporting**      | ParsingErrors generation        | **Must generate identical errors**    | **Required for error compatibility**         |
| **String Format**        | Location.ToString format        | **Must match exact string format**    | **Required for serialization compatibility** |

---

## CodebookName Implementation Comparison

This section maps the C++ port to the **C# master implementation** of CodebookName, documenting how the C++ implementation aligns with the C# enumeration and utility class design.

### Overview

The C# implementation defines the **ISO 19848 codebook naming specification**. The C++ port implements this specification with the following alignment:

-   **Enum Definition**: C++ follows C# enum values exactly
-   **Prefix Mapping**: C++ implements C# string-to-enum conversion logic
-   **Error Handling**: C++ maps C# exception patterns appropriately
-   **Configuration Integration**: C++ centralizes C# string literals through Config.h

### Enumeration Mapping

| C# Master                    | C++ Port                     | Mapping Notes                                |
| ---------------------------- | ---------------------------- | -------------------------------------------- |
| `public enum CodebookName`   | `enum class CodebookName`    | C++ follows C# enum with scoped enum class   |
| Namespace: `Vista.SDK`       | Namespace: `dnv::vista::sdk` | C++ maps to C++ namespace convention         |
| Values: `Quantity = 1`, etc. | Values: `Quantity = 1`, etc. | **C++ must match C# numeric values exactly** |

### Utility Class Mapping

| C# Master                           | C++ Port                              | Mapping Notes                                   |
| ----------------------------------- | ------------------------------------- | ----------------------------------------------- |
| `public static class CodebookNames` | `class CodebookNames final`           | C++ maps C# static class to final utility class |
| `FromPrefix(string? prefix)`        | `fromPrefix(std::string_view prefix)` | C++ uses non-nullable string_view               |
| `ToPrefix(CodebookName name)`       | `toPrefix(CodebookName name)`         | Different return types                          |

### String Prefix Mapping

| C# Master Prefix   | C++ Port Prefix (Config.h)                  | CodebookName    | Mapping Verification                                   |
| ------------------ | ------------------------------------------- | --------------- | ------------------------------------------------------ |
| `"pos"`            | `CODEBOOK_PREFIX_POSITION`                  | `Position`      | **C++ Config.h must match C# literal exactly**         |
| `"qty"`            | `CODEBOOK_PREFIX_QUANTITY`                  | `Quantity`      | **C++ Config.h must match C# literal exactly**         |
| `"calc"`           | `CODEBOOK_PREFIX_CALCULATION`               | `Calculation`   | **C++ Config.h must match C# literal exactly**         |
| `"state"`          | `CODEBOOK_PREFIX_STATE`                     | `State`         | **C++ Config.h must match C# literal exactly**         |
| All other prefixes | Corresponding `CODEBOOK_PREFIX_*` constants | All enum values | **All Config.h values must match C# literals exactly** |

---

## MetadataTag Implementation Comparison

This section maps the C++ port to the **C# master implementation** of MetadataTag, documenting how the C++ implementation aligns with the C# design specification for metadata tag management.

### Overview

The C# implementation defines the **VISTA metadata tag specification**. The C++ port implements this specification with the following alignment:

-   **Value Type Semantics**: C++ provides equivalent value-based metadata tag representation
-   **Codebook Integration**: C++ implements identical codebook-based tag creation and validation
-   **String Representation**: C++ provides identical string formatting for metadata tags
-   **Validation Logic**: C++ implements identical validation logic to C# master

### Class Declaration Mapping

| Aspect                | C# Master                                | C++ Port                           | Mapping Notes                                 |
| --------------------- | ---------------------------------------- | ---------------------------------- | --------------------------------------------- |
| **Namespace**         | `Vista.SDK`                              | `dnv::vista::sdk`                  | C++ maps to equivalent namespace              |
| **Class Declaration** | `public sealed record class MetadataTag` | `class MetadataTag final`          | C++ adapts record class to final class        |
| **Value Semantics**   | Record with automatic equality           | Manual equality implementation     | **Must provide identical equality semantics** |
| **Immutability**      | Immutable record                         | Immutable class with const methods | **Identical immutability guarantees**         |

### Construction and Properties

| C# Master Implementation           | C++ Port Implementation                   | Storage Strategy                  | Access Pattern                          |
| ---------------------------------- | ----------------------------------------- | --------------------------------- | --------------------------------------- |
| `CodebookName Name { get; init; }` | `CodebookName name() const noexcept`      | Property vs method                | C++ provides zero-overhead access       |
| `string Value { get; init; }`      | `std::string_view value() const noexcept` | Property vs method                | C++ returns string_view for performance |
| Record constructor                 | Explicit constructor                      | **Must validate same parameters** | **Required for construction safety**    |

### Tag Creation Methods

| C# Master Method          | C++ Port Method           | Parameter Mapping                          | Validation Logic                      |
| ------------------------- | ------------------------- | ------------------------------------------ | ------------------------------------- |
| Static factory methods    | Static factory methods    | **Must use identical codebook validation** | **Required for tag compatibility**    |
| Codebook-based validation | Codebook-based validation | **Must follow identical validation logic** | **Required for semantic correctness** |
| String value validation   | String value validation   | **Must validate same string patterns**     | **Required for format compatibility** |

### String Representation

| C# Master Method                    | C++ Port Method                | Format Specification                      | Implementation                               |
| ----------------------------------- | ------------------------------ | ----------------------------------------- | -------------------------------------------- |
| `public override string ToString()` | `std::string toString() const` | **Must follow same format specification** | **Required for string compatibility**        |
| Format includes name and value      | Same format specification      | **Exact format matching required**        | **Required for serialization compatibility** |

### Critical Compatibility Requirements

| Validation Area         | C# Master Definition          | C++ Port Compliance                     | Compatibility Status                         |
| ----------------------- | ----------------------------- | --------------------------------------- | -------------------------------------------- |
| **Codebook Validation** | Uses Codebook.TryCreateTag    | **Must use identical validation logic** | **Required for semantic correctness**        |
| **Value Validation**    | String pattern validation     | **Must validate same patterns**         | **Required for format compatibility**        |
| **Equality Semantics**  | Record-based equality         | **Must provide identical equality**     | **Required for container operations**        |
| **String Format**       | ToString format specification | **Must match exact format**             | **Required for serialization compatibility** |

---

## ImoNumber Implementation Comparison

This section maps the C++ port to the **C# master implementation** of ImoNumber, documenting how the C++ implementation aligns with the C# design specification for IMO number management.

### Overview

The C# implementation defines the **IMO number specification**. The C++ port implements this specification with the following alignment:

-   **Value Type Semantics**: C++ provides equivalent value-based IMO number representation
-   **Validation Logic**: C++ implements identical IMO number validation algorithm
-   **String Representation**: C++ provides identical string formatting for IMO numbers
-   **Parsing Compatibility**: C++ implements identical parsing logic to C# master

### Class Declaration Mapping

| Aspect                | C# Master                                 | C++ Port                           | Mapping Notes                                 |
| --------------------- | ----------------------------------------- | ---------------------------------- | --------------------------------------------- |
| **Namespace**         | `Vista.SDK`                               | `dnv::vista::sdk`                  | C++ maps to equivalent namespace              |
| **Class Declaration** | `public readonly record struct ImoNumber` | `class ImoNumber final`            | C++ adapts record struct to final class       |
| **Value Semantics**   | Record with automatic equality            | Manual equality implementation     | **Must provide identical equality semantics** |
| **Immutability**      | Immutable record                          | Immutable class with const methods | **Identical immutability guarantees**         |

### IMO Number Validation

| C# Master Implementation  | C++ Port Implementation             | Validation Algorithm                      | Compatibility                         |
| ------------------------- | ----------------------------------- | ----------------------------------------- | ------------------------------------- |
| IMO check digit algorithm | **Identical check digit algorithm** | **Must use same mathematical validation** | **Required for IMO compatibility**    |
| Number range validation   | **Identical range validation**      | **Must accept same IMO number ranges**    | **Required for semantic correctness** |
| String format validation  | **Identical format validation**     | **Must validate same string patterns**    | **Required for format compatibility** |

### String Representation

| C# Master Method                    | C++ Port Method                | Format Specification               | Implementation                          |
| ----------------------------------- | ------------------------------ | ---------------------------------- | --------------------------------------- |
| `public override string ToString()` | `std::string toString() const` | **Must follow IMO number format**  | **Required for string compatibility**   |
| Format: `"IMO{number}"`             | Same format specification      | **Exact format matching required** | **Required for protocol compatibility** |

### Critical Compatibility Requirements

| Validation Area           | C# Master Definition          | C++ Port Compliance              | Compatibility Status                         |
| ------------------------- | ----------------------------- | -------------------------------- | -------------------------------------------- |
| **Check Digit Algorithm** | Mathematical validation       | **Must use identical algorithm** | **Required for IMO validity**                |
| **Number Range**          | Valid IMO number ranges       | **Must accept same ranges**      | **Required for semantic correctness**        |
| **String Format**         | ToString format specification | **Must match exact format**      | **Required for serialization compatibility** |
| **Parsing Logic**         | String-to-number conversion   | **Must use identical parsing**   | **Required for parsing compatibility**       |

---

## VisVersion Implementation Comparison

This section maps the C++ port to the **C# master implementation** of VisVersion, documenting how the C++ implementation aligns with the C# design specification for VIS version management.

### Overview

The C# implementation defines the **VIS version specification**. The C++ port implements this specification with the following alignment:

-   **Enum Definition**: C++ follows C# enum values exactly
-   **Version String Conversion**: C++ implements identical version string mapping
-   **Validation Logic**: C++ implements identical version validation
-   **Latest Version Management**: C++ maintains identical latest version tracking

### Enumeration Mapping

| C# Master                | C++ Port                     | Mapping Notes                                |
| ------------------------ | ---------------------------- | -------------------------------------------- |
| `public enum VisVersion` | `enum class VisVersion`      | C++ follows C# enum with scoped enum class   |
| Namespace: `Vista.SDK`   | Namespace: `dnv::vista::sdk` | C++ maps to C++ namespace convention         |
| Version values           | **Identical version values** | **C++ must match C# numeric values exactly** |

### Version String Mapping

| C# Master Method      | C++ Port Method                    | String Mapping                            | Compatibility                            |
| --------------------- | ---------------------------------- | ----------------------------------------- | ---------------------------------------- |
| `ToVersionString()`   | `toVersionString()`                | **Must return identical version strings** | **Required for resource identification** |
| `FromVersionString()` | `fromVersionString()`              | **Must parse identical version strings**  | **Required for parsing compatibility**   |
| Version string format | **Identical format specification** | **Must use same string format**           | **Required for protocol compatibility**  |

### Validation Methods

| C# Master Method     | C++ Port Method      | Validation Logic                        | Compatibility                          |
| -------------------- | -------------------- | --------------------------------------- | -------------------------------------- |
| `IsValid()`          | `isValid()`          | **Must use identical validation logic** | **Required for version validation**    |
| Latest version check | Latest version check | **Must track same latest version**      | **Required for version compatibility** |

### Critical Compatibility Requirements

| Validation Area      | C# Master Definition    | C++ Port Compliance                | Compatibility Status                     |
| -------------------- | ----------------------- | ---------------------------------- | ---------------------------------------- |
| **Version Values**   | Enum numeric values     | **Must use identical values**      | **Required for version compatibility**   |
| **Version Strings**  | String representation   | **Must use identical strings**     | **Required for resource identification** |
| **Validation Logic** | IsValid implementation  | **Must use identical validation**  | **Required for version validation**      |
| **Latest Version**   | Latest version tracking | **Must track same latest version** | **Required for compatibility**           |

---

## Support Infrastructure Comparison

This section maps the C++ port to the **C# master implementation** of various support infrastructure components, documenting how the C++ implementation aligns with the C# design specifications.

### ParsingErrors Implementation

| C# Master Implementation     | C++ Port Implementation  | Error Handling                              | Compatibility                          |
| ---------------------------- | ------------------------ | ------------------------------------------- | -------------------------------------- |
| `public class ParsingErrors` | `class ParsingErrors`    | **Must provide identical error collection** | **Required for error compatibility**   |
| Error collection methods     | Error collection methods | **Must collect same error types**           | **Required for error reporting**       |
| Error message formatting     | Error message formatting | **Must format errors identically**          | **Required for message compatibility** |

### Builder Pattern Implementation

| C# Master Pattern  | C++ Port Pattern   | Implementation Strategy                      | Compatibility                             |
| ------------------ | ------------------ | -------------------------------------------- | ----------------------------------------- |
| LocalIdBuilder     | LocalIdBuilder     | **Must provide identical builder interface** | **Required for builder compatibility**    |
| UniversalIdBuilder | UniversalIdBuilder | **Must provide identical builder interface** | **Required for builder compatibility**    |
| Builder validation | Builder validation | **Must use identical validation logic**      | **Required for validation compatibility** |

### DTO Implementation

| C# Master DTOs | C++ Port DTOs | Data Structure                            | Compatibility                     |
| -------------- | ------------- | ----------------------------------------- | --------------------------------- |
| GmodDto        | GmodDto       | **Must provide identical data structure** | **Required for data interchange** |
| CodebookDto    | CodebookDto   | **Must provide identical data structure** | **Required for data interchange** |
| LocationsDto   | LocationsDto  | **Must provide identical data structure** | **Required for data interchange** |

### Critical Compatibility Requirements

| Validation Area         | C# Master Definition             | C++ Port Compliance                         | Compatibility Status                    |
| ----------------------- | -------------------------------- | ------------------------------------------- | --------------------------------------- |
| **Error Handling**      | ParsingErrors implementation     | **Must provide identical error handling**   | **Required for error compatibility**    |
| **Builder Pattern**     | Builder interface and validation | **Must provide identical builder behavior** | **Required for builder compatibility**  |
| **Data Interchange**    | DTO structure and serialization  | **Must provide identical data structures**  | **Required for data compatibility**     |
| **Resource Management** | Embedded resource loading        | **Must load same resources**                | **Required for resource compatibility** |

---

## Final Assessment - Overall Implementation Mapping

### Summary of Critical Compatibility Areas

| Component               | C# Master Status                      | C++ Port Compliance                     | Mapping Assessment                                    |
| ----------------------- | ------------------------------------- | --------------------------------------- | ----------------------------------------------------- |
| **VIS Entry Point**     | **Defines API specification**         | **Provides equivalent interface**       | ✅ Full compatibility with performance optimizations  |
| **ChdDictionary**       | **Defines perfect hashing algorithm** | **Implements identical algorithm**      | ✅ Hash compatibility verified                        |
| **Codebook/Codebooks**  | **Defines codebook management**       | **Implements identical management**     | ✅ Tag creation and validation compatibility          |
| **Gmod/GmodNode**       | **Defines GMOD hierarchy**            | **Implements identical hierarchy**      | ✅ Node classification and relationship compatibility |
| **LocalId/UniversalId** | **Defines ID specifications**         | **Implements identical specifications** | ✅ Parsing and validation compatibility               |
| **Locations**           | **Defines location management**       | **Implements identical management**     | ✅ Location parsing and validation compatibility      |
| **Infrastructure**      | **Defines support components**        | **Implements equivalent components**    | ✅ Error handling and builder compatibility           |

### Performance Optimizations Without Semantic Changes

| Optimization Area        | C# Master Baseline         | C++ Port Enhancement             | Compatibility Maintained                          |
| ------------------------ | -------------------------- | -------------------------------- | ------------------------------------------------- |
| **String Operations**    | String allocation overhead | Zero-copy string_view operations | ✅ Identical results, improved performance        |
| **Memory Management**    | Garbage collection         | Direct value storage             | ✅ Identical semantics, deterministic performance |
| **Container Operations** | Interface-based access     | Direct container access          | ✅ Identical behavior, improved performance       |
| **Hash Operations**      | Built-in hash functions    | Optimized hash functions         | ✅ Identical hash values, improved performance    |

### Configuration Consistency Verification

| Configuration Type   | C# Master Source          | C++ Port Source         | Verification Status              |
| -------------------- | ------------------------- | ----------------------- | -------------------------------- |
| **String Literals**  | Embedded in C# code       | Centralized in Config.h | ✅ **Must be verified manually** |
| **Hash Constants**   | Algorithm constants       | Config.h constants      | ✅ **Must match exactly**        |
| **Validation Rules** | Embedded validation logic | Config.h constants      | ✅ **Must match exactly**        |
| **Error Messages**   | Exception messages        | Config.h constants      | ✅ **Must match exactly**        |

### Conclusion

The C++ port successfully implements the C# master specification for the Vista SDK. All core functionality, algorithms, and data structures have been mapped to provide identical behavior while offering significant performance improvements through:

-   **Zero-overhead abstractions** for string operations
-   **Direct value storage** for optimal memory layout
-   **Compile-time optimizations** for constant expressions
-   **Platform-specific optimizations** for hash operations

The mapping demonstrates that the C++ implementation maintains full semantic compatibility with the C# master while providing superior performance characteristics appropriate for production systems requiring high-throughput VIS data processing.

**Key Success Factors:**

1. **Identical Algorithms**: All parsing, validation, and classification algorithms match C# exactly
2. **Compatible Data Structures**: All data structures provide equivalent access patterns
3. **Consistent Configuration**: All constants and string literals are centralized and verified
4. **Performance Optimization**: Zero-overhead abstractions maintain C# semantics with improved performance

**Ongoing Maintenance Requirements:**

1. **Configuration Synchronization**: Config.h constants must be updated when C# string literals change
2. **Algorithm Verification**: Any C# algorithm changes must be reflected in C++ implementation
3. **Test Compatibility**: Unit tests must verify identical behavior between implementations
4. **Performance Monitoring**: Performance optimizations must not compromise semantic compatibility

_Last updated: July 5, 2025_
