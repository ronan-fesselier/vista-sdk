/**
 * @file data_channel_json.h
 * @brief C API for JSON serialization of dnv::vista::sdk::transport::datachannel::DataChannelListPackage
 * @details Mirrors the domain-object overloads of
 *          dnv::vista::sdk::transport::serialization::json::datachannel::toJsonString/fromJsonString
 *          (Extensions.h). The intermediate DataChannelListPackageDto and toDto/toDomain are
 *          internal implementation details, never exposed directly - they exist only to bridge
 *          the domain model and the JSON representation, which this header already covers
 *          end-to-end via the domain type dnv_vista_sdk_dcl_list_package_t
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "../../../datachannel/data_channel.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Parse a DataChannelListPackage from a JSON string
     * @param json JSON string to parse, must not be NULL
     * @return Owned handle, must be released with dnv_vista_sdk_dcl_list_package_free,
     *         or NULL (with the last error message set) if `json` is NULL, fails to
     *         parse, or fails domain construction (e.g. an unrecognized Format/
     *         DataChannelType type string, an invalid Restriction::WhiteSpace value,
     *         or a Range with low > high)
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_list_package_t* dnv_vista_sdk_dcl_list_package_from_json(const char* json);

    /**
     * @brief Serialize a DataChannelListPackage to a JSON string
     * @param package Handle obtained from this API, must not be NULL
     * @param prettyPrint 1 for 2-space indented output, 0 for compact output
     * @return Owned, null-terminated string, must be released with
     *         dnv_vista_sdk_dcl_json_string_free, or NULL if `package` is NULL
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_dcl_list_package_to_json(
        const dnv_vista_sdk_dcl_list_package_t* package, int prettyPrint);

    /**
     * @brief Release a string obtained from this API
     * @param str String obtained from dnv_vista_sdk_dcl_list_package_to_json, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_json_string_free(char* str);

#ifdef __cplusplus
}
#endif
