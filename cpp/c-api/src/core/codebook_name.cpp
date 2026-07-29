#include "dnv/vista/sdk/c/core/codebook_name.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromCodebookName;
using dnv::vista::sdk::c::toCodebookName;

int dnv_vista_sdk_codebook_names_from_prefix(const char* prefix, dnv_vista_sdk_codebook_name_t* outName)
{
    if (prefix == nullptr || outName == nullptr)
    {
        c::setLastError("prefix and outName must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return c::cApiTryCatch<int>([&]() -> int {
        *outName = fromCodebookName(CodebookNames::fromPrefix(prefix));
        return 1;
    });
}

const char* dnv_vista_sdk_codebook_names_to_prefix(dnv_vista_sdk_codebook_name_t name)
{
    return c::cApiTryCatch<const char*>(
        [&]() -> const char* { return CodebookNames::toPrefix(toCodebookName(name)).data(); });
}

const char* dnv_vista_sdk_codebook_names_to_string(dnv_vista_sdk_codebook_name_t name)
{
    return c::cApiTryCatch<const char*>(
        [&]() -> const char* { return CodebookNames::toString(toCodebookName(name)).data(); });
}
