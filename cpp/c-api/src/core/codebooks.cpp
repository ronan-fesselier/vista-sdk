#include "dnv/vista/sdk/c/core/codebooks.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromCodebook;
using dnv::vista::sdk::c::toCodebooks;

const dnv_vista_sdk_codebook_t* dnv_vista_sdk_codebooks_at(
    const dnv_vista_sdk_codebooks_t* codebooks, dnv_vista_sdk_codebook_name_t name)
{
    if (codebooks == nullptr)
    {
        c::setLastErrorMessage("codebooks must not be null");
        return nullptr;
    }

    try
    {
        return fromCodebook(&(*toCodebooks(codebooks))[static_cast<CodebookName>(name)]);
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}
