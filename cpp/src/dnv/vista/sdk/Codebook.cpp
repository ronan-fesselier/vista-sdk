/**
 * @file Codebook.cpp
 * @brief Implementation of codebook-related components
 */

#include "pch.h"

#include "dnv/vista/sdk/Codebook.h"

#include "dnv/vista/sdk/MetadataTag.h"
#include "dnv/vista/sdk/VIS.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// Constants
	//=====================================================================

	namespace
	{
		constexpr const char* NUMBER_GROUP = "<number>";
		constexpr const char* DEFAULT_GROUP_NAME = "DEFAULT_GROUP";
		constexpr const char* UNKNOWN_GROUP = "UNKNOWN";

		constexpr const char* WHITESPACE = " \t\n\r\f\v";
	}

	//=====================================================================
	// Position Validation
	//=====================================================================

	//----------------------------------------------
	// Public static methods
	//----------------------------------------------

	PositionValidationResult PositionValidationResults::fromString( std::string_view name )
	{
		static const std::unordered_map<std::string, PositionValidationResult> nameMap{
			{ "Valid", PositionValidationResult::Valid },
			{ "Invalid", PositionValidationResult::Invalid },
			{ "InvalidOrder", PositionValidationResult::InvalidOrder },
			{ "InvalidGrouping", PositionValidationResult::InvalidGrouping },
			{ "Custom", PositionValidationResult::Custom } };

		auto it{ nameMap.find( std::string( name ) ) };
		if ( it != nameMap.end() )
		{
			return it->second;
		}

		throw std::invalid_argument( "Unknown position validation result: " + std::string( name ) );
	}

	//=====================================================================
	// Standard Values Container
	//=====================================================================

	//----------------------------------------------
	// Construction / destruction
	//----------------------------------------------

	CodebookStandardValues::CodebookStandardValues( CodebookName name, const std::unordered_set<std::string>& standardValues )
		: m_name{ name },
		  m_standardValues{ standardValues }
	{
	}

	//----------------------------------------------
	// Public Methods
	//----------------------------------------------

	size_t CodebookStandardValues::count() const
	{
		return m_standardValues.size();
	}

	bool CodebookStandardValues::contains( std::string_view tagValue ) const
	{
		if ( m_name == CodebookName::Position )
		{
			int parsedValue;
			auto result = std::from_chars( tagValue.data(), tagValue.data() + tagValue.size(), parsedValue );
			if ( result.ec == std::errc() && result.ptr == tagValue.data() + tagValue.size() )
			{
				return true;
			}
		}

		/* TODO: Performance  - Consider C++20 heterogeneous lookup with transparent hashers. */
		return m_standardValues.find( std::string( tagValue ) ) != m_standardValues.end();
	}

	//----------------------------------------------
	// Iteration
	//----------------------------------------------

	CodebookStandardValues::iterator CodebookStandardValues::begin() const
	{
		return m_standardValues.begin();
	}

	CodebookStandardValues::iterator CodebookStandardValues::end() const
	{
		return m_standardValues.end();
	}

	//=====================================================================
	// Groups Container
	//=====================================================================

	//----------------------------------------------
	// Construction / destruction
	//----------------------------------------------

	CodebookGroups::CodebookGroups( const std::unordered_set<std::string>& groups )
		: m_groups{ groups }
	{
	}

	//----------------------------------------------
	// Public Methods
	//----------------------------------------------

	size_t CodebookGroups::count() const
	{
		return m_groups.size();
	}

	bool CodebookGroups::contains( std::string_view group ) const
	{
		/* TODO: Performance  - Consider C++20 heterogeneous lookup with transparent hashers. */
		return m_groups.find( std::string( group ) ) != m_groups.end();
	}

	//----------------------------------------------
	// Iteration
	//----------------------------------------------

	CodebookGroups::Iterator CodebookGroups::begin() const
	{
		return m_groups.begin();
	}

	CodebookGroups::Iterator CodebookGroups::end() const
	{
		return m_groups.end();
	}

	//=====================================================================
	// Main Codebook Class
	//=====================================================================

	//----------------------------------------------
	// Construction / destruction
	//----------------------------------------------

	Codebook::Codebook( const CodebookDto& dto )
		: m_name{},
		  m_groupMap{},
		  m_standardValues{},
		  m_groups{},
		  m_rawData{}
	{
		static const std::unordered_map<std::string, CodebookName> nameMap{
			{ "positions", CodebookName::Position },
			{ "calculations", CodebookName::Calculation },
			{ "quantities", CodebookName::Quantity },
			{ "states", CodebookName::State },
			{ "contents", CodebookName::Content },
			{ "commands", CodebookName::Command },
			{ "types", CodebookName::Type },
			{ "functional_services", CodebookName::FunctionalServices },
			{ "maintenance_category", CodebookName::MaintenanceCategory },
			{ "activity_type", CodebookName::ActivityType },
			{ "detail", CodebookName::Detail } };

		auto it{ nameMap.find( std::string( dto.name() ) ) };
		if ( it == nameMap.end() )
		{
			const std::string errorMsg = "Unknown codebook name: " + std::string( dto.name() );
			throw std::invalid_argument( errorMsg );
		}
		m_name = it->second;

		std::unordered_set<std::string> valueSet{};
		std::unordered_set<std::string> groupSet{};

		m_rawData.reserve( dto.values().size() );

		size_t totalValueCount = std::accumulate(
			dto.values().begin(), dto.values().end(), static_cast<size_t>( 0 ),
			[]( size_t sum, const auto& pair ) {
				/* Count only non-<number> values for accurate sizing */
				return sum + std::count_if( pair.second.begin(), pair.second.end(),
								 []( const std::string& v ) { return v != NUMBER_GROUP; } );
			} );

		m_groupMap.reserve( totalValueCount );
		valueSet.reserve( totalValueCount );
		groupSet.reserve( dto.values().size() );

		/* Process each group and its values from the DTO */
		for ( const auto& [groupKey, values] : dto.values() )
		{
			/* Trim whitespace from group key */
			std::string_view groupKeyView( groupKey );
			const auto groupFirst = groupKeyView.find_first_not_of( WHITESPACE );
			std::string trimmedGroup;
			if ( groupFirst != std::string_view::npos )
			{
				const auto groupLast = groupKeyView.find_last_not_of( WHITESPACE );
				trimmedGroup = std::string( groupKeyView.substr( groupFirst, groupLast - groupFirst + 1 ) );
			}

			std::vector<std::string> trimmedValues;
			trimmedValues.reserve( values.size() );
			bool groupHasValidValue = false;

			/* Process and trim each value within the group */
			for ( const auto& value : values )
			{
				std::string_view valueView( value );
				const auto valueFirst = valueView.find_first_not_of( WHITESPACE );
				std::string trimmedValue;
				if ( valueFirst != std::string_view::npos )
				{
					const auto valueLast = valueView.find_last_not_of( WHITESPACE );
					trimmedValue = std::string( valueView.substr( valueFirst, valueLast - valueFirst + 1 ) );
				}

				trimmedValues.push_back( trimmedValue );

				/* Map non-<number> values to their group, handling duplicates */
				if ( trimmedValue != NUMBER_GROUP )
				{
					groupHasValidValue = true;

					valueSet.insert( trimmedValue );
				}
			}

			/* Store raw (but trimmed) group-value mapping */
			m_rawData.emplace( trimmedGroup, std::move( trimmedValues ) );

			/* Add group to the set only if it contained actual values */
			if ( groupHasValidValue )
			{
				groupSet.insert( trimmedGroup );
			}
		}

		m_standardValues = CodebookStandardValues{ m_name, valueSet };
		m_groups = CodebookGroups{ groupSet };
	}

	//----------------------------------------------
	// Public Methods
	//----------------------------------------------

	CodebookName Codebook::name() const
	{
		return m_name;
	}

	const CodebookGroups& Codebook::groups() const
	{
		return m_groups;
	}

	const CodebookStandardValues& Codebook::standardValues() const
	{
		return m_standardValues;
	}

	const std::unordered_map<std::string, std::vector<std::string>>& Codebook::rawData() const
	{
		return m_rawData;
	}

	bool Codebook::hasGroup( std::string_view group ) const
	{
		return m_groups.contains( group );
	}

	bool Codebook::hasStandardValue( std::string_view value ) const
	{
		return m_standardValues.contains( value );
	}

	std::optional<MetadataTag> Codebook::tryCreateTag( std::string_view valueView ) const
	{
		if ( valueView.empty() ||
			 std::all_of( valueView.begin(), valueView.end(), []( unsigned char c ) { return std::isspace( c ); } ) )
		{
			return std::nullopt;
		}

		bool isCustom = false;

		if ( m_name == CodebookName::Position )
		{
			PositionValidationResult positionValidity = validatePosition( valueView );

			if ( positionValidity < PositionValidationResult::Valid )
			{
				return std::nullopt;
			}

			if ( positionValidity == PositionValidationResult::Custom )
			{
				isCustom = true;
			}
		}
		else
		{
			if ( !VIS::isISOString( valueView ) )
			{
				return std::nullopt;
			}

			if ( m_name != CodebookName::Detail && !m_standardValues.contains( valueView ) )
			{
				isCustom = true;
			}
		}

		std::string value{ valueView };

		return MetadataTag( m_name, value, isCustom );
	}

	MetadataTag Codebook::createTag( const std::string& value ) const
	{
		auto tagOpt = tryCreateTag( value );
		if ( !tagOpt )
		{
			throw std::invalid_argument( "Invalid value for metadata tag: codebook=" + std::to_string( static_cast<int>( m_name ) ) + ", value=" + value );
		}
		return std::move( tagOpt.value() );
	}

	PositionValidationResult Codebook::validatePosition( std::string_view position ) const
	{
		if ( position.empty() ||
			 std::all_of( position.begin(), position.end(), []( unsigned char c ) { return std::isspace( c ); } ) )
		{
			return PositionValidationResult::Invalid;
		}

		if ( !VIS::isISOString( position ) )
		{
			return PositionValidationResult::Invalid;
		}

		size_t first_char = position.find_first_not_of( WHITESPACE );
		size_t last_char = position.find_last_not_of( WHITESPACE );

		if ( first_char == std::string_view::npos )
		{
			return PositionValidationResult::Invalid;
		}

		std::string_view trimmedView = position.substr( first_char, last_char - first_char + 1 );

		if ( trimmedView.length() != position.length() )
		{
			return PositionValidationResult::Invalid;
		}

		if ( m_standardValues.contains( trimmedView ) )
		{
			return PositionValidationResult::Valid;
		}

		int parsedValue;
		auto result = std::from_chars( trimmedView.data(), trimmedView.data() + trimmedView.size(), parsedValue );
		if ( result.ec == std::errc() && result.ptr == trimmedView.data() + trimmedView.size() )
		{
			return PositionValidationResult::Valid;
		}

		size_t hyphenPos = trimmedView.find( '-' );
		if ( hyphenPos == std::string_view::npos )
		{
			return PositionValidationResult::Custom;
		}

		/* Split the position string by hyphens */
		std::vector<std::string_view> parts{};
		size_t start = 0;
		while ( hyphenPos != std::string_view::npos )
		{
			parts.push_back( trimmedView.substr( start, hyphenPos - start ) );
			start = hyphenPos + 1;
			hyphenPos = trimmedView.find( '-', start );
		}
		parts.push_back( trimmedView.substr( start ) );

		/* Recursively validate each part */
		std::vector<PositionValidationResult> validations{};
		validations.reserve( parts.size() );
		PositionValidationResult worstResult = PositionValidationResult::Valid;

		for ( const auto& partView : parts )
		{
			PositionValidationResult partValidation = validatePosition( partView );
			validations.push_back( partValidation );

			if ( partValidation > worstResult )
			{
				worstResult = partValidation;
			}
		}

		if ( worstResult == PositionValidationResult::Invalid ||
			 worstResult == PositionValidationResult::InvalidOrder ||
			 worstResult == PositionValidationResult::InvalidGrouping )
		{
			return worstResult;
		}

		bool numberNotAtEnd{ false };
		std::vector<std::string_view> nonNumericParts{};
		nonNumericParts.reserve( parts.size() );

		for ( size_t i = 0; i < parts.size(); ++i )
		{
			int checkVal;
			auto checkResult = std::from_chars( parts[i].data(), parts[i].data() + parts[i].size(), checkVal );
			bool isNumber = ( checkResult.ec == std::errc() && checkResult.ptr == parts[i].data() + parts[i].size() );

			if ( isNumber )
			{
				if ( i < parts.size() - 1 )
				{
					numberNotAtEnd = true;
				}
			}
			else
			{
				nonNumericParts.push_back( parts[i] );
			}
		}

		bool notAlphabeticallySorted = false;
		if ( nonNumericParts.size() > 1 )
		{
			if ( !std::is_sorted( nonNumericParts.begin(), nonNumericParts.end() ) )
			{
				notAlphabeticallySorted = true;
			}
		}

		if ( numberNotAtEnd || notAlphabeticallySorted )
		{
			return PositionValidationResult::InvalidOrder;
		}

		bool allSubPartsValid = std::all_of( validations.begin(), validations.end(),
			[]( PositionValidationResult v ) {
				return v == PositionValidationResult::Valid;
			} );

		if ( allSubPartsValid )
		{
			std::vector<std::string> groups{};
			groups.reserve( parts.size() );
			std::unordered_set<std::string> uniqueGroups{};
			bool hasDefaultGroup = false;

			/* Determine the group for each part */
			for ( const auto& partView : parts )
			{
				int checkVal;
				auto checkResult = std::from_chars( partView.data(), partView.data() + partView.size(), checkVal );
				bool isNumber = ( checkResult.ec == std::errc() && checkResult.ptr == partView.data() + partView.size() );

				std::string groupName;
				if ( isNumber )
				{
					groupName = NUMBER_GROUP;
				}
				else
				{
					/* TODO: Performance - Creates temporary std::string for lookup */
					std::string partStrForLookup( partView );
					auto it = m_groupMap.find( partStrForLookup );
					groupName = ( it != m_groupMap.end() ) ? it->second : UNKNOWN_GROUP;
				}

				groups.push_back( groupName );
				uniqueGroups.insert( groupName );
				if ( groupName == DEFAULT_GROUP_NAME )
				{
					hasDefaultGroup = true;
				}
			}

			/* Grouping is invalid if multiple parts belong to the same non-DEFAULT group */
			if ( !hasDefaultGroup && uniqueGroups.size() != groups.size() )
			{
				return PositionValidationResult::InvalidGrouping;
			}
		}

		return worstResult;
	}
}
