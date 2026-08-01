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
    try
    {
        return reinterpret_cast<dnv_vista_sdk_imo_number_t*>(new ImoNumber{ value });
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

dnv_vista_sdk_imo_number_t* dnv_vista_sdk_imo_number_from_string(const char* value)
{
    if (value == nullptr)
    {
        c::setLastErrorMessage("value must not be null");
        return nullptr;
    }

    auto imoNumber = ImoNumber::fromString(value);
    if (!imoNumber.has_value())
    {
        c::setLastErrorMessage("invalid IMO number string");
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
        c::setLastErrorMessage("a and b must not be null");
        return 0;
    }

    return (*toImoNumber(a) == *toImoNumber(b)) ? 1 : 0;
}

char* dnv_vista_sdk_imo_number_to_string(const dnv_vista_sdk_imo_number_t* imoNumber)
{
    if (imoNumber == nullptr)
    {
        c::setLastErrorMessage("imoNumber must not be null");
        return nullptr;
    }

    return toOwnedCString(toImoNumber(imoNumber)->toString());
}

void dnv_vista_sdk_imo_number_string_free(char* str)
{
    delete[] str;
}
