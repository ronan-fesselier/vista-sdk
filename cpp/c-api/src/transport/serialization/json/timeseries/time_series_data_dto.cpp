#include "dnv/vista/sdk/c/transport/serialization/json/timeseries/time_series_data_dto.h"

#include "../../../../cast_internal.h"
#include "../../../../error_internal.h"

#include <vector>

using namespace dnv::vista::sdk;
using namespace dnv::vista::sdk::c;
namespace tsdjson = dnv::vista::sdk::transport::serialization::json::timeseries;
namespace serjson = dnv::vista::sdk::transport::serialization::json;

/*=========================================================================
 * Internal cast helpers
 *=======================================================================*/

static inline tsdjson::TimeSeriesDataPackageDto* toDtoPkg(dnv_vista_sdk_tsd_dto_package_t* p)
{
    return reinterpret_cast<tsdjson::TimeSeriesDataPackageDto*>(p);
}

static inline const tsdjson::TimeSeriesDataPackageDto* toDtoPkg(const dnv_vista_sdk_tsd_dto_package_t* p)
{
    return reinterpret_cast<const tsdjson::TimeSeriesDataPackageDto*>(p);
}

static inline dnv_vista_sdk_tsd_dto_package_t* fromDtoPkg(tsdjson::TimeSeriesDataPackageDto* p)
{
    return reinterpret_cast<dnv_vista_sdk_tsd_dto_package_t*>(p);
}

static inline tsdjson::PackageDto* toPkg(dnv_vista_sdk_tsd_dto_pkg_t* p)
{
    return reinterpret_cast<tsdjson::PackageDto*>(p);
}

static inline const tsdjson::PackageDto* toPkg(const dnv_vista_sdk_tsd_dto_pkg_t* p)
{
    return reinterpret_cast<const tsdjson::PackageDto*>(p);
}

static inline dnv_vista_sdk_tsd_dto_pkg_t* fromPkgRef(tsdjson::PackageDto& p)
{
    return reinterpret_cast<dnv_vista_sdk_tsd_dto_pkg_t*>(&p);
}

static inline tsdjson::HeaderDto* toHeader(dnv_vista_sdk_tsd_dto_header_t* h)
{
    return reinterpret_cast<tsdjson::HeaderDto*>(h);
}

static inline const tsdjson::HeaderDto* toHeader(const dnv_vista_sdk_tsd_dto_header_t* h)
{
    return reinterpret_cast<const tsdjson::HeaderDto*>(h);
}

static inline dnv_vista_sdk_tsd_dto_header_t* fromHeaderRef(tsdjson::HeaderDto& h)
{
    return reinterpret_cast<dnv_vista_sdk_tsd_dto_header_t*>(&h);
}

static inline tsdjson::TimeSpanDto* toTimeSpan(dnv_vista_sdk_tsd_dto_time_span_t* ts)
{
    return reinterpret_cast<tsdjson::TimeSpanDto*>(ts);
}

static inline const tsdjson::TimeSpanDto* toTimeSpan(const dnv_vista_sdk_tsd_dto_time_span_t* ts)
{
    return reinterpret_cast<const tsdjson::TimeSpanDto*>(ts);
}

static inline dnv_vista_sdk_tsd_dto_time_span_t* fromTimeSpanRef(tsdjson::TimeSpanDto& ts)
{
    return reinterpret_cast<dnv_vista_sdk_tsd_dto_time_span_t*>(&ts);
}

static inline tsdjson::ConfigurationReferenceDto* toCfgRef(dnv_vista_sdk_tsd_dto_cfg_ref_t* r)
{
    return reinterpret_cast<tsdjson::ConfigurationReferenceDto*>(r);
}

static inline const tsdjson::ConfigurationReferenceDto* toCfgRef(const dnv_vista_sdk_tsd_dto_cfg_ref_t* r)
{
    return reinterpret_cast<const tsdjson::ConfigurationReferenceDto*>(r);
}

static inline dnv_vista_sdk_tsd_dto_cfg_ref_t* fromCfgRefRef(tsdjson::ConfigurationReferenceDto& r)
{
    return reinterpret_cast<dnv_vista_sdk_tsd_dto_cfg_ref_t*>(&r);
}

static inline tsdjson::TimeSeriesDataDto* toTsd(dnv_vista_sdk_tsd_dto_tsd_t* t)
{
    return reinterpret_cast<tsdjson::TimeSeriesDataDto*>(t);
}

static inline const tsdjson::TimeSeriesDataDto* toTsd(const dnv_vista_sdk_tsd_dto_tsd_t* t)
{
    return reinterpret_cast<const tsdjson::TimeSeriesDataDto*>(t);
}

static inline dnv_vista_sdk_tsd_dto_tsd_t* fromTsdRef(tsdjson::TimeSeriesDataDto& t)
{
    return reinterpret_cast<dnv_vista_sdk_tsd_dto_tsd_t*>(&t);
}

static inline tsdjson::TabularDataDto* toTabular(dnv_vista_sdk_tsd_dto_tabular_t* t)
{
    return reinterpret_cast<tsdjson::TabularDataDto*>(t);
}

static inline const tsdjson::TabularDataDto* toTabular(const dnv_vista_sdk_tsd_dto_tabular_t* t)
{
    return reinterpret_cast<const tsdjson::TabularDataDto*>(t);
}

static inline dnv_vista_sdk_tsd_dto_tabular_t* fromTabularRef(tsdjson::TabularDataDto& t)
{
    return reinterpret_cast<dnv_vista_sdk_tsd_dto_tabular_t*>(&t);
}

static inline tsdjson::TabularDataSetDto* toTabSet(dnv_vista_sdk_tsd_dto_tab_set_t* s)
{
    return reinterpret_cast<tsdjson::TabularDataSetDto*>(s);
}

static inline const tsdjson::TabularDataSetDto* toTabSet(const dnv_vista_sdk_tsd_dto_tab_set_t* s)
{
    return reinterpret_cast<const tsdjson::TabularDataSetDto*>(s);
}

static inline dnv_vista_sdk_tsd_dto_tab_set_t* fromTabSetRef(tsdjson::TabularDataSetDto& s)
{
    return reinterpret_cast<dnv_vista_sdk_tsd_dto_tab_set_t*>(&s);
}

static inline tsdjson::EventDataDto* toEvent(dnv_vista_sdk_tsd_dto_event_t* e)
{
    return reinterpret_cast<tsdjson::EventDataDto*>(e);
}

static inline const tsdjson::EventDataDto* toEvent(const dnv_vista_sdk_tsd_dto_event_t* e)
{
    return reinterpret_cast<const tsdjson::EventDataDto*>(e);
}

static inline dnv_vista_sdk_tsd_dto_event_t* fromEventRef(tsdjson::EventDataDto& e)
{
    return reinterpret_cast<dnv_vista_sdk_tsd_dto_event_t*>(&e);
}

static inline tsdjson::EventDataSetDto* toEventSet(dnv_vista_sdk_tsd_dto_event_set_t* s)
{
    return reinterpret_cast<tsdjson::EventDataSetDto*>(s);
}

static inline const tsdjson::EventDataSetDto* toEventSet(const dnv_vista_sdk_tsd_dto_event_set_t* s)
{
    return reinterpret_cast<const tsdjson::EventDataSetDto*>(s);
}

static inline dnv_vista_sdk_tsd_dto_event_set_t* fromEventSetRef(tsdjson::EventDataSetDto& s)
{
    return reinterpret_cast<dnv_vista_sdk_tsd_dto_event_set_t*>(&s);
}

/*=========================================================================
 * Root: TimeSeriesDataPackageDto
 *=======================================================================*/

dnv_vista_sdk_tsd_dto_package_t* dnv_vista_sdk_tsd_to_dto(const dnv_vista_sdk_tsd_data_package_t* domain)
{
    if (domain == nullptr)
    {
        c::setLastError("domain must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_tsd_dto_package_t*>([&]() {
        auto* p = new tsdjson::TimeSeriesDataPackageDto;
        *p = tsdjson::toDto(*toTsdDataPackage(domain));
        return fromDtoPkg(p);
    });
}

dnv_vista_sdk_tsd_data_package_t* dnv_vista_sdk_tsd_to_domain(const dnv_vista_sdk_tsd_dto_package_t* dto)
{
    if (dto == nullptr)
    {
        c::setLastError("dto must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_tsd_data_package_t*>(
        [&]() { return fromTsdDataPackage(tsdjson::toDomain(*toDtoPkg(dto))); });
}

char* dnv_vista_sdk_tsd_dto_to_json(const dnv_vista_sdk_tsd_dto_package_t* dto, int prettyPrint)
{
    if (dto == nullptr)
    {
        c::setLastError("dto must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<char*>(
        [&]() { return toOwnedCString(tsdjson::toJsonString(*toDtoPkg(dto), prettyPrint != 0)); });
}

dnv_vista_sdk_tsd_dto_package_t* dnv_vista_sdk_tsd_dto_from_json(const char* json)
{
    if (json == nullptr)
    {
        c::setLastError("json must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_tsd_dto_package_t*>([&]() -> dnv_vista_sdk_tsd_dto_package_t* {
        auto opt = tsdjson::fromJsonString(json);
        if (!opt.has_value())
        {
            c::setLastError("invalid TimeSeriesDataPackage JSON", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
            return nullptr;
        }

        auto* p = new tsdjson::TimeSeriesDataPackageDto;
        *p = std::move(*opt);
        return fromDtoPkg(p);
    });
}

void dnv_vista_sdk_tsd_dto_package_free(dnv_vista_sdk_tsd_dto_package_t* p)
{
    delete toDtoPkg(p);
}

dnv_vista_sdk_tsd_dto_pkg_t* dnv_vista_sdk_tsd_dto_package_get_pkg(dnv_vista_sdk_tsd_dto_package_t* p)
{
    if (p == nullptr)
    {
        return nullptr;
    }

    return fromPkgRef(toDtoPkg(p)->package);
}

/*=========================================================================
 * PackageDto
 *=======================================================================*/

int dnv_vista_sdk_tsd_dto_pkg_has_header(const dnv_vista_sdk_tsd_dto_pkg_t* pkg)
{
    if (pkg == nullptr)
    {
        return 0;
    }

    return toPkg(pkg)->header.has_value() ? 1 : 0;
}

dnv_vista_sdk_tsd_dto_header_t* dnv_vista_sdk_tsd_dto_pkg_get_header(dnv_vista_sdk_tsd_dto_pkg_t* pkg)
{
    if (pkg == nullptr || !toPkg(pkg)->header.has_value())
    {
        return nullptr;
    }

    return fromHeaderRef(*toPkg(pkg)->header);
}

void dnv_vista_sdk_tsd_dto_pkg_ensure_header(dnv_vista_sdk_tsd_dto_pkg_t* pkg)
{
    if (pkg == nullptr)
    {
        return;
    }

    if (!toPkg(pkg)->header.has_value())
    {
        toPkg(pkg)->header.emplace();
    }
}

void dnv_vista_sdk_tsd_dto_pkg_clear_header(dnv_vista_sdk_tsd_dto_pkg_t* pkg)
{
    if (pkg == nullptr)
    {
        return;
    }

    toPkg(pkg)->header.reset();
}

size_t dnv_vista_sdk_tsd_dto_pkg_tsd_count(const dnv_vista_sdk_tsd_dto_pkg_t* pkg)
{
    if (pkg == nullptr)
    {
        return 0;
    }

    return toPkg(pkg)->timeSeriesData.size();
}

dnv_vista_sdk_tsd_dto_tsd_t* dnv_vista_sdk_tsd_dto_pkg_tsd_at(dnv_vista_sdk_tsd_dto_pkg_t* pkg, size_t index)
{
    if (pkg == nullptr)
    {
        return nullptr;
    }

    auto& entries = toPkg(pkg)->timeSeriesData;
    if (index >= entries.size())
    {
        return nullptr;
    }

    return fromTsdRef(entries[index]);
}

dnv_vista_sdk_tsd_dto_tsd_t* dnv_vista_sdk_tsd_dto_pkg_tsd_push(dnv_vista_sdk_tsd_dto_pkg_t* pkg)
{
    if (pkg == nullptr)
    {
        return nullptr;
    }

    toPkg(pkg)->timeSeriesData.emplace_back();
    return fromTsdRef(toPkg(pkg)->timeSeriesData.back());
}

void dnv_vista_sdk_tsd_dto_pkg_tsd_remove(dnv_vista_sdk_tsd_dto_pkg_t* pkg, size_t index)
{
    if (pkg == nullptr)
    {
        return;
    }

    auto& entries = toPkg(pkg)->timeSeriesData;
    if (index >= entries.size())
    {
        return;
    }

    entries.erase(entries.begin() + static_cast<std::ptrdiff_t>(index));
}

/*=========================================================================
 * HeaderDto
 *=======================================================================*/

const char* dnv_vista_sdk_tsd_dto_header_get_ship_id(const dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr)
    {
        return nullptr;
    }

    return toHeader(h)->shipId.c_str();
}

void dnv_vista_sdk_tsd_dto_header_set_ship_id(dnv_vista_sdk_tsd_dto_header_t* h, const char* v)
{
    if (h == nullptr || v == nullptr)
    {
        return;
    }

    toHeader(h)->shipId = v;
}

int dnv_vista_sdk_tsd_dto_header_has_time_span(const dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr)
    {
        return 0;
    }

    return toHeader(h)->timeSpan.has_value() ? 1 : 0;
}

dnv_vista_sdk_tsd_dto_time_span_t* dnv_vista_sdk_tsd_dto_header_get_time_span(dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr || !toHeader(h)->timeSpan.has_value())
    {
        return nullptr;
    }

    return fromTimeSpanRef(*toHeader(h)->timeSpan);
}

void dnv_vista_sdk_tsd_dto_header_ensure_time_span(dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr)
    {
        return;
    }

    if (!toHeader(h)->timeSpan.has_value())
    {
        toHeader(h)->timeSpan.emplace();
    }
}

void dnv_vista_sdk_tsd_dto_header_clear_time_span(dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr)
    {
        return;
    }

    toHeader(h)->timeSpan.reset();
}

int dnv_vista_sdk_tsd_dto_header_has_date_created(const dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr)
    {
        return 0;
    }

    return toHeader(h)->dateCreated.has_value() ? 1 : 0;
}

const char* dnv_vista_sdk_tsd_dto_header_get_date_created(const dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr || !toHeader(h)->dateCreated.has_value())
    {
        return nullptr;
    }

    return toHeader(h)->dateCreated->c_str();
}

void dnv_vista_sdk_tsd_dto_header_set_date_created(dnv_vista_sdk_tsd_dto_header_t* h, const char* v)
{
    if (h == nullptr || v == nullptr)
    {
        return;
    }

    toHeader(h)->dateCreated = v;
}

void dnv_vista_sdk_tsd_dto_header_clear_date_created(dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr)
    {
        return;
    }

    toHeader(h)->dateCreated.reset();
}

int dnv_vista_sdk_tsd_dto_header_has_date_modified(const dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr)
    {
        return 0;
    }

    return toHeader(h)->dateModified.has_value() ? 1 : 0;
}

const char* dnv_vista_sdk_tsd_dto_header_get_date_modified(const dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr || !toHeader(h)->dateModified.has_value())
    {
        return nullptr;
    }

    return toHeader(h)->dateModified->c_str();
}

void dnv_vista_sdk_tsd_dto_header_set_date_modified(dnv_vista_sdk_tsd_dto_header_t* h, const char* v)
{
    if (h == nullptr || v == nullptr)
    {
        return;
    }

    toHeader(h)->dateModified = v;
}

void dnv_vista_sdk_tsd_dto_header_clear_date_modified(dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr)
    {
        return;
    }

    toHeader(h)->dateModified.reset();
}

int dnv_vista_sdk_tsd_dto_header_has_author(const dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr)
    {
        return 0;
    }

    return toHeader(h)->author.has_value() ? 1 : 0;
}

const char* dnv_vista_sdk_tsd_dto_header_get_author(const dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr || !toHeader(h)->author.has_value())
    {
        return nullptr;
    }

    return toHeader(h)->author->c_str();
}

void dnv_vista_sdk_tsd_dto_header_set_author(dnv_vista_sdk_tsd_dto_header_t* h, const char* v)
{
    if (h == nullptr || v == nullptr)
    {
        return;
    }

    toHeader(h)->author = v;
}

void dnv_vista_sdk_tsd_dto_header_clear_author(dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr)
    {
        return;
    }

    toHeader(h)->author.reset();
}

size_t dnv_vista_sdk_tsd_dto_header_system_cfg_count(const dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr || !toHeader(h)->systemConfiguration.has_value())
    {
        return 0;
    }

    return toHeader(h)->systemConfiguration->size();
}

dnv_vista_sdk_tsd_dto_cfg_ref_t* dnv_vista_sdk_tsd_dto_header_system_cfg_at(
    dnv_vista_sdk_tsd_dto_header_t* h, size_t index)
{
    if (h == nullptr || !toHeader(h)->systemConfiguration.has_value())
    {
        return nullptr;
    }

    auto& entries = *toHeader(h)->systemConfiguration;
    if (index >= entries.size())
    {
        return nullptr;
    }

    return fromCfgRefRef(entries[index]);
}

dnv_vista_sdk_tsd_dto_cfg_ref_t* dnv_vista_sdk_tsd_dto_header_system_cfg_push(dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr)
    {
        return nullptr;
    }

    if (!toHeader(h)->systemConfiguration.has_value())
    {
        toHeader(h)->systemConfiguration.emplace();
    }

    toHeader(h)->systemConfiguration->emplace_back();
    return fromCfgRefRef(toHeader(h)->systemConfiguration->back());
}

void dnv_vista_sdk_tsd_dto_header_system_cfg_remove(dnv_vista_sdk_tsd_dto_header_t* h, size_t index)
{
    if (h == nullptr || !toHeader(h)->systemConfiguration.has_value())
    {
        return;
    }

    auto& entries = *toHeader(h)->systemConfiguration;
    if (index >= entries.size())
    {
        return;
    }

    entries.erase(entries.begin() + static_cast<std::ptrdiff_t>(index));
}

int dnv_vista_sdk_tsd_dto_header_has_custom_headers(const dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr)
    {
        return 0;
    }

    return toHeader(h)->customHeaders.has_value() ? 1 : 0;
}

dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_tsd_dto_header_get_custom_headers(
    dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr || !toHeader(h)->customHeaders.has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<dnv_vista_sdk_serializable_document_t*>(&*toHeader(h)->customHeaders);
}

void dnv_vista_sdk_tsd_dto_header_ensure_custom_headers(dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr)
    {
        return;
    }

    if (!toHeader(h)->customHeaders.has_value())
    {
        toHeader(h)->customHeaders.emplace();
    }
}

void dnv_vista_sdk_tsd_dto_header_clear_custom_headers(dnv_vista_sdk_tsd_dto_header_t* h)
{
    if (h == nullptr)
    {
        return;
    }

    toHeader(h)->customHeaders.reset();
}

/*=========================================================================
 * TimeSpanDto
 *=======================================================================*/

const char* dnv_vista_sdk_tsd_dto_time_span_get_start(const dnv_vista_sdk_tsd_dto_time_span_t* ts)
{
    if (ts == nullptr)
    {
        return nullptr;
    }

    return toTimeSpan(ts)->start.c_str();
}

void dnv_vista_sdk_tsd_dto_time_span_set_start(dnv_vista_sdk_tsd_dto_time_span_t* ts, const char* v)
{
    if (ts == nullptr || v == nullptr)
    {
        return;
    }

    toTimeSpan(ts)->start = v;
}

const char* dnv_vista_sdk_tsd_dto_time_span_get_end(const dnv_vista_sdk_tsd_dto_time_span_t* ts)
{
    if (ts == nullptr)
    {
        return nullptr;
    }

    return toTimeSpan(ts)->end.c_str();
}

void dnv_vista_sdk_tsd_dto_time_span_set_end(dnv_vista_sdk_tsd_dto_time_span_t* ts, const char* v)
{
    if (ts == nullptr || v == nullptr)
    {
        return;
    }

    toTimeSpan(ts)->end = v;
}

/*=========================================================================
 * ConfigurationReferenceDto
 *=======================================================================*/

const char* dnv_vista_sdk_tsd_dto_cfg_ref_get_id(const dnv_vista_sdk_tsd_dto_cfg_ref_t* r)
{
    if (r == nullptr)
    {
        return nullptr;
    }

    return toCfgRef(r)->id.c_str();
}

void dnv_vista_sdk_tsd_dto_cfg_ref_set_id(dnv_vista_sdk_tsd_dto_cfg_ref_t* r, const char* v)
{
    if (r == nullptr || v == nullptr)
    {
        return;
    }

    toCfgRef(r)->id = v;
}

const char* dnv_vista_sdk_tsd_dto_cfg_ref_get_timestamp(const dnv_vista_sdk_tsd_dto_cfg_ref_t* r)
{
    if (r == nullptr)
    {
        return nullptr;
    }

    return toCfgRef(r)->timeStamp.c_str();
}

void dnv_vista_sdk_tsd_dto_cfg_ref_set_timestamp(dnv_vista_sdk_tsd_dto_cfg_ref_t* r, const char* v)
{
    if (r == nullptr || v == nullptr)
    {
        return;
    }

    toCfgRef(r)->timeStamp = v;
}

/*=========================================================================
 * TimeSeriesDataDto
 *=======================================================================*/

int dnv_vista_sdk_tsd_dto_tsd_has_data_cfg(const dnv_vista_sdk_tsd_dto_tsd_t* tsd)
{
    if (tsd == nullptr)
    {
        return 0;
    }

    return toTsd(tsd)->dataConfiguration.has_value() ? 1 : 0;
}

dnv_vista_sdk_tsd_dto_cfg_ref_t* dnv_vista_sdk_tsd_dto_tsd_get_data_cfg(dnv_vista_sdk_tsd_dto_tsd_t* tsd)
{
    if (tsd == nullptr || !toTsd(tsd)->dataConfiguration.has_value())
    {
        return nullptr;
    }

    return fromCfgRefRef(*toTsd(tsd)->dataConfiguration);
}

void dnv_vista_sdk_tsd_dto_tsd_ensure_data_cfg(dnv_vista_sdk_tsd_dto_tsd_t* tsd)
{
    if (tsd == nullptr)
    {
        return;
    }

    if (!toTsd(tsd)->dataConfiguration.has_value())
    {
        toTsd(tsd)->dataConfiguration.emplace();
    }
}

void dnv_vista_sdk_tsd_dto_tsd_clear_data_cfg(dnv_vista_sdk_tsd_dto_tsd_t* tsd)
{
    if (tsd == nullptr)
    {
        return;
    }

    toTsd(tsd)->dataConfiguration.reset();
}

size_t dnv_vista_sdk_tsd_dto_tsd_tabular_count(const dnv_vista_sdk_tsd_dto_tsd_t* tsd)
{
    if (tsd == nullptr || !toTsd(tsd)->tabularData.has_value())
    {
        return 0;
    }

    return toTsd(tsd)->tabularData->size();
}

dnv_vista_sdk_tsd_dto_tabular_t* dnv_vista_sdk_tsd_dto_tsd_tabular_at(dnv_vista_sdk_tsd_dto_tsd_t* tsd, size_t index)
{
    if (tsd == nullptr || !toTsd(tsd)->tabularData.has_value())
    {
        return nullptr;
    }

    auto& entries = *toTsd(tsd)->tabularData;
    if (index >= entries.size())
    {
        return nullptr;
    }

    return fromTabularRef(entries[index]);
}

dnv_vista_sdk_tsd_dto_tabular_t* dnv_vista_sdk_tsd_dto_tsd_tabular_push(dnv_vista_sdk_tsd_dto_tsd_t* tsd)
{
    if (tsd == nullptr)
    {
        return nullptr;
    }

    if (!toTsd(tsd)->tabularData.has_value())
    {
        toTsd(tsd)->tabularData.emplace();
    }

    toTsd(tsd)->tabularData->emplace_back();
    return fromTabularRef(toTsd(tsd)->tabularData->back());
}

void dnv_vista_sdk_tsd_dto_tsd_tabular_remove(dnv_vista_sdk_tsd_dto_tsd_t* tsd, size_t index)
{
    if (tsd == nullptr || !toTsd(tsd)->tabularData.has_value())
    {
        return;
    }

    auto& entries = *toTsd(tsd)->tabularData;
    if (index >= entries.size())
    {
        return;
    }

    entries.erase(entries.begin() + static_cast<std::ptrdiff_t>(index));
}

int dnv_vista_sdk_tsd_dto_tsd_has_event(const dnv_vista_sdk_tsd_dto_tsd_t* tsd)
{
    if (tsd == nullptr)
    {
        return 0;
    }

    return toTsd(tsd)->eventData.has_value() ? 1 : 0;
}

dnv_vista_sdk_tsd_dto_event_t* dnv_vista_sdk_tsd_dto_tsd_get_event(dnv_vista_sdk_tsd_dto_tsd_t* tsd)
{
    if (tsd == nullptr || !toTsd(tsd)->eventData.has_value())
    {
        return nullptr;
    }

    return fromEventRef(*toTsd(tsd)->eventData);
}

void dnv_vista_sdk_tsd_dto_tsd_ensure_event(dnv_vista_sdk_tsd_dto_tsd_t* tsd)
{
    if (tsd == nullptr)
    {
        return;
    }

    if (!toTsd(tsd)->eventData.has_value())
    {
        toTsd(tsd)->eventData.emplace();
    }
}

void dnv_vista_sdk_tsd_dto_tsd_clear_event(dnv_vista_sdk_tsd_dto_tsd_t* tsd)
{
    if (tsd == nullptr)
    {
        return;
    }

    toTsd(tsd)->eventData.reset();
}

int dnv_vista_sdk_tsd_dto_tsd_has_custom_data_kinds(const dnv_vista_sdk_tsd_dto_tsd_t* tsd)
{
    if (tsd == nullptr)
    {
        return 0;
    }

    return toTsd(tsd)->customDataKinds.has_value() ? 1 : 0;
}

dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_tsd_dto_tsd_get_custom_data_kinds(dnv_vista_sdk_tsd_dto_tsd_t* tsd)
{
    if (tsd == nullptr || !toTsd(tsd)->customDataKinds.has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<dnv_vista_sdk_serializable_document_t*>(&*toTsd(tsd)->customDataKinds);
}

void dnv_vista_sdk_tsd_dto_tsd_ensure_custom_data_kinds(dnv_vista_sdk_tsd_dto_tsd_t* tsd)
{
    if (tsd == nullptr)
    {
        return;
    }

    if (!toTsd(tsd)->customDataKinds.has_value())
    {
        toTsd(tsd)->customDataKinds.emplace();
    }
}

void dnv_vista_sdk_tsd_dto_tsd_clear_custom_data_kinds(dnv_vista_sdk_tsd_dto_tsd_t* tsd)
{
    if (tsd == nullptr)
    {
        return;
    }

    toTsd(tsd)->customDataKinds.reset();
}

/*=========================================================================
 * TabularDataDto
 *=======================================================================*/

int dnv_vista_sdk_tsd_dto_tabular_has_number_of_data_set(const dnv_vista_sdk_tsd_dto_tabular_t* t)
{
    if (t == nullptr)
    {
        return 0;
    }

    return toTabular(t)->numberOfDataSet.has_value() ? 1 : 0;
}

size_t dnv_vista_sdk_tsd_dto_tabular_get_number_of_data_set(const dnv_vista_sdk_tsd_dto_tabular_t* t)
{
    if (t == nullptr || !toTabular(t)->numberOfDataSet.has_value())
    {
        return 0;
    }

    return *toTabular(t)->numberOfDataSet;
}

void dnv_vista_sdk_tsd_dto_tabular_set_number_of_data_set(dnv_vista_sdk_tsd_dto_tabular_t* t, size_t v)
{
    if (t == nullptr)
    {
        return;
    }

    toTabular(t)->numberOfDataSet = v;
}

void dnv_vista_sdk_tsd_dto_tabular_clear_number_of_data_set(dnv_vista_sdk_tsd_dto_tabular_t* t)
{
    if (t == nullptr)
    {
        return;
    }

    toTabular(t)->numberOfDataSet.reset();
}

int dnv_vista_sdk_tsd_dto_tabular_has_number_of_data_channel(const dnv_vista_sdk_tsd_dto_tabular_t* t)
{
    if (t == nullptr)
    {
        return 0;
    }

    return toTabular(t)->numberOfDataChannel.has_value() ? 1 : 0;
}

size_t dnv_vista_sdk_tsd_dto_tabular_get_number_of_data_channel(const dnv_vista_sdk_tsd_dto_tabular_t* t)
{
    if (t == nullptr || !toTabular(t)->numberOfDataChannel.has_value())
    {
        return 0;
    }

    return *toTabular(t)->numberOfDataChannel;
}

void dnv_vista_sdk_tsd_dto_tabular_set_number_of_data_channel(dnv_vista_sdk_tsd_dto_tabular_t* t, size_t v)
{
    if (t == nullptr)
    {
        return;
    }

    toTabular(t)->numberOfDataChannel = v;
}

void dnv_vista_sdk_tsd_dto_tabular_clear_number_of_data_channel(dnv_vista_sdk_tsd_dto_tabular_t* t)
{
    if (t == nullptr)
    {
        return;
    }

    toTabular(t)->numberOfDataChannel.reset();
}

size_t dnv_vista_sdk_tsd_dto_tabular_channel_id_count(const dnv_vista_sdk_tsd_dto_tabular_t* t)
{
    if (t == nullptr || !toTabular(t)->dataChannelIds.has_value())
    {
        return 0;
    }

    return toTabular(t)->dataChannelIds->size();
}

const char* dnv_vista_sdk_tsd_dto_tabular_channel_id_at(const dnv_vista_sdk_tsd_dto_tabular_t* t, size_t index)
{
    if (t == nullptr || !toTabular(t)->dataChannelIds.has_value())
    {
        return nullptr;
    }

    const auto& ids = *toTabular(t)->dataChannelIds;
    if (index >= ids.size())
    {
        return nullptr;
    }

    return ids[index].c_str();
}

void dnv_vista_sdk_tsd_dto_tabular_set_channel_ids(
    dnv_vista_sdk_tsd_dto_tabular_t* t, const char* const* ids, size_t count)
{
    if (t == nullptr)
    {
        return;
    }

    std::vector<std::string> v;
    v.reserve(count);
    for (size_t i = 0; i < count; ++i)
    {
        v.emplace_back(ids[i]);
    }

    toTabular(t)->dataChannelIds = std::move(v);
}

void dnv_vista_sdk_tsd_dto_tabular_clear_channel_ids(dnv_vista_sdk_tsd_dto_tabular_t* t)
{
    if (t == nullptr)
    {
        return;
    }

    toTabular(t)->dataChannelIds.reset();
}

size_t dnv_vista_sdk_tsd_dto_tabular_data_set_count(const dnv_vista_sdk_tsd_dto_tabular_t* t)
{
    if (t == nullptr || !toTabular(t)->dataSets.has_value())
    {
        return 0;
    }

    return toTabular(t)->dataSets->size();
}

dnv_vista_sdk_tsd_dto_tab_set_t* dnv_vista_sdk_tsd_dto_tabular_data_set_at(
    dnv_vista_sdk_tsd_dto_tabular_t* t, size_t index)
{
    if (t == nullptr || !toTabular(t)->dataSets.has_value())
    {
        return nullptr;
    }

    auto& sets = *toTabular(t)->dataSets;
    if (index >= sets.size())
    {
        return nullptr;
    }

    return fromTabSetRef(sets[index]);
}

dnv_vista_sdk_tsd_dto_tab_set_t* dnv_vista_sdk_tsd_dto_tabular_data_set_push(dnv_vista_sdk_tsd_dto_tabular_t* t)
{
    if (t == nullptr)
    {
        return nullptr;
    }

    if (!toTabular(t)->dataSets.has_value())
    {
        toTabular(t)->dataSets.emplace();
    }

    toTabular(t)->dataSets->emplace_back();
    return fromTabSetRef(toTabular(t)->dataSets->back());
}

void dnv_vista_sdk_tsd_dto_tabular_data_set_remove(dnv_vista_sdk_tsd_dto_tabular_t* t, size_t index)
{
    if (t == nullptr || !toTabular(t)->dataSets.has_value())
    {
        return;
    }

    auto& sets = *toTabular(t)->dataSets;
    if (index >= sets.size())
    {
        return;
    }

    sets.erase(sets.begin() + static_cast<std::ptrdiff_t>(index));
}

/*=========================================================================
 * TabularDataSetDto
 *=======================================================================*/

const char* dnv_vista_sdk_tsd_dto_tab_set_get_timestamp(const dnv_vista_sdk_tsd_dto_tab_set_t* s)
{
    if (s == nullptr)
    {
        return nullptr;
    }

    return toTabSet(s)->timeStamp.c_str();
}

void dnv_vista_sdk_tsd_dto_tab_set_set_timestamp(dnv_vista_sdk_tsd_dto_tab_set_t* s, const char* v)
{
    if (s == nullptr || v == nullptr)
    {
        return;
    }

    toTabSet(s)->timeStamp = v;
}

size_t dnv_vista_sdk_tsd_dto_tab_set_value_count(const dnv_vista_sdk_tsd_dto_tab_set_t* s)
{
    if (s == nullptr)
    {
        return 0;
    }

    return toTabSet(s)->value.size();
}

const char* dnv_vista_sdk_tsd_dto_tab_set_value_at(const dnv_vista_sdk_tsd_dto_tab_set_t* s, size_t index)
{
    if (s == nullptr)
    {
        return nullptr;
    }

    const auto& values = toTabSet(s)->value;
    if (index >= values.size())
    {
        return nullptr;
    }

    return values[index].c_str();
}

void dnv_vista_sdk_tsd_dto_tab_set_set_values(
    dnv_vista_sdk_tsd_dto_tab_set_t* s, const char* const* values, size_t count)
{
    if (s == nullptr)
    {
        return;
    }

    std::vector<std::string> v;
    v.reserve(count);
    for (size_t i = 0; i < count; ++i)
    {
        v.emplace_back(values[i]);
    }

    toTabSet(s)->value = std::move(v);
}

size_t dnv_vista_sdk_tsd_dto_tab_set_quality_count(const dnv_vista_sdk_tsd_dto_tab_set_t* s)
{
    if (s == nullptr || !toTabSet(s)->quality.has_value())
    {
        return 0;
    }

    return toTabSet(s)->quality->size();
}

const char* dnv_vista_sdk_tsd_dto_tab_set_quality_at(const dnv_vista_sdk_tsd_dto_tab_set_t* s, size_t index)
{
    if (s == nullptr || !toTabSet(s)->quality.has_value())
    {
        return nullptr;
    }

    const auto& q = *toTabSet(s)->quality;
    if (index >= q.size())
    {
        return nullptr;
    }

    return q[index].c_str();
}

void dnv_vista_sdk_tsd_dto_tab_set_set_quality(
    dnv_vista_sdk_tsd_dto_tab_set_t* s, const char* const* quality, size_t count)
{
    if (s == nullptr)
    {
        return;
    }

    std::vector<std::string> v;
    v.reserve(count);
    for (size_t i = 0; i < count; ++i)
    {
        v.emplace_back(quality[i]);
    }

    toTabSet(s)->quality = std::move(v);
}

void dnv_vista_sdk_tsd_dto_tab_set_clear_quality(dnv_vista_sdk_tsd_dto_tab_set_t* s)
{
    if (s == nullptr)
    {
        return;
    }

    toTabSet(s)->quality.reset();
}

/*=========================================================================
 * EventDataDto
 *=======================================================================*/

int dnv_vista_sdk_tsd_dto_event_has_number_of_data_set(const dnv_vista_sdk_tsd_dto_event_t* e)
{
    if (e == nullptr)
    {
        return 0;
    }

    return toEvent(e)->numberOfDataSet.has_value() ? 1 : 0;
}

size_t dnv_vista_sdk_tsd_dto_event_get_number_of_data_set(const dnv_vista_sdk_tsd_dto_event_t* e)
{
    if (e == nullptr || !toEvent(e)->numberOfDataSet.has_value())
    {
        return 0;
    }

    return *toEvent(e)->numberOfDataSet;
}

void dnv_vista_sdk_tsd_dto_event_set_number_of_data_set(dnv_vista_sdk_tsd_dto_event_t* e, size_t v)
{
    if (e == nullptr)
    {
        return;
    }

    toEvent(e)->numberOfDataSet = v;
}

void dnv_vista_sdk_tsd_dto_event_clear_number_of_data_set(dnv_vista_sdk_tsd_dto_event_t* e)
{
    if (e == nullptr)
    {
        return;
    }

    toEvent(e)->numberOfDataSet.reset();
}

size_t dnv_vista_sdk_tsd_dto_event_data_set_count(const dnv_vista_sdk_tsd_dto_event_t* e)
{
    if (e == nullptr || !toEvent(e)->dataSet.has_value())
    {
        return 0;
    }

    return toEvent(e)->dataSet->size();
}

dnv_vista_sdk_tsd_dto_event_set_t* dnv_vista_sdk_tsd_dto_event_data_set_at(
    dnv_vista_sdk_tsd_dto_event_t* e, size_t index)
{
    if (e == nullptr || !toEvent(e)->dataSet.has_value())
    {
        return nullptr;
    }

    auto& sets = *toEvent(e)->dataSet;
    if (index >= sets.size())
    {
        return nullptr;
    }

    return fromEventSetRef(sets[index]);
}

dnv_vista_sdk_tsd_dto_event_set_t* dnv_vista_sdk_tsd_dto_event_data_set_push(dnv_vista_sdk_tsd_dto_event_t* e)
{
    if (e == nullptr)
    {
        return nullptr;
    }

    if (!toEvent(e)->dataSet.has_value())
    {
        toEvent(e)->dataSet.emplace();
    }

    toEvent(e)->dataSet->emplace_back();
    return fromEventSetRef(toEvent(e)->dataSet->back());
}

void dnv_vista_sdk_tsd_dto_event_data_set_remove(dnv_vista_sdk_tsd_dto_event_t* e, size_t index)
{
    if (e == nullptr || !toEvent(e)->dataSet.has_value())
    {
        return;
    }

    auto& sets = *toEvent(e)->dataSet;
    if (index >= sets.size())
    {
        return;
    }

    sets.erase(sets.begin() + static_cast<std::ptrdiff_t>(index));
}

/*=========================================================================
 * EventDataSetDto
 *=======================================================================*/

const char* dnv_vista_sdk_tsd_dto_event_set_get_timestamp(const dnv_vista_sdk_tsd_dto_event_set_t* s)
{
    if (s == nullptr)
    {
        return nullptr;
    }

    return toEventSet(s)->timeStamp.c_str();
}

void dnv_vista_sdk_tsd_dto_event_set_set_timestamp(dnv_vista_sdk_tsd_dto_event_set_t* s, const char* v)
{
    if (s == nullptr || v == nullptr)
    {
        return;
    }

    toEventSet(s)->timeStamp = v;
}

const char* dnv_vista_sdk_tsd_dto_event_set_get_data_channel_id(const dnv_vista_sdk_tsd_dto_event_set_t* s)
{
    if (s == nullptr)
    {
        return nullptr;
    }

    return toEventSet(s)->dataChannelId.c_str();
}

void dnv_vista_sdk_tsd_dto_event_set_set_data_channel_id(dnv_vista_sdk_tsd_dto_event_set_t* s, const char* v)
{
    if (s == nullptr || v == nullptr)
    {
        return;
    }

    toEventSet(s)->dataChannelId = v;
}

const char* dnv_vista_sdk_tsd_dto_event_set_get_value(const dnv_vista_sdk_tsd_dto_event_set_t* s)
{
    if (s == nullptr)
    {
        return nullptr;
    }

    return toEventSet(s)->value.c_str();
}

void dnv_vista_sdk_tsd_dto_event_set_set_value(dnv_vista_sdk_tsd_dto_event_set_t* s, const char* v)
{
    if (s == nullptr || v == nullptr)
    {
        return;
    }

    toEventSet(s)->value = v;
}

int dnv_vista_sdk_tsd_dto_event_set_has_quality(const dnv_vista_sdk_tsd_dto_event_set_t* s)
{
    if (s == nullptr)
    {
        return 0;
    }

    return toEventSet(s)->quality.has_value() ? 1 : 0;
}

const char* dnv_vista_sdk_tsd_dto_event_set_get_quality(const dnv_vista_sdk_tsd_dto_event_set_t* s)
{
    if (s == nullptr || !toEventSet(s)->quality.has_value())
    {
        return nullptr;
    }

    return toEventSet(s)->quality->c_str();
}

void dnv_vista_sdk_tsd_dto_event_set_set_quality(dnv_vista_sdk_tsd_dto_event_set_t* s, const char* v)
{
    if (s == nullptr || v == nullptr)
    {
        return;
    }

    toEventSet(s)->quality = v;
}

void dnv_vista_sdk_tsd_dto_event_set_clear_quality(dnv_vista_sdk_tsd_dto_event_set_t* s)
{
    if (s == nullptr)
    {
        return;
    }

    toEventSet(s)->quality.reset();
}
