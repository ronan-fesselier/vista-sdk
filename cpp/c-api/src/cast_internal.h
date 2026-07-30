#pragma once

#include "dnv/vista/sdk/c/core/codebook_name.h"
#include "dnv/vista/sdk/c/core/codebook.h"
#include "dnv/vista/sdk/c/core/codebooks.h"
#include "dnv/vista/sdk/c/core/metadata_tag.h"
#include "dnv/vista/sdk/c/core/vis.h"

#include <dnv/VistaSDK.h>

#include <cstring>
#include <string>

namespace dnv::vista::sdk::c
{
    static_assert(static_cast<int>(CodebookName::Quantity) == DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);
    static_assert(static_cast<int>(CodebookName::Content) == DNV_VISTA_SDK_CODEBOOK_NAME_CONTENT);
    static_assert(static_cast<int>(CodebookName::Calculation) == DNV_VISTA_SDK_CODEBOOK_NAME_CALCULATION);
    static_assert(static_cast<int>(CodebookName::State) == DNV_VISTA_SDK_CODEBOOK_NAME_STATE);
    static_assert(static_cast<int>(CodebookName::Command) == DNV_VISTA_SDK_CODEBOOK_NAME_COMMAND);
    static_assert(static_cast<int>(CodebookName::Type) == DNV_VISTA_SDK_CODEBOOK_NAME_TYPE);
    static_assert(
        static_cast<int>(CodebookName::FunctionalServices) == DNV_VISTA_SDK_CODEBOOK_NAME_FUNCTIONAL_SERVICES);
    static_assert(
        static_cast<int>(CodebookName::MaintenanceCategory) == DNV_VISTA_SDK_CODEBOOK_NAME_MAINTENANCE_CATEGORY);
    static_assert(static_cast<int>(CodebookName::ActivityType) == DNV_VISTA_SDK_CODEBOOK_NAME_ACTIVITY_TYPE);
    static_assert(static_cast<int>(CodebookName::Position) == DNV_VISTA_SDK_CODEBOOK_NAME_POSITION);
    static_assert(static_cast<int>(CodebookName::Detail) == DNV_VISTA_SDK_CODEBOOK_NAME_DETAIL);

    inline char* toOwnedCString(const std::string& str)
    {
        char* result = new char[str.size() + 1];
        std::memcpy(result, str.c_str(), str.size() + 1);
        return result;
    }

    inline const VIS* toVis(const dnv_vista_sdk_vis_t* vis)
    {
        return reinterpret_cast<const VIS*>(vis);
    }

    inline CodebookName toCodebookName(dnv_vista_sdk_codebook_name_t name)
    {
        return static_cast<CodebookName>(name);
    }

    inline dnv_vista_sdk_codebook_name_t fromCodebookName(CodebookName name)
    {
        return static_cast<dnv_vista_sdk_codebook_name_t>(name);
    }

    inline const Codebook* toCodebook(const dnv_vista_sdk_codebook_t* codebook)
    {
        return reinterpret_cast<const Codebook*>(codebook);
    }

    inline const dnv_vista_sdk_codebook_t* fromCodebook(const Codebook* codebook)
    {
        return reinterpret_cast<const dnv_vista_sdk_codebook_t*>(codebook);
    }

    inline const Codebooks* toCodebooks(const dnv_vista_sdk_codebooks_t* codebooks)
    {
        return reinterpret_cast<const Codebooks*>(codebooks);
    }

    inline const MetadataTag* toTag(const dnv_vista_sdk_metadata_tag_t* tag)
    {
        return reinterpret_cast<const MetadataTag*>(tag);
    }
} // namespace dnv::vista::sdk::c
