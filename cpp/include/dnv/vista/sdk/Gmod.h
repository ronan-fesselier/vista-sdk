/**
 * @file Gmod.h
 * @brief Generic Product Model (GMOD) interface for the DNV Vessel Information Structure (VIS)
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
	 * @brief Helper class to manage parent nodes during traversal
	 *
	 * Tracks the parent nodes in traversal and manages occurrence counting
	 * to prevent infinite recursion in cyclic structures.
	 */
	class Parents
	{
	public:
		/**
		 * @brief Add a parent node to the chain
		 * @param parent Node to add
		 */
		void push( const GmodNode* parent );

		/**
		 * @brief Remove the last parent node
		 */
		void pop();

		/**
		 * @brief Count occurrences of a node in the parent chain
		 * @param node Node to count
		 * @return Number of times the node appears in the chain
		 */
		int occurrences( const GmodNode& node ) const;

		/**
		 * @brief Get the last node in the chain, if any
		 * @return Pointer to last node, or nullptr if chain is empty
		 */
		const GmodNode* lastOrDefault() const;

		/**
		 * @brief Convert parent pointers to vector of actual nodes
		 * @return Vector of node copies
		 */
		std::vector<GmodNode> nodes() const;

		/**
		 * @brief Direct access to the parent node pointers
		 * @return Constant reference to the vector of parent node pointers
		 */
		const std::vector<const GmodNode*>& nodePointers() const;

	private:
		std::vector<const GmodNode*> m_nodes;				///< Parent node pointers in traversal order
		std::unordered_map<std::string, int> m_occurrences; ///< Node occurrence counter
	};

	/**
	 * @brief Generic Product Model (GMOD) for vessel information systems
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
			static constexpr int DEFAULT_MAX_TRAVERSAL_OCCURRENCE = 1;

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
		Gmod( const Gmod& other );

		/**
		 * @brief Move constructor for Gmod
		 * @param other The source Gmod object to move from
		 */
		Gmod( Gmod&& other ) noexcept;

		/** @brief Destructor */
		~Gmod();

		/**
		 * @brief Move assignment operator for Gmod
		 * @param other The source Gmod object to move from
		 * @return Reference to this object
		 */
		Gmod& operator=( Gmod&& other ) noexcept;

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
		VisVersion visVersion() const;

		/**
		 * @brief Get the root node of the GMOD hierarchy
		 * @return Reference to the root node
		 */
		const GmodNode& rootNode() const;

		/**
		 * @brief Try to find a node by code
		 * @param code The node code to find
		 * @param[out] node The found node, if successful
		 * @return true if node was found, false otherwise
		 */
		bool tryGetNode( const std::string& code, GmodNode& node ) const;

		/**
		 * @brief Try to find a node by code (string_view version)
		 * @param code The node code to find
		 * @param[out] node The found node, if successful
		 * @return true if node was found, false otherwise
		 */
		bool tryGetNode( std::string_view code, GmodNode& node ) const;

		/**
		 * @brief Parse a path string into a GmodPath
		 * @param item Path string to parse (e.g. "VE/D101/C201")
		 * @return Parsed GmodPath
		 * @throws std::invalid_argument if parsing fails
		 */
		GmodPath parsePath( const std::string& item ) const;

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
		GmodPath parseFromFullPath( const std::string& item ) const;

		/**
		 * @brief Try to parse a full path string
		 * @param item Full path string to parse
		 * @param[out] path The parsed path, if successful
		 * @return true if parsing succeeded, false otherwise
		 */
		bool tryParseFromFullPath( const std::string& item, std::optional<GmodPath>& path ) const;

		/**
		 * @brief Traverse the GMOD hierarchy from root node
		 * @param handler Function called for each node visited
		 * @param options Traversal configuration options
		 * @return true if traversal completed successfully
		 */
		bool traverse( const TraverseHandler& handler, const TraversalOptions& options = {} ) const;

		/**
		 * @brief Traverse the GMOD hierarchy from a specific starting node
		 * @param rootNode Node to start traversal from
		 * @param handler Function called for each node visited
		 * @param options Traversal configuration options
		 * @return true if traversal completed successfully
		 */
		bool traverse( const GmodNode& rootNode, const TraverseHandler& handler, const TraversalOptions& options = {} ) const;

		/**
		 * @brief Stateful traversal of GMOD from root node
		 * @tparam TState Type of state object to maintain during traversal
		 * @param state State object passed to handler for context preservation
		 * @param handler Function called for each node visited
		 * @param options Traversal configuration options
		 * @return true if traversal completed successfully
		 */
		template <typename TState>
		bool traverse(
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
		bool traverse(
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
		static bool isLeafNode( const std::string& fullType );

		/**
		 * @brief Check if a node is a leaf node by metadata
		 * @param metadata Node metadata to check
		 * @return true if it's a leaf node
		 */
		static bool isLeafNode( const GmodNodeMetadata& metadata );

		/**
		 * @brief Check if a node is a function node by category
		 * @param category Node category to check
		 * @return true if it's a function node
		 */
		static bool isFunctionNode( const std::string& category );

		/**
		 * @brief Check if a node is a function node by metadata
		 * @param metadata Node metadata to check
		 * @return true if it's a function node
		 */
		static bool isFunctionNode( const GmodNodeMetadata& metadata );

		/**
		 * @brief Check if a node is a product selection by metadata
		 * @param metadata Node metadata to check
		 * @return true if it's a product selection
		 */
		static bool isProductSelection( const GmodNodeMetadata& metadata );

		/**
		 * @brief Check if a node is a product type by metadata
		 * @param metadata Node metadata to check
		 * @return true if it's a product type
		 */
		static bool isProductType( const GmodNodeMetadata& metadata );

		/**
		 * @brief Check if a node is an asset by metadata
		 * @param metadata Node metadata to check
		 * @return true if it's an asset
		 */
		static bool isAsset( const GmodNodeMetadata& metadata );

		/**
		 * @brief Check if a node is an asset function node by metadata
		 * @param metadata Node metadata to check
		 * @return true if it's an asset function node
		 */
		static bool isAssetFunctionNode( const GmodNodeMetadata& metadata );

		/**
		 * @brief Check if there's a product type assignment relationship
		 * @param parent Parent node to check
		 * @param child Child node to check
		 * @return true if there's a product type assignment
		 */
		static bool isProductTypeAssignment( const GmodNode* parent, const GmodNode* child );

		/**
		 * @brief Check if there's a product selection assignment relationship
		 * @param parent Parent node to check
		 * @param child Child node to check
		 * @return true if there's a product selection assignment
		 */
		static bool isProductSelectionAssignment( const GmodNode* parent, const GmodNode* child );

	private:
		/**
		 * @brief Context data for traversal operations
		 */
		struct TraversalContext
		{
			Parents parents;				///< Parent node management
			const TraverseHandler& handler; ///< Handler function for traversal
			int maxTraversalOccurrence;		///< Maximum node occurrences allowed
			size_t nodesVisited = 0;		///< Counter for total nodes visited
			size_t maxNodes;				///< Maximum nodes to visit

			/**
			 * @brief Construct a traversal context
			 * @param h Handler function
			 * @param maxOcc Maximum occurrences
			 * @param maxN Maximum nodes to visit
			 */
			TraversalContext( const TraverseHandler& h, int maxOcc, size_t maxN )
				: handler( h ), maxTraversalOccurrence( maxOcc ), maxNodes( maxN ) {}
		};

		/**
		 * @brief Context data for stateful traversal operations
		 */
		template <typename TState>
		struct StatefulTraversalContext
		{
			Parents parents;							///< Parent node management
			TState& state;								///< User-defined state
			const std::function<TraversalHandlerResult( ///< Handler function
				TState&,
				const std::vector<GmodNode>&,
				const GmodNode& )>& handler;
			int maxTraversalOccurrence; ///< Maximum node occurrences allowed
			size_t nodesVisited = 0;	///< Counter for total nodes visited
			size_t maxNodes;			///< Maximum nodes to visit

			/**
			 * @brief Construct a stateful traversal context
			 * @param s User-defined state object
			 * @param h Handler function
			 * @param maxOcc Maximum occurrences
			 * @param maxN Maximum nodes to visit
			 */
			StatefulTraversalContext(
				TState& s,
				const std::function<TraversalHandlerResult( TState&, const std::vector<GmodNode>&, const GmodNode& )>& h,
				int maxOcc,
				size_t maxN )
				: state( s ), handler( h ), maxTraversalOccurrence( maxOcc ), maxNodes( maxN ) {}
		};

		/**
		 * @brief Internal recursive traversal method
		 * @param context Traversal context
		 * @param node Current node to process
		 * @return Traversal result
		 */
		TraversalHandlerResult traverseNode( TraversalContext& context, const GmodNode& node ) const;

		/**
		 * @brief Internal recursive stateful traversal method
		 * @param context Stateful traversal context
		 * @param node Current node to process
		 * @return Traversal result
		 */
		template <typename TState>
		TraversalHandlerResult traverseNode( StatefulTraversalContext<TState>& context, const GmodNode& node ) const;

	public:
		/**
		 * @brief Check if the GMOD node dictionary is empty
		 * @return true if the node map is empty, false otherwise
		 */
		bool isEmpty() const
		{
			return m_nodeMap.isEmpty();
		}

		/**
		 * @brief Initialize the node dictionary from a GmodDto
		 * @param dto The GmodDto containing node data
		 * @return true if initialization was successful, false otherwise
		 */
		bool initializeNodeDictionary( const GmodDto& dto );

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
	bool Gmod::traverse(
		TState& state,
		const std::function<Gmod::TraversalHandlerResult( TState&, const std::vector<GmodNode>&, const GmodNode& )>& handler,
		const Gmod::TraversalOptions& options ) const
	{
		return traverse( state, rootNode(), handler, options );
	}

	/**
	 * @brief Implementation of stateful traversal from specific node
	 */
	template <typename TState>
	bool Gmod::traverse(
		TState& state,
		const GmodNode& rootNode,
		const std::function<Gmod::TraversalHandlerResult( TState&, const std::vector<GmodNode>&, const GmodNode& )>& handler,
		const Gmod::TraversalOptions& options ) const
	{
		StatefulTraversalContext<TState> context(
			state, handler, options.maxTraversalOccurrence, options.maxNodes );

		TraversalHandlerResult result = traverseNode( context, rootNode );

		return result != TraversalHandlerResult::Stop;
	}

	/**
	 * @brief Implementation of stateful recursive traversal
	 */
	template <typename TState>
	Gmod::TraversalHandlerResult Gmod::traverseNode( StatefulTraversalContext<TState>& context, const GmodNode& node ) const
	{
		if ( context.nodesVisited >= context.maxNodes )
		{
			return TraversalHandlerResult::Stop;
		}

		if ( node.GetMetadata().GetInstallSubstructure().has_value() &&
			 !node.GetMetadata().GetInstallSubstructure().value() )
		{
			return TraversalHandlerResult::Continue;
		}

		context.nodesVisited++;

		bool skipOccurrenceCheck = IsProductSelectionAssignment( context.parents.LastOrDefault(), &node );
		if ( !skipOccurrenceCheck )
		{
			int occ = context.parents.Occurrences( node );
			if ( occ == context.maxTraversalOccurrence )
			{
				return TraversalHandlerResult::SkipSubtree;
			}
			if ( occ > context.maxTraversalOccurrence )
			{
				SPDLOG_ERROR( "Invalid state - node occurred more than expected" );
				return TraversalHandlerResult::Stop;
			}
		}

		std::vector<GmodNode> currentPath = context.parents.GetNodes();

		TraversalHandlerResult result = context.handler( context.state, currentPath, node );

		if ( result == TraversalHandlerResult::Stop || result == TraversalHandlerResult::SkipSubtree )
		{
			return result;
		}

		context.parents.Push( &node );

		for ( const GmodNode* child : node.GetChildren() )
		{
			if ( child == nullptr )
				continue;

			result = traverseNode( context, *child );

			if ( result == TraversalHandlerResult::Stop )
			{
				return result;
			}
		}

		context.parents.Pop();

		return TraversalHandlerResult::Continue;
	}
}
