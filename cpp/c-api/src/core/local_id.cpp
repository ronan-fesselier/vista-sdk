#include "dnv/vista/sdk/c/core/local_id.h"

#include "local_id_internal.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromGmodPathRef;
using dnv::vista::sdk::c::fromLocalId;
using dnv::vista::sdk::c::fromLocalIdBuilderRef;
using dnv::vista::sdk::c::fromTagRef;
using dnv::vista::sdk::c::selectMetadataTag;
using dnv::vista::sdk::c::toCodebookName;
using dnv::vista::sdk::c::toLocalId;
using dnv::vista::sdk::c::toOwnedCString;

const char* dnv_vista_sdk_local_id_naming_rule(void)
{
    return LocalId::namingRule().data();
}

void dnv_vista_sdk_local_id_free(dnv_vista_sdk_local_id_t* localId)
{
    delete reinterpret_cast<LocalId*>(localId);
}

dnv_vista_sdk_local_id_t* dnv_vista_sdk_local_id_from_string(const char* localIdStr)
{
    if (localIdStr == nullptr)
    {
        c::setLastError("localIdStr must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    auto localId = LocalId::fromString(localIdStr);
    if (!localId.has_value())
    {
        c::setLastError("invalid LocalId string", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
    }

    return fromLocalId(std::move(localId));
}

dnv_vista_sdk_local_id_t* dnv_vista_sdk_local_id_from_string_with_errors(
    const char* localIdStr, dnv_vista_sdk_parsing_errors_t** outErrors)
{
    if (localIdStr == nullptr || outErrors == nullptr)
    {
        c::setLastError("localIdStr and outErrors must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    ParsingErrors errors;
    auto localId = LocalId::fromString(localIdStr, errors);

    *outErrors = reinterpret_cast<dnv_vista_sdk_parsing_errors_t*>(new ParsingErrors{ std::move(errors) });

    if (!localId.has_value())
    {
        c::setLastError("invalid LocalId string", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
    }

    return fromLocalId(std::move(localId));
}

const char* dnv_vista_sdk_local_id_version(const dnv_vista_sdk_local_id_t* localId)
{
    if (localId == nullptr)
    {
        c::setLastError("localId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return VisVersions::toString(toLocalId(localId)->version()).data();
}

const dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_local_id_primary_item(const dnv_vista_sdk_local_id_t* localId)
{
    if (localId == nullptr)
    {
        c::setLastError("localId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromGmodPathRef(toLocalId(localId)->primaryItem());
}

const dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_local_id_secondary_item(const dnv_vista_sdk_local_id_t* localId)
{
    if (localId == nullptr)
    {
        c::setLastError("localId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& path = toLocalId(localId)->secondaryItem();
    if (!path.has_value())
    {
        return nullptr;
    }

    return fromGmodPathRef(*path);
}

int dnv_vista_sdk_local_id_is_verbose_mode(const dnv_vista_sdk_local_id_t* localId)
{
    if (localId == nullptr)
    {
        c::setLastError("localId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toLocalId(localId)->isVerboseMode() ? 1 : 0;
}

int dnv_vista_sdk_local_id_has_custom_tag(const dnv_vista_sdk_local_id_t* localId)
{
    if (localId == nullptr)
    {
        c::setLastError("localId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toLocalId(localId)->hasCustomTag() ? 1 : 0;
}

const dnv_vista_sdk_metadata_tag_t* dnv_vista_sdk_local_id_metadata_tag(
    const dnv_vista_sdk_local_id_t* localId, dnv_vista_sdk_codebook_name_t name)
{
    if (localId == nullptr)
    {
        c::setLastError("localId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto* tag = selectMetadataTag(*toLocalId(localId), toCodebookName(name));
    if (tag == nullptr)
    {
        c::setLastError("invalid codebook name", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    if (!tag->has_value())
    {
        return nullptr;
    }

    return fromTagRef(**tag);
}

const dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder(const dnv_vista_sdk_local_id_t* localId)
{
    if (localId == nullptr)
    {
        c::setLastError("localId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromLocalIdBuilderRef(toLocalId(localId)->builder());
}

char* dnv_vista_sdk_local_id_to_string(const dnv_vista_sdk_local_id_t* localId)
{
    if (localId == nullptr)
    {
        c::setLastError("localId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toOwnedCString(toLocalId(localId)->toString());
}
