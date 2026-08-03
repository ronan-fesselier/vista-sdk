#include "dnv/vista/sdk/c/core/gmod_path.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromGmodNode;
using dnv::vista::sdk::c::fromGmodPath;
using dnv::vista::sdk::c::fromGmodPathValue;
using dnv::vista::sdk::c::toGmod;
using dnv::vista::sdk::c::toGmodPath;
using dnv::vista::sdk::c::toIndividualizableSet;
using dnv::vista::sdk::c::toLocations;
using dnv::vista::sdk::c::toOwnedCString;

void dnv_vista_sdk_gmod_path_free(dnv_vista_sdk_gmod_path_t* path)
{
    delete reinterpret_cast<GmodPath*>(path);
}

dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_gmod_path_from_short_path_version(const char* item, const char* visVersion)
{
    if (item == nullptr || visVersion == nullptr)
    {
        c::setLastError("item and visVersion must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto version = VisVersions::fromString(visVersion);
    if (!version.has_value())
    {
        c::setLastError("unrecognized VIS version", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    auto path = GmodPath::fromShortPath(item, *version);
    if (!path.has_value())
    {
        c::setLastError("invalid path string", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
    }

    return fromGmodPath(std::move(path));
}

dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_gmod_path_from_short_path(
    const char* item, const dnv_vista_sdk_gmod_t* gmod, const dnv_vista_sdk_locations_t* locations)
{
    if (item == nullptr || gmod == nullptr || locations == nullptr)
    {
        c::setLastError("item, gmod and locations must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    auto path = GmodPath::fromShortPath(item, *toGmod(gmod), *toLocations(locations));
    if (!path.has_value())
    {
        c::setLastError("invalid path string", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
    }

    return fromGmodPath(std::move(path));
}

dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_gmod_path_from_short_path_with_errors(
    const char* item,
    const dnv_vista_sdk_gmod_t* gmod,
    const dnv_vista_sdk_locations_t* locations,
    dnv_vista_sdk_parsing_errors_t** outErrors)
{
    if (item == nullptr || gmod == nullptr || locations == nullptr || outErrors == nullptr)
    {
        c::setLastError("item, gmod, locations and outErrors must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    ParsingErrors errors;
    auto path = GmodPath::fromShortPath(item, *toGmod(gmod), *toLocations(locations), errors);

    *outErrors = reinterpret_cast<dnv_vista_sdk_parsing_errors_t*>(new ParsingErrors{ std::move(errors) });

    if (!path.has_value())
    {
        c::setLastError("invalid path string", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
    }

    return fromGmodPath(std::move(path));
}

dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_gmod_path_from_full_path(
    const char* fullPathStr, const dnv_vista_sdk_gmod_t* gmod, const dnv_vista_sdk_locations_t* locations)
{
    if (fullPathStr == nullptr || gmod == nullptr || locations == nullptr)
    {
        c::setLastError("fullPathStr, gmod and locations must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    auto path = GmodPath::fromFullPath(fullPathStr, *toGmod(gmod), *toLocations(locations));
    if (!path.has_value())
    {
        c::setLastError("invalid path string", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
    }

    return fromGmodPath(std::move(path));
}

dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_gmod_path_from_full_path_with_errors(
    const char* fullPathStr,
    const dnv_vista_sdk_gmod_t* gmod,
    const dnv_vista_sdk_locations_t* locations,
    dnv_vista_sdk_parsing_errors_t** outErrors)
{
    if (fullPathStr == nullptr || gmod == nullptr || locations == nullptr || outErrors == nullptr)
    {
        c::setLastError(
            "fullPathStr, gmod, locations and outErrors must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    ParsingErrors errors;
    auto path = GmodPath::fromFullPath(fullPathStr, *toGmod(gmod), *toLocations(locations), errors);

    *outErrors = reinterpret_cast<dnv_vista_sdk_parsing_errors_t*>(new ParsingErrors{ std::move(errors) });

    if (!path.has_value())
    {
        c::setLastError("invalid path string", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
    }

    return fromGmodPath(std::move(path));
}

const char* dnv_vista_sdk_gmod_path_version(const dnv_vista_sdk_gmod_path_t* path)
{
    if (path == nullptr)
    {
        c::setLastError("path must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return VisVersions::toString(toGmodPath(path)->version()).data();
}

const dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_path_node(const dnv_vista_sdk_gmod_path_t* path)
{
    if (path == nullptr)
    {
        c::setLastError("path must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromGmodNode(&toGmodPath(path)->node());
}

size_t dnv_vista_sdk_gmod_path_length(const dnv_vista_sdk_gmod_path_t* path)
{
    if (path == nullptr)
    {
        c::setLastError("path must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toGmodPath(path)->length();
}

const dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_path_at(const dnv_vista_sdk_gmod_path_t* path, size_t index)
{
    if (path == nullptr)
    {
        c::setLastError("path must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto* self = toGmodPath(path);
    if (index >= self->length())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return fromGmodNode(&(*self)[index]);
}

int dnv_vista_sdk_gmod_path_is_mappable(const dnv_vista_sdk_gmod_path_t* path)
{
    if (path == nullptr)
    {
        c::setLastError("path must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toGmodPath(path)->isMappable() ? 1 : 0;
}

int dnv_vista_sdk_gmod_path_is_individualizable(const dnv_vista_sdk_gmod_path_t* path)
{
    if (path == nullptr)
    {
        c::setLastError("path must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toGmodPath(path)->isIndividualizable() ? 1 : 0;
}

dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_gmod_path_without_locations(const dnv_vista_sdk_gmod_path_t* path)
{
    if (path == nullptr)
    {
        c::setLastError("path must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromGmodPathValue(toGmodPath(path)->withoutLocations());
}

char* dnv_vista_sdk_gmod_path_normal_assignment_name(const dnv_vista_sdk_gmod_path_t* path, size_t nodeDepth)
{
    if (path == nullptr)
    {
        c::setLastError("path must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    auto name = toGmodPath(path)->normalAssignmentName(nodeDepth);
    if (!name.has_value())
    {
        c::setLastError("no normal assignment name at this depth", DNV_VISTA_SDK_ERROR_DOMAIN);
        return nullptr;
    }

    return toOwnedCString(*name);
}

size_t dnv_vista_sdk_gmod_path_individualizable_set_count(const dnv_vista_sdk_gmod_path_t* path)
{
    if (path == nullptr)
    {
        c::setLastError("path must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toGmodPath(path)->individualizableSets().size();
}

dnv_vista_sdk_gmod_individualizable_set_t* dnv_vista_sdk_gmod_path_individualizable_set_at(
    const dnv_vista_sdk_gmod_path_t* path, size_t index)
{
    if (path == nullptr)
    {
        c::setLastError("path must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    auto sets = toGmodPath(path)->individualizableSets();
    if (index >= sets.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return reinterpret_cast<dnv_vista_sdk_gmod_individualizable_set_t*>(
        new GmodIndividualizableSet{ std::move(sets[index]) });
}

size_t dnv_vista_sdk_gmod_path_common_name_count(const dnv_vista_sdk_gmod_path_t* path)
{
    if (path == nullptr)
    {
        c::setLastError("path must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toGmodPath(path)->commonNames().size();
}

int dnv_vista_sdk_gmod_path_common_name_depth_at(const dnv_vista_sdk_gmod_path_t* path, size_t index, size_t* outDepth)
{
    if (path == nullptr || outDepth == nullptr)
    {
        c::setLastError("path and outDepth must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    const auto commonNames = toGmodPath(path)->commonNames();
    if (index >= commonNames.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return 0;
    }

    *outDepth = commonNames[index].first;
    return 1;
}

char* dnv_vista_sdk_gmod_path_common_name_at(const dnv_vista_sdk_gmod_path_t* path, size_t index)
{
    if (path == nullptr)
    {
        c::setLastError("path must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto commonNames = toGmodPath(path)->commonNames();
    if (index >= commonNames.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return toOwnedCString(commonNames[index].second);
}

char* dnv_vista_sdk_gmod_path_to_string(const dnv_vista_sdk_gmod_path_t* path)
{
    if (path == nullptr)
    {
        c::setLastError("path must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toOwnedCString(toGmodPath(path)->toString());
}

char* dnv_vista_sdk_gmod_path_to_full_path_string(const dnv_vista_sdk_gmod_path_t* path)
{
    if (path == nullptr)
    {
        c::setLastError("path must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toOwnedCString(toGmodPath(path)->toFullPathString());
}

char* dnv_vista_sdk_gmod_path_to_string_dump(const dnv_vista_sdk_gmod_path_t* path)
{
    if (path == nullptr)
    {
        c::setLastError("path must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toOwnedCString(toGmodPath(path)->toStringDump());
}
