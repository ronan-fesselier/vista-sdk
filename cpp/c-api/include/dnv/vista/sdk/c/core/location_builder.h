/**
 * @file location_builder.h
 * @brief C API for dnv::vista::sdk::LocationBuilder
 * @details `dnv_vista_sdk_location_builder_t*` is owned by value and must be released
 *          with dnv_vista_sdk_location_builder_free. Every with_x/without_x function
 *          returns a new, independent handle - it does not mutate the input
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "location.h"
#include "location_group.h"
#include "locations.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_location_builder dnv_vista_sdk_location_builder_t;

    /**
     * @brief Create a new LocationBuilder for a Locations instance
     * @param locations Handle obtained from dnv_vista_sdk_vis_locations
     * @return Owned handle, must be released with dnv_vista_sdk_location_builder_free,
     *         or NULL if `locations` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_create(
        const dnv_vista_sdk_locations_t* locations);

    /**
     * @brief Release a LocationBuilder handle
     * @param builder Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_location_builder_free(dnv_vista_sdk_location_builder_t* builder);

    /**
     * @brief Set the number component
     * @param builder Handle obtained from this API
     * @param number Location number (must be > 0)
     * @return New owned handle, or NULL if `builder` is NULL or `number` < 1
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_with_number(
        const dnv_vista_sdk_location_builder_t* builder, int number);

    /**
     * @brief Remove the number component
     * @param builder Handle obtained from this API
     * @return New owned handle, or NULL if `builder` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_without_number(
        const dnv_vista_sdk_location_builder_t* builder);

    /**
     * @brief Set the side component
     * @param builder Handle obtained from this API
     * @param side Side character (must be valid for the builder's VIS version)
     * @return New owned handle, or NULL if `builder` is NULL or `side` is invalid
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_with_side(
        const dnv_vista_sdk_location_builder_t* builder, char side);

    /**
     * @brief Remove the side component
     * @param builder Handle obtained from this API
     * @return New owned handle, or NULL if `builder` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_without_side(
        const dnv_vista_sdk_location_builder_t* builder);

    /**
     * @brief Set the vertical component
     * @param builder Handle obtained from this API
     * @param vertical Vertical character (must be valid for the builder's VIS version)
     * @return New owned handle, or NULL if `builder` is NULL or `vertical` is invalid
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_with_vertical(
        const dnv_vista_sdk_location_builder_t* builder, char vertical);

    /**
     * @brief Remove the vertical component
     * @param builder Handle obtained from this API
     * @return New owned handle, or NULL if `builder` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_without_vertical(
        const dnv_vista_sdk_location_builder_t* builder);

    /**
     * @brief Set the transverse component
     * @param builder Handle obtained from this API
     * @param transverse Transverse character (must be valid for the builder's VIS version)
     * @return New owned handle, or NULL if `builder` is NULL or `transverse` is invalid
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_with_transverse(
        const dnv_vista_sdk_location_builder_t* builder, char transverse);

    /**
     * @brief Remove the transverse component
     * @param builder Handle obtained from this API
     * @return New owned handle, or NULL if `builder` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_without_transverse(
        const dnv_vista_sdk_location_builder_t* builder);

    /**
     * @brief Set the longitudinal component
     * @param builder Handle obtained from this API
     * @param longitudinal Longitudinal character (must be valid for the builder's VIS version)
     * @return New owned handle, or NULL if `builder` is NULL or `longitudinal` is invalid
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_with_longitudinal(
        const dnv_vista_sdk_location_builder_t* builder, char longitudinal);

    /**
     * @brief Remove the longitudinal component
     * @param builder Handle obtained from this API
     * @return New owned handle, or NULL if `builder` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_without_longitudinal(
        const dnv_vista_sdk_location_builder_t* builder);

    /**
     * @brief Parse and set location components from an existing Location
     * @param builder Handle obtained from this API
     * @param location Location to parse and extract components from, may be NULL to
     *                 mirror the C++ optional overload (fails in that case)
     * @return New owned handle, or NULL if `builder`/`location` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_with_location(
        const dnv_vista_sdk_location_builder_t* builder, const dnv_vista_sdk_location_t* location);

    /**
     * @brief Remove a location component by group
     * @param builder Handle obtained from this API
     * @param group The location group component to remove
     * @return New owned handle, or NULL if `builder` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_without_value(
        const dnv_vista_sdk_location_builder_t* builder, dnv_vista_sdk_location_group_t group);

    /**
     * @brief Get the location number component
     * @param builder Handle obtained from this API
     * @param outNumber Set to the number component on success
     * @return 1 if the number component is set, 0 if unset or `builder`/`outNumber` is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_location_builder_number(
        const dnv_vista_sdk_location_builder_t* builder, int* outNumber);

    /**
     * @brief Get the side location component
     * @param builder Handle obtained from this API
     * @param outSide Set to the side component on success
     * @return 1 if the side component is set, 0 if unset or `builder`/`outSide` is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_location_builder_side(
        const dnv_vista_sdk_location_builder_t* builder, char* outSide);

    /**
     * @brief Get the vertical location component
     * @param builder Handle obtained from this API
     * @param outVertical Set to the vertical component on success
     * @return 1 if the vertical component is set, 0 if unset or `builder`/`outVertical` is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_location_builder_vertical(
        const dnv_vista_sdk_location_builder_t* builder, char* outVertical);

    /**
     * @brief Get the transverse location component
     * @param builder Handle obtained from this API
     * @param outTransverse Set to the transverse component on success
     * @return 1 if the transverse component is set, 0 if unset or `builder`/`outTransverse` is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_location_builder_transverse(
        const dnv_vista_sdk_location_builder_t* builder, char* outTransverse);

    /**
     * @brief Get the longitudinal location component
     * @param builder Handle obtained from this API
     * @param outLongitudinal Set to the longitudinal component on success
     * @return 1 if the longitudinal component is set, 0 if unset or `builder`/`outLongitudinal` is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_location_builder_longitudinal(
        const dnv_vista_sdk_location_builder_t* builder, char* outLongitudinal);

    /**
     * @brief Set a location component by character code, auto-detecting its group
     * @param builder Handle obtained from this API
     * @param code Character code (e.g. 'S' for Side, 'U' for Upper)
     * @return New owned handle, or NULL if `builder` is NULL or `code` is not valid
     *         for any location group
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_with_code(
        const dnv_vista_sdk_location_builder_t* builder, char code);

    /**
     * @brief Build the final Location from the builder's current state
     * @param builder Handle obtained from this API
     * @return Owned handle, must be released with dnv_vista_sdk_location_free, or NULL
     *         if `builder` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_location_t* dnv_vista_sdk_location_builder_build(
        const dnv_vista_sdk_location_builder_t* builder);

    /**
     * @brief Convert the builder's current state to its string representation
     * @param builder Handle obtained from this API
     * @return Owned, null-terminated string, must be released with
     *         dnv_vista_sdk_string_free, or NULL if `builder` is NULL
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_location_builder_to_string(const dnv_vista_sdk_location_builder_t* builder);

#ifdef __cplusplus
}
#endif
