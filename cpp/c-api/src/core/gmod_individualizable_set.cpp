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
        c::setLastErrorMessage("nodeIndices and sourcePath must not be null");
        return nullptr;
    }

    std::vector<int> indices(nodeIndices, nodeIndices + nodeIndicesCount);

    try
    {
        return reinterpret_cast<dnv_vista_sdk_gmod_individualizable_set_t*>(
            new GmodIndividualizableSet{ indices, *toGmodPath(sourcePath) });
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

void dnv_vista_sdk_gmod_individualizable_set_free(dnv_vista_sdk_gmod_individualizable_set_t* set)
{
    delete reinterpret_cast<GmodIndividualizableSet*>(set);
}

dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_gmod_individualizable_set_build(dnv_vista_sdk_gmod_individualizable_set_t* set)
{
    if (set == nullptr)
    {
        c::setLastErrorMessage("set must not be null");
        return nullptr;
    }

    try
    {
        return fromGmodPathValue(toIndividualizableSet(set)->build());
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

size_t dnv_vista_sdk_gmod_individualizable_set_node_count(const dnv_vista_sdk_gmod_individualizable_set_t* set)
{
    if (set == nullptr)
    {
        c::setLastErrorMessage("set must not be null");
        return 0;
    }

    try
    {
        return toIndividualizableSet(set)->nodes().size();
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return 0;
    }
}

dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_individualizable_set_node_at(
    const dnv_vista_sdk_gmod_individualizable_set_t* set, size_t index)
{
    if (set == nullptr)
    {
        c::setLastErrorMessage("set must not be null");
        return nullptr;
    }

    try
    {
        auto nodes = toIndividualizableSet(set)->nodes();
        if (index >= nodes.size())
        {
            c::setLastErrorMessage("index out of range");
            return nullptr;
        }

        return reinterpret_cast<dnv_vista_sdk_gmod_node_t*>(new GmodNode{ std::move(nodes[index]) });
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

void dnv_vista_sdk_gmod_individualizable_set_node_free(dnv_vista_sdk_gmod_node_t* node)
{
    delete reinterpret_cast<GmodNode*>(node);
}

size_t dnv_vista_sdk_gmod_individualizable_set_index_count(const dnv_vista_sdk_gmod_individualizable_set_t* set)
{
    if (set == nullptr)
    {
        c::setLastErrorMessage("set must not be null");
        return 0;
    }

    return toIndividualizableSet(set)->nodeIndices().size();
}

int dnv_vista_sdk_gmod_individualizable_set_index_at(
    const dnv_vista_sdk_gmod_individualizable_set_t* set, size_t position, int* outIndex)
{
    if (set == nullptr || outIndex == nullptr)
    {
        c::setLastErrorMessage("set and outIndex must not be null");
        return 0;
    }

    const auto& indices = toIndividualizableSet(set)->nodeIndices();
    if (position >= indices.size())
    {
        c::setLastErrorMessage("position out of range");
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
        c::setLastErrorMessage("set must not be null");
        return nullptr;
    }

    try
    {
        auto location = toIndividualizableSet(set)->location();
        if (!location.has_value())
        {
            return nullptr;
        }

        return reinterpret_cast<dnv_vista_sdk_location_t*>(new Location{ std::move(*location) });
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

char* dnv_vista_sdk_gmod_individualizable_set_to_string(const dnv_vista_sdk_gmod_individualizable_set_t* set)
{
    if (set == nullptr)
    {
        c::setLastErrorMessage("set must not be null");
        return nullptr;
    }

    try
    {
        return toOwnedCString(toIndividualizableSet(set)->toString());
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

void dnv_vista_sdk_gmod_individualizable_set_string_free(char* str)
{
    delete[] str;
}
