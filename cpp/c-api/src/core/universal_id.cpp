#include "dnv/vista/sdk/c/core/universal_id.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromImoNumberRef;
using dnv::vista::sdk::c::fromLocalIdRef;
using dnv::vista::sdk::c::fromUniversalId;
using dnv::vista::sdk::c::fromUniversalIdBuilderRef;
using dnv::vista::sdk::c::toOwnedCString;
using dnv::vista::sdk::c::toUniversalId;

const char* dnv_vista_sdk_universal_id_naming_entity(void)
{
    return UniversalId::namingEntity().data();
}

void dnv_vista_sdk_universal_id_free(dnv_vista_sdk_universal_id_t* universalId)
{
    delete reinterpret_cast<UniversalId*>(universalId);
}

dnv_vista_sdk_universal_id_t* dnv_vista_sdk_universal_id_from_string(const char* universalIdStr)
{
    if (universalIdStr == nullptr)
    {
        c::setLastError("universalIdStr must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_universal_id_t*>([&]() -> dnv_vista_sdk_universal_id_t* {
        auto universalId = UniversalId::fromString(universalIdStr);
        if (!universalId.has_value())
        {
            c::setLastError("invalid UniversalId string", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        }

        return fromUniversalId(std::move(universalId));
    });
}

const dnv_vista_sdk_imo_number_t* dnv_vista_sdk_universal_id_imo_number(const dnv_vista_sdk_universal_id_t* universalId)
{
    if (universalId == nullptr)
    {
        c::setLastError("universalId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromImoNumberRef(toUniversalId(universalId)->imoNumber());
}

const dnv_vista_sdk_local_id_t* dnv_vista_sdk_universal_id_local_id(const dnv_vista_sdk_universal_id_t* universalId)
{
    if (universalId == nullptr)
    {
        c::setLastError("universalId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromLocalIdRef(toUniversalId(universalId)->localId());
}

const dnv_vista_sdk_universal_id_builder_t* dnv_vista_sdk_universal_id_builder(
    const dnv_vista_sdk_universal_id_t* universalId)
{
    if (universalId == nullptr)
    {
        c::setLastError("universalId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromUniversalIdBuilderRef(toUniversalId(universalId)->builder());
}

char* dnv_vista_sdk_universal_id_to_string(const dnv_vista_sdk_universal_id_t* universalId)
{
    if (universalId == nullptr)
    {
        c::setLastError("universalId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toOwnedCString(toUniversalId(universalId)->toString());
}
