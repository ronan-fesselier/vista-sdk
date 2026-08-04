/**
 * @file local_id_mqtt.h
 * @brief C API for dnv::vista::sdk::mqtt::LocalId
 * @details `dnv_vista_sdk_local_id_mqtt_t*` is owned by value and must be released
 *          with dnv_vista_sdk_local_id_mqtt_free
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "codebook_name.h"
#include "gmod_path.h"
#include "local_id_builder.h"
#include "metadata_tag.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_local_id_mqtt dnv_vista_sdk_local_id_mqtt_t;

    /**
     * @brief Construct an MQTT LocalId from a validated LocalIdBuilder
     * @param builder Handle obtained from local_id_builder API
     * @return Owned handle, must be released with dnv_vista_sdk_local_id_mqtt_free,
     *         or NULL if `builder` is NULL or invalid/empty
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_mqtt_t* dnv_vista_sdk_local_id_mqtt_create(
        const dnv_vista_sdk_local_id_builder_t* builder);

    /**
     * @brief Release an MQTT LocalId handle
     * @param localId Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_local_id_mqtt_free(dnv_vista_sdk_local_id_mqtt_t* localId);

    /** @brief Get the VIS version string, or NULL if `localId` is NULL */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_local_id_mqtt_version(const dnv_vista_sdk_local_id_mqtt_t* localId);

    /**
     * @brief Get the primary item
     * @return Borrowed pointer, valid as long as `localId` is valid, or NULL if
     *         `localId` is NULL
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_local_id_mqtt_primary_item(
        const dnv_vista_sdk_local_id_mqtt_t* localId);

    /**
     * @brief Get the secondary item
     * @return Borrowed pointer, valid as long as `localId` is valid, or NULL if
     *         `localId` is NULL or unset
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_local_id_mqtt_secondary_item(
        const dnv_vista_sdk_local_id_mqtt_t* localId);

    /**
     * @brief Get a metadata tag by codebook name
     * @return Borrowed pointer, valid as long as `localId` is valid, or NULL if
     *         `localId` is NULL or the tag is unset
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_metadata_tag_t* dnv_vista_sdk_local_id_mqtt_metadata_tag(
        const dnv_vista_sdk_local_id_mqtt_t* localId, dnv_vista_sdk_codebook_name_t name);

    /**
     * @brief Get the builder that created this MQTT LocalId
     * @return Borrowed pointer, valid as long as `localId` is valid, or NULL if
     *         `localId` is NULL
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_mqtt_builder(
        const dnv_vista_sdk_local_id_mqtt_t* localId);

    /**
     * @brief Convert to MQTT-compatible topic string representation
     * @return Owned, null-terminated string, must be released with
     *         dnv_vista_sdk_string_free, or NULL if `localId` is NULL
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_local_id_mqtt_to_string(const dnv_vista_sdk_local_id_mqtt_t* localId);

#ifdef __cplusplus
}
#endif
