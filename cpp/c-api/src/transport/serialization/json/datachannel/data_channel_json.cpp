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
        c::setLastErrorMessage("json must not be null");
        return nullptr;
    }

    try
    {
        auto domain = dcljson::fromJsonString(json, nullptr);
        if (!domain.has_value())
        {
            c::setLastErrorMessage("invalid DataChannelListPackage JSON");
            return nullptr;
        }

        return fromDataChannelListPackage(std::move(*domain));
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

char* dnv_vista_sdk_dcl_list_package_to_json(const dnv_vista_sdk_dcl_list_package_t* package, int prettyPrint)
{
    if (package == nullptr)
    {
        c::setLastErrorMessage("package must not be null");
        return nullptr;
    }

    try
    {
        return toOwnedCString(dcljson::toJsonString(*toDataChannelListPackage(package), prettyPrint != 0));
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

void dnv_vista_sdk_dcl_json_string_free(char* str)
{
    delete[] str;
}
