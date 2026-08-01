#include "dnv/vista/sdk/c/core/imo_number.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::toImoNumber;
using dnv::vista::sdk::c::toOwnedCString;

int dnv_vista_sdk_imo_number_is_valid(int imoNumber)
{
    return ImoNumber::isValid(imoNumber) ? 1 : 0;
}

dnv_vista_sdk_imo_number_t* dnv_vista_sdk_imo_number_create(int value)
{
    return c::cApiTryCatch<dnv_vista_sdk_imo_number_t*>([&]() -> dnv_vista_sdk_imo_number_t* {
        return reinterpret_cast<dnv_vista_sdk_imo_number_t*>(new ImoNumber{ value });
    });
}

dnv_vista_sdk_imo_number_t* dnv_vista_sdk_imo_number_from_string(const char* value)
{
    if (value == nullptr)
    {
        c::setLastError("value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    auto imoNumber = ImoNumber::fromString(value);
    if (!imoNumber.has_value())
    {
        c::setLastError("invalid IMO number string", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return reinterpret_cast<dnv_vista_sdk_imo_number_t*>(new ImoNumber{ std::move(*imoNumber) });
}

void dnv_vista_sdk_imo_number_free(dnv_vista_sdk_imo_number_t* imoNumber)
{
    delete reinterpret_cast<ImoNumber*>(imoNumber);
}

int dnv_vista_sdk_imo_number_equals(const dnv_vista_sdk_imo_number_t* a, const dnv_vista_sdk_imo_number_t* b)
{
    if (a == nullptr || b == nullptr)
    {
        c::setLastError("a and b must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return (*toImoNumber(a) == *toImoNumber(b)) ? 1 : 0;
}

char* dnv_vista_sdk_imo_number_to_string(const dnv_vista_sdk_imo_number_t* imoNumber)
{
    if (imoNumber == nullptr)
    {
        c::setLastError("imoNumber must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toOwnedCString(toImoNumber(imoNumber)->toString());
}
