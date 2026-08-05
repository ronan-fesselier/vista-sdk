#include "dnv/vista/sdk/c/query/gmod_path_query.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::toGmodPath;
using dnv::vista::sdk::c::toGmodPathQuery;

void dnv_vista_sdk_gmod_path_query_free(dnv_vista_sdk_gmod_path_query_t* query)
{
    delete reinterpret_cast<GmodPathQuery*>(query);
}

int dnv_vista_sdk_gmod_path_query_match(
    const dnv_vista_sdk_gmod_path_query_t* query, const dnv_vista_sdk_gmod_path_t* path)
{
    if (query == nullptr || path == nullptr)
    {
        c::setLastErrorMessage("query and path must not be null");
        return 0;
    }

    return toGmodPathQuery(query)->match(*toGmodPath(path)) ? 1 : 0;
}
