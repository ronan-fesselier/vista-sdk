#include "dnv/vista/sdk/c/query/local_id_query.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::toLocalId;
using dnv::vista::sdk::c::toLocalIdQuery;

void dnv_vista_sdk_local_id_query_free(dnv_vista_sdk_local_id_query_t* query)
{
    delete reinterpret_cast<LocalIdQuery*>(query);
}

int dnv_vista_sdk_local_id_query_match(
    const dnv_vista_sdk_local_id_query_t* query, const dnv_vista_sdk_local_id_t* localId)
{
    if (query == nullptr || localId == nullptr)
    {
        c::setLastError("query and localId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toLocalIdQuery(query)->match(*toLocalId(localId)) ? 1 : 0;
}

int dnv_vista_sdk_local_id_query_match_string(const dnv_vista_sdk_local_id_query_t* query, const char* localIdStr)
{
    if (query == nullptr || localIdStr == nullptr)
    {
        c::setLastError("query and localIdStr must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toLocalIdQuery(query)->match(localIdStr) ? 1 : 0;
}
