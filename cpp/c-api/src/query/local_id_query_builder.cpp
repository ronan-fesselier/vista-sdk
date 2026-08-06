#include "dnv/vista/sdk/c/query/local_id_query_builder.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromGmodPathRef;
using dnv::vista::sdk::c::fromLocalIdQueryBuilder;
using dnv::vista::sdk::c::fromLocalIdQueryValue;
using dnv::vista::sdk::c::toGmodPath;
using dnv::vista::sdk::c::toGmodPathQuery;
using dnv::vista::sdk::c::toLocalId;
using dnv::vista::sdk::c::toLocalIdQueryBuilder;
using dnv::vista::sdk::c::toMetadataTagsQuery;

dnv_vista_sdk_local_id_query_builder_t* dnv_vista_sdk_local_id_query_builder_create(void)
{
    return fromLocalIdQueryBuilder(LocalIdQueryBuilder::create());
}

dnv_vista_sdk_local_id_query_builder_t* dnv_vista_sdk_local_id_query_builder_from(
    const dnv_vista_sdk_local_id_t* localId)
{
    if (localId == nullptr)
    {
        c::setLastErrorMessage("localId must not be null");
        return nullptr;
    }

    return fromLocalIdQueryBuilder(LocalIdQueryBuilder::from(*toLocalId(localId)));
}

dnv_vista_sdk_local_id_query_builder_t* dnv_vista_sdk_local_id_query_builder_from_string(const char* localIdStr)
{
    if (localIdStr == nullptr)
    {
        c::setLastErrorMessage("localIdStr must not be null");
        return nullptr;
    }

    auto builder = LocalIdQueryBuilder::from(localIdStr);
    if (!builder.has_value())
    {
        c::setLastErrorMessage("invalid LocalId string");
    }

    return fromLocalIdQueryBuilder(std::move(builder));
}

void dnv_vista_sdk_local_id_query_builder_free(dnv_vista_sdk_local_id_query_builder_t* builder)
{
    delete reinterpret_cast<LocalIdQueryBuilder*>(builder);
}

dnv_vista_sdk_local_id_query_builder_t* dnv_vista_sdk_local_id_query_builder_with_primary_item(
    const dnv_vista_sdk_local_id_query_builder_t* builder, const dnv_vista_sdk_gmod_path_t* primaryItem)
{
    if (builder == nullptr || primaryItem == nullptr)
    {
        c::setLastErrorMessage("builder and primaryItem must not be null");
        return nullptr;
    }

    return fromLocalIdQueryBuilder(toLocalIdQueryBuilder(builder)->withPrimaryItem(*toGmodPath(primaryItem)));
}

dnv_vista_sdk_local_id_query_builder_t* dnv_vista_sdk_local_id_query_builder_with_primary_item_query(
    const dnv_vista_sdk_local_id_query_builder_t* builder, const dnv_vista_sdk_gmod_path_query_t* primaryItem)
{
    if (builder == nullptr || primaryItem == nullptr)
    {
        c::setLastErrorMessage("builder and primaryItem must not be null");
        return nullptr;
    }

    return fromLocalIdQueryBuilder(toLocalIdQueryBuilder(builder)->withPrimaryItem(*toGmodPathQuery(primaryItem)));
}

dnv_vista_sdk_local_id_query_builder_t* dnv_vista_sdk_local_id_query_builder_with_secondary_item(
    const dnv_vista_sdk_local_id_query_builder_t* builder, const dnv_vista_sdk_gmod_path_t* secondaryItem)
{
    if (builder == nullptr || secondaryItem == nullptr)
    {
        c::setLastErrorMessage("builder and secondaryItem must not be null");
        return nullptr;
    }

    return fromLocalIdQueryBuilder(toLocalIdQueryBuilder(builder)->withSecondaryItem(*toGmodPath(secondaryItem)));
}

dnv_vista_sdk_local_id_query_builder_t* dnv_vista_sdk_local_id_query_builder_with_secondary_item_query(
    const dnv_vista_sdk_local_id_query_builder_t* builder, const dnv_vista_sdk_gmod_path_query_t* secondaryItem)
{
    if (builder == nullptr || secondaryItem == nullptr)
    {
        c::setLastErrorMessage("builder and secondaryItem must not be null");
        return nullptr;
    }

    return fromLocalIdQueryBuilder(toLocalIdQueryBuilder(builder)->withSecondaryItem(*toGmodPathQuery(secondaryItem)));
}

dnv_vista_sdk_local_id_query_builder_t* dnv_vista_sdk_local_id_query_builder_with_any_secondary_item(
    const dnv_vista_sdk_local_id_query_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return fromLocalIdQueryBuilder(toLocalIdQueryBuilder(builder)->withAnySecondaryItem());
}

dnv_vista_sdk_local_id_query_builder_t* dnv_vista_sdk_local_id_query_builder_without_secondary_item(
    const dnv_vista_sdk_local_id_query_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return fromLocalIdQueryBuilder(toLocalIdQueryBuilder(builder)->withoutSecondaryItem());
}

dnv_vista_sdk_local_id_query_builder_t* dnv_vista_sdk_local_id_query_builder_with_tags(
    const dnv_vista_sdk_local_id_query_builder_t* builder, const dnv_vista_sdk_metadata_tags_query_t* tags)
{
    if (builder == nullptr || tags == nullptr)
    {
        c::setLastErrorMessage("builder and tags must not be null");
        return nullptr;
    }

    return fromLocalIdQueryBuilder(toLocalIdQueryBuilder(builder)->withTags(*toMetadataTagsQuery(tags)));
}

dnv_vista_sdk_local_id_query_builder_t* dnv_vista_sdk_local_id_query_builder_without_locations(
    const dnv_vista_sdk_local_id_query_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return fromLocalIdQueryBuilder(toLocalIdQueryBuilder(builder)->withoutLocations());
}

const dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_local_id_query_builder_primary_item(
    const dnv_vista_sdk_local_id_query_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    const auto* path = toLocalIdQueryBuilder(builder)->primaryItem();
    if (path == nullptr)
    {
        return nullptr;
    }

    return fromGmodPathRef(*path);
}

const dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_local_id_query_builder_secondary_item(
    const dnv_vista_sdk_local_id_query_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    const auto* path = toLocalIdQueryBuilder(builder)->secondaryItem();
    if (path == nullptr)
    {
        return nullptr;
    }

    return fromGmodPathRef(*path);
}

dnv_vista_sdk_local_id_query_t* dnv_vista_sdk_local_id_query_builder_build(
    const dnv_vista_sdk_local_id_query_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return fromLocalIdQueryValue(toLocalIdQueryBuilder(builder)->build());
}
