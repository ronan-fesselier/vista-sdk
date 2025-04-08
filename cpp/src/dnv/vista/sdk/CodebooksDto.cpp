#include "pch.h"

#include "dnv/vista/sdk/CodebooksDto.h"

namespace dnv::vista::sdk
{
	CodebookDto CodebookDto::FromJson( const rapidjson::Value& json )
	{
		CodebookDto dto;

		if ( !json.HasMember( "name" ) || !json["name"].IsString() )
		{
			throw std::invalid_argument( "Codebook JSON missing required 'name' field or field is not a string" );
		}

		dto.name = json["name"].GetString();

		if ( json.HasMember( "values" ) && json["values"].IsObject() )
		{
			for ( auto it = json["values"].MemberBegin(); it != json["values"].MemberEnd(); ++it )
			{
				if ( !it->value.IsArray() )
				{
					SPDLOG_WARN( "Group '{}' values are not in array format, skipping", it->name.GetString() );
					continue;
				}

				std::vector<std::string> groupValues;
				groupValues.reserve( it->value.Size() );

				for ( const auto& value : it->value.GetArray() )
				{
					if ( value.IsString() )
					{
						groupValues.push_back( value.GetString() );
					}
					else
					{
						SPDLOG_WARN( "Non-string value found in group '{}', skipping", it->name.GetString() );
					}
				}

				if ( !groupValues.empty() )
				{
					dto.values[it->name.GetString()] = std::move( groupValues );
				}
			}
		}

		return dto;
	}

	bool CodebookDto::TryFromJson( const rapidjson::Value& json, CodebookDto& dto )
	{
		try
		{
			dto = FromJson( json );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error deserializing CodebookDto: {}", e.what() );
			return false;
		}
	}

	rapidjson::Value CodebookDto::ToJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		rapidjson::Value obj( rapidjson::kObjectType );

		obj.AddMember( "name", rapidjson::Value( name.c_str(), allocator ).Move(), allocator );

		rapidjson::Value valuesObj( rapidjson::kObjectType );

		for ( const auto& group : values )
		{
			rapidjson::Value groupValues( rapidjson::kArrayType );

			for ( const auto& value : group.second )
			{
				groupValues.PushBack( rapidjson::Value( value.c_str(), allocator ).Move(), allocator );
			}

			valuesObj.AddMember( rapidjson::Value( group.first.c_str(), allocator ).Move(),
				groupValues, allocator );
		}

		obj.AddMember( "values", valuesObj, allocator );

		return obj;
	}

	CodebooksDto CodebooksDto::FromJson( const rapidjson::Value& json )
	{
		CodebooksDto dto;

		if ( !json.HasMember( "visRelease" ) || !json["visRelease"].IsString() )
		{
			throw std::invalid_argument( "Codebooks JSON missing required 'visRelease' field or field is not a string" );
		}

		dto.visVersion = json["visRelease"].GetString();

		if ( json.HasMember( "items" ) && json["items"].IsArray() )
		{
			dto.items.reserve( json["items"].Size() );

			for ( const auto& item : json["items"].GetArray() )
			{
				try
				{
					dto.items.push_back( CodebookDto::FromJson( item ) );
				}
				catch ( const std::exception& e )
				{
					SPDLOG_WARN( "Skipping invalid codebook: {}", e.what() );
				}
			}
		}

		return dto;
	}

	bool CodebooksDto::TryFromJson( const rapidjson::Value& json, CodebooksDto& dto )
	{
		try
		{
			dto = FromJson( json );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error deserializing CodebooksDto: {}", e.what() );
			return false;
		}
	}

	rapidjson::Value CodebooksDto::ToJson( rapidjson::Document::AllocatorType& allocator ) const
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
