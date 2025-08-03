/**
 * @file GmodPath.h
 * @brief Declarations for GmodPath and related classes for representing paths in the Generic Product Model (GMOD).
 * @details Defines the `GmodPath` class for representing hierarchical paths according to ISO 19848,
 *          along with helper classes for parsing, validation, iteration, and modification of path segments.
 */

#pragma once

#include "GmodNode.h"

#include "config/Platform.h"
#include "utils/Hashing.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// Forward declarations
	//=====================================================================

	class Gmod;
	class GmodIndividualizableSet;
	class Location;
	class Locations;

	struct GmodParsePathResult;

	enum class TraversalHandlerResult;
	enum class VisVersion;

	namespace internal
	{
		struct ParseContext;

		dnv::vista::sdk::TraversalHandlerResult parseInternalTraversalHandler(
			ParseContext& context,
			const std::vector<const GmodNode*>& traversedParents,
			const GmodNode& currentNode );
	}

	//=====================================================================
	// GmodPath class
	//=====================================================================

	class GmodPath final
	{
		//----------------------------------------------
		// Forward declarations
		//----------------------------------------------

		friend class GmodIndividualizableSet;

		friend dnv::vista::sdk::TraversalHandlerResult internal::parseInternalTraversalHandler(
			internal::ParseContext&,
			const std::vector<const GmodNode*>&,
			const GmodNode& );

	public:
		class Enumerator;

		//----------------------------------------------
		// Construction
		//----------------------------------------------

		GmodPath( const Gmod& gmod, GmodNode node, std::vector<GmodNode> parents = {}, bool skipVerify = false );

		/** @brief Default constructor. */
		inline GmodPath();

		/** @brief Copy constructor */
		inline GmodPath( const GmodPath& other );

		/** @brief Move constructor */
		inline GmodPath( GmodPath&& other ) noexcept;

		//----------------------------------------------
		// Destruction
		//----------------------------------------------

		/** @brief Destructor */
		~GmodPath() = default;

		//----------------------------------------------
		// Assignment operators
		//----------------------------------------------

		/** @brief Copy assignment operator */
		inline GmodPath& operator=( const GmodPath& other );

		/** @brief Move assignment operator */
		inline GmodPath& operator=( GmodPath&& other ) noexcept;

		//----------------------------------------------
		// Equality operators
		//----------------------------------------------

		[[nodiscard]] inline bool operator==( const GmodPath& other ) const noexcept;
		[[nodiscard]] inline bool operator!=( const GmodPath& other ) const noexcept;

		//----------------------------------------------
		// Lookup operators
		//----------------------------------------------

		[[nodiscard]] inline const GmodNode& operator[]( size_t index ) const;
		[[nodiscard]] inline GmodNode& operator[]( size_t index );

		//----------------------------------------------
		// Accessors
		//----------------------------------------------

		/**
		 * @brief Get the VIS version associated with these codebooks.
		 * @return The `VisVersion` enum value.
		 */
		[[nodiscard]] inline VisVersion visVersion() const noexcept;
		[[nodiscard]] inline const Gmod* gmod() const noexcept;
		[[nodiscard]] inline const GmodNode& node() const;
		[[nodiscard]] inline const std::vector<GmodNode>& parents() const noexcept;
		[[nodiscard]] inline size_t length() const noexcept;

		/**
		 * @brief Gets all individualizable sets in this path
		 * @return Vector of individualizable sets that can be modified independently
		 */
		[[nodiscard]] std::vector<GmodIndividualizableSet> individualizableSets() const;

		/**
		 * @brief Gets the normal assignment name for a node at specified depth
		 * @param nodeDepth The depth of the node to get assignment name for (0-based index)
		 * @return Optional assignment name, nullopt if not found
		 */
		[[nodiscard]] std::optional<std::string> normalAssignmentName( size_t nodeDepth ) const;

		/**
		 * @brief Gets common names for all function nodes in the path
		 * @return Vector of pairs containing depth and common name
		 */
		[[nodiscard]] std::vector<std::pair<size_t, std::string>> commonNames() const;

		/**
		 * @brief Calculates a hash code for this GmodPath.
		 * @details The hash code is based on the path's target node, parents hierarchy, and locations.
		 *          Paths that are equal according to `operator==` must produce the same hash code.
		 * @return A `size_t` hash code value suitable for use in hash-based containers.
		 */
		[[nodiscard]] VISTA_SDK_CPP_FORCE_INLINE int hashCode() const noexcept;

		//----------------------------------------------
		// State inspection methods
		//----------------------------------------------

		/**
		 * @brief Validates the hierarchical relationship between parents and target node
		 * @param parents Vector of parent nodes in hierarchical order
		 * @param node The target node to validate
		 * @return True if the path is valid, false otherwise
		 */
		[[nodiscard]] inline static bool isValid( const std::vector<GmodNode*>& parents, const GmodNode& node );

		/**
		 * @brief Validates path and identifies where the hierarchy breaks
		 * @param parents Vector of parent nodes in hierarchical order
		 * @param node The target node to validate
		 * @param missingLinkAt [out] Index where validation failed (-1 if valid)
		 * @return True if the path is valid, false otherwise
		 */
		[[nodiscard]] static bool isValid( const std::vector<GmodNode*>& parents, const GmodNode& node, int& missingLinkAt );

		/**
		 * @brief Checks if this path can be mapped to physical locations
		 * @return True if the target node is mappable, false otherwise
		 */
		[[nodiscard]] inline bool isMappable() const;

		/**
		 * @brief Checks if this path contains individualizable nodes
		 * @return True if any nodes can be individualized, false otherwise
		 */
		[[nodiscard]] bool isIndividualizable() const;

		//----------------------------------------------
		// String conversion methods
		//----------------------------------------------

		/**
		 * @brief Converts the path to its short string representation
		 * @return Short path string showing only leaf nodes and target
		 */
		[[nodiscard]] inline std::string toString() const;

		/**
		 * @brief Writes the path's string representation to a generic output iterator.
		 * @tparam OutputIt The type of the output iterator.
		 * @param out The output iterator to write to.
		 * @param separator Character to use between path segments (default: '/').
		 * @return An iterator pointing to the end of the written output.
		 */
		template <typename OutputIt>
		inline OutputIt toString( OutputIt out, char separator = '/' ) const;

		/**
		 * @brief Converts the path to full hierarchical string representation
		 * @return Complete path string from root to target
		 */
		[[nodiscard]] std::string toFullPathString() const;

		/**
		 * @brief Writes the path's full hierarchical string representation to a generic output iterator.
		 * @tparam OutputIt The type of the output iterator.
		 * @param out The output iterator to write to.
		 * @return An iterator pointing to the end of the written output.
		 */
		template <typename OutputIt>
		inline OutputIt toFullPathString( OutputIt out ) const;

		/**
		 * @brief Creates detailed debug representation of the path
		 * @return Detailed string with codes, names, and assignment names
		 */
		[[nodiscard]] inline std::string toStringDump() const;

		/**
		 * @brief Writes the path's detailed debug representation to a generic output iterator.
		 * @tparam OutputIt The type of the output iterator.
		 * @param out The output iterator to write to.
		 * @return An iterator pointing to the end of the written output.
		 */
		template <typename OutputIt>
		inline OutputIt toStringDump( OutputIt out ) const;

		//----------------------------------------------
		// Path manipulation methods
		//----------------------------------------------

		/**
		 * @brief Creates a copy of this path with all location information removed
		 * @return New GmodPath instance without any location data
		 */
		[[nodiscard]] GmodPath withoutLocations() const;

		//----------------------------------------------
		// Path enumeration methods
		//----------------------------------------------

		/**
		 * @brief Gets the full hierarchical path with all intermediate nodes
		 * @return Enumerator for the complete path from root to target
		 */
		Enumerator inline fullPath() const;

		/**
		 * @brief Gets the full hierarchical path starting from a specific depth
		 * @param fromDepth The depth to start enumeration from (0 = root, -1 = from beginning)
		 * @return Enumerator starting from the specified depth
		 */
		Enumerator inline fullPathFrom( size_t fromDepth ) const;

		//----------------------------------------------
		// Parsing methods
		//----------------------------------------------

		/**
		 * @brief Parses a path string using specified VIS version
		 * @param item The path string to parse
		 * @param visVersion The VIS version to use for parsing
		 * @return Parsed GmodPath instance
		 * @throws std::invalid_argument if parsing fails
		 */
		[[nodiscard]] VISTA_SDK_CPP_FORCE_INLINE static GmodPath parse( std::string_view item, VisVersion visVersion );
		[[nodiscard]] VISTA_SDK_CPP_FORCE_INLINE static GmodPath parse( std::string_view item, const Gmod& gmod, const Locations& locations );
		[[nodiscard]] VISTA_SDK_CPP_FORCE_INLINE static GmodPath parseFullPath( std::string_view item, VisVersion visVersion );

		/**
		 * @brief Attempts to parse a path string, returning success/failure
		 * @param item The path string to parse
		 * @param visVersion The VIS version to use for parsing
		 * @param outPath [out] The parsed path if successful
		 * @return True if parsing succeeded, false otherwise
		 */
		[[nodiscard]] VISTA_SDK_CPP_FORCE_INLINE static bool tryParse( std::string_view item, VisVersion visVersion, std::optional<GmodPath>& outPath );
		[[nodiscard]] VISTA_SDK_CPP_FORCE_INLINE static bool tryParse( std::string_view item, const Gmod& gmod, const Locations& locations, std::optional<GmodPath>& outPath );

		[[nodiscard]] VISTA_SDK_CPP_FORCE_INLINE static bool tryParseFullPath( std::string_view item, VisVersion visVersion, std::optional<GmodPath>& outPath );
		[[nodiscard]] VISTA_SDK_CPP_FORCE_INLINE static bool tryParseFullPath( std::string_view item, const Gmod& gmod, const Locations& locations, std::optional<GmodPath>& outPath );

		//----------------------------------------------
		// Enumeration
		//----------------------------------------------

		[[nodiscard]] inline Enumerator enumerator( size_t fromDepth = std::numeric_limits<size_t>::max() ) const;

	private:
		//----------------------------------------------
		// Private member variables
		//----------------------------------------------

		VisVersion m_visVersion;
		const Gmod* m_gmod;
		std::optional<GmodNode> m_node;
		std::vector<GmodNode> m_parents;

	private:
		//----------------------------------------------
		// Private static parsing methods
		//----------------------------------------------

		/**
		 * @brief Internal helper for parsing partial path strings with traversal
		 * @param item The path string to parse (allows partial paths)
		 * @param gmod The GMOD instance for node lookup and traversal
		 * @param locations The locations instance for location parsing
		 * @return Parse result with either success path or error message
		 * @details Uses GMOD traversal to find complete hierarchical path from partial input
		 */
		static GmodParsePathResult parseInternal(
			std::string_view item, const Gmod& gmod, const Locations& locations );

		/**
		 * @brief Internal helper for parsing full path strings
		 * @param item The path string to parse
		 * @param gmod The GMOD instance for node lookup
		 * @param locations The locations instance for location parsing
		 * @return Parse result with either success path or error message
		 */
		static GmodParsePathResult parseFullPathInternal(
			std::string_view item, const Gmod& gmod, const Locations& locations );

	public:
		//----------------------------------------------
		// GmodPath::enumerator
		//----------------------------------------------

		class Enumerator final
		{
			friend class GmodPath;

		public:
			//----------------------------
			// Types definitions
			//----------------------------

			/** @brief Represents a single path element entry. */
			using PathElement = std::pair<size_t, const GmodNode*>;

		private:
			//----------------------------
			// Construction
			//----------------------------

			/**
			 * @brief Internal constructor for creating Enumerator.
			 * @param pathInst Pointer to the GmodPath to enumerate.
			 * @param startIndex Starting index for enumeration.
			 */
			Enumerator( const GmodPath* pathInst, size_t startIndex = std::numeric_limits<size_t>::max() );

		public:
			/** @brief Default constructor */
			Enumerator() = delete;

			/** @brief Copy constructor */
			Enumerator( const Enumerator& ) = default;

			/** @brief Move constructor */
			Enumerator( Enumerator&& ) noexcept = default;

			//----------------------------
			// Destruction
			//----------------------------

			/** @brief Destructor */
			~Enumerator() = default;

			//----------------------------
			// Assignment operators
			//----------------------------

			/** @brief Copy assignment operator */
			Enumerator& operator=( const Enumerator& ) = default;

			/** @brief Move assignment operator */
			Enumerator& operator=( Enumerator&& ) noexcept = default;

			//----------------------------
			// Enumeration methods
			//----------------------------

			/**
			 * @brief Advances the enumerator to the next element.
			 * @return True if the enumerator successfully moved to the next element;
			 *         false if the enumerator has passed the end of the collection.
			 */
			inline bool next();

			/**
			 * @brief Gets the current element.
			 * @return The current path element (depth, node).
			 */
			inline const PathElement& current() const;

			/**
			 * @brief Resets the enumerator to its initial position before first element
			 */
			inline void reset();

		private:
			//----------------------------
			// Private member variables
			//----------------------------

			const GmodPath* m_pathInstance;
			size_t m_currentIndex;
			mutable PathElement m_current;
		};
	};

	//=====================================================================
	// GmodIndividualizableSet class
	//=====================================================================

	class GmodIndividualizableSet final
	{
	public:
		//----------------------------------------------
		// Construction
		//----------------------------------------------

		GmodIndividualizableSet( const std::vector<int>& nodeIndices, const GmodPath& sourcePath );

		GmodIndividualizableSet() = delete;
		GmodIndividualizableSet( const GmodIndividualizableSet& ) = delete;
		GmodIndividualizableSet( GmodIndividualizableSet&& ) noexcept = default;

		//----------------------------------------------
		// Destruction
		//----------------------------------------------

		~GmodIndividualizableSet() = default;

		//----------------------------------------------
		// Assignment operators
		//----------------------------------------------

		GmodIndividualizableSet& operator=( const GmodIndividualizableSet& ) = delete;
		GmodIndividualizableSet& operator=( GmodIndividualizableSet&& ) noexcept = default;

		//----------------------------------------------
		// Build
		//----------------------------------------------

		GmodPath build();

		//----------------------------------------------
		// Accessors
		//----------------------------------------------

		[[nodiscard]] std::vector<GmodNode*> nodes() const;
		[[nodiscard]] inline const std::vector<int>& nodeIndices() const noexcept;
		[[nodiscard]] inline std::optional<Location> location() const;

		//----------------------------------------------
		// Utility methods
		//----------------------------------------------

		void setLocation( const std::optional<Location>& location );
		[[nodiscard]] std::string toString() const;

	private:
		//----------------------------------------------
		// Private member variables
		//----------------------------------------------

		std::vector<int> m_nodeIndices;
		GmodPath m_path;
		bool m_isBuilt;
	};

	//=====================================================================
	// GmodParsePathResult
	//=====================================================================

	struct GmodParsePathResult
	{
		struct Ok
		{
			GmodPath path;
			inline explicit Ok( GmodPath p );
		};

		struct Error
		{
			std::string error;
			inline explicit Error( std::string e );
		};

		std::variant<Ok, Error> result;

		inline GmodParsePathResult( Ok ok );
		inline GmodParsePathResult( Error err );

		inline bool isOk() const noexcept;
		inline bool isError() const noexcept;

		inline Ok& ok();
		inline const Ok& ok() const;

		inline Error& error();
		inline const Error& error() const;
	};
}

#include "GmodPath.inl"
