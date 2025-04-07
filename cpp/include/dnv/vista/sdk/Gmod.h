#pragma once

#include "ChdDictionary.h"
#include "GmodNode.h"

namespace dnv::vista::sdk
{
	class GmodPath;
	struct GmodDto;
	enum class VisVersion;

	/**
	 * @brief Main GMOD (Generic Marine Object Dictionary) class
	 *
	 * Represents the Generic Marine Object Dictionary from ISO 19848,
	 * containing a hierarchical structure of nodes with parent-child relationships.
	 */
	class Gmod
	{
	public:
		/**
		 * @brief Enumeration for traversal handler results
		 */
		enum class TraversalHandlerResult
		{
			Continue,	 ///< Continue traversal
			SkipSubtree, ///< Skip current subtree but continue traversal
			Stop		 ///< Stop traversal completely
		};

	public:
		/**
		 * @brief Default constructor
		 */
		Gmod() = default;

		/**
		 * @brief Constructor using GMOD DTO
		 * @param version VIS version
		 * @param dto GMOD data transfer object
		 */
		Gmod( VisVersion version, const GmodDto& dto );

		/**
		 * @brief Constructor using node map
		 * @param version VIS version
		 * @param nodeMap Dictionary of nodes
		 */
		Gmod( VisVersion version, const std::unordered_map<std::string, GmodNode>& nodeMap );

		/**
		 * @brief Get the VIS version
		 * @return The VIS version
		 */
		VisVersion GetVisVersion() const;

		/**
		 * @brief Get the root node
		 * @return The root node
		 */
		const GmodNode& GetRootNode() const;

		/**
		 * @brief Access a node by its code
		 * @param key The code to look up
		 * @return The node
		 * @throws std::out_of_range if node not found
		 */
		const GmodNode& operator[]( const std::string& key ) const;

		/**
		 * @brief Try to get a node by its code
		 * @param code The code to look up
		 * @param[out] node The node if found
		 * @return true if found, false otherwise
		 */
		bool TryGetNode( const std::string& code, GmodNode& node ) const;

		/**
		 * @brief Try to get a node by its code
		 * @param code The code to look up as string_view
		 * @param[out] node The node if found
		 * @return true if found, false otherwise
		 */
		bool TryGetNode( std::string_view code, GmodNode& node ) const;

		/**
		 * @brief Parse a path
		 * @param item The path string
		 * @return The parsed path
		 * @throws std::invalid_argument if parsing fails
		 */
		GmodPath ParsePath( const std::string& item ) const;

		/**
		 * @brief Try to parse a path
		 * @param item The path string
		 * @param[out] path The parsed path if successful
		 * @return true if parsing succeeded, false otherwise
		 */
		bool TryParsePath( const std::string& item, std::optional<GmodPath>& path ) const;

		/**
		 * @brief Parse a full path
		 * @param item The full path string
		 * @return The parsed path
		 * @throws std::invalid_argument if parsing fails
		 */
		GmodPath ParseFromFullPath( const std::string& item ) const;

		/**
		 * @brief Try to parse a full path
		 * @param item The full path string
		 * @param[out] path The parsed path if successful
		 * @return true if parsing succeeded, false otherwise
		 */
		bool TryParseFromFullPath( const std::string& item, std::optional<GmodPath>& path ) const;

		/**
		 * @brief Check if a type is a potential parent
		 * @param type The type to check
		 * @return true if potential parent, false otherwise
		 */
		static bool IsPotentialParent( const std::string& type );

		/**
		 * @brief Check if a full type is a leaf node
		 * @param fullType The full type to check
		 * @return true if leaf node, false otherwise
		 */
		static bool IsLeafNode( const std::string& fullType );

		/**
		 * @brief Check if metadata represents a leaf node
		 * @param metadata The node metadata
		 * @return true if leaf node, false otherwise
		 */
		static bool IsLeafNode( const GmodNodeMetadata& metadata );

		/**
		 * @brief Check if category represents a function node
		 * @param category The category to check
		 * @return true if function node, false otherwise
		 */
		static bool IsFunctionNode( const std::string& category );

		/**
		 * @brief Check if metadata represents a function node
		 * @param metadata The node metadata
		 * @return true if function node, false otherwise
		 */
		static bool IsFunctionNode( const GmodNodeMetadata& metadata );

		/**
		 * @brief Check if metadata represents a product selection
		 * @param metadata The node metadata
		 * @return true if product selection, false otherwise
		 */
		static bool IsProductSelection( const GmodNodeMetadata& metadata );

		/**
		 * @brief Check if metadata represents a product type
		 * @param metadata The node metadata
		 * @return true if product type, false otherwise
		 */
		static bool IsProductType( const GmodNodeMetadata& metadata );

		/**
		 * @brief Check if metadata represents an asset
		 * @param metadata The node metadata
		 * @return true if asset, false otherwise
		 */
		static bool IsAsset( const GmodNodeMetadata& metadata );

		/**
		 * @brief Check if metadata represents an asset function node
		 * @param metadata The node metadata
		 * @return true if asset function node, false otherwise
		 */
		static bool IsAssetFunctionNode( const GmodNodeMetadata& metadata );

		/**
		 * @brief Check if parent-child relation represents a product type assignment
		 * @param parent The parent node
		 * @param child The child node
		 * @return true if product type assignment, false otherwise
		 */
		static bool IsProductTypeAssignment( const GmodNode* parent, const GmodNode* child );

		/**
		 * @brief Check if parent-child relation represents a product selection assignment
		 * @param parent The parent node
		 * @param child The child node
		 * @return true if product selection assignment, false otherwise
		 */
		static bool IsProductSelectionAssignment( const GmodNode* parent, const GmodNode* child );

	private:
		VisVersion m_visVersion;
		GmodNode m_rootNode;
		ChdDictionary<GmodNode> m_nodeMap;

		static const inline std::vector<std::string> s_potentialParentScopeTypes = { "SELECTION", "GROUP", "LEAF" };
		static const inline std::vector<std::string> s_leafTypes = { "ASSET FUNCTION LEAF", "PRODUCT FUNCTION LEAF" };

	public:
		/**
		 * @brief Iterator for traversing GMOD nodes
		 */
		class Iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = GmodNode;
			using difference_type = std::ptrdiff_t;
			using pointer = const GmodNode*;
			using reference = const GmodNode&;

			/**
			 * @brief Default constructor
			 */
			Iterator() = default;

			/**
			 * @brief Constructor from ChdDictionary iterator
			 * @param innerIt The inner dictionary iterator
			 */
			Iterator( ChdDictionary<GmodNode>::Iterator innerIt );

			/**
			 * @brief Dereference operator
			 * @return Reference to the current node
			 */
			reference operator*() const;

			/**
			 * @brief Arrow operator
			 * @return Pointer to the current node
			 */
			pointer operator->() const;

			/**
			 * @brief Pre-increment operator
			 * @return Reference to this iterator after increment
			 */
			Iterator& operator++();

			/**
			 * @brief Post-increment operator
			 * @return Copy of iterator before increment
			 */
			Iterator operator++( int );

			/**
			 * @brief Equality comparison
			 * @param other Iterator to compare with
			 * @return True if iterators are equal
			 */
			bool operator==( const Iterator& other ) const;

			/**
			 * @brief Inequality comparison
			 * @param other Iterator to compare with
			 * @return True if iterators are not equal
			 */
			bool operator!=( const Iterator& other ) const;

		private:
			ChdDictionary<GmodNode>::Iterator m_innerIt;
		};

		/**
		 * @brief Get iterator to the beginning
		 * @return Iterator to the first node
		 */
		Iterator begin() const;

		/**
		 * @brief Get iterator to the end
		 * @return Iterator past the last node
		 */
		Iterator end() const;
	};
}
