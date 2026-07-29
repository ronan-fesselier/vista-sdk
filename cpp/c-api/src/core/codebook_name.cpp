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
        c::setLastErrorMessage("prefix and outName must not be null");
        return 0;
    }

    try
    {
        *outName = fromCodebookName(CodebookNames::fromPrefix(prefix));
        return 1;
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return 0;
    }
}

const char* dnv_vista_sdk_codebook_names_to_prefix(dnv_vista_sdk_codebook_name_t name)
{
    try
    {
        return CodebookNames::toPrefix(toCodebookName(name)).data();
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

const char* dnv_vista_sdk_codebook_names_to_string(dnv_vista_sdk_codebook_name_t name)
{
    try
    {
        return CodebookNames::toString(toCodebookName(name)).data();
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}
