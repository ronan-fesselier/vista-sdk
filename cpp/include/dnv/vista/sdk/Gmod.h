/**
 * @file Gmod.h
 * @brief Generic MODel (GMOD) interface for the DNV Vessel Information Structure (VIS)
 *
 * This file defines the Gmod class, a core component of the VISTA SDK that represents
 * the hierarchical structure of vessel components and systems according to the
 * DNV Vessel Information Structure standard.
 */

#pragma once

#include "ChdDictionary.h"
#include "GmodNode.h"

namespace dnv::vista::sdk
{
	enum class VisVersion;
	struct GmodDto;
	class GmodPath;

	/**
	 * @brief Generic MODel (GMOD) for vessel information systems
	 *
	 * Represents the hierarchical structure of vessel components and systems
	 * according to the DNV Vessel Information Structure (VIS) standard. The GMOD
	 * provides functionality for traversing the hierarchy, locating nodes, and
	 * working with vessel component paths.
	 */
	class Gmod
	{
	public:
		/**
		 * @brief Result values returned by traversal handler functions
		 */
		enum class TraversalHandlerResult
		{
			Stop,		 ///< Stop traversal completely
			SkipSubtree, ///< Skip traversing children of current node
			Continue	 ///< Continue normal traversal
		};

		/**
		 * @brief Options to configure GMOD traversal behavior
		 */
		struct TraversalOptions
		{
			/** @brief Default maximum number of times a node can be visited */
			static constexpr int DEFAULT_MAX_TRAVERSAL_OCCURRENCE = 3;

			/** @brief Maximum depth to traverse from root */
			int maxDepth = 100;

			/** @brief Maximum number of times a node can occur in a traversal path */
			int maxTraversalOccurrence = DEFAULT_MAX_TRAVERSAL_OCCURRENCE;

			/** @brief Maximum total nodes to visit during traversal */
			size_t maxNodes = 100000;

			/** @brief Default constructor */
			TraversalOptions() : maxTraversalOccurrence( DEFAULT_MAX_TRAVERSAL_OCCURRENCE ) {}

			/**
			 * @brief Constructor with custom occurrence limit
			 * @param maxTraversalOccurrence Maximum times a node can be visited
			 */
			explicit TraversalOptions( int maxTraversalOccurrence ) : maxTraversalOccurrence( maxTraversalOccurrence ) {}
		};

		/** @brief Handler function type for traversal without state */
		using TraverseHandler = std::function<TraversalHandlerResult( const std::vector<GmodNode>&, const GmodNode& )>;

	public:
		/** @brief Default constructor is deleted - Gmod requires initialization data */
		Gmod() = delete;

		/**
		 * @brief Construct from DTO and VIS version
		 * @param version The Vessel Information Structure version
		 * @param dto Data transfer object containing GMOD data
		 */
		Gmod( VisVersion version, const GmodDto& dto );

		/**
		 * @brief Construct from node map and VIS version
		 * @param version The Vessel Information Structure version
		 * @param nodeMap Map of node codes to GmodNode objects
		 */
		Gmod( VisVersion version, const std::unordered_map<std::string, GmodNode>& nodeMap );

		/** @brief Copy constructor */
		Gmod( const Gmod& other ) = default;

		/** @brief Move constructor is deleted */
		Gmod( Gmod&& other ) noexcept = delete;

		/** @brief Destructor */
		~Gmod();

		/** @brief Copy assignment is deleted */
		Gmod& operator=( const Gmod& other ) = delete;

		/** @brief Move assignment */
		Gmod& operator=( Gmod&& other ) noexcept = default;

		/**
		 * @brief Access node by code
		 * @param key Node code to look up
		 * @return Reference to the node if found, or to an empty node if not found
		 */
		const GmodNode& operator[]( const std::string& key ) const;

		/**
		 * @brief Get the VIS version used by this GMOD
		 * @return The VIS version
		 */
		VisVersion GetVisVersion() const;

		/**
		 * @brief Get the root node of the GMOD hierarchy
		 * @return Reference to the root node
		 */
		const GmodNode& GetRootNode() const;

		/**
		 * @brief Try to find a node by code
		 * @param code The node code to find
		 * @param[out] node The found node, if successful
		 * @return true if node was found, false otherwise
		 */
		bool TryGetNode( const std::string& code, GmodNode& node ) const;

		/**
		 * @brief Try to find a node by code (string_view version)
		 * @param code The node code to find
		 * @param[out] node The found node, if successful
		 * @return true if node was found, false otherwise
		 */
		bool TryGetNode( std::string_view code, GmodNode& node ) const;

		/**
		 * @brief Parse a path string into a GmodPath
		 * @param item Path string to parse (e.g. "VE/D101/C201")
		 * @return Parsed GmodPath
		 * @throws std::invalid_argument if parsing fails
		 */
		GmodPath ParsePath( const std::string& item ) const;

		/**
		 * @brief Try to parse a path string into a GmodPath
		 * @param item Path string to parse
		 * @param[out] path The parsed path, if successful
		 * @return true if parsing succeeded, false otherwise
		 */
		bool TryParsePath( const std::string& item, std::optional<GmodPath>& path ) const;

		/**
		 * @brief Parse a full path string
		 * @param item Full path string to parse
		 * @return Parsed GmodPath
		 * @throws std::invalid_argument if parsing fails
		 */
		GmodPath ParseFromFullPath( const std::string& item ) const;

		/**
		 * @brief Try to parse a full path string
		 * @param item Full path string to parse
		 * @param[out] path The parsed path, if successful
		 * @return true if parsing succeeded, false otherwise
		 */
		bool TryParseFromFullPath( const std::string& item, std::optional<GmodPath>& path ) const;

		/**
		 * @brief Traverse the GMOD hierarchy from root node
		 * @param handler Function called for each node visited
		 * @param options Traversal configuration options
		 * @return true if traversal completed successfully
		 */
		bool Traverse( const TraverseHandler& handler, const TraversalOptions& options = {} ) const;

		/**
		 * @brief Traverse the GMOD hierarchy from a specific starting node
		 * @param rootNode Node to start traversal from
		 * @param handler Function called for each node visited
		 * @param options Traversal configuration options
		 * @return true if traversal completed successfully
		 */
		bool Traverse( const GmodNode& rootNode, const TraverseHandler& handler, const TraversalOptions& options = {} ) const;

		/**
		 * @brief Stateful traversal of GMOD from root node
		 * @tparam TState Type of state object to maintain during traversal
		 * @param state State object passed to handler for context preservation
		 * @param handler Function called for each node visited
		 * @param options Traversal configuration options
		 * @return true if traversal completed successfully
		 */
		template <typename TState>
		bool Traverse(
			TState& state,
			const std::function<TraversalHandlerResult( TState&, const std::vector<GmodNode>&, const GmodNode& )>& handler,
			const TraversalOptions& options = {} ) const;

		/**
		 * @brief Stateful traversal of GMOD from specific starting node
		 * @tparam TState Type of state object to maintain during traversal
		 * @param state State object passed to handler for context preservation
		 * @param rootNode Node to start traversal from
		 * @param handler Function called for each node visited
		 * @param options Traversal configuration options
		 * @return true if traversal completed successfully
		 */
		template <typename TState>
		bool Traverse(
			TState& state,
			const GmodNode& rootNode,
			const std::function<TraversalHandlerResult( TState&, const std::vector<GmodNode>&, const GmodNode& )>& handler,
			const TraversalOptions& options = {} ) const;

	public:
		/**
		 * @brief Check if a node is a leaf node by type
		 * @param fullType The node type string to check
		 * @return true if it's a leaf node
		 */
		static bool IsLeafNode( const std::string& fullType );

		/**
		 * @brief Check if a node is a leaf node by metadata
		 * @param metadata Node metadata to check
		 * @return true if it's a leaf node
		 */
		static bool IsLeafNode( const GmodNodeMetadata& metadata );

		/**
		 * @brief Check if a node is a function node by category
		 * @param category Node category to check
		 * @return true if it's a function node
		 */
		static bool IsFunctionNode( const std::string& category );

		/**
		 * @brief Check if a node is a function node by metadata
		 * @param metadata Node metadata to check
		 * @return true if it's a function node
		 */
		static bool IsFunctionNode( const GmodNodeMetadata& metadata );

		/**
		 * @brief Check if a node is a product selection by metadata
		 * @param metadata Node metadata to check
		 * @return true if it's a product selection
		 */
		static bool IsProductSelection( const GmodNodeMetadata& metadata );

		/**
		 * @brief Check if a node is a product type by metadata
		 * @param metadata Node metadata to check
		 * @return true if it's a product type
		 */
		static bool IsProductType( const GmodNodeMetadata& metadata );

		/**
		 * @brief Check if a node is an asset by metadata
		 * @param metadata Node metadata to check
		 * @return true if it's an asset
		 */
		static bool IsAsset( const GmodNodeMetadata& metadata );

		/**
		 * @brief Check if a node is an asset function node by metadata
		 * @param metadata Node metadata to check
		 * @return true if it's an asset function node
		 */
		static bool IsAssetFunctionNode( const GmodNodeMetadata& metadata );

		/**
		 * @brief Check if there's a product type assignment relationship
		 * @param parent Parent node to check
		 * @param child Child node to check
		 * @return true if there's a product type assignment
		 */
		static bool IsProductTypeAssignment( const GmodNode* parent, const GmodNode* child );

		/**
		 * @brief Check if there's a product selection assignment relationship
		 * @param parent Parent node to check
		 * @param child Child node to check
		 * @return true if there's a product selection assignment
		 */
		static bool IsProductSelectionAssignment( const GmodNode* parent, const GmodNode* child );

	private:
		/** @brief VIS version for this GMOD */
		VisVersion m_visVersion;

		/** @brief Root node of the hierarchy */
		GmodNode m_rootNode;

		/** @brief Optimized dictionary for node lookups */
		ChdDictionary<GmodNode> m_nodeMap;

		/** @brief List of node types classified as leaf nodes */
		static const inline std::vector<std::string> s_leafTypes = { "ASSET FUNCTION LEAF", "PRODUCT FUNCTION LEAF" };

	public:
		/**
		 * @brief Iterator for traversing nodes in the GMOD
		 */
		class Iterator
		{
		public:
			/** @brief STL iterator traits */
			using iterator_category = std::forward_iterator_tag;
			using value_type = GmodNode;
			using difference_type = std::ptrdiff_t;
			using pointer = const GmodNode*;
			using reference = const GmodNode&;

			/** @brief Default constructor */
			Iterator() = default;

			/**
			 * @brief Constructor from inner iterator
			 * @param innerIt CHD dictionary iterator
			 */
			Iterator( ChdDictionary<GmodNode>::Iterator innerIt );

			/** @brief Dereference operator */
			reference operator*() const;

			/** @brief Arrow operator */
			pointer operator->() const;

			/** @brief Pre-increment operator */
			Iterator& operator++();

			/** @brief Post-increment operator */
			Iterator operator++( int );

			/** @brief Equality operator */
			bool operator==( const Iterator& other ) const;

			/** @brief Inequality operator */
			bool operator!=( const Iterator& other ) const;

		private:
			/** @brief Inner CHD dictionary iterator */
			ChdDictionary<GmodNode>::Iterator m_innerIt;
		};

		/** @brief Get iterator to beginning of nodes */
		Iterator begin() const;

		/** @brief Get iterator to end of nodes */
		Iterator end() const;
	};

	/**
	 * @brief Implementation of stateful traversal from root node
	 */
	template <typename TState>
	bool Gmod::Traverse(
		TState& state,
		const std::function<Gmod::TraversalHandlerResult( TState&, const std::vector<GmodNode>&, const GmodNode& )>& handler,
		const Gmod::TraversalOptions& options ) const
	{
		return Gmod::Traverse( state, Gmod::GetRootNode(), handler, options );
	}

	/**
	 * @brief Implementation of stateful traversal from specific node
	 */
	template <typename TState>
	bool Gmod::Traverse(
		TState& state,
		const GmodNode& rootNode,
		const std::function<Gmod::TraversalHandlerResult( TState&, const std::vector<GmodNode>&, const GmodNode& )>& handler,
		const Gmod::TraversalOptions& options ) const
	{
		std::queue<std::pair<GmodNode, std::vector<GmodNode>>> queue;
		queue.push( { rootNode, {} } );

		std::unordered_map<std::string, int> visitCount;
		size_t nodesVisited = 0;

		while ( !queue.empty() && nodesVisited < options.maxNodes )
		{
			auto [current, parents] = queue.front();
			queue.pop();

			if ( visitCount[current.GetCode()]++ >= options.maxTraversalOccurrence )
			{
				continue;
			}

			nodesVisited++;

			TraversalHandlerResult result = handler( state, parents, current );

			if ( result == TraversalHandlerResult::Stop )
			{
				return true;
			}

			if ( result == TraversalHandlerResult::SkipSubtree )
			{
				continue;
			}

			if ( parents.size() < options.maxDepth )
			{
				std::vector<GmodNode> newParents = parents;
				newParents.push_back( current );

				for ( auto* child : current.GetChildren() )
				{
					queue.push( { *child, newParents } );
				}
			}
		}

		return true;
	}
}
