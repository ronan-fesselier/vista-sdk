#include "pch.h"

#include "dnv/vista/sdk/LocationsDto.h"

namespace dnv::vista::sdk
{
	RelativeLocationsDto RelativeLocationsDto::FromJson( const rapidjson::Value& json )
	{
		RelativeLocationsDto dto;

		if ( !json.HasMember( "code" ) || !json["code"].IsString() || json["code"].GetStringLength() == 0 )
		{
			throw std::runtime_error( "Missing or invalid 'code' field in RelativeLocationsDto JSON." );
		}

		if ( !json.HasMember( "name" ) || !json["name"].IsString() )
		{
			throw std::runtime_error( "Missing or invalid 'name' field in RelativeLocationsDto JSON." );
		}

		dto.code = json["code"].GetString()[0];

		dto.name = json["name"].GetString();

		if ( json.HasMember( "definition" ) && json["definition"].IsString() )
		{
			dto.definition = json["definition"].GetString();
		}

		return dto;
	}

	rapidjson::Value RelativeLocationsDto::ToJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		rapidjson::Value obj( rapidjson::kObjectType );

		char codeStr[2] = { code, '\0' };
		obj.AddMember( "code", rapidjson::Value( codeStr, allocator ).Move(), allocator );

		obj.AddMember( "name", rapidjson::Value( name.c_str(), allocator ).Move(), allocator );

		if ( definition.has_value() )
		{
			obj.AddMember( "definition", rapidjson::Value( definition.value().c_str(), allocator ).Move(), allocator );
		}

		return obj;
	}

	LocationsDto LocationsDto::FromJson( const rapidjson::Value& json )
	{
		LocationsDto dto;

		if ( !json.HasMember( "visRelease" ) || !json["visRelease"].IsString() )
		{
			throw std::runtime_error( "Missing or invalid 'visRelease' field in LocationsDto JSON." );
		}

		dto.visVersion = json["visRelease"].GetString();

		if ( !json.HasMember( "items" ) || !json["items"].IsArray() )
		{
			throw std::runtime_error( "Missing or invalid 'items' field in LocationsDto JSON." );
		}

		dto.items.reserve( json["items"].Size() );
		for ( const auto& item : json["items"].GetArray() )
		{
			try
			{
				dto.items.push_back( RelativeLocationsDto::FromJson( item ) );
			}
			catch ( const std::exception& e )
			{
				SPDLOG_WARN( "Skipping invalid location item: {}", e.what() );
			}
		}

		return dto;
	}

	rapidjson::Value LocationsDto::ToJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		rapidjson::Value obj( rapidjson::kObjectType );

		obj.AddMember( "visRelease", rapidjson::Value( visVersion.c_str(), allocator ).Move(), allocator );

		rapidjson::Value itemsArray( rapidjson::kArrayType );
		for ( const auto& item : items )
		{
			itemsArray.PushBack( item.ToJson( allocator ), allocator );
		}

		obj.AddMember( "items", itemsArray, allocator );

		return obj;
	}
}
