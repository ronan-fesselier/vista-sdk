#pragma once

#include "dnv/vista/sdk/c/core/codebook_name.h"
#include "dnv/vista/sdk/c/core/codebook.h"
#include "dnv/vista/sdk/c/core/codebooks.h"
#include "dnv/vista/sdk/c/core/gmod.h"
#include "dnv/vista/sdk/c/core/gmod_individualizable_set.h"
#include "dnv/vista/sdk/c/core/gmod_node.h"
#include "dnv/vista/sdk/c/core/gmod_node_metadata.h"
#include "dnv/vista/sdk/c/core/gmod_path.h"
#include "dnv/vista/sdk/c/core/imo_number.h"
#include "dnv/vista/sdk/c/core/local_id.h"
#include "dnv/vista/sdk/c/core/local_id_builder.h"
#include "dnv/vista/sdk/c/core/local_id_mqtt.h"
#include "dnv/vista/sdk/c/core/location.h"
#include "dnv/vista/sdk/c/core/location_builder.h"
#include "dnv/vista/sdk/c/core/locations.h"
#include "dnv/vista/sdk/c/core/metadata_tag.h"
#include "dnv/vista/sdk/c/core/parsing_errors.h"
#include "dnv/vista/sdk/c/core/relative_location.h"
#include "dnv/vista/sdk/c/core/universal_id.h"
#include "dnv/vista/sdk/c/core/universal_id_builder.h"
#include "dnv/vista/sdk/c/core/vis.h"
#include "dnv/vista/sdk/c/query/gmod_path_query.h"
#include "dnv/vista/sdk/c/query/gmod_path_query_builder.h"
#include "dnv/vista/sdk/c/query/local_id_query.h"
#include "dnv/vista/sdk/c/query/local_id_query_builder.h"
#include "dnv/vista/sdk/c/query/metadata_tags_query.h"
#include "dnv/vista/sdk/c/query/metadata_tags_query_builder.h"
#include "dnv/vista/sdk/c/transport/ship_id.h"
#include "dnv/vista/sdk/c/types/datetime/date_time.h"
#include "dnv/vista/sdk/c/types/datetime/date_time_offset.h"
#include "dnv/vista/sdk/c/types/datetime/time_span.h"
#include "dnv/vista/sdk/c/types/decimal/decimal.h"

#include <dnv/VistaSDK.h>

#include <cstring>
#include <string>
#include <type_traits>

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

    static_assert(static_cast<int>(DateTime::Format::Iso8601) == DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601);
    static_assert(static_cast<int>(DateTime::Format::Iso8601Precise) == DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601_PRECISE);
    static_assert(
        static_cast<int>(DateTime::Format::Iso8601PreciseTrimmed) ==
        DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601_PRECISE_TRIMMED);
    static_assert(static_cast<int>(DateTime::Format::Iso8601Millis) == DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601_MILLIS);
    static_assert(static_cast<int>(DateTime::Format::Iso8601Micros) == DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601_MICROS);
    static_assert(
        static_cast<int>(DateTime::Format::Iso8601Extended) == DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601_EXTENDED);
    static_assert(static_cast<int>(DateTime::Format::Iso8601Basic) == DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601_BASIC);
    static_assert(static_cast<int>(DateTime::Format::Iso8601Date) == DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601_DATE);
    static_assert(static_cast<int>(DateTime::Format::Iso8601Time) == DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601_TIME);
    static_assert(static_cast<int>(DateTime::Format::UnixSeconds) == DNV_VISTA_SDK_DATE_TIME_FORMAT_UNIX_SECONDS);
    static_assert(
        static_cast<int>(DateTime::Format::UnixMilliseconds) == DNV_VISTA_SDK_DATE_TIME_FORMAT_UNIX_MILLISECONDS);

    static_assert(static_cast<int>(Decimal::RoundingMode::ToNearest) == DNV_VISTA_SDK_DECIMAL_ROUNDING_MODE_TO_NEAREST);
    static_assert(
        static_cast<int>(Decimal::RoundingMode::ToNearestTiesAway) ==
        DNV_VISTA_SDK_DECIMAL_ROUNDING_MODE_TO_NEAREST_TIES_AWAY);
    static_assert(static_cast<int>(Decimal::RoundingMode::ToZero) == DNV_VISTA_SDK_DECIMAL_ROUNDING_MODE_TO_ZERO);
    static_assert(
        static_cast<int>(Decimal::RoundingMode::ToPositiveInfinity) ==
        DNV_VISTA_SDK_DECIMAL_ROUNDING_MODE_TO_POSITIVE_INFINITY);
    static_assert(
        static_cast<int>(Decimal::RoundingMode::ToNegativeInfinity) ==
        DNV_VISTA_SDK_DECIMAL_ROUNDING_MODE_TO_NEGATIVE_INFINITY);

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

    inline const ImoNumber* toImoNumber(const dnv_vista_sdk_imo_number_t* imoNumber)
    {
        return reinterpret_cast<const ImoNumber*>(imoNumber);
    }

    inline dnv_vista_sdk_imo_number_t* fromImoNumberValue(const ImoNumber& imoNumber)
    {
        return reinterpret_cast<dnv_vista_sdk_imo_number_t*>(new ImoNumber{ imoNumber });
    }

    inline const GmodNodeMetadata* toMetadata(const dnv_vista_sdk_gmod_node_metadata_t* metadata)
    {
        return reinterpret_cast<const GmodNodeMetadata*>(metadata);
    }

    inline const dnv_vista_sdk_gmod_node_metadata_t* fromMetadata(const GmodNodeMetadata& metadata)
    {
        return reinterpret_cast<const dnv_vista_sdk_gmod_node_metadata_t*>(&metadata);
    }

    inline const GmodNode* toGmodNode(const dnv_vista_sdk_gmod_node_t* node)
    {
        return reinterpret_cast<const GmodNode*>(node);
    }

    inline const dnv_vista_sdk_gmod_node_t* fromGmodNode(const GmodNode* node)
    {
        return reinterpret_cast<const dnv_vista_sdk_gmod_node_t*>(node);
    }

    inline dnv_vista_sdk_gmod_node_t* fromGmodNode(std::optional<GmodNode>&& node)
    {
        if (!node.has_value())
        {
            return nullptr;
        }

        return reinterpret_cast<dnv_vista_sdk_gmod_node_t*>(new GmodNode{ std::move(*node) });
    }

    inline const Gmod* toGmod(const dnv_vista_sdk_gmod_t* gmod)
    {
        return reinterpret_cast<const Gmod*>(gmod);
    }

    inline const GmodPath* toGmodPath(const dnv_vista_sdk_gmod_path_t* path)
    {
        return reinterpret_cast<const GmodPath*>(path);
    }

    inline dnv_vista_sdk_gmod_path_t* fromGmodPath(std::optional<GmodPath>&& path)
    {
        if (!path.has_value())
        {
            return nullptr;
        }

        return reinterpret_cast<dnv_vista_sdk_gmod_path_t*>(new GmodPath{ std::move(*path) });
    }

    inline dnv_vista_sdk_gmod_path_t* fromGmodPathValue(GmodPath&& path)
    {
        return reinterpret_cast<dnv_vista_sdk_gmod_path_t*>(new GmodPath{ std::move(path) });
    }

    inline GmodIndividualizableSet* toIndividualizableSet(dnv_vista_sdk_gmod_individualizable_set_t* set)
    {
        return reinterpret_cast<GmodIndividualizableSet*>(set);
    }

    inline const GmodIndividualizableSet* toIndividualizableSet(const dnv_vista_sdk_gmod_individualizable_set_t* set)
    {
        return reinterpret_cast<const GmodIndividualizableSet*>(set);
    }

    inline const LocalIdBuilder* toLocalIdBuilder(const dnv_vista_sdk_local_id_builder_t* builder)
    {
        return reinterpret_cast<const LocalIdBuilder*>(builder);
    }

    inline dnv_vista_sdk_local_id_builder_t* fromLocalIdBuilder(LocalIdBuilder&& builder)
    {
        return reinterpret_cast<dnv_vista_sdk_local_id_builder_t*>(new LocalIdBuilder{ std::move(builder) });
    }

    inline const LocalId* toLocalId(const dnv_vista_sdk_local_id_t* localId)
    {
        return reinterpret_cast<const LocalId*>(localId);
    }

    inline dnv_vista_sdk_local_id_t* fromLocalId(std::optional<LocalId>&& localId)
    {
        if (!localId.has_value())
        {
            return nullptr;
        }

        return reinterpret_cast<dnv_vista_sdk_local_id_t*>(new LocalId{ std::move(*localId) });
    }

    inline dnv_vista_sdk_local_id_t* fromLocalIdValue(LocalId&& localId)
    {
        return reinterpret_cast<dnv_vista_sdk_local_id_t*>(new LocalId{ std::move(localId) });
    }

    inline const mqtt::LocalId* toLocalIdMqtt(const dnv_vista_sdk_local_id_mqtt_t* localId)
    {
        return reinterpret_cast<const mqtt::LocalId*>(localId);
    }

    inline const dnv_vista_sdk_local_id_builder_t* fromLocalIdBuilderRef(const LocalIdBuilder& builder)
    {
        return reinterpret_cast<const dnv_vista_sdk_local_id_builder_t*>(&builder);
    }

    inline const dnv_vista_sdk_gmod_path_t* fromGmodPathRef(const GmodPath& path)
    {
        return reinterpret_cast<const dnv_vista_sdk_gmod_path_t*>(&path);
    }

    inline const dnv_vista_sdk_metadata_tag_t* fromTagRef(const MetadataTag& tag)
    {
        return reinterpret_cast<const dnv_vista_sdk_metadata_tag_t*>(&tag);
    }

    inline const UniversalIdBuilder* toUniversalIdBuilder(const dnv_vista_sdk_universal_id_builder_t* builder)
    {
        return reinterpret_cast<const UniversalIdBuilder*>(builder);
    }

    inline dnv_vista_sdk_universal_id_builder_t* fromUniversalIdBuilder(UniversalIdBuilder&& builder)
    {
        return reinterpret_cast<dnv_vista_sdk_universal_id_builder_t*>(new UniversalIdBuilder{ std::move(builder) });
    }

    inline const UniversalId* toUniversalId(const dnv_vista_sdk_universal_id_t* universalId)
    {
        return reinterpret_cast<const UniversalId*>(universalId);
    }

    inline dnv_vista_sdk_universal_id_t* fromUniversalId(std::optional<UniversalId>&& universalId)
    {
        if (!universalId.has_value())
        {
            return nullptr;
        }

        return reinterpret_cast<dnv_vista_sdk_universal_id_t*>(new UniversalId{ std::move(*universalId) });
    }

    inline dnv_vista_sdk_universal_id_t* fromUniversalIdValue(UniversalId&& universalId)
    {
        return reinterpret_cast<dnv_vista_sdk_universal_id_t*>(new UniversalId{ std::move(universalId) });
    }

    inline const dnv_vista_sdk_imo_number_t* fromImoNumberRef(const ImoNumber& imoNumber)
    {
        return reinterpret_cast<const dnv_vista_sdk_imo_number_t*>(&imoNumber);
    }

    inline const dnv_vista_sdk_local_id_t* fromLocalIdRef(const LocalId& localId)
    {
        return reinterpret_cast<const dnv_vista_sdk_local_id_t*>(&localId);
    }

    inline const dnv_vista_sdk_universal_id_builder_t* fromUniversalIdBuilderRef(const UniversalIdBuilder& builder)
    {
        return reinterpret_cast<const dnv_vista_sdk_universal_id_builder_t*>(&builder);
    }

    inline const MetadataTagsQueryBuilder* toMetadataTagsQueryBuilder(
        const dnv_vista_sdk_metadata_tags_query_builder_t* builder)
    {
        return reinterpret_cast<const MetadataTagsQueryBuilder*>(builder);
    }

    inline dnv_vista_sdk_metadata_tags_query_builder_t* fromMetadataTagsQueryBuilder(MetadataTagsQueryBuilder&& builder)
    {
        return reinterpret_cast<dnv_vista_sdk_metadata_tags_query_builder_t*>(
            new MetadataTagsQueryBuilder{ std::move(builder) });
    }

    inline const dnv_vista_sdk_metadata_tags_query_builder_t* fromMetadataTagsQueryBuilderRef(
        const MetadataTagsQueryBuilder& builder)
    {
        return reinterpret_cast<const dnv_vista_sdk_metadata_tags_query_builder_t*>(&builder);
    }

    inline const MetadataTagsQuery* toMetadataTagsQuery(const dnv_vista_sdk_metadata_tags_query_t* query)
    {
        return reinterpret_cast<const MetadataTagsQuery*>(query);
    }

    inline dnv_vista_sdk_metadata_tags_query_t* fromMetadataTagsQueryValue(MetadataTagsQuery&& query)
    {
        return reinterpret_cast<dnv_vista_sdk_metadata_tags_query_t*>(new MetadataTagsQuery{ std::move(query) });
    }

    struct GmodPathQueryBuilderWrapper
    {
        enum class Variant
        {
            Path,
            Nodes
        };
        Variant variant;
        std::unique_ptr<GmodPathQueryBuilder> builder;
    };

    inline GmodPathQueryBuilderWrapper* toGmodPathQueryBuilderWrapper(dnv_vista_sdk_gmod_path_query_builder_t* builder)
    {
        return reinterpret_cast<GmodPathQueryBuilderWrapper*>(builder);
    }

    inline const GmodPathQueryBuilderWrapper* toGmodPathQueryBuilderWrapper(
        const dnv_vista_sdk_gmod_path_query_builder_t* builder)
    {
        return reinterpret_cast<const GmodPathQueryBuilderWrapper*>(builder);
    }

    inline dnv_vista_sdk_gmod_path_query_builder_t* fromGmodPathQueryBuilderWrapper(
        GmodPathQueryBuilderWrapper::Variant variant, std::unique_ptr<GmodPathQueryBuilder> builder)
    {
        return reinterpret_cast<dnv_vista_sdk_gmod_path_query_builder_t*>(
            new GmodPathQueryBuilderWrapper{ variant, std::move(builder) });
    }

    inline const GmodPathQuery* toGmodPathQuery(const dnv_vista_sdk_gmod_path_query_t* query)
    {
        return reinterpret_cast<const GmodPathQuery*>(query);
    }

    inline dnv_vista_sdk_gmod_path_query_t* fromGmodPathQueryValue(GmodPathQuery&& query)
    {
        return reinterpret_cast<dnv_vista_sdk_gmod_path_query_t*>(new GmodPathQuery{ std::move(query) });
    }

    inline const dnv_vista_sdk_gmod_path_query_t* fromGmodPathQueryRef(const GmodPathQuery& query)
    {
        return reinterpret_cast<const dnv_vista_sdk_gmod_path_query_t*>(&query);
    }

    inline const LocalIdQueryBuilder* toLocalIdQueryBuilder(const dnv_vista_sdk_local_id_query_builder_t* builder)
    {
        return reinterpret_cast<const LocalIdQueryBuilder*>(builder);
    }

    inline dnv_vista_sdk_local_id_query_builder_t* fromLocalIdQueryBuilder(LocalIdQueryBuilder&& builder)
    {
        return reinterpret_cast<dnv_vista_sdk_local_id_query_builder_t*>(new LocalIdQueryBuilder{ std::move(builder) });
    }

    inline dnv_vista_sdk_local_id_query_builder_t* fromLocalIdQueryBuilder(std::optional<LocalIdQueryBuilder>&& builder)
    {
        if (!builder.has_value())
        {
            return nullptr;
        }

        return reinterpret_cast<dnv_vista_sdk_local_id_query_builder_t*>(
            new LocalIdQueryBuilder{ std::move(*builder) });
    }

    inline const LocalIdQuery* toLocalIdQuery(const dnv_vista_sdk_local_id_query_t* query)
    {
        return reinterpret_cast<const LocalIdQuery*>(query);
    }

    inline dnv_vista_sdk_local_id_query_t* fromLocalIdQueryValue(LocalIdQuery&& query)
    {
        return reinterpret_cast<dnv_vista_sdk_local_id_query_t*>(new LocalIdQuery{ std::move(query) });
    }

    inline TimeSpan toTimeSpan(dnv_vista_sdk_time_span_t ts)
    {
        return TimeSpan{ ts.ticks };
    }

    inline dnv_vista_sdk_time_span_t fromTimeSpan(const TimeSpan& ts)
    {
        return dnv_vista_sdk_time_span_t{ ts.ticks() };
    }

    inline DateTime toDateTime(dnv_vista_sdk_date_time_t dt)
    {
        return DateTime{ dt.ticks };
    }

    inline dnv_vista_sdk_date_time_t fromDateTime(const DateTime& dt)
    {
        return dnv_vista_sdk_date_time_t{ dt.ticks() };
    }

    inline DateTime::Format toDateTimeFormat(dnv_vista_sdk_date_time_format_t format)
    {
        return static_cast<DateTime::Format>(format);
    }

    inline DateTimeOffset toDateTimeOffset(dnv_vista_sdk_date_time_offset_t dto)
    {
        return DateTimeOffset{ toDateTime(dto.dateTime), toTimeSpan(dto.offset) };
    }

    inline dnv_vista_sdk_date_time_offset_t fromDateTimeOffset(const DateTimeOffset& dto)
    {
        return dnv_vista_sdk_date_time_offset_t{ fromDateTime(dto.dateTime()), fromTimeSpan(dto.offset()) };
    }

    inline Decimal toDecimal(dnv_vista_sdk_decimal_t d)
    {
        Decimal result;
        result.flags() = d.flags;
        result.mantissa() = { d.mantissa[0], d.mantissa[1], d.mantissa[2] };
        return result;
    }

    inline dnv_vista_sdk_decimal_t fromDecimal(const Decimal& d)
    {
        dnv_vista_sdk_decimal_t result{};
        result.flags = d.flags();
        result.mantissa[0] = d.mantissa()[0];
        result.mantissa[1] = d.mantissa()[1];
        result.mantissa[2] = d.mantissa()[2];
        return result;
    }

    inline Decimal::RoundingMode toRoundingMode(dnv_vista_sdk_decimal_rounding_mode_t mode)
    {
        return static_cast<Decimal::RoundingMode>(mode);
    }

    inline const transport::ShipId* toShipId(const dnv_vista_sdk_ship_id_t* shipId)
    {
        return reinterpret_cast<const transport::ShipId*>(shipId);
    }

    inline dnv_vista_sdk_ship_id_t* fromShipId(transport::ShipId&& shipId)
    {
        return reinterpret_cast<dnv_vista_sdk_ship_id_t*>(new transport::ShipId{ std::move(shipId) });
    }

    inline dnv_vista_sdk_ship_id_t* fromShipId(std::optional<transport::ShipId>&& shipId)
    {
        if (!shipId.has_value())
        {
            return nullptr;
        }

        return reinterpret_cast<dnv_vista_sdk_ship_id_t*>(new transport::ShipId{ std::move(*shipId) });
    }
} // namespace dnv::vista::sdk::c
