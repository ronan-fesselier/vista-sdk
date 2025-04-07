#include "pch.h"

#include "dnv/vista/sdk/Codebook.h"
#include "dnv/vista/sdk/MetadataTag.h"

#include "dnv/vista/sdk/VIS.h"

namespace dnv::vista::sdk
{
	PositionValidationResult PositionValidationResults::FromString( const std::string& name )
	{
		if ( name == "Valid" )
			return PositionValidationResult::Valid;
		else if ( name == "Invalid" )
			return PositionValidationResult::Invalid;
		else if ( name == "InvalidOrder" )
			return PositionValidationResult::InvalidOrder;
		else if ( name == "InvalidGrouping" )
			return PositionValidationResult::InvalidGrouping;
		else if ( name == "Custom" )
			return PositionValidationResult::Custom;
		else
			throw std::invalid_argument( "Unknown position validation result: " + name );
	}

	CodebookStandardValues::CodebookStandardValues( CodebookName name, const std::unordered_set<std::string>& standardValues )
		: m_name( name ), m_standardValues( standardValues )
	{
	}

	size_t CodebookStandardValues::Count() const
	{
		return m_standardValues.size();
	}

	bool CodebookStandardValues::Contains( const std::string& tagValue ) const
	{
		if ( m_name == CodebookName::Position )
		{
			try
			{
				auto val = std::stoi( tagValue );
				(void)val;
				return true;
			}
			catch ( const std::invalid_argument& )
			{
			}
			catch ( const std::out_of_range& )
			{
			}
		}

		return m_standardValues.find( tagValue ) != m_standardValues.end();
	}

	CodebookStandardValues::iterator CodebookStandardValues::begin() const
	{
		return m_standardValues.begin();
	}

	CodebookStandardValues::iterator CodebookStandardValues::end() const
	{
		return m_standardValues.end();
	}

	CodebookGroups::CodebookGroups( const std::unordered_set<std::string>& groups )
		: m_groups( groups )
	{
	}

	size_t CodebookGroups::Count() const
	{
		return m_groups.size();
	}

	bool CodebookGroups::Contains( const std::string& group ) const
	{
		return m_groups.find( group ) != m_groups.end();
	}

	CodebookGroups::iterator CodebookGroups::begin() const
	{
		return m_groups.begin();
	}

	CodebookGroups::iterator CodebookGroups::end() const
	{
		return m_groups.end();
	}

	Codebook::Codebook( const CodebookDto& dto )
	{
		if ( dto.name == "positions" )
			m_name = CodebookName::Position;
		else if ( dto.name == "calculations" )
			m_name = CodebookName::Calculation;
		else if ( dto.name == "quantities" )
			m_name = CodebookName::Quantity;
		else if ( dto.name == "states" )
			m_name = CodebookName::State;
		else if ( dto.name == "contents" )
			m_name = CodebookName::Content;
		else if ( dto.name == "commands" )
			m_name = CodebookName::Command;
		else if ( dto.name == "types" )
			m_name = CodebookName::Type;
		else if ( dto.name == "functional_services" )
			m_name = CodebookName::FunctionalServices;
		else if ( dto.name == "maintenance_category" )
			m_name = CodebookName::MaintenanceCategory;
		else if ( dto.name == "activity_type" )
			m_name = CodebookName::ActivityType;
		else if ( dto.name == "detail" )
			m_name = CodebookName::Detail;
		else
			throw std::invalid_argument( "Unknown metadata tag: " + dto.name );

		m_rawData = dto.values;

		std::vector<std::pair<std::string, std::string>> data;
		std::unordered_set<std::string> valueSet;
		std::unordered_set<std::string> groupSet;

		for ( const auto& [group, values] : dto.values )
		{
			std::string trimmedGroup = group;
			trimmedGroup.erase( 0, trimmedGroup.find_first_not_of( " \t\n\r\f\v" ) );
			trimmedGroup.erase( trimmedGroup.find_last_not_of( " \t\n\r\f\v" ) + 1 );

			for ( const auto& value : values )
			{
				std::string trimmedValue = value;
				trimmedValue.erase( 0, trimmedValue.find_first_not_of( " \t\n\r\f\v" ) );
				trimmedValue.erase( trimmedValue.find_last_not_of( " \t\n\r\f\v" ) + 1 );

				if ( trimmedValue != "<number>" )
				{
					data.push_back( { trimmedGroup, trimmedValue } );
					m_groupMap[trimmedValue] = trimmedGroup;
					valueSet.insert( trimmedValue );
					groupSet.insert( trimmedGroup );
				}
			}
		}

		m_standardValues = CodebookStandardValues( m_name, valueSet );
		m_groups = CodebookGroups( groupSet );
	}

	CodebookName Codebook::GetName() const
	{
		return m_name;
	}

	const CodebookGroups& Codebook::GetGroups() const
	{
		return m_groups;
	}

	const CodebookStandardValues& Codebook::GetStandardValues() const
	{
		return m_standardValues;
	}

	const std::unordered_map<std::string, std::vector<std::string>>& Codebook::GetRawData() const
	{
		return m_rawData;
	}

	bool Codebook::HasGroup( const std::string& group ) const
	{
		return m_groups.Contains( group );
	}

	bool Codebook::HasStandardValue( const std::string& value ) const
	{
		return m_standardValues.Contains( value );
	}

	std::optional<MetadataTag> Codebook::TryCreateTag( const std::string_view valueView ) const
	{
		if ( valueView.empty() )
			return std::nullopt;

		if ( std::all_of( valueView.begin(), valueView.end(), []( unsigned char c ) { return std::isspace( c ); } ) )
			return std::nullopt;

		std::string value{ valueView };
		bool isCustom = false;

		if ( m_name == CodebookName::Position )
		{
			auto positionValidity = ValidatePosition( value );
			if ( static_cast<int>( positionValidity ) < 100 )
				return std::nullopt;

			if ( positionValidity == PositionValidationResult::Custom )
				isCustom = true;
		}
		else
		{
			if ( !VIS::IsISOString( value ) )
				return std::nullopt;
			if ( m_name != CodebookName::Detail && !m_standardValues.Contains( value ) )
				isCustom = true;
		}

		return MetadataTag( m_name, value, isCustom );
	}

	MetadataTag Codebook::CreateTag( const std::string& value ) const
	{
		auto tag = TryCreateTag( value );
		if ( !tag.has_value() )
			throw std::invalid_argument( "Invalid value for metadata tag: codebook=" +
										 std::to_string( static_cast<int>( m_name ) ) + ", value=" + value );

		return tag.value();
	}

	PositionValidationResult Codebook::ValidatePosition( const std::string& position ) const
	{
		if ( position.empty() ||
			 std::all_of( position.begin(), position.end(), []( unsigned char c ) { return std::isspace( c ); } ) ||
			 !VIS::IsISOString( position ) )
		{
			return PositionValidationResult::Invalid;
		}

		std::string trimmedPosition = position;
		trimmedPosition.erase( 0, trimmedPosition.find_first_not_of( " \t\n\r\f\v" ) );
		trimmedPosition.erase( trimmedPosition.find_last_not_of( " \t\n\r\f\v" ) + 1 );
		if ( trimmedPosition.length() != position.length() )
			return PositionValidationResult::Invalid;

		if ( m_standardValues.Contains( position ) )
			return PositionValidationResult::Valid;

		try
		{
			auto val = std::stoi( position );
			(void)val;
			return PositionValidationResult::Valid;
		}
		catch ( const std::invalid_argument& )
		{
		}
		catch ( const std::out_of_range& )
		{
		}

		if ( position.find( '-' ) == std::string::npos )
			return PositionValidationResult::Custom;

		std::vector<std::string> positions;
		std::string temp;
		for ( char c : position )
		{
			if ( c == '-' )
			{
				positions.push_back( temp );
				temp.clear();
			}
			else
			{
				temp += c;
			}
		}
		positions.push_back( temp );

		std::vector<PositionValidationResult> validations;
		for ( const auto& positionStr : positions )
		{
			validations.push_back( ValidatePosition( positionStr ) );
		}

		if ( std::any_of( validations.begin(), validations.end(),
				 []( PositionValidationResult v ) { return static_cast<int>( v ) < 100; } ) )
		{
			return *std::max_element( validations.begin(), validations.end() );
		}

		bool numberNotAtEnd = false;
		for ( size_t i = 0; i < positions.size() - 1; ++i )
		{
			try
			{
				auto val = std::stoi( positions[i] );
				(void)val;

				numberNotAtEnd = true;
				break;
			}
			catch ( const std::invalid_argument& )
			{
			}
			catch ( const std::out_of_range& )
			{
			}
		}

		std::vector<std::string> positionsWithoutNumber;
		for ( const auto& p : positions )
		{
			try
			{
				auto val = std::stoi( p );
				(void)val;
			}
			catch ( const std::invalid_argument& )
			{
				positionsWithoutNumber.push_back( p );
			}
			catch ( const std::out_of_range& )
			{
				positionsWithoutNumber.push_back( p );
			}
		}

		std::vector<std::string> alphabeticallySorted = positionsWithoutNumber;
		std::sort( alphabeticallySorted.begin(), alphabeticallySorted.end() );
		bool notAlphabeticallySorted = positionsWithoutNumber != alphabeticallySorted;

		if ( numberNotAtEnd || notAlphabeticallySorted )
			return PositionValidationResult::InvalidOrder;

		bool allValid = std::all_of( validations.begin(), validations.end(),
			[]( PositionValidationResult v ) {
				return static_cast<int>( v ) == static_cast<int>( PositionValidationResult::Valid );
			} );

		if ( allValid )
		{
			std::vector<std::string> groups;
			for ( const auto& p : positions )
			{
				try
				{
					auto val = std::stoi( p );
					(void)val;

					groups.push_back( "<number>" );
				}
				catch ( const std::invalid_argument& )
				{
					auto it = m_groupMap.find( p );
					if ( it != m_groupMap.end() )
						groups.push_back( it->second );
					else
						groups.push_back( "UNKNOWN" );
				}
				catch ( const std::out_of_range& )
				{
					auto it = m_groupMap.find( p );
					if ( it != m_groupMap.end() )
						groups.push_back( it->second );
					else
						groups.push_back( "UNKNOWN" );
				}
			}

			std::unordered_set<std::string> groupsSet( groups.begin(), groups.end() );

			auto defaultGroupIt = std::find( groups.begin(), groups.end(), "DEFAULT_GROUP" );
			if ( defaultGroupIt == groups.end() && groupsSet.size() != groups.size() )
				return PositionValidationResult::InvalidGrouping;
		}

		return *std::max_element( validations.begin(), validations.end() );
	}
}
