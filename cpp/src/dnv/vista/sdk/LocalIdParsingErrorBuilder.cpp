/**
 * @file LocalIdParsingErrorBuilder.cpp
 * @brief Implementation of the LocalIdParsingErrorBuilder class.
 */

#include "pch.h"

#include "dnv/vista/sdk/LocalIdParsingErrorBuilder.h"

#include "dnv/vista/sdk/ParsingErrors.h"

namespace dnv::vista::sdk
{
	namespace
	{
		//=====================================================================
		// Static data maps
		//=====================================================================

		static const std::unordered_map<LocalIdParsingState, std::string> PREDEFINED_ERROR_MESSAGES = {
			{ LocalIdParsingState::NamingRule, "Missing or invalid naming rule" },
			{ LocalIdParsingState::VisVersion, "Missing or invalid vis version" },
			{ LocalIdParsingState::PrimaryItem, "Invalid or missing Primary item. Local IDs require atleast primary item and 1 metadata tag." },
			{ LocalIdParsingState::SecondaryItem, "Invalid secondary item" },
			{ LocalIdParsingState::ItemDescription, "Missing or invalid /meta prefix" },
			{ LocalIdParsingState::MetaQuantity, "Invalid metadata tag: Quantity" },
			{ LocalIdParsingState::MetaContent, "Invalid metadata tag: Content" },
			{ LocalIdParsingState::MetaCommand, "Invalid metadata tag: Command" },
			{ LocalIdParsingState::MetaPosition, "Invalid metadata tag: Position" },
			{ LocalIdParsingState::MetaCalculation, "Invalid metadata tag: Calculation" },
			{ LocalIdParsingState::MetaState, "Invalid metadata tag: State" },
			{ LocalIdParsingState::MetaType, "Invalid metadata tag: Type" },
			{ LocalIdParsingState::MetaDetail, "Invalid metadata tag: Detail" },
			{ LocalIdParsingState::EmptyState, "Missing primary path or metadata" },

			{ LocalIdParsingState::Formatting, "Formatting error in Local ID string" },
			{ LocalIdParsingState::Completeness, "Incomplete Local ID structure" },
			{ LocalIdParsingState::NamingEntity, "Invalid naming entity" },
			{ LocalIdParsingState::IMONumber, "Invalid IMO number" } };

		static const std::unordered_map<LocalIdParsingState, std::string> STATE_TO_STRING = {
			{ LocalIdParsingState::NamingRule, "NamingRule" },
			{ LocalIdParsingState::VisVersion, "VisVersion" },
			{ LocalIdParsingState::PrimaryItem, "PrimaryItem" },
			{ LocalIdParsingState::SecondaryItem, "SecondaryItem" },
			{ LocalIdParsingState::ItemDescription, "ItemDescription" },
			{ LocalIdParsingState::MetaQuantity, "MetaQuantity" },
			{ LocalIdParsingState::MetaContent, "MetaContent" },
			{ LocalIdParsingState::MetaCommand, "MetaCommand" },
			{ LocalIdParsingState::MetaPosition, "MetaPosition" },
			{ LocalIdParsingState::MetaCalculation, "MetaCalculation" },
			{ LocalIdParsingState::MetaState, "MetaState" },
			{ LocalIdParsingState::MetaType, "MetaType" },
			{ LocalIdParsingState::MetaDetail, "MetaDetail" },
			{ LocalIdParsingState::EmptyState, "EmptyState" },

			{ LocalIdParsingState::Formatting, "Formatting" },
			{ LocalIdParsingState::Completeness, "Completeness" },
			{ LocalIdParsingState::NamingEntity, "NamingEntity" },
			{ LocalIdParsingState::IMONumber, "IMONumber" } };
	}

	//=====================================================================
	// LocalIdParsingErrorBuilder class
	//=====================================================================

	//----------------------------------------------
	// Static factory method
	//----------------------------------------------

	LocalIdParsingErrorBuilder LocalIdParsingErrorBuilder::create()
	{
		return LocalIdParsingErrorBuilder{};
	}

	//----------------------------------------------
	// Public methods
	//----------------------------------------------

	LocalIdParsingErrorBuilder& LocalIdParsingErrorBuilder::addError( LocalIdParsingState state )
	{
		auto it = PREDEFINED_ERROR_MESSAGES.find( state );
		if ( it == PREDEFINED_ERROR_MESSAGES.end() )
		{
			auto stateIt = STATE_TO_STRING.find( state );
			std::string stateStr = ( stateIt != STATE_TO_STRING.end() )
									   ? stateIt->second
									   : std::to_string( static_cast<int>( state ) );

			throw std::runtime_error( "Couldn't find predefined message for: " + stateStr );
		}

		const std::string& message = it->second;
		m_errors.emplace_back( state, message );

		return *this;
	}

	LocalIdParsingErrorBuilder& LocalIdParsingErrorBuilder::addError( LocalIdParsingState state, const std::optional<std::string>& message )
	{
		if ( !message.has_value() || message->empty() || std::all_of( message->begin(), message->end(), []( char c ) { return std::isspace( c ); } ) )
		{
			return addError( state );
		}

		m_errors.emplace_back( state, *message );

		return *this;
	}

	bool LocalIdParsingErrorBuilder::hasError() const
	{
		return !m_errors.empty();
	}

	ParsingErrors LocalIdParsingErrorBuilder::build() const
	{
		if ( m_errors.empty() )
		{
			return ParsingErrors::Empty;
		}

		std::vector<ParsingErrors::ErrorEntry> errorEntries;
		errorEntries.reserve( m_errors.size() );

		for ( const auto& [state, message] : m_errors )
		{
			auto it = STATE_TO_STRING.find( state );
			std::string stateStr = ( it != STATE_TO_STRING.end() )
									   ? it->second
									   : "State" + std::to_string( static_cast<int>( state ) );

			if ( it == STATE_TO_STRING.end() )
			{
				SPDLOG_WARN( "Building ParsingErrors with unknown state enum value: {}", static_cast<int>( state ) );
			}

			errorEntries.emplace_back( stateStr, message );
		}

		return ParsingErrors{ errorEntries };
	}
}
