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
	 * @brief Class responsible for converting GMOD objects to a higher version
	 */
	class GmodVersioning final
	{
	public:
		/**
		 * @brief Constructor
		 *
		 * @param dto Dictionary of GmodVersioningDto objects indexed by version string
		 */
		explicit GmodVersioning( const std::unordered_map<std::string, GmodVersioningDto>& dto );

		//-------------------------------------------------------------------------
		// Public Conversion Methods
		//-------------------------------------------------------------------------

		/**
		 * @brief Convert a GmodNode from one version to a higher version
		 * @throws std::invalid_argument If targetVersion is not higher than sourceVersion
		 */
		std::optional<GmodNode> convertNode( VisVersion sourceVersion, const GmodNode& sourceNode,
			VisVersion targetVersion ) const;

		/**
		 * @brief Convert a GmodPath from one version to a higher version
		 * @throws std::invalid_argument If targetVersion is not higher than sourceVersion
		 */
		std::optional<GmodPath> convertPath( VisVersion sourceVersion, const GmodPath& sourcePath,
			VisVersion targetVersion ) const;

		/**
		 * @brief Convert a LocalIdBuilder from one version to a higher version
		 */
		std::optional<LocalIdBuilder> convertLocalId( const LocalIdBuilder& sourceLocalId,
			VisVersion targetVersion ) const;

		/**
		 * @brief Convert a LocalId from one version to a higher version
		 */
		std::optional<LocalId> convertLocalId( const LocalId& sourceLocalId,
			VisVersion targetVersion ) const;

	private:
		//-------------------------------------------------------------------------
		// Private Type Definitions
		//-------------------------------------------------------------------------

		/**
		 * @brief Enumeration of conversion operation types
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
		 */
		class GmodVersioningNode final
		{
		public:
			/**
			 * @brief Constructor for GmodVersioningNode
			 */
			GmodVersioningNode( VisVersion visVersion,
				const std::unordered_map<std::string, GmodNodeConversionDto>& dto );

			/**
			 * @brief Get the VIS version of this versioning node
			 */
			VisVersion visVersion() const;

			/**
			 * @brief Try to get code changes for a specific node code
			 */
			bool tryGetCodeChanges( const std::string& code, GmodNodeConversion& nodeChanges ) const;

		private:
			VisVersion m_visVersion;
			std::unordered_map<std::string, GmodNodeConversion> m_versioningNodeChanges;
		};

		//-------------------------------------------------------------------------
		// Private Data Members
		//-------------------------------------------------------------------------

		std::unordered_map<VisVersion, GmodVersioningNode> m_versioningsMap;

		//-------------------------------------------------------------------------
		// Private Helper Methods
		//-------------------------------------------------------------------------

		/**
		 * @brief Internal implementation for converting a node between adjacent versions
		 */
		GmodNode convertNodeInternal( VisVersion sourceVersion, const GmodNode& sourceNode,
			VisVersion targetVersion ) const;

		/**
		 * @brief Try to get a versioning node for a specific VIS version
		 */
		bool tryGetVersioningNode( VisVersion visVersion, GmodVersioningNode& versioningNode ) const;

		//-------------------------------------------------------------------------
		// Private Validation Methods
		//-------------------------------------------------------------------------

		/**
		 * @brief Validate source and target versions
		 */
		void validateSourceAndTargetVersions( VisVersion sourceVersion, VisVersion targetVersion ) const;

		/**
		 * @brief Validate source and target version pair
		 */
		void validateSourceAndTargetVersionPair( VisVersion sourceVersion, VisVersion targetVersion ) const;

		//-------------------------------------------------------------------------
		// Private Static Utility Methods
		//-------------------------------------------------------------------------

		/**
		 * @brief Parse a conversion type from a string
		 */
		static ConversionType parseConversionType( const std::string& type );
	};
}
