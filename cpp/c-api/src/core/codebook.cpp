#include "dnv/vista/sdk/c/core/codebook.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::toCodebook;

dnv_vista_sdk_codebook_name_t dnv_vista_sdk_codebook_name(const dnv_vista_sdk_codebook_t* codebook)
{
    if (codebook == nullptr)
    {
        c::setLastError("codebook must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return static_cast<dnv_vista_sdk_codebook_name_t>(0);
    }

    return static_cast<dnv_vista_sdk_codebook_name_t>(toCodebook(codebook)->name());
}

size_t dnv_vista_sdk_codebook_standard_values_count(const dnv_vista_sdk_codebook_t* codebook)
{
    if (codebook == nullptr)
    {
        c::setLastError("codebook must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toCodebook(codebook)->standardValues().size();
}

const char* dnv_vista_sdk_codebook_standard_value_at(const dnv_vista_sdk_codebook_t* codebook, size_t index)
{
    if (codebook == nullptr)
    {
        c::setLastError("codebook must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& values = toCodebook(codebook)->standardValues();
    if (index >= values.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    auto it = values.begin();
    std::advance(it, index);
    return it->c_str();
}

size_t dnv_vista_sdk_codebook_groups_count(const dnv_vista_sdk_codebook_t* codebook)
{
    if (codebook == nullptr)
    {
        c::setLastError("codebook must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toCodebook(codebook)->groups().size();
}

const char* dnv_vista_sdk_codebook_group_at(const dnv_vista_sdk_codebook_t* codebook, size_t index)
{
    if (codebook == nullptr)
    {
        c::setLastError("codebook must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& groups = toCodebook(codebook)->groups();
    if (index >= groups.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    auto it = groups.begin();
    std::advance(it, index);
    return it->c_str();
}

int dnv_vista_sdk_codebook_has_group(const dnv_vista_sdk_codebook_t* codebook, const char* group)
{
    if (codebook == nullptr || group == nullptr)
    {
        c::setLastError("codebook and group must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toCodebook(codebook)->hasGroup(group) ? 1 : 0;
}

dnv_vista_sdk_position_validation_result_t dnv_vista_sdk_codebook_validate_position(
    const dnv_vista_sdk_codebook_t* codebook, const char* position)
{
    if (codebook == nullptr || position == nullptr)
    {
        c::setLastError("codebook and position must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return DNV_VISTA_SDK_POSITION_VALIDATION_INVALID;
    }

    return static_cast<dnv_vista_sdk_position_validation_result_t>(toCodebook(codebook)->validatePosition(position));
}

int dnv_vista_sdk_codebook_has_standard_value(const dnv_vista_sdk_codebook_t* codebook, const char* value)
{
    if (codebook == nullptr || value == nullptr)
    {
        c::setLastError("codebook and value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toCodebook(codebook)->hasStandardValue(value) ? 1 : 0;
}

dnv_vista_sdk_metadata_tag_t* dnv_vista_sdk_codebook_create_tag(
    const dnv_vista_sdk_codebook_t* codebook, const char* value)
{
    if (codebook == nullptr || value == nullptr)
    {
        c::setLastError("codebook and value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    auto tag = toCodebook(codebook)->createTag(value);
    if (!tag.has_value())
    {
        c::setLastError("invalid tag value", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return reinterpret_cast<dnv_vista_sdk_metadata_tag_t*>(new MetadataTag{ std::move(*tag) });
}
