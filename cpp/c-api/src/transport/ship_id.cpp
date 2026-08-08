#include "dnv/vista/sdk/c/transport/ship_id.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromImoNumberValue;
using dnv::vista::sdk::c::fromShipId;
using dnv::vista::sdk::c::toImoNumber;
using dnv::vista::sdk::c::toOwnedCString;
using dnv::vista::sdk::c::toShipId;

dnv_vista_sdk_ship_id_t* dnv_vista_sdk_ship_id_from_imo_number(const dnv_vista_sdk_imo_number_t* imoNumber)
{
    if (imoNumber == nullptr)
    {
        c::setLastErrorMessage("imoNumber must not be null");
        return nullptr;
    }

    return fromShipId(transport::ShipId{ *toImoNumber(imoNumber) });
}

dnv_vista_sdk_ship_id_t* dnv_vista_sdk_ship_id_from_other_id(const char* otherId)
{
    if (otherId == nullptr)
    {
        c::setLastErrorMessage("otherId must not be null");
        return nullptr;
    }

    try
    {
        return fromShipId(transport::ShipId{ std::string{ otherId } });
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

dnv_vista_sdk_ship_id_t* dnv_vista_sdk_ship_id_from_string(const char* value)
{
    if (value == nullptr)
    {
        c::setLastErrorMessage("value must not be null");
        return nullptr;
    }

    auto shipId = transport::ShipId::fromString(value);
    if (!shipId.has_value())
    {
        c::setLastErrorMessage("invalid ShipId string");
    }

    return fromShipId(std::move(shipId));
}

void dnv_vista_sdk_ship_id_free(dnv_vista_sdk_ship_id_t* shipId)
{
    delete reinterpret_cast<transport::ShipId*>(shipId);
}

int dnv_vista_sdk_ship_id_equals(const dnv_vista_sdk_ship_id_t* a, const dnv_vista_sdk_ship_id_t* b)
{
    if (a == nullptr || b == nullptr)
    {
        c::setLastErrorMessage("a and b must not be null");
        return 0;
    }

    return *toShipId(a) == *toShipId(b) ? 1 : 0;
}

int dnv_vista_sdk_ship_id_is_imo_number(const dnv_vista_sdk_ship_id_t* shipId)
{
    if (shipId == nullptr)
    {
        c::setLastErrorMessage("shipId must not be null");
        return 0;
    }

    return toShipId(shipId)->isImoNumber() ? 1 : 0;
}

int dnv_vista_sdk_ship_id_is_other_id(const dnv_vista_sdk_ship_id_t* shipId)
{
    if (shipId == nullptr)
    {
        c::setLastErrorMessage("shipId must not be null");
        return 0;
    }

    return toShipId(shipId)->isOtherId() ? 1 : 0;
}

dnv_vista_sdk_imo_number_t* dnv_vista_sdk_ship_id_imo_number(const dnv_vista_sdk_ship_id_t* shipId)
{
    if (shipId == nullptr)
    {
        c::setLastErrorMessage("shipId must not be null");
        return nullptr;
    }

    const auto imoNumber = toShipId(shipId)->imoNumber();
    if (!imoNumber.has_value())
    {
        return nullptr;
    }

    return fromImoNumberValue(*imoNumber);
}

const char* dnv_vista_sdk_ship_id_other_id(const dnv_vista_sdk_ship_id_t* shipId)
{
    if (shipId == nullptr)
    {
        c::setLastErrorMessage("shipId must not be null");
        return nullptr;
    }

    const auto otherId = toShipId(shipId)->otherId();
    if (!otherId.has_value())
    {
        return nullptr;
    }

    return otherId->data();
}

char* dnv_vista_sdk_ship_id_to_string(const dnv_vista_sdk_ship_id_t* shipId)
{
    if (shipId == nullptr)
    {
        c::setLastErrorMessage("shipId must not be null");
        return nullptr;
    }

    return toOwnedCString(toShipId(shipId)->toString());
}

void dnv_vista_sdk_ship_id_string_free(char* str)
{
    delete[] str;
}
