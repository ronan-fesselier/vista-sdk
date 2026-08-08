/**
 * @file serializable_document.h
 * @brief C API for dnv::vista::sdk::transport::serialization::json::SerializableDocument
 * @details `dnv_vista_sdk_serializable_document_t*` is owned by value and must be
 *          released with dnv_vista_sdk_serializable_document_free. Array elements and
 *          object values are borrowed pointers into the owning document (via index/key
 *          accessors), never freed independently.
 *
 *          The templated `from<T>()`/`to<T>()` conversions and the ToSerializableDocumentTag/
 *          FromSerializableDocumentTag customization points are not exposed - they exist
 *          to adapt arbitrary C++ JSON library types via ADL/tag_invoke, which has no C
 *          equivalent. The set()/push_back()/find() API on this type already covers
 *          building and reading the same document tree from C
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_serializable_document dnv_vista_sdk_serializable_document_t;

    /** @brief Mirrors dnv::vista::sdk::transport::serialization::json::SerializableDocument::Kind */
    typedef enum
    {
        DNV_VISTA_SDK_SERIALIZABLE_DOCUMENT_KIND_NULL = 0,
        DNV_VISTA_SDK_SERIALIZABLE_DOCUMENT_KIND_BOOLEAN,
        DNV_VISTA_SDK_SERIALIZABLE_DOCUMENT_KIND_INTEGER,
        DNV_VISTA_SDK_SERIALIZABLE_DOCUMENT_KIND_DOUBLE,
        DNV_VISTA_SDK_SERIALIZABLE_DOCUMENT_KIND_STRING,
        DNV_VISTA_SDK_SERIALIZABLE_DOCUMENT_KIND_ARRAY,
        DNV_VISTA_SDK_SERIALIZABLE_DOCUMENT_KIND_OBJECT
    } dnv_vista_sdk_serializable_document_kind_t;

    /** @brief Construct a JSON null */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_null(void);
    /** @brief Construct a JSON boolean */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_from_boolean(
        int value);
    /** @brief Construct a JSON integer */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_from_integer(
        int64_t value);
    /** @brief Construct a JSON floating point number */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_from_double(
        double value);
    /**
     * @brief Construct a JSON string
     * @return Owned handle, or NULL if `value` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_from_string(
        const char* value);
    /** @brief Construct an empty JSON array */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_array(void);
    /** @brief Construct an empty JSON object */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_object(void);

    /**
     * @brief Release a SerializableDocument handle
     * @details Releasing a document also releases any array elements/object values
     *          appended or set into it - do not separately free values obtained
     *          from dnv_vista_sdk_serializable_document_at/find
     * @param doc Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_serializable_document_free(dnv_vista_sdk_serializable_document_t* doc);

    /**
     * @brief Deep-copy a SerializableDocument handle
     * @return Owned handle, must be released with dnv_vista_sdk_serializable_document_free,
     *         or NULL if `doc` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_clone(
        const dnv_vista_sdk_serializable_document_t* doc);

    /**
     * @brief Compare two SerializableDocument handles for structural equality
     * @return 1 if both represent the same value tree, 0 otherwise or if either is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_serializable_document_equals(
        const dnv_vista_sdk_serializable_document_t* a, const dnv_vista_sdk_serializable_document_t* b);

    /** @brief Get the value kind, or DNV_VISTA_SDK_SERIALIZABLE_DOCUMENT_KIND_NULL if `doc` is NULL */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_serializable_document_kind_t
    dnv_vista_sdk_serializable_document_kind(const dnv_vista_sdk_serializable_document_t* doc);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_serializable_document_is_null(
        const dnv_vista_sdk_serializable_document_t* doc);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_serializable_document_is_boolean(
        const dnv_vista_sdk_serializable_document_t* doc);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_serializable_document_is_integer(
        const dnv_vista_sdk_serializable_document_t* doc);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_serializable_document_is_double(
        const dnv_vista_sdk_serializable_document_t* doc);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_serializable_document_is_string(
        const dnv_vista_sdk_serializable_document_t* doc);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_serializable_document_is_array(
        const dnv_vista_sdk_serializable_document_t* doc);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_serializable_document_is_object(
        const dnv_vista_sdk_serializable_document_t* doc);

    /**
     * @brief Get the boolean value
     * @details Sets the last error message and returns 0 if `doc` is not a boolean (the
     *          C++ method throws std::bad_variant_access, caught at the ABI boundary)
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_serializable_document_as_boolean(
        const dnv_vista_sdk_serializable_document_t* doc);
    /** @brief Get the integer value; sets the last error message and returns 0 if not an integer */
    DNV_VISTA_SDK_C_API int64_t
    dnv_vista_sdk_serializable_document_as_integer(const dnv_vista_sdk_serializable_document_t* doc);
    /** @brief Get the double value; sets the last error message and returns 0.0 if not a double */
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_serializable_document_as_double(
        const dnv_vista_sdk_serializable_document_t* doc);
    /**
     * @brief Get the string value
     * @return Null-terminated string, owned by `doc` - valid as long as `doc` is valid,
     *         or NULL (with the last error message set) if `doc` is not a string
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_serializable_document_as_string(
        const dnv_vista_sdk_serializable_document_t* doc);

    /**
     * @brief Get the number of elements in this array
     * @return Element count, or 0 if `doc` is NULL or not an array
     */
    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_serializable_document_array_size(const dnv_vista_sdk_serializable_document_t* doc);

    /**
     * @brief Get an array element by index
     * @param doc Handle obtained from this API, must be an array
     * @param index Zero-based index, must be < dnv_vista_sdk_serializable_document_array_size(doc)
     * @return Borrowed pointer, valid as long as `doc` is valid, or NULL if `doc` is
     *         NULL, not an array, or `index` is out of range
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_array_at(
        const dnv_vista_sdk_serializable_document_t* doc, size_t index);

    /**
     * @brief Get the number of key/value pairs in this object
     * @return Entry count, or 0 if `doc` is NULL or not an object
     */
    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_serializable_document_object_size(const dnv_vista_sdk_serializable_document_t* doc);

    /**
     * @brief Get an object entry's key by insertion-order index
     * @return Null-terminated string, owned by `doc` - valid as long as `doc` is valid,
     *         or NULL if `doc` is NULL, not an object, or `index` is out of range
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_serializable_document_object_key_at(
        const dnv_vista_sdk_serializable_document_t* doc, size_t index);

    /**
     * @brief Get an object entry's value by insertion-order index
     * @return Borrowed pointer, valid as long as `doc` is valid, or NULL if `doc` is
     *         NULL, not an object, or `index` is out of range
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_serializable_document_t*
    dnv_vista_sdk_serializable_document_object_value_at(const dnv_vista_sdk_serializable_document_t* doc, size_t index);

    /**
     * @brief Look up a key in this object
     * @return Borrowed pointer, valid as long as `doc` is valid, or NULL if `doc` is
     *         NULL, not an object, or `key` is not present
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_find(
        const dnv_vista_sdk_serializable_document_t* doc, const char* key);

    /**
     * @brief Check whether this object contains a key
     * @return 1 if `doc` is an object and contains `key`, 0 otherwise
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_serializable_document_contains(
        const dnv_vista_sdk_serializable_document_t* doc, const char* key);

    /**
     * @brief Set a key/value pair on this object, preserving insertion order
     * @details If `doc` is not yet an object, it is reset to an empty object first. If
     *          `key` already exists, its value is replaced in place. Takes ownership of
     *          `value` - do not free it separately after this call
     * @param doc Handle obtained from this API, must not be NULL
     * @param key Key to set, must not be NULL
     * @param value Handle to take ownership of, must not be NULL
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_serializable_document_set(
        dnv_vista_sdk_serializable_document_t* doc, const char* key, dnv_vista_sdk_serializable_document_t* value);

    /**
     * @brief Append a value to this array
     * @details If `doc` is not yet an array, it is reset to an empty array first. Takes
     *          ownership of `value` - do not free it separately after this call
     * @param doc Handle obtained from this API, must not be NULL
     * @param value Handle to take ownership of, must not be NULL
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_serializable_document_push_back(
        dnv_vista_sdk_serializable_document_t* doc, dnv_vista_sdk_serializable_document_t* value);

#ifdef __cplusplus
}
#endif
