#include "pch.h"

#include "dnv/vista/sdk/CodebookName.h"

namespace dnv::vista::sdk
{
	namespace
	{
		constexpr const char* POSITION_PREFIX = "pos";
		constexpr const char* QUANTITY_PREFIX = "qty";
		constexpr const char* CALCULATION_PREFIX = "calc";
		constexpr const char* STATE_PREFIX = "state";
		constexpr const char* CONTENT_PREFIX = "cnt";
		constexpr const char* COMMAND_PREFIX = "cmd";
		constexpr const char* TYPE_PREFIX = "type";
		constexpr const char* FUNCTIONAL_SERVICES_PREFIX = "funct.svc";
		constexpr const char* MAINTENANCE_CATEGORY_PREFIX = "maint.cat";
		constexpr const char* ACTIVITY_TYPE_PREFIX = "act.type";
		constexpr const char* DETAIL_PREFIX = "detail";
	}

	CodebookName CodebookNames::fromPrefix( const std::string_view prefix )
	{
		if ( prefix.empty() )
		{
			SPDLOG_ERROR( "Rejecting empty codebook prefix" );
			throw std::invalid_argument( "prefix" );
		}

		SPDLOG_INFO( "Converting prefix '{}' to CodebookName enum", prefix );

		static const std::unordered_map<std::string_view, CodebookName> prefixMap = {
			{ POSITION_PREFIX, CodebookName::Position },
			{ QUANTITY_PREFIX, CodebookName::Quantity },
			{ CALCULATION_PREFIX, CodebookName::Calculation },
			{ STATE_PREFIX, CodebookName::State },
			{ CONTENT_PREFIX, CodebookName::Content },
			{ COMMAND_PREFIX, CodebookName::Command },
			{ TYPE_PREFIX, CodebookName::Type },
			{ FUNCTIONAL_SERVICES_PREFIX, CodebookName::FunctionalServices },
			{ MAINTENANCE_CATEGORY_PREFIX, CodebookName::MaintenanceCategory },
			{ ACTIVITY_TYPE_PREFIX, CodebookName::ActivityType },
			{ DETAIL_PREFIX, CodebookName::Detail } };

		auto it = prefixMap.find( prefix );
		if ( it != prefixMap.end() )
		{
			SPDLOG_INFO( "Successfully mapped prefix '{}' to CodebookName enum value {}",
				prefix, static_cast<int>( it->second ) );
			return it->second;
		}

		SPDLOG_WARN( "Unknown prefix: '{}'. Valid prefixes are: pos, qty, calc, state, cnt, cmd, type, funct.svc, maint.cat, act.type, detail", prefix );
		throw std::invalid_argument( "unknown prefix: " + std::string( prefix ) );
	}

	std::string CodebookNames::toPrefix( CodebookName name )
	{
		SPDLOG_INFO( "Converting CodebookName enum {} to prefix string", static_cast<int>( name ) );

		std::string result;
		switch ( name )
		{
			case CodebookName::Position:
				result = POSITION_PREFIX;
				break;
			case CodebookName::Quantity:
				result = QUANTITY_PREFIX;
				break;
			case CodebookName::Calculation:
				result = CALCULATION_PREFIX;
				break;
			case CodebookName::State:
				result = STATE_PREFIX;
				break;
			case CodebookName::Content:
				result = CONTENT_PREFIX;
				break;
			case CodebookName::Command:
				result = COMMAND_PREFIX;
				break;
			case CodebookName::Type:
				result = TYPE_PREFIX;
				break;
			case CodebookName::FunctionalServices:
				result = FUNCTIONAL_SERVICES_PREFIX;
				break;
			case CodebookName::MaintenanceCategory:
				result = MAINTENANCE_CATEGORY_PREFIX;
				break;
			case CodebookName::ActivityType:
				result = ACTIVITY_TYPE_PREFIX;
				break;
			case CodebookName::Detail:
				result = DETAIL_PREFIX;
				break;
			case CodebookName::Unknown:
				SPDLOG_WARN( "Converting Unknown codebook (value 0) to empty string" );
				result = "";
				break;
			default:
			{
				SPDLOG_WARN( "Unknown codebook: {}", static_cast<int>( name ) );
				throw std::invalid_argument( "unknown codebook: " + std::to_string( static_cast<int>( name ) ) );
			}
		}

		SPDLOG_INFO( "Successfully converted CodebookName enum value {} to prefix '{}'",
			static_cast<int>( name ), result );
		return result;
	}
}
