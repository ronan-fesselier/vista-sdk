#include "dnv/vista/sdk/c/transport/timeseries/data_channel_id.h"

#include "../../cast_internal.h"
#include "../../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromLocalIdRef;
using dnv::vista::sdk::c::fromTsdChannelId;
using dnv::vista::sdk::c::toOwnedCString;
using dnv::vista::sdk::c::toTsdChannelId;

dnv_vista_sdk_tsd_channel_id_t* dnv_vista_sdk_tsd_channel_id_from_string(const char* value)
{
    if (value == nullptr)
    {
        c::setLastError("value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    auto channelId = transport::timeseries::DataChannelId::fromString(value);
    if (!channelId.has_value())
    {
        c::setLastError("invalid DataChannelId string", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
    }

    return fromTsdChannelId(std::move(channelId));
}

void dnv_vista_sdk_tsd_channel_id_free(dnv_vista_sdk_tsd_channel_id_t* channelId)
{
    delete reinterpret_cast<transport::timeseries::DataChannelId*>(channelId);
}

int dnv_vista_sdk_tsd_channel_id_equals(
    const dnv_vista_sdk_tsd_channel_id_t* a, const dnv_vista_sdk_tsd_channel_id_t* b)
{
    if (a == nullptr || b == nullptr)
    {
        c::setLastError("a and b must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return *toTsdChannelId(a) == *toTsdChannelId(b) ? 1 : 0;
}

int dnv_vista_sdk_tsd_channel_id_is_local_id(const dnv_vista_sdk_tsd_channel_id_t* channelId)
{
    if (channelId == nullptr)
    {
        c::setLastError("channelId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toTsdChannelId(channelId)->isLocalId() ? 1 : 0;
}

int dnv_vista_sdk_tsd_channel_id_is_short_id(const dnv_vista_sdk_tsd_channel_id_t* channelId)
{
    if (channelId == nullptr)
    {
        c::setLastError("channelId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toTsdChannelId(channelId)->isShortId() ? 1 : 0;
}

const dnv_vista_sdk_local_id_t* dnv_vista_sdk_tsd_channel_id_local_id(const dnv_vista_sdk_tsd_channel_id_t* channelId)
{
    if (channelId == nullptr)
    {
        c::setLastError("channelId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto localId = toTsdChannelId(channelId)->localId();
    if (!localId.has_value())
    {
        return nullptr;
    }

    return fromLocalIdRef(localId->get());
}

const char* dnv_vista_sdk_tsd_channel_id_short_id(const dnv_vista_sdk_tsd_channel_id_t* channelId)
{
    if (channelId == nullptr)
    {
        c::setLastError("channelId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto shortId = toTsdChannelId(channelId)->shortId();
    if (!shortId.has_value())
    {
        return nullptr;
    }

    return shortId->data();
}

char* dnv_vista_sdk_tsd_channel_id_to_string(const dnv_vista_sdk_tsd_channel_id_t* channelId)
{
    if (channelId == nullptr)
    {
        c::setLastError("channelId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toOwnedCString(toTsdChannelId(channelId)->toString());
}
