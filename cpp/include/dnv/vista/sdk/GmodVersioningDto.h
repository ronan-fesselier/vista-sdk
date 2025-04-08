#pragma once

namespace dnv::vista::sdk
{
	/**
	 * @brief Data transfer object for GMOD node assignment changes
	 *
	 * Represents a change in assignment between VIS versions.
	 * Maps to C# record: GmodVersioningAssignmentChangeDto(string OldAssignment, string CurrentAssignment)
	 */
	struct GmodVersioningAssignmentChangeDto
	{
		/** @brief Previous assignment value (JSON: "oldAssignment") */
		std::string oldAssignment;

		/** @brief Current assignment value (JSON: "currentAssignment") */
		std::string currentAssignment;

		/**
		 * @brief Default constructor
		 */
		GmodVersioningAssignmentChangeDto() = default;

		/**
		 * @brief Constructor with parameters
		 * @param oldAssignment Previous assignment value
		 * @param currentAssignment Current assignment value
		 */
		GmodVersioningAssignmentChangeDto(
			std::string oldAssignment,
			std::string currentAssignment )
			: oldAssignment( std::move( oldAssignment ) ),
			  currentAssignment( std::move( currentAssignment ) ) {}

		/**
		 * @brief Deserialize a GmodVersioningAssignmentChangeDto from a RapidJSON object
		 * @param json The RapidJSON object to deserialize
		 * @return The deserialized GmodVersioningAssignmentChangeDto
		 */
		static GmodVersioningAssignmentChangeDto FromJson( const rapidjson::Value& json );

		/**
		 * @brief Serialize this GmodVersioningAssignmentChangeDto to a RapidJSON Value
		 * @param allocator The JSON value allocator to use
		 * @return The serialized JSON value
		 */
		rapidjson::Value ToJson( rapidjson::Document::AllocatorType& allocator ) const;
	};

	/**
	 * @brief Data transfer object for GMOD node conversion information
	 *
	 * Contains instructions for converting a node between VIS versions.
	 * Maps to C# record: GmodNodeConversionDto(HashSet<string> Operations, string Source, string Target,
	 *                                          string OldAssignment, string NewAssignment, bool DeleteAssignment)
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
		 * @brief Default constructor
		 */
		GmodNodeConversionDto() = default;

		/**
		 * @brief Constructor with parameters
		 * @param operations Set of operations to apply
		 * @param source Source node code
		 * @param target Target node code
		 * @param oldAssignment Old assignment code
		 * @param newAssignment New assignment code
		 * @param deleteAssignment Whether to delete assignment
		 */
		GmodNodeConversionDto(
			std::unordered_set<std::string> operations,
			std::string source,
			std::string target,
			std::string oldAssignment,
			std::string newAssignment,
			bool deleteAssignment )
			: operations( std::move( operations ) ),
			  source( std::move( source ) ),
			  target( std::move( target ) ),
			  oldAssignment( std::move( oldAssignment ) ),
			  newAssignment( std::move( newAssignment ) ),
			  deleteAssignment( deleteAssignment ) {}

		/**
		 * @brief Deserialize a GmodNodeConversionDto from a RapidJSON object
		 * @param json The RapidJSON object to deserialize
		 * @return The deserialized GmodNodeConversionDto
		 */
		static GmodNodeConversionDto FromJson( const rapidjson::Value& json );

		/**
		 * @brief Serialize this GmodNodeConversionDto to a RapidJSON Value
		 * @param allocator The JSON value allocator to use
		 * @return The serialized JSON value
		 */
		rapidjson::Value ToJson( rapidjson::Document::AllocatorType& allocator ) const;
	};

	/**
	 * @brief Data transfer object for GMOD version conversion information
	 *
	 * Contains all node conversion information for a specific VIS version.
	 * Maps to C# record: GmodVersioningDto(string VisVersion, IReadOnlyDictionary<string, GmodNodeConversionDto> Items)
	 */
	struct GmodVersioningDto
	{
		/** @brief VIS version identifier (JSON: "visRelease") */
		std::string visVersion;

		/** @brief Map of node codes to their conversion information (JSON: "items") */
		std::unordered_map<std::string, GmodNodeConversionDto> items;

		/**
		 * @brief Default constructor
		 */
		GmodVersioningDto() = default;

		/**
		 * @brief Constructor with parameters
		 * @param visVersion VIS version identifier
		 * @param items Map of node codes to conversion information
		 */
		GmodVersioningDto(
			std::string visVersion,
			std::unordered_map<std::string, GmodNodeConversionDto> items )
			: visVersion( std::move( visVersion ) ),
			  items( std::move( items ) ) {}

		/**
		 * @brief Deserialize a GmodVersioningDto from a RapidJSON object
		 * @param json The RapidJSON object to deserialize
		 * @return The deserialized GmodVersioningDto
		 */
		static GmodVersioningDto FromJson( const rapidjson::Value& json );

		/**
		 * @brief Serialize this GmodVersioningDto to a RapidJSON Value
		 * @param allocator The JSON value allocator to use
		 * @return The serialized JSON value
		 */
		rapidjson::Value ToJson( rapidjson::Document::AllocatorType& allocator ) const;
	};
}
