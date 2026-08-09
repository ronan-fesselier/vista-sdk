#include "dnv/vista/sdk/c/transport/serialization/json/datachannel/data_channel_json.h"

#include "../../../../cast_internal.h"
#include "../../../../error_internal.h"

using namespace dnv::vista::sdk;
using namespace dnv::vista::sdk::c;
namespace dcljson = dnv::vista::sdk::transport::serialization::json::datachannel;

dnv_vista_sdk_dcl_list_package_t* dnv_vista_sdk_dcl_list_package_from_json(const char* json)
{
    if (json == nullptr)
    {
        c::setLastError("json must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_dcl_list_package_t*>([&]() -> dnv_vista_sdk_dcl_list_package_t* {
        auto domain = dcljson::fromJsonString(json, nullptr);
        if (!domain.has_value())
        {
            c::setLastError("invalid DataChannelListPackage JSON", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
            return nullptr;
        }

        return fromDataChannelListPackage(std::move(*domain));
    });
}

char* dnv_vista_sdk_dcl_list_package_to_json(const dnv_vista_sdk_dcl_list_package_t* package, int prettyPrint)
{
    if (package == nullptr)
    {
        c::setLastError("package must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<char*>([&]() -> char* {
        return toOwnedCString(dcljson::toJsonString(*toDataChannelListPackage(package), prettyPrint != 0));
    });
}
