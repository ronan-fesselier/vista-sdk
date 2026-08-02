/**
 * @file gmod_node_metadata.h
 * @brief C API for dnv::vista::sdk::GmodNodeMetadata
 * @details `dnv_vista_sdk_gmod_node_metadata_t*` is a borrowed pointer, owned by the
 *          parent GmodNode - never freed independently
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_gmod_node_metadata dnv_vista_sdk_gmod_node_metadata_t;

    /**
     * @brief Get the node category
     * @param metadata Handle obtained from dnv_vista_sdk_gmod_node_metadata
     * @return Null-terminated string (e.g. "ASSET FUNCTION"), owned by `metadata` -
     *         valid as long as `metadata` is valid, or NULL if `metadata` is NULL
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_gmod_node_metadata_category(
        const dnv_vista_sdk_gmod_node_metadata_t* metadata);

    /**
     * @brief Get the node type
     * @param metadata Handle obtained from dnv_vista_sdk_gmod_node_metadata
     * @return Null-terminated string (e.g. "COMPOSITION"), owned by `metadata` - valid
     *         as long as `metadata` is valid, or NULL if `metadata` is NULL
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_gmod_node_metadata_type(
        const dnv_vista_sdk_gmod_node_metadata_t* metadata);

    /**
     * @brief Get the full type ("Category Type")
     * @param metadata Handle obtained from dnv_vista_sdk_gmod_node_metadata
     * @return Null-terminated string, owned by `metadata` - valid as long as
     *         `metadata` is valid, or NULL if `metadata` is NULL
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_gmod_node_metadata_full_type(
        const dnv_vista_sdk_gmod_node_metadata_t* metadata);

    /**
     * @brief Get the node name
     * @param metadata Handle obtained from dnv_vista_sdk_gmod_node_metadata
     * @return Null-terminated string, owned by `metadata` - valid as long as
     *         `metadata` is valid, or NULL if `metadata` is NULL
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_gmod_node_metadata_name(
        const dnv_vista_sdk_gmod_node_metadata_t* metadata);

    /**
     * @brief Get the common/friendly name
     * @param metadata Handle obtained from dnv_vista_sdk_gmod_node_metadata
     * @return Null-terminated string, owned by `metadata` - valid as long as
     *         `metadata` is valid, or NULL if `metadata` is NULL or not set
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_gmod_node_metadata_common_name(
        const dnv_vista_sdk_gmod_node_metadata_t* metadata);

    /**
     * @brief Get the technical definition
     * @param metadata Handle obtained from dnv_vista_sdk_gmod_node_metadata
     * @return Null-terminated string, owned by `metadata` - valid as long as
     *         `metadata` is valid, or NULL if `metadata` is NULL or not set
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_gmod_node_metadata_definition(
        const dnv_vista_sdk_gmod_node_metadata_t* metadata);

    /**
     * @brief Get the user-friendly definition
     * @param metadata Handle obtained from dnv_vista_sdk_gmod_node_metadata
     * @return Null-terminated string, owned by `metadata` - valid as long as
     *         `metadata` is valid, or NULL if `metadata` is NULL or not set
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_gmod_node_metadata_common_definition(
        const dnv_vista_sdk_gmod_node_metadata_t* metadata);

    /**
     * @brief Get the installation substructure flag
     * @param metadata Handle obtained from dnv_vista_sdk_gmod_node_metadata
     * @param outValue Set to the flag value on success
     * @return 1 if the flag is set, 0 if unset or `metadata`/`outValue` is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_gmod_node_metadata_install_substructure(
        const dnv_vista_sdk_gmod_node_metadata_t* metadata, int* outValue);

    /**
     * @brief Get the number of normal assignment name entries
     * @param metadata Handle obtained from dnv_vista_sdk_gmod_node_metadata
     * @return Entry count, or 0 if `metadata` is NULL
     */
    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_count(const dnv_vista_sdk_gmod_node_metadata_t* metadata);

    /**
     * @brief Get a normal assignment name entry's key by index
     * @param metadata Handle obtained from dnv_vista_sdk_gmod_node_metadata
     * @param index Zero-based index, must be <
     *              dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_count(metadata)
     * @return Null-terminated string, owned by `metadata` - valid as long as
     *         `metadata` is valid, or NULL if `metadata` is NULL or `index` is out of range
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_key_at(
        const dnv_vista_sdk_gmod_node_metadata_t* metadata, size_t index);

    /**
     * @brief Get a normal assignment name entry's value by index
     * @param metadata Handle obtained from dnv_vista_sdk_gmod_node_metadata
     * @param index Zero-based index, must be <
     *              dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_count(metadata)
     * @return Null-terminated string, owned by `metadata` - valid as long as
     *         `metadata` is valid, or NULL if `metadata` is NULL or `index` is out of range
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_value_at(
        const dnv_vista_sdk_gmod_node_metadata_t* metadata, size_t index);

#ifdef __cplusplus
}
#endif
