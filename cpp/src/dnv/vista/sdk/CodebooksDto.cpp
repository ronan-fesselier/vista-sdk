#include "pch.h"

#include "dnv/vista/sdk/CodebooksDto.h"

namespace dnv::vista::sdk
{
	CodebookDto::CodebookDto( std::string name, std::unordered_map<std::string, std::vector<std::string>> values )
		: name( std::move( name ) ), values( std::move( values ) ) {}

	CodebookDto CodebookDto::fromJson( const rapidjson::Value& json )
	{
		CodebookDto dto;

		if ( !json.HasMember( "name" ) || !json["name"].IsString() )
		{
			SPDLOG_ERROR( "Codebook JSON missing required 'name' field or field is not a string" );
			throw std::invalid_argument( "Codebook JSON missing required 'name' field or field is not a string" );
		}

		dto.name = json["name"].GetString();
		SPDLOG_INFO( "Parsing CodebookDto with name: {}", dto.name );

		if ( json.HasMember( "values" ) && json["values"].IsObject() )
		{
			size_t totalValues = 0;
			for ( auto it = json["values"].MemberBegin(); it != json["values"].MemberEnd(); ++it )
			{
				const char* groupName = it->name.GetString();

				if ( !it->value.IsArray() )
				{
					SPDLOG_WARN( "Group '{}' values are not in array format, skipping", groupName );
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
						SPDLOG_WARN( "Non-string value found in group '{}', skipping", groupName );
					}
				}

				if ( !groupValues.empty() )
				{
					totalValues += groupValues.size();
					dto.values[groupName] = std::move( groupValues );
				}
			}

			SPDLOG_INFO( "Parsed {} groups with {} total values for codebook '{}'",
				dto.values.size(), totalValues, dto.name );
		}
		else
		{
			SPDLOG_WARN( "No values found for codebook '{}'", dto.name );
		}

		return dto;
	}

	bool CodebookDto::tryFromJson( const rapidjson::Value& json, CodebookDto& dto )
	{
		try
		{
			dto = fromJson( json );
			SPDLOG_INFO( "Successfully parsed CodebookDto '{}' with {} value groups",
				dto.name, dto.values.size() );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error deserializing CodebookDto: {}", e.what() );
			return false;
		}
	}

	rapidjson::Value CodebookDto::toJson( rapidjson::Document::AllocatorType& allocator ) const
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
		SPDLOG_INFO( "Serialized CodebookDto '{}' with {} groups", name, values.size() );

		return obj;
	}

	CodebooksDto::CodebooksDto( std::string visVersion, std::vector<CodebookDto> items )
		: visVersion( std::move( visVersion ) ), items( std::move( items ) ) {}

	CodebooksDto CodebooksDto::fromJson( const rapidjson::Value& json )
	{
		CodebooksDto dto;

		if ( !json.HasMember( "visRelease" ) || !json["visRelease"].IsString() )
		{
			SPDLOG_ERROR( "Codebooks JSON missing required 'visRelease' field or field is not a string" );
			throw std::invalid_argument( "Codebooks JSON missing required 'visRelease' field or field is not a string" );
		}

		dto.visVersion = json["visRelease"].GetString();
		SPDLOG_INFO( "Parsing CodebooksDto for VIS version: {}", dto.visVersion );

		if ( json.HasMember( "items" ) && json["items"].IsArray() )
		{
			size_t totalItems = json["items"].Size();
			dto.items.reserve( totalItems );

			SPDLOG_INFO( "Found {} codebook items to parse", totalItems );

			size_t successCount = 0;
			for ( const auto& item : json["items"].GetArray() )
			{
				try
				{
					dto.items.push_back( CodebookDto::fromJson( item ) );
					successCount++;
				}
				catch ( const std::exception& e )
				{
					SPDLOG_WARN( "Skipping invalid codebook: {}", e.what() );
				}
			}

			SPDLOG_INFO( "Successfully parsed {}/{} codebooks for VIS version {}",
				successCount, totalItems, dto.visVersion );
		}
		else
		{
			SPDLOG_WARN( "No 'items' array found in CodebooksDto or not an array" );
		}

		return dto;
	}

	bool CodebooksDto::tryFromJson( const rapidjson::Value& json, CodebooksDto& dto )
	{
		try
		{
			dto = fromJson( json );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error deserializing CodebooksDto: {}", e.what() );
			return false;
		}
	}

	rapidjson::Value CodebooksDto::toJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		rapidjson::Value obj( rapidjson::kObjectType );

		obj.AddMember( "visRelease", rapidjson::Value( visVersion.c_str(), allocator ).Move(), allocator );

		rapidjson::Value itemsArray( rapidjson::kArrayType );

		for ( const auto& item : items )
		{
			itemsArray.PushBack( item.toJson( allocator ), allocator );
		}

		obj.AddMember( "items", itemsArray, allocator );
		SPDLOG_INFO( "Serialized CodebooksDto with {} items for VIS version {}",
			items.size(), visVersion );

		return obj;
	}
}
