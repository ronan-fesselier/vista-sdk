/**
 * @file universal_id_builder.h
 * @brief C API for dnv::vista::sdk::UniversalIdBuilder
 * @details `dnv_vista_sdk_universal_id_builder_t*` is owned by value and must be
 *          released with dnv_vista_sdk_universal_id_builder_free. Every with_x/without_x
 *          function returns a new, independent handle - it does not mutate the input
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "imo_number.h"
#include "local_id_builder.h"
#include "parsing_errors.h"
#include "universal_id.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_universal_id_builder dnv_vista_sdk_universal_id_builder_t;

    /** @brief Get the naming entity identifier ("data.dnv.com") - static, never NULL */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_universal_id_builder_naming_entity(void);

    /**
     * @brief Create a new UniversalIdBuilder for a VIS version
     * @param visVersion VIS version string (e.g. "3-4a"), used to initialize the
     *                   builder's LocalIdBuilder component
     * @return Owned handle, must be released with dnv_vista_sdk_universal_id_builder_free,
     *         or NULL if `visVersion` is invalid or NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_universal_id_builder_t* dnv_vista_sdk_universal_id_builder_create(
        const char* visVersion);

    /**
     * @brief Release a UniversalIdBuilder handle
     * @param builder Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_universal_id_builder_free(dnv_vista_sdk_universal_id_builder_t* builder);

    /**
     * @brief Get the IMO number
     * @return Owned handle (a copy), must be released with dnv_vista_sdk_imo_number_free,
     *         or NULL if `builder` is NULL or unset
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_imo_number_t* dnv_vista_sdk_universal_id_builder_imo_number(
        const dnv_vista_sdk_universal_id_builder_t* builder);

    /**
     * @brief Get the LocalIdBuilder
     * @return Owned handle (a copy), must be released with dnv_vista_sdk_local_id_builder_free,
     *         or NULL if `builder` is NULL or unset
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_universal_id_builder_local_id(
        const dnv_vista_sdk_universal_id_builder_t* builder);

    /** @brief 1 if both IMO number and a valid LocalIdBuilder are set, 0 otherwise or if `builder` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_universal_id_builder_is_valid(
        const dnv_vista_sdk_universal_id_builder_t* builder);

    /** @brief Set the IMO number. Returns NULL if `builder`/`imoNumber` is NULL */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_universal_id_builder_t* dnv_vista_sdk_universal_id_builder_with_imo_number(
        const dnv_vista_sdk_universal_id_builder_t* builder, const dnv_vista_sdk_imo_number_t* imoNumber);

    /** @brief Remove the IMO number. Returns NULL if `builder` is NULL */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_universal_id_builder_t* dnv_vista_sdk_universal_id_builder_without_imo_number(
        const dnv_vista_sdk_universal_id_builder_t* builder);

    /** @brief Set the LocalIdBuilder. Returns NULL if `builder`/`localIdBuilder` is NULL */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_universal_id_builder_t* dnv_vista_sdk_universal_id_builder_with_local_id(
        const dnv_vista_sdk_universal_id_builder_t* builder, const dnv_vista_sdk_local_id_builder_t* localIdBuilder);

    /** @brief Remove the LocalIdBuilder. Returns NULL if `builder` is NULL */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_universal_id_builder_t* dnv_vista_sdk_universal_id_builder_without_local_id(
        const dnv_vista_sdk_universal_id_builder_t* builder);

    /**
     * @brief Build the final UniversalId from the builder's current state
     * @return Owned handle, must be released with dnv_vista_sdk_universal_id_free,
     *         or NULL if `builder` is NULL or invalid (missing IMO number or LocalIdBuilder)
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_universal_id_t* dnv_vista_sdk_universal_id_builder_build(
        const dnv_vista_sdk_universal_id_builder_t* builder);

    /**
     * @brief Convert builder state to string representation
     * @return Owned, null-terminated string, must be released with
     *         dnv_vista_sdk_string_free, or NULL if `builder`
     *         is NULL or invalid (missing/invalid IMO number or LocalIdBuilder)
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_universal_id_builder_to_string(
        const dnv_vista_sdk_universal_id_builder_t* builder);

    /**
     * @brief Parse a UniversalId string
     * @return Owned handle, must be released with dnv_vista_sdk_universal_id_free,
     *         or NULL if `universalIdStr` is invalid or NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_universal_id_t* dnv_vista_sdk_universal_id_builder_from_string(
        const char* universalIdStr);

    /**
     * @brief Parse a UniversalId string, collecting detailed error information on failure
     * @param universalIdStr UniversalId string to parse
     * @param outErrors Set to an owned ParsingErrors handle (must be released with
     *                  dnv_vista_sdk_parsing_errors_free), even on success. Left
     *                  untouched if `universalIdStr`/`outErrors` is NULL
     * @return Owned handle, must be released with dnv_vista_sdk_universal_id_free,
     *         or NULL if `universalIdStr` is invalid or NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_universal_id_t* dnv_vista_sdk_universal_id_builder_from_string_with_errors(
        const char* universalIdStr, dnv_vista_sdk_parsing_errors_t** outErrors);

#ifdef __cplusplus
}
#endif
