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
		// Helper Functions
		//=====================================================================

		static const std::string& internString( const std::string& value )
		{
			static std::unordered_map<std::string, std::string> cache;
			static size_t hits = 0, misses = 0, calls = 0;
			calls++;

			if ( value.size() > 22 ) // Common SSO threshold
			{
				auto it = cache.find( value );
				if ( it != cache.end() )
				{
					hits++;
					if ( calls % 10000 == 0 )
					{
					}
					return it->second;
				}

				misses++;
				return cache.emplace( value, value ).first->first;
			}

			return value;
		}

		template <typename T>
		size_t estimateMemoryUsage( const std::vector<T>& collection )
		{
			return sizeof( std::vector<T> ) + collection.capacity() * sizeof( T );
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
	// Accessors
	//----------------------------------------------

	const std::string& DataChannelTypeNameDto::type() const
	{
		return m_type;
	}

	const std::string& DataChannelTypeNameDto::description() const
	{
		return m_description;
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
				SPDLOG_ERROR( "JSON value for DataChannelTypeNameDto is not an object" );
				return std::nullopt;
			}

			DataChannelTypeNameDto dto = json.get<DataChannelTypeNameDto>();

			return std::optional<DataChannelTypeNameDto>{ std::move( dto ) };
		}
		catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
		{
			SPDLOG_ERROR( "nlohmann::json exception during DataChannelTypeNameDto parsing: {}", ex.what() );
			return std::nullopt;
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			SPDLOG_ERROR( "Standard exception during DataChannelTypeNameDto parsing: {}", ex.what() );
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
			std::string errorMsg = fmt::format( "Failed to deserialize DataChannelTypeNameDto from JSON: {}", ex.what() );
			SPDLOG_ERROR( errorMsg );
			throw std::invalid_argument( errorMsg );
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			std::string errorMsg = fmt::format( "Failed to deserialize DataChannelTypeNameDto from JSON: {}", ex.what() );
			SPDLOG_ERROR( errorMsg );
			throw std::invalid_argument( errorMsg );
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
	// Private Serialization Methods
	//----------------------------------------------

	void from_json( const nlohmann::json& j, DataChannelTypeNameDto& dto )
	{
		if ( !j.contains( TYPE_KEY ) || !j.at( TYPE_KEY ).is_string() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u, fmt::format( "DataChannelTypeNameDto JSON missing required '{}' field or not a string", TYPE_KEY ), nullptr );
		}
		if ( j.contains( DESCRIPTION_KEY ) && !j.at( DESCRIPTION_KEY ).is_string() )
		{
			throw nlohmann::json::type_error::create( 302, fmt::format( "DataChannelTypeNameDto JSON field '{}' is not a string", DESCRIPTION_KEY ), nullptr );
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

		if ( dto.m_type.empty() )
		{
			SPDLOG_WARN( "Parsed DataChannelTypeNameDto has empty type field" );
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
	// Accessors
	//----------------------------------------------

	const std::vector<DataChannelTypeNameDto>& DataChannelTypeNamesDto::values() const
	{
		return m_values;
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
				SPDLOG_ERROR( "JSON value for DataChannelTypeNamesDto is not an object" );
				return std::nullopt;
			}

			DataChannelTypeNamesDto dto = json.get<DataChannelTypeNamesDto>();

			return std::optional<DataChannelTypeNamesDto>{ std::move( dto ) };
		}
		catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
		{
			SPDLOG_ERROR( "nlohmann::json exception during DataChannelTypeNamesDto parsing: {}", ex.what() );
			return std::nullopt;
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			SPDLOG_ERROR( "Standard exception during DataChannelTypeNamesDto parsing: {}", ex.what() );
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
			std::string errorMsg = fmt::format( "Failed to deserialize DataChannelTypeNamesDto from JSON: {}", ex.what() );
			SPDLOG_ERROR( errorMsg );
			throw std::invalid_argument( errorMsg );
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			std::string errorMsg = fmt::format( "Failed to deserialize DataChannelTypeNamesDto from JSON: {}", ex.what() );
			SPDLOG_ERROR( errorMsg );
			throw std::invalid_argument( errorMsg );
		}
	}

	nlohmann::json DataChannelTypeNamesDto::toJson() const
	{
		nlohmann::json j = *this;
		return j;
	}

	//----------------------------------------------
	// Private Serialization Methods
	//----------------------------------------------

	void to_json( nlohmann::json& j, const DataChannelTypeNamesDto& dto )
	{
		j = nlohmann::json{ { VALUES_KEY, dto.m_values } };
	}

	void from_json( const nlohmann::json& j, DataChannelTypeNamesDto& dto )
	{
		dto.m_values.clear();

		if ( j.contains( VALUES_KEY ) )
		{
			if ( !j.at( VALUES_KEY ).is_array() )
			{
				throw nlohmann::json::type_error::create( 302, fmt::format( "DataChannelTypeNamesDto JSON field '{}' is not an array", VALUES_KEY ), nullptr );
			}

			const auto& jsonArray = j.at( VALUES_KEY );
			size_t valueCount = jsonArray.size();

			dto.m_values.reserve( valueCount );
			size_t successCount = 0;

			for ( const auto& itemJson : jsonArray )
			{
				try
				{
					dto.m_values.emplace_back( itemJson.get<DataChannelTypeNameDto>() );
					successCount++;
				}
				catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
				{
					SPDLOG_ERROR( "Skipping malformed data channel type name at index {}: {}", successCount, ex.what() );
				}
				catch ( [[maybe_unused]] const std::exception& ex )
				{
					SPDLOG_ERROR( "Standard exception parsing data channel type name at index {}: {}", successCount, ex.what() );
				}
			}

			if ( dto.m_values.size() > 1000 )
			{
				[[maybe_unused]] size_t approxBytes = estimateMemoryUsage( dto.m_values );
			}
		}
		else
		{
			SPDLOG_WARN( "No '{}' array found in data channel type names JSON", VALUES_KEY );
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
	// Accessors
	//----------------------------------------------

	const std::string& FormatDataTypeDto::type() const
	{
		return m_type;
	}

	const std::string& FormatDataTypeDto::description() const
	{
		return m_description;
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
				SPDLOG_ERROR( "JSON value for FormatDataTypeDto is not an object" );
				return std::nullopt;
			}

			FormatDataTypeDto dto = json.get<FormatDataTypeDto>();

			return std::optional<FormatDataTypeDto>{ std::move( dto ) };
		}
		catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
		{
			SPDLOG_ERROR( "nlohmann::json exception during FormatDataTypeDto parsing: {}", ex.what() );
			return std::nullopt;
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			SPDLOG_ERROR( "Standard exception during FormatDataTypeDto parsing: {}", ex.what() );
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
			std::string errorMsg = fmt::format( "Failed to deserialize FormatDataTypeDto from JSON: {}", ex.what() );
			SPDLOG_ERROR( errorMsg );
			throw std::invalid_argument( errorMsg );
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			std::string errorMsg = fmt::format( "Failed to deserialize FormatDataTypeDto from JSON: {}", ex.what() );
			SPDLOG_ERROR( errorMsg );
			throw std::invalid_argument( errorMsg );
		}
	}

	nlohmann::json FormatDataTypeDto::toJson() const
	{
		nlohmann::json j = *this;
		return j;
	}

	//----------------------------------------------
	// Private Serialization Methods
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
			throw nlohmann::json::parse_error::create( 101, 0u, fmt::format( "FormatDataTypeDto JSON missing required '{}' field or not a string", TYPE_KEY ), nullptr );
		}
		if ( j.contains( DESCRIPTION_KEY ) && !j.at( DESCRIPTION_KEY ).is_string() )
		{
			throw nlohmann::json::type_error::create( 302, fmt::format( "FormatDataTypeDto JSON field '{}' is not a string", DESCRIPTION_KEY ), nullptr );
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

		if ( dto.m_type.empty() )
		{
			SPDLOG_WARN( "Parsed FormatDataTypeDto has empty type field" );
		}
	}

	//=====================================================================
	// Collection of  Format Data Type Data Transfer Objects
	//=====================================================================

	//----------------------------------------------
	// Construction / destruction
	//----------------------------------------------

	FormatDataTypesDto::FormatDataTypesDto( std::vector<FormatDataTypeDto> values )
		: m_values{ std::move( values ) }
	{
	}

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	const std::vector<FormatDataTypeDto>& FormatDataTypesDto::values() const
	{
		return m_values;
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
				SPDLOG_ERROR( "JSON value for FormatDataTypesDto is not an object" );
				return std::nullopt;
			}

			FormatDataTypesDto dto = json.get<FormatDataTypesDto>();

			return std::optional<FormatDataTypesDto>{ std::move( dto ) };
		}
		catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
		{
			SPDLOG_ERROR( "nlohmann::json exception during FormatDataTypesDto parsing: {}", ex.what() );
			return std::nullopt;
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			SPDLOG_ERROR( "Standard exception during FormatDataTypesDto parsing: {}", ex.what() );
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
			std::string errorMsg = fmt::format( "Failed to deserialize FormatDataTypesDto from JSON: {}", ex.what() );
			SPDLOG_ERROR( errorMsg );
			throw std::invalid_argument( errorMsg );
		}
		catch ( const std::exception& ex )
		{
			std::string errorMsg = fmt::format( "Failed to deserialize FormatDataTypesDto from JSON: {}", ex.what() );
			SPDLOG_ERROR( errorMsg );
			throw std::invalid_argument( errorMsg );
		}
	}

	nlohmann::json FormatDataTypesDto::toJson() const
	{
		nlohmann::json j = *this;
		return j;
	}

	//----------------------------------------------
	// Private Serialization Methods
	//----------------------------------------------

	void to_json( nlohmann::json& j, const FormatDataTypesDto& dto )
	{
		j = nlohmann::json{ { VALUES_KEY, dto.m_values } };
	}

	void from_json( const nlohmann::json& j, FormatDataTypesDto& dto )
	{
		dto.m_values.clear();

		if ( j.contains( VALUES_KEY ) )
		{
			if ( !j.at( VALUES_KEY ).is_array() )
			{
				throw nlohmann::json::type_error::create( 302, fmt::format( "FormatDataTypesDto JSON field '{}' is not an array", VALUES_KEY ), nullptr );
			}

			const auto& jsonArray = j.at( VALUES_KEY );
			size_t valueCount = jsonArray.size();

			dto.m_values.reserve( valueCount );
			size_t successCount = 0;

			for ( const auto& itemJson : jsonArray )
			{
				try
				{
					dto.m_values.emplace_back( itemJson.get<FormatDataTypeDto>() );
					successCount++;
				}
				catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
				{
					SPDLOG_ERROR( "Skipping malformed format data type at index {}: {}", successCount, ex.what() );
				}
				catch ( [[maybe_unused]] const std::exception& ex )
				{
					SPDLOG_ERROR( "Standard exception parsing format data type at index {}: {}", successCount, ex.what() );
				}
			}
		}
		else
		{
			SPDLOG_WARN( "No '{}' array found in format data types JSON", VALUES_KEY );
		}
	}
}
