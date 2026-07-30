#include "dnv/vista/sdk/c/core/metadata_tag.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::toOwnedCString;
using dnv::vista::sdk::c::toTag;

void dnv_vista_sdk_metadata_tag_free(dnv_vista_sdk_metadata_tag_t* tag)
{
    delete reinterpret_cast<MetadataTag*>(tag);
}

dnv_vista_sdk_codebook_name_t dnv_vista_sdk_metadata_tag_name(const dnv_vista_sdk_metadata_tag_t* tag)
{
    if (tag == nullptr)
    {
        c::setLastError("tag must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return static_cast<dnv_vista_sdk_codebook_name_t>(0);
    }

    return static_cast<dnv_vista_sdk_codebook_name_t>(toTag(tag)->name());
}

const char* dnv_vista_sdk_metadata_tag_value(const dnv_vista_sdk_metadata_tag_t* tag)
{
    if (tag == nullptr)
    {
        c::setLastError("tag must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toTag(tag)->value().c_str();
}

int dnv_vista_sdk_metadata_tag_is_custom(const dnv_vista_sdk_metadata_tag_t* tag)
{
    if (tag == nullptr)
    {
        c::setLastError("tag must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toTag(tag)->isCustom() ? 1 : 0;
}

char* dnv_vista_sdk_metadata_tag_to_string(const dnv_vista_sdk_metadata_tag_t* tag)
{
    if (tag == nullptr)
    {
        c::setLastError("tag must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toOwnedCString(toTag(tag)->toString());
}
