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
			Stop,		 ///< Stop traversal completely
			SkipSubtree, ///< Skip current subtree but continue traversal
			Continue	 ///< Continue traversal
		};

		/**
		 * @brief Options for traversal operations
		 */
		struct TraversalOptions
		{
			static constexpr int DEFAULT_MAX_TRAVERSAL_OCCURRENCE = 1;
			int MaxOccurrence;

			TraversalOptions() : MaxOccurrence( DEFAULT_MAX_TRAVERSAL_OCCURRENCE ) {}
			explicit TraversalOptions( int maxOccurrence ) : MaxOccurrence( maxOccurrence ) {}
		};

		using TraverseHandler = std::function<TraversalHandlerResult( const std::vector<GmodNode>&, const GmodNode& )>;

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
		 * @brief Traverse the GMOD hierarchy
		 * @param handler The traversal handler function
		 * @param options Optional traversal options
		 * @return true if traversal completed, false if stopped early
		 */
		bool Traverse( const TraverseHandler& handler, const TraversalOptions& options = {} ) const;

		/**
		 * @brief Traverse the GMOD hierarchy from a specific node
		 * @param startNode The node to start traversal from
		 * @param handler The traversal handler function
		 * @param options Optional traversal options
		 * @return true if traversal completed, false if stopped early
		 */
		bool TraverseFrom(
			const GmodNode& startNode,
			const TraverseHandler& handler,
			const TraversalOptions& options = {} ) const;

		/**
		 * @brief Traverse the GMOD hierarchy with state
		 * @tparam TState Type of the state object
		 * @param state State object passed to the handler
		 * @param handler The traversal handler function that receives state
		 * @param options Optional traversal options
		 * @return true if traversal completed, false if stopped early
		 */
		template <typename TState>
		bool Traverse(
			TState& state,
			const std::function<TraversalHandlerResult( TState&, const std::vector<GmodNode>&, const GmodNode& )>& handler,
			const TraversalOptions& options = {} ) const
		{
			return TraverseFrom( GetRootNode(), state, handler, options );
		}

		/**
		 * @brief Traverse the GMOD hierarchy from a specific node with state
		 * @tparam TState Type of the state object
		 * @param startNode The node to start traversal from
		 * @param state State object passed to the handler
		 * @param handler The traversal handler function that receives state
		 * @param options Optional traversal options
		 * @return true if traversal completed, false if stopped early
		 */
		template <typename TState>
		bool TraverseFrom(
			const GmodNode& startNode,
			TState& state,
			const std::function<TraversalHandlerResult( TState&, const std::vector<GmodNode>&, const GmodNode& )>& handler,
			const TraversalOptions& options = {} ) const;

		/**
		 * @brief Check if a path exists between nodes
		 * @param fromPath The starting path
		 * @param to The destination node
		 * @param[out] remainingParents Parents that remain after the common path
		 * @return true if path exists, false otherwise
		 */
		bool PathExistsBetween(
			const std::vector<GmodNode>& fromPath,
			const GmodNode& to,
			std::vector<GmodNode>& remainingParents ) const;

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

		class Parents
		{
		public:
			void Push( const GmodNode& parent );
			void Pop();
			int Occurrences( const GmodNode& node ) const;
			const GmodNode* LastOrDefault() const;
			const std::vector<GmodNode>& AsList() const;

		private:
			std::unordered_map<std::string, int> m_occurrences;
			std::vector<GmodNode> m_parents;
		};

		template <typename TState>
		TraversalHandlerResult TraverseNode(
			Parents& parents,
			const GmodNode& node,
			TState& state,
			const std::function<TraversalHandlerResult( TState&, const std::vector<GmodNode>&, const GmodNode& )>& handler,
			int maxOccurrence ) const;

		struct PathExistsContext
		{
			PathExistsContext( const GmodNode& toNode ) : To( toNode ) {}
			const GmodNode& To;
			std::vector<GmodNode> RemainingParents;
			std::vector<GmodNode> FromPath;
		};

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

	template <typename TState>
	bool Gmod::TraverseFrom(
		const GmodNode& startNode,
		TState& state,
		const std::function<TraversalHandlerResult( TState&, const std::vector<GmodNode>&, const GmodNode& )>& handler,
		const TraversalOptions& options ) const
	{
		Parents parents;
		return TraverseNode( parents, startNode, state, handler, options.MaxOccurrence ) == TraversalHandlerResult::Continue;
	}

	template <typename TState>
	Gmod::TraversalHandlerResult Gmod::TraverseNode(
		Parents& parents,
		const GmodNode& node,
		TState& state,
		const std::function<TraversalHandlerResult( TState&, const std::vector<GmodNode>&, const GmodNode& )>& handler,
		int maxOccurrence ) const
	{
		if ( !node.GetMetadata().GetInstallSubstructure() )
			return TraversalHandlerResult::Continue;

		auto result = handler( state, parents.AsList(), node );
		if ( result == TraversalHandlerResult::Stop || result == TraversalHandlerResult::SkipSubtree )
			return result;

		bool skipOccurrenceCheck = IsProductSelectionAssignment( parents.LastOrDefault(), &node );

		if ( !skipOccurrenceCheck )
		{
			auto occ = parents.Occurrences( node );
			if ( occ == maxOccurrence )
				return TraversalHandlerResult::SkipSubtree;
			if ( occ > maxOccurrence )
			{
				SPDLOG_ERROR( "Invalid state - node occurred more than expected" );
				throw std::runtime_error( "Invalid state - node occurred more than expected" );
			}
		}

		parents.Push( node );

		for ( const GmodNode* child : node.GetChildren() )
		{
			result = TraverseNode( parents, *child, state, handler, maxOccurrence );
			if ( result == TraversalHandlerResult::Stop )
				return result;
		}

		parents.Pop();
		return TraversalHandlerResult::Continue;
	}
}
