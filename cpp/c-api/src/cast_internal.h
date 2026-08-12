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
#include "dnv/vista/sdk/c/transport/serialization/json/serializable_document.h"
#include "dnv/vista/sdk/c/transport/datachannel/data_channel.h"
#include "dnv/vista/sdk/c/transport/timeseries/data_channel_id.h"
#include "dnv/vista/sdk/c/transport/timeseries/time_series_data.h"
#include "dnv/vista/sdk/c/transport/iso19848.h"
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
    namespace dcl = transport::datachannel;

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

    static_assert(static_cast<int>(dcl::Restriction::WhiteSpace::Preserve) == DNV_VISTA_SDK_DCL_WHITE_SPACE_PRESERVE);
    static_assert(static_cast<int>(dcl::Restriction::WhiteSpace::Replace) == DNV_VISTA_SDK_DCL_WHITE_SPACE_REPLACE);
    static_assert(static_cast<int>(dcl::Restriction::WhiteSpace::Collapse) == DNV_VISTA_SDK_DCL_WHITE_SPACE_COLLAPSE);

    static_assert(static_cast<int>(transport::ISO19848Version::v2018) == DNV_VISTA_SDK_ISO19848_VERSION_V2018);
    static_assert(static_cast<int>(transport::ISO19848Version::v2024) == DNV_VISTA_SDK_ISO19848_VERSION_V2024);

    static_assert(static_cast<int>(transport::Value::Type::Decimal) == DNV_VISTA_SDK_ISO19848_VALUE_TYPE_DECIMAL);
    static_assert(static_cast<int>(transport::Value::Type::Integer) == DNV_VISTA_SDK_ISO19848_VALUE_TYPE_INTEGER);
    static_assert(static_cast<int>(transport::Value::Type::Boolean) == DNV_VISTA_SDK_ISO19848_VALUE_TYPE_BOOLEAN);
    static_assert(static_cast<int>(transport::Value::Type::String) == DNV_VISTA_SDK_ISO19848_VALUE_TYPE_STRING);
    static_assert(static_cast<int>(transport::Value::Type::DateTime) == DNV_VISTA_SDK_ISO19848_VALUE_TYPE_DATE_TIME);

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

    inline transport::ISO19848Version toIso19848Version(dnv_vista_sdk_iso19848_version_t version)
    {
        return static_cast<transport::ISO19848Version>(version);
    }

    inline dnv_vista_sdk_iso19848_version_t fromIso19848Version(transport::ISO19848Version version)
    {
        return static_cast<dnv_vista_sdk_iso19848_version_t>(version);
    }

    inline const transport::ISO19848* toIso19848(const dnv_vista_sdk_iso19848_t* iso19848)
    {
        return reinterpret_cast<const transport::ISO19848*>(iso19848);
    }

    inline const transport::DataChannelTypeName* toDataChannelTypeName(
        const dnv_vista_sdk_iso19848_data_channel_type_name_t* typeName)
    {
        return reinterpret_cast<const transport::DataChannelTypeName*>(typeName);
    }

    inline dnv_vista_sdk_iso19848_data_channel_type_name_t* fromDataChannelTypeName(
        transport::DataChannelTypeName&& typeName)
    {
        return reinterpret_cast<dnv_vista_sdk_iso19848_data_channel_type_name_t*>(
            new transport::DataChannelTypeName{ std::move(typeName) });
    }

    inline const transport::DataChannelTypeNames* toDataChannelTypeNames(
        const dnv_vista_sdk_iso19848_data_channel_type_names_t* typeNames)
    {
        return reinterpret_cast<const transport::DataChannelTypeNames*>(typeNames);
    }

    inline dnv_vista_sdk_iso19848_data_channel_type_names_t* fromDataChannelTypeNames(
        transport::DataChannelTypeNames&& typeNames)
    {
        return reinterpret_cast<dnv_vista_sdk_iso19848_data_channel_type_names_t*>(
            new transport::DataChannelTypeNames{ std::move(typeNames) });
    }

    inline const transport::FormatDataType* toFormatDataType(const dnv_vista_sdk_iso19848_format_data_type_t* fdt)
    {
        return reinterpret_cast<const transport::FormatDataType*>(fdt);
    }

    inline dnv_vista_sdk_iso19848_format_data_type_t* fromFormatDataType(transport::FormatDataType&& fdt)
    {
        return reinterpret_cast<dnv_vista_sdk_iso19848_format_data_type_t*>(
            new transport::FormatDataType{ std::move(fdt) });
    }

    inline const transport::FormatDataTypes* toFormatDataTypes(const dnv_vista_sdk_iso19848_format_data_types_t* fdts)
    {
        return reinterpret_cast<const transport::FormatDataTypes*>(fdts);
    }

    inline dnv_vista_sdk_iso19848_format_data_types_t* fromFormatDataTypes(transport::FormatDataTypes&& fdts)
    {
        return reinterpret_cast<dnv_vista_sdk_iso19848_format_data_types_t*>(
            new transport::FormatDataTypes{ std::move(fdts) });
    }

    inline const transport::Value* toIso19848Value(const dnv_vista_sdk_iso19848_value_t* value)
    {
        return reinterpret_cast<const transport::Value*>(value);
    }

    inline dnv_vista_sdk_iso19848_value_t* fromIso19848Value(transport::Value&& value)
    {
        return reinterpret_cast<dnv_vista_sdk_iso19848_value_t*>(new transport::Value{ std::move(value) });
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

    inline const transport::serialization::json::SerializableDocument* toSerializableDocument(
        const dnv_vista_sdk_serializable_document_t* doc)
    {
        return reinterpret_cast<const transport::serialization::json::SerializableDocument*>(doc);
    }

    inline dnv_vista_sdk_serializable_document_t* fromSerializableDocument(
        transport::serialization::json::SerializableDocument&& doc)
    {
        return reinterpret_cast<dnv_vista_sdk_serializable_document_t*>(
            new transport::serialization::json::SerializableDocument{ std::move(doc) });
    }

    inline const dnv_vista_sdk_serializable_document_t* fromSerializableDocumentRef(
        const transport::serialization::json::SerializableDocument& doc)
    {
        return reinterpret_cast<const dnv_vista_sdk_serializable_document_t*>(&doc);
    }

    inline dcl::Restriction::WhiteSpace toWhiteSpace(dnv_vista_sdk_dcl_white_space_t value)
    {
        return static_cast<dcl::Restriction::WhiteSpace>(value);
    }

    inline dnv_vista_sdk_dcl_white_space_t fromWhiteSpace(dcl::Restriction::WhiteSpace value)
    {
        return static_cast<dnv_vista_sdk_dcl_white_space_t>(value);
    }

    inline dcl::Restriction* toRestriction(dnv_vista_sdk_dcl_restriction_t* r)
    {
        return reinterpret_cast<dcl::Restriction*>(r);
    }

    inline const dcl::Restriction* toRestriction(const dnv_vista_sdk_dcl_restriction_t* r)
    {
        return reinterpret_cast<const dcl::Restriction*>(r);
    }

    inline dnv_vista_sdk_dcl_restriction_t* fromRestriction(dcl::Restriction&& r)
    {
        return reinterpret_cast<dnv_vista_sdk_dcl_restriction_t*>(new dcl::Restriction{ std::move(r) });
    }

    inline dcl::Range* toRange(dnv_vista_sdk_dcl_range_t* r)
    {
        return reinterpret_cast<dcl::Range*>(r);
    }

    inline const dcl::Range* toRange(const dnv_vista_sdk_dcl_range_t* r)
    {
        return reinterpret_cast<const dcl::Range*>(r);
    }

    inline dnv_vista_sdk_dcl_range_t* fromRange(dcl::Range&& r)
    {
        return reinterpret_cast<dnv_vista_sdk_dcl_range_t*>(new dcl::Range{ std::move(r) });
    }

    inline dcl::Format* toFormat(dnv_vista_sdk_dcl_format_t* f)
    {
        return reinterpret_cast<dcl::Format*>(f);
    }

    inline const dcl::Format* toFormat(const dnv_vista_sdk_dcl_format_t* f)
    {
        return reinterpret_cast<const dcl::Format*>(f);
    }

    inline dnv_vista_sdk_dcl_format_t* fromFormat(dcl::Format&& f)
    {
        return reinterpret_cast<dnv_vista_sdk_dcl_format_t*>(new dcl::Format{ std::move(f) });
    }

    inline dcl::DataChannelType* toDataChannelType(dnv_vista_sdk_dcl_data_channel_type_t* dct)
    {
        return reinterpret_cast<dcl::DataChannelType*>(dct);
    }

    inline const dcl::DataChannelType* toDataChannelType(const dnv_vista_sdk_dcl_data_channel_type_t* dct)
    {
        return reinterpret_cast<const dcl::DataChannelType*>(dct);
    }

    inline dnv_vista_sdk_dcl_data_channel_type_t* fromDataChannelType(dcl::DataChannelType&& dct)
    {
        return reinterpret_cast<dnv_vista_sdk_dcl_data_channel_type_t*>(new dcl::DataChannelType{ std::move(dct) });
    }

    inline dcl::NameObject* toNameObject(dnv_vista_sdk_dcl_name_object_t* n)
    {
        return reinterpret_cast<dcl::NameObject*>(n);
    }

    inline const dcl::NameObject* toNameObject(const dnv_vista_sdk_dcl_name_object_t* n)
    {
        return reinterpret_cast<const dcl::NameObject*>(n);
    }

    inline dnv_vista_sdk_dcl_name_object_t* fromNameObject(dcl::NameObject&& n)
    {
        return reinterpret_cast<dnv_vista_sdk_dcl_name_object_t*>(new dcl::NameObject{ std::move(n) });
    }

    inline dcl::Unit* toUnit(dnv_vista_sdk_dcl_unit_t* u)
    {
        return reinterpret_cast<dcl::Unit*>(u);
    }

    inline const dcl::Unit* toUnit(const dnv_vista_sdk_dcl_unit_t* u)
    {
        return reinterpret_cast<const dcl::Unit*>(u);
    }

    inline dnv_vista_sdk_dcl_unit_t* fromUnit(dcl::Unit&& u)
    {
        return reinterpret_cast<dnv_vista_sdk_dcl_unit_t*>(new dcl::Unit{ std::move(u) });
    }

    inline dcl::Property* toProperty(dnv_vista_sdk_dcl_property_t* p)
    {
        return reinterpret_cast<dcl::Property*>(p);
    }

    inline const dcl::Property* toProperty(const dnv_vista_sdk_dcl_property_t* p)
    {
        return reinterpret_cast<const dcl::Property*>(p);
    }

    inline dnv_vista_sdk_dcl_property_t* fromProperty(dcl::Property&& p)
    {
        return reinterpret_cast<dnv_vista_sdk_dcl_property_t*>(new dcl::Property{ std::move(p) });
    }

    inline dcl::ConfigurationReference* toConfigurationReference(dnv_vista_sdk_dcl_configuration_reference_t* c)
    {
        return reinterpret_cast<dcl::ConfigurationReference*>(c);
    }

    inline const dcl::ConfigurationReference* toConfigurationReference(
        const dnv_vista_sdk_dcl_configuration_reference_t* c)
    {
        return reinterpret_cast<const dcl::ConfigurationReference*>(c);
    }

    inline dnv_vista_sdk_dcl_configuration_reference_t* fromConfigurationReference(dcl::ConfigurationReference&& c)
    {
        return reinterpret_cast<dnv_vista_sdk_dcl_configuration_reference_t*>(
            new dcl::ConfigurationReference{ std::move(c) });
    }

    inline dcl::VersionInformation* toVersionInformation(dnv_vista_sdk_dcl_version_information_t* v)
    {
        return reinterpret_cast<dcl::VersionInformation*>(v);
    }

    inline const dcl::VersionInformation* toVersionInformation(const dnv_vista_sdk_dcl_version_information_t* v)
    {
        return reinterpret_cast<const dcl::VersionInformation*>(v);
    }

    inline dnv_vista_sdk_dcl_version_information_t* fromVersionInformation(dcl::VersionInformation&& v)
    {
        return reinterpret_cast<dnv_vista_sdk_dcl_version_information_t*>(new dcl::VersionInformation{ std::move(v) });
    }

    inline dcl::DataChannelId* toDclChannelId(dnv_vista_sdk_dcl_channel_id_t* id)
    {
        return reinterpret_cast<dcl::DataChannelId*>(id);
    }

    inline const dcl::DataChannelId* toDclChannelId(const dnv_vista_sdk_dcl_channel_id_t* id)
    {
        return reinterpret_cast<const dcl::DataChannelId*>(id);
    }

    inline dnv_vista_sdk_dcl_channel_id_t* fromDclChannelId(dcl::DataChannelId&& id)
    {
        return reinterpret_cast<dnv_vista_sdk_dcl_channel_id_t*>(new dcl::DataChannelId{ std::move(id) });
    }

    inline dcl::Header* toHeader(dnv_vista_sdk_dcl_header_t* h)
    {
        return reinterpret_cast<dcl::Header*>(h);
    }

    inline const dcl::Header* toHeader(const dnv_vista_sdk_dcl_header_t* h)
    {
        return reinterpret_cast<const dcl::Header*>(h);
    }

    inline dnv_vista_sdk_dcl_header_t* fromHeader(dcl::Header&& h)
    {
        return reinterpret_cast<dnv_vista_sdk_dcl_header_t*>(new dcl::Header{ std::move(h) });
    }

    inline dcl::DataChannel* toDclDataChannel(dnv_vista_sdk_dcl_data_channel_t* dc)
    {
        return reinterpret_cast<dcl::DataChannel*>(dc);
    }

    inline const dcl::DataChannel* toDclDataChannel(const dnv_vista_sdk_dcl_data_channel_t* dc)
    {
        return reinterpret_cast<const dcl::DataChannel*>(dc);
    }

    inline dnv_vista_sdk_dcl_data_channel_t* fromDclDataChannel(dcl::DataChannel&& dc)
    {
        return reinterpret_cast<dnv_vista_sdk_dcl_data_channel_t*>(new dcl::DataChannel{ std::move(dc) });
    }

    inline dcl::DataChannelList* toDataChannelList(dnv_vista_sdk_dcl_data_channel_list_t* list)
    {
        return reinterpret_cast<dcl::DataChannelList*>(list);
    }

    inline const dcl::DataChannelList* toDataChannelList(const dnv_vista_sdk_dcl_data_channel_list_t* list)
    {
        return reinterpret_cast<const dcl::DataChannelList*>(list);
    }

    inline dnv_vista_sdk_dcl_data_channel_list_t* fromDataChannelList(dcl::DataChannelList&& list)
    {
        return reinterpret_cast<dnv_vista_sdk_dcl_data_channel_list_t*>(new dcl::DataChannelList{ std::move(list) });
    }

    inline dcl::Package* toDclPackage(dnv_vista_sdk_dcl_package_t* p)
    {
        return reinterpret_cast<dcl::Package*>(p);
    }

    inline const dcl::Package* toDclPackage(const dnv_vista_sdk_dcl_package_t* p)
    {
        return reinterpret_cast<const dcl::Package*>(p);
    }

    inline dnv_vista_sdk_dcl_package_t* fromDclPackage(dcl::Package&& p)
    {
        return reinterpret_cast<dnv_vista_sdk_dcl_package_t*>(new dcl::Package{ std::move(p) });
    }

    inline dcl::DataChannelListPackage* toDataChannelListPackage(dnv_vista_sdk_dcl_list_package_t* p)
    {
        return reinterpret_cast<dcl::DataChannelListPackage*>(p);
    }

    inline const dcl::DataChannelListPackage* toDataChannelListPackage(const dnv_vista_sdk_dcl_list_package_t* p)
    {
        return reinterpret_cast<const dcl::DataChannelListPackage*>(p);
    }

    inline dnv_vista_sdk_dcl_list_package_t* fromDataChannelListPackage(dcl::DataChannelListPackage&& p)
    {
        return reinterpret_cast<dnv_vista_sdk_dcl_list_package_t*>(new dcl::DataChannelListPackage{ std::move(p) });
    }

    inline const transport::timeseries::DataChannelId* toTsdChannelId(const dnv_vista_sdk_tsd_channel_id_t* channelId)
    {
        return reinterpret_cast<const transport::timeseries::DataChannelId*>(channelId);
    }

    inline dnv_vista_sdk_tsd_channel_id_t* fromTsdChannelId(
        std::optional<transport::timeseries::DataChannelId>&& channelId)
    {
        if (!channelId.has_value())
        {
            return nullptr;
        }

        return reinterpret_cast<dnv_vista_sdk_tsd_channel_id_t*>(
            new transport::timeseries::DataChannelId{ std::move(*channelId) });
    }

    namespace tsd = transport::timeseries;

    inline tsd::TimeSpan* toTsdTimeSpan(dnv_vista_sdk_tsd_time_span_t* ts)
    {
        return reinterpret_cast<tsd::TimeSpan*>(ts);
    }

    inline const tsd::TimeSpan* toTsdTimeSpan(const dnv_vista_sdk_tsd_time_span_t* ts)
    {
        return reinterpret_cast<const tsd::TimeSpan*>(ts);
    }

    inline dnv_vista_sdk_tsd_time_span_t* fromTsdTimeSpan(tsd::TimeSpan&& ts)
    {
        return reinterpret_cast<dnv_vista_sdk_tsd_time_span_t*>(new tsd::TimeSpan{ std::move(ts) });
    }

    inline tsd::ConfigurationReference* toTsdConfigRef(dnv_vista_sdk_tsd_config_ref_t* c)
    {
        return reinterpret_cast<tsd::ConfigurationReference*>(c);
    }

    inline const tsd::ConfigurationReference* toTsdConfigRef(const dnv_vista_sdk_tsd_config_ref_t* c)
    {
        return reinterpret_cast<const tsd::ConfigurationReference*>(c);
    }

    inline dnv_vista_sdk_tsd_config_ref_t* fromTsdConfigRef(tsd::ConfigurationReference&& c)
    {
        return reinterpret_cast<dnv_vista_sdk_tsd_config_ref_t*>(new tsd::ConfigurationReference{ std::move(c) });
    }

    inline tsd::Header* toTsdHeader(dnv_vista_sdk_tsd_header_t* h)
    {
        return reinterpret_cast<tsd::Header*>(h);
    }

    inline const tsd::Header* toTsdHeader(const dnv_vista_sdk_tsd_header_t* h)
    {
        return reinterpret_cast<const tsd::Header*>(h);
    }

    inline dnv_vista_sdk_tsd_header_t* fromTsdHeader(tsd::Header&& h)
    {
        return reinterpret_cast<dnv_vista_sdk_tsd_header_t*>(new tsd::Header{ std::move(h) });
    }

    inline tsd::TabularDataSet* toTsdTabularDataSet(dnv_vista_sdk_tsd_tabular_data_set_t* d)
    {
        return reinterpret_cast<tsd::TabularDataSet*>(d);
    }

    inline const tsd::TabularDataSet* toTsdTabularDataSet(const dnv_vista_sdk_tsd_tabular_data_set_t* d)
    {
        return reinterpret_cast<const tsd::TabularDataSet*>(d);
    }

    inline dnv_vista_sdk_tsd_tabular_data_set_t* fromTsdTabularDataSet(tsd::TabularDataSet&& d)
    {
        return reinterpret_cast<dnv_vista_sdk_tsd_tabular_data_set_t*>(new tsd::TabularDataSet{ std::move(d) });
    }

    inline tsd::TabularData* toTsdTabularData(dnv_vista_sdk_tsd_tabular_data_t* d)
    {
        return reinterpret_cast<tsd::TabularData*>(d);
    }

    inline const tsd::TabularData* toTsdTabularData(const dnv_vista_sdk_tsd_tabular_data_t* d)
    {
        return reinterpret_cast<const tsd::TabularData*>(d);
    }

    inline dnv_vista_sdk_tsd_tabular_data_t* fromTsdTabularData(tsd::TabularData&& d)
    {
        return reinterpret_cast<dnv_vista_sdk_tsd_tabular_data_t*>(new tsd::TabularData{ std::move(d) });
    }

    inline tsd::EventDataSet* toTsdEventDataSet(dnv_vista_sdk_tsd_event_data_set_t* d)
    {
        return reinterpret_cast<tsd::EventDataSet*>(d);
    }

    inline const tsd::EventDataSet* toTsdEventDataSet(const dnv_vista_sdk_tsd_event_data_set_t* d)
    {
        return reinterpret_cast<const tsd::EventDataSet*>(d);
    }

    inline dnv_vista_sdk_tsd_event_data_set_t* fromTsdEventDataSet(tsd::EventDataSet&& d)
    {
        return reinterpret_cast<dnv_vista_sdk_tsd_event_data_set_t*>(new tsd::EventDataSet{ std::move(d) });
    }

    inline tsd::EventData* toTsdEventData(dnv_vista_sdk_tsd_event_data_t* d)
    {
        return reinterpret_cast<tsd::EventData*>(d);
    }

    inline const tsd::EventData* toTsdEventData(const dnv_vista_sdk_tsd_event_data_t* d)
    {
        return reinterpret_cast<const tsd::EventData*>(d);
    }

    inline dnv_vista_sdk_tsd_event_data_t* fromTsdEventData(tsd::EventData&& d)
    {
        return reinterpret_cast<dnv_vista_sdk_tsd_event_data_t*>(new tsd::EventData{ std::move(d) });
    }

    inline tsd::TimeSeriesData* toTsdTimeSeriesData(dnv_vista_sdk_tsd_time_series_data_t* d)
    {
        return reinterpret_cast<tsd::TimeSeriesData*>(d);
    }

    inline const tsd::TimeSeriesData* toTsdTimeSeriesData(const dnv_vista_sdk_tsd_time_series_data_t* d)
    {
        return reinterpret_cast<const tsd::TimeSeriesData*>(d);
    }

    inline dnv_vista_sdk_tsd_time_series_data_t* fromTsdTimeSeriesData(tsd::TimeSeriesData&& d)
    {
        return reinterpret_cast<dnv_vista_sdk_tsd_time_series_data_t*>(new tsd::TimeSeriesData{ std::move(d) });
    }

    inline tsd::Package* toTsdPackage(dnv_vista_sdk_tsd_package_t* p)
    {
        return reinterpret_cast<tsd::Package*>(p);
    }

    inline const tsd::Package* toTsdPackage(const dnv_vista_sdk_tsd_package_t* p)
    {
        return reinterpret_cast<const tsd::Package*>(p);
    }

    inline dnv_vista_sdk_tsd_package_t* fromTsdPackage(tsd::Package&& p)
    {
        return reinterpret_cast<dnv_vista_sdk_tsd_package_t*>(new tsd::Package{ std::move(p) });
    }

    inline tsd::TimeSeriesDataPackage* toTsdDataPackage(dnv_vista_sdk_tsd_data_package_t* p)
    {
        return reinterpret_cast<tsd::TimeSeriesDataPackage*>(p);
    }

    inline const tsd::TimeSeriesDataPackage* toTsdDataPackage(const dnv_vista_sdk_tsd_data_package_t* p)
    {
        return reinterpret_cast<const tsd::TimeSeriesDataPackage*>(p);
    }

    inline dnv_vista_sdk_tsd_data_package_t* fromTsdDataPackage(tsd::TimeSeriesDataPackage&& p)
    {
        return reinterpret_cast<dnv_vista_sdk_tsd_data_package_t*>(new tsd::TimeSeriesDataPackage{ std::move(p) });
    }
} // namespace dnv::vista::sdk::c
