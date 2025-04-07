#include "pch.h"

#include "dnv/vista/sdk/ISO19848Dtos.h"

namespace dnv::vista::sdk
{
	DataChannelTypeNameDto::DataChannelTypeNameDto(
		std::string type,
		std::string description )
		: type( std::move( type ) ), description( std::move( description ) )
	{
	}

	DataChannelTypeNameDto DataChannelTypeNameDto::fromJson( const rapidjson::Value& json )
	{
		SPDLOG_INFO( "Parsing DataChannelTypeNameDto from JSON" );
		DataChannelTypeNameDto dto;

		if ( json.HasMember( "type" ) && json["type"].IsString() )
			dto.type = json["type"].GetString();

		if ( json.HasMember( "description" ) && json["description"].IsString() )
			dto.description = json["description"].GetString();

		SPDLOG_INFO( "Parsed DataChannelTypeNameDto: type={}, description={}",
			dto.type, dto.description );
		return dto;
	}

	bool DataChannelTypeNameDto::tryFromJson( const rapidjson::Value& json, DataChannelTypeNameDto& dto )
	{
		try
		{
			dto = fromJson( json );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error deserializing DataChannelTypeNameDto: {}", e.what() );
			return false;
		}
	}

	rapidjson::Value DataChannelTypeNameDto::toJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		SPDLOG_INFO( "Serializing DataChannelTypeNameDto: type={}", type );
		rapidjson::Value obj( rapidjson::kObjectType );

		obj.AddMember( "type", rapidjson::Value( type.c_str(), allocator ).Move(), allocator );
		obj.AddMember( "description", rapidjson::Value( description.c_str(), allocator ).Move(), allocator );

		return obj;
	}

	DataChannelTypeNamesDto::DataChannelTypeNamesDto(
		std::vector<DataChannelTypeNameDto> values )
		: values( std::move( values ) )
	{
	}

	DataChannelTypeNamesDto DataChannelTypeNamesDto::fromJson( const rapidjson::Value& json )
	{
		SPDLOG_INFO( "Parsing data channel type names from JSON" );
		DataChannelTypeNamesDto dto;

		if ( json.HasMember( "values" ) && json["values"].IsArray() )
		{
			size_t valueCount = json["values"].Size();
			SPDLOG_INFO( "Found {} data channel type entries to parse", valueCount );

			dto.values.reserve( valueCount );
			size_t successCount = 0;

			for ( const auto& item : json["values"].GetArray() )
			{
				try
				{
					dto.values.push_back( DataChannelTypeNameDto::fromJson( item ) );
					successCount++;
				}
				catch ( const std::exception& e )
				{
					SPDLOG_ERROR( "Skipping malformed data channel type name: {}", e.what() );
				}
			}

			SPDLOG_INFO( "Successfully parsed {}/{} data channel type names",
				successCount, valueCount );
		}
		else
		{
			SPDLOG_WARN( "No 'values' array found in data channel type names JSON" );
		}

		return dto;
	}

	bool DataChannelTypeNamesDto::tryFromJson( const rapidjson::Value& json, DataChannelTypeNamesDto& dto )
	{
		try
		{
			dto = fromJson( json );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error deserializing DataChannelTypeNamesDto: {}", e.what() );
			return false;
		}
	}

	rapidjson::Value DataChannelTypeNamesDto::toJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		SPDLOG_INFO( "Serializing {} data channel type names to JSON", values.size() );
		rapidjson::Value obj( rapidjson::kObjectType );

		rapidjson::Value valuesArray( rapidjson::kArrayType );
		for ( const auto& value : values )
		{
			valuesArray.PushBack( value.toJson( allocator ), allocator );
		}

		obj.AddMember( "values", valuesArray, allocator );
		return obj;
	}

	FormatDataTypeDto::FormatDataTypeDto(
		std::string type,
		std::string description )
		: type( std::move( type ) ), description( std::move( description ) )
	{
	}

	FormatDataTypeDto FormatDataTypeDto::fromJson( const rapidjson::Value& json )
	{
		SPDLOG_INFO( "Parsing FormatDataTypeDto from JSON" );
		FormatDataTypeDto dto;

		if ( json.HasMember( "type" ) && json["type"].IsString() )
			dto.type = json["type"].GetString();

		if ( json.HasMember( "description" ) && json["description"].IsString() )
			dto.description = json["description"].GetString();

		SPDLOG_INFO( "Parsed FormatDataTypeDto: type={}, description={}",
			dto.type, dto.description );
		return dto;
	}

	bool FormatDataTypeDto::tryFromJson( const rapidjson::Value& json, FormatDataTypeDto& dto )
	{
		try
		{
			dto = fromJson( json );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error deserializing FormatDataTypeDto: {}", e.what() );
			return false;
		}
	}

	rapidjson::Value FormatDataTypeDto::toJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		SPDLOG_INFO( "Serializing FormatDataTypeDto: type={}", type );
		rapidjson::Value obj( rapidjson::kObjectType );

		obj.AddMember( "type", rapidjson::Value( type.c_str(), allocator ).Move(), allocator );
		obj.AddMember( "description", rapidjson::Value( description.c_str(), allocator ).Move(), allocator );

		return obj;
	}

	FormatDataTypesDto::FormatDataTypesDto(
		std::vector<FormatDataTypeDto> values )
		: values( std::move( values ) )
	{
	}

	FormatDataTypesDto FormatDataTypesDto::fromJson( const rapidjson::Value& json )
	{
		SPDLOG_INFO( "Parsing format data types from JSON" );
		FormatDataTypesDto dto;

		if ( json.HasMember( "values" ) && json["values"].IsArray() )
		{
			size_t valueCount = json["values"].Size();
			SPDLOG_INFO( "Found {} format data type entries to parse", valueCount );

			dto.values.reserve( valueCount );
			size_t successCount = 0;

			for ( const auto& item : json["values"].GetArray() )
			{
				try
				{
					dto.values.push_back( FormatDataTypeDto::fromJson( item ) );
					successCount++;
				}
				catch ( const std::exception& e )
				{
					SPDLOG_ERROR( "Skipping malformed format data type: {}", e.what() );
				}
			}

			SPDLOG_INFO( "Successfully parsed {}/{} format data types",
				successCount, valueCount );
		}
		else
		{
			SPDLOG_WARN( "No 'values' array found in format data types JSON" );
		}

		return dto;
	}

	bool FormatDataTypesDto::tryFromJson( const rapidjson::Value& json, FormatDataTypesDto& dto )
	{
		try
		{
			dto = fromJson( json );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error deserializing FormatDataTypesDto: {}", e.what() );
			return false;
		}
	}

	rapidjson::Value FormatDataTypesDto::toJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		SPDLOG_INFO( "Serializing {} format data types to JSON", values.size() );
		rapidjson::Value obj( rapidjson::kObjectType );

		rapidjson::Value valuesArray( rapidjson::kArrayType );
		for ( const auto& value : values )
		{
			valuesArray.PushBack( value.toJson( allocator ), allocator );
		}

		obj.AddMember( "values", valuesArray, allocator );
		return obj;
	}
}
