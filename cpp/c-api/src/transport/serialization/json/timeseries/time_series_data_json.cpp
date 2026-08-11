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
        c::setLastErrorMessage("json must not be null");
        return nullptr;
    }

    try
    {
        auto domain = tsdjson::fromJsonString(json, nullptr);
        if (!domain.has_value())
        {
            c::setLastErrorMessage("invalid TimeSeriesDataPackage JSON");
            return nullptr;
        }

        return fromTsdDataPackage(std::move(*domain));
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

char* dnv_vista_sdk_tsd_data_package_to_json(const dnv_vista_sdk_tsd_data_package_t* dataPackage, int prettyPrint)
{
    if (dataPackage == nullptr)
    {
        c::setLastErrorMessage("dataPackage must not be null");
        return nullptr;
    }

    try
    {
        return toOwnedCString(tsdjson::toJsonString(*toTsdDataPackage(dataPackage), prettyPrint != 0));
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

void dnv_vista_sdk_tsd_json_string_free(char* str)
{
    delete[] str;
}
