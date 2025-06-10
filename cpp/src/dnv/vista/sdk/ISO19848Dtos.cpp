/**
 * @file ISO19848Dtos.cpp
 * @brief Implementation of ISO 19848 data transfer objects
 */

#include "pch.h"

#include "dnv/vista/sdk/ISO19848Dtos.h"

namespace dnv::vista::sdk
{
	namespace
	{
		//=====================================================================
		// Constants
		//=====================================================================

		static constexpr const char* VALUES_KEY = "values";
		static constexpr const char* TYPE_KEY = "type";
		static constexpr const char* DESCRIPTION_KEY = "description";

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
	// Single Data Channel Type Data Transfer Objects
	//=====================================================================

	//----------------------------------------------
	// Construction / destruction
	//----------------------------------------------

	DataChannelTypeNameDto::DataChannelTypeNameDto( std::string type, std::string description )
		: m_type{ std::move( type ) },
		  m_description{ std::move( description ) }
	{
	}

	//----------------------------------------------
	// Serialization
	//----------------------------------------------

	std::optional<DataChannelTypeNameDto> DataChannelTypeNameDto::tryFromJson( const nlohmann::json& json )
	{
		try
		{
			if ( !json.is_object() )
			{
				return std::nullopt;
			}
			return std::optional<DataChannelTypeNameDto>{ json.get<DataChannelTypeNameDto>() };
		}
		catch ( ... )
		{
			return std::nullopt;
		}
	}

	DataChannelTypeNameDto DataChannelTypeNameDto::fromJson( const nlohmann::json& json )
	{
		try
		{
			return json.get<DataChannelTypeNameDto>();
		}
		catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
		{
			throw std::invalid_argument( fmt::format( "Failed to deserialize DataChannelTypeNameDto from JSON: {}", ex.what() ) );
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			throw std::invalid_argument( fmt::format( "Failed to deserialize DataChannelTypeNameDto from JSON: {}", ex.what() ) );
		}
	}

	nlohmann::json DataChannelTypeNameDto::toJson() const
	{
		nlohmann::json j = *this;

		return j;
	}

	void to_json( nlohmann::json& j, const DataChannelTypeNameDto& dto )
	{
		j = nlohmann::json{
			{ TYPE_KEY, dto.m_type },
			{ DESCRIPTION_KEY, dto.m_description } };
	}

	//----------------------------------------------
	// Private serialization methods
	//----------------------------------------------

	void from_json( const nlohmann::json& j, DataChannelTypeNameDto& dto )
	{
		if ( !j.contains( TYPE_KEY ) || !j.at( TYPE_KEY ).is_string() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u, "Missing or invalid 'type' field", nullptr );
		}

		if ( j.contains( DESCRIPTION_KEY ) && !j.at( DESCRIPTION_KEY ).is_string() )
		{
			throw nlohmann::json::type_error::create( 302, "'description' field must be string", nullptr );
		}

		dto.m_type = internString( j.at( TYPE_KEY ).get<std::string>() );

		if ( j.contains( DESCRIPTION_KEY ) )
		{
			dto.m_description = internString( j.at( DESCRIPTION_KEY ).get<std::string>() );
		}
		else
		{
			dto.m_description.clear();
		}
	}

	//=====================================================================
	// Collection of Data Channel Type Data Transfer Objects
	//=====================================================================

	//----------------------------------------------
	// Construction / destruction
	//----------------------------------------------

	DataChannelTypeNamesDto::DataChannelTypeNamesDto( std::vector<DataChannelTypeNameDto> values )
		: m_values{ std::move( values ) }
	{
	}

	//----------------------------------------------
	// Serialization
	//----------------------------------------------

	std::optional<DataChannelTypeNamesDto> DataChannelTypeNamesDto::tryFromJson( const nlohmann::json& json )
	{
		try
		{
			if ( !json.is_object() )
			{
				return std::nullopt;
			}
			return std::optional<DataChannelTypeNamesDto>{ json.get<DataChannelTypeNamesDto>() };
		}
		catch ( ... )
		{
			return std::nullopt;
		}
	}

	DataChannelTypeNamesDto DataChannelTypeNamesDto::fromJson( const nlohmann::json& json )
	{
		try
		{
			return json.get<DataChannelTypeNamesDto>();
		}
		catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
		{
			throw std::invalid_argument( fmt::format( "Failed to deserialize DataChannelTypeNamesDto from JSON: {}", ex.what() ) );
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			throw std::invalid_argument( fmt::format( "Failed to deserialize DataChannelTypeNamesDto from JSON: {}", ex.what() ) );
		}
	}

	nlohmann::json DataChannelTypeNamesDto::toJson() const
	{
		return *this;
	}

	//----------------------------------------------
	// Private serialization methods
	//----------------------------------------------

	void to_json( nlohmann::json& j, const DataChannelTypeNamesDto& dto )
	{
		j = nlohmann::json{ { VALUES_KEY, dto.m_values } };
	}

	void from_json( const nlohmann::json& j, DataChannelTypeNamesDto& dto )
	{
		dto.m_values.clear();

		if ( !j.contains( VALUES_KEY ) || !j.at( VALUES_KEY ).is_array() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u,
				fmt::format( "DataChannelTypeNamesDto JSON missing required '{}' field", VALUES_KEY ), nullptr );
		}

		const auto& jsonArray = j.at( VALUES_KEY );
		dto.m_values.reserve( jsonArray.size() );

		for ( const auto& itemJson : jsonArray )
		{
			try
			{
				dto.m_values.emplace_back( itemJson.get<DataChannelTypeNameDto>() );
			}
			catch ( ... )
			{
			}
		}
	}

	//=====================================================================
	// Single Format Data Type Data Transfer Objects
	//=====================================================================

	//----------------------------------------------
	// Construction / destruction
	//----------------------------------------------

	FormatDataTypeDto::FormatDataTypeDto( std::string type, std::string description )
		: m_type{ std::move( type ) },
		  m_description{ std::move( description ) }
	{
	}

	//----------------------------------------------
	// Serialization
	//----------------------------------------------

	std::optional<FormatDataTypeDto> FormatDataTypeDto::tryFromJson( const nlohmann::json& json )
	{
		try
		{
			if ( !json.is_object() )
			{
				return std::nullopt;
			}
			return std::optional<FormatDataTypeDto>{ json.get<FormatDataTypeDto>() };
		}
		catch ( ... )
		{
			return std::nullopt;
		}
	}

	FormatDataTypeDto FormatDataTypeDto::fromJson( const nlohmann::json& json )
	{
		try
		{
			return json.get<FormatDataTypeDto>();
		}
		catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
		{
			throw std::invalid_argument( fmt::format( "Failed to deserialize FormatDataTypeDto from JSON: {}", ex.what() ) );
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			throw std::invalid_argument( fmt::format( "Failed to deserialize FormatDataTypeDto from JSON: {}", ex.what() ) );
		}
	}

	nlohmann::json FormatDataTypeDto::toJson() const
	{
		nlohmann::json j = *this;
		return j;
	}

	//----------------------------------------------
	// Private serialization methods
	//----------------------------------------------

	void to_json( nlohmann::json& j, const FormatDataTypeDto& dto )
	{
		j = nlohmann::json{
			{ TYPE_KEY, dto.m_type },
			{ DESCRIPTION_KEY, dto.m_description } };
	}

	void from_json( const nlohmann::json& j, FormatDataTypeDto& dto )
	{
		if ( !j.contains( TYPE_KEY ) || !j.at( TYPE_KEY ).is_string() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u,
				fmt::format( "FormatDataTypeDto JSON missing required '{}' field or not a string", TYPE_KEY ), nullptr );
		}
		if ( j.contains( DESCRIPTION_KEY ) && !j.at( DESCRIPTION_KEY ).is_string() )
		{
			throw nlohmann::json::type_error::create( 302,
				fmt::format( "FormatDataTypeDto JSON field '{}' is not a string", DESCRIPTION_KEY ), nullptr );
		}

		dto.m_type = internString( j.at( TYPE_KEY ).get<std::string>() );

		if ( j.contains( DESCRIPTION_KEY ) )
		{
			dto.m_description = internString( j.at( DESCRIPTION_KEY ).get<std::string>() );
		}
		else
		{
			dto.m_description.clear();
		}
	}

	//=====================================================================
	// Collection of Format Data Type Data Transfer Objects
	//=====================================================================

	//----------------------------------------------
	// Construction / destruction
	//----------------------------------------------

	FormatDataTypesDto::FormatDataTypesDto( std::vector<FormatDataTypeDto> values )
		: m_values{ std::move( values ) }
	{
	}

	//----------------------------------------------
	// Serialization
	//----------------------------------------------

	std::optional<FormatDataTypesDto> FormatDataTypesDto::tryFromJson( const nlohmann::json& json )
	{
		try
		{
			if ( !json.is_object() )
			{
				return std::nullopt;
			}
			return std::optional<FormatDataTypesDto>{ json.get<FormatDataTypesDto>() };
		}
		catch ( ... )
		{
			return std::nullopt;
		}
	}

	FormatDataTypesDto FormatDataTypesDto::fromJson( const nlohmann::json& json )
	{
		try
		{
			return json.get<FormatDataTypesDto>();
		}
		catch ( const nlohmann::json::exception& ex )
		{
			throw std::invalid_argument( fmt::format( "Failed to deserialize FormatDataTypesDto from JSON: {}", ex.what() ) );
		}
		catch ( const std::exception& ex )
		{
			throw std::invalid_argument( fmt::format( "Failed to deserialize FormatDataTypesDto from JSON: {}", ex.what() ) );
		}
	}

	nlohmann::json FormatDataTypesDto::toJson() const
	{
		return *this;
	}

	//----------------------------------------------
	// Private serialization methods
	//----------------------------------------------

	void to_json( nlohmann::json& j, const FormatDataTypesDto& dto )
	{
		j = nlohmann::json{ { VALUES_KEY, dto.m_values } };
	}

	void from_json( const nlohmann::json& j, FormatDataTypesDto& dto )
	{
		dto.m_values.clear();

		if ( !j.contains( VALUES_KEY ) || !j.at( VALUES_KEY ).is_array() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u,
				fmt::format( "FormatDataTypesDto JSON missing required '{}' field", VALUES_KEY ), nullptr );
		}

		const auto& jsonArray = j.at( VALUES_KEY );
		dto.m_values.reserve( jsonArray.size() );

		for ( const auto& itemJson : jsonArray )
		{
			try
			{
				dto.m_values.emplace_back( itemJson.get<FormatDataTypeDto>() );
			}
			catch ( ... )
			{
			}
		}
	}
}
