/**
 * @file ship_id.h
 * @brief C API for dnv::vista::sdk::transport::ShipId
 * @details `dnv_vista_sdk_ship_id_t*` is owned by value and must be released with
 *          dnv_vista_sdk_ship_id_free. The C++ `match()` template (compile-time
 *          visitor over the underlying std::variant) is not exposed - isImoNumber()/
 *          isOtherId() plus imoNumber()/otherId() cover the same runtime dispatch
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "../core/imo_number.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_ship_id dnv_vista_sdk_ship_id_t;

    /**
     * @brief Construct a ShipId from an IMO number
     * @param imoNumber IMO number to wrap, must not be NULL
     * @return Owned handle, must be released with dnv_vista_sdk_ship_id_free, or
     *         NULL if `imoNumber` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_ship_id_t* dnv_vista_sdk_ship_id_from_imo_number(
        const dnv_vista_sdk_imo_number_t* imoNumber);

    /**
     * @brief Construct a ShipId from an alternative identifier string
     * @param otherId Alternative ship identifier, must not be NULL or empty
     * @return Owned handle, must be released with dnv_vista_sdk_ship_id_free, or
     *         NULL if `otherId` is NULL or empty
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_ship_id_t* dnv_vista_sdk_ship_id_from_other_id(const char* otherId);

    /**
     * @brief Parse a ShipId from its string representation
     * @param value String with "IMO" prefix (case-insensitive) followed by a valid IMO
     *              number, or an alternative identifier
     * @return Owned handle, must be released with dnv_vista_sdk_ship_id_free, or NULL
     *         if `value` is NULL or empty
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_ship_id_t* dnv_vista_sdk_ship_id_from_string(const char* value);

    /**
     * @brief Release a ShipId handle
     * @param shipId Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_ship_id_free(dnv_vista_sdk_ship_id_t* shipId);

    /**
     * @brief Compare two ShipId handles for equality
     * @return 1 if both represent the same ShipId, 0 otherwise or if either is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_ship_id_equals(
        const dnv_vista_sdk_ship_id_t* a, const dnv_vista_sdk_ship_id_t* b);

    /** @brief 1 if this ShipId holds an IMO number, 0 otherwise or if `shipId` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_ship_id_is_imo_number(const dnv_vista_sdk_ship_id_t* shipId);

    /** @brief 1 if this ShipId holds an alternative identifier, 0 otherwise or if `shipId` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_ship_id_is_other_id(const dnv_vista_sdk_ship_id_t* shipId);

    /**
     * @brief Get the IMO number, if this ShipId holds one
     * @return Owned handle (a copy), must be released with dnv_vista_sdk_imo_number_free,
     *         or NULL if `shipId` is NULL or holds an alternative identifier
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_imo_number_t* dnv_vista_sdk_ship_id_imo_number(
        const dnv_vista_sdk_ship_id_t* shipId);

    /**
     * @brief Get the alternative identifier, if this ShipId holds one
     * @return Null-terminated string, owned by `shipId` - valid as long as `shipId` is
     *         valid, or NULL if `shipId` is NULL or holds an IMO number
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_ship_id_other_id(const dnv_vista_sdk_ship_id_t* shipId);

    /**
     * @brief Convert to string representation
     * @return Owned, null-terminated string (IMO number with "IMO" prefix, or the
     *         alternative identifier), must be released with dnv_vista_sdk_ship_id_string_free,
     *         or NULL if `shipId` is NULL
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_ship_id_to_string(const dnv_vista_sdk_ship_id_t* shipId);

    /**
     * @brief Release a string obtained from this API
     * @param str String obtained from dnv_vista_sdk_ship_id_to_string, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_ship_id_string_free(char* str);

#ifdef __cplusplus
}
#endif
