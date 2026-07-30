#include "dnv/vista/sdk/c/core/locations.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromLocation;
using dnv::vista::sdk::c::fromRelativeLocation;
using dnv::vista::sdk::c::toLocationGroup;
using dnv::vista::sdk::c::toLocations;

const char* dnv_vista_sdk_locations_version(const dnv_vista_sdk_locations_t* locations)
{
    if (locations == nullptr)
    {
        c::setLastErrorMessage("locations must not be null");
        return nullptr;
    }

    return VisVersions::toString(toLocations(locations)->version()).data();
}

size_t dnv_vista_sdk_locations_relative_location_count(const dnv_vista_sdk_locations_t* locations)
{
    if (locations == nullptr)
    {
        c::setLastErrorMessage("locations must not be null");
        return 0;
    }

    return toLocations(locations)->relativeLocations().size();
}

const dnv_vista_sdk_relative_location_t* dnv_vista_sdk_locations_relative_location_at(
    const dnv_vista_sdk_locations_t* locations, size_t index)
{
    if (locations == nullptr)
    {
        c::setLastErrorMessage("locations must not be null");
        return nullptr;
    }

    const auto& relativeLocations = toLocations(locations)->relativeLocations();
    if (index >= relativeLocations.size())
    {
        c::setLastErrorMessage("index out of range");
        return nullptr;
    }

    return fromRelativeLocation(relativeLocations[index]);
}

size_t dnv_vista_sdk_locations_group_count(
    const dnv_vista_sdk_locations_t* locations, dnv_vista_sdk_location_group_t group)
{
    if (locations == nullptr)
    {
        c::setLastErrorMessage("locations must not be null");
        return 0;
    }

    const auto& groups = toLocations(locations)->groups();
    const auto it = groups.find(toLocationGroup(group));
    if (it == groups.end())
    {
        return 0;
    }

    return it->second.size();
}

const dnv_vista_sdk_relative_location_t* dnv_vista_sdk_locations_group_at(
    const dnv_vista_sdk_locations_t* locations, dnv_vista_sdk_location_group_t group, size_t index)
{
    if (locations == nullptr)
    {
        c::setLastErrorMessage("locations must not be null");
        return nullptr;
    }

    const auto& groups = toLocations(locations)->groups();
    const auto it = groups.find(toLocationGroup(group));
    if (it == groups.end() || index >= it->second.size())
    {
        c::setLastErrorMessage("index out of range");
        return nullptr;
    }

    return fromRelativeLocation(it->second[index]);
}

dnv_vista_sdk_location_t* dnv_vista_sdk_locations_parse(
    const dnv_vista_sdk_locations_t* locations, const char* locationStr)
{
    if (locations == nullptr || locationStr == nullptr)
    {
        c::setLastErrorMessage("locations and locationStr must not be null");
        return nullptr;
    }

    auto location = toLocations(locations)->fromString(locationStr);
    if (!location.has_value())
    {
        c::setLastErrorMessage("invalid location string");
    }

    return fromLocation(std::move(location));
}

dnv_vista_sdk_location_t* dnv_vista_sdk_locations_parse_with_errors(
    const dnv_vista_sdk_locations_t* locations, const char* locationStr, dnv_vista_sdk_parsing_errors_t** outErrors)
{
    if (locations == nullptr || locationStr == nullptr || outErrors == nullptr)
    {
        c::setLastErrorMessage("locations, locationStr and outErrors must not be null");
        return nullptr;
    }

    ParsingErrors errors;
    auto location = toLocations(locations)->fromString(locationStr, errors);

    *outErrors = reinterpret_cast<dnv_vista_sdk_parsing_errors_t*>(new ParsingErrors{ std::move(errors) });

    if (!location.has_value())
    {
        c::setLastErrorMessage("invalid location string");
    }

    return fromLocation(std::move(location));
}
