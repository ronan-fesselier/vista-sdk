#include "dnv/vista/sdk/c/core/gmod_node_metadata.h"

#include "../cast_internal.h"
#include "../error_internal.h"

#include <iterator>

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::toMetadata;

const char* dnv_vista_sdk_gmod_node_metadata_category(const dnv_vista_sdk_gmod_node_metadata_t* metadata)
{
    if (metadata == nullptr)
    {
        c::setLastError("metadata must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toMetadata(metadata)->category().data();
}

const char* dnv_vista_sdk_gmod_node_metadata_type(const dnv_vista_sdk_gmod_node_metadata_t* metadata)
{
    if (metadata == nullptr)
    {
        c::setLastError("metadata must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toMetadata(metadata)->type().data();
}

const char* dnv_vista_sdk_gmod_node_metadata_full_type(const dnv_vista_sdk_gmod_node_metadata_t* metadata)
{
    if (metadata == nullptr)
    {
        c::setLastError("metadata must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toMetadata(metadata)->fullType().data();
}

const char* dnv_vista_sdk_gmod_node_metadata_name(const dnv_vista_sdk_gmod_node_metadata_t* metadata)
{
    if (metadata == nullptr)
    {
        c::setLastError("metadata must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toMetadata(metadata)->name().data();
}

const char* dnv_vista_sdk_gmod_node_metadata_common_name(const dnv_vista_sdk_gmod_node_metadata_t* metadata)
{
    if (metadata == nullptr)
    {
        c::setLastError("metadata must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& commonName = toMetadata(metadata)->commonName();
    return commonName.has_value() ? commonName->c_str() : nullptr;
}

const char* dnv_vista_sdk_gmod_node_metadata_definition(const dnv_vista_sdk_gmod_node_metadata_t* metadata)
{
    if (metadata == nullptr)
    {
        c::setLastError("metadata must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& definition = toMetadata(metadata)->definition();
    return definition.has_value() ? definition->c_str() : nullptr;
}

const char* dnv_vista_sdk_gmod_node_metadata_common_definition(const dnv_vista_sdk_gmod_node_metadata_t* metadata)
{
    if (metadata == nullptr)
    {
        c::setLastError("metadata must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& commonDefinition = toMetadata(metadata)->commonDefinition();
    return commonDefinition.has_value() ? commonDefinition->c_str() : nullptr;
}

int dnv_vista_sdk_gmod_node_metadata_install_substructure(
    const dnv_vista_sdk_gmod_node_metadata_t* metadata, int* outValue)
{
    if (metadata == nullptr || outValue == nullptr)
    {
        c::setLastError("metadata and outValue must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    const auto installSubstructure = toMetadata(metadata)->installSubstructure();
    if (!installSubstructure.has_value())
    {
        return 0;
    }

    *outValue = *installSubstructure ? 1 : 0;
    return 1;
}

size_t dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_count(const dnv_vista_sdk_gmod_node_metadata_t* metadata)
{
    if (metadata == nullptr)
    {
        c::setLastError("metadata must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toMetadata(metadata)->normalAssignmentNames().size();
}

const char* dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_key_at(
    const dnv_vista_sdk_gmod_node_metadata_t* metadata, size_t index)
{
    if (metadata == nullptr)
    {
        c::setLastError("metadata must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& names = toMetadata(metadata)->normalAssignmentNames();
    if (index >= names.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    auto it = names.begin();
    std::advance(it, static_cast<std::ptrdiff_t>(index));
    return it->first.c_str();
}

const char* dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_value_at(
    const dnv_vista_sdk_gmod_node_metadata_t* metadata, size_t index)
{
    if (metadata == nullptr)
    {
        c::setLastError("metadata must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& names = toMetadata(metadata)->normalAssignmentNames();
    if (index >= names.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    auto it = names.begin();
    std::advance(it, static_cast<std::ptrdiff_t>(index));
    return it->second.c_str();
}
