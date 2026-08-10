/**
 * @file data_channel_id.h
 * @brief C API for dnv::vista::sdk::transport::timeseries::DataChannelId
 * @details `dnv_vista_sdk_tsd_channel_id_t*` is owned by value and must be released
 *          with dnv_vista_sdk_tsd_channel_id_free. The C++ `match()` template
 *          (compile-time visitor over the underlying std::variant) is not exposed -
 *          is_local_id()/is_short_id() plus local_id()/short_id() cover the same
 *          runtime dispatch.
 *
 *          Prefixed `dnv_vista_sdk_tsd_channel_id_*` (timeseries), distinct from
 *          `dnv_vista_sdk_dcl_channel_id_*` (datachannel.h) - the latter mirrors
 *          `datachannel::DataChannelId` (Table 15: LocalId + optional ShortId/
 *          NameObject, not a union), a different C++ type entirely
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "../../core/local_id.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_tsd_channel_id dnv_vista_sdk_tsd_channel_id_t;

    /**
     * @brief Parse a DataChannelId from its string representation
     * @details Attempts to parse `value` as a LocalId. If that fails (e.g. malformed
     *          VIS syntax, or a string not intended as a LocalId at all), `value` is
     *          stored verbatim as a ShortId instead. There is no '/'-prefix check - a
     *          string starting with '/' that fails to parse as a valid LocalId still
     *          becomes a ShortId. Only an empty `value` returns NULL
     * @return Owned handle, must be released with dnv_vista_sdk_tsd_channel_id_free,
     *         or NULL if `value` is NULL or empty
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_channel_id_t* dnv_vista_sdk_tsd_channel_id_from_string(const char* value);

    /**
     * @brief Release a DataChannelId handle
     * @param channelId Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_channel_id_free(dnv_vista_sdk_tsd_channel_id_t* channelId);

    /**
     * @brief Compare two DataChannelId handles for equality
     * @return 1 if both represent the same DataChannelId, 0 otherwise or if either is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_channel_id_equals(
        const dnv_vista_sdk_tsd_channel_id_t* a, const dnv_vista_sdk_tsd_channel_id_t* b);

    /** @brief 1 if this DataChannelId holds a LocalId, 0 otherwise or if `channelId` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_channel_id_is_local_id(const dnv_vista_sdk_tsd_channel_id_t* channelId);

    /** @brief 1 if this DataChannelId holds a ShortId, 0 otherwise or if `channelId` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_channel_id_is_short_id(const dnv_vista_sdk_tsd_channel_id_t* channelId);

    /**
     * @brief Get the LocalId, if this DataChannelId holds one
     * @return Borrowed pointer, valid as long as `channelId` is valid, or NULL if
     *         `channelId` is NULL or holds a ShortId
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_local_id_t* dnv_vista_sdk_tsd_channel_id_local_id(
        const dnv_vista_sdk_tsd_channel_id_t* channelId);

    /**
     * @brief Get the ShortId, if this DataChannelId holds one
     * @return Null-terminated string, owned by `channelId` - valid as long as
     *         `channelId` is valid, or NULL if `channelId` is NULL or holds a LocalId
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_channel_id_short_id(
        const dnv_vista_sdk_tsd_channel_id_t* channelId);

    /**
     * @brief Convert to string representation
     * @return Owned, null-terminated string (the LocalId's string form, or the
     *         ShortId), must be released with dnv_vista_sdk_tsd_channel_id_string_free,
     *         or NULL if `channelId` is NULL
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_tsd_channel_id_to_string(const dnv_vista_sdk_tsd_channel_id_t* channelId);

    /**
     * @brief Release a string obtained from this API
     * @param str String obtained from dnv_vista_sdk_tsd_channel_id_to_string, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_channel_id_string_free(char* str);

#ifdef __cplusplus
}
#endif
