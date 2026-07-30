#include "dnv/vista/sdk/c/core/relative_location.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromLocationRef;
using dnv::vista::sdk::c::toRelativeLocation;

char dnv_vista_sdk_relative_location_code(const dnv_vista_sdk_relative_location_t* relativeLocation)
{
    if (relativeLocation == nullptr)
    {
        c::setLastError("relativeLocation must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return '\0';
    }

    return toRelativeLocation(relativeLocation)->code();
}

const char* dnv_vista_sdk_relative_location_name(const dnv_vista_sdk_relative_location_t* relativeLocation)
{
    if (relativeLocation == nullptr)
    {
        c::setLastError("relativeLocation must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toRelativeLocation(relativeLocation)->name().c_str();
}

const char* dnv_vista_sdk_relative_location_definition(const dnv_vista_sdk_relative_location_t* relativeLocation)
{
    if (relativeLocation == nullptr)
    {
        c::setLastError("relativeLocation must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& definition = toRelativeLocation(relativeLocation)->definition();
    if (!definition.has_value())
    {
        return nullptr;
    }

    return definition->c_str();
}

const dnv_vista_sdk_location_t* dnv_vista_sdk_relative_location_location(
    const dnv_vista_sdk_relative_location_t* relativeLocation)
{
    if (relativeLocation == nullptr)
    {
        c::setLastError("relativeLocation must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromLocationRef(toRelativeLocation(relativeLocation)->location());
}
