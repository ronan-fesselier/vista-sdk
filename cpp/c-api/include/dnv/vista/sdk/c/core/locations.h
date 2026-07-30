/**
 * @file locations.h
 * @brief C API for dnv::vista::sdk::Locations
 * @details `dnv_vista_sdk_locations_t*` is a borrowed pointer into the VIS singleton's
 *          cache - never freed, same lifetime rules as Gmod/Codebooks
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "location.h"
#include "location_group.h"
#include "parsing_errors.h"
#include "relative_location.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_locations dnv_vista_sdk_locations_t;

    /**
     * @brief Get the VIS version string for a Locations instance
     * @param locations Handle obtained from dnv_vista_sdk_vis_locations
     * @return Null-terminated version string, owned by the library - valid for the
     *         lifetime of the program, or NULL if `locations` is NULL
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_locations_version(const dnv_vista_sdk_locations_t* locations);

    /**
     * @brief Get the number of relative locations
     * @param locations Handle obtained from dnv_vista_sdk_vis_locations
     * @return Count, or 0 if `locations` is NULL
     */
    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_locations_relative_location_count(const dnv_vista_sdk_locations_t* locations);

    /**
     * @brief Get a relative location by index
     * @param locations Handle obtained from dnv_vista_sdk_vis_locations
     * @param index Zero-based index, must be < dnv_vista_sdk_locations_relative_location_count(locations)
     * @return Borrowed pointer, valid as long as `locations` is valid, or NULL if
     *         `locations` is NULL or `index` is out of range
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_relative_location_t* dnv_vista_sdk_locations_relative_location_at(
        const dnv_vista_sdk_locations_t* locations, size_t index);

    /**
     * @brief Get the number of relative locations belonging to a group
     * @param locations Handle obtained from dnv_vista_sdk_vis_locations
     * @param group Location group
     * @return Count, or 0 if `locations` is NULL or the group has no entries
     */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_locations_group_count(
        const dnv_vista_sdk_locations_t* locations, dnv_vista_sdk_location_group_t group);

    /**
     * @brief Get a relative location belonging to a group, by index
     * @param locations Handle obtained from dnv_vista_sdk_vis_locations
     * @param group Location group
     * @param index Zero-based index, must be < dnv_vista_sdk_locations_group_count(locations, group)
     * @return Borrowed pointer, valid as long as `locations` is valid, or NULL if
     *         `locations` is NULL or `index` is out of range
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_relative_location_t* dnv_vista_sdk_locations_group_at(
        const dnv_vista_sdk_locations_t* locations, dnv_vista_sdk_location_group_t group, size_t index);

    /**
     * @brief Parse a location string
     * @param locations Handle obtained from dnv_vista_sdk_vis_locations
     * @param locationStr Location string to parse (e.g. "1PS")
     * @return Owned handle, must be released with dnv_vista_sdk_location_free, or NULL
     *         if `locationStr` is invalid or `locations`/`locationStr` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_location_t* dnv_vista_sdk_locations_parse(
        const dnv_vista_sdk_locations_t* locations, const char* locationStr);

    /**
     * @brief Parse a location string, collecting detailed error information on failure
     * @param locations Handle obtained from dnv_vista_sdk_vis_locations
     * @param locationStr Location string to parse (e.g. "1PS")
     * @param outErrors Set to an owned ParsingErrors handle (must be released with
     *                  dnv_vista_sdk_parsing_errors_free), even on success (empty in
     *                  that case). Left untouched if `locations`/`locationStr`/`outErrors`
     *                  is NULL
     * @return Owned handle, must be released with dnv_vista_sdk_location_free, or NULL
     *         if `locationStr` is invalid or any argument is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_location_t* dnv_vista_sdk_locations_parse_with_errors(
        const dnv_vista_sdk_locations_t* locations,
        const char* locationStr,
        dnv_vista_sdk_parsing_errors_t** outErrors);

#ifdef __cplusplus
}
#endif
