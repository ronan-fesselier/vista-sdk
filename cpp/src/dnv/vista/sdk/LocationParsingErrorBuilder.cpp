/**
 * @file LocationParsingErrorBuilder.cpp
 * @brief Implementation of the LocationParsingErrorBuilder class.
 */

#include "pch.h"

#include "dnv/vista/sdk/LocationParsingErrorBuilder.h"

#include "dnv/vista/sdk/ParsingErrors.h"

namespace dnv::vista::sdk
{
	namespace
	{
		//=====================================================================
		// Static data maps
		//=====================================================================

		static const std::unordered_map<LocationValidationResult, std::string> VALIDATION_RESULT_TO_STRING = {
			{ LocationValidationResult::Invalid, "Invalid" },
			{ LocationValidationResult::InvalidCode, "InvalidCode" },
			{ LocationValidationResult::InvalidOrder, "InvalidOrder" },
			{ LocationValidationResult::NullOrWhiteSpace, "NullOrWhiteSpace" },
			{ LocationValidationResult::Valid, "Valid" } };
	}

	//=====================================================================
	// LocationParsingErrorBuilder class
	//=====================================================================

	//----------------------------------------------
	// Static factory method
	//----------------------------------------------

	LocationParsingErrorBuilder LocationParsingErrorBuilder::create()
	{
		return LocationParsingErrorBuilder();
	}

	//----------------------------------------------
	// Public methods
	//----------------------------------------------

	LocationParsingErrorBuilder& LocationParsingErrorBuilder::addError( LocationValidationResult validationResult, const std::optional<std::string>& message )
	{
		std::string actualMessage = message.has_value() ? *message : "";

		SPDLOG_ERROR( "Adding location parsing error for validation result {}: {}", static_cast<int>( validationResult ), actualMessage );

		m_errors.emplace_back( validationResult, actualMessage );

		return *this;
	}

	bool LocationParsingErrorBuilder::hasError() const
	{
		return !m_errors.empty();
	}

	ParsingErrors LocationParsingErrorBuilder::build() const
	{
		if ( m_errors.empty() )
		{
			return ParsingErrors::Empty;
		}

		std::vector<ParsingErrors::ErrorEntry> errorEntries;
		errorEntries.reserve( m_errors.size() );

		for ( const auto& [validationResult, message] : m_errors )
		{
			auto it = VALIDATION_RESULT_TO_STRING.find( validationResult );
			std::string resultStr = ( it != VALIDATION_RESULT_TO_STRING.end() )
										? it->second
										: "ValidationResult" + std::to_string( static_cast<int>( validationResult ) );

			if ( it == VALIDATION_RESULT_TO_STRING.end() )
			{
				SPDLOG_WARN( "Building ParsingErrors with unknown validation result enum value: {}", static_cast<int>( validationResult ) );
			}

			errorEntries.emplace_back( resultStr, message );
		}

		return ParsingErrors{ errorEntries };
	}
}
