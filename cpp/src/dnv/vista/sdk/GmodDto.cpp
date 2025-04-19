#include "pch.h"

#include "dnv/vista/sdk/GmodDto.h"

namespace dnv::vista::sdk
{
	GmodNodeDto::GmodNodeDto(
		std::string category,
		std::string type,
		std::string code,
		std::string name,
		std::optional<std::string> commonName,
		std::optional<std::string> definition,
		std::optional<std::string> commonDefinition,
		std::optional<bool> installSubstructure,
		std::optional<std::unordered_map<std::string,
			std::string>>
			normalAssignmentNames )
		: category( std::move( category ) ),
		  type( std::move( type ) ),
		  code( std::move( code ) ),
		  name( std::move( name ) ),
		  commonName( std::move( commonName ) ),
		  definition( std::move( definition ) ),
		  commonDefinition( std::move( commonDefinition ) ),
		  installSubstructure( installSubstructure ),
		  normalAssignmentNames( std::move( normalAssignmentNames ) )
	{
	}

	GmodNodeDto GmodNodeDto::fromJson( const rapidjson::Value& json )
	{
		SPDLOG_DEBUG( "Parsing GmodNodeDto from JSON" );
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

		SPDLOG_DEBUG( "Parsed required GMOD node fields: category={}, type={}, code={}, name={}",
			node.category, node.type, node.code, node.name );

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
			auto& namesObj = json["normalAssignmentNames"];
			assignmentNames.reserve( namesObj.MemberCount() );

			for ( auto it = namesObj.MemberBegin(); it != namesObj.MemberEnd(); ++it )
			{
				if ( it->name.IsString() && it->value.IsString() )
					assignmentNames[it->name.GetString()] = it->value.GetString();
			}

			if ( !assignmentNames.empty() )
			{
				node.normalAssignmentNames = std::move( assignmentNames );
				SPDLOG_DEBUG( "Parsed {} normal assignment name mappings", assignmentNames.size() );
			}
		}

		SPDLOG_DEBUG( "Successfully parsed GmodNodeDto: code={}", node.code );
		return node;
	}

	bool GmodNodeDto::tryFromJson( const rapidjson::Value& json, GmodNodeDto& dto )
	{
		try
		{
			dto = fromJson( json );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error deserializing GmodNodeDto: {}", e.what() );
			return false;
		}
	}

	rapidjson::Value GmodNodeDto::toJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		SPDLOG_INFO( "Serializing GmodNodeDto: code={}", code );
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

	GmodDto::GmodDto( std::string visVersion, std::vector<GmodNodeDto> items, std::vector<std::vector<std::string>> relations )
		: visVersion( std::move( visVersion ) ),
		  items( std::move( items ) ),
		  relations( std::move( relations ) )
	{
	}

	GmodDto GmodDto::fromJson( const rapidjson::Value& json )
	{
		SPDLOG_INFO( "Parsing GMOD from JSON" );
		GmodDto dto;

		dto.visVersion = "unknown";

		if ( json.HasMember( "visRelease" ) && json["visRelease"].IsString() )
		{
			dto.visVersion = json["visRelease"].GetString();
			SPDLOG_INFO( "GMOD VIS version: {}", dto.visVersion );
		}

		if ( json.HasMember( "items" ) && json["items"].IsArray() )
		{
			size_t itemCount = json["items"].Size();
			SPDLOG_INFO( "Found {} GMOD node items to parse", itemCount );
			dto.items.reserve( itemCount );

			size_t successCount = 0;
			for ( rapidjson::SizeType i = 0; i < json["items"].Size(); ++i )
			{
				try
				{
					dto.items.push_back( GmodNodeDto::fromJson( json["items"][i] ) );
					successCount++;
				}
				catch ( const std::exception& e )
				{
					SPDLOG_ERROR( "Skipping malformed GMOD node at index {}: {}", i, e.what() );
				}
			}

			SPDLOG_INFO( "Successfully parsed {}/{} GMOD nodes", successCount, itemCount );
		}
		else
		{
			SPDLOG_WARN( "GMOD missing 'items' array or not in array format" );
		}

		if ( json.HasMember( "relations" ) && json["relations"].IsArray() )
		{
			size_t relationCount = json["relations"].Size();
			SPDLOG_INFO( "Found {} GMOD relation entries to parse", relationCount );
			dto.relations.reserve( relationCount );

			size_t validRelationCount = 0;
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
					{
						dto.relations.push_back( std::move( relationPair ) );
						validRelationCount++;
					}
				}
			}

			SPDLOG_DEBUG( "Added {} valid relations to GMOD", validRelationCount );
		}
		else
		{
			SPDLOG_INFO( "GMOD has no relations or 'relations' is not an array" );
		}

		SPDLOG_INFO( "GMOD parsing complete: {} nodes, {} relations, VIS version {}",
			dto.items.size(), dto.relations.size(), dto.visVersion );
		return dto;
	}

	bool GmodDto::tryFromJson( const rapidjson::Value& json, GmodDto& dto )
	{
		try
		{
			dto = fromJson( json );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error deserializing GmodDto: {}", e.what() );
			return false;
		}
	}

	rapidjson::Value GmodDto::toJson( rapidjson::Document::AllocatorType& allocator ) const
	{
		SPDLOG_INFO( "Serializing GmodDto with {} nodes and {} relations",
			items.size(), relations.size() );
		rapidjson::Value obj( rapidjson::kObjectType );

		obj.AddMember( "visRelease", rapidjson::Value( visVersion.c_str(), allocator ).Move(), allocator );

		rapidjson::Value itemsArray( rapidjson::kArrayType );
		for ( const auto& item : items )
		{
			itemsArray.PushBack( item.toJson( allocator ), allocator );
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

		SPDLOG_INFO( "Successfully serialized GmodDto for VIS version {}", visVersion );
		return obj;
	}
}
