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

	DataChannelTypeNameDto DataChannelTypeNameDto::FromJson( const rapidjson::Value& json )
	{
		DataChannelTypeNameDto dto;

		if ( json.HasMember( "type" ) && json["type"].IsString() )
			dto.type = json["type"].GetString();

		if ( json.HasMember( "description" ) && json["description"].IsString() )
			dto.description = json["description"].GetString();

		return dto;
	}

	rapidjson::Value DataChannelTypeNameDto::ToJson( rapidjson::Document::AllocatorType& allocator ) const
	{
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

	DataChannelTypeNamesDto DataChannelTypeNamesDto::FromJson( const rapidjson::Value& json )
	{
		DataChannelTypeNamesDto dto;

		if ( json.HasMember( "values" ) && json["values"].IsArray() )
		{
			dto.values.reserve( json["values"].Size() );
			for ( const auto& item : json["values"].GetArray() )
			{
				try
				{
					dto.values.push_back( DataChannelTypeNameDto::FromJson( item ) );
				}
				catch ( const std::exception& e )
				{
					SPDLOG_ERROR( "Warning: Skipping malformed data channel type name: {}", e.what() );
				}
			}
		}

		return dto;
	}

	rapidjson::Value DataChannelTypeNamesDto::ToJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		rapidjson::Value obj( rapidjson::kObjectType );

		rapidjson::Value valuesArray( rapidjson::kArrayType );
		for ( const auto& value : values )
		{
			valuesArray.PushBack( value.ToJson( allocator ), allocator );
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

	FormatDataTypeDto FormatDataTypeDto::FromJson( const rapidjson::Value& json )
	{
		FormatDataTypeDto dto;

		if ( json.HasMember( "type" ) && json["type"].IsString() )
			dto.type = json["type"].GetString();

		if ( json.HasMember( "description" ) && json["description"].IsString() )
			dto.description = json["description"].GetString();

		return dto;
	}

	rapidjson::Value FormatDataTypeDto::ToJson( rapidjson::Document::AllocatorType& allocator ) const
	{
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

	FormatDataTypesDto FormatDataTypesDto::FromJson( const rapidjson::Value& json )
	{
		FormatDataTypesDto dto;

		if ( json.HasMember( "values" ) && json["values"].IsArray() )
		{
			dto.values.reserve( json["values"].Size() );
			for ( const auto& item : json["values"].GetArray() )
			{
				try
				{
					dto.values.push_back( FormatDataTypeDto::FromJson( item ) );
				}
				catch ( const std::exception& e )
				{
					SPDLOG_ERROR( "Warning: Skipping malformed format data type: {}", e.what() );
				}
			}
		}

		return dto;
	}

	rapidjson::Value FormatDataTypesDto::ToJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		rapidjson::Value obj( rapidjson::kObjectType );

		rapidjson::Value valuesArray( rapidjson::kArrayType );
		for ( const auto& value : values )
		{
			valuesArray.PushBack( value.ToJson( allocator ), allocator );
		}

		obj.AddMember( "values", valuesArray, allocator );

		return obj;
	}
}
