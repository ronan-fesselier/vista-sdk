#include "dnv/vista/sdk/c/query/metadata_tags_query.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromMetadataTagsQueryBuilderRef;
using dnv::vista::sdk::c::toLocalId;
using dnv::vista::sdk::c::toMetadataTagsQuery;

void dnv_vista_sdk_metadata_tags_query_free(dnv_vista_sdk_metadata_tags_query_t* query)
{
    delete reinterpret_cast<MetadataTagsQuery*>(query);
}

int dnv_vista_sdk_metadata_tags_query_match(
    const dnv_vista_sdk_metadata_tags_query_t* query, const dnv_vista_sdk_local_id_t* localId)
{
    if (query == nullptr || localId == nullptr)
    {
        c::setLastErrorMessage("query and localId must not be null");
        return 0;
    }

    return toMetadataTagsQuery(query)->match(*toLocalId(localId)) ? 1 : 0;
}

const dnv_vista_sdk_metadata_tags_query_builder_t* dnv_vista_sdk_metadata_tags_query_builder(
    const dnv_vista_sdk_metadata_tags_query_t* query)
{
    if (query == nullptr)
    {
        c::setLastErrorMessage("query must not be null");
        return nullptr;
    }

    return fromMetadataTagsQueryBuilderRef(toMetadataTagsQuery(query)->builder());
}
