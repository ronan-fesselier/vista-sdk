#pragma once

#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <rapidjson/document.h>

namespace dnv::vista::sdk
{
	/**
	 * @brief Data transfer object for a GMOD (Generic Marine Object Dictionary) node
	 *
	 * Represents a node in the Generic Marine Object Dictionary as defined by ISO 19848.
	 * Contains all metadata associated with a node including its category, type, code, name,
	 * and optional attributes.
	 */

	struct GmodNodeDto
	{
		std::string category;
		std::string type;
		std::string code;
		std::string name;
		std::optional<std::string> commonName;
		std::optional<std::string> definition;
		std::optional<std::string> commonDefinition;
		std::optional<bool> installSubstructure;
		std::optional<std::unordered_map<std::string, std::string>> normalAssignmentNames;

		static GmodNodeDto FromJson( const rapidjson::Value& json )
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
	};
	/**
	 * @brief Data transfer object for a complete GMOD (Generic Marine Object Dictionary)
	 *
	 * Represents the entire Generic Marine Object Dictionary for a specific VIS version,
	 * containing all nodes and their relationships as defined in ISO 19848.
	 */

	struct GmodDto
	{
		std::string visVersion;
		std::vector<GmodNodeDto> items;
		std::vector<std::vector<std::string>> relations;

		static GmodDto FromJson( const rapidjson::Value& json )
		{
			GmodDto dto;

			dto.visVersion = "unknown";

			if ( json.HasMember( "visRelease" ) && json["visRelease"].IsString() )
				dto.visVersion = json["visRelease"].GetString();

			if ( json.HasMember( "items" ) && json["items"].IsArray() )
			{
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
				for ( const auto& relation : json["relations"].GetArray() )
				{
					if ( relation.IsArray() )
					{
						std::vector<std::string> relationPair;
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
	};
}
