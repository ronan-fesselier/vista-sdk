/**
 * @file LocationsDto.cpp
 * @brief Implementation of data transfer objects for locations in the VIS standard
 */

#include "pch.h"

#include "dnv/vista/sdk/LocationsDto.h"

#include "dnv/vista/sdk/config/DtoKeys.h"

namespace dnv::vista::sdk
{
	namespace
	{
		//=====================================================================
		// JSON parsing helper functions
		//=====================================================================

		std::string_view extractCodeHint( const nlohmann::json& json ) noexcept
		{
			try
			{
				if ( json.contains( dto::LOCATIONS_DTO_KEY_CODE ) && json.at( dto::LOCATIONS_DTO_KEY_CODE ).is_string() )
				{
					const auto& str = json.at( dto::LOCATIONS_DTO_KEY_CODE ).get_ref<const std::string&>();
					return std::string_view{ str };
				}

				return dto::LOCATIONS_DTO_UNKNOWN_CODE;
			}
			catch ( ... )
			{
				return dto::LOCATIONS_DTO_UNKNOWN_CODE;
			}
		}

		std::string_view extractVisHint( const nlohmann::json& json ) noexcept
		{
			try
			{
				if ( json.contains( dto::LOCATIONS_DTO_KEY_VIS_RELEASE ) && json.at( dto::LOCATIONS_DTO_KEY_VIS_RELEASE ).is_string() )
				{
					const auto& str = json.at( dto::LOCATIONS_DTO_KEY_VIS_RELEASE ).get_ref<const std::string&>();
					return std::string_view{ str };
				}

				return dto::LOCATIONS_DTO_UNKNOWN_VERSION;
			}
			catch ( ... )
			{
				return dto::LOCATIONS_DTO_UNKNOWN_VERSION;
			}
		}
	}

	//=====================================================================
	// Relative Location data transfer objects
	//=====================================================================

	//----------------------------------------------
	// Serialization
	//----------------------------------------------

	std::optional<RelativeLocationsDto> RelativeLocationsDto::tryFromJson( const nlohmann::json& json )
	{
		[[maybe_unused]] const auto codeHint = extractCodeHint( json );

		try
		{
			if ( !json.is_object() )
			{
				fmt::print( stderr, "ERROR: JSON value for RelativeLocationsDto is not an object\n" );
				return std::nullopt;
			}

			if ( !json.contains( dto::LOCATIONS_DTO_KEY_CODE ) || !json.at( dto::LOCATIONS_DTO_KEY_CODE ).is_string() )
			{
				fmt::print( stderr,
					"ERROR: RelativeLocationsDto JSON missing required '{}' field or not a string\n",
					dto::LOCATIONS_DTO_KEY_CODE );

				return std::nullopt;
			}
			if ( !json.contains( dto::LOCATIONS_DTO_KEY_NAME ) || !json.at( dto::LOCATIONS_DTO_KEY_NAME ).is_string() )
			{
				fmt::print( stderr,
					"ERROR: RelativeLocationsDto JSON missing required '{}' field or not a string\n",
					dto::LOCATIONS_DTO_KEY_NAME );

				return std::nullopt;
			}

			std::string codeStr = json.at( dto::LOCATIONS_DTO_KEY_CODE ).get<std::string>();
			if ( codeStr.empty() || codeStr.length() != 1 )
			{
				fmt::print( stderr, "ERROR: RelativeLocationsDto (hint: code='{}') has invalid code format\n", codeHint );

				return std::nullopt;
			}

			std::string tempName = json.at( dto::LOCATIONS_DTO_KEY_NAME ).get<std::string>();
			if ( tempName.empty() )
			{
				fmt::print( stderr, "WARN: Empty name field found in RelativeLocationsDto code='{}'\n", codeStr );
			}

			char tempCode = codeStr[0];
			std::optional<std::string> tempDefinition = std::nullopt;

			if ( json.contains( dto::LOCATIONS_DTO_KEY_DEFINITION ) )
			{
				if ( json.at( dto::LOCATIONS_DTO_KEY_DEFINITION ).is_string() )
				{
					tempDefinition = json.at( dto::LOCATIONS_DTO_KEY_DEFINITION ).get<std::string>();
				}
				else if ( !json.at( dto::LOCATIONS_DTO_KEY_DEFINITION ).is_null() )
				{
					fmt::print( stderr, "WARN: RelativeLocationsDto code='{}' has non-string definition field\n", codeStr );
				}
			}

			RelativeLocationsDto result(
				std::move( tempCode ),
				std::move( tempName ),
				std::move( tempDefinition ) );

			return result;
		}
		catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
		{
			fmt::print( stderr, "ERROR: JSON exception during RelativeLocationsDto parsing (hint: code='{}'): {}\n",
				codeHint, ex.what() );

			return std::nullopt;
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			fmt::print( stderr, "ERROR: Standard exception during RelativeLocationsDto parsing (hint: code='{}'): {}\n",
				codeHint, ex.what() );

			return std::nullopt;
		}
	}

	RelativeLocationsDto RelativeLocationsDto::fromJson( const nlohmann::json& json )
	{
		auto dtoOpt = RelativeLocationsDto::tryFromJson( json );
		if ( !dtoOpt.has_value() )
		{
			throw std::invalid_argument( "Failed to deserialize RelativeLocationsDto from JSON" );
		}

		return std::move( dtoOpt ).value();
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
			{ dto::LOCATIONS_DTO_KEY_CODE, std::string{ 1, dto.m_code } },
			{ dto::LOCATIONS_DTO_KEY_NAME, dto.m_name } };

		if ( dto.m_definition.has_value() )
		{
			j[dto::LOCATIONS_DTO_KEY_DEFINITION] = dto.m_definition.value();
		}
	}

	void from_json( const nlohmann::json& j, RelativeLocationsDto& dto )
	{
		const auto codeIt = j.find( dto::LOCATIONS_DTO_KEY_CODE );
		const auto nameIt = j.find( dto::LOCATIONS_DTO_KEY_NAME );
		const auto defIt = j.find( dto::LOCATIONS_DTO_KEY_DEFINITION );

		if ( codeIt == j.end() || !codeIt->is_string() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u, "Missing or invalid 'code' field", nullptr );
		}
		if ( nameIt == j.end() || !nameIt->is_string() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u, "Missing or invalid 'name' field", nullptr );
		}

		std::string codeStr = codeIt->get<std::string>();
		if ( codeStr.empty() || codeStr.length() != 1 )
		{
			throw nlohmann::json::type_error::create( 302, "'code' field must be single character", nullptr );
		}

		dto.m_code = codeStr[0];
		dto.m_name = ( nameIt->get<std::string>() );
		if ( dto.m_name.empty() )
		{
			fmt::print( stderr, "WARN: Empty name field found in RelativeLocationsDto code='{}'\n", dto.m_code );
		}

		if ( defIt != j.end() && defIt->is_string() )
		{
			dto.m_definition = ( defIt->get<std::string>() );

			if ( dto.m_definition.has_value() && dto.m_definition->empty() )
			{
				fmt::print( stderr, "WARN: Empty definition field found in RelativeLocationsDto code='{}'\n", dto.m_code );
			}
		}
		else
		{
			dto.m_definition.reset();
		}
	}

	//=====================================================================
	// Location data transfer objects
	//=====================================================================

	//----------------------------------------------
	// Serialization
	//----------------------------------------------

	std::optional<LocationsDto> LocationsDto::tryFromJson( const nlohmann::json& json )
	{
		[[maybe_unused]] const auto visHint = extractVisHint( json );

		try
		{
			if ( !json.is_object() )
			{
				fmt::print( stderr, "ERROR: JSON value for LocationsDto is not an object\n" );

				return std::nullopt;
			}

			if ( !json.contains( dto::LOCATIONS_DTO_KEY_VIS_RELEASE ) || !json.at( dto::LOCATIONS_DTO_KEY_VIS_RELEASE ).is_string() )
			{
				fmt::print(
					stderr,
					"ERROR: LocationsDto JSON missing required '{}' field or not a string\n",
					dto::LOCATIONS_DTO_KEY_VIS_RELEASE );

				return std::nullopt;
			}
			if ( !json.contains( dto::LOCATIONS_DTO_KEY_ITEMS ) || !json.at( dto::LOCATIONS_DTO_KEY_ITEMS ).is_array() )
			{
				fmt::print( stderr,
					"ERROR: LocationsDto JSON missing required '{}' array\n",
					dto::LOCATIONS_DTO_KEY_ITEMS );

				return std::nullopt;
			}

			LocationsDto dto = json.get<LocationsDto>();

			return std::optional<LocationsDto>{ std::move( dto ) };
		}
		catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
		{
			fmt::print( stderr, "ERROR: JSON exception during LocationsDto parsing (hint: visRelease='{}'): {}\n",
				visHint, ex.what() );

			return std::nullopt;
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			fmt::print( stderr, "ERROR: Standard exception during LocationsDto parsing (hint: visRelease='{}'): {}\n",
				visHint, ex.what() );

			return std::nullopt;
		}
	}

	LocationsDto LocationsDto::fromJson( const nlohmann::json& json )
	{
		auto dtoOpt = LocationsDto::tryFromJson( json );
		if ( !dtoOpt.has_value() )
		{
			throw std::invalid_argument( "Failed to deserialize LocationsDto from JSON" );
		}

		return std::move( dtoOpt ).value();
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
			{ dto::LOCATIONS_DTO_KEY_VIS_RELEASE, dto.m_visVersion },
			{ dto::LOCATIONS_DTO_KEY_ITEMS, dto.m_items } };
	}

	void from_json( const nlohmann::json& j, LocationsDto& dto )
	{
		dto.m_items.clear();

		const auto visIt = j.find( dto::LOCATIONS_DTO_KEY_VIS_RELEASE );
		const auto itemsIt = j.find( dto::LOCATIONS_DTO_KEY_ITEMS );

		if ( visIt == j.end() || !visIt->is_string() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u,
				fmt::format( "LocationsDto JSON missing required '{}' field", dto::LOCATIONS_DTO_KEY_VIS_RELEASE ), nullptr );
		}
		if ( itemsIt == j.end() || !itemsIt->is_array() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u,
				fmt::format( "LocationsDto JSON missing required '{}' field", dto::LOCATIONS_DTO_KEY_ITEMS ), nullptr );
		}

		dto.m_visVersion = visIt->get<std::string>();

		if ( dto.m_visVersion.empty() )
		{
			fmt::print( stderr, "WARN: Empty visVersion field found in LocationsDto\n" );
		}

		const auto& jsonArray = *itemsIt;
		const size_t totalItems = jsonArray.size();
		size_t successCount = 0;

		if ( totalItems > 10000 )
		{
			[[maybe_unused]] const size_t approxMemoryUsage = ( totalItems * sizeof( RelativeLocationsDto ) ) / ( 1024 * 1024 );
		}

		const size_t reserveSize = totalItems < 1000
									   ? totalItems + totalItems / 4
									   : totalItems + totalItems / 16;

		dto.m_items.reserve( reserveSize );

		for ( const auto& itemJson : jsonArray )
		{
			auto itemOpt = RelativeLocationsDto::tryFromJson( itemJson );
			if ( itemOpt.has_value() )
			{
				dto.m_items.emplace_back( std::move( *itemOpt ) );
				successCount++;
			}
			else
			{
				fmt::print( stderr, "WARN: Skipping invalid RelativeLocationsDto item during parsing\n" );
			}
		}

		if ( totalItems > 0 && successCount < totalItems * 9 / 10 )
		{
			if ( dto.m_items.capacity() > dto.m_items.size() * 4 / 3 )
			{
				dto.m_items.shrink_to_fit();
			}
		}
	}
}
