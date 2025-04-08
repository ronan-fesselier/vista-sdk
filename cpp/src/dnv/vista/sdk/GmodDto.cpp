#include "pch.h"

#include "dnv/vista/sdk/GmodDto.h"
#include <spdlog/spdlog.h>

namespace dnv::vista::sdk
{
	GmodNodeDto GmodNodeDto::FromJson( const rapidjson::Value& json )
	{
		GmodNodeDto node;

		node.category = "UNKNOWN";
		node.type = "UNKNOWN";
		node.code = "UNKNOWN";
		node.name = "UNKNOWN";

		if ( json.HasMember( "category" ) && json["category"].IsString() )
			node.category = json["category"].GetString();

		if ( json.HasMember( "type" ) && json["type"].IsString() )
			node.type = json["type"].GetString();

		if ( json.HasMember( "code" ) && json["code"].IsString() )
			node.code = json["code"].GetString();

		if ( json.HasMember( "name" ) && json["name"].IsString() )
			node.name = json["name"].GetString();

		if ( json.HasMember( "commonName" ) && json["commonName"].IsString() )
			node.commonName = json["commonName"].GetString();

		if ( json.HasMember( "definition" ) && json["definition"].IsString() )
			node.definition = json["definition"].GetString();

		if ( json.HasMember( "commonDefinition" ) && json["commonDefinition"].IsString() )
			node.commonDefinition = json["commonDefinition"].GetString();

		if ( json.HasMember( "installSubstructure" ) && json["installSubstructure"].IsBool() )
			node.installSubstructure = json["installSubstructure"].GetBool();

		if ( json.HasMember( "normalAssignmentNames" ) && json["normalAssignmentNames"].IsObject() )
		{
			std::unordered_map<std::string, std::string> assignmentNames;
			for ( auto it = json["normalAssignmentNames"].MemberBegin(); it != json["normalAssignmentNames"].MemberEnd(); ++it )
			{
				if ( it->name.IsString() && it->value.IsString() )
					assignmentNames[it->name.GetString()] = it->value.GetString();
			}
			if ( !assignmentNames.empty() )
				node.normalAssignmentNames = std::move( assignmentNames );
		}

		return node;
	}

	bool GmodNodeDto::TryFromJson( const rapidjson::Value& json, GmodNodeDto& dto )
	{
		try
		{
			dto = FromJson( json );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error deserializing GmodNodeDto: {}", e.what() );
			return false;
		}
	}

	rapidjson::Value GmodNodeDto::ToJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		rapidjson::Value obj( rapidjson::kObjectType );

		obj.AddMember( "category", rapidjson::Value( category.c_str(), allocator ).Move(), allocator );
		obj.AddMember( "type", rapidjson::Value( type.c_str(), allocator ).Move(), allocator );
		obj.AddMember( "code", rapidjson::Value( code.c_str(), allocator ).Move(), allocator );
		obj.AddMember( "name", rapidjson::Value( name.c_str(), allocator ).Move(), allocator );

		if ( commonName.has_value() )
			obj.AddMember( "commonName", rapidjson::Value( commonName.value().c_str(), allocator ).Move(), allocator );

		if ( definition.has_value() )
			obj.AddMember( "definition", rapidjson::Value( definition.value().c_str(), allocator ).Move(), allocator );

		if ( commonDefinition.has_value() )
			obj.AddMember( "commonDefinition", rapidjson::Value( commonDefinition.value().c_str(), allocator ).Move(), allocator );

		if ( installSubstructure.has_value() )
			obj.AddMember( "installSubstructure", rapidjson::Value( installSubstructure.value() ).Move(), allocator );

		if ( normalAssignmentNames.has_value() && !normalAssignmentNames.value().empty() )
		{
			rapidjson::Value assignmentObj( rapidjson::kObjectType );
			for ( const auto& [key, value] : normalAssignmentNames.value() )
			{
				assignmentObj.AddMember(
					rapidjson::Value( key.c_str(), allocator ).Move(),
					rapidjson::Value( value.c_str(), allocator ).Move(),
					allocator );
			}
			obj.AddMember( "normalAssignmentNames", assignmentObj, allocator );
		}

		return obj;
	}

	GmodDto GmodDto::FromJson( const rapidjson::Value& json )
	{
		GmodDto dto;

		dto.visVersion = "unknown";

		if ( json.HasMember( "visRelease" ) && json["visRelease"].IsString() )
			dto.visVersion = json["visRelease"].GetString();

		if ( json.HasMember( "items" ) && json["items"].IsArray() )
		{
			dto.items.reserve( json["items"].Size() );
			for ( const auto& item : json["items"].GetArray() )
			{
				try
				{
					dto.items.push_back( GmodNodeDto::FromJson( item ) );
				}
				catch ( const std::exception& e )
				{
					SPDLOG_ERROR( "Warning: Skipping malformed GMOD node: {}", e.what() );
				}
			}
		}

		if ( json.HasMember( "relations" ) && json["relations"].IsArray() )
		{
			dto.relations.reserve( json["relations"].Size() );
			for ( const auto& relation : json["relations"].GetArray() )
			{
				if ( relation.IsArray() )
				{
					std::vector<std::string> relationPair;
					relationPair.reserve( relation.Size() );
					for ( const auto& rel : relation.GetArray() )
					{
						if ( rel.IsString() )
							relationPair.push_back( rel.GetString() );
					}
					if ( !relationPair.empty() )
						dto.relations.push_back( std::move( relationPair ) );
				}
			}
		}

		return dto;
	}

	bool GmodDto::TryFromJson( const rapidjson::Value& json, GmodDto& dto )
	{
		try
		{
			dto = FromJson( json );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error deserializing GmodDto: {}", e.what() );
			return false;
		}
	}

	rapidjson::Value GmodDto::ToJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		rapidjson::Value obj( rapidjson::kObjectType );

		obj.AddMember( "visRelease", rapidjson::Value( visVersion.c_str(), allocator ).Move(), allocator );

		rapidjson::Value itemsArray( rapidjson::kArrayType );
		for ( const auto& item : items )
		{
			itemsArray.PushBack( item.ToJson( allocator ), allocator );
		}
		obj.AddMember( "items", itemsArray, allocator );

		rapidjson::Value relationsArray( rapidjson::kArrayType );
		for ( const auto& relation : relations )
		{
			rapidjson::Value relationArray( rapidjson::kArrayType );
			for ( const auto& rel : relation )
			{
				relationArray.PushBack( rapidjson::Value( rel.c_str(), allocator ).Move(), allocator );
			}
			relationsArray.PushBack( relationArray, allocator );
		}
		obj.AddMember( "relations", relationsArray, allocator );

		return obj;
	}
}
