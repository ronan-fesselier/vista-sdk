/**
 * @file location_group.h
 * @brief C API for dnv::vista::sdk::LocationGroup
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

    /** @brief Mirrors dnv::vista::sdk::LocationGroup - same underlying values */
    typedef enum
    {
        DNV_VISTA_SDK_LOCATION_GROUP_NUMBER = 0,
        DNV_VISTA_SDK_LOCATION_GROUP_SIDE,
        DNV_VISTA_SDK_LOCATION_GROUP_VERTICAL,
        DNV_VISTA_SDK_LOCATION_GROUP_TRANSVERSE,
        DNV_VISTA_SDK_LOCATION_GROUP_LONGITUDINAL
    } dnv_vista_sdk_location_group_t;

#ifdef __cplusplus
}
#endif
