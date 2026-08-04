#include "dnv/vista/sdk/c/core/local_id_builder.h"

#include "local_id_internal.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromGmodPathValue;
using dnv::vista::sdk::c::fromLocalId;
using dnv::vista::sdk::c::fromLocalIdBuilder;
using dnv::vista::sdk::c::fromLocalIdValue;
using dnv::vista::sdk::c::selectMetadataTag;
using dnv::vista::sdk::c::toCodebookName;
using dnv::vista::sdk::c::toGmodPath;
using dnv::vista::sdk::c::toLocalIdBuilder;
using dnv::vista::sdk::c::toOwnedCString;
using dnv::vista::sdk::c::toTag;

const char* dnv_vista_sdk_local_id_builder_naming_rule(void)
{
    return LocalIdBuilder::namingRule().data();
}

dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_create(const char* visVersion)
{
    if (visVersion == nullptr)
    {
        c::setLastErrorMessage("visVersion must not be null");
        return nullptr;
    }

    const auto version = VisVersions::fromString(visVersion);
    if (!version.has_value())
    {
        c::setLastErrorMessage("unrecognized VIS version");
        return nullptr;
    }

    return fromLocalIdBuilder(LocalIdBuilder::create(*version));
}

void dnv_vista_sdk_local_id_builder_free(dnv_vista_sdk_local_id_builder_t* builder)
{
    delete reinterpret_cast<LocalIdBuilder*>(builder);
}

const char* dnv_vista_sdk_local_id_builder_version(const dnv_vista_sdk_local_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    const auto version = toLocalIdBuilder(builder)->version();
    if (!version.has_value())
    {
        return nullptr;
    }

    return VisVersions::toString(*version).data();
}

int dnv_vista_sdk_local_id_builder_is_verbose_mode(const dnv_vista_sdk_local_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return 0;
    }

    return toLocalIdBuilder(builder)->isVerboseMode() ? 1 : 0;
}

int dnv_vista_sdk_local_id_builder_is_valid(const dnv_vista_sdk_local_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return 0;
    }

    return toLocalIdBuilder(builder)->isValid() ? 1 : 0;
}

int dnv_vista_sdk_local_id_builder_is_empty(const dnv_vista_sdk_local_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return 0;
    }

    return toLocalIdBuilder(builder)->isEmpty() ? 1 : 0;
}

int dnv_vista_sdk_local_id_builder_is_empty_metadata(const dnv_vista_sdk_local_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return 0;
    }

    return toLocalIdBuilder(builder)->isEmptyMetadata() ? 1 : 0;
}

int dnv_vista_sdk_local_id_builder_has_custom_tag(const dnv_vista_sdk_local_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return 0;
    }

    return toLocalIdBuilder(builder)->hasCustomTag() ? 1 : 0;
}

dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_local_id_builder_primary_item(const dnv_vista_sdk_local_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    const auto& path = toLocalIdBuilder(builder)->primaryItem();
    if (!path.has_value())
    {
        return nullptr;
    }

    return fromGmodPathValue(GmodPath{ *path });
}

dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_local_id_builder_secondary_item(
    const dnv_vista_sdk_local_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    const auto& path = toLocalIdBuilder(builder)->secondaryItem();
    if (!path.has_value())
    {
        return nullptr;
    }

    return fromGmodPathValue(GmodPath{ *path });
}

dnv_vista_sdk_metadata_tag_t* dnv_vista_sdk_local_id_builder_metadata_tag(
    const dnv_vista_sdk_local_id_builder_t* builder, dnv_vista_sdk_codebook_name_t name)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    const auto* tag = selectMetadataTag(*toLocalIdBuilder(builder), toCodebookName(name));
    if (tag == nullptr)
    {
        c::setLastErrorMessage("invalid codebook name");
        return nullptr;
    }

    if (!tag->has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<dnv_vista_sdk_metadata_tag_t*>(new MetadataTag{ **tag });
}

dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_with_vis_version(
    const dnv_vista_sdk_local_id_builder_t* builder, const char* visVersion)
{
    if (builder == nullptr || visVersion == nullptr)
    {
        c::setLastErrorMessage("builder and visVersion must not be null");
        return nullptr;
    }

    try
    {
        return fromLocalIdBuilder(toLocalIdBuilder(builder)->withVisVersion(std::string_view{ visVersion }));
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_without_vis_version(
    const dnv_vista_sdk_local_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return fromLocalIdBuilder(toLocalIdBuilder(builder)->withoutVisVersion());
}

dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_with_primary_item(
    const dnv_vista_sdk_local_id_builder_t* builder, const dnv_vista_sdk_gmod_path_t* path)
{
    if (builder == nullptr || path == nullptr)
    {
        c::setLastErrorMessage("builder and path must not be null");
        return nullptr;
    }

    return fromLocalIdBuilder(toLocalIdBuilder(builder)->withPrimaryItem(*toGmodPath(path)));
}

dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_without_primary_item(
    const dnv_vista_sdk_local_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return fromLocalIdBuilder(toLocalIdBuilder(builder)->withoutPrimaryItem());
}

dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_with_secondary_item(
    const dnv_vista_sdk_local_id_builder_t* builder, const dnv_vista_sdk_gmod_path_t* path)
{
    if (builder == nullptr || path == nullptr)
    {
        c::setLastErrorMessage("builder and path must not be null");
        return nullptr;
    }

    return fromLocalIdBuilder(toLocalIdBuilder(builder)->withSecondaryItem(*toGmodPath(path)));
}

dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_without_secondary_item(
    const dnv_vista_sdk_local_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return fromLocalIdBuilder(toLocalIdBuilder(builder)->withoutSecondaryItem());
}

dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_with_metadata_tag(
    const dnv_vista_sdk_local_id_builder_t* builder, const dnv_vista_sdk_metadata_tag_t* tag)
{
    if (builder == nullptr || tag == nullptr)
    {
        c::setLastErrorMessage("builder and tag must not be null");
        return nullptr;
    }

    try
    {
        return fromLocalIdBuilder(toLocalIdBuilder(builder)->withMetadataTag(*toTag(tag)));
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_without_metadata_tag(
    const dnv_vista_sdk_local_id_builder_t* builder, dnv_vista_sdk_codebook_name_t name)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return fromLocalIdBuilder(toLocalIdBuilder(builder)->withoutMetadataTag(toCodebookName(name)));
}

dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_with_verbose_mode(
    const dnv_vista_sdk_local_id_builder_t* builder, int verbose)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return fromLocalIdBuilder(toLocalIdBuilder(builder)->withVerboseMode(verbose != 0));
}

dnv_vista_sdk_local_id_t* dnv_vista_sdk_local_id_builder_build(const dnv_vista_sdk_local_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    try
    {
        return fromLocalIdValue(toLocalIdBuilder(builder)->build());
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

char* dnv_vista_sdk_local_id_builder_to_string(const dnv_vista_sdk_local_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return toOwnedCString(toLocalIdBuilder(builder)->toString());
}

dnv_vista_sdk_local_id_t* dnv_vista_sdk_local_id_builder_from_string(const char* localIdStr)
{
    if (localIdStr == nullptr)
    {
        c::setLastErrorMessage("localIdStr must not be null");
        return nullptr;
    }

    auto localId = LocalIdBuilder::fromString(localIdStr);
    if (!localId.has_value())
    {
        c::setLastErrorMessage("invalid LocalId string");
    }

    return fromLocalId(std::move(localId));
}

dnv_vista_sdk_local_id_t* dnv_vista_sdk_local_id_builder_from_string_with_errors(
    const char* localIdStr, dnv_vista_sdk_parsing_errors_t** outErrors)
{
    if (localIdStr == nullptr || outErrors == nullptr)
    {
        c::setLastErrorMessage("localIdStr and outErrors must not be null");
        return nullptr;
    }

    ParsingErrors errors;
    auto localId = LocalIdBuilder::fromString(localIdStr, errors);

    *outErrors = reinterpret_cast<dnv_vista_sdk_parsing_errors_t*>(new ParsingErrors{ std::move(errors) });

    if (!localId.has_value())
    {
        c::setLastErrorMessage("invalid LocalId string");
    }

    return fromLocalId(std::move(localId));
}

void dnv_vista_sdk_local_id_builder_string_free(char* str)
{
    delete[] str;
}
