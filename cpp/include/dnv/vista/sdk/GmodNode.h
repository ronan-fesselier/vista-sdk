#pragma once

#include "GmodDto.h"
#include "Locations.h"

namespace dnv::vista::sdk
{
	enum class VisVersion;
	class ParsingErrors;

	/**
	 * @brief Metadata for a GMOD (Generic Marine Object Dictionary) node
	 *
	 * Contains descriptive information about a node including its category, type,
	 * name, and other optional properties as defined in ISO 19848.
	 */
	class GmodNodeMetadata
	{
	public:
		/**
		 * @brief Construct a new GmodNodeMetadata object
		 *
		 * @param category The category classification of the node (e.g., "PRODUCT", "ASSET")
		 * @param type The type classification within the category (e.g., "SELECTION", "TYPE")
		 * @param name The human-readable name of the node
		 * @param commonName Optional common name or alias for the node
		 * @param definition Optional detailed definition of the node
		 * @param commonDefinition Optional common definition for the node
		 * @param installSubstructure Optional flag indicating if substructure should be installed
		 * @param normalAssignmentNames Optional mapping of normal assignment names
		 */
		GmodNodeMetadata(
			const std::string& category,
			const std::string& type,
			const std::string& name,
			const std::optional<std::string>& commonName = std::nullopt,
			const std::optional<std::string>& definition = std::nullopt,
			const std::optional<std::string>& commonDefinition = std::nullopt,
			const std::optional<bool>& installSubstructure = std::nullopt,
			const std::unordered_map<std::string, std::string>& normalAssignmentNames = {} );

		/**
		 * @brief Get the category of the node
		 * @return Reference to the category string
		 */
		const std::string& GetCategory() const;

		/**
		 * @brief Get the type of the node
		 * @return Reference to the type string
		 */
		const std::string& GetType() const;

		/**
		 * @brief Get the name of the node
		 * @return Reference to the name string
		 */
		const std::string& GetName() const;

		/**
		 * @brief Get the common name of the node
		 * @return Optional reference to the common name
		 */
		const std::optional<std::string>& GetCommonName() const;

		/**
		 * @brief Get the definition of the node
		 * @return Optional reference to the definition
		 */
		const std::optional<std::string>& GetDefinition() const;

		/**
		 * @brief Get the common definition of the node
		 * @return Optional reference to the common definition
		 */
		const std::optional<std::string>& GetCommonDefinition() const;

		/**
		 * @brief Get the install substructure flag
		 * @return Optional reference to the install substructure flag
		 */
		const std::optional<bool>& GetInstallSubstructure() const;

		/**
		 * @brief Get the normal assignment names mapping
		 * @return Reference to the map of normal assignment names
		 */
		const std::unordered_map<std::string, std::string>& GetNormalAssignmentNames() const;

		/**
		 * @brief Get the full type string (category + type)
		 * @return Reference to the full type string
		 */
		const std::string& GetFullType() const;

		/**
		 * @brief Equality comparison operator
		 * @param other The metadata to compare with
		 * @return True if all properties are equal
		 */
		bool operator==( const GmodNodeMetadata& other ) const;

		/**
		 * @brief Inequality comparison operator
		 * @param other The metadata to compare with
		 * @return True if any property differs
		 */
		bool operator!=( const GmodNodeMetadata& other ) const;

	private:
		std::string m_category;												  ///< Category classification
		std::string m_type;													  ///< Type classification
		std::string m_name;													  ///< Human-readable name
		std::optional<std::string> m_commonName;							  ///< Optional common name/alias
		std::optional<std::string> m_definition;							  ///< Optional detailed definition
		std::optional<std::string> m_commonDefinition;						  ///< Optional common definition
		std::optional<bool> m_installSubstructure;							  ///< Optional installation flag
		std::unordered_map<std::string, std::string> m_normalAssignmentNames; ///< Assignment name mapping
		std::string m_fullType;												  ///< Combined category and type
	};

	/**
	 * @brief A node in the Generic Marine Object Dictionary (GMOD)
	 *
	 * Represents a single node in the hierarchical structure of the Generic Marine Object
	 * Dictionary as defined by ISO 19848. Contains metadata, relationships to parent/child
	 * nodes, and optional location information.
	 */
	class GmodNode
	{
	public:
		/**
		 * @brief Default constructor
		 *
		 * Creates an empty node with default values and no relationships
		 */
		GmodNode();

		/**
		 * @brief Construct from DTO
		 *
		 * @param version The VIS version
		 * @param dto The data transfer object containing node information
		 */
		GmodNode( VisVersion version, const GmodNodeDto& dto );

		/**
		 * @brief Copy constructor (default)
		 */
		GmodNode( const GmodNode& ) = default;

		/**
		 * @brief Copy assignment operator (default)
		 */
		GmodNode& operator=( const GmodNode& ) = default;

		/**
		 * @brief Move constructor (default)
		 */
		GmodNode( GmodNode&& ) noexcept = default;

		/**
		 * @brief Move assignment operator (default)
		 */
		GmodNode& operator=( GmodNode&& ) noexcept = default;

		/**
		 * @brief Destructor (default)
		 */
		~GmodNode() = default;

		/**
		 * @brief Get the unique code identifier
		 * @return Reference to the code string
		 */
		const std::string& GetCode() const;

		/**
		 * @brief Get the location information
		 * @return Optional reference to the location
		 */
		const std::optional<Location>& GetLocation() const;

		/**
		 * @brief Get the VIS version
		 * @return The VIS version enum value
		 */
		VisVersion GetVisVersion() const;

		/**
		 * @brief Get the node metadata
		 * @return Reference to the metadata
		 */
		const GmodNodeMetadata& GetMetadata() const;

		/**
		 * @brief Get the child nodes
		 * @return Reference to the vector of child node pointers
		 */
		const std::vector<GmodNode*>& GetChildren() const;

		/**
		 * @brief Get the parent nodes
		 * @return Reference to the vector of parent node pointers
		 */
		const std::vector<GmodNode*>& GetParents() const;

		/**
		 * @brief Create a copy of this node without location information
		 * @return A new GmodNode with location removed
		 */
		GmodNode WithoutLocation() const;

		/**
		 * @brief Create a copy of this node with the specified location
		 *
		 * @param location The location string to add
		 * @return A new GmodNode with the specified location
		 * @throws std::invalid_argument If the location string is invalid
		 */
		GmodNode WithLocation( const std::string& location ) const;

		/**
		 * @brief Try to create a copy of this node with the specified location
		 *
		 * @param locationStr The location string to add
		 * @return A new GmodNode with the location if valid, otherwise a copy of this node
		 */
		GmodNode TryWithLocation( const std::string& locationStr ) const;

		/**
		 * @brief Try to create a copy of this node with the specified location, capturing errors
		 *
		 * @param locationStr The location string to add
		 * @param errors Object to receive parsing errors
		 * @return A new GmodNode with the location if valid, otherwise a copy of this node
		 */
		GmodNode TryWithLocation( const std::string& locationStr, ParsingErrors& errors ) const;

		/**
		 * @brief Create a copy of this node with the specified location object
		 *
		 * @param location The location object to add
		 * @return A new GmodNode with the specified location
		 */
		GmodNode WithLocation( const Location& location ) const;

		/**
		 * @brief Try to create a copy of this node with the optional location
		 *
		 * @param location The optional location to add
		 * @return A new GmodNode with the location if provided, otherwise a copy of this node
		 */
		GmodNode TryWithLocation( const std::optional<Location>& location ) const;

		/**
		 * @brief Check if the node is individualizable
		 *
		 * A node is individualizable if it can be assigned a unique instance identifier.
		 *
		 * @param isTargetNode Flag indicating if this is a target node
		 * @param isInSet Flag indicating if this node is in a set
		 * @return True if the node is individualizable
		 */
		bool IsIndividualizable( bool isTargetNode = false, bool isInSet = false ) const;

		/**
		 * @brief Check if the node is a function composition
		 * @return True if the node is an asset or product function composition
		 */
		bool IsFunctionComposition() const;

		/**
		 * @brief Check if the node is mappable
		 * @return True if the node can be mapped to other nodes
		 */
		bool IsMappable() const;

		/**
		 * @brief Check if the node is a product selection
		 * @return True if the node is a product selection
		 */
		bool IsProductSelection() const;

		/**
		 * @brief Check if the node is a product type
		 * @return True if the node is a product type
		 */
		bool IsProductType() const;

		/**
		 * @brief Check if the node is an asset
		 * @return True if the node is an asset
		 */
		bool IsAsset() const;

		/**
		 * @brief Check if the node is a leaf node
		 * @return True if the node is a leaf node in the hierarchy
		 */
		bool IsLeafNode() const;

		/**
		 * @brief Check if the node is a function node
		 * @return True if the node is a function node
		 */
		bool IsFunctionNode() const;

		/**
		 * @brief Check if the node is an asset function node
		 * @return True if the node is an asset function node
		 */
		bool IsAssetFunctionNode() const;

		/**
		 * @brief Check if the node is the root node
		 * @return True if the node is the root of the hierarchy (code is "VE")
		 */
		bool IsRoot() const;

		/**
		 * @brief Get the product type node associated with this node
		 * @return Pointer to the product type node, or nullptr if not applicable
		 */
		const GmodNode* ProductType() const;

		/**
		 * @brief Get the product selection node associated with this node
		 * @return Pointer to the product selection node, or nullptr if not applicable
		 */
		const GmodNode* ProductSelection() const;

		/**
		 * @brief Check if this node has a specific child node
		 *
		 * @param node The potential child node
		 * @return True if the node is a child of this node
		 */
		bool IsChild( const GmodNode& node ) const;

		/**
		 * @brief Check if this node has a child with a specific code
		 *
		 * @param code The code to check for
		 * @return True if a child with the specified code exists
		 */
		bool IsChild( const std::string& code ) const;

		/**
		 * @brief Equality comparison operator
		 *
		 * Nodes are considered equal if they have the same code and location
		 *
		 * @param other The node to compare with
		 * @return True if nodes are equal
		 */
		bool operator==( const GmodNode& other ) const;

		/**
		 * @brief Inequality comparison operator
		 *
		 * @param other The node to compare with
		 * @return True if nodes are not equal
		 */
		bool operator!=( const GmodNode& other ) const;

		/**
		 * @brief Convert the node to a string representation
		 * @return String representation in the format "code[-location]"
		 */
		std::string ToString() const;

		/**
		 * @brief Write the node's string representation to a string stream
		 * @param builder The string stream to write to
		 */
		void ToString( std::stringstream& builder ) const;

		/**
		 * @brief Add a child node
		 * @param child Pointer to the child node to add
		 */
		void AddChild( GmodNode* child );

		/**
		 * @brief Add a parent node
		 * @param parent Pointer to the parent node to add
		 */
		void AddParent( GmodNode* parent );

		/**
		 * @brief Trim excess capacity and build child code index
		 *
		 * Optimizes memory usage and builds an index of child codes for fast lookups.
		 */
		void Trim();

	private:
		std::string m_code;							   ///< Unique identifier code
		std::optional<Location> m_location;			   ///< Optional location information
		VisVersion m_visVersion;					   ///< VIS version of this node
		GmodNodeMetadata m_metadata;				   ///< Node metadata
		std::vector<GmodNode*> m_children;			   ///< Child nodes
		std::vector<GmodNode*> m_parents;			   ///< Parent nodes
		std::unordered_set<std::string> m_childrenSet; ///< Set of child codes for fast lookups
	};
}
