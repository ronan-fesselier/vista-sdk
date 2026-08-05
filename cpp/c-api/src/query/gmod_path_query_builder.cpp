#include "dnv/vista/sdk/c/query/gmod_path_query_builder.h"

#include "../cast_internal.h"
#include "../error_internal.h"

#include <memory>
#include <vector>

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromGmodPathQueryBuilderWrapper;
using dnv::vista::sdk::c::fromGmodPathQueryValue;
using dnv::vista::sdk::c::fromGmodPathRef;
using dnv::vista::sdk::c::GmodPathQueryBuilderWrapper;
using dnv::vista::sdk::c::toGmodNode;
using dnv::vista::sdk::c::toGmodPath;
using dnv::vista::sdk::c::toGmodPathQueryBuilderWrapper;
using dnv::vista::sdk::c::toLocation;

namespace
{
    std::vector<Location> toLocationVector(const dnv_vista_sdk_location_t* const* locations, size_t locationCount)
    {
        std::vector<Location> result;
        result.reserve(locationCount);
        for (size_t i = 0; i < locationCount; ++i)
        {
            result.push_back(*toLocation(locations[i]));
        }
        return result;
    }

    const GmodPathQueryBuilder::Path* asPath(const GmodPathQueryBuilderWrapper* w)
    {
        if (w->variant != GmodPathQueryBuilderWrapper::Variant::Path)
        {
            return nullptr;
        }
        return static_cast<const GmodPathQueryBuilder::Path*>(w->builder.get());
    }

    const GmodPathQueryBuilder::Nodes* asNodes(const GmodPathQueryBuilderWrapper* w)
    {
        if (w->variant != GmodPathQueryBuilderWrapper::Variant::Nodes)
        {
            return nullptr;
        }
        return static_cast<const GmodPathQueryBuilder::Nodes*>(w->builder.get());
    }
} // namespace

dnv_vista_sdk_gmod_path_query_builder_t* dnv_vista_sdk_gmod_path_query_builder_create(void)
{
    return fromGmodPathQueryBuilderWrapper(
        GmodPathQueryBuilderWrapper::Variant::Nodes,
        std::make_unique<GmodPathQueryBuilder::Nodes>(GmodPathQueryBuilder::create()));
}

dnv_vista_sdk_gmod_path_query_builder_t* dnv_vista_sdk_gmod_path_query_builder_from(
    const dnv_vista_sdk_gmod_path_t* path)
{
    if (path == nullptr)
    {
        c::setLastError("path must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromGmodPathQueryBuilderWrapper(
        GmodPathQueryBuilderWrapper::Variant::Path,
        std::make_unique<GmodPathQueryBuilder::Path>(GmodPathQueryBuilder::from(*toGmodPath(path))));
}

void dnv_vista_sdk_gmod_path_query_builder_free(dnv_vista_sdk_gmod_path_query_builder_t* builder)
{
    delete reinterpret_cast<GmodPathQueryBuilderWrapper*>(builder);
}

const dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_gmod_path_query_builder_path(
    const dnv_vista_sdk_gmod_path_query_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastError("builder must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto* path = asPath(toGmodPathQueryBuilderWrapper(builder));
    if (path == nullptr)
    {
        c::setLastError("builder is not a Path-variant handle", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromGmodPathRef(path->path());
}

dnv_vista_sdk_gmod_path_query_builder_t* dnv_vista_sdk_gmod_path_query_builder_path_with_node_all_locations(
    const dnv_vista_sdk_gmod_path_query_builder_t* builder, const char* code, int matchAllLocations)
{
    if (builder == nullptr || code == nullptr)
    {
        c::setLastError("builder and code must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto* path = asPath(toGmodPathQueryBuilderWrapper(builder));
    if (path == nullptr)
    {
        c::setLastError("builder is not a Path-variant handle", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const std::string codeStr{ code };
    return c::cApiTryCatch<dnv_vista_sdk_gmod_path_query_builder_t*>([&]() -> dnv_vista_sdk_gmod_path_query_builder_t* {
        return fromGmodPathQueryBuilderWrapper(
            GmodPathQueryBuilderWrapper::Variant::Path,
            std::make_unique<GmodPathQueryBuilder::Path>(path->withNode(
                [&codeStr](const std::unordered_map<std::string, const GmodNode*, StringHash, std::equal_to<>>& nodes)
                    -> const GmodNode* {
                    auto it = nodes.find(codeStr);
                    return it != nodes.end() ? it->second : nullptr;
                },
                matchAllLocations != 0)));
    });
}

dnv_vista_sdk_gmod_path_query_builder_t* dnv_vista_sdk_gmod_path_query_builder_path_with_node_locations(
    const dnv_vista_sdk_gmod_path_query_builder_t* builder,
    const char* code,
    const dnv_vista_sdk_location_t* const* locations,
    size_t locationCount)
{
    if (builder == nullptr || code == nullptr)
    {
        c::setLastError("builder and code must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto* path = asPath(toGmodPathQueryBuilderWrapper(builder));
    if (path == nullptr)
    {
        c::setLastError("builder is not a Path-variant handle", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const std::string codeStr{ code };
    const auto locs = toLocationVector(locations, locationCount);
    return c::cApiTryCatch<dnv_vista_sdk_gmod_path_query_builder_t*>([&]() -> dnv_vista_sdk_gmod_path_query_builder_t* {
        return fromGmodPathQueryBuilderWrapper(
            GmodPathQueryBuilderWrapper::Variant::Path,
            std::make_unique<GmodPathQueryBuilder::Path>(path->withNode(
                [&codeStr](const std::unordered_map<std::string, const GmodNode*, StringHash, std::equal_to<>>& nodes)
                    -> const GmodNode* {
                    auto it = nodes.find(codeStr);
                    return it != nodes.end() ? it->second : nullptr;
                },
                locs)));
    });
}

dnv_vista_sdk_gmod_path_query_builder_t* dnv_vista_sdk_gmod_path_query_builder_with_any_node_before(
    const dnv_vista_sdk_gmod_path_query_builder_t* builder, const char* code)
{
    if (builder == nullptr || code == nullptr)
    {
        c::setLastError("builder and code must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto* path = asPath(toGmodPathQueryBuilderWrapper(builder));
    if (path == nullptr)
    {
        c::setLastError("builder is not a Path-variant handle", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const std::string codeStr{ code };
    return c::cApiTryCatch<dnv_vista_sdk_gmod_path_query_builder_t*>([&]() -> dnv_vista_sdk_gmod_path_query_builder_t* {
        return fromGmodPathQueryBuilderWrapper(
            GmodPathQueryBuilderWrapper::Variant::Path,
            std::make_unique<GmodPathQueryBuilder::Path>(path->withAnyNodeBefore(
                [&codeStr](const std::unordered_map<std::string, const GmodNode*, StringHash, std::equal_to<>>& nodes)
                    -> const GmodNode* {
                    auto it = nodes.find(codeStr);
                    return it != nodes.end() ? it->second : nullptr;
                })));
    });
}

dnv_vista_sdk_gmod_path_query_builder_t* dnv_vista_sdk_gmod_path_query_builder_with_any_node_after(
    const dnv_vista_sdk_gmod_path_query_builder_t* builder, const char* code)
{
    if (builder == nullptr || code == nullptr)
    {
        c::setLastError("builder and code must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto* path = asPath(toGmodPathQueryBuilderWrapper(builder));
    if (path == nullptr)
    {
        c::setLastError("builder is not a Path-variant handle", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const std::string codeStr{ code };
    return c::cApiTryCatch<dnv_vista_sdk_gmod_path_query_builder_t*>([&]() -> dnv_vista_sdk_gmod_path_query_builder_t* {
        return fromGmodPathQueryBuilderWrapper(
            GmodPathQueryBuilderWrapper::Variant::Path,
            std::make_unique<GmodPathQueryBuilder::Path>(path->withAnyNodeAfter(
                [&codeStr](const std::unordered_map<std::string, const GmodNode*, StringHash, std::equal_to<>>& nodes)
                    -> const GmodNode* {
                    auto it = nodes.find(codeStr);
                    return it != nodes.end() ? it->second : nullptr;
                })));
    });
}

dnv_vista_sdk_gmod_path_query_builder_t* dnv_vista_sdk_gmod_path_query_builder_without_locations(
    const dnv_vista_sdk_gmod_path_query_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastError("builder must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto* path = asPath(toGmodPathQueryBuilderWrapper(builder));
    if (path == nullptr)
    {
        c::setLastError("builder is not a Path-variant handle", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromGmodPathQueryBuilderWrapper(
        GmodPathQueryBuilderWrapper::Variant::Path,
        std::make_unique<GmodPathQueryBuilder::Path>(path->withoutLocations()));
}

dnv_vista_sdk_gmod_path_query_builder_t* dnv_vista_sdk_gmod_path_query_builder_with_node_all_locations(
    const dnv_vista_sdk_gmod_path_query_builder_t* builder,
    const dnv_vista_sdk_gmod_node_t* node,
    int matchAllLocations)
{
    if (builder == nullptr || node == nullptr)
    {
        c::setLastError("builder and node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto* nodes = asNodes(toGmodPathQueryBuilderWrapper(builder));
    if (nodes == nullptr)
    {
        c::setLastError("builder is not a Nodes-variant handle", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromGmodPathQueryBuilderWrapper(
        GmodPathQueryBuilderWrapper::Variant::Nodes,
        std::make_unique<GmodPathQueryBuilder::Nodes>(nodes->withNode(*toGmodNode(node), matchAllLocations != 0)));
}

dnv_vista_sdk_gmod_path_query_builder_t* dnv_vista_sdk_gmod_path_query_builder_with_node_locations(
    const dnv_vista_sdk_gmod_path_query_builder_t* builder,
    const dnv_vista_sdk_gmod_node_t* node,
    const dnv_vista_sdk_location_t* const* locations,
    size_t locationCount)
{
    if (builder == nullptr || node == nullptr)
    {
        c::setLastError("builder and node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto* nodes = asNodes(toGmodPathQueryBuilderWrapper(builder));
    if (nodes == nullptr)
    {
        c::setLastError("builder is not a Nodes-variant handle", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto locs = toLocationVector(locations, locationCount);
    return fromGmodPathQueryBuilderWrapper(
        GmodPathQueryBuilderWrapper::Variant::Nodes,
        std::make_unique<GmodPathQueryBuilder::Nodes>(nodes->withNode(*toGmodNode(node), locs)));
}

dnv_vista_sdk_gmod_path_query_t* dnv_vista_sdk_gmod_path_query_builder_build(
    const dnv_vista_sdk_gmod_path_query_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastError("builder must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromGmodPathQueryValue(toGmodPathQueryBuilderWrapper(builder)->builder->build());
}
