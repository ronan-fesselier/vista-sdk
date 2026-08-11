/**
 * @file time_series_data_json.h
 * @brief C API for JSON serialization of dnv::vista::sdk::transport::timeseries::TimeSeriesDataPackage
 * @details Mirrors the domain-object overloads of
 *          dnv::vista::sdk::transport::serialization::json::timeseries::toJsonString/fromJsonString
 *          (Extensions.h). The intermediate TimeSeriesDataPackageDto and toDto/toDomain are
 *          internal implementation details, never exposed directly - same rationale as
 *          data_channel_json.h for the DataChannelList package: the domain type
 *          dnv_vista_sdk_tsd_data_package_t already covers building and reading the same
 *          content end-to-end
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "dnv/vista/sdk/c/transport/timeseries/time_series_data.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Parse a TimeSeriesDataPackage from a JSON string
     * @param json JSON string to parse, must not be NULL
     * @return Owned handle, must be released with dnv_vista_sdk_tsd_data_package_free,
     *         or NULL (with the last error message set) if `json` is NULL, fails to
     *         parse, or fails domain construction
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_data_package_t* dnv_vista_sdk_tsd_data_package_from_json(const char* json);

    /**
     * @brief Serialize a TimeSeriesDataPackage to a JSON string
     * @param dataPackage Handle obtained from this API, must not be NULL
     * @param prettyPrint 1 for 2-space indented output, 0 for compact output
     * @return Owned, null-terminated string, must be released with
     *         dnv_vista_sdk_string_free, or NULL if `dataPackage` is NULL
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_tsd_data_package_to_json(
        const dnv_vista_sdk_tsd_data_package_t* dataPackage, int prettyPrint);

#ifdef __cplusplus
}
#endif
