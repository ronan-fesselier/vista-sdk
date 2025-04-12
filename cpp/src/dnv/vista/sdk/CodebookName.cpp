#include "pch.h"

#include "dnv/vista/sdk/CodebookName.h"

namespace dnv::vista::sdk
{
	CodebookName CodebookNames::FromPrefix( const std::string_view prefix )
	{
		if ( prefix.empty() )
			throw std::invalid_argument( "prefix" );

		if ( prefix == "pos" )
			return CodebookName::Position;
		else if ( prefix == "qty" )
			return CodebookName::Quantity;
		else if ( prefix == "calc" )
			return CodebookName::Calculation;
		else if ( prefix == "state" )
			return CodebookName::State;
		else if ( prefix == "cnt" )
			return CodebookName::Content;
		else if ( prefix == "cmd" )
			return CodebookName::Command;
		else if ( prefix == "type" )
			return CodebookName::Type;
		else if ( prefix == "funct.svc" )
			return CodebookName::FunctionalServices;
		else if ( prefix == "maint.cat" )
			return CodebookName::MaintenanceCategory;
		else if ( prefix == "act.type" )
			return CodebookName::ActivityType;
		else if ( prefix == "detail" )
			return CodebookName::Detail;
		else
		{
			SPDLOG_ERROR( "Unknown prefix: {}", prefix );
			throw std::invalid_argument( "unknown prefix: " + std::string( prefix ) );
		}
	}

	std::string CodebookNames::ToPrefix( CodebookName name )
	{
		switch ( name )
		{
			case CodebookName::Position:
				return "pos";
			case CodebookName::Quantity:
				return "qty";
			case CodebookName::Calculation:
				return "calc";
			case CodebookName::State:
				return "state";
			case CodebookName::Content:
				return "cnt";
			case CodebookName::Command:
				return "cmd";
			case CodebookName::Type:
				return "type";
			case CodebookName::FunctionalServices:
				return "funct.svc";
			case CodebookName::MaintenanceCategory:
				return "maint.cat";
			case CodebookName::ActivityType:
				return "act.type";
			case CodebookName::Detail:
				return "detail";
			default:
			{
				SPDLOG_ERROR( "Unknown codebook: {}", static_cast<int>( name ) );
				throw std::invalid_argument( "unknown codebook: " + std::to_string( static_cast<int>( name ) ) );
			}
		}
	}
}
