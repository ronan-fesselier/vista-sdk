#include "dnv/vista/sdk/c/transport/serialization/json/timeseries/time_series_data_json.h"

#include "../../../../cast_internal.h"
#include "../../../../error_internal.h"

using namespace dnv::vista::sdk;
using namespace dnv::vista::sdk::c;
namespace tsdjson = dnv::vista::sdk::transport::serialization::json::timeseries;

dnv_vista_sdk_tsd_data_package_t* dnv_vista_sdk_tsd_data_package_from_json(const char* json)
{
    if (json == nullptr)
    {
        c::setLastError("json must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_tsd_data_package_t*>([&]() -> dnv_vista_sdk_tsd_data_package_t* {
        auto domain = tsdjson::fromJsonString(json, nullptr);
        if (!domain.has_value())
        {
            c::setLastError("invalid TimeSeriesDataPackage JSON", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
            return nullptr;
        }

        return fromTsdDataPackage(std::move(*domain));
    });
}

char* dnv_vista_sdk_tsd_data_package_to_json(const dnv_vista_sdk_tsd_data_package_t* dataPackage, int prettyPrint)
{
    if (dataPackage == nullptr)
    {
        c::setLastError("dataPackage must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<char*>([&]() -> char* {
        return toOwnedCString(tsdjson::toJsonString(*toTsdDataPackage(dataPackage), prettyPrint != 0));
    });
}
