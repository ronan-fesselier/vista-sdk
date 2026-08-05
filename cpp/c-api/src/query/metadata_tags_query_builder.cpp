#include "dnv/vista/sdk/c/query/metadata_tags_query_builder.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromMetadataTagsQueryBuilder;
using dnv::vista::sdk::c::fromMetadataTagsQueryValue;
using dnv::vista::sdk::c::toCodebookName;
using dnv::vista::sdk::c::toLocalId;
using dnv::vista::sdk::c::toMetadataTagsQueryBuilder;
using dnv::vista::sdk::c::toTag;

dnv_vista_sdk_metadata_tags_query_builder_t* dnv_vista_sdk_metadata_tags_query_builder_create(void)
{
    return fromMetadataTagsQueryBuilder(MetadataTagsQueryBuilder::create());
}

dnv_vista_sdk_metadata_tags_query_builder_t* dnv_vista_sdk_metadata_tags_query_builder_from(
    const dnv_vista_sdk_local_id_t* localId, int allowOtherTags)
{
    if (localId == nullptr)
    {
        c::setLastError("localId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromMetadataTagsQueryBuilder(MetadataTagsQueryBuilder::from(*toLocalId(localId), allowOtherTags != 0));
}

void dnv_vista_sdk_metadata_tags_query_builder_free(dnv_vista_sdk_metadata_tags_query_builder_t* builder)
{
    delete reinterpret_cast<MetadataTagsQueryBuilder*>(builder);
}

dnv_vista_sdk_metadata_tags_query_builder_t* dnv_vista_sdk_metadata_tags_query_builder_with_tag(
    const dnv_vista_sdk_metadata_tags_query_builder_t* builder, dnv_vista_sdk_codebook_name_t name, const char* value)
{
    if (builder == nullptr || value == nullptr)
    {
        c::setLastError("builder and value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromMetadataTagsQueryBuilder(toMetadataTagsQueryBuilder(builder)->withTag(toCodebookName(name), value));
}

dnv_vista_sdk_metadata_tags_query_builder_t* dnv_vista_sdk_metadata_tags_query_builder_with_metadata_tag(
    const dnv_vista_sdk_metadata_tags_query_builder_t* builder, const dnv_vista_sdk_metadata_tag_t* tag)
{
    if (builder == nullptr || tag == nullptr)
    {
        c::setLastError("builder and tag must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromMetadataTagsQueryBuilder(toMetadataTagsQueryBuilder(builder)->withTag(*toTag(tag)));
}

dnv_vista_sdk_metadata_tags_query_builder_t* dnv_vista_sdk_metadata_tags_query_builder_with_allow_other_tags(
    const dnv_vista_sdk_metadata_tags_query_builder_t* builder, int allowOthers)
{
    if (builder == nullptr)
    {
        c::setLastError("builder must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromMetadataTagsQueryBuilder(toMetadataTagsQueryBuilder(builder)->withAllowOtherTags(allowOthers != 0));
}

dnv_vista_sdk_metadata_tags_query_t* dnv_vista_sdk_metadata_tags_query_builder_build(
    const dnv_vista_sdk_metadata_tags_query_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastError("builder must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromMetadataTagsQueryValue(toMetadataTagsQueryBuilder(builder)->build());
}
