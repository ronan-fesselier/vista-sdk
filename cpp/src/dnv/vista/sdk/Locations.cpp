#include "pch.h"

#include "dnv/vista/sdk/Locations.h"
#include "dnv/vista/sdk/ParsingErrors.h"
#include "dnv/vista/sdk/VisVersion.h"

namespace dnv::vista::sdk
{
	struct LocationParsingErrorBuilder
	{
		static LocationParsingErrorBuilder Empty;

		bool HasError = false;
		std::vector<std::pair<LocationValidationResult, std::string>> m_errors;

		static LocationParsingErrorBuilder Create()
		{
			LocationParsingErrorBuilder builder;
			builder.HasError = true;
			return builder;
		}

		void AddError( LocationValidationResult result, const std::string& message )
		{
			m_errors.emplace_back( result, message );
			HasError = true;
		}

		ParsingErrors Build() const
		{
			std::vector<ParsingErrors::ErrorEntry> convertedErrors;
			for ( const auto& [result, message] : m_errors )
			{
				convertedErrors.emplace_back( std::to_string( static_cast<int>( result ) ), message );
			}
			return ParsingErrors( convertedErrors );
		}
	};

	LocationParsingErrorBuilder LocationParsingErrorBuilder::Empty;

	Location::Location( const std::string& value )
		: m_value( value )
	{
	}

	const std::string& Location::GetValue() const
	{
		return m_value;
	}

	std::string Location::ToString() const
	{
		return m_value;
	}

	Location::operator std::string() const
	{
		return m_value;
	}

	bool Location::operator==( const Location& other ) const
	{
		return m_value == other.m_value;
	}

	bool Location::operator!=( const Location& other ) const
	{
		return !( *this == other );
	}

	RelativeLocation::RelativeLocation( char code, const std::string& name,
		const Location& location,
		const std::optional<std::string>& definition )
		: m_code( code ), m_name( name ), m_location( location ), m_definition( definition )
	{
	}

	char RelativeLocation::GetCode() const
	{
		return m_code;
	}

	const std::string& RelativeLocation::GetName() const
	{
		return m_name;
	}

	const std::optional<std::string>& RelativeLocation::GetDefinition() const
	{
		return m_definition;
	}

	const Location& RelativeLocation::GetLocation() const
	{
		return m_location;
	}

	size_t RelativeLocation::GetHashCode() const
	{
		return std::hash<char>{}( m_code );
	}

	bool RelativeLocation::operator==( const RelativeLocation& other ) const
	{
		return m_code == other.m_code;
	}

	bool RelativeLocation::operator!=( const RelativeLocation& other ) const
	{
		return !( *this == other );
	}

	LocationCharDict::LocationCharDict( size_t size )
		: m_table( size )
	{
	}

	std::optional<char>& LocationCharDict::operator[]( LocationGroup key )
	{
		int index = static_cast<int>( key ) - 1;
		if ( index >= m_table.size() )
		{
			SPDLOG_ERROR( "Index out of range for LocationCharDict: {}", index );
			throw std::runtime_error( "Unsupported code: " + std::to_string( static_cast<int>( key ) ) );
		}
		return m_table[index];
	}

	bool LocationCharDict::TryAdd( LocationGroup key, char value, std::optional<char>& existingValue )
	{
		auto& v = ( *this )[key];
		if ( v.has_value() )
		{
			existingValue = v;
			return false;
		}

		existingValue = std::nullopt;
		v = value;
		return true;
	}

	Locations::Locations( VisVersion version, const LocationsDto& dto )
		: m_visVersion( version )
	{
		for ( const auto& item : dto.items )
		{
			m_locationCodes.push_back( item.code );
		}

		for ( const auto& relLocDto : dto.items )
		{
			Location loc( std::string( 1, relLocDto.code ) );

			RelativeLocation relLoc(
				relLocDto.code,
				relLocDto.name,
				loc,
				relLocDto.definition );

			m_relativeLocations.push_back( relLoc );

			if ( relLocDto.code == 'H' || relLocDto.code == 'V' )
				continue;

			LocationGroup key;
			if ( relLocDto.code == 'N' )
				key = LocationGroup::Number;
			else if ( relLocDto.code == 'P' || relLocDto.code == 'C' || relLocDto.code == 'S' )
				key = LocationGroup::Side;
			else if ( relLocDto.code == 'U' || relLocDto.code == 'M' || relLocDto.code == 'L' )
				key = LocationGroup::Vertical;
			else if ( relLocDto.code == 'I' || relLocDto.code == 'O' )
				key = LocationGroup::Transverse;
			else if ( relLocDto.code == 'F' || relLocDto.code == 'A' )
				key = LocationGroup::Longitudinal;
			else
			{
				SPDLOG_ERROR( "Unsupported code: {}", relLocDto.code );
				throw std::runtime_error( std::string( "Unsupported code: " ) + relLocDto.code );
			}

			if ( m_groups.find( key ) == m_groups.end() )
				m_groups[key] = std::vector<RelativeLocation>();

			if ( key == LocationGroup::Number )
				continue;

			m_reversedGroups[relLocDto.code] = key;
			m_groups[key].push_back( relLoc );
		}
	}

	VisVersion Locations::GetVisVersion() const
	{
		return m_visVersion;
	}

	const std::vector<RelativeLocation>& Locations::GetRelativeLocations() const
	{
		return m_relativeLocations;
	}

	const std::unordered_map<LocationGroup, std::vector<RelativeLocation>>& Locations::GetGroups() const
	{
		return m_groups;
	}

	Location Locations::Parse( const std::string& locationStr )
	{
		auto errorBuilder = LocationParsingErrorBuilder::Empty;

		Location location;
		if ( !TryParseInternal( locationStr, locationStr, location, errorBuilder ) )
		{
			SPDLOG_ERROR( "Invalid value for location: {}", locationStr );
			throw std::invalid_argument( "Invalid value for location: " + locationStr +
										 ", errors: " + errorBuilder.Build().ToString() );
		}

		return location;
	}

	Location Locations::Parse( std::string_view locationStr )
	{
		auto errorBuilder = LocationParsingErrorBuilder::Empty;

		Location location;
		if ( !TryParseInternal( locationStr, std::nullopt, location, errorBuilder ) )
		{
			SPDLOG_ERROR( "Invalid value for location: {}", locationStr );
			throw std::invalid_argument( "Invalid value for location: " + std::string( locationStr ) +
										 ", errors: " + errorBuilder.Build().ToString() );
		}

		return location;
	}

	bool Locations::TryParse( const std::optional<std::string>& value, Location& location ) const
	{
		auto errorBuilder = LocationParsingErrorBuilder::Empty;
		std::string_view view = value.has_value() ? std::string_view( *value ) : std::string_view();
		return TryParseInternal( view, value, location, errorBuilder );
	}

	bool Locations::TryParse( const std::optional<std::string>& value, Location& location, ParsingErrors& errors )
	{
		auto errorBuilder = LocationParsingErrorBuilder::Empty;
		std::string_view view = value.has_value() ? std::string_view( *value ) : std::string_view();
		bool result = TryParseInternal( view, value, location, errorBuilder );
		errors = errorBuilder.Build();
		return result;
	}

	bool Locations::TryParse( std::string_view value, Location& location )
	{
		auto errorBuilder = LocationParsingErrorBuilder::Empty;
		return TryParseInternal( value, std::nullopt, location, errorBuilder );
	}

	bool Locations::TryParse( std::string_view value, Location& location, ParsingErrors& errors )
	{
		auto errorBuilder = LocationParsingErrorBuilder::Empty;
		bool result = TryParseInternal( value, std::nullopt, location, errorBuilder );
		errors = errorBuilder.Build();
		return result;
	}

	void Locations::AddError( LocationParsingErrorBuilder& errorBuilder,
		LocationValidationResult name,
		const std::string& message )
	{
		if ( !errorBuilder.HasError )
			errorBuilder = LocationParsingErrorBuilder::Create();
		errorBuilder.AddError( name, message );
	}

	bool Locations::TryParseInternal( std::string_view span,
		const std::optional<std::string>& originalStr,
		Location& location,
		LocationParsingErrorBuilder& errorBuilder ) const
	{
		if ( span.empty() )
		{
			AddError( errorBuilder, LocationValidationResult::NullOrWhiteSpace,
				"Invalid location: contains only whitespace" );
			return false;
		}

		bool isWhitespace = std::all_of( span.begin(), span.end(),
			[]( unsigned char c ) { return std::isspace( c ); } );

		if ( isWhitespace )
		{
			AddError( errorBuilder, LocationValidationResult::NullOrWhiteSpace,
				"Invalid location: contains only whitespace" );
			return false;
		}

		std::optional<size_t> prevDigitIndex;
		std::optional<size_t> digitStartIndex;
		std::optional<size_t> charsStartIndex;
		std::optional<int> number;

		LocationCharDict charDict( 4 );

		for ( size_t i = 0; i < span.size(); i++ )
		{
			const char ch = span[i];

			if ( std::isdigit( ch ) )
			{
				if ( !digitStartIndex.has_value() && i != 0 )
				{
					std::string errorMsg = "Invalid location: numeric location should start before location code(s) in location: '";
					errorMsg += originalStr.value_or( std::string( span ) );
					errorMsg += "'";

					AddError( errorBuilder, LocationValidationResult::Invalid, errorMsg );
					return false;
				}

				if ( prevDigitIndex.has_value() && prevDigitIndex.value() != ( i - 1 ) )
				{
					std::string errorMsg = "Invalid location: cannot have multiple separated digits in location: '";
					errorMsg += originalStr.value_or( std::string( span ) );
					errorMsg += "'";

					AddError( errorBuilder, LocationValidationResult::Invalid, errorMsg );
					return false;
				}

				if ( !digitStartIndex.has_value() )
				{
					number = ch - '0';
					digitStartIndex = i;
				}
				else
				{
					int num;
					if ( !TryParseInt( span, static_cast<int>( digitStartIndex.value() ),
							 static_cast<int>( i - digitStartIndex.value() ) + 1, num ) )
					{
						std::string errorMsg = "Invalid location: failed to parse numeric location: '";
						errorMsg += originalStr.value_or( std::string( span ) );
						errorMsg += "'";

						AddError( errorBuilder, LocationValidationResult::Invalid, errorMsg );
						return false;
					}
					number = num;
				}

				prevDigitIndex = i;
			}
			else
			{
				if ( m_reversedGroups.find( ch ) == m_reversedGroups.end() )
				{
					std::string invalidChars;
					std::string spanStr = std::string( span );
					bool first = true;

					for ( char c : spanStr )
					{
						if ( !std::isdigit( c ) && ( c == 'N' || std::find( m_locationCodes.begin(), m_locationCodes.end(), c ) == m_locationCodes.end() ) )
						{
							if ( !first )
								invalidChars += ",";
							invalidChars += "'";
							invalidChars += c;
							invalidChars += "'";
							first = false;
						}
					}

					std::string errorMsg = "Invalid location code: '";
					errorMsg += originalStr.value_or( std::string( span ) );
					errorMsg += "' with invalid location code(s): ";
					errorMsg += invalidChars;

					AddError( errorBuilder, LocationValidationResult::InvalidCode, errorMsg );
					return false;
				}

				LocationGroup group = m_reversedGroups.at( ch );
				std::optional<char> existingCh;

				if ( !charDict.TryAdd( group, ch, existingCh ) )
				{
					std::string errorMsg = "Invalid location: Multiple '";

					switch ( group )
					{
						case LocationGroup::Number:
							errorMsg += "Number";
							break;
						case LocationGroup::Side:
							errorMsg += "Side";
							break;
						case LocationGroup::Vertical:
							errorMsg += "Vertical";
							break;
						case LocationGroup::Transverse:
							errorMsg += "Transverse";
							break;
						case LocationGroup::Longitudinal:
							errorMsg += "Longitudinal";
							break;
					}

					errorMsg += "' values. Got both '";
					errorMsg += existingCh.value();
					errorMsg += "' and '";
					errorMsg += ch;
					errorMsg += "' in '";
					errorMsg += originalStr.value_or( std::string( span ) );
					errorMsg += "'";

					AddError( errorBuilder, LocationValidationResult::Invalid, errorMsg );
					return false;
				}

				if ( !charsStartIndex.has_value() )
				{
					charsStartIndex = i;
				}
				else if ( i > 0 )
				{
					char prevCh = span[i - 1];
					if ( ch < prevCh )
					{
						std::string errorMsg = "Invalid location: '";
						errorMsg += originalStr.value_or( std::string( span ) );
						errorMsg += "' not alphabetically sorted";

						AddError( errorBuilder, LocationValidationResult::InvalidOrder, errorMsg );
						return false;
					}
				}
			}
		}

		location = Location( originalStr.value_or( std::string( span ) ) );
		return true;
	}

	bool Locations::TryParseInt( std::string_view span, int start, int length, int& number )
	{
		if ( start < 0 || length <= 0 || start + length > span.size() )
			return false;

		std::string_view slice = span.substr( start, length );

		const char* begin = slice.data();
		const char* end = begin + slice.size();

		auto result = std::from_chars( begin, end, number );
		return result.ec == std::errc();
	}
}
