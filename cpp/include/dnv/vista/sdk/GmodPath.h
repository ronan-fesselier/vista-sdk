#pragma once

#include "GmodNode.h"

namespace dnv::vista::sdk
{
	class Gmod;
	class GmodParsePathResult;
	class GmodIndividualizableSet;
	class Locations;

	enum class VisVersion;

	/**
	 * @brief Represents a path in the Generic Product Model (GMOD)
	 *
	 * A GmodPath consists of a sequence of parent nodes and a target node,
	 * forming a hierarchical path through the GMOD structure as defined in ISO 19848.
	 * Each node in the path may have an optional location.
	 */
	class GmodPath
	{
	public:
		GmodPath() = default; // TODO Delete later

		/**
		 * @brief Construct a path from parent nodes and target node
		 *
		 * @param parents Vector of parent nodes in the path (from root to leaf)
		 * @param node The target node at the end of the path
		 * @param skipVerify If true, skip validation of parent-child relationships
		 * @throws std::invalid_argument If skipVerify is false and the path is invalid
		 */
		GmodPath( std::vector<GmodNode> parents, GmodNode node, bool skipVerify = false );

		/**
		 * @brief Copy constructor
		 * @param other The path to copy
		 */
		GmodPath( const GmodPath& other );

		/**
		 * @brief Move constructor
		 */
		GmodPath( GmodPath&& ) noexcept;

		/**
		 * @brief Destructor
		 */
		~GmodPath();

		/**
		 * @brief Copy assignment operator
		 * @param other The path to copy
		 * @return Reference to this path after assignment
		 */
		GmodPath& operator=( const GmodPath& other );

		/**
		 * @brief Move assignment operator
		 * @return Reference to this path after move assignment
		 */
		GmodPath& operator=( GmodPath&& ) noexcept;

		/**
		 * @brief Equality comparison
		 *
		 * Paths are equal if they have the same nodes at the same positions,
		 * including their location information.
		 *
		 * @param other The path to compare with
		 * @return True if paths are equal
		 */
		bool operator==( const GmodPath& other ) const;

		/**
		 * @brief Inequality comparison
		 * @param other The path to compare with
		 * @return True if paths are not equal
		 */
		bool operator!=( const GmodPath& other ) const;

		/**
		 * @brief Access a node at a specific depth in the path
		 *
		 * @param depth Zero-based depth index (0 is root, GetLength()-1 is target node)
		 * @return Const reference to the node at specified depth
		 * @throws std::out_of_range If depth is out of valid range
		 */
		const GmodNode& operator[]( int depth ) const;

		/**
		 * @brief Access a node at a specific depth in the path (non-const version)
		 *
		 * @param depth Zero-based depth index (0 is root, GetLength()-1 is target node)
		 * @return Reference to the node at specified depth
		 * @throws std::out_of_range If depth is out of valid range
		 */
		GmodNode& operator[]( int depth );

		size_t getHashCode() const;

		/**
		 * @brief Get the full path as depth-node pairs
		 * @return Vector of pairs containing (depth, node) for each node in the path
		 */
		std::vector<std::pair<int, GmodNode>> GetFullPath() const;

		/**
		 * @brief Get the parent nodes in the path
		 * @return Const reference to the vector of parent nodes
		 */
		const std::vector<GmodNode>& GetParents() const;

		/**
		 * @brief Get the target node at the end of the path
		 * @return Const reference to the target node
		 */
		const GmodNode& GetNode() const;

		/**
		 * @brief Get the VIS version of this path
		 * @return The VIS version enum value
		 */
		VisVersion GetVisVersion() const;

		/**
		 * @brief Get the total length of the path (parents + target node)
		 * @return The number of nodes in the path
		 */
		int GetLength() const;

		/**
		 * @brief Check if the path's target node is mappable
		 * @return True if the target node can be mapped to other nodes
		 */
		bool IsMappable() const;

		/**
		 * @brief Create a copy of this path with all locations removed
		 * @return A new GmodPath with the same nodes but no locations
		 */
		GmodPath WithoutLocations() const;

		/**
		 * @brief Convert the path to a string representation
		 *
		 * Format: code1/code2/.../targetCode (with optional location suffixes)
		 *
		 * @return String representation of the path
		 */
		std::string ToString() const;

		/**
		 * @brief Write string representation to a string stream
		 *
		 * @param builder The string stream to write to
		 * @param separator The separator character to use between nodes (default: '/')
		 */
		void ToString( std::stringstream& builder, char separator = '/' ) const;

		/**
		 * @brief Convert to a full path string representation including all nodes
		 * @return String representation of the full path
		 */
		std::string ToFullPathString() const;

		/**
		 * @brief Write full path string representation to a string stream
		 * @param builder The string stream to write to
		 */
		void ToFullPathString( std::stringstream& builder ) const;

		/**
		 * @brief Get a detailed string representation for debugging
		 * @return Detailed string representation with node information
		 */
		std::string ToStringDump() const;

		/**
		 * @brief Write detailed debug representation to a string stream
		 * @param builder The string stream to write to
		 */
		void ToStringDump( std::stringstream& builder ) const;

		/**
		 * @brief Get normal assignment name for a node at specific depth
		 *
		 * @param nodeDepth The depth of the node to get the name for
		 * @return The normal assignment name if present, or std::nullopt
		 */
		std::optional<std::string> GetNormalAssignmentName( int nodeDepth ) const;

		/**
		 * @brief Get all common names in the path
		 * @return Vector of pairs with (depth, common_name)
		 */
		std::vector<std::pair<int, std::string>> GetCommonNames() const;

		/**
		 * @brief Check if the path contains any individualizable nodes
		 * @return True if at least one node in the path is individualizable
		 */
		bool IsIndividualizable() const;

		/**
		 * @brief Get all individualizable sets in this path
		 *
		 * An individualizable set is a group of nodes that can be assigned
		 * a unique instance identifier.
		 *
		 * @return Vector of individualizable sets
		 */
		std::vector<GmodIndividualizableSet> GetIndividualizableSets() const;

		/**
		 * @brief Validate a path structure
		 *
		 * @param parents Vector of parent nodes
		 * @param node Target node
		 * @return True if the path is valid
		 */
		static bool IsValid( const std::vector<GmodNode>& parents, const GmodNode& node );

		/**
		 * @brief Validate a path structure and get the location of any invalid link
		 *
		 * @param parents Vector of parent nodes
		 * @param node Target node
		 * @param[out] missingLinkAt Index of the invalid parent-child link if any
		 * @return True if the path is valid
		 */
		static bool IsValid( const std::vector<GmodNode>& parents, const GmodNode& node, int& missingLinkAt );

		/**
		 * @brief Parse a path string with a specific VIS version
		 *
		 * @param item The path string to parse
		 * @param visVersion The VIS version to use for parsing
		 * @return The parsed GmodPath
		 * @throws std::invalid_argument If parsing fails
		 */
		static GmodPath Parse( const std::string& item, VisVersion visVersion );

		/**
		 * @brief Try to parse a path string with a specific VIS version
		 *
		 * @param item The path string to parse
		 * @param visVersion The VIS version to use for parsing
		 * @param[out] path Output parameter to store the parsed path if successful
		 * @return True if parsing succeeded, false otherwise
		 */
		static bool TryParse( const std::string& item, VisVersion visVersion, std::optional<GmodPath>& path );

		/**
		 * @brief Parse a path string using specific GMOD and Locations objects
		 *
		 * @param item The path string to parse
		 * @param gmod The GMOD object to use for resolving nodes
		 * @param locations The Locations object to use for resolving locations
		 * @return The parsed GmodPath
		 * @throws std::invalid_argument If parsing fails
		 */
		static GmodPath Parse( const std::string& item, const Gmod& gmod, const Locations& locations );

		/**
		 * @brief Try to parse a path string using specific GMOD and Locations objects
		 *
		 * @param item The path string to parse
		 * @param gmod The GMOD object to use for resolving nodes
		 * @param locations The Locations object to use for resolving locations
		 * @param[out] path Output parameter to store the parsed path if successful
		 * @return True if parsing succeeded, false otherwise
		 */
		static bool TryParse( const std::string& item, const Gmod& gmod, const Locations& locations, GmodPath& path );

		/**
		 * @brief Parse a full path string with a specific VIS version
		 *
		 * @param pathStr The full path string to parse
		 * @param visVersion The VIS version to use for parsing
		 * @return The parsed GmodPath
		 * @throws std::invalid_argument If parsing fails
		 */
		static GmodPath ParseFullPath( const std::string& pathStr, VisVersion visVersion );

		/**
		 * @brief Try to parse a full path string with a specific VIS version
		 *
		 * @param pathStr The full path string to parse
		 * @param visVersion The VIS version to use for parsing
		 * @param[out] path Output parameter to store the parsed path if successful
		 * @return True if parsing succeeded, false otherwise
		 */
		static bool TryParseFullPath( const std::string& pathStr, VisVersion visVersion, GmodPath& path );

		/**
		 * @brief Try to parse a full path string with a specific VIS version
		 *
		 * @param pathStr The full path string to parse as string_view
		 * @param visVersion The VIS version to use for parsing
		 * @param[out] path Output parameter to store the parsed path if successful
		 * @return True if parsing succeeded, false otherwise
		 */
		static bool TryParseFullPath( std::string_view pathStr, VisVersion visVersion, GmodPath& path );

		/**
		 * @brief Try to parse a full path string using specific GMOD and Locations objects
		 *
		 * @param pathStr The full path string to parse as string_view
		 * @param gmod The GMOD object to use for resolving nodes
		 * @param locations The Locations object to use for resolving locations
		 * @param[out] path Output parameter to store the parsed path if successful
		 * @return True if parsing succeeded, false otherwise
		 */
		static bool TryParseFullPath( std::string_view pathStr, const Gmod& gmod, const Locations& locations, GmodPath& path );

	private:
		/**
		 * @brief Internal method to parse a path string
		 *
		 * @param item The path string to parse
		 * @param gmod The GMOD object to use for resolving nodes
		 * @param locations The Locations object to use for resolving locations
		 * @return A GmodParsePathResult containing either a path or an error
		 */
		static GmodParsePathResult ParseInternal( const std::string& item, const Gmod& gmod, const Locations& locations );

		/**
		 * @brief Internal method to parse a full path string
		 *
		 * @param span The full path string to parse as string_view
		 * @param gmod The GMOD object to use for resolving nodes
		 * @param locations The Locations object to use for resolving locations
		 * @return A GmodParsePathResult containing either a path or an error
		 */
		static GmodParsePathResult ParseFullPathInternal( std::string_view span, const Gmod& gmod, const Locations& locations );

	private:
		/** @brief Vector of parent nodes in the path (from root to target) */
		std::vector<GmodNode> m_parents;

		/** @brief Target node at the end of the path */
		GmodNode m_node;

		/** @brief VIS version associated with this path */
		VisVersion m_visVersion;

	private:
		/**
		 * @brief Enumerator for traversing the GmodPath
		 *
		 * Provides sequential access to the nodes in the path with their depths.
		 */
		class Enumerator
		{
		public:
			/**
			 * @brief Construct a new Enumerator
			 *
			 * @param path The path to enumerate
			 * @param fromDepth Optional starting depth (default is beginning of path)
			 */
			Enumerator( const GmodPath& path, std::optional<int> fromDepth = std::nullopt );

			/**
			 * @brief Move to the next node in the path
			 * @return True if moved to a valid node, false if reached the end
			 */
			bool MoveNext();

			/**
			 * @brief Get the current node and its depth
			 * @return Pair of (depth, node_reference)
			 */
			std::pair<int, std::reference_wrapper<const GmodNode>> GetCurrent() const;

			/**
			 * @brief Iterator adapter for the Enumerator
			 *
			 * Enables use with range-based for loops and standard algorithms.
			 */
			class Iterator
			{
			private:
				/** @brief Reference to the underlying enumerator */
				Enumerator& m_enumerator;

				/** @brief Flag indicating if this is the end iterator */
				bool m_end;

			public:
				/**
				 * @brief Construct a new Iterator
				 *
				 * @param enumerator The enumerator to iterate
				 * @param end True if this is the end iterator
				 */
				Iterator( Enumerator& enumerator, bool end );

				/**
				 * @brief Pre-increment operator
				 * @return Reference to this iterator after advancing
				 */
				Iterator& operator++();

				/**
				 * @brief Inequality comparison
				 * @param other Iterator to compare with
				 * @return True if iterators are not equal
				 */
				bool operator!=( const Iterator& other ) const;

				/**
				 * @brief Dereference operator
				 * @return The current depth-node pair
				 */
				std::pair<int, std::reference_wrapper<const GmodNode>> operator*() const;
			};

			/**
			 * @brief Get iterator to the beginning
			 * @return Iterator positioned at the first node
			 */
			Iterator begin();

			/**
			 * @brief Get iterator to the end
			 * @return Iterator positioned after the last node
			 */
			Iterator end();

		private:
			/** @brief Reference to the path being enumerated */
			const GmodPath& m_path;

			/** @brief Current position index in the path */
			int m_current;

			/** @brief Current depth value */
			int m_depth;

			/** @brief Initial depth to start from */
			std::optional<int> m_fromDepth;
		};
	};

	/**
	 * @brief Represents the result of parsing a GmodPath
	 *
	 * Abstract base class for success and error results from path parsing operations.
	 */
	class GmodParsePathResult
	{
	protected:
		/** @brief Protected constructor to prevent direct instantiation */
		GmodParsePathResult() = default;

	public:
		/** @brief Virtual destructor */
		virtual ~GmodParsePathResult() = default;

		/** @brief Success result type */
		class Ok;

		/** @brief Error result type */
		class Err;
	};

	/**
	 * @brief Successful path parsing result
	 *
	 * Contains the successfully parsed GmodPath.
	 */
	class GmodParsePathResult::Ok : public GmodParsePathResult
	{
	public:
		/** @brief The successfully parsed path */
		GmodPath path;

		/**
		 * @brief Construct a successful result
		 * @param path The parsed path
		 */
		explicit Ok( const GmodPath& path );
		Ok( GmodPath&& p );
	};

	/**
	 * @brief Failed path parsing result
	 *
	 * Contains an error message describing why parsing failed.
	 */
	class GmodParsePathResult::Err : public GmodParsePathResult
	{
	public:
		/** @brief Error message describing the parsing failure */
		std::string error;

		/**
		 * @brief Construct an error result
		 * @param error The error message
		 */
		explicit Err( const std::string& error );
	};

	/**
	 * @brief Represents a set of individualizable nodes in a GmodPath
	 *
	 * An individualizable set is a group of nodes that can be assigned
	 * a unique instance identifier and share the same location.
	 */
	class GmodIndividualizableSet
	{
	public:
		/**
		 * @brief Construct a new individualizable set
		 *
		 * @param nodes Vector of node indices in the path
		 * @param path Reference to the path containing the nodes
		 * @throws std::invalid_argument If constraints are not satisfied
		 */
		GmodIndividualizableSet( const std::vector<int>& nodes, GmodPath& path );

		/**
		 * @brief Get the actual nodes in this set
		 * @return Vector of nodes
		 */
		std::vector<GmodNode> GetNodes() const;

		/**
		 * @brief Get the indices of nodes in this set
		 * @return Const reference to the vector of indices
		 */
		const std::vector<int>& GetNodeIndices() const;

		/**
		 * @brief Get the location associated with this set
		 * @return The location if present, or std::nullopt
		 */
		std::optional<Location> GetLocation() const;

		/**
		 * @brief Set the location for all nodes in this set
		 * @param location The location to set, or std::nullopt to remove location
		 */
		void SetLocation( const std::optional<Location>& location );

		/**
		 * @brief Build and return the modified path
		 * @return The path with any modifications applied
		 * @throws std::runtime_error If the set was already built
		 */
		GmodPath Build();

		/**
		 * @brief Convert to string representation
		 * @return String representation of the individualizable set
		 */
		std::string ToString() const;

	private:
		/** @brief Indices of nodes in the individualizable set */
		std::vector<int> m_nodes;

		/** @brief Pointer to the path containing the nodes */
		GmodPath* m_path;
	};

	/**
	 * @brief Represents a single node in the path
	 *
	 * Contains the node's code and optional location.
	 */
	struct PathNode
	{
		/** @brief Code identifying the node */
		std::string code;

		/** @brief Optional location associated with the node */
		std::optional<Location> location;

		/**
		 * @brief Construct a new PathNode
		 *
		 * @param code The node code (default: empty string)
		 * @param location The optional location (default: no location)
		 */
		PathNode( const std::string& code = "", const std::optional<Location>& location = std::nullopt );
	};

	/**
	 * @brief Represents the context for parsing a GmodPath
	 *
	 * Contains the state needed during path parsing operations.
	 */
	struct ParseContext
	{
		/** @brief Queue of remaining path nodes to process */
		std::queue<PathNode> parts;

		/** @brief Current node to find */
		PathNode toFind;

		/** @brief Mapping of node codes to their locations */
		std::unordered_map<std::string, Location> locations;

		/** @brief The resulting path if parsing succeeds */
		std::optional<GmodPath> path;

		/**
		 * @brief Construct a new parse context
		 * @param parts Queue of path nodes to process
		 */
		ParseContext( std::queue<PathNode> parts );
	};
}
