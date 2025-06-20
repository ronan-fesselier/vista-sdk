/**
 * @file ISO19848Dtos.cpp
 * @brief Implementation of ISO 19848 data transfer objects
 */

#include "pch.h"

#include "dnv/vista/sdk/ISO19848Dtos.h"

#include "dnv/vista/sdk/Config.h"

namespace dnv::vista::sdk
{
	namespace
	{
		//=====================================================================
		// JSON parsing helper functions
		//=====================================================================

		static constexpr std::string_view UNKNOWN_TYPE = "[unknown type]";

		std::string_view extractTypeHint( const nlohmann::json& json ) noexcept
		{
			try
			{
				if ( json.contains( ISO19848_DTO_KEY_TYPE ) && json.at( ISO19848_DTO_KEY_TYPE ).is_string() )
				{
					const auto& str = json.at( ISO19848_DTO_KEY_TYPE ).get_ref<const std::string&>();
					return std::string_view{ str };
				}
				return UNKNOWN_TYPE;
			}
			catch ( ... )
			{
				return UNKNOWN_TYPE;
			}
		}
	}

	//=====================================================================
	// Single Data Channel Type data transfer objects
	//=====================================================================

	//----------------------------------------------
	// Serialization
	//----------------------------------------------

	std::optional<DataChannelTypeNameDto> DataChannelTypeNameDto::tryFromJson( const nlohmann::json& json )
	{
		const auto typeHint = extractTypeHint( json );

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
			SPDLOG_ERROR( "JSON exception during DataChannelTypeNameDto parsing (hint: type='{}'): {}",
				typeHint, ex.what() );
			return std::nullopt;
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			SPDLOG_ERROR( "Standard exception during DataChannelTypeNameDto parsing (hint: type='{}'): {}",
				typeHint, ex.what() );
			return std::nullopt;
		}
	}

	DataChannelTypeNameDto DataChannelTypeNameDto::fromJson( const nlohmann::json& json )
	{
		const auto typeHint = extractTypeHint( json );
		auto dtoOpt = DataChannelTypeNameDto::tryFromJson( json );
		if ( !dtoOpt.has_value() )
		{
			throw std::invalid_argument( fmt::format( "Failed to deserialize DataChannelTypeNameDto from JSON (hint: type='{}')",
				typeHint ) );
		}

		return std::move( dtoOpt ).value();
	}

	nlohmann::json DataChannelTypeNameDto::toJson() const
	{
		nlohmann::json result;
		to_json( result, *this );

		return result;
	}

	//----------------------------------------------
	// Private serialization methods
	//----------------------------------------------

	void from_json( const nlohmann::json& j, DataChannelTypeNameDto& dto )
	{
		const auto typeIt = j.find( ISO19848_DTO_KEY_TYPE );
		const auto descIt = j.find( ISO19848_DTO_KEY_DESCRIPTION );

		if ( typeIt == j.end() || !typeIt->is_string() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u,
				fmt::format( "DataChannelTypeNameDto JSON missing required '{}' field or not a string", ISO19848_DTO_KEY_TYPE ), nullptr );
		}
		if ( descIt == j.end() || !descIt->is_string() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u,
				fmt::format( "DataChannelTypeNameDto JSON missing required '{}' field or not a string", ISO19848_DTO_KEY_DESCRIPTION ), nullptr );
		}

		dto.m_type = typeIt->get<std::string>();
		dto.m_description = descIt->get<std::string>();

		if ( dto.m_type.empty() )
		{
			SPDLOG_WARN( "Empty 'type' field found in DataChannelTypeNameDto" );
		}
		if ( dto.m_description.empty() )
		{
			SPDLOG_WARN( "Empty 'description' field found in DataChannelTypeNameDto" );
		}
	}

	void to_json( nlohmann::json& j, const DataChannelTypeNameDto& dto )
	{
		j = nlohmann::json{
			{ ISO19848_DTO_KEY_TYPE, dto.m_type },
			{ ISO19848_DTO_KEY_DESCRIPTION, dto.m_description } };
	}

	//=====================================================================
	// Collection of Data Channel Type data transfer objects
	//=====================================================================

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

			if ( !json.contains( ISO19848_DTO_KEY_VALUES ) || !json.at( ISO19848_DTO_KEY_VALUES ).is_array() )
			{
				SPDLOG_ERROR( "DataChannelTypeNamesDto JSON missing required '{}' array", ISO19848_DTO_KEY_VALUES );
				return std::nullopt;
			}

			const auto& valuesArray = json.at( ISO19848_DTO_KEY_VALUES );
			size_t totalItems = valuesArray.size();
			size_t successCount = 0;

			if ( totalItems > 10000 )
			{
				[[maybe_unused]] const size_t approxMemoryUsage = ( totalItems * sizeof( DataChannelTypeNameDto ) ) / ( 1024 * 1024 );
				SPDLOG_DEBUG( "Large ISO19848 data channel types loaded: ~{} MB estimated memory usage", approxMemoryUsage );
			}

			std::vector<DataChannelTypeNameDto> tempValues;
			const size_t reserveSize = totalItems < 1000
										   ? totalItems + totalItems / 4
										   : totalItems + totalItems / 16;
			tempValues.reserve( reserveSize );

			for ( const auto& itemJson : valuesArray )
			{
				auto itemOpt = DataChannelTypeNameDto::tryFromJson( itemJson );
				if ( itemOpt.has_value() )
				{
					tempValues.emplace_back( std::move( *itemOpt ) );
					successCount++;
				}
				else
				{
					SPDLOG_WARN( "Skipping invalid DataChannelTypeNameDto item during parsing" );
				}
			}

			SPDLOG_DEBUG( "Successfully parsed {}/{} data channel type names", successCount, totalItems );

			/* If parsing failed for more than 10% of items, shrink the vector to potentially save memory */
			if ( totalItems > 0 && successCount < totalItems * 9 / 10 )
			{
				if ( tempValues.capacity() > tempValues.size() * 4 / 3 )
				{
					tempValues.shrink_to_fit();
				}
			}

			DataChannelTypeNamesDto resultDto( std::move( tempValues ) );
			return std::optional<DataChannelTypeNamesDto>{ std::move( resultDto ) };
		}
		catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
		{
			SPDLOG_ERROR( "JSON exception during DataChannelTypeNamesDto parsing: {}", ex.what() );
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
		auto dtoOpt = DataChannelTypeNamesDto::tryFromJson( json );
		if ( !dtoOpt.has_value() )
		{
			throw std::invalid_argument( "Failed to deserialize DataChannelTypeNamesDto from JSON" );
		}
		return std::move( dtoOpt ).value();
	}

	nlohmann::json DataChannelTypeNamesDto::toJson() const
	{
		nlohmann::json result;
		to_json( result, *this );
		return result;
	}

	//----------------------------------------------
	// Private serialization methods
	//----------------------------------------------

	void from_json( const nlohmann::json& j, DataChannelTypeNamesDto& dto )
	{
		if ( !j.contains( ISO19848_DTO_KEY_VALUES ) || !j.at( ISO19848_DTO_KEY_VALUES ).is_array() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u, fmt::format( "DataChannelTypeNamesDto JSON missing required '{}' array", ISO19848_DTO_KEY_VALUES ), nullptr );
		}

		dto.m_values = j.at( ISO19848_DTO_KEY_VALUES ).get<std::vector<DataChannelTypeNameDto>>();
	}

	void to_json( nlohmann::json& j, const DataChannelTypeNamesDto& dto )
	{
		j = nlohmann::json{ { ISO19848_DTO_KEY_VALUES, dto.m_values } };
	}

	//=====================================================================
	// Single Format Data Type data transfer objects
	//=====================================================================

	//----------------------------------------------
	// Serialization
	//----------------------------------------------

	std::optional<FormatDataTypeDto> FormatDataTypeDto::tryFromJson( const nlohmann::json& json )
	{
		const auto typeHint = extractTypeHint( json );

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
			SPDLOG_ERROR( "JSON exception during FormatDataTypeDto parsing (hint: type='{}'): {}",
				typeHint, ex.what() );
			return std::nullopt;
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			SPDLOG_ERROR( "Standard exception during FormatDataTypeDto parsing (hint: type='{}'): {}",
				typeHint, ex.what() );
			return std::nullopt;
		}
	}

	FormatDataTypeDto FormatDataTypeDto::fromJson( const nlohmann::json& json )
	{
		const auto typeHint = extractTypeHint( json );
		auto dtoOpt = FormatDataTypeDto::tryFromJson( json );
		if ( !dtoOpt.has_value() )
		{
			throw std::invalid_argument( fmt::format( "Failed to deserialize FormatDataTypeDto from JSON (hint: type='{}')",
				typeHint ) );
		}

		return std::move( dtoOpt ).value();
	}

	nlohmann::json FormatDataTypeDto::toJson() const
	{
		nlohmann::json result;
		to_json( result, *this );
		return result;
	}

	//----------------------------------------------
	// Private serialization methods
	//----------------------------------------------

	void from_json( const nlohmann::json& j, FormatDataTypeDto& dto )
	{
		const auto typeIt = j.find( ISO19848_DTO_KEY_TYPE );
		const auto descIt = j.find( ISO19848_DTO_KEY_DESCRIPTION );

		if ( typeIt == j.end() || !typeIt->is_string() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u,
				fmt::format( "FormatDataTypeDto JSON missing required '{}' field or not a string", ISO19848_DTO_KEY_TYPE ), nullptr );
		}
		if ( descIt == j.end() || !descIt->is_string() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u,
				fmt::format( "FormatDataTypeDto JSON missing required '{}' field or not a string", ISO19848_DTO_KEY_DESCRIPTION ), nullptr );
		}

		dto.m_type = typeIt->get<std::string>();
		dto.m_description = descIt->get<std::string>();

		if ( dto.m_type.empty() )
		{
			SPDLOG_WARN( "Empty 'type' field found in FormatDataTypeDto" );
		}
		if ( dto.m_description.empty() )
		{
			SPDLOG_WARN( "Empty 'description' field found in FormatDataTypeDto" );
		}
	}
	void to_json( nlohmann::json& j, const FormatDataTypeDto& dto )
	{
		j = nlohmann::json{
			{ ISO19848_DTO_KEY_TYPE, dto.m_type },
			{ ISO19848_DTO_KEY_DESCRIPTION, dto.m_description } };
	}

	//=====================================================================
	// Collection of Format Data Type data transfer objects
	//=====================================================================

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

			if ( !json.contains( ISO19848_DTO_KEY_VALUES ) || !json.at( ISO19848_DTO_KEY_VALUES ).is_array() )
			{
				SPDLOG_ERROR( "FormatDataTypesDto JSON missing required '{}' array", ISO19848_DTO_KEY_VALUES );
				return std::nullopt;
			}

			const auto& valuesArray = json.at( ISO19848_DTO_KEY_VALUES );
			size_t totalItems = valuesArray.size();
			size_t successCount = 0;

			if ( totalItems > 10000 )
			{
				[[maybe_unused]] const size_t approxMemoryUsage = ( totalItems * sizeof( FormatDataTypeDto ) ) / ( 1024 * 1024 );
				SPDLOG_DEBUG( "Large ISO19848 format data types loaded: ~{} MB estimated memory usage", approxMemoryUsage );
			}

			std::vector<FormatDataTypeDto> tempValues;
			const size_t reserveSize = totalItems < 1000
										   ? totalItems + totalItems / 4
										   : totalItems + totalItems / 16;
			tempValues.reserve( reserveSize );

			for ( const auto& itemJson : valuesArray )
			{
				auto itemOpt = FormatDataTypeDto::tryFromJson( itemJson );
				if ( itemOpt.has_value() )
				{
					tempValues.emplace_back( std::move( *itemOpt ) );
					successCount++;
				}
				else
				{
					SPDLOG_WARN( "Skipping invalid FormatDataTypeDto item during parsing" );
				}
			}

			SPDLOG_DEBUG( "Successfully parsed {}/{} format data types", successCount, totalItems );

			/* If parsing failed for more than 10% of items, shrink the vector to potentially save memory */
			if ( totalItems > 0 && successCount < totalItems * 9 / 10 )
			{
				if ( tempValues.capacity() > tempValues.size() * 4 / 3 )
				{
					tempValues.shrink_to_fit();
				}
			}

			FormatDataTypesDto resultDto( std::move( tempValues ) );
			return std::optional<FormatDataTypesDto>{ std::move( resultDto ) };
		}
		catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
		{
			SPDLOG_ERROR( "JSON exception during FormatDataTypesDto parsing: {}", ex.what() );
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
		auto dtoOpt = FormatDataTypesDto::tryFromJson( json );
		if ( !dtoOpt.has_value() )
		{
			throw std::invalid_argument( "Failed to deserialize FormatDataTypesDto from JSON" );
		}
		return std::move( dtoOpt ).value();
	}

	nlohmann::json FormatDataTypesDto::toJson() const
	{
		nlohmann::json result;
		to_json( result, *this );
		return result;
	}

	//----------------------------------------------
	// Private serialization methods
	//----------------------------------------------

	void from_json( const nlohmann::json& j, FormatDataTypesDto& dto )
	{
		if ( !j.contains( ISO19848_DTO_KEY_VALUES ) || !j.at( ISO19848_DTO_KEY_VALUES ).is_array() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u, fmt::format( "FormatDataTypesDto JSON missing required '{}' array", ISO19848_DTO_KEY_VALUES ), nullptr );
		}

		dto.m_values = j.at( ISO19848_DTO_KEY_VALUES ).get<std::vector<FormatDataTypeDto>>();
	}

	void to_json( nlohmann::json& j, const FormatDataTypesDto& dto )
	{
		j = nlohmann::json{ { ISO19848_DTO_KEY_VALUES, dto.m_values } };
	}
}
