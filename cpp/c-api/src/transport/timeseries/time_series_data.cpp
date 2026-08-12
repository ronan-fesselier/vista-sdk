#include "dnv/vista/sdk/c/transport/timeseries/time_series_data.h"

#include "../../cast_internal.h"
#include "../../error_internal.h"

#include <memory>
#include <vector>

using namespace dnv::vista::sdk;
using namespace dnv::vista::sdk::c;
namespace tsd = dnv::vista::sdk::transport::timeseries;

/*=====================================================================
 * TimeSpan
 *===================================================================*/

dnv_vista_sdk_tsd_time_span_t* dnv_vista_sdk_tsd_time_span_create(
    dnv_vista_sdk_date_time_offset_t start, dnv_vista_sdk_date_time_offset_t end)
{
    return c::cApiTryCatch<dnv_vista_sdk_tsd_time_span_t*>([&]() -> dnv_vista_sdk_tsd_time_span_t* {
        return fromTsdTimeSpan(tsd::TimeSpan{ toDateTimeOffset(start), toDateTimeOffset(end) });
    });
}

void dnv_vista_sdk_tsd_time_span_free(dnv_vista_sdk_tsd_time_span_t* timeSpan)
{
    delete reinterpret_cast<tsd::TimeSpan*>(timeSpan);
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_tsd_time_span_start(const dnv_vista_sdk_tsd_time_span_t* timeSpan)
{
    if (timeSpan == nullptr)
    {
        c::setLastError("timeSpan must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return dnv_vista_sdk_date_time_offset_t{};
    }

    return fromDateTimeOffset(toTsdTimeSpan(timeSpan)->start());
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_tsd_time_span_end(const dnv_vista_sdk_tsd_time_span_t* timeSpan)
{
    if (timeSpan == nullptr)
    {
        c::setLastError("timeSpan must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return dnv_vista_sdk_date_time_offset_t{};
    }

    return fromDateTimeOffset(toTsdTimeSpan(timeSpan)->end());
}

void dnv_vista_sdk_tsd_time_span_set_start(
    dnv_vista_sdk_tsd_time_span_t* timeSpan, dnv_vista_sdk_date_time_offset_t start)
{
    if (timeSpan == nullptr)
    {
        c::setLastError("timeSpan must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    c::cApiTryCatch<void>([&]() -> void { toTsdTimeSpan(timeSpan)->setStart(toDateTimeOffset(start)); });
}

void dnv_vista_sdk_tsd_time_span_set_end(dnv_vista_sdk_tsd_time_span_t* timeSpan, dnv_vista_sdk_date_time_offset_t end)
{
    if (timeSpan == nullptr)
    {
        c::setLastError("timeSpan must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    c::cApiTryCatch<void>([&]() -> void { toTsdTimeSpan(timeSpan)->setEnd(toDateTimeOffset(end)); });
}

/*=====================================================================
 * ConfigurationReference
 *===================================================================*/

dnv_vista_sdk_tsd_config_ref_t* dnv_vista_sdk_tsd_config_ref_create(
    const char* id, dnv_vista_sdk_date_time_offset_t timeStamp)
{
    if (id == nullptr)
    {
        c::setLastError("id must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromTsdConfigRef(tsd::ConfigurationReference{ std::string{ id }, toDateTimeOffset(timeStamp) });
}

void dnv_vista_sdk_tsd_config_ref_free(dnv_vista_sdk_tsd_config_ref_t* configRef)
{
    delete reinterpret_cast<tsd::ConfigurationReference*>(configRef);
}

const char* dnv_vista_sdk_tsd_config_ref_id(const dnv_vista_sdk_tsd_config_ref_t* configRef)
{
    if (configRef == nullptr)
    {
        c::setLastError("configRef must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toTsdConfigRef(configRef)->id().c_str();
}

void dnv_vista_sdk_tsd_config_ref_set_id(dnv_vista_sdk_tsd_config_ref_t* configRef, const char* id)
{
    if (configRef == nullptr || id == nullptr)
    {
        c::setLastError("configRef and id must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdConfigRef(configRef)->setId(std::string{ id });
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_tsd_config_ref_timestamp(const dnv_vista_sdk_tsd_config_ref_t* configRef)
{
    if (configRef == nullptr)
    {
        c::setLastError("configRef must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return dnv_vista_sdk_date_time_offset_t{};
    }

    return fromDateTimeOffset(toTsdConfigRef(configRef)->timeStamp());
}

void dnv_vista_sdk_tsd_config_ref_set_timestamp(
    dnv_vista_sdk_tsd_config_ref_t* configRef, dnv_vista_sdk_date_time_offset_t timeStamp)
{
    if (configRef == nullptr)
    {
        c::setLastError("configRef must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdConfigRef(configRef)->setTimeStamp(toDateTimeOffset(timeStamp));
}

/*=====================================================================
 * Header
 *===================================================================*/

dnv_vista_sdk_tsd_header_t* dnv_vista_sdk_tsd_header_create(const dnv_vista_sdk_ship_id_t* shipId)
{
    if (shipId == nullptr)
    {
        c::setLastError("shipId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromTsdHeader(tsd::Header{ *toShipId(shipId) });
}

void dnv_vista_sdk_tsd_header_free(dnv_vista_sdk_tsd_header_t* header)
{
    delete reinterpret_cast<tsd::Header*>(header);
}

const dnv_vista_sdk_ship_id_t* dnv_vista_sdk_tsd_header_ship_id(const dnv_vista_sdk_tsd_header_t* header)
{
    if (header == nullptr)
    {
        c::setLastError("header must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_ship_id_t*>(&toTsdHeader(header)->shipId());
}

void dnv_vista_sdk_tsd_header_set_ship_id(dnv_vista_sdk_tsd_header_t* header, const dnv_vista_sdk_ship_id_t* shipId)
{
    if (header == nullptr || shipId == nullptr)
    {
        c::setLastError("header and shipId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdHeader(header)->setShipId(*toShipId(shipId));
}

const dnv_vista_sdk_tsd_time_span_t* dnv_vista_sdk_tsd_header_time_span(const dnv_vista_sdk_tsd_header_t* header)
{
    if (header == nullptr || !toTsdHeader(header)->timeSpan().has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_tsd_time_span_t*>(&*toTsdHeader(header)->timeSpan());
}

void dnv_vista_sdk_tsd_header_set_time_span(
    dnv_vista_sdk_tsd_header_t* header, const dnv_vista_sdk_tsd_time_span_t* timeSpan)
{
    if (header == nullptr || timeSpan == nullptr)
    {
        c::setLastError("header and timeSpan must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdHeader(header)->setTimeSpan(*toTsdTimeSpan(timeSpan));
}

void dnv_vista_sdk_tsd_header_clear_time_span(dnv_vista_sdk_tsd_header_t* header)
{
    if (header == nullptr)
    {
        c::setLastError("header must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdHeader(header)->setTimeSpan(std::nullopt);
}

int dnv_vista_sdk_tsd_header_has_date_created(const dnv_vista_sdk_tsd_header_t* header)
{
    return header != nullptr && toTsdHeader(header)->dateCreated().has_value() ? 1 : 0;
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_tsd_header_date_created(const dnv_vista_sdk_tsd_header_t* header)
{
    if (header == nullptr || !toTsdHeader(header)->dateCreated().has_value())
    {
        c::setLastError("header must not be null and dateCreated must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return dnv_vista_sdk_date_time_offset_t{};
    }

    return fromDateTimeOffset(*toTsdHeader(header)->dateCreated());
}

void dnv_vista_sdk_tsd_header_set_date_created(
    dnv_vista_sdk_tsd_header_t* header, dnv_vista_sdk_date_time_offset_t dateCreated)
{
    if (header == nullptr)
    {
        c::setLastError("header must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdHeader(header)->setDateCreated(toDateTimeOffset(dateCreated));
}

void dnv_vista_sdk_tsd_header_clear_date_created(dnv_vista_sdk_tsd_header_t* header)
{
    if (header == nullptr)
    {
        c::setLastError("header must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdHeader(header)->setDateCreated(std::nullopt);
}

int dnv_vista_sdk_tsd_header_has_date_modified(const dnv_vista_sdk_tsd_header_t* header)
{
    return header != nullptr && toTsdHeader(header)->dateModified().has_value() ? 1 : 0;
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_tsd_header_date_modified(const dnv_vista_sdk_tsd_header_t* header)
{
    if (header == nullptr || !toTsdHeader(header)->dateModified().has_value())
    {
        c::setLastError("header must not be null and dateModified must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return dnv_vista_sdk_date_time_offset_t{};
    }

    return fromDateTimeOffset(*toTsdHeader(header)->dateModified());
}

void dnv_vista_sdk_tsd_header_set_date_modified(
    dnv_vista_sdk_tsd_header_t* header, dnv_vista_sdk_date_time_offset_t dateModified)
{
    if (header == nullptr)
    {
        c::setLastError("header must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdHeader(header)->setDateModified(toDateTimeOffset(dateModified));
}

void dnv_vista_sdk_tsd_header_clear_date_modified(dnv_vista_sdk_tsd_header_t* header)
{
    if (header == nullptr)
    {
        c::setLastError("header must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdHeader(header)->setDateModified(std::nullopt);
}

const char* dnv_vista_sdk_tsd_header_author(const dnv_vista_sdk_tsd_header_t* header)
{
    if (header == nullptr || !toTsdHeader(header)->author().has_value())
    {
        return nullptr;
    }

    return toTsdHeader(header)->author()->c_str();
}

void dnv_vista_sdk_tsd_header_set_author(dnv_vista_sdk_tsd_header_t* header, const char* author)
{
    if (header == nullptr || author == nullptr)
    {
        c::setLastError("header and author must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdHeader(header)->setAuthor(std::string{ author });
}

void dnv_vista_sdk_tsd_header_clear_author(dnv_vista_sdk_tsd_header_t* header)
{
    if (header == nullptr)
    {
        c::setLastError("header must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdHeader(header)->setAuthor(std::nullopt);
}

size_t dnv_vista_sdk_tsd_header_system_configuration_count(const dnv_vista_sdk_tsd_header_t* header)
{
    if (header == nullptr || !toTsdHeader(header)->systemConfiguration().has_value())
    {
        return 0;
    }

    return toTsdHeader(header)->systemConfiguration()->size();
}

const dnv_vista_sdk_tsd_config_ref_t* dnv_vista_sdk_tsd_header_system_configuration_at(
    const dnv_vista_sdk_tsd_header_t* header, size_t index)
{
    if (header == nullptr || !toTsdHeader(header)->systemConfiguration().has_value())
    {
        c::setLastError(
            "header must not be null and systemConfiguration must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& entries = *toTsdHeader(header)->systemConfiguration();
    if (index >= entries.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_tsd_config_ref_t*>(&entries[index]);
}

void dnv_vista_sdk_tsd_header_set_system_configuration(
    dnv_vista_sdk_tsd_header_t* header, const dnv_vista_sdk_tsd_config_ref_t* const* entries, size_t count)
{
    if (header == nullptr)
    {
        c::setLastError("header must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    std::vector<tsd::ConfigurationReference> result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i)
    {
        result.push_back(*toTsdConfigRef(entries[i]));
    }

    toTsdHeader(header)->setSystemConfiguration(std::move(result));
}

void dnv_vista_sdk_tsd_header_clear_system_configuration(dnv_vista_sdk_tsd_header_t* header)
{
    if (header == nullptr)
    {
        c::setLastError("header must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdHeader(header)->setSystemConfiguration(std::nullopt);
}

const dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_tsd_header_custom_headers(
    const dnv_vista_sdk_tsd_header_t* header)
{
    if (header == nullptr || !toTsdHeader(header)->customHeaders().has_value())
    {
        return nullptr;
    }

    return fromSerializableDocumentRef(*toTsdHeader(header)->customHeaders());
}

void dnv_vista_sdk_tsd_header_set_custom_headers(
    dnv_vista_sdk_tsd_header_t* header, dnv_vista_sdk_serializable_document_t* value)
{
    if (header == nullptr || value == nullptr)
    {
        c::setLastError("header and value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    std::unique_ptr<transport::serialization::json::SerializableDocument> owned{
        reinterpret_cast<transport::serialization::json::SerializableDocument*>(value)
    };
    toTsdHeader(header)->setCustomHeaders(std::move(*owned));
}

void dnv_vista_sdk_tsd_header_clear_custom_headers(dnv_vista_sdk_tsd_header_t* header)
{
    if (header == nullptr)
    {
        c::setLastError("header must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdHeader(header)->setCustomHeaders(std::nullopt);
}

/*=====================================================================
 * TabularDataSet
 *===================================================================*/

dnv_vista_sdk_tsd_tabular_data_set_t* dnv_vista_sdk_tsd_tabular_data_set_create(
    dnv_vista_sdk_date_time_offset_t timeStamp, const char* const* values, size_t valueCount)
{
    if (values == nullptr && valueCount != 0)
    {
        c::setLastError("values must not be null when valueCount is nonzero", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    std::vector<std::string> value;
    value.reserve(valueCount);
    for (size_t i = 0; i < valueCount; ++i)
    {
        value.emplace_back(values[i]);
    }

    return fromTsdTabularDataSet(tsd::TabularDataSet{ toDateTimeOffset(timeStamp), std::move(value) });
}

void dnv_vista_sdk_tsd_tabular_data_set_free(dnv_vista_sdk_tsd_tabular_data_set_t* dataSet)
{
    delete reinterpret_cast<tsd::TabularDataSet*>(dataSet);
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_tsd_tabular_data_set_time_stamp(
    const dnv_vista_sdk_tsd_tabular_data_set_t* dataSet)
{
    if (dataSet == nullptr)
    {
        c::setLastError("dataSet must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return dnv_vista_sdk_date_time_offset_t{};
    }

    return fromDateTimeOffset(toTsdTabularDataSet(dataSet)->timeStamp());
}

void dnv_vista_sdk_tsd_tabular_data_set_set_time_stamp(
    dnv_vista_sdk_tsd_tabular_data_set_t* dataSet, dnv_vista_sdk_date_time_offset_t timeStamp)
{
    if (dataSet == nullptr)
    {
        c::setLastError("dataSet must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdTabularDataSet(dataSet)->setTimeStamp(toDateTimeOffset(timeStamp));
}

size_t dnv_vista_sdk_tsd_tabular_data_set_value_count(const dnv_vista_sdk_tsd_tabular_data_set_t* dataSet)
{
    if (dataSet == nullptr)
    {
        return 0;
    }

    return toTsdTabularDataSet(dataSet)->value().size();
}

const char* dnv_vista_sdk_tsd_tabular_data_set_value_at(
    const dnv_vista_sdk_tsd_tabular_data_set_t* dataSet, size_t index)
{
    if (dataSet == nullptr)
    {
        c::setLastError("dataSet must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& values = toTsdTabularDataSet(dataSet)->value();
    if (index >= values.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return values[index].c_str();
}

void dnv_vista_sdk_tsd_tabular_data_set_set_values(
    dnv_vista_sdk_tsd_tabular_data_set_t* dataSet, const char* const* values, size_t count)
{
    if (dataSet == nullptr)
    {
        c::setLastError("dataSet must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    std::vector<std::string> value;
    value.reserve(count);
    for (size_t i = 0; i < count; ++i)
    {
        value.emplace_back(values[i]);
    }

    toTsdTabularDataSet(dataSet)->setValue(std::move(value));
}

size_t dnv_vista_sdk_tsd_tabular_data_set_quality_count(const dnv_vista_sdk_tsd_tabular_data_set_t* dataSet)
{
    if (dataSet == nullptr || !toTsdTabularDataSet(dataSet)->quality().has_value())
    {
        return 0;
    }

    return toTsdTabularDataSet(dataSet)->quality()->size();
}

const char* dnv_vista_sdk_tsd_tabular_data_set_quality_at(
    const dnv_vista_sdk_tsd_tabular_data_set_t* dataSet, size_t index)
{
    if (dataSet == nullptr || !toTsdTabularDataSet(dataSet)->quality().has_value())
    {
        c::setLastError("dataSet must not be null and quality must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& quality = *toTsdTabularDataSet(dataSet)->quality();
    if (index >= quality.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return quality[index].c_str();
}

void dnv_vista_sdk_tsd_tabular_data_set_set_quality(
    dnv_vista_sdk_tsd_tabular_data_set_t* dataSet, const char* const* quality, size_t count)
{
    if (dataSet == nullptr)
    {
        c::setLastError("dataSet must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    std::vector<std::string> result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i)
    {
        result.emplace_back(quality[i]);
    }

    toTsdTabularDataSet(dataSet)->setQuality(std::move(result));
}

void dnv_vista_sdk_tsd_tabular_data_set_clear_quality(dnv_vista_sdk_tsd_tabular_data_set_t* dataSet)
{
    if (dataSet == nullptr)
    {
        c::setLastError("dataSet must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdTabularDataSet(dataSet)->setQuality(std::nullopt);
}

/*=====================================================================
 * TabularData
 *===================================================================*/

dnv_vista_sdk_tsd_tabular_data_t* dnv_vista_sdk_tsd_tabular_data_create(
    const dnv_vista_sdk_tsd_channel_id_t* const* dataChannelIds,
    size_t dataChannelIdCount,
    const dnv_vista_sdk_tsd_tabular_data_set_t* const* dataSets,
    size_t dataSetCount)
{
    if ((dataChannelIds == nullptr && dataChannelIdCount != 0) || (dataSets == nullptr && dataSetCount != 0))
    {
        c::setLastError(
            "dataChannelIds and dataSets must not be null when their count is nonzero",
            DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    std::vector<tsd::DataChannelId> channelIds;
    channelIds.reserve(dataChannelIdCount);
    for (size_t i = 0; i < dataChannelIdCount; ++i)
    {
        channelIds.push_back(*toTsdChannelId(dataChannelIds[i]));
    }

    std::vector<tsd::TabularDataSet> dataSetsVec;
    dataSetsVec.reserve(dataSetCount);
    for (size_t i = 0; i < dataSetCount; ++i)
    {
        dataSetsVec.push_back(*toTsdTabularDataSet(dataSets[i]));
    }

    return fromTsdTabularData(tsd::TabularData{ std::move(channelIds), std::move(dataSetsVec) });
}

void dnv_vista_sdk_tsd_tabular_data_free(dnv_vista_sdk_tsd_tabular_data_t* tabularData)
{
    delete reinterpret_cast<tsd::TabularData*>(tabularData);
}

size_t dnv_vista_sdk_tsd_tabular_data_channel_id_count(const dnv_vista_sdk_tsd_tabular_data_t* tabularData)
{
    if (tabularData == nullptr)
    {
        return 0;
    }

    return toTsdTabularData(tabularData)->dataChannelIds().size();
}

const dnv_vista_sdk_tsd_channel_id_t* dnv_vista_sdk_tsd_tabular_data_channel_id_at(
    const dnv_vista_sdk_tsd_tabular_data_t* tabularData, size_t index)
{
    if (tabularData == nullptr)
    {
        c::setLastError("tabularData must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& ids = toTsdTabularData(tabularData)->dataChannelIds();
    if (index >= ids.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_tsd_channel_id_t*>(&ids[index]);
}

void dnv_vista_sdk_tsd_tabular_data_set_channel_ids(
    dnv_vista_sdk_tsd_tabular_data_t* tabularData,
    const dnv_vista_sdk_tsd_channel_id_t* const* dataChannelIds,
    size_t count)
{
    if (tabularData == nullptr)
    {
        c::setLastError("tabularData must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    std::vector<tsd::DataChannelId> channelIds;
    channelIds.reserve(count);
    for (size_t i = 0; i < count; ++i)
    {
        channelIds.push_back(*toTsdChannelId(dataChannelIds[i]));
    }

    toTsdTabularData(tabularData)->setDataChannelIds(std::move(channelIds));
}

size_t dnv_vista_sdk_tsd_tabular_data_data_set_count(const dnv_vista_sdk_tsd_tabular_data_t* tabularData)
{
    if (tabularData == nullptr)
    {
        return 0;
    }

    return toTsdTabularData(tabularData)->dataSets().size();
}

const dnv_vista_sdk_tsd_tabular_data_set_t* dnv_vista_sdk_tsd_tabular_data_data_set_at(
    const dnv_vista_sdk_tsd_tabular_data_t* tabularData, size_t index)
{
    if (tabularData == nullptr)
    {
        c::setLastError("tabularData must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& sets = toTsdTabularData(tabularData)->dataSets();
    if (index >= sets.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_tsd_tabular_data_set_t*>(&sets[index]);
}

void dnv_vista_sdk_tsd_tabular_data_set_data_sets(
    dnv_vista_sdk_tsd_tabular_data_t* tabularData,
    const dnv_vista_sdk_tsd_tabular_data_set_t* const* dataSets,
    size_t count)
{
    if (tabularData == nullptr)
    {
        c::setLastError("tabularData must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    std::vector<tsd::TabularDataSet> sets;
    sets.reserve(count);
    for (size_t i = 0; i < count; ++i)
    {
        sets.push_back(*toTsdTabularDataSet(dataSets[i]));
    }

    toTsdTabularData(tabularData)->setDataSets(std::move(sets));
}

int dnv_vista_sdk_tsd_tabular_data_validate(const dnv_vista_sdk_tsd_tabular_data_t* tabularData)
{
    if (tabularData == nullptr)
    {
        c::setLastError("tabularData must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    const auto result = toTsdTabularData(tabularData)->validate();
    if (!result)
    {
        c::setLastError(
            result.errors().empty() ? "validation failed" : result.errors().front(), DNV_VISTA_SDK_ERROR_DOMAIN);
        return 0;
    }

    return 1;
}

/*=====================================================================
 * EventDataSet
 *===================================================================*/

dnv_vista_sdk_tsd_event_data_set_t* dnv_vista_sdk_tsd_event_data_set_create(
    dnv_vista_sdk_date_time_offset_t timeStamp, const dnv_vista_sdk_tsd_channel_id_t* dataChannelId, const char* value)
{
    if (dataChannelId == nullptr || value == nullptr)
    {
        c::setLastError("dataChannelId and value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromTsdEventDataSet(
        tsd::EventDataSet{ toDateTimeOffset(timeStamp), *toTsdChannelId(dataChannelId), std::string{ value } });
}

void dnv_vista_sdk_tsd_event_data_set_free(dnv_vista_sdk_tsd_event_data_set_t* dataSet)
{
    delete reinterpret_cast<tsd::EventDataSet*>(dataSet);
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_tsd_event_data_set_time_stamp(
    const dnv_vista_sdk_tsd_event_data_set_t* dataSet)
{
    if (dataSet == nullptr)
    {
        c::setLastError("dataSet must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return dnv_vista_sdk_date_time_offset_t{};
    }

    return fromDateTimeOffset(toTsdEventDataSet(dataSet)->timeStamp());
}

void dnv_vista_sdk_tsd_event_data_set_set_time_stamp(
    dnv_vista_sdk_tsd_event_data_set_t* dataSet, dnv_vista_sdk_date_time_offset_t timeStamp)
{
    if (dataSet == nullptr)
    {
        c::setLastError("dataSet must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdEventDataSet(dataSet)->setTimeStamp(toDateTimeOffset(timeStamp));
}

const dnv_vista_sdk_tsd_channel_id_t* dnv_vista_sdk_tsd_event_data_set_data_channel_id(
    const dnv_vista_sdk_tsd_event_data_set_t* dataSet)
{
    if (dataSet == nullptr)
    {
        c::setLastError("dataSet must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_tsd_channel_id_t*>(&toTsdEventDataSet(dataSet)->dataChannelId());
}

void dnv_vista_sdk_tsd_event_data_set_set_data_channel_id(
    dnv_vista_sdk_tsd_event_data_set_t* dataSet, const dnv_vista_sdk_tsd_channel_id_t* dataChannelId)
{
    if (dataSet == nullptr || dataChannelId == nullptr)
    {
        c::setLastError("dataSet and dataChannelId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdEventDataSet(dataSet)->setDataChannelId(*toTsdChannelId(dataChannelId));
}

const char* dnv_vista_sdk_tsd_event_data_set_value(const dnv_vista_sdk_tsd_event_data_set_t* dataSet)
{
    if (dataSet == nullptr)
    {
        c::setLastError("dataSet must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toTsdEventDataSet(dataSet)->value().c_str();
}

void dnv_vista_sdk_tsd_event_data_set_set_value(dnv_vista_sdk_tsd_event_data_set_t* dataSet, const char* value)
{
    if (dataSet == nullptr || value == nullptr)
    {
        c::setLastError("dataSet and value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdEventDataSet(dataSet)->setValue(std::string{ value });
}

const char* dnv_vista_sdk_tsd_event_data_set_quality(const dnv_vista_sdk_tsd_event_data_set_t* dataSet)
{
    if (dataSet == nullptr || !toTsdEventDataSet(dataSet)->quality().has_value())
    {
        return nullptr;
    }

    return toTsdEventDataSet(dataSet)->quality()->c_str();
}

void dnv_vista_sdk_tsd_event_data_set_set_quality(dnv_vista_sdk_tsd_event_data_set_t* dataSet, const char* quality)
{
    if (dataSet == nullptr || quality == nullptr)
    {
        c::setLastError("dataSet and quality must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdEventDataSet(dataSet)->setQuality(std::string{ quality });
}

void dnv_vista_sdk_tsd_event_data_set_clear_quality(dnv_vista_sdk_tsd_event_data_set_t* dataSet)
{
    if (dataSet == nullptr)
    {
        c::setLastError("dataSet must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdEventDataSet(dataSet)->setQuality(std::nullopt);
}

/*=====================================================================
 * EventData
 *===================================================================*/

dnv_vista_sdk_tsd_event_data_t* dnv_vista_sdk_tsd_event_data_create(void)
{
    return fromTsdEventData(tsd::EventData{});
}

void dnv_vista_sdk_tsd_event_data_free(dnv_vista_sdk_tsd_event_data_t* eventData)
{
    delete reinterpret_cast<tsd::EventData*>(eventData);
}

size_t dnv_vista_sdk_tsd_event_data_data_set_count(const dnv_vista_sdk_tsd_event_data_t* eventData)
{
    if (eventData == nullptr || !toTsdEventData(eventData)->dataSet().has_value())
    {
        return 0;
    }

    return toTsdEventData(eventData)->dataSet()->size();
}

const dnv_vista_sdk_tsd_event_data_set_t* dnv_vista_sdk_tsd_event_data_data_set_at(
    const dnv_vista_sdk_tsd_event_data_t* eventData, size_t index)
{
    if (eventData == nullptr || !toTsdEventData(eventData)->dataSet().has_value())
    {
        c::setLastError("eventData must not be null and dataSet must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& sets = *toTsdEventData(eventData)->dataSet();
    if (index >= sets.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_tsd_event_data_set_t*>(&sets[index]);
}

void dnv_vista_sdk_tsd_event_data_set_data_set(
    dnv_vista_sdk_tsd_event_data_t* eventData, const dnv_vista_sdk_tsd_event_data_set_t* const* dataSets, size_t count)
{
    if (eventData == nullptr)
    {
        c::setLastError("eventData must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    std::vector<tsd::EventDataSet> sets;
    sets.reserve(count);
    for (size_t i = 0; i < count; ++i)
    {
        sets.push_back(*toTsdEventDataSet(dataSets[i]));
    }

    toTsdEventData(eventData)->setDataSet(std::move(sets));
}

void dnv_vista_sdk_tsd_event_data_clear_data_set(dnv_vista_sdk_tsd_event_data_t* eventData)
{
    if (eventData == nullptr)
    {
        c::setLastError("eventData must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdEventData(eventData)->setDataSet(std::nullopt);
}

/*=====================================================================
 * TimeSeriesData
 *===================================================================*/

dnv_vista_sdk_tsd_time_series_data_t* dnv_vista_sdk_tsd_time_series_data_create(void)
{
    return fromTsdTimeSeriesData(tsd::TimeSeriesData{});
}

void dnv_vista_sdk_tsd_time_series_data_free(dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData)
{
    delete reinterpret_cast<tsd::TimeSeriesData*>(timeSeriesData);
}

const dnv_vista_sdk_tsd_config_ref_t* dnv_vista_sdk_tsd_time_series_data_data_configuration(
    const dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData)
{
    if (timeSeriesData == nullptr || !toTsdTimeSeriesData(timeSeriesData)->dataConfiguration().has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_tsd_config_ref_t*>(
        &*toTsdTimeSeriesData(timeSeriesData)->dataConfiguration());
}

void dnv_vista_sdk_tsd_time_series_data_set_data_configuration(
    dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData, const dnv_vista_sdk_tsd_config_ref_t* configRef)
{
    if (timeSeriesData == nullptr || configRef == nullptr)
    {
        c::setLastError("timeSeriesData and configRef must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdTimeSeriesData(timeSeriesData)->setDataConfiguration(*toTsdConfigRef(configRef));
}

void dnv_vista_sdk_tsd_time_series_data_clear_data_configuration(dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData)
{
    if (timeSeriesData == nullptr)
    {
        c::setLastError("timeSeriesData must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdTimeSeriesData(timeSeriesData)->setDataConfiguration(std::nullopt);
}

size_t dnv_vista_sdk_tsd_time_series_data_tabular_data_count(const dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData)
{
    if (timeSeriesData == nullptr || !toTsdTimeSeriesData(timeSeriesData)->tabularData().has_value())
    {
        return 0;
    }

    return toTsdTimeSeriesData(timeSeriesData)->tabularData()->size();
}

const dnv_vista_sdk_tsd_tabular_data_t* dnv_vista_sdk_tsd_time_series_data_tabular_data_at(
    const dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData, size_t index)
{
    if (timeSeriesData == nullptr || !toTsdTimeSeriesData(timeSeriesData)->tabularData().has_value())
    {
        c::setLastError(
            "timeSeriesData must not be null and tabularData must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& entries = *toTsdTimeSeriesData(timeSeriesData)->tabularData();
    if (index >= entries.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_tsd_tabular_data_t*>(&entries[index]);
}

void dnv_vista_sdk_tsd_time_series_data_set_tabular_data(
    dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData,
    const dnv_vista_sdk_tsd_tabular_data_t* const* entries,
    size_t count)
{
    if (timeSeriesData == nullptr)
    {
        c::setLastError("timeSeriesData must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    std::vector<tsd::TabularData> result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i)
    {
        result.push_back(*toTsdTabularData(entries[i]));
    }

    toTsdTimeSeriesData(timeSeriesData)->setTabularData(std::move(result));
}

void dnv_vista_sdk_tsd_time_series_data_clear_tabular_data(dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData)
{
    if (timeSeriesData == nullptr)
    {
        c::setLastError("timeSeriesData must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdTimeSeriesData(timeSeriesData)->setTabularData(std::nullopt);
}

const dnv_vista_sdk_tsd_event_data_t* dnv_vista_sdk_tsd_time_series_data_event_data(
    const dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData)
{
    if (timeSeriesData == nullptr || !toTsdTimeSeriesData(timeSeriesData)->eventData().has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_tsd_event_data_t*>(&*toTsdTimeSeriesData(timeSeriesData)->eventData());
}

void dnv_vista_sdk_tsd_time_series_data_set_event_data(
    dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData, const dnv_vista_sdk_tsd_event_data_t* eventData)
{
    if (timeSeriesData == nullptr || eventData == nullptr)
    {
        c::setLastError("timeSeriesData and eventData must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdTimeSeriesData(timeSeriesData)->setEventData(*toTsdEventData(eventData));
}

void dnv_vista_sdk_tsd_time_series_data_clear_event_data(dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData)
{
    if (timeSeriesData == nullptr)
    {
        c::setLastError("timeSeriesData must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdTimeSeriesData(timeSeriesData)->setEventData(std::nullopt);
}

const dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_tsd_time_series_data_custom_data_kinds(
    const dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData)
{
    if (timeSeriesData == nullptr || !toTsdTimeSeriesData(timeSeriesData)->customDataKinds().has_value())
    {
        return nullptr;
    }

    return fromSerializableDocumentRef(*toTsdTimeSeriesData(timeSeriesData)->customDataKinds());
}

void dnv_vista_sdk_tsd_time_series_data_set_custom_data_kinds(
    dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData, dnv_vista_sdk_serializable_document_t* value)
{
    if (timeSeriesData == nullptr || value == nullptr)
    {
        c::setLastError("timeSeriesData and value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    std::unique_ptr<transport::serialization::json::SerializableDocument> owned{
        reinterpret_cast<transport::serialization::json::SerializableDocument*>(value)
    };
    toTsdTimeSeriesData(timeSeriesData)->setCustomDataKinds(std::move(*owned));
}

void dnv_vista_sdk_tsd_time_series_data_clear_custom_data_kinds(dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData)
{
    if (timeSeriesData == nullptr)
    {
        c::setLastError("timeSeriesData must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdTimeSeriesData(timeSeriesData)->setCustomDataKinds(std::nullopt);
}

namespace
{
    transport::ValidateResult<> invokeTsdValidateCallback(
        dnv_vista_sdk_tsd_validate_callback_t callback,
        void* userdata,
        const DateTimeOffset& timeStamp,
        const transport::datachannel::DataChannel& dataChannel,
        const transport::Value& value,
        const std::optional<std::string>& quality)
    {
        const char* errorMessage = nullptr;
        const int ok = callback(
            fromDateTimeOffset(timeStamp),
            reinterpret_cast<const dnv_vista_sdk_dcl_data_channel_t*>(&dataChannel),
            reinterpret_cast<const dnv_vista_sdk_iso19848_value_t*>(&value),
            quality.has_value() ? quality->c_str() : nullptr,
            &errorMessage,
            userdata);

        if (ok != 0)
        {
            return transport::ValidateResult<>::ok();
        }

        return transport::ValidateResult<>::invalid(
            errorMessage != nullptr ? std::string{ errorMessage } : std::string{ "custom validation failed" });
    }
} // namespace

int dnv_vista_sdk_tsd_time_series_data_validate(
    const dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData,
    const dnv_vista_sdk_dcl_list_package_t* dcPackage,
    dnv_vista_sdk_tsd_validate_callback_t onTabularData,
    void* tabularUserdata,
    dnv_vista_sdk_tsd_validate_callback_t onEventData,
    void* eventUserdata)
{
    if (timeSeriesData == nullptr || dcPackage == nullptr || onTabularData == nullptr || onEventData == nullptr)
    {
        c::setLastError(
            "timeSeriesData, dcPackage, onTabularData and onEventData must not be null",
            DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return c::cApiTryCatch<int>([&]() -> int {
        const auto result = toTsdTimeSeriesData(timeSeriesData)
                                ->validate(
                                    *toDataChannelListPackage(dcPackage),
                                    [onTabularData, tabularUserdata](
                                        const DateTimeOffset& timeStamp,
                                        const transport::datachannel::DataChannel& dataChannel,
                                        const transport::Value& value,
                                        const std::optional<std::string>& quality) {
                                        return invokeTsdValidateCallback(
                                            onTabularData, tabularUserdata, timeStamp, dataChannel, value, quality);
                                    },
                                    [onEventData, eventUserdata](
                                        const DateTimeOffset& timeStamp,
                                        const transport::datachannel::DataChannel& dataChannel,
                                        const transport::Value& value,
                                        const std::optional<std::string>& quality) {
                                        return invokeTsdValidateCallback(
                                            onEventData, eventUserdata, timeStamp, dataChannel, value, quality);
                                    });

        if (!result)
        {
            c::setLastError(
                result.errors().empty() ? "validation failed" : result.errors().front(), DNV_VISTA_SDK_ERROR_DOMAIN);
            return 0;
        }

        return 1;
    });
}

/*=====================================================================
 * Package
 *===================================================================*/

dnv_vista_sdk_tsd_package_t* dnv_vista_sdk_tsd_package_create(
    const dnv_vista_sdk_tsd_header_t* header,
    const dnv_vista_sdk_tsd_time_series_data_t* const* timeSeriesData,
    size_t count)
{
    if (timeSeriesData == nullptr && count != 0)
    {
        c::setLastError("timeSeriesData must not be null when count is nonzero", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    std::optional<tsd::Header> headerOpt;
    if (header != nullptr)
    {
        headerOpt = *toTsdHeader(header);
    }

    std::vector<tsd::TimeSeriesData> entries;
    entries.reserve(count);
    for (size_t i = 0; i < count; ++i)
    {
        entries.push_back(*toTsdTimeSeriesData(timeSeriesData[i]));
    }

    return fromTsdPackage(tsd::Package{ std::move(headerOpt), std::move(entries) });
}

void dnv_vista_sdk_tsd_package_free(dnv_vista_sdk_tsd_package_t* package)
{
    delete reinterpret_cast<tsd::Package*>(package);
}

const dnv_vista_sdk_tsd_header_t* dnv_vista_sdk_tsd_package_header(const dnv_vista_sdk_tsd_package_t* package)
{
    if (package == nullptr || !toTsdPackage(package)->header().has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_tsd_header_t*>(&*toTsdPackage(package)->header());
}

void dnv_vista_sdk_tsd_package_set_header(
    dnv_vista_sdk_tsd_package_t* package, const dnv_vista_sdk_tsd_header_t* header)
{
    if (package == nullptr)
    {
        c::setLastError("package must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    if (header == nullptr)
    {
        toTsdPackage(package)->setHeader(std::nullopt);
        return;
    }

    toTsdPackage(package)->setHeader(*toTsdHeader(header));
}

size_t dnv_vista_sdk_tsd_package_time_series_data_count(const dnv_vista_sdk_tsd_package_t* package)
{
    if (package == nullptr)
    {
        return 0;
    }

    return toTsdPackage(package)->timeSeriesData().size();
}

const dnv_vista_sdk_tsd_time_series_data_t* dnv_vista_sdk_tsd_package_time_series_data_at(
    const dnv_vista_sdk_tsd_package_t* package, size_t index)
{
    if (package == nullptr)
    {
        c::setLastError("package must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& entries = toTsdPackage(package)->timeSeriesData();
    if (index >= entries.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_tsd_time_series_data_t*>(&entries[index]);
}

void dnv_vista_sdk_tsd_package_set_time_series_data(
    dnv_vista_sdk_tsd_package_t* package, const dnv_vista_sdk_tsd_time_series_data_t* const* entries, size_t count)
{
    if (package == nullptr)
    {
        c::setLastError("package must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    std::vector<tsd::TimeSeriesData> result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i)
    {
        result.push_back(*toTsdTimeSeriesData(entries[i]));
    }

    toTsdPackage(package)->setTimeSeriesData(std::move(result));
}

/*=====================================================================
 * TimeSeriesDataPackage
 *===================================================================*/

dnv_vista_sdk_tsd_data_package_t* dnv_vista_sdk_tsd_data_package_create(const dnv_vista_sdk_tsd_package_t* package)
{
    if (package == nullptr)
    {
        c::setLastError("package must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromTsdDataPackage(tsd::TimeSeriesDataPackage{ *toTsdPackage(package) });
}

void dnv_vista_sdk_tsd_data_package_free(dnv_vista_sdk_tsd_data_package_t* dataPackage)
{
    delete reinterpret_cast<tsd::TimeSeriesDataPackage*>(dataPackage);
}

const dnv_vista_sdk_tsd_package_t* dnv_vista_sdk_tsd_data_package_package(
    const dnv_vista_sdk_tsd_data_package_t* dataPackage)
{
    if (dataPackage == nullptr)
    {
        c::setLastError("dataPackage must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_tsd_package_t*>(&toTsdDataPackage(dataPackage)->package());
}

void dnv_vista_sdk_tsd_data_package_set_package(
    dnv_vista_sdk_tsd_data_package_t* dataPackage, const dnv_vista_sdk_tsd_package_t* package)
{
    if (dataPackage == nullptr || package == nullptr)
    {
        c::setLastError("dataPackage and package must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toTsdDataPackage(dataPackage)->setPackage(*toTsdPackage(package));
}
