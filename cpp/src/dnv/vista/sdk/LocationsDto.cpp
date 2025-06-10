/**
 * @file LocationsDto.cpp
 * @brief Implementation of Data Transfer Objects for locations in the VIS standard
 */

#include "pch.h"

#include "dnv/vista/sdk/LocationsDto.h"

namespace dnv::vista::sdk
{
	namespace
	{
		//=====================================================================
		// Constants
		//=====================================================================

		static constexpr std::string_view CODE_KEY = "code";
		static constexpr std::string_view NAME_KEY = "name";
		static constexpr std::string_view DEFINITION_KEY = "definition";
		static constexpr std::string_view VIS_RELEASE_KEY = "visRelease";
		static constexpr std::string_view ITEMS_KEY = "items";

		//=====================================================================
		// Helper functions
		//=====================================================================

		static const std::string& internString( const std::string& value )
		{
			if ( value.size() <= 22 )
			{
				static std::unordered_map<std::string, std::string> cache;
				static std::mutex cacheMutex;

				std::lock_guard<std::mutex> lock( cacheMutex );
				auto [it, inserted] = cache.try_emplace( value, value );
				return it->second;
			}

			return value;
		}
	}

	//=====================================================================
	// Relative Location Data Transfer Objects
	//=====================================================================

	//----------------------------------------------
	// Construction / destruction
	//----------------------------------------------

	RelativeLocationsDto::RelativeLocationsDto( char code, std::string name, std::optional<std::string> definition )
		: m_code{ code },
		  m_name{ std::move( name ) },
		  m_definition{ std::move( definition ) }
	{
	}

	//----------------------------------------------
	// Serialization
	//----------------------------------------------

	std::optional<RelativeLocationsDto> RelativeLocationsDto::tryFromJson( const nlohmann::json& json )
	{
		try
		{
			if ( !json.is_object() )
			{
				return std::nullopt;
			}
			return std::optional<RelativeLocationsDto>{ json.get<RelativeLocationsDto>() };
		}
		catch ( ... )
		{
			return std::nullopt;
		}
	}

	RelativeLocationsDto RelativeLocationsDto::fromJson( const nlohmann::json& json )
	{
		try
		{
			return json.get<RelativeLocationsDto>();
		}
		catch ( const nlohmann::json::exception& ex )
		{
			throw std::invalid_argument( fmt::format( "Failed to deserialize RelativeLocationsDto from JSON: {}", ex.what() ) );
		}
		catch ( const std::exception& ex )
		{
			throw std::invalid_argument( fmt::format( "Failed to deserialize RelativeLocationsDto from JSON: {}", ex.what() ) );
		}
	}

	//----------------------------------------------
	// Private serialization methods
	//---------------------------------------------

	nlohmann::json RelativeLocationsDto::toJson() const
	{
		nlohmann::json j = *this;
		return j;
	}

	void to_json( nlohmann::json& j, const RelativeLocationsDto& dto )
	{
		j = nlohmann::json{
			{ CODE_KEY, std::string( 1, dto.m_code ) },
			{ NAME_KEY, dto.m_name } };
		if ( dto.m_definition.has_value() )
		{
			j[DEFINITION_KEY] = dto.m_definition.value();
		}
	}

	void from_json( const nlohmann::json& j, RelativeLocationsDto& dto )
	{
		if ( !j.contains( CODE_KEY ) )
		{
			throw nlohmann::json::parse_error::create( 101, 0u, "Missing 'code' field", nullptr );
		}
		if ( !j.at( CODE_KEY ).is_string() )
		{
			throw nlohmann::json::type_error::create( 302, "'code' field must be string", nullptr );
		}

		std::string codeStr = j.at( CODE_KEY ).get<std::string>();
		if ( codeStr.empty() || codeStr.length() != 1 )
		{
			throw nlohmann::json::type_error::create( 302, "'code' field must be single character", nullptr );
		}

		if ( !j.contains( NAME_KEY ) || !j.at( NAME_KEY ).is_string() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u, "Missing or invalid 'name' field", nullptr );
		}

		dto.m_code = codeStr[0];
		dto.m_name = internString( j.at( NAME_KEY ).get<std::string>() );

		if ( j.contains( DEFINITION_KEY ) && j.at( DEFINITION_KEY ).is_string() )
		{
			dto.m_definition = internString( j.at( DEFINITION_KEY ).get<std::string>() );
		}
		else
		{
			dto.m_definition.reset();
		}
	}

	//=====================================================================
	// Location Data Transfer Objects
	//=====================================================================

	//----------------------------------------------
	// Construction / destruction
	//----------------------------------------------

	LocationsDto::LocationsDto( std::string visVersion, std::vector<RelativeLocationsDto> items )
		: m_visVersion{ std::move( visVersion ) },
		  m_items{ std::move( items ) }
	{
	}

	//----------------------------------------------
	// Serialization
	//----------------------------------------------

	std::optional<LocationsDto> LocationsDto::tryFromJson( const nlohmann::json& json )
	{
		try
		{
			if ( !json.is_object() )
			{
				return std::nullopt;
			}
			return std::optional<LocationsDto>{ json.get<LocationsDto>() };
		}
		catch ( ... )
		{
			return std::nullopt;
		}
	}

	LocationsDto LocationsDto::fromJson( const nlohmann::json& json )
	{
		try
		{
			return json.get<LocationsDto>();
		}
		catch ( const nlohmann::json::exception& ex )
		{
			throw std::invalid_argument( fmt::format( "Failed to deserialize LocationsDto from JSON: {}", ex.what() ) );
		}
		catch ( const std::exception& ex )
		{
			throw std::invalid_argument( fmt::format( "Failed to deserialize LocationsDto from JSON: {}", ex.what() ) );
		}
	}

	nlohmann::json LocationsDto::toJson() const
	{
		return *this;
	}

	//----------------------------------------------
	// Private serialization methods
	//----------------------------------------------

	void to_json( nlohmann::json& j, const LocationsDto& dto )
	{
		j = nlohmann::json{
			{ VIS_RELEASE_KEY, dto.m_visVersion },
			{ ITEMS_KEY, dto.m_items } };
	}

	void from_json( const nlohmann::json& j, LocationsDto& dto )
	{
		dto.m_items.clear();

		if ( !j.contains( VIS_RELEASE_KEY ) || !j.at( VIS_RELEASE_KEY ).is_string() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u,
				fmt::format( "LocationsDto JSON missing required '{}' field", VIS_RELEASE_KEY ), nullptr );
		}
		if ( !j.contains( ITEMS_KEY ) || !j.at( ITEMS_KEY ).is_array() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u,
				fmt::format( "LocationsDto JSON missing required '{}' field", ITEMS_KEY ), nullptr );
		}

		dto.m_visVersion = internString( j.at( VIS_RELEASE_KEY ).get<std::string>() );

		const auto& jsonArray = j.at( ITEMS_KEY );
		dto.m_items.reserve( jsonArray.size() );

		for ( const auto& itemJson : jsonArray )
		{
			try
			{
				dto.m_items.emplace_back( itemJson.get<RelativeLocationsDto>() );
			}
			catch ( ... )
			{
			}
		}
	}
}
