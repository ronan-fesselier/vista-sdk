#include "dnv/vista/sdk/c/core/location.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::toLocation;

void dnv_vista_sdk_location_free(dnv_vista_sdk_location_t* location)
{
    delete reinterpret_cast<Location*>(location);
}

const char* dnv_vista_sdk_location_value(const dnv_vista_sdk_location_t* location)
{
    if (location == nullptr)
    {
        c::setLastError("location must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toLocation(location)->value().c_str();
}
