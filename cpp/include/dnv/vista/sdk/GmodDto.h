#pragma once

namespace dnv::vista::sdk
{
	/**
	 * @brief Data transfer object for a GMOD (Generic Marine Object Dictionary) node
	 *
	 * Represents a node in the Generic Marine Object Dictionary as defined by ISO 19848.
	 * Contains all metadata associated with a node including its category, type, code, name,
	 * and optional attributes.
	 * Maps to C# record: GmodNodeDto(string Category, string Type, string Code, string Name, string? CommonName, ...)
	 */
	struct GmodNodeDto
	{
		/**
		 * @brief Default constructor
		 */
		GmodNodeDto() = default;

		/**
		 * @brief Constructor with parameters
		 *
		 * @param category The category classification
		 * @param type The type classification
		 * @param code The unique code identifier
		 * @param name The human-readable name
		 * @param commonName Optional common name/alias
		 * @param definition Optional detailed definition
		 * @param commonDefinition Optional common definition
		 * @param installSubstructure Optional installation flag
		 * @param normalAssignmentNames Optional assignment name mapping
		 */
		GmodNodeDto(
			std::string category,
			std::string type,
			std::string code,
			std::string name,
			std::optional<std::string> commonName = std::nullopt,
			std::optional<std::string> definition = std::nullopt,
			std::optional<std::string> commonDefinition = std::nullopt,
			std::optional<bool> installSubstructure = std::nullopt,
			std::optional<std::unordered_map<std::string, std::string>> normalAssignmentNames = std::nullopt )
			: category( std::move( category ) ),
			  type( std::move( type ) ),
			  code( std::move( code ) ),
			  name( std::move( name ) ),
			  commonName( std::move( commonName ) ),
			  definition( std::move( definition ) ),
			  commonDefinition( std::move( commonDefinition ) ),
			  installSubstructure( installSubstructure ),
			  normalAssignmentNames( std::move( normalAssignmentNames ) ) {}

		/**
		 * @brief Deserialize a GmodNodeDto from a RapidJSON object
		 * @param json The RapidJSON object to deserialize
		 * @return The deserialized GmodNodeDto
		 */
		static GmodNodeDto FromJson( const rapidjson::Value& json );

		/**
		 * @brief Try to deserialize a GmodNodeDto from a RapidJSON object
		 * @param json The RapidJSON object to deserialize
		 * @param dto Output parameter to receive the deserialized object
		 * @return True if deserialization was successful, false otherwise
		 */
		static bool TryFromJson( const rapidjson::Value& json, GmodNodeDto& dto );

		/**
		 * @brief Serialize this GmodNodeDto to a RapidJSON Value
		 * @param allocator The JSON value allocator to use
		 * @return The serialized JSON value
		 */
		rapidjson::Value ToJson( rapidjson::Document::AllocatorType& allocator ) const;

		/** @brief Category classification of the node (e.g., "PRODUCT", "ASSET") */
		std::string category;

		/** @brief Type classification within the category (e.g., "SELECTION", "TYPE") */
		std::string type;

		/** @brief Unique code identifier for the node */
		std::string code;

		/** @brief Human-readable name of the node */
		std::string name;

		/** @brief Optional common name or alias */
		std::optional<std::string> commonName;

		/** @brief Optional detailed definition */
		std::optional<std::string> definition;

		/** @brief Optional common definition */
		std::optional<std::string> commonDefinition;

		/** @brief Optional installation flag */
		std::optional<bool> installSubstructure;

		/** @brief Optional mapping of normal assignment names */
		std::optional<std::unordered_map<std::string, std::string>> normalAssignmentNames;
	};

	/**
	 * @brief Data transfer object for a complete GMOD (Generic Marine Object Dictionary)
	 *
	 * Represents the entire Generic Marine Object Dictionary for a specific VIS version,
	 * containing all nodes and their relationships as defined in ISO 19848.
	 * Maps to C# record: GmodDto(string VisVersion, GmodNodeDto[] Items, string[][] Relations)
	 */
	struct GmodDto
	{
		/**
		 * @brief Default constructor
		 */
		GmodDto() = default;

		/**
		 * @brief Constructor with parameters
		 *
		 * @param visVersion The VIS version string
		 * @param items Collection of GMOD node DTOs
		 * @param relations Collection of relationships between nodes
		 */
		GmodDto(
			std::string visVersion,
			std::vector<GmodNodeDto> items,
			std::vector<std::vector<std::string>> relations )
			: visVersion( std::move( visVersion ) ),
			  items( std::move( items ) ),
			  relations( std::move( relations ) ) {}

		/**
		 * @brief Deserialize a GmodDto from a RapidJSON object
		 * @param json The RapidJSON object to deserialize
		 * @return The deserialized GmodDto
		 */
		static GmodDto FromJson( const rapidjson::Value& json );

		/**
		 * @brief Try to deserialize a GmodDto from a RapidJSON object
		 * @param json The RapidJSON object to deserialize
		 * @param dto Output parameter to receive the deserialized object
		 * @return True if deserialization was successful, false otherwise
		 */
		static bool TryFromJson( const rapidjson::Value& json, GmodDto& dto );

		/**
		 * @brief Serialize this GmodDto to a RapidJSON Value
		 * @param allocator The JSON value allocator to use
		 * @return The serialized JSON value
		 */
		rapidjson::Value ToJson( rapidjson::Document::AllocatorType& allocator ) const;

		/** @brief VIS version string (e.g., "3.8a") */
		std::string visVersion;

		/** @brief Collection of GMOD node DTOs */
		std::vector<GmodNodeDto> items;

		/** @brief Collection of relationships between nodes */
		std::vector<std::vector<std::string>> relations;
	};
}
