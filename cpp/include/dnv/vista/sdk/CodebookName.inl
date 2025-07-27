/**
 * @file CodebookName.inl
 * @brief Inline implementations for performance-critical CodebookName operations
 */

#pragma once

#include "config/CodebookConstants.h"
#include "config/Platform.h"

namespace dnv::vista::sdk
{
	namespace
	{
		//=====================================================================
		// Prefix-to-CodebookName mapping table
		//=====================================================================

		struct PrefixMapping
		{
			std::string_view prefix;
			CodebookName name;
		};

		static constexpr std::array<PrefixMapping, 11> s_prefixMappings{
			{ { codebook::CODEBOOK_PREFIX_POSITION, CodebookName::Position },
				{ codebook::CODEBOOK_PREFIX_QUANTITY, CodebookName::Quantity },
				{ codebook::CODEBOOK_PREFIX_STATE, CodebookName::State },
				{ codebook::CODEBOOK_PREFIX_CONTENT, CodebookName::Content },
				{ codebook::CODEBOOK_PREFIX_COMMAND, CodebookName::Command },
				{ codebook::CODEBOOK_PREFIX_TYPE, CodebookName::Type },
				{ codebook::CODEBOOK_PREFIX_CALCULATION, CodebookName::Calculation },
				{ codebook::CODEBOOK_PREFIX_DETAIL, CodebookName::Detail },
				{ codebook::CODEBOOK_PREFIX_FUNCTIONAL_SERVICES, CodebookName::FunctionalServices },
				{ codebook::CODEBOOK_PREFIX_MAINTENANCE_CATEGORY, CodebookName::MaintenanceCategory },
				{ codebook::CODEBOOK_PREFIX_ACTIVITY_TYPE, CodebookName::ActivityType } } };
	}

	//=====================================================================
	// CodebookNames class
	//=====================================================================

	//----------------------------------------------
	// Public static methods
	//----------------------------------------------

	inline CodebookName CodebookNames::fromPrefix( std::string_view prefix )
	{
		if ( prefix.empty() )
		{
			throw std::invalid_argument( "Prefix cannot be empty." );
		}

		for ( const auto& mapping : s_prefixMappings )
		{
			if ( mapping.prefix == prefix )
			{
				return mapping.name;
			}
		}

		throw std::invalid_argument( fmt::format( "Unknown prefix: {}", prefix ) );
	}

	inline std::string_view CodebookNames::toPrefix( CodebookName name )
	{
		switch ( name )
		{
			case CodebookName::Position:
			{
				return codebook::CODEBOOK_PREFIX_POSITION;
			}
			case CodebookName::Quantity:
			{
				return codebook::CODEBOOK_PREFIX_QUANTITY;
			}
			case CodebookName::Calculation:
			{
				return codebook::CODEBOOK_PREFIX_CALCULATION;
			}
			case CodebookName::State:
			{
				return codebook::CODEBOOK_PREFIX_STATE;
			}
			case CodebookName::Content:
			{
				return codebook::CODEBOOK_PREFIX_CONTENT;
			}
			case CodebookName::Command:
			{
				return codebook::CODEBOOK_PREFIX_COMMAND;
			}
			case CodebookName::Type:
			{
				return codebook::CODEBOOK_PREFIX_TYPE;
			}
			case CodebookName::FunctionalServices:
			{
				return codebook::CODEBOOK_PREFIX_FUNCTIONAL_SERVICES;
			}
			case CodebookName::MaintenanceCategory:
			{
				return codebook::CODEBOOK_PREFIX_MAINTENANCE_CATEGORY;
			}
			case CodebookName::ActivityType:
			{
				return codebook::CODEBOOK_PREFIX_ACTIVITY_TYPE;
			}
			case CodebookName::Detail:
			{
				return codebook::CODEBOOK_PREFIX_DETAIL;
			}
			default:
			{
				throw std::invalid_argument( fmt::format( "Unknown codebook: {}", static_cast<int>( name ) ) );
			}
		}
	}
}
