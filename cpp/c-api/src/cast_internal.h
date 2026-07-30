#pragma once

#include "dnv/vista/sdk/c/core/codebook_name.h"
#include "dnv/vista/sdk/c/core/codebook.h"
#include "dnv/vista/sdk/c/core/codebooks.h"
#include "dnv/vista/sdk/c/core/location.h"
#include "dnv/vista/sdk/c/core/location_builder.h"
#include "dnv/vista/sdk/c/core/locations.h"
#include "dnv/vista/sdk/c/core/metadata_tag.h"
#include "dnv/vista/sdk/c/core/parsing_errors.h"
#include "dnv/vista/sdk/c/core/relative_location.h"
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

    static_assert(static_cast<int>(LocationGroup::Number) == DNV_VISTA_SDK_LOCATION_GROUP_NUMBER);
    static_assert(static_cast<int>(LocationGroup::Side) == DNV_VISTA_SDK_LOCATION_GROUP_SIDE);
    static_assert(static_cast<int>(LocationGroup::Vertical) == DNV_VISTA_SDK_LOCATION_GROUP_VERTICAL);
    static_assert(static_cast<int>(LocationGroup::Transverse) == DNV_VISTA_SDK_LOCATION_GROUP_TRANSVERSE);
    static_assert(static_cast<int>(LocationGroup::Longitudinal) == DNV_VISTA_SDK_LOCATION_GROUP_LONGITUDINAL);

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

    inline const ParsingErrors* toErrors(const dnv_vista_sdk_parsing_errors_t* errors)
    {
        return reinterpret_cast<const ParsingErrors*>(errors);
    }

    inline const Location* toLocation(const dnv_vista_sdk_location_t* location)
    {
        return reinterpret_cast<const Location*>(location);
    }

    inline const Locations* toLocations(const dnv_vista_sdk_locations_t* locations)
    {
        return reinterpret_cast<const Locations*>(locations);
    }

    inline dnv_vista_sdk_location_t* fromLocation(std::optional<Location>&& location)
    {
        if (!location.has_value())
        {
            return nullptr;
        }

        return reinterpret_cast<dnv_vista_sdk_location_t*>(new Location{ std::move(*location) });
    }

    inline LocationGroup toLocationGroup(dnv_vista_sdk_location_group_t group)
    {
        return static_cast<LocationGroup>(group);
    }

    inline const dnv_vista_sdk_location_t* fromLocationRef(const Location& location)
    {
        return reinterpret_cast<const dnv_vista_sdk_location_t*>(&location);
    }

    inline const RelativeLocation* toRelativeLocation(const dnv_vista_sdk_relative_location_t* relativeLocation)
    {
        return reinterpret_cast<const RelativeLocation*>(relativeLocation);
    }

    inline const dnv_vista_sdk_relative_location_t* fromRelativeLocation(const RelativeLocation& relativeLocation)
    {
        return reinterpret_cast<const dnv_vista_sdk_relative_location_t*>(&relativeLocation);
    }

    inline const LocationBuilder* toBuilder(const dnv_vista_sdk_location_builder_t* builder)
    {
        return reinterpret_cast<const LocationBuilder*>(builder);
    }

    inline dnv_vista_sdk_location_builder_t* fromBuilder(LocationBuilder&& builder)
    {
        return reinterpret_cast<dnv_vista_sdk_location_builder_t*>(new LocationBuilder{ std::move(builder) });
    }
} // namespace dnv::vista::sdk::c
