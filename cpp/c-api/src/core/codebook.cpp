#include "dnv/vista/sdk/c/core/codebook.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::toCodebook;

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
