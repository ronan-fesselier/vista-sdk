#pragma once

#include "GmodVersioningDto.h"
#include "LocalId.h"

namespace dnv::vista::sdk
{
	class GmodNode;
	class GmodPath;
	class LocalIdBuilder;
	enum class VisVersion;

	/**
	 * @brief Class responsible for converting GMOD objects between different VIS versions
	 *
	 * GmodVersioning facilitates the conversion of GMOD (Generic Product Model)
	 * entities between different VIS (Vessel Information Structure) versions, ensuring
	 * compatibility and proper transformation of codes, paths, and local identifiers.
	 */
	class GmodVersioning final
	{
	private:
		/**
		 * @brief Enumeration of conversion operation types
		 *
		 * Defines the types of conversions that can be applied to GMOD nodes
		 * when converting between different VIS versions.
		 */
		enum class ConversionType
		{
			ChangeCode = 0,		   ///< Changes the code of a node
			Merge,				   ///< Merges two or more nodes into one
			Move,				   ///< Moves a node to a different parent
			AssignmentChange = 20, ///< Changes a node's assignment
			AssignmentDelete = 21  ///< Deletes a node's assignment
		};

		/**
		 * @brief Structure holding information about a node conversion
		 *
		 * Contains all the necessary data to convert a node from one VIS version
		 * to another, including source and target codes, assignment information, etc.
		 */
		struct GmodNodeConversion final
		{
			std::unordered_set<ConversionType> operations; ///< Operations to perform during conversion
			std::string source;							   ///< Source node code
			std::optional<std::string> target;			   ///< Target node code (if applicable)
			std::optional<std::string> oldAssignment;	   ///< Old assignment code (if applicable)
			std::optional<std::string> newAssignment;	   ///< New assignment code (if applicable)
			std::optional<bool> deleteAssignment;		   ///< Whether to delete assignment (if applicable)
		};

		/**
		 * @brief Class representing versioning information for a specific VIS version
		 *
		 * Contains conversion information for nodes in a specific VIS version,
		 * enabling lookup of how nodes should transform when moving to another version.
		 */
		class GmodVersioningNode final
		{
		public:
			/**
			 * @brief Constructor for GmodVersioningNode
			 *
			 * @param visVersion The VIS version this node belongs to
			 * @param dto Map of node codes to their conversion data
			 */
			GmodVersioningNode( VisVersion visVersion, const std::unordered_map<std::string, GmodNodeConversionDto>& dto );

			/**
			 * @brief Get the VIS version of this versioning node
			 * @return The VIS version
			 */
			VisVersion visVersion() const;

			/**
			 * @brief Try to get code changes for a specific node code
			 *
			 * @param code The node code to look up
			 * @param[out] nodeChanges Output parameter for the node changes if found
			 * @return True if changes were found, false otherwise
			 */
			bool tryGetCodeChanges( const std::string& code, GmodNodeConversion& nodeChanges ) const;

		private:
			VisVersion m_visVersion;													 ///< The VIS version of this versioning node
			std::unordered_map<std::string, GmodNodeConversion> m_versioningNodeChanges; ///< Map of node codes to their conversion information
		};

		std::unordered_map<VisVersion, GmodVersioningNode> m_versioningsMap; ///< Map of VIS versions to their versioning information

		/**
		 * @brief Parse a conversion type from a string
		 *
		 * @param type String representation of the conversion type
		 * @return The parsed ConversionType enum value
		 * @throws std::invalid_argument If the type string is unknown
		 */
		static ConversionType parseConversionType( const std::string& type );

		/**
		 * @brief Validate source and target versions
		 *
		 * @param sourceVersion Source VIS version
		 * @param targetVersion Target VIS version
		 * @throws std::invalid_argument If either version is invalid
		 */
		void validateSourceAndTargetVersions( VisVersion sourceVersion, VisVersion targetVersion ) const;

		/**
		 * @brief Validate source and target version pair
		 *
		 * Checks both versions are valid and not the same.
		 *
		 * @param sourceVersion Source VIS version
		 * @param targetVersion Target VIS version
		 * @throws std::invalid_argument If versions are invalid or the same
		 */
		void validateSourceAndTargetVersionPair( VisVersion sourceVersion, VisVersion targetVersion ) const;

		/**
		 * @brief Try to get a versioning node for a specific VIS version
		 *
		 * @param visVersion The VIS version to look up
		 * @param[out] versioningNode Output parameter for the versioning node if found
		 * @return True if the versioning node was found, false otherwise
		 */
		bool tryGetVersioningNode( VisVersion visVersion, GmodVersioningNode& versioningNode ) const;

		/**
		 * @brief Internal implementation for converting a node between adjacent versions
		 *
		 * @param sourceVersion Source VIS version
		 * @param sourceNode Source node to convert
		 * @param targetVersion Target VIS version
		 * @return The converted node, or an empty node if conversion failed
		 */
		GmodNode convertNodeInternal( VisVersion sourceVersion, const GmodNode& sourceNode, VisVersion targetVersion ) const;

	public:
		/**
		 * @brief Constructor
		 *
		 * @param dto Dictionary of GmodVersioningDto objects indexed by version string
		 */
		explicit GmodVersioning( const std::unordered_map<std::string, GmodVersioningDto>& dto );

		/**
		 * @brief Convert a GmodNode from one version to another
		 *
		 * @param sourceVersion Source VIS version
		 * @param sourceNode Source node
		 * @param targetVersion Target VIS version
		 * @return Converted node, or nullopt if conversion fails
		 */
		std::optional<GmodNode> convertNode( VisVersion sourceVersion, const GmodNode& sourceNode, VisVersion targetVersion ) const;

		/**
		 * @brief Convert a LocalIdBuilder from one version to another
		 *
		 * @param sourceLocalId Source LocalIdBuilder
		 * @param targetVersion Target VIS version
		 * @return Converted LocalIdBuilder, or nullopt if conversion fails
		 * @throws std::invalid_argument If the source LocalIdBuilder has no VIS version
		 */
		std::optional<LocalIdBuilder> convertLocalId( const LocalIdBuilder& sourceLocalId, VisVersion targetVersion ) const;

		/**
		 * @brief Convert a LocalId from one version to another
		 *
		 * @param sourceLocalId Source LocalId
		 * @param targetVersion Target VIS version
		 * @return Converted LocalId, or nullopt if conversion fails
		 */
		std::optional<LocalId> convertLocalId( const LocalId& sourceLocalId, VisVersion targetVersion ) const;

		/**
		 * @brief Convert a GmodPath from one version to another
		 *
		 * @param sourceVersion Source VIS version
		 * @param sourcePath Source path
		 * @param targetVersion Target VIS version
		 * @return Converted path, or nullopt if conversion fails
		 */
		std::optional<GmodPath> convertPath( VisVersion sourceVersion, const GmodPath& sourcePath, VisVersion targetVersion ) const;
	};
}
