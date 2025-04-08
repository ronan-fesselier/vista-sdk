#include "pch.h"

#include "dnv/vista/sdk/GmodVersioningDto.h"

namespace dnv::vista::sdk
{
	GmodVersioningAssignmentChangeDto GmodVersioningAssignmentChangeDto::FromJson( const rapidjson::Value& json )
	{
		GmodVersioningAssignmentChangeDto dto;

		if ( json.HasMember( "oldAssignment" ) && json["oldAssignment"].IsString() )
			dto.oldAssignment = json["oldAssignment"].GetString();

		if ( json.HasMember( "currentAssignment" ) && json["currentAssignment"].IsString() )
			dto.currentAssignment = json["currentAssignment"].GetString();

		return dto;
	}

	rapidjson::Value GmodVersioningAssignmentChangeDto::ToJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		rapidjson::Value obj( rapidjson::kObjectType );

		obj.AddMember( "oldAssignment", rapidjson::Value( oldAssignment.c_str(), allocator ).Move(), allocator );
		obj.AddMember( "currentAssignment", rapidjson::Value( currentAssignment.c_str(), allocator ).Move(), allocator );

		return obj;
	}

	GmodNodeConversionDto GmodNodeConversionDto::FromJson( const rapidjson::Value& json )
	{
		GmodNodeConversionDto dto;

		if ( json.HasMember( "operations" ) && json["operations"].IsArray() )
		{
			for ( const auto& operation : json["operations"].GetArray() )
			{
				if ( operation.IsString() )
					dto.operations.insert( operation.GetString() );
			}
		}

		if ( json.HasMember( "source" ) && json["source"].IsString() )
			dto.source = json["source"].GetString();

		if ( json.HasMember( "target" ) && json["target"].IsString() )
			dto.target = json["target"].GetString();

		if ( json.HasMember( "oldAssignment" ) && json["oldAssignment"].IsString() )
			dto.oldAssignment = json["oldAssignment"].GetString();

		if ( json.HasMember( "newAssignment" ) && json["newAssignment"].IsString() )
			dto.newAssignment = json["newAssignment"].GetString();

		if ( json.HasMember( "deleteAssignment" ) && json["deleteAssignment"].IsBool() )
			dto.deleteAssignment = json["deleteAssignment"].GetBool();

		return dto;
	}

	rapidjson::Value GmodNodeConversionDto::ToJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		rapidjson::Value obj( rapidjson::kObjectType );

		rapidjson::Value operationsArray( rapidjson::kArrayType );
		for ( const auto& operation : operations )
		{
			operationsArray.PushBack( rapidjson::Value( operation.c_str(), allocator ).Move(), allocator );
		}
		obj.AddMember( "operations", operationsArray, allocator );

		obj.AddMember( "source", rapidjson::Value( source.c_str(), allocator ).Move(), allocator );
		obj.AddMember( "target", rapidjson::Value( target.c_str(), allocator ).Move(), allocator );
		obj.AddMember( "oldAssignment", rapidjson::Value( oldAssignment.c_str(), allocator ).Move(), allocator );
		obj.AddMember( "newAssignment", rapidjson::Value( newAssignment.c_str(), allocator ).Move(), allocator );

		obj.AddMember( "deleteAssignment", rapidjson::Value( deleteAssignment ).Move(), allocator );

		return obj;
	}

	GmodVersioningDto GmodVersioningDto::FromJson( const rapidjson::Value& json )
	{
		GmodVersioningDto dto;

		if ( json.HasMember( "visRelease" ) && json["visRelease"].IsString() )
			dto.visVersion = json["visRelease"].GetString();

		if ( json.HasMember( "items" ) && json["items"].IsObject() )
		{
			for ( auto it = json["items"].MemberBegin(); it != json["items"].MemberEnd(); ++it )
			{
				if ( it->value.IsObject() )
				{
					try
					{
						dto.items[it->name.GetString()] = GmodNodeConversionDto::FromJson( it->value );
					}
					catch ( const std::exception& e )
					{
						SPDLOG_ERROR( "Error parsing GmodNodeConversionDto: {}", e.what() );
					}
				}
			}
		}

		return dto;
	}

	rapidjson::Value GmodVersioningDto::ToJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		rapidjson::Value obj( rapidjson::kObjectType );

		obj.AddMember( "visRelease", rapidjson::Value( visVersion.c_str(), allocator ).Move(), allocator );

		rapidjson::Value itemsObj( rapidjson::kObjectType );
		for ( const auto& [key, value] : items )
		{
			itemsObj.AddMember(
				rapidjson::Value( key.c_str(), allocator ).Move(),
				value.ToJson( allocator ),
				allocator );
		}
		obj.AddMember( "items", itemsObj, allocator );

		return obj;
	}
}
