#include "dnv/vista/sdk/c/transport/serialization/json/datachannel/data_channel_dto.h"

#include "../../../../cast_internal.h"
#include "../../../../error_internal.h"

#include <vector>

using namespace dnv::vista::sdk;
using namespace dnv::vista::sdk::c;
namespace dcljson = dnv::vista::sdk::transport::serialization::json::datachannel;
namespace serjson = dnv::vista::sdk::transport::serialization::json;

/*=========================================================================
 * Internal cast helpers
 *=======================================================================*/

static inline dcljson::DataChannelListPackageDto* toDtoPkg(dnv_vista_sdk_dcl_dto_package_t* p)
{
    return reinterpret_cast<dcljson::DataChannelListPackageDto*>(p);
}

static inline const dcljson::DataChannelListPackageDto* toDtoPkg(const dnv_vista_sdk_dcl_dto_package_t* p)
{
    return reinterpret_cast<const dcljson::DataChannelListPackageDto*>(p);
}

static inline dnv_vista_sdk_dcl_dto_package_t* fromDtoPkg(dcljson::DataChannelListPackageDto* p)
{
    return reinterpret_cast<dnv_vista_sdk_dcl_dto_package_t*>(p);
}

static inline dcljson::PackageDto* toPkg(dnv_vista_sdk_dcl_dto_pkg_t* p)
{
    return reinterpret_cast<dcljson::PackageDto*>(p);
}

static inline dnv_vista_sdk_dcl_dto_pkg_t* fromPkgRef(dcljson::PackageDto& p)
{
    return reinterpret_cast<dnv_vista_sdk_dcl_dto_pkg_t*>(&p);
}

static inline dcljson::HeaderDto* toHeader(dnv_vista_sdk_dcl_dto_header_t* h)
{
    return reinterpret_cast<dcljson::HeaderDto*>(h);
}

static inline const dcljson::HeaderDto* toHeader(const dnv_vista_sdk_dcl_dto_header_t* h)
{
    return reinterpret_cast<const dcljson::HeaderDto*>(h);
}

static inline dnv_vista_sdk_dcl_dto_header_t* fromHeaderRef(dcljson::HeaderDto& h)
{
    return reinterpret_cast<dnv_vista_sdk_dcl_dto_header_t*>(&h);
}

static inline dcljson::ConfigurationReferenceDto* toCfgRef(dnv_vista_sdk_dcl_dto_cfg_ref_t* r)
{
    return reinterpret_cast<dcljson::ConfigurationReferenceDto*>(r);
}

static inline const dcljson::ConfigurationReferenceDto* toCfgRef(const dnv_vista_sdk_dcl_dto_cfg_ref_t* r)
{
    return reinterpret_cast<const dcljson::ConfigurationReferenceDto*>(r);
}

static inline dnv_vista_sdk_dcl_dto_cfg_ref_t* fromCfgRefRef(dcljson::ConfigurationReferenceDto& r)
{
    return reinterpret_cast<dnv_vista_sdk_dcl_dto_cfg_ref_t*>(&r);
}

static inline dcljson::VersionInformationDto* toVerInfo(dnv_vista_sdk_dcl_dto_ver_info_t* v)
{
    return reinterpret_cast<dcljson::VersionInformationDto*>(v);
}

static inline const dcljson::VersionInformationDto* toVerInfo(const dnv_vista_sdk_dcl_dto_ver_info_t* v)
{
    return reinterpret_cast<const dcljson::VersionInformationDto*>(v);
}

static inline dnv_vista_sdk_dcl_dto_ver_info_t* fromVerInfoRef(dcljson::VersionInformationDto& v)
{
    return reinterpret_cast<dnv_vista_sdk_dcl_dto_ver_info_t*>(&v);
}

static inline dcljson::DataChannelListDto* toChList(dnv_vista_sdk_dcl_dto_ch_list_t* l)
{
    return reinterpret_cast<dcljson::DataChannelListDto*>(l);
}

static inline const dcljson::DataChannelListDto* toChList(const dnv_vista_sdk_dcl_dto_ch_list_t* l)
{
    return reinterpret_cast<const dcljson::DataChannelListDto*>(l);
}

static inline dnv_vista_sdk_dcl_dto_ch_list_t* fromChListRef(dcljson::DataChannelListDto& l)
{
    return reinterpret_cast<dnv_vista_sdk_dcl_dto_ch_list_t*>(&l);
}

static inline dcljson::DataChannelDto* toChannel(dnv_vista_sdk_dcl_dto_channel_t* ch)
{
    return reinterpret_cast<dcljson::DataChannelDto*>(ch);
}

static inline dnv_vista_sdk_dcl_dto_channel_t* fromChannelRef(dcljson::DataChannelDto& ch)
{
    return reinterpret_cast<dnv_vista_sdk_dcl_dto_channel_t*>(&ch);
}

static inline dcljson::DataChannelIdDto* toChId(dnv_vista_sdk_dcl_dto_ch_id_t* id)
{
    return reinterpret_cast<dcljson::DataChannelIdDto*>(id);
}

static inline const dcljson::DataChannelIdDto* toChId(const dnv_vista_sdk_dcl_dto_ch_id_t* id)
{
    return reinterpret_cast<const dcljson::DataChannelIdDto*>(id);
}

static inline dnv_vista_sdk_dcl_dto_ch_id_t* fromChIdRef(dcljson::DataChannelIdDto& id)
{
    return reinterpret_cast<dnv_vista_sdk_dcl_dto_ch_id_t*>(&id);
}

static inline dcljson::NameObjectDto* toNameObj(dnv_vista_sdk_dcl_dto_name_obj_t* n)
{
    return reinterpret_cast<dcljson::NameObjectDto*>(n);
}

static inline const dcljson::NameObjectDto* toNameObj(const dnv_vista_sdk_dcl_dto_name_obj_t* n)
{
    return reinterpret_cast<const dcljson::NameObjectDto*>(n);
}

static inline dnv_vista_sdk_dcl_dto_name_obj_t* fromNameObjRef(dcljson::NameObjectDto& n)
{
    return reinterpret_cast<dnv_vista_sdk_dcl_dto_name_obj_t*>(&n);
}

static inline dcljson::PropertyDto* toProperty(dnv_vista_sdk_dcl_dto_property_t* p)
{
    return reinterpret_cast<dcljson::PropertyDto*>(p);
}

static inline const dcljson::PropertyDto* toProperty(const dnv_vista_sdk_dcl_dto_property_t* p)
{
    return reinterpret_cast<const dcljson::PropertyDto*>(p);
}

static inline dnv_vista_sdk_dcl_dto_property_t* fromPropertyRef(dcljson::PropertyDto& p)
{
    return reinterpret_cast<dnv_vista_sdk_dcl_dto_property_t*>(&p);
}

static inline dcljson::DataChannelTypeDto* toChType(dnv_vista_sdk_dcl_dto_ch_type_t* t)
{
    return reinterpret_cast<dcljson::DataChannelTypeDto*>(t);
}

static inline const dcljson::DataChannelTypeDto* toChType(const dnv_vista_sdk_dcl_dto_ch_type_t* t)
{
    return reinterpret_cast<const dcljson::DataChannelTypeDto*>(t);
}

static inline dnv_vista_sdk_dcl_dto_ch_type_t* fromChTypeRef(dcljson::DataChannelTypeDto& t)
{
    return reinterpret_cast<dnv_vista_sdk_dcl_dto_ch_type_t*>(&t);
}

static inline dcljson::FormatDto* toFormat(dnv_vista_sdk_dcl_dto_format_t* f)
{
    return reinterpret_cast<dcljson::FormatDto*>(f);
}

static inline const dcljson::FormatDto* toFormat(const dnv_vista_sdk_dcl_dto_format_t* f)
{
    return reinterpret_cast<const dcljson::FormatDto*>(f);
}

static inline dnv_vista_sdk_dcl_dto_format_t* fromFormatRef(dcljson::FormatDto& f)
{
    return reinterpret_cast<dnv_vista_sdk_dcl_dto_format_t*>(&f);
}

static inline dcljson::RestrictionDto* toRestriction(dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    return reinterpret_cast<dcljson::RestrictionDto*>(r);
}

static inline const dcljson::RestrictionDto* toRestriction(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    return reinterpret_cast<const dcljson::RestrictionDto*>(r);
}

static inline dnv_vista_sdk_dcl_dto_restriction_t* fromRestrictionRef(dcljson::RestrictionDto& r)
{
    return reinterpret_cast<dnv_vista_sdk_dcl_dto_restriction_t*>(&r);
}

static inline dcljson::RangeDto* toRange(dnv_vista_sdk_dcl_dto_range_t* r)
{
    return reinterpret_cast<dcljson::RangeDto*>(r);
}

static inline const dcljson::RangeDto* toRange(const dnv_vista_sdk_dcl_dto_range_t* r)
{
    return reinterpret_cast<const dcljson::RangeDto*>(r);
}

static inline dnv_vista_sdk_dcl_dto_range_t* fromRangeRef(dcljson::RangeDto& r)
{
    return reinterpret_cast<dnv_vista_sdk_dcl_dto_range_t*>(&r);
}

static inline dcljson::UnitDto* toUnit(dnv_vista_sdk_dcl_dto_unit_t* u)
{
    return reinterpret_cast<dcljson::UnitDto*>(u);
}

static inline const dcljson::UnitDto* toUnit(const dnv_vista_sdk_dcl_dto_unit_t* u)
{
    return reinterpret_cast<const dcljson::UnitDto*>(u);
}

static inline dnv_vista_sdk_dcl_dto_unit_t* fromUnitRef(dcljson::UnitDto& u)
{
    return reinterpret_cast<dnv_vista_sdk_dcl_dto_unit_t*>(&u);
}

/*=========================================================================
 * Root: DataChannelListPackageDto
 *=======================================================================*/

dnv_vista_sdk_dcl_dto_package_t* dnv_vista_sdk_dcl_to_dto(const dnv_vista_sdk_dcl_list_package_t* domain)
{
    if (domain == nullptr)
    {
        c::setLastError("domain must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_dcl_dto_package_t*>([&]() {
        auto* p = new dcljson::DataChannelListPackageDto;
        *p = dcljson::toDto(*toDataChannelListPackage(domain));
        return fromDtoPkg(p);
    });
}

dnv_vista_sdk_dcl_list_package_t* dnv_vista_sdk_dcl_to_domain(const dnv_vista_sdk_dcl_dto_package_t* dto)
{
    if (dto == nullptr)
    {
        c::setLastError("dto must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_dcl_list_package_t*>(
        [&]() { return fromDataChannelListPackage(dcljson::toDomain(*toDtoPkg(dto))); });
}

char* dnv_vista_sdk_dcl_dto_to_json(const dnv_vista_sdk_dcl_dto_package_t* dto, int prettyPrint)
{
    if (dto == nullptr)
    {
        c::setLastError("dto must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<char*>(
        [&]() { return toOwnedCString(dcljson::toJsonString(*toDtoPkg(dto), prettyPrint != 0)); });
}

dnv_vista_sdk_dcl_dto_package_t* dnv_vista_sdk_dcl_dto_from_json(const char* json)
{
    if (json == nullptr)
    {
        c::setLastError("json must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_dcl_dto_package_t*>([&]() -> dnv_vista_sdk_dcl_dto_package_t* {
        auto opt = dcljson::fromJsonString(json);
        if (!opt.has_value())
        {
            c::setLastError("invalid DataChannelListPackage JSON", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
            return nullptr;
        }

        auto* p = new dcljson::DataChannelListPackageDto;
        *p = std::move(*opt);
        return fromDtoPkg(p);
    });
}

void dnv_vista_sdk_dcl_dto_package_free(dnv_vista_sdk_dcl_dto_package_t* p)
{
    delete toDtoPkg(p);
}

dnv_vista_sdk_dcl_dto_pkg_t* dnv_vista_sdk_dcl_dto_package_get_pkg(dnv_vista_sdk_dcl_dto_package_t* p)
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

dnv_vista_sdk_dcl_dto_header_t* dnv_vista_sdk_dcl_dto_pkg_get_header(dnv_vista_sdk_dcl_dto_pkg_t* pkg)
{
    if (pkg == nullptr)
    {
        return nullptr;
    }

    return fromHeaderRef(toPkg(pkg)->header);
}

dnv_vista_sdk_dcl_dto_ch_list_t* dnv_vista_sdk_dcl_dto_pkg_get_channel_list(dnv_vista_sdk_dcl_dto_pkg_t* pkg)
{
    if (pkg == nullptr)
    {
        return nullptr;
    }

    return fromChListRef(toPkg(pkg)->dataChannelList);
}

/*=========================================================================
 * HeaderDto
 *=======================================================================*/

const char* dnv_vista_sdk_dcl_dto_header_get_ship_id(const dnv_vista_sdk_dcl_dto_header_t* h)
{
    if (h == nullptr)
    {
        return nullptr;
    }

    return toHeader(h)->shipId.c_str();
}

void dnv_vista_sdk_dcl_dto_header_set_ship_id(dnv_vista_sdk_dcl_dto_header_t* h, const char* v)
{
    if (h == nullptr || v == nullptr)
    {
        return;
    }

    toHeader(h)->shipId = v;
}

dnv_vista_sdk_dcl_dto_cfg_ref_t* dnv_vista_sdk_dcl_dto_header_get_cfg_ref(dnv_vista_sdk_dcl_dto_header_t* h)
{
    if (h == nullptr)
    {
        return nullptr;
    }

    return fromCfgRefRef(toHeader(h)->dataChannelListId);
}

int dnv_vista_sdk_dcl_dto_header_has_ver_info(const dnv_vista_sdk_dcl_dto_header_t* h)
{
    if (h == nullptr)
    {
        return 0;
    }

    return toHeader(h)->versionInformation.has_value() ? 1 : 0;
}

dnv_vista_sdk_dcl_dto_ver_info_t* dnv_vista_sdk_dcl_dto_header_get_ver_info(dnv_vista_sdk_dcl_dto_header_t* h)
{
    if (h == nullptr || !toHeader(h)->versionInformation.has_value())
    {
        return nullptr;
    }

    return fromVerInfoRef(*toHeader(h)->versionInformation);
}

void dnv_vista_sdk_dcl_dto_header_ensure_ver_info(dnv_vista_sdk_dcl_dto_header_t* h)
{
    if (h == nullptr)
    {
        return;
    }

    if (!toHeader(h)->versionInformation.has_value())
    {
        toHeader(h)->versionInformation.emplace();
    }
}

void dnv_vista_sdk_dcl_dto_header_clear_ver_info(dnv_vista_sdk_dcl_dto_header_t* h)
{
    if (h == nullptr)
    {
        return;
    }

    toHeader(h)->versionInformation.reset();
}

int dnv_vista_sdk_dcl_dto_header_has_author(const dnv_vista_sdk_dcl_dto_header_t* h)
{
    if (h == nullptr)
    {
        return 0;
    }

    return toHeader(h)->author.has_value() ? 1 : 0;
}

const char* dnv_vista_sdk_dcl_dto_header_get_author(const dnv_vista_sdk_dcl_dto_header_t* h)
{
    if (h == nullptr || !toHeader(h)->author.has_value())
    {
        return nullptr;
    }

    return toHeader(h)->author->c_str();
}

void dnv_vista_sdk_dcl_dto_header_set_author(dnv_vista_sdk_dcl_dto_header_t* h, const char* v)
{
    if (h == nullptr || v == nullptr)
    {
        return;
    }

    toHeader(h)->author = v;
}

void dnv_vista_sdk_dcl_dto_header_clear_author(dnv_vista_sdk_dcl_dto_header_t* h)
{
    if (h == nullptr)
    {
        return;
    }

    toHeader(h)->author.reset();
}

int dnv_vista_sdk_dcl_dto_header_has_date_created(const dnv_vista_sdk_dcl_dto_header_t* h)
{
    if (h == nullptr)
    {
        return 0;
    }

    return toHeader(h)->dateCreated.has_value() ? 1 : 0;
}

const char* dnv_vista_sdk_dcl_dto_header_get_date_created(const dnv_vista_sdk_dcl_dto_header_t* h)
{
    if (h == nullptr || !toHeader(h)->dateCreated.has_value())
    {
        return nullptr;
    }

    return toHeader(h)->dateCreated->c_str();
}

void dnv_vista_sdk_dcl_dto_header_set_date_created(dnv_vista_sdk_dcl_dto_header_t* h, const char* v)
{
    if (h == nullptr || v == nullptr)
    {
        return;
    }

    toHeader(h)->dateCreated = v;
}

void dnv_vista_sdk_dcl_dto_header_clear_date_created(dnv_vista_sdk_dcl_dto_header_t* h)
{
    if (h == nullptr)
    {
        return;
    }

    toHeader(h)->dateCreated.reset();
}

int dnv_vista_sdk_dcl_dto_header_has_custom_headers(const dnv_vista_sdk_dcl_dto_header_t* h)
{
    if (h == nullptr)
    {
        return 0;
    }

    return toHeader(h)->customHeaders.has_value() ? 1 : 0;
}

dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_dcl_dto_header_get_custom_headers(
    dnv_vista_sdk_dcl_dto_header_t* h)
{
    if (h == nullptr || !toHeader(h)->customHeaders.has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<dnv_vista_sdk_serializable_document_t*>(&*toHeader(h)->customHeaders);
}

void dnv_vista_sdk_dcl_dto_header_ensure_custom_headers(dnv_vista_sdk_dcl_dto_header_t* h)
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

void dnv_vista_sdk_dcl_dto_header_clear_custom_headers(dnv_vista_sdk_dcl_dto_header_t* h)
{
    if (h == nullptr)
    {
        return;
    }

    toHeader(h)->customHeaders.reset();
}

/*=========================================================================
 * ConfigurationReferenceDto
 *=======================================================================*/

const char* dnv_vista_sdk_dcl_dto_cfg_ref_get_id(const dnv_vista_sdk_dcl_dto_cfg_ref_t* r)
{
    if (r == nullptr)
    {
        return nullptr;
    }

    return toCfgRef(r)->id.c_str();
}

void dnv_vista_sdk_dcl_dto_cfg_ref_set_id(dnv_vista_sdk_dcl_dto_cfg_ref_t* r, const char* v)
{
    if (r == nullptr || v == nullptr)
    {
        return;
    }

    toCfgRef(r)->id = v;
}

const char* dnv_vista_sdk_dcl_dto_cfg_ref_get_timestamp(const dnv_vista_sdk_dcl_dto_cfg_ref_t* r)
{
    if (r == nullptr)
    {
        return nullptr;
    }

    return toCfgRef(r)->timeStamp.c_str();
}

void dnv_vista_sdk_dcl_dto_cfg_ref_set_timestamp(dnv_vista_sdk_dcl_dto_cfg_ref_t* r, const char* v)
{
    if (r == nullptr || v == nullptr)
    {
        return;
    }

    toCfgRef(r)->timeStamp = v;
}

int dnv_vista_sdk_dcl_dto_cfg_ref_has_version(const dnv_vista_sdk_dcl_dto_cfg_ref_t* r)
{
    if (r == nullptr)
    {
        return 0;
    }

    return toCfgRef(r)->version.has_value() ? 1 : 0;
}

const char* dnv_vista_sdk_dcl_dto_cfg_ref_get_version(const dnv_vista_sdk_dcl_dto_cfg_ref_t* r)
{
    if (r == nullptr || !toCfgRef(r)->version.has_value())
    {
        return nullptr;
    }

    return toCfgRef(r)->version->c_str();
}

void dnv_vista_sdk_dcl_dto_cfg_ref_set_version(dnv_vista_sdk_dcl_dto_cfg_ref_t* r, const char* v)
{
    if (r == nullptr || v == nullptr)
    {
        return;
    }

    toCfgRef(r)->version = v;
}

void dnv_vista_sdk_dcl_dto_cfg_ref_clear_version(dnv_vista_sdk_dcl_dto_cfg_ref_t* r)
{
    if (r == nullptr)
    {
        return;
    }

    toCfgRef(r)->version.reset();
}

/*=========================================================================
 * VersionInformationDto
 *=======================================================================*/

const char* dnv_vista_sdk_dcl_dto_ver_info_get_naming_rule(const dnv_vista_sdk_dcl_dto_ver_info_t* v)
{
    if (v == nullptr)
    {
        return nullptr;
    }

    return toVerInfo(v)->namingRule.c_str();
}

void dnv_vista_sdk_dcl_dto_ver_info_set_naming_rule(dnv_vista_sdk_dcl_dto_ver_info_t* v, const char* s)
{
    if (v == nullptr || s == nullptr)
    {
        return;
    }

    toVerInfo(v)->namingRule = s;
}

const char* dnv_vista_sdk_dcl_dto_ver_info_get_naming_scheme_version(const dnv_vista_sdk_dcl_dto_ver_info_t* v)
{
    if (v == nullptr)
    {
        return nullptr;
    }

    return toVerInfo(v)->namingSchemeVersion.c_str();
}

void dnv_vista_sdk_dcl_dto_ver_info_set_naming_scheme_version(dnv_vista_sdk_dcl_dto_ver_info_t* v, const char* s)
{
    if (v == nullptr || s == nullptr)
    {
        return;
    }

    toVerInfo(v)->namingSchemeVersion = s;
}

int dnv_vista_sdk_dcl_dto_ver_info_has_reference_url(const dnv_vista_sdk_dcl_dto_ver_info_t* v)
{
    if (v == nullptr)
    {
        return 0;
    }

    return toVerInfo(v)->referenceUrl.has_value() ? 1 : 0;
}

const char* dnv_vista_sdk_dcl_dto_ver_info_get_reference_url(const dnv_vista_sdk_dcl_dto_ver_info_t* v)
{
    if (v == nullptr || !toVerInfo(v)->referenceUrl.has_value())
    {
        return nullptr;
    }

    return toVerInfo(v)->referenceUrl->c_str();
}

void dnv_vista_sdk_dcl_dto_ver_info_set_reference_url(dnv_vista_sdk_dcl_dto_ver_info_t* v, const char* s)
{
    if (v == nullptr || s == nullptr)
    {
        return;
    }

    toVerInfo(v)->referenceUrl = s;
}

void dnv_vista_sdk_dcl_dto_ver_info_clear_reference_url(dnv_vista_sdk_dcl_dto_ver_info_t* v)
{
    if (v == nullptr)
    {
        return;
    }

    toVerInfo(v)->referenceUrl.reset();
}

/*=========================================================================
 * DataChannelListDto
 *=======================================================================*/

size_t dnv_vista_sdk_dcl_dto_ch_list_count(const dnv_vista_sdk_dcl_dto_ch_list_t* list)
{
    if (list == nullptr)
    {
        return 0;
    }

    return toChList(list)->dataChannels.size();
}

dnv_vista_sdk_dcl_dto_channel_t* dnv_vista_sdk_dcl_dto_ch_list_at(dnv_vista_sdk_dcl_dto_ch_list_t* list, size_t index)
{
    if (list == nullptr)
    {
        return nullptr;
    }

    auto& channels = toChList(list)->dataChannels;
    if (index >= channels.size())
    {
        return nullptr;
    }

    return fromChannelRef(channels[index]);
}

dnv_vista_sdk_dcl_dto_channel_t* dnv_vista_sdk_dcl_dto_ch_list_push(dnv_vista_sdk_dcl_dto_ch_list_t* list)
{
    if (list == nullptr)
    {
        return nullptr;
    }

    toChList(list)->dataChannels.emplace_back();
    return fromChannelRef(toChList(list)->dataChannels.back());
}

void dnv_vista_sdk_dcl_dto_ch_list_remove(dnv_vista_sdk_dcl_dto_ch_list_t* list, size_t index)
{
    if (list == nullptr)
    {
        return;
    }

    auto& channels = toChList(list)->dataChannels;
    if (index >= channels.size())
    {
        return;
    }

    channels.erase(channels.begin() + static_cast<std::ptrdiff_t>(index));
}

/*=========================================================================
 * DataChannelDto
 *=======================================================================*/

dnv_vista_sdk_dcl_dto_ch_id_t* dnv_vista_sdk_dcl_dto_channel_get_id(dnv_vista_sdk_dcl_dto_channel_t* ch)
{
    if (ch == nullptr)
    {
        return nullptr;
    }

    return fromChIdRef(toChannel(ch)->dataChannelId);
}

dnv_vista_sdk_dcl_dto_property_t* dnv_vista_sdk_dcl_dto_channel_get_property(dnv_vista_sdk_dcl_dto_channel_t* ch)
{
    if (ch == nullptr)
    {
        return nullptr;
    }

    return fromPropertyRef(toChannel(ch)->property);
}

/*=========================================================================
 * DataChannelIdDto
 *=======================================================================*/

const char* dnv_vista_sdk_dcl_dto_ch_id_get_local_id(const dnv_vista_sdk_dcl_dto_ch_id_t* id)
{
    if (id == nullptr)
    {
        return nullptr;
    }

    return toChId(id)->localId.c_str();
}

void dnv_vista_sdk_dcl_dto_ch_id_set_local_id(dnv_vista_sdk_dcl_dto_ch_id_t* id, const char* v)
{
    if (id == nullptr || v == nullptr)
    {
        return;
    }

    toChId(id)->localId = v;
}

int dnv_vista_sdk_dcl_dto_ch_id_has_short_id(const dnv_vista_sdk_dcl_dto_ch_id_t* id)
{
    if (id == nullptr)
    {
        return 0;
    }

    return toChId(id)->shortId.has_value() ? 1 : 0;
}

const char* dnv_vista_sdk_dcl_dto_ch_id_get_short_id(const dnv_vista_sdk_dcl_dto_ch_id_t* id)
{
    if (id == nullptr || !toChId(id)->shortId.has_value())
    {
        return nullptr;
    }

    return toChId(id)->shortId->c_str();
}

void dnv_vista_sdk_dcl_dto_ch_id_set_short_id(dnv_vista_sdk_dcl_dto_ch_id_t* id, const char* v)
{
    if (id == nullptr || v == nullptr)
    {
        return;
    }

    toChId(id)->shortId = v;
}

void dnv_vista_sdk_dcl_dto_ch_id_clear_short_id(dnv_vista_sdk_dcl_dto_ch_id_t* id)
{
    if (id == nullptr)
    {
        return;
    }

    toChId(id)->shortId.reset();
}

int dnv_vista_sdk_dcl_dto_ch_id_has_name_object(const dnv_vista_sdk_dcl_dto_ch_id_t* id)
{
    if (id == nullptr)
    {
        return 0;
    }

    return toChId(id)->nameObject.has_value() ? 1 : 0;
}

dnv_vista_sdk_dcl_dto_name_obj_t* dnv_vista_sdk_dcl_dto_ch_id_get_name_object(dnv_vista_sdk_dcl_dto_ch_id_t* id)
{
    if (id == nullptr || !toChId(id)->nameObject.has_value())
    {
        return nullptr;
    }

    return fromNameObjRef(*toChId(id)->nameObject);
}

void dnv_vista_sdk_dcl_dto_ch_id_ensure_name_object(dnv_vista_sdk_dcl_dto_ch_id_t* id)
{
    if (id == nullptr)
    {
        return;
    }

    if (!toChId(id)->nameObject.has_value())
    {
        toChId(id)->nameObject.emplace();
    }
}

void dnv_vista_sdk_dcl_dto_ch_id_clear_name_object(dnv_vista_sdk_dcl_dto_ch_id_t* id)
{
    if (id == nullptr)
    {
        return;
    }

    toChId(id)->nameObject.reset();
}

/*=========================================================================
 * NameObjectDto
 *=======================================================================*/

const char* dnv_vista_sdk_dcl_dto_name_obj_get_naming_rule(const dnv_vista_sdk_dcl_dto_name_obj_t* n)
{
    if (n == nullptr)
    {
        return nullptr;
    }

    return toNameObj(n)->namingRule.c_str();
}

void dnv_vista_sdk_dcl_dto_name_obj_set_naming_rule(dnv_vista_sdk_dcl_dto_name_obj_t* n, const char* v)
{
    if (n == nullptr || v == nullptr)
    {
        return;
    }

    toNameObj(n)->namingRule = v;
}

int dnv_vista_sdk_dcl_dto_name_obj_has_custom(const dnv_vista_sdk_dcl_dto_name_obj_t* n)
{
    if (n == nullptr)
    {
        return 0;
    }

    return toNameObj(n)->customNameObjects.has_value() ? 1 : 0;
}

dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_dcl_dto_name_obj_get_custom(dnv_vista_sdk_dcl_dto_name_obj_t* n)
{
    if (n == nullptr || !toNameObj(n)->customNameObjects.has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<dnv_vista_sdk_serializable_document_t*>(
        &*toNameObj(n)->customNameObjects);
}

void dnv_vista_sdk_dcl_dto_name_obj_ensure_custom(dnv_vista_sdk_dcl_dto_name_obj_t* n)
{
    if (n == nullptr)
    {
        return;
    }

    if (!toNameObj(n)->customNameObjects.has_value())
    {
        toNameObj(n)->customNameObjects.emplace();
    }
}

void dnv_vista_sdk_dcl_dto_name_obj_clear_custom(dnv_vista_sdk_dcl_dto_name_obj_t* n)
{
    if (n == nullptr)
    {
        return;
    }

    toNameObj(n)->customNameObjects.reset();
}

/*=========================================================================
 * PropertyDto
 *=======================================================================*/

dnv_vista_sdk_dcl_dto_ch_type_t* dnv_vista_sdk_dcl_dto_property_get_ch_type(dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return nullptr;
    }

    return fromChTypeRef(toProperty(p)->dataChannelType);
}

dnv_vista_sdk_dcl_dto_format_t* dnv_vista_sdk_dcl_dto_property_get_format(dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return nullptr;
    }

    return fromFormatRef(toProperty(p)->format);
}

int dnv_vista_sdk_dcl_dto_property_has_range(const dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return 0;
    }

    return toProperty(p)->range.has_value() ? 1 : 0;
}

dnv_vista_sdk_dcl_dto_range_t* dnv_vista_sdk_dcl_dto_property_get_range(dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr || !toProperty(p)->range.has_value())
    {
        return nullptr;
    }

    return fromRangeRef(*toProperty(p)->range);
}

void dnv_vista_sdk_dcl_dto_property_ensure_range(dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return;
    }

    if (!toProperty(p)->range.has_value())
    {
        toProperty(p)->range.emplace();
    }
}

void dnv_vista_sdk_dcl_dto_property_clear_range(dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return;
    }

    toProperty(p)->range.reset();
}

int dnv_vista_sdk_dcl_dto_property_has_unit(const dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return 0;
    }

    return toProperty(p)->unit.has_value() ? 1 : 0;
}

dnv_vista_sdk_dcl_dto_unit_t* dnv_vista_sdk_dcl_dto_property_get_unit(dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr || !toProperty(p)->unit.has_value())
    {
        return nullptr;
    }

    return fromUnitRef(*toProperty(p)->unit);
}

void dnv_vista_sdk_dcl_dto_property_ensure_unit(dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return;
    }

    if (!toProperty(p)->unit.has_value())
    {
        toProperty(p)->unit.emplace();
    }
}

void dnv_vista_sdk_dcl_dto_property_clear_unit(dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return;
    }

    toProperty(p)->unit.reset();
}

int dnv_vista_sdk_dcl_dto_property_has_quality_coding(const dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return 0;
    }

    return toProperty(p)->qualityCoding.has_value() ? 1 : 0;
}

const char* dnv_vista_sdk_dcl_dto_property_get_quality_coding(const dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr || !toProperty(p)->qualityCoding.has_value())
    {
        return nullptr;
    }

    return toProperty(p)->qualityCoding->c_str();
}

void dnv_vista_sdk_dcl_dto_property_set_quality_coding(dnv_vista_sdk_dcl_dto_property_t* p, const char* v)
{
    if (p == nullptr || v == nullptr)
    {
        return;
    }

    toProperty(p)->qualityCoding = v;
}

void dnv_vista_sdk_dcl_dto_property_clear_quality_coding(dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return;
    }

    toProperty(p)->qualityCoding.reset();
}

int dnv_vista_sdk_dcl_dto_property_has_alert_priority(const dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return 0;
    }

    return toProperty(p)->alertPriority.has_value() ? 1 : 0;
}

const char* dnv_vista_sdk_dcl_dto_property_get_alert_priority(const dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr || !toProperty(p)->alertPriority.has_value())
    {
        return nullptr;
    }

    return toProperty(p)->alertPriority->c_str();
}

void dnv_vista_sdk_dcl_dto_property_set_alert_priority(dnv_vista_sdk_dcl_dto_property_t* p, const char* v)
{
    if (p == nullptr || v == nullptr)
    {
        return;
    }

    toProperty(p)->alertPriority = v;
}

void dnv_vista_sdk_dcl_dto_property_clear_alert_priority(dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return;
    }

    toProperty(p)->alertPriority.reset();
}

int dnv_vista_sdk_dcl_dto_property_has_name(const dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return 0;
    }

    return toProperty(p)->name.has_value() ? 1 : 0;
}

const char* dnv_vista_sdk_dcl_dto_property_get_name(const dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr || !toProperty(p)->name.has_value())
    {
        return nullptr;
    }

    return toProperty(p)->name->c_str();
}

void dnv_vista_sdk_dcl_dto_property_set_name(dnv_vista_sdk_dcl_dto_property_t* p, const char* v)
{
    if (p == nullptr || v == nullptr)
    {
        return;
    }

    toProperty(p)->name = v;
}

void dnv_vista_sdk_dcl_dto_property_clear_name(dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return;
    }

    toProperty(p)->name.reset();
}

int dnv_vista_sdk_dcl_dto_property_has_remarks(const dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return 0;
    }

    return toProperty(p)->remarks.has_value() ? 1 : 0;
}

const char* dnv_vista_sdk_dcl_dto_property_get_remarks(const dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr || !toProperty(p)->remarks.has_value())
    {
        return nullptr;
    }

    return toProperty(p)->remarks->c_str();
}

void dnv_vista_sdk_dcl_dto_property_set_remarks(dnv_vista_sdk_dcl_dto_property_t* p, const char* v)
{
    if (p == nullptr || v == nullptr)
    {
        return;
    }

    toProperty(p)->remarks = v;
}

void dnv_vista_sdk_dcl_dto_property_clear_remarks(dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return;
    }

    toProperty(p)->remarks.reset();
}

int dnv_vista_sdk_dcl_dto_property_has_custom_properties(const dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return 0;
    }

    return toProperty(p)->customProperties.has_value() ? 1 : 0;
}

dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_dcl_dto_property_get_custom_properties(
    dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr || !toProperty(p)->customProperties.has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<dnv_vista_sdk_serializable_document_t*>(&*toProperty(p)->customProperties);
}

void dnv_vista_sdk_dcl_dto_property_ensure_custom_properties(dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return;
    }

    if (!toProperty(p)->customProperties.has_value())
    {
        toProperty(p)->customProperties.emplace();
    }
}

void dnv_vista_sdk_dcl_dto_property_clear_custom_properties(dnv_vista_sdk_dcl_dto_property_t* p)
{
    if (p == nullptr)
    {
        return;
    }

    toProperty(p)->customProperties.reset();
}

/*=========================================================================
 * DataChannelTypeDto
 *=======================================================================*/

const char* dnv_vista_sdk_dcl_dto_ch_type_get_type(const dnv_vista_sdk_dcl_dto_ch_type_t* t)
{
    if (t == nullptr)
    {
        return nullptr;
    }

    return toChType(t)->type.c_str();
}

void dnv_vista_sdk_dcl_dto_ch_type_set_type(dnv_vista_sdk_dcl_dto_ch_type_t* t, const char* v)
{
    if (t == nullptr || v == nullptr)
    {
        return;
    }

    toChType(t)->type = v;
}

int dnv_vista_sdk_dcl_dto_ch_type_has_update_cycle(const dnv_vista_sdk_dcl_dto_ch_type_t* t)
{
    if (t == nullptr)
    {
        return 0;
    }

    return toChType(t)->updateCycle.has_value() ? 1 : 0;
}

double dnv_vista_sdk_dcl_dto_ch_type_get_update_cycle(const dnv_vista_sdk_dcl_dto_ch_type_t* t)
{
    if (t == nullptr || !toChType(t)->updateCycle.has_value())
    {
        return 0.0;
    }

    return *toChType(t)->updateCycle;
}

void dnv_vista_sdk_dcl_dto_ch_type_set_update_cycle(dnv_vista_sdk_dcl_dto_ch_type_t* t, double v)
{
    if (t == nullptr)
    {
        return;
    }

    toChType(t)->updateCycle = v;
}

void dnv_vista_sdk_dcl_dto_ch_type_clear_update_cycle(dnv_vista_sdk_dcl_dto_ch_type_t* t)
{
    if (t == nullptr)
    {
        return;
    }

    toChType(t)->updateCycle.reset();
}

int dnv_vista_sdk_dcl_dto_ch_type_has_calculation_period(const dnv_vista_sdk_dcl_dto_ch_type_t* t)
{
    if (t == nullptr)
    {
        return 0;
    }

    return toChType(t)->calculationPeriod.has_value() ? 1 : 0;
}

double dnv_vista_sdk_dcl_dto_ch_type_get_calculation_period(const dnv_vista_sdk_dcl_dto_ch_type_t* t)
{
    if (t == nullptr || !toChType(t)->calculationPeriod.has_value())
    {
        return 0.0;
    }

    return *toChType(t)->calculationPeriod;
}

void dnv_vista_sdk_dcl_dto_ch_type_set_calculation_period(dnv_vista_sdk_dcl_dto_ch_type_t* t, double v)
{
    if (t == nullptr)
    {
        return;
    }

    toChType(t)->calculationPeriod = v;
}

void dnv_vista_sdk_dcl_dto_ch_type_clear_calculation_period(dnv_vista_sdk_dcl_dto_ch_type_t* t)
{
    if (t == nullptr)
    {
        return;
    }

    toChType(t)->calculationPeriod.reset();
}

/*=========================================================================
 * FormatDto
 *=======================================================================*/

const char* dnv_vista_sdk_dcl_dto_format_get_type(const dnv_vista_sdk_dcl_dto_format_t* f)
{
    if (f == nullptr)
    {
        return nullptr;
    }

    return toFormat(f)->type.c_str();
}

void dnv_vista_sdk_dcl_dto_format_set_type(dnv_vista_sdk_dcl_dto_format_t* f, const char* v)
{
    if (f == nullptr || v == nullptr)
    {
        return;
    }

    toFormat(f)->type = v;
}

int dnv_vista_sdk_dcl_dto_format_has_restriction(const dnv_vista_sdk_dcl_dto_format_t* f)
{
    if (f == nullptr)
    {
        return 0;
    }

    return toFormat(f)->restriction.has_value() ? 1 : 0;
}

dnv_vista_sdk_dcl_dto_restriction_t* dnv_vista_sdk_dcl_dto_format_get_restriction(dnv_vista_sdk_dcl_dto_format_t* f)
{
    if (f == nullptr || !toFormat(f)->restriction.has_value())
    {
        return nullptr;
    }

    return fromRestrictionRef(*toFormat(f)->restriction);
}

void dnv_vista_sdk_dcl_dto_format_ensure_restriction(dnv_vista_sdk_dcl_dto_format_t* f)
{
    if (f == nullptr)
    {
        return;
    }

    if (!toFormat(f)->restriction.has_value())
    {
        toFormat(f)->restriction.emplace();
    }
}

void dnv_vista_sdk_dcl_dto_format_clear_restriction(dnv_vista_sdk_dcl_dto_format_t* f)
{
    if (f == nullptr)
    {
        return;
    }

    toFormat(f)->restriction.reset();
}

/*=========================================================================
 * RestrictionDto
 *=======================================================================*/

size_t dnv_vista_sdk_dcl_dto_restriction_enumeration_count(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->enumeration.has_value())
    {
        return 0;
    }

    return toRestriction(r)->enumeration->size();
}

const char* dnv_vista_sdk_dcl_dto_restriction_enumeration_at(const dnv_vista_sdk_dcl_dto_restriction_t* r, size_t i)
{
    if (r == nullptr || !toRestriction(r)->enumeration.has_value())
    {
        return nullptr;
    }

    const auto& e = *toRestriction(r)->enumeration;
    if (i >= e.size())
    {
        return nullptr;
    }

    return e[i].c_str();
}

void dnv_vista_sdk_dcl_dto_restriction_set_enumeration(
    dnv_vista_sdk_dcl_dto_restriction_t* r, const char* const* values, size_t count)
{
    if (r == nullptr)
    {
        return;
    }

    std::vector<std::string> v;
    v.reserve(count);
    for (size_t i = 0; i < count; ++i)
    {
        v.emplace_back(values[i]);
    }

    toRestriction(r)->enumeration = std::move(v);
}

void dnv_vista_sdk_dcl_dto_restriction_clear_enumeration(dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->enumeration.reset();
}

int dnv_vista_sdk_dcl_dto_restriction_has_fraction_digits(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return 0;
    }

    return toRestriction(r)->fractionDigits.has_value() ? 1 : 0;
}

uint32_t dnv_vista_sdk_dcl_dto_restriction_get_fraction_digits(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->fractionDigits.has_value())
    {
        return 0;
    }

    return *toRestriction(r)->fractionDigits;
}

void dnv_vista_sdk_dcl_dto_restriction_set_fraction_digits(dnv_vista_sdk_dcl_dto_restriction_t* r, uint32_t v)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->fractionDigits = v;
}

void dnv_vista_sdk_dcl_dto_restriction_clear_fraction_digits(dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->fractionDigits.reset();
}

int dnv_vista_sdk_dcl_dto_restriction_has_length(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return 0;
    }

    return toRestriction(r)->length.has_value() ? 1 : 0;
}

uint32_t dnv_vista_sdk_dcl_dto_restriction_get_length(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->length.has_value())
    {
        return 0;
    }

    return *toRestriction(r)->length;
}

void dnv_vista_sdk_dcl_dto_restriction_set_length(dnv_vista_sdk_dcl_dto_restriction_t* r, uint32_t v)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->length = v;
}

void dnv_vista_sdk_dcl_dto_restriction_clear_length(dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->length.reset();
}

int dnv_vista_sdk_dcl_dto_restriction_has_max_exclusive(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return 0;
    }

    return toRestriction(r)->maxExclusive.has_value() ? 1 : 0;
}

double dnv_vista_sdk_dcl_dto_restriction_get_max_exclusive(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->maxExclusive.has_value())
    {
        return 0.0;
    }

    return *toRestriction(r)->maxExclusive;
}

void dnv_vista_sdk_dcl_dto_restriction_set_max_exclusive(dnv_vista_sdk_dcl_dto_restriction_t* r, double v)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->maxExclusive = v;
}

void dnv_vista_sdk_dcl_dto_restriction_clear_max_exclusive(dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->maxExclusive.reset();
}

int dnv_vista_sdk_dcl_dto_restriction_has_max_inclusive(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return 0;
    }

    return toRestriction(r)->maxInclusive.has_value() ? 1 : 0;
}

double dnv_vista_sdk_dcl_dto_restriction_get_max_inclusive(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->maxInclusive.has_value())
    {
        return 0.0;
    }

    return *toRestriction(r)->maxInclusive;
}

void dnv_vista_sdk_dcl_dto_restriction_set_max_inclusive(dnv_vista_sdk_dcl_dto_restriction_t* r, double v)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->maxInclusive = v;
}

void dnv_vista_sdk_dcl_dto_restriction_clear_max_inclusive(dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->maxInclusive.reset();
}

int dnv_vista_sdk_dcl_dto_restriction_has_max_length(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return 0;
    }

    return toRestriction(r)->maxLength.has_value() ? 1 : 0;
}

uint32_t dnv_vista_sdk_dcl_dto_restriction_get_max_length(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->maxLength.has_value())
    {
        return 0;
    }

    return *toRestriction(r)->maxLength;
}

void dnv_vista_sdk_dcl_dto_restriction_set_max_length(dnv_vista_sdk_dcl_dto_restriction_t* r, uint32_t v)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->maxLength = v;
}

void dnv_vista_sdk_dcl_dto_restriction_clear_max_length(dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->maxLength.reset();
}

int dnv_vista_sdk_dcl_dto_restriction_has_min_exclusive(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return 0;
    }

    return toRestriction(r)->minExclusive.has_value() ? 1 : 0;
}

double dnv_vista_sdk_dcl_dto_restriction_get_min_exclusive(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->minExclusive.has_value())
    {
        return 0.0;
    }

    return *toRestriction(r)->minExclusive;
}

void dnv_vista_sdk_dcl_dto_restriction_set_min_exclusive(dnv_vista_sdk_dcl_dto_restriction_t* r, double v)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->minExclusive = v;
}

void dnv_vista_sdk_dcl_dto_restriction_clear_min_exclusive(dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->minExclusive.reset();
}

int dnv_vista_sdk_dcl_dto_restriction_has_min_inclusive(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return 0;
    }

    return toRestriction(r)->minInclusive.has_value() ? 1 : 0;
}

double dnv_vista_sdk_dcl_dto_restriction_get_min_inclusive(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->minInclusive.has_value())
    {
        return 0.0;
    }

    return *toRestriction(r)->minInclusive;
}

void dnv_vista_sdk_dcl_dto_restriction_set_min_inclusive(dnv_vista_sdk_dcl_dto_restriction_t* r, double v)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->minInclusive = v;
}

void dnv_vista_sdk_dcl_dto_restriction_clear_min_inclusive(dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->minInclusive.reset();
}

int dnv_vista_sdk_dcl_dto_restriction_has_min_length(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return 0;
    }

    return toRestriction(r)->minLength.has_value() ? 1 : 0;
}

uint32_t dnv_vista_sdk_dcl_dto_restriction_get_min_length(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->minLength.has_value())
    {
        return 0;
    }

    return *toRestriction(r)->minLength;
}

void dnv_vista_sdk_dcl_dto_restriction_set_min_length(dnv_vista_sdk_dcl_dto_restriction_t* r, uint32_t v)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->minLength = v;
}

void dnv_vista_sdk_dcl_dto_restriction_clear_min_length(dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->minLength.reset();
}

int dnv_vista_sdk_dcl_dto_restriction_has_pattern(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return 0;
    }

    return toRestriction(r)->pattern.has_value() ? 1 : 0;
}

const char* dnv_vista_sdk_dcl_dto_restriction_get_pattern(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->pattern.has_value())
    {
        return nullptr;
    }

    return toRestriction(r)->pattern->c_str();
}

void dnv_vista_sdk_dcl_dto_restriction_set_pattern(dnv_vista_sdk_dcl_dto_restriction_t* r, const char* v)
{
    if (r == nullptr || v == nullptr)
    {
        return;
    }

    toRestriction(r)->pattern = v;
}

void dnv_vista_sdk_dcl_dto_restriction_clear_pattern(dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->pattern.reset();
}

int dnv_vista_sdk_dcl_dto_restriction_has_total_digits(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return 0;
    }

    return toRestriction(r)->totalDigits.has_value() ? 1 : 0;
}

uint32_t dnv_vista_sdk_dcl_dto_restriction_get_total_digits(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->totalDigits.has_value())
    {
        return 0;
    }

    return *toRestriction(r)->totalDigits;
}

void dnv_vista_sdk_dcl_dto_restriction_set_total_digits(dnv_vista_sdk_dcl_dto_restriction_t* r, uint32_t v)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->totalDigits = v;
}

void dnv_vista_sdk_dcl_dto_restriction_clear_total_digits(dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->totalDigits.reset();
}

int dnv_vista_sdk_dcl_dto_restriction_has_white_space(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return 0;
    }

    return toRestriction(r)->whiteSpace.has_value() ? 1 : 0;
}

const char* dnv_vista_sdk_dcl_dto_restriction_get_white_space(const dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->whiteSpace.has_value())
    {
        return nullptr;
    }

    return toRestriction(r)->whiteSpace->c_str();
}

void dnv_vista_sdk_dcl_dto_restriction_set_white_space(dnv_vista_sdk_dcl_dto_restriction_t* r, const char* v)
{
    if (r == nullptr || v == nullptr)
    {
        return;
    }

    toRestriction(r)->whiteSpace = v;
}

void dnv_vista_sdk_dcl_dto_restriction_clear_white_space(dnv_vista_sdk_dcl_dto_restriction_t* r)
{
    if (r == nullptr)
    {
        return;
    }

    toRestriction(r)->whiteSpace.reset();
}

/*=========================================================================
 * RangeDto
 *=======================================================================*/

double dnv_vista_sdk_dcl_dto_range_get_low(const dnv_vista_sdk_dcl_dto_range_t* r)
{
    if (r == nullptr)
    {
        return 0.0;
    }

    return toRange(r)->low;
}

void dnv_vista_sdk_dcl_dto_range_set_low(dnv_vista_sdk_dcl_dto_range_t* r, double v)
{
    if (r == nullptr)
    {
        return;
    }

    toRange(r)->low = v;
}

double dnv_vista_sdk_dcl_dto_range_get_high(const dnv_vista_sdk_dcl_dto_range_t* r)
{
    if (r == nullptr)
    {
        return 0.0;
    }

    return toRange(r)->high;
}

void dnv_vista_sdk_dcl_dto_range_set_high(dnv_vista_sdk_dcl_dto_range_t* r, double v)
{
    if (r == nullptr)
    {
        return;
    }

    toRange(r)->high = v;
}

/*=========================================================================
 * UnitDto
 *=======================================================================*/

const char* dnv_vista_sdk_dcl_dto_unit_get_symbol(const dnv_vista_sdk_dcl_dto_unit_t* u)
{
    if (u == nullptr)
    {
        return nullptr;
    }

    return toUnit(u)->unitSymbol.c_str();
}

void dnv_vista_sdk_dcl_dto_unit_set_symbol(dnv_vista_sdk_dcl_dto_unit_t* u, const char* v)
{
    if (u == nullptr || v == nullptr)
    {
        return;
    }

    toUnit(u)->unitSymbol = v;
}

int dnv_vista_sdk_dcl_dto_unit_has_quantity_name(const dnv_vista_sdk_dcl_dto_unit_t* u)
{
    if (u == nullptr)
    {
        return 0;
    }

    return toUnit(u)->quantityName.has_value() ? 1 : 0;
}

const char* dnv_vista_sdk_dcl_dto_unit_get_quantity_name(const dnv_vista_sdk_dcl_dto_unit_t* u)
{
    if (u == nullptr || !toUnit(u)->quantityName.has_value())
    {
        return nullptr;
    }

    return toUnit(u)->quantityName->c_str();
}

void dnv_vista_sdk_dcl_dto_unit_set_quantity_name(dnv_vista_sdk_dcl_dto_unit_t* u, const char* v)
{
    if (u == nullptr || v == nullptr)
    {
        return;
    }

    toUnit(u)->quantityName = v;
}

void dnv_vista_sdk_dcl_dto_unit_clear_quantity_name(dnv_vista_sdk_dcl_dto_unit_t* u)
{
    if (u == nullptr)
    {
        return;
    }

    toUnit(u)->quantityName.reset();
}

int dnv_vista_sdk_dcl_dto_unit_has_custom_elements(const dnv_vista_sdk_dcl_dto_unit_t* u)
{
    if (u == nullptr)
    {
        return 0;
    }

    return toUnit(u)->customElements.has_value() ? 1 : 0;
}

dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_dcl_dto_unit_get_custom_elements(dnv_vista_sdk_dcl_dto_unit_t* u)
{
    if (u == nullptr || !toUnit(u)->customElements.has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<dnv_vista_sdk_serializable_document_t*>(
        &*toUnit(u)->customElements);
}

void dnv_vista_sdk_dcl_dto_unit_ensure_custom_elements(dnv_vista_sdk_dcl_dto_unit_t* u)
{
    if (u == nullptr)
    {
        return;
    }

    if (!toUnit(u)->customElements.has_value())
    {
        toUnit(u)->customElements.emplace();
    }
}

void dnv_vista_sdk_dcl_dto_unit_clear_custom_elements(dnv_vista_sdk_dcl_dto_unit_t* u)
{
    if (u == nullptr)
    {
        return;
    }

    toUnit(u)->customElements.reset();
}
