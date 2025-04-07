#include "pch.h"

#include "dnv/vista/sdk/LocationsDto.h"

namespace dnv::vista::sdk
{
	//-------------------------------------------------------------------------
	// RelativeLocationsDto implementation
	//-------------------------------------------------------------------------

	RelativeLocationsDto::RelativeLocationsDto( char code, std::string name, std::optional<std::string> definition )
		: code( code ),
		  name( std::move( name ) ),
		  definition( std::move( definition ) )
	{
	}

	RelativeLocationsDto RelativeLocationsDto::fromJson( const rapidjson::Value& json )
	{
		SPDLOG_INFO( "Parsing RelativeLocationsDto from JSON" );
		RelativeLocationsDto dto;

		if ( !json.HasMember( "code" ) || !json["code"].IsString() || json["code"].GetStringLength() == 0 )
		{
			SPDLOG_ERROR( "Missing or invalid 'code' field in RelativeLocationsDto JSON." );
			throw std::runtime_error( "Missing or invalid 'code' field in RelativeLocationsDto JSON." );
		}

		if ( !json.HasMember( "name" ) || !json["name"].IsString() )
		{
			SPDLOG_ERROR( "Missing or invalid 'name' field in RelativeLocationsDto JSON." );
			throw std::runtime_error( "Missing or invalid 'name' field in RelativeLocationsDto JSON." );
		}

		dto.code = json["code"].GetString()[0];
		dto.name = json["name"].GetString();

		if ( json.HasMember( "definition" ) && json["definition"].IsString() )
		{
			dto.definition = json["definition"].GetString();
		}

		SPDLOG_INFO( "Successfully parsed relative location: code={}, name={}",
			dto.code, dto.name );
		return dto;
	}

	bool RelativeLocationsDto::tryFromJson( const rapidjson::Value& json, RelativeLocationsDto& dto )
	{
		try
		{
			dto = fromJson( json );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error deserializing RelativeLocationsDto: {}", e.what() );
			return false;
		}
	}

	rapidjson::Value RelativeLocationsDto::toJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		SPDLOG_INFO( "Serializing RelativeLocationsDto: code={}, name={}", code, name );
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

	//-------------------------------------------------------------------------
	// LocationsDto implementation
	//-------------------------------------------------------------------------

	LocationsDto::LocationsDto( std::string visVersion, std::vector<RelativeLocationsDto> items )
		: visVersion( std::move( visVersion ) ),
		  items( std::move( items ) )
	{
	}

	LocationsDto LocationsDto::fromJson( const rapidjson::Value& json )
	{
		SPDLOG_INFO( "Parsing LocationsDto from JSON" );
		LocationsDto dto;

		if ( !json.HasMember( "visRelease" ) || !json["visRelease"].IsString() )
		{
			SPDLOG_ERROR( "Missing or invalid 'visRelease' field in LocationsDto JSON." );
			throw std::runtime_error( "Missing or invalid 'visRelease' field in LocationsDto JSON." );
		}

		dto.visVersion = json["visRelease"].GetString();
		SPDLOG_INFO( "Parsing locations for VIS version: {}", dto.visVersion );

		if ( !json.HasMember( "items" ) || !json["items"].IsArray() )
		{
			SPDLOG_ERROR( "Missing or invalid 'items' field in LocationsDto JSON." );
			throw std::runtime_error( "Missing or invalid 'items' field in LocationsDto JSON." );
		}

		size_t itemCount = json["items"].Size();
		SPDLOG_INFO( "Found {} location items to parse", itemCount );
		dto.items.reserve( itemCount );

		size_t successCount = 0;
		for ( const auto& item : json["items"].GetArray() )
		{
			try
			{
				dto.items.push_back( RelativeLocationsDto::fromJson( item ) );
				successCount++;
			}
			catch ( const std::exception& e )
			{
				SPDLOG_WARN( "Skipping invalid location item: {}", e.what() );
			}
		}

		SPDLOG_INFO( "Successfully parsed {}/{} locations for VIS {}",
			successCount, itemCount, dto.visVersion );
		return dto;
	}

	bool LocationsDto::tryFromJson( const rapidjson::Value& json, LocationsDto& dto )
	{
		try
		{
			dto = fromJson( json );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error deserializing LocationsDto: {}", e.what() );
			return false;
		}
	}

	rapidjson::Value LocationsDto::toJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		SPDLOG_INFO( "Serializing LocationsDto: visVersion={}, items={}",
			visVersion, items.size() );
		rapidjson::Value obj( rapidjson::kObjectType );

		obj.AddMember( "visRelease", rapidjson::Value( visVersion.c_str(), allocator ).Move(), allocator );

		rapidjson::Value itemsArray( rapidjson::kArrayType );
		for ( const auto& item : items )
		{
			itemsArray.PushBack( item.toJson( allocator ), allocator );
		}
		obj.AddMember( "items", itemsArray, allocator );

		return obj;
	}
}
