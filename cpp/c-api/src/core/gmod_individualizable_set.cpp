#include "dnv/vista/sdk/c/core/gmod_individualizable_set.h"

#include "../cast_internal.h"
#include "../error_internal.h"

#include <vector>

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromGmodPathValue;
using dnv::vista::sdk::c::toGmodPath;
using dnv::vista::sdk::c::toIndividualizableSet;
using dnv::vista::sdk::c::toOwnedCString;

dnv_vista_sdk_gmod_individualizable_set_t* dnv_vista_sdk_gmod_individualizable_set_create(
    const int* nodeIndices, size_t nodeIndicesCount, const dnv_vista_sdk_gmod_path_t* sourcePath)
{
    if (nodeIndices == nullptr || sourcePath == nullptr)
    {
        c::setLastError("nodeIndices and sourcePath must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    std::vector<int> indices(nodeIndices, nodeIndices + nodeIndicesCount);

    return c::cApiTryCatch<dnv_vista_sdk_gmod_individualizable_set_t*>(
        [&]() -> dnv_vista_sdk_gmod_individualizable_set_t* {
            return reinterpret_cast<dnv_vista_sdk_gmod_individualizable_set_t*>(
                new GmodIndividualizableSet{ indices, *toGmodPath(sourcePath) });
        });
}

void dnv_vista_sdk_gmod_individualizable_set_free(dnv_vista_sdk_gmod_individualizable_set_t* set)
{
    delete reinterpret_cast<GmodIndividualizableSet*>(set);
}

dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_gmod_individualizable_set_build(dnv_vista_sdk_gmod_individualizable_set_t* set)
{
    if (set == nullptr)
    {
        c::setLastError("set must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_gmod_path_t*>(
        [&]() -> dnv_vista_sdk_gmod_path_t* { return fromGmodPathValue(toIndividualizableSet(set)->build()); });
}

size_t dnv_vista_sdk_gmod_individualizable_set_node_count(const dnv_vista_sdk_gmod_individualizable_set_t* set)
{
    if (set == nullptr)
    {
        c::setLastError("set must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return c::cApiTryCatch<size_t>([&]() -> size_t { return toIndividualizableSet(set)->nodes().size(); });
}

dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_individualizable_set_node_at(
    const dnv_vista_sdk_gmod_individualizable_set_t* set, size_t index)
{
    if (set == nullptr)
    {
        c::setLastError("set must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_gmod_node_t*>([&]() -> dnv_vista_sdk_gmod_node_t* {
        auto nodes = toIndividualizableSet(set)->nodes();
        if (index >= nodes.size())
        {
            c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
            return nullptr;
        }

        return reinterpret_cast<dnv_vista_sdk_gmod_node_t*>(new GmodNode{ std::move(nodes[index]) });
    });
}

void dnv_vista_sdk_gmod_individualizable_set_node_free(dnv_vista_sdk_gmod_node_t* node)
{
    delete reinterpret_cast<GmodNode*>(node);
}

size_t dnv_vista_sdk_gmod_individualizable_set_index_count(const dnv_vista_sdk_gmod_individualizable_set_t* set)
{
    if (set == nullptr)
    {
        c::setLastError("set must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toIndividualizableSet(set)->nodeIndices().size();
}

int dnv_vista_sdk_gmod_individualizable_set_index_at(
    const dnv_vista_sdk_gmod_individualizable_set_t* set, size_t position, int* outIndex)
{
    if (set == nullptr || outIndex == nullptr)
    {
        c::setLastError("set and outIndex must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    const auto& indices = toIndividualizableSet(set)->nodeIndices();
    if (position >= indices.size())
    {
        c::setLastError("position out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return 0;
    }

    *outIndex = indices[position];
    return 1;
}

dnv_vista_sdk_location_t* dnv_vista_sdk_gmod_individualizable_set_location(
    const dnv_vista_sdk_gmod_individualizable_set_t* set)
{
    if (set == nullptr)
    {
        c::setLastError("set must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_location_t*>([&]() -> dnv_vista_sdk_location_t* {
        auto location = toIndividualizableSet(set)->location();
        if (!location.has_value())
        {
            return nullptr;
        }

        return reinterpret_cast<dnv_vista_sdk_location_t*>(new Location{ std::move(*location) });
    });
}

char* dnv_vista_sdk_gmod_individualizable_set_to_string(const dnv_vista_sdk_gmod_individualizable_set_t* set)
{
    if (set == nullptr)
    {
        c::setLastError("set must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<char*>([&]() -> char* { return toOwnedCString(toIndividualizableSet(set)->toString()); });
}
