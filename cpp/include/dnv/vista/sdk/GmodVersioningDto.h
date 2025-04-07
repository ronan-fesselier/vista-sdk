#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <rapidjson/document.h>

namespace dnv::vista::sdk
{
	/**
	 * @brief Data transfer object for GMOD node assignment changes
	 *
	 * Represents a change in assignment between VIS versions.
	 */
	struct GmodVersioningAssignmentChangeDto
	{
		/** @brief Previous assignment value (JSON: "oldAssignment") */
		std::string oldAssignment;

		/** @brief Current assignment value (JSON: "currentAssignment") */
		std::string currentAssignment;

		/**
		 * @brief Deserialize a GmodVersioningAssignmentChangeDto from a RapidJSON object
		 * @param json The RapidJSON object to deserialize
		 * @return The deserialized GmodVersioningAssignmentChangeDto
		 */
		static GmodVersioningAssignmentChangeDto FromJson( const rapidjson::Value& json )
		{
			GmodVersioningAssignmentChangeDto dto;
			dto.oldAssignment = json["oldAssignment"].GetString();
			dto.currentAssignment = json["currentAssignment"].GetString();
			return dto;
		}
	};

	/**
	 * @brief Data transfer object for GMOD node conversion information
	 *
	 * Contains instructions for converting a node between VIS versions.
	 */
	struct GmodNodeConversionDto
	{
		/** @brief Set of operations to apply (JSON: "operations") */
		std::unordered_set<std::string> operations;

		/** @brief Source node code (JSON: "source") */
		std::string source;

		/** @brief Target node code (JSON: "target") */
		std::string target;

		/** @brief Old assignment code (JSON: "oldAssignment") */
		std::string oldAssignment;

		/** @brief New assignment code (JSON: "newAssignment") */
		std::string newAssignment;

		/** @brief Whether to delete assignment (JSON: "deleteAssignment") */
		bool deleteAssignment = false;

		/**
		 * @brief Deserialize a GmodNodeConversionDto from a RapidJSON object
		 * @param json The RapidJSON object to deserialize
		 * @return The deserialized GmodNodeConversionDto
		 */
		static GmodNodeConversionDto FromJson( const rapidjson::Value& json )
		{
			GmodNodeConversionDto dto;

			if ( json.HasMember( "operations" ) && json["operations"].IsArray() )
			{
				for ( const auto& operation : json["operations"].GetArray() )
				{
					dto.operations.insert( operation.GetString() );
				}
			}

			dto.source = json["source"].GetString();
			dto.target = json["target"].GetString();
			dto.oldAssignment = json["oldAssignment"].GetString();
			dto.newAssignment = json["newAssignment"].GetString();
			dto.deleteAssignment = json["deleteAssignment"].GetBool();

			return dto;
		}
	};

	/**
	 * @brief Data transfer object for GMOD version conversion information
	 *
	 * Contains all node conversion information for a specific VIS version.
	 */
	struct GmodVersioningDto
	{
		/** @brief VIS version identifier (JSON: "visRelease") */
		std::string visVersion;

		/** @brief Map of node codes to their conversion information (JSON: "items") */
		std::unordered_map<std::string, GmodNodeConversionDto> items;

		/**
		 * @brief Deserialize a GmodVersioningDto from a RapidJSON object
		 * @param json The RapidJSON object to deserialize
		 * @return The deserialized GmodVersioningDto
		 */
		static GmodVersioningDto FromJson( const rapidjson::Value& json )
		{
			GmodVersioningDto dto;

			dto.visVersion = json["visRelease"].GetString();

			if ( json.HasMember( "items" ) && json["items"].IsObject() )
			{
				for ( auto it = json["items"].MemberBegin(); it != json["items"].MemberEnd(); ++it )
				{
					dto.items[it->name.GetString()] = GmodNodeConversionDto::FromJson( it->value );
				}
			}

			return dto;
		}
	};
}
