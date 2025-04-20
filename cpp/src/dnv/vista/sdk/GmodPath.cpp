#include "pch.h"

#include "dnv/vista/sdk/GmodPath.h"

#include "dnv/vista/sdk/Codebook.h"
#include "dnv/vista/sdk/Gmod.h"
#include "dnv/vista/sdk/Gmodnode.h"
#include "dnv/vista/sdk/Locations.h"
#include "dnv/vista/sdk/MetadataTag.h"
#include "dnv/vista/sdk/VIS.h"
#include "dnv/vista/sdk/VisVersion.h"

namespace dnv::vista::sdk
{
	GmodPath::GmodPath( std::vector<GmodNode> parents, GmodNode node, bool skipVerify )
		: m_parents( std::move( parents ) ),
		  m_node( std::move( node ) ),
		  m_visVersion( node.visVersion() ) // TODO: C26800
	{
		SPDLOG_INFO( "Creating GmodPath with {} parent nodes, target node '{}' (skipVerify={})",
			m_parents.size(), m_node.code(), skipVerify );

		if ( !skipVerify )
		{
			SPDLOG_INFO( "Validating GmodPath parent-child relationships" );
			int missingLinkAt;
			if ( !isValid( m_parents, m_node, missingLinkAt ) )
			{
				SPDLOG_ERROR( "Invalid GmodPath: Missing link at depth {}", missingLinkAt );
				throw std::invalid_argument(
					"Invalid path: Missing link at depth " +
					std::to_string( missingLinkAt ) );
			}
			SPDLOG_INFO( "GmodPath validation successful" );
		}
		else
		{
			SPDLOG_INFO( "Skipping path validation as requested" );
		}
	}

	GmodPath::GmodPath( const GmodPath& other )
		: m_parents( other.m_parents ),
		  m_node( other.m_node ),
		  m_visVersion( other.m_visVersion )
	{
		SPDLOG_INFO( "Creating GmodPath via copy constructor (parents={}, node='{}')",
			other.m_parents.size(), other.m_node.code() );
	}

	GmodPath::GmodPath( GmodPath&& other ) noexcept
		: m_parents( std::move( other.m_parents ) ),
		  m_node( std::move( other.m_node ) ),
		  m_visVersion( other.m_visVersion )
	{
		SPDLOG_INFO( "Moving GmodPath (parents={}, node='{}')",
			m_parents.size(), m_node.code() );
	}

	GmodPath::~GmodPath()
	{
	}

	GmodPath& GmodPath::operator=( const GmodPath& other )
	{
		SPDLOG_INFO( "Assigning GmodPath (parents={}, node='{}')",
			other.m_parents.size(), other.m_node.code() );

		if ( this != &other )
		{
			m_parents = other.m_parents;
			m_node = other.m_node;
			m_visVersion = other.m_visVersion;
		}
		return *this;
	}

	GmodPath& GmodPath::operator=( GmodPath&& other ) noexcept
	{
		if ( this != &other )
		{
			SPDLOG_INFO( "Move-assigning GmodPath from (parents={}, node='{}')",
				other.m_parents.size(), other.m_node.code() );

			m_parents = std::move( other.m_parents );
			m_node = std::move( other.m_node );
			m_visVersion = other.m_visVersion;
		}
		return *this;
	}

	bool GmodPath::operator==( const GmodPath& other ) const
	{
		SPDLOG_INFO( "Comparing GmodPaths for equality (this: {}, other: {})",
			m_node.code(), other.m_node.code() );

		if ( m_visVersion != other.m_visVersion )
			return false;

		if ( m_node != other.m_node )
			return false;

		if ( m_parents.size() != other.m_parents.size() )
			return false;

		for ( size_t i = 0; i < m_parents.size(); ++i )
		{
			if ( m_parents[i] != other.m_parents[i] )
				return false;
		}

		return true;
	}

	bool GmodPath::operator!=( const GmodPath& other ) const
	{
		SPDLOG_INFO( "Comparing GmodPaths for inequality" );
		return !( *this == other );
	}

	const GmodNode& GmodPath::operator[]( int depth ) const
	{
		if ( depth < 0 )
		{
			SPDLOG_ERROR( "GmodPath::operator[]: Negative depth {}", depth );
			throw std::out_of_range( "Depth cannot be negative" );
		}

		if ( depth < static_cast<int>( m_parents.size() ) )
		{
			return m_parents[depth];
		}

		if ( depth == static_cast<int>( m_parents.size() ) )
		{
			return m_node;
		}

		SPDLOG_ERROR( "GmodPath::operator[]: Depth {} exceeds path length {}",
			depth, m_parents.size() + 1 );
		throw std::out_of_range( "Depth exceeds path length" );
	}

	GmodNode& GmodPath::operator[]( int depth )
	{
		if ( depth < 0 )
		{
			SPDLOG_ERROR( "GmodPath::operator[]: Negative depth {}", depth );
			throw std::out_of_range( "Depth cannot be negative" );
		}

		if ( depth < static_cast<int>( m_parents.size() ) )
		{
			return m_parents[depth];
		}

		if ( depth == static_cast<int>( m_parents.size() ) )
		{
			return m_node;
		}

		SPDLOG_ERROR( "GmodPath::operator[]: Depth {} exceeds path length {}",
			depth, m_parents.size() + 1 );
		throw std::out_of_range( "Depth exceeds path length" );
	}

	size_t GmodPath::hashCode() const
	{
		SPDLOG_DEBUG( "Calculating hash code for GmodPath" );

		size_t hash = 17;

		// Hash the VIS version
		hash = hash * 31 + std::hash<int>{}( static_cast<int>( m_visVersion ) );

		// Hash each parent node
		for ( const auto& parent : m_parents )
			hash = hash * 31 + std::hash<std::string>{}( parent.code() );

		// Hash the target node
		hash = hash * 31 + std::hash<std::string>{}( m_node.code() );

		return hash;
	}

	std::vector<std::pair<int, GmodNode>> GmodPath::fullPath() const
	{
		SPDLOG_INFO( "Getting full path with {} parents + 1 target node", m_parents.size() );
		std::vector<std::pair<int, GmodNode>> result;
		result.reserve( m_parents.size() + 1 );

		for ( size_t i = 0; i < m_parents.size(); ++i )
		{
			result.emplace_back( static_cast<int>( i ), m_parents[i] );
		}

		result.emplace_back( static_cast<int>( m_parents.size() ), m_node );

		return result;
	}

	const std::vector<GmodNode>& GmodPath::parents() const
	{
		SPDLOG_INFO( "Getting parents vector (size={})", m_parents.size() );
		return m_parents;
	}

	const GmodNode& GmodPath::node() const
	{
		SPDLOG_INFO( "Getting target node: '{}'", m_node.code() );
		return m_node;
	}

	VisVersion GmodPath::visVersion() const
	{
		SPDLOG_INFO( "Getting VIS version: {}", static_cast<int>( m_visVersion ) );
		return m_visVersion;
	}

	int GmodPath::length() const
	{
		int length = static_cast<int>( m_parents.size() ) + 1;
		SPDLOG_INFO( "Getting path length: {}", length );
		return length;
	}

	bool GmodPath::isMappable() const
	{
		SPDLOG_INFO( "Checking if path with target node '{}' is mappable", m_node.code() );
		return m_node.isMappable();
	}

	GmodPath GmodPath::withoutLocations() const
	{
		SPDLOG_INFO( "Creating copy of path without locations (original path has {} parents)", m_parents.size() );

		std::vector<GmodNode> parentsWithoutLocations;
		parentsWithoutLocations.reserve( m_parents.size() );

		for ( const auto& parent : m_parents )
		{
			parentsWithoutLocations.push_back( parent.withoutLocation() );
		}

		GmodNode nodeWithoutLocation = m_node.withoutLocation();

		return GmodPath( std::move( parentsWithoutLocations ), std::move( nodeWithoutLocation ), true );
	}

	std::string GmodPath::toString() const
	{
		SPDLOG_INFO( "Converting path to string (node='{}')", m_node.code() );
		std::stringstream builder;
		toString( builder );
		return builder.str();
	}

	void GmodPath::toString( std::stringstream& builder, char separator ) const
	{
		SPDLOG_INFO( "Building path string with separator '{}'", separator );

		std::string nodeString = m_node.code();

		if ( m_node.location().has_value() )
		{
			nodeString += "-" + m_node.location().value().toString();
		}

		if ( m_parents.empty() )
		{
			builder << nodeString;
			return;
		}

		bool isFirst = true;

		for ( const auto& parent : m_parents )
		{
			if ( parent.code() == "ROOT" || parent.code().empty() )
				continue;

			if ( !isFirst )
				builder << separator;
			else
				isFirst = false;

			builder << parent.code();

			if ( parent.location().has_value() )
			{
				builder << "-" << parent.location().value().toString();
			}
		}

		if ( !isFirst )
			builder << separator;

		builder << nodeString;
	}

	std::string GmodPath::toFullPathString() const
	{
		SPDLOG_INFO( "Converting path to full path string (target='{}')", m_node.code() );
		std::stringstream builder;
		toFullPathString( builder );
		return builder.str();
	}

	void GmodPath::toFullPathString( std::stringstream& builder ) const
	{
		SPDLOG_INFO( "Building full path string including all parent nodes" );

		bool isFirst = true;

		for ( const auto& parent : m_parents )
		{
			if ( !isFirst )
				builder << '/';
			else
				isFirst = false;

			builder << parent.code();

			if ( parent.location().has_value() )
			{
				builder << "-" << parent.location().value().toString();
			}
		}

		if ( !isFirst )
			builder << '/';

		builder << m_node.code();

		if ( m_node.location().has_value() )
		{
			builder << "-" << m_node.location().value().toString();
		}
	}

	std::string GmodPath::toStringDump() const
	{
		SPDLOG_INFO( "Creating detailed string dump of path" );
		std::stringstream builder;
		toStringDump( builder );
		return builder.str();
	}

	void GmodPath::toStringDump( std::stringstream& builder ) const
	{
		SPDLOG_INFO( "Building detailed path dump" );

		builder << "GmodPath [VIS Version: " << static_cast<int>( m_visVersion ) << "]\n";
		builder << "Parents (" << m_parents.size() << "):\n";

		for ( size_t i = 0; i < m_parents.size(); ++i )
		{
			builder << "  [" << i << "] " << m_parents[i].code();

			if ( m_parents[i].location().has_value() )
			{
				builder << "-" << m_parents[i].location().value().toString();
			}

			builder << "\n";
		}

		builder << "Target: " << m_node.code();

		if ( m_node.location().has_value() )
		{
			builder << "-" << m_node.location().value().toString();
		}

		builder << " (Mappable: " << ( m_node.isMappable() ? "Yes" : "No" ) << ")";
	}

	std::optional<std::string> GmodPath::normalAssignmentName( int nodeDepth ) const
	{
		SPDLOG_INFO( "Getting normal assignment name for node at depth {}", nodeDepth );

		if ( nodeDepth < 0 || nodeDepth > static_cast<int>( m_parents.size() ) )
		{
			SPDLOG_INFO( "Node depth {} is out of range (0-{})",
				nodeDepth, m_parents.size() );
			return std::nullopt;
		}

		const GmodNode& node = ( *this )[nodeDepth];

		auto commonName = node.metadata().commonName();
		if ( commonName.has_value() )
		{
			SPDLOG_INFO( "Found common name '{}' for node at depth {}",
				commonName.value(), nodeDepth );
			return commonName;
		}

		SPDLOG_INFO( "No common name found for node at depth {}", nodeDepth );
		return std::nullopt;
	}

	std::vector<std::pair<int, std::string>> GmodPath::commonNames() const
	{
		SPDLOG_INFO( "Getting all common names in path" );
		std::vector<std::pair<int, std::string>> results;

		for ( size_t i = 0; i < m_parents.size(); ++i )
		{
			const GmodNode& node = m_parents[i];
			auto commonName = node.metadata().commonName();
			if ( commonName.has_value() )
			{
				SPDLOG_INFO( "Found common name '{}' for parent node at depth {}",
					commonName.value(), i );
				results.emplace_back( static_cast<int>( i ), commonName.value() );
			}
		}

		auto targetCommonName = m_node.metadata().commonName();
		if ( targetCommonName.has_value() )
		{
			int depth = static_cast<int>( m_parents.size() );
			SPDLOG_INFO( "Found common name '{}' for target node at depth {}",
				targetCommonName.value(), depth );
			results.emplace_back( depth, targetCommonName.value() );
		}

		return results;
	}

	bool GmodPath::isIndividualizable() const
	{
		SPDLOG_INFO( "Checking if path is individualizable" );

		auto sets = individualizableSets();
		bool result = !sets.empty();

		SPDLOG_INFO( "Path is individualizable: {}", result ? "Yes" : "No" );
		return result;
	}

	std::vector<GmodIndividualizableSet> GmodPath::individualizableSets() const
	{
		SPDLOG_INFO( "Getting individualizable sets for path" );
		std::vector<GmodIndividualizableSet> result;

		if ( m_parents.size() < 1 )
		{
			SPDLOG_INFO( "Path too short for individualization (needs at least 2 total nodes)" );
			return result;
		}

		std::vector<GmodNode> allNodes = m_parents;
		allNodes.push_back( m_node );

		int startIndex = -1;

		for ( size_t i = 0; i < allNodes.size(); ++i )
		{
			const auto& node = allNodes[i];

			bool canHaveLocation = node.isMappable();

			if ( canHaveLocation && startIndex == -1 )
			{
				startIndex = static_cast<int>( i );
				continue;
			}

			if ( ( !canHaveLocation || i == allNodes.size() - 1 ) && startIndex != -1 )
			{
				int endIndex = static_cast<int>( i );
				if ( canHaveLocation && i == allNodes.size() - 1 )
				{
					endIndex = static_cast<int>( i );
				}
				else
				{
					endIndex = static_cast<int>( i - 1 );
				}

				if ( endIndex - startIndex >= 1 )
				{
					std::vector<int> indices;
					for ( int i = startIndex; i <= endIndex; i++ )
					{
						indices.push_back( i );
					}
					GmodPath pathCopy = *this;
					result.emplace_back( indices, pathCopy );
					SPDLOG_INFO( "Added individualizable set from {} to {}", startIndex, endIndex );
				}

				startIndex = -1;
			}
		}

		SPDLOG_INFO( "Found {} individualizable sets in path", result.size() );
		return result;
	}

	bool GmodPath::isValid( const std::vector<GmodNode>& parents, const GmodNode& node )
	{
		int missingLinkAt;
		return isValid( parents, node, missingLinkAt );
	}

	bool GmodPath::isValid( const std::vector<GmodNode>& parents, const GmodNode& node, int& missingLinkAt )
	{
		SPDLOG_INFO( "Validating path with {} parents and target node '{}'",
			parents.size(), node.code() );

		missingLinkAt = -1;

		if ( parents.empty() )
		{
			SPDLOG_ERROR( "Invalid path: Parents list is empty" );
			missingLinkAt = 0;
			return false;
		}

		if ( !parents[0].isRoot() )
		{
			SPDLOG_ERROR( "Invalid path: First parent '{}' is not the root node",
				parents[0].code() );
			missingLinkAt = 0;
			return false;
		}

		for ( size_t i = 0; i < parents.size(); i++ )
		{
			const auto& parent = parents[i];
			size_t nextIndex = i + 1;
			const auto& child = nextIndex < parents.size() ? parents[nextIndex] : node;

			if ( !parent.isChild( child ) )
			{
				SPDLOG_ERROR( "Invalid path: '{}' is not a parent of '{}'",
					parent.code(), child.code() );
				missingLinkAt = static_cast<int>( i );
				return false;
			}
		}

		SPDLOG_INFO( "Path validation successful" );
		return true;
	}

	GmodPath GmodPath::parse( const std::string& item, VisVersion visVersion )
	{
		SPDLOG_INFO( "Parsing path '{}' with VIS version {}", item, static_cast<int>( visVersion ) );

		auto& vis = VIS::instance();
		const auto& gmod = vis.gmod( visVersion );
		auto locations = vis.locations( visVersion );

		return parse( item, gmod, locations );
	}

	bool GmodPath::tryParse( const std::string& item, VisVersion visVersion, std::optional<GmodPath>& path )
	{
		SPDLOG_INFO( "Attempting to parse path '{}' with VIS version {}",
			item, static_cast<int>( visVersion ) );

		try
		{
			auto& vis = VIS::instance();
			const auto& gmod = vis.gmod( visVersion );
			auto locations = vis.locations( visVersion );
			SPDLOG_WARN( "TRY" );
			GmodPath parsedPath = parse( item, gmod, locations );
			path = std::move( parsedPath );

			SPDLOG_INFO( "Successfully parsed path" );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Failed to parse path: {}", e.what() );
			path.reset();
			return false;
		}
	}

	GmodPath GmodPath::parse( const std::string& item, const Gmod& gmod, const Locations& locations )
	{
		SPDLOG_INFO( "Parsing path '{}' using provided GMOD and Locations", item );

		auto result = parseInternal( item, gmod, locations );

		auto* okResult = dynamic_cast<GmodParsePathResult::Ok*>( &result );
		if ( okResult != nullptr )
		{
			SPDLOG_INFO( "Successfully parsed path" );
			return std::move( okResult->path );
		}

		auto* errResult = dynamic_cast<GmodParsePathResult::Err*>( &result );
		if ( errResult == nullptr )
		{
			SPDLOG_ERROR( "Failed to parse path: unknown error" );
			throw std::invalid_argument( "Failed to parse path: unknown error" );
		}

		SPDLOG_ERROR( "Failed to parse path: {}", errResult->error );
		throw std::invalid_argument( "Failed to parse path: " + errResult->error );
	}

	bool GmodPath::tryParse( const std::string& item, const Gmod& gmod, const Locations& locations, GmodPath& path )
	{
		SPDLOG_INFO( "Attempting to parse path '{}' using provided GMOD and Locations", item );

		try
		{
			path = parse( item, gmod, locations );
			SPDLOG_INFO( "Successfully parsed path" );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Failed to parse path: {}", e.what() );
			return false;
		}
	}

	GmodPath GmodPath::parseFullPath( const std::string& pathStr, VisVersion visVersion )
	{
		SPDLOG_INFO( "Parsing full path '{}' with VIS version {}", pathStr, static_cast<int>( visVersion ) );

		auto& vis = VIS::instance();
		const auto& gmod = vis.gmod( visVersion );
		auto locations = vis.locations( visVersion );

		auto result = parseFullPathInternal( pathStr, gmod, locations );

		auto* okResult = dynamic_cast<GmodParsePathResult::Ok*>( &result );
		if ( okResult != nullptr )
		{
			SPDLOG_INFO( "Successfully parsed full path" );
			return std::move( okResult->path );
		}

		auto* errResult = dynamic_cast<GmodParsePathResult::Err*>( &result );
		SPDLOG_ERROR( "Failed to parse full path: {}", errResult->error );
		throw std::invalid_argument( "Failed to parse full path: " + errResult->error );
	}

	bool GmodPath::tryParseFullPath( const std::string& pathStr, VisVersion visVersion, GmodPath& path )
	{
		SPDLOG_INFO( "Attempting to parse full path '{}' with VIS version {}",
			pathStr, static_cast<int>( visVersion ) );

		try
		{
			path = parseFullPath( pathStr, visVersion );
			SPDLOG_INFO( "Successfully parsed full path" );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Failed to parse full path: {}", e.what() );
			return false;
		}
	}

	bool GmodPath::tryParseFullPath( std::string_view pathStr, VisVersion visVersion, GmodPath& path )
	{
		SPDLOG_INFO( "Attempting to parse full path '{}' with VIS version {}",
			pathStr, static_cast<int>( visVersion ) );

		try
		{
			auto& vis = VIS::instance();
			const auto& gmod = vis.gmod( visVersion );
			auto locations = vis.locations( visVersion );

			auto result = parseFullPathInternal( pathStr, gmod, locations );

			auto* okResult = dynamic_cast<GmodParsePathResult::Ok*>( &result );
			if ( okResult != nullptr )
			{
				path = std::move( okResult->path );
				SPDLOG_INFO( "Successfully parsed full path" );
				return true;
			}

			auto* errResult = dynamic_cast<GmodParsePathResult::Err*>( &result );
			SPDLOG_ERROR( "Failed to parse full path: {}", errResult->error );
			return false;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Failed to parse full path: {}", e.what() );
			return false;
		}
	}

	bool GmodPath::tryParseFullPath( std::string_view pathStr, const Gmod& gmod, const Locations& locations, GmodPath& path )
	{
		SPDLOG_INFO( "Attempting to parse full path '{}' using provided GMOD and Locations", pathStr );

		try
		{
			auto result = parseFullPathInternal( pathStr, gmod, locations );

			auto* okResult = dynamic_cast<GmodParsePathResult::Ok*>( &result );
			if ( okResult != nullptr )
			{
				path = std::move( okResult->path );
				SPDLOG_INFO( "Successfully parsed full path" );
				return true;
			}

			auto* errResult = dynamic_cast<GmodParsePathResult::Err*>( &result );
			SPDLOG_ERROR( "Failed to parse full path: {}", errResult->error );
			return false;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Failed to parse full path: {}", e.what() );
			return false;
		}
	}

	struct LocationSetsVisitor
	{
		int currentParentStart = -1;

		std::optional<std::tuple<int, int, std::optional<Location>>> Visit(
			const GmodNode& node,
			int i,
			const std::vector<GmodNode>& parents,
			const GmodNode& target )
		{
			bool isTargetNode = ( i == static_cast<int>( parents.size() ) );
			bool isParent = !Gmod::isLeafNode( node.metadata() );
			std::optional<std::tuple<int, int, std::optional<Location>>> nodes = std::nullopt;

			if ( currentParentStart == -1 )
			{
				if ( isParent )
				{
					currentParentStart = i;
				}

				if ( node.isIndividualizable( isTargetNode ) )
				{
					nodes = std::make_tuple( i, i, node.location() );
				}
			}
			else
			{
				if ( isParent || isTargetNode )
				{
					if ( currentParentStart + 1 == i )
					{
						if ( node.isIndividualizable( isTargetNode ) )
						{
							nodes = std::make_tuple( currentParentStart, i, node.location() );
						}
						else
						{
							currentParentStart = -1;
						}
					}
					else
					{
						nodes = std::make_tuple( currentParentStart, i - 1, std::nullopt );
						currentParentStart = i;
					}
				}
				else
				{
					if ( currentParentStart != -1 )
					{
						nodes = std::make_tuple( currentParentStart, i - 1, std::nullopt );
						currentParentStart = -1;
					}
				}

				if ( isTargetNode && node.isIndividualizable( isTargetNode ) )
				{
					if ( currentParentStart != -1 && !nodes.has_value() )
					{
						nodes = std::make_tuple( currentParentStart, i, node.location() );
					}
					else if ( currentParentStart == -1 )
					{
						nodes = std::make_tuple( i, i, node.location() );
					}
				}
			}

			return nodes;
		}
	};

	GmodParsePathResult GmodPath::parseInternal( const std::string& item, const Gmod& gmod, const Locations& locations )
	{
		try
		{
			if ( gmod.visVersion() != locations.visVersion() )
			{
				return GmodParsePathResult::Err( "Got different VIS versions for Gmod and Locations arguments" );
			}

			std::queue<PathNode> parts;
			std::istringstream ss( item );
			std::string partStr;

			SPDLOG_INFO( "Parsing path: '{}'", item );

			while ( std::getline( ss, partStr, '/' ) )
			{
				size_t dashPos = partStr.find( '-' );
				if ( dashPos != std::string::npos )
				{
					std::string code = partStr.substr( 0, dashPos );
					std::string locStr = partStr.substr( dashPos + 1 );

					GmodNode node;
					if ( !gmod.tryGetNode( code, node ) )
					{
						return GmodParsePathResult::Err( "Failed to get GmodNode for " + partStr );
					}

					Location location;
					if ( !locations.tryParse( locStr, location ) )
					{
						return GmodParsePathResult::Err( "Failed to parse location " + locStr );
					}

					parts.emplace( code, std::make_optional<Location>( location ) );
					SPDLOG_INFO( "Added part with location: code='{}', loc='{}', original='{}'",
						code, locStr, partStr );
				}
				else
				{
					GmodNode node;
					if ( !gmod.tryGetNode( partStr, node ) )
					{
						return GmodParsePathResult::Err( "Failed to get GmodNode for " + partStr );
					}

					parts.emplace( partStr );
					SPDLOG_INFO( "Added part without location: '{}'", partStr );
				}
			}

			if ( parts.empty() )
			{
				return GmodParsePathResult::Err( "Failed to find any parts" );
			}

			PathNode toFind = parts.front();
			parts.pop();

			GmodNode baseNode;
			if ( !gmod.tryGetNode( toFind.code, baseNode ) )
			{
				return GmodParsePathResult::Err( "Failed to find base node" );
			}

			std::vector<std::string> nodeCodes;
			std::unordered_map<std::string, Location> nodeLocations;
			bool found = false;

			struct TraversalState
			{
				PathNode currentToFind;
				std::queue<PathNode> remainingParts;
				std::vector<std::string>* nodeCodes;
				std::unordered_map<std::string, Location>* nodeLocations;
				bool* found;
			};

			TraversalState state = {
				toFind,
				parts,
				&nodeCodes,
				&nodeLocations,
				&found };

			auto handler = [state]( const std::vector<GmodNode>& parents, const GmodNode& current ) mutable -> Gmod::TraversalHandlerResult {
				std::string nodeCode;
				try
				{
					nodeCode = current.code();
					SPDLOG_INFO( "Traversing node: '{}'", nodeCode );
				}
				catch ( const std::exception& e )
				{
					SPDLOG_ERROR( "Exception getting node code: {}", e.what() );
					return Gmod::TraversalHandlerResult::Stop;
				}

				if ( nodeCode != state.currentToFind.code )
				{
					SPDLOG_INFO( "Node '{}' does not match current to find '{}', continuing traversal",
						nodeCode, state.currentToFind.code );
					return Gmod::TraversalHandlerResult::Continue;
				}

				try
				{
					if ( current.code() != state.currentToFind.code )
					{
						return Gmod::TraversalHandlerResult::Continue;
					}

					if ( !( *state.found ) )
					{
						if ( state.nodeCodes->empty() )
						{
							try
							{
								state.nodeCodes->reserve( parents.size() + 1 );
							}
							catch ( const std::bad_alloc& )
							{
								SPDLOG_ERROR( "Memory allocation failed during path parsing" );
								return Gmod::TraversalHandlerResult::Stop;
							}

							for ( const auto& parent : parents )
							{
								state.nodeCodes->push_back( parent.code() );

								if ( parent.location() )
								{
									( *state.nodeLocations )[parent.code()] = *parent.location();
								}
							}

							state.nodeCodes->push_back( current.code() );
						}
						else
						{
							state.nodeCodes->push_back( current.code() );
						}

						if ( state.currentToFind.location )
						{
							( *state.nodeLocations )[current.code()] = *state.currentToFind.location;
						}
						else if ( current.location() )
						{
							( *state.nodeLocations )[current.code()] = *current.location();
						}
					}

					if ( !state.remainingParts.empty() )
					{
						state.currentToFind = state.remainingParts.front();
						state.remainingParts.pop();
						return Gmod::TraversalHandlerResult::Continue;
					}

					*state.found = true;
					return Gmod::TraversalHandlerResult::Stop;
				}
				catch ( const std::exception& ex )
				{
					SPDLOG_ERROR( "Exception in traversal handler: {}", ex.what() );
					return Gmod::TraversalHandlerResult::Stop;
				}
			};

			gmod.traverse( baseNode, handler );

			if ( !found )
			{
				return GmodParsePathResult::Err( "Failed to find path after traversal" );
			}

			std::vector<GmodNode> pathNodes;
			pathNodes.reserve( nodeCodes.size() - 1 );

			for ( size_t i = 0; i < nodeCodes.size() - 1; i++ )
			{
				std::string code = nodeCodes[i];
				GmodNode node;

				if ( !gmod.tryGetNode( code, node ) )
				{
					return GmodParsePathResult::Err( "Failed to get node for: " + code );
				}

				auto locIt = nodeLocations.find( code );
				if ( locIt != nodeLocations.end() )
				{
					node = node.withLocation( locIt->second );
				}

				pathNodes.push_back( node );
			}

			std::string targetCode = nodeCodes.back();
			GmodNode targetNode;

			if ( !gmod.tryGetNode( targetCode, targetNode ) )
			{
				return GmodParsePathResult::Err( "Failed to get target node for: " + targetCode );
			}

			auto targetLocIt = nodeLocations.find( targetCode );
			if ( targetLocIt != nodeLocations.end() )
			{
				targetNode = targetNode.withLocation( targetLocIt->second );
			}

			try
			{
				bool skipValidation = true;
				GmodPath path( pathNodes, targetNode, skipValidation );

				SPDLOG_INFO( "Successfully created GmodPath with {} parents and target node '{}'",
					pathNodes.size(), targetNode.code() );

				return GmodParsePathResult::Ok( std::move( path ) );
			}
			catch ( const std::exception& ex )
			{
				SPDLOG_ERROR( "Failed to create GmodPath: {}", ex.what() );
				return GmodParsePathResult::Err( "Failed to create GmodPath: " + std::string( ex.what() ) );
			}
		}
		catch ( const std::bad_alloc& ex )
		{
			SPDLOG_ERROR( "Memory allocation error in ParseInternal: {}", ex.what() );
			return GmodParsePathResult::Err( "Memory allocation error: " + std::string( ex.what() ) );
		}
		catch ( const std::exception& ex )
		{
			SPDLOG_ERROR( "Exception in ParseInternal: {}", ex.what() );
			return GmodParsePathResult::Err( "Exception: " + std::string( ex.what() ) );
		}
	}

	GmodParsePathResult GmodPath::parseFullPathInternal( std::string_view pathStr, const Gmod& gmod, const Locations& locations )
	{
		if ( pathStr.empty() || std::all_of( pathStr.begin(), pathStr.end(), []( char c ) { return std::isspace( c ); } ) )
		{
			return GmodParsePathResult::Err( "Item is empty" );
		}

		if ( !pathStr.starts_with( gmod.rootNode().code() ) )
		{
			return GmodParsePathResult::Err( "Path must start with " + gmod.rootNode().code() );
		}

		std::vector<GmodNode> nodes;
		std::string pathStrCopy( pathStr );
		std::istringstream ss( pathStrCopy );
		std::string part;

		while ( std::getline( ss, part, '/' ) )
		{
			size_t dashIndex = part.find( '-' );
			GmodNode node;

			if ( dashIndex == std::string::npos )
			{
				if ( !gmod.tryGetNode( part, node ) )
				{
					return GmodParsePathResult::Err( "Failed to get GmodNode for " + part );
				}
			}
			else
			{
				std::string code = part.substr( 0, dashIndex );
				std::string locationStr = part.substr( dashIndex + 1 );

				if ( !gmod.tryGetNode( code, node ) )
				{
					return GmodParsePathResult::Err( "Failed to get GmodNode for " + code );
				}

				Location location;
				if ( !locations.tryParse( locationStr, location ) )
				{
					return GmodParsePathResult::Err( "Failed to parse location - " + locationStr );
				}

				node = node.withLocation( location );
			}

			nodes.push_back( node );
		}

		if ( nodes.empty() )
		{
			return GmodParsePathResult::Err( "Failed to find any nodes" );
		}

		GmodNode endNode = nodes.back();
		nodes.pop_back();

		int missingLinkAt;
		if ( !isValid( nodes, endNode, missingLinkAt ) )
		{
			return GmodParsePathResult::Err( "Sequence of nodes are invalid" );
		}

		LocationSetsVisitor visitor;
		for ( size_t i = 0; i < nodes.size() + 1; i++ )
		{
			const GmodNode& node = i < nodes.size() ? nodes[i] : endNode;
			auto setResult = visitor.Visit( node, static_cast<int>( i ), nodes, endNode );

			if ( setResult )
			{
				auto [start, end, location] = *setResult;

				for ( int j = start; j <= end; j++ )
				{
					if ( j < static_cast<int>( nodes.size() ) )
					{
						if ( location )
						{
							nodes[j] = nodes[j].withLocation( *location );
						}
						else
						{
							nodes[j] = nodes[j].withoutLocation();
						}
					}
					else
					{
						if ( location )
						{
							endNode = endNode.withLocation( *location );
						}
						else
						{
							endNode = endNode.withoutLocation();
						}
					}
				}
			}
		}

		return GmodParsePathResult::Ok( GmodPath( nodes, endNode, true ) );
	}

	GmodParsePathResult::Ok::Ok( const GmodPath& p ) : path( p )
	{
	}

	GmodParsePathResult::Ok::Ok( GmodPath&& p ) : path( std::move( p ) )
	{
	}

	GmodParsePathResult::Err::Err( const std::string& errorMessage ) : error( errorMessage )
	{
	}

	GmodIndividualizableSet::GmodIndividualizableSet( const std::vector<int>& nodes, GmodPath& path )
		: m_nodes( nodes ), m_path( &path )
	{
	}

	std::vector<GmodNode> GmodIndividualizableSet::nodes() const
	{
		SPDLOG_INFO( "Getting nodes for individualizable set with {} indices", m_nodes.size() );

		std::vector<GmodNode> result;
		result.reserve( m_nodes.size() );

		for ( int index : m_nodes )
		{
			try
			{
				result.push_back( ( *m_path )[index] );
				SPDLOG_INFO( "Added node '{}' at index {}", ( *m_path )[index].code(), index );
			}
			catch ( const std::exception& e )
			{
				SPDLOG_ERROR( "Failed to get node at index {}: {}", index, e.what() );
			}
		}

		SPDLOG_INFO( "Returning {} nodes from individualizable set", result.size() );
		return result;
	}

	const std::vector<int>& GmodIndividualizableSet::nodeIndices() const
	{
		return m_nodes;
	}

	std::optional<Location> GmodIndividualizableSet::location() const
	{
		SPDLOG_INFO( "Getting location for individualizable set with {} indices", m_nodes.size() );

		if ( m_nodes.empty() )
		{
			SPDLOG_INFO( "Individualizable set is empty, no location to return" );
			return std::nullopt;
		}

		std::optional<Location> firstLocation;
		try
		{
			firstLocation = ( *m_path )[m_nodes[0]].location();
			SPDLOG_INFO( "First node (index {}) location: {}",
				m_nodes[0],
				firstLocation.has_value() ? firstLocation.value().toString() : "none" );
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Failed to get location from first node at index {}: {}",
				m_nodes[0], e.what() );
			return std::nullopt;
		}

		for ( size_t i = 1; i < m_nodes.size(); ++i )
		{
			try
			{
				const auto& nodeLocation = ( *m_path )[m_nodes[i]].location();

				if ( nodeLocation != firstLocation )
				{
					SPDLOG_WARN( "Node at index {} has different location than first node", m_nodes[i] );
					return firstLocation;
				}
			}
			catch ( const std::exception& e )
			{
				SPDLOG_ERROR( "Failed to get location from node at index {}: {}",
					m_nodes[i], e.what() );
				continue;
			}
		}

		SPDLOG_INFO( "Returning common location for individualizable set" );
		return firstLocation;
	}

	void GmodIndividualizableSet::setLocation( const std::optional<Location>& location )
	{
		SPDLOG_INFO( "Setting location {} for individualizable set with {} indices",
			location.has_value() ? location.value().toString() : "null",
			m_nodes.size() );

		if ( m_nodes.empty() )
		{
			SPDLOG_INFO( "Individualizable set is empty, nothing to set" );
			return;
		}

		for ( int index : m_nodes )
		{
			try
			{
				GmodNode& node = ( *m_path )[index];

				GmodNode updatedNode = node.tryWithLocation( location );

				( *m_path )[index] = updatedNode;

				SPDLOG_INFO( "Set location for node '{}' at index {}",
					node.code(),
					index );
			}
			catch ( const std::exception& e )
			{
				SPDLOG_ERROR( "Failed to set location for node at index {}: {}",
					index, e.what() );
			}
		}

		SPDLOG_INFO( "Finished setting location for individualizable set" );
	}

	GmodPath GmodIndividualizableSet::build()
	{
		SPDLOG_INFO( "Building path from individualizable set" );

		if ( !m_path )
		{
			SPDLOG_ERROR( "Cannot build path: m_path is null" );
			throw std::runtime_error( "Cannot build path: m_path is null" );
		}

		return *m_path;
	}

	std::string GmodIndividualizableSet::toString() const
	{
		SPDLOG_INFO( "Converting individualizable set to string representation" );

		std::stringstream ss;
		ss << "GmodIndividualizableSet { Nodes: " << m_nodes.size();

		if ( !m_nodes.empty() )
		{
			ss << ", Indices: [";
			for ( size_t i = 0; i < m_nodes.size(); ++i )
			{
				if ( i > 0 )
					ss << ", ";
				ss << m_nodes[i];
			}
			ss << "]";

			if ( m_path )
			{
				ss << ", Nodes: [";
				bool first = true;
				for ( int index : m_nodes )
				{
					try
					{
						if ( !first )
							ss << ", ";
						first = false;

						const GmodNode& node = ( *m_path )[index];
						ss << node.code();

						if ( node.location().has_value() )
							ss << "-" << node.location().value().toString();
					}
					catch ( const std::exception& )
					{
						if ( !first )
							ss << ", ";
						first = false;
						ss << "?";
					}
				}
				ss << "]";

				auto loc = location();
				ss << ", CommonLocation: " << ( loc.has_value() ? loc.value().toString() : "none" );
			}
			else
			{
				ss << ", Path: null";
			}
		}

		ss << " }";
		return ss.str();
	}

	ParseContext::ParseContext( std::queue<PathNode> parts )
		: parts( std::move( parts ) ),
		  toFind(),
		  locations(),
		  path( std::nullopt )
	{
	}

	PathNode::PathNode( const std::string& code, const std::optional<Location>& location )
		: code( code ),
		  location( location )
	{
	}

	GmodPath::Enumerator::Enumerator( const GmodPath& path, std::optional<int> fromDepth )
		: m_path( path ),
		  m_current( -1 ),
		  m_depth( fromDepth.value_or( 0 ) ),
		  m_fromDepth( fromDepth )
	{
	}

	bool GmodPath::Enumerator::moveNext()
	{
		m_current++;

		if ( m_current >= static_cast<int>( m_path.length() ) )
		{
			SPDLOG_INFO( "Enumerator reached end of path (length={})", m_path.length() );
			return false;
		}

		if ( m_current > 0 )
		{
			m_depth++;
		}

		if ( m_fromDepth.has_value() && m_current < m_fromDepth.value() )
		{
			SPDLOG_INFO( "Skipping node at depth {} (below fromDepth={})",
				m_current, m_fromDepth.value() );
			return moveNext();
		}

		SPDLOG_INFO( "Enumerator advanced to position {} at depth {}", m_current, m_depth );
		return true;
	}

	std::pair<int, std::reference_wrapper<const GmodNode>> GmodPath::Enumerator::current() const
	{
		if ( m_current < 0 || m_current > static_cast<int>( m_path.length() - 1 ) )
		{
			SPDLOG_ERROR( "Enumerator::GetCurrent called with invalid position (m_current={})", m_current );
			throw std::out_of_range( "Enumerator position is invalid" );
		}

		return std::make_pair( m_depth, std::cref( m_path[m_current] ) );
	}

	GmodPath::Enumerator::Iterator GmodPath::Enumerator::begin()
	{
		return Iterator( *this, false );
	}

	GmodPath::Enumerator::Iterator GmodPath::Enumerator::end()
	{
		return Iterator( *this, true );
	}

	GmodPath::Enumerator::Iterator::Iterator( Enumerator& enumerator, bool end )
		: m_enumerator( enumerator ),
		  m_end( end )
	{
		if ( !m_end )
		{
			m_enumerator.m_current = -1;
			m_enumerator.m_depth = m_enumerator.m_fromDepth.value_or( 0 );

			if ( !m_enumerator.moveNext() )
			{
				m_end = true;
			}
		}
	}
}
