#include "pch.h"

#include "dnv/vista/sdk/GmodPath.h"

#include "dnv/vista/sdk/Codebook.h"
#include "dnv/vista/sdk/Gmod.h"
#include "dnv/vista/sdk/Locations.h"
#include "dnv/vista/sdk/VIS.h"
#include "dnv/vista/sdk/VisVersion.h"

namespace dnv::vista::sdk
{
	GmodPath::GmodPath( std::vector<GmodNode> parents, GmodNode node, bool skipVerify )
		: m_parents( std::move( parents ) ), m_node( std::move( node ) ), m_visVersion( node.GetVisVersion() )
	{
		if ( !skipVerify )
		{
			if ( m_parents.empty() )
			{
				SPDLOG_ERROR( "Invalid GmodPath: no parents, and the node is not the root of the Gmod." );
				throw std::invalid_argument( "Invalid GmodPath: no parents, and the node is not the root of the Gmod." );
			}

			if ( !m_parents.front().IsRoot() )
			{
				SPDLOG_ERROR( "Invalid GmodPath: the first parent should be the root of the Gmod." );
				throw std::invalid_argument( "Invalid GmodPath: the first parent should be the root of the Gmod." );
			}

			for ( size_t i = 0; i < m_parents.size(); ++i )
			{
				const auto& parent = m_parents[i];
				const auto& child = ( i + 1 < m_parents.size() ) ? m_parents[i + 1] : m_node;

				if ( !parent.IsChild( child ) )
				{
					SPDLOG_ERROR( "Invalid GmodPath: a child node is not linked to its parent." );
					throw std::invalid_argument( "Invalid GmodPath: a child node is not linked to its parent." );
				}
			}
		}
	}

	GmodPath::GmodPath( const GmodPath& other )
		: m_parents( other.m_parents ), m_node( other.m_node ), m_visVersion( other.m_visVersion )
	{
	}

	GmodPath& GmodPath::operator=( const GmodPath& other )
	{
		if ( this != &other )
		{
			m_parents = other.m_parents;
			m_node = other.m_node;
			m_visVersion = other.m_visVersion;
		}
		return *this;
	}

	std::vector<std::pair<int, GmodNode>> GmodPath::GetFullPath() const
	{
		std::vector<std::pair<int, GmodNode>> fullPath;
		fullPath.reserve( m_parents.size() + 1 );

		for ( size_t i = 0; i < m_parents.size(); ++i )
		{
			fullPath.emplace_back( static_cast<int>( i ), m_parents[i] );
		}

		fullPath.emplace_back( static_cast<int>( m_parents.size() ), m_node );

		return fullPath;
	}

	const std::vector<GmodNode>& GmodPath::GetParents() const
	{
		return m_parents;
	}

	const GmodNode& GmodPath::GetNode() const
	{
		return m_node;
	}

	VisVersion GmodPath::GetVisVersion() const
	{
		return m_visVersion;
	}

	int GmodPath::GetLength() const
	{
		return static_cast<int>( m_parents.size() + 1 );
	}

	bool GmodPath::IsMappable() const
	{
		return m_node.IsMappable();
	}

	const GmodNode& GmodPath::operator[]( int depth ) const
	{
		if ( depth < 0 || depth > static_cast<int>( m_parents.size() ) )
		{
			SPDLOG_ERROR( "Index out of range for GmodPath indexer" );
			throw std::out_of_range( "Index out of range for GmodPath indexer" );
		}
		return ( depth < static_cast<int>( m_parents.size() ) ) ? m_parents[depth] : m_node;
	}

	GmodNode& GmodPath::operator[]( int depth )
	{
		if ( depth < 0 || depth > static_cast<int>( m_parents.size() ) )
		{
			SPDLOG_ERROR( "Index out of range for GmodPath indexer" );
			throw std::out_of_range( "Index out of range for GmodPath indexer" );
		}
		return ( depth < static_cast<int>( m_parents.size() ) ) ? m_parents[depth] : m_node;
	}

	GmodPath GmodPath::WithoutLocations() const
	{
		std::vector<GmodNode> parentsWithoutLocations;
		parentsWithoutLocations.reserve( m_parents.size() );

		for ( const auto& parent : m_parents )
		{
			parentsWithoutLocations.push_back( parent.WithoutLocation() );
		}

		return GmodPath( parentsWithoutLocations, m_node.WithoutLocation() );
	}

	std::string GmodPath::ToString() const
	{
		std::stringstream builder;
		ToString( builder );
		return builder.str();
	}

	void GmodPath::ToString( std::stringstream& builder, char separator ) const
	{
		for ( const auto& parent : m_parents )
		{
			if ( !parent.IsLeafNode() )
				continue;

			builder << parent.ToString() << separator;
		}

		builder << m_node.ToString();
	}

	std::string GmodPath::ToFullPathString() const
	{
		std::stringstream builder;
		ToFullPathString( builder );
		return builder.str();
	}

	void GmodPath::ToFullPathString( std::stringstream& builder ) const
	{
		for ( size_t i = 0; i < m_parents.size(); ++i )
		{
			builder << m_parents[i].ToString();
			if ( i != m_parents.size() - 1 || !m_node.ToString().empty() )
			{
				builder << '/';
			}
		}

		builder << m_node.ToString();
	}

	std::string GmodPath::ToStringDump() const
	{
		std::stringstream builder;
		ToStringDump( builder );
		return builder.str();
	}

	void GmodPath::ToStringDump( std::stringstream& builder ) const
	{
		for ( size_t i = 0; i < m_parents.size(); ++i )
		{
			if ( i > 0 )
			{
				builder << " | ";
			}

			builder << m_parents[i].ToString();

			if ( auto location = m_parents[i].GetLocation(); location.has_value() )
			{
				builder << "/L:" << location->ToString();
			}
		}

		if ( !m_parents.empty() )
		{
			builder << " | ";
		}

		builder << m_node.ToString();

		if ( auto location = m_node.GetLocation(); location.has_value() )
		{
			builder << "/L:" << location->ToString();
		}
	}

	bool GmodPath::operator==( const GmodPath& other ) const
	{
		if ( m_parents.size() != other.m_parents.size() )
		{
			return false;
		}

		for ( size_t i = 0; i < m_parents.size(); ++i )
		{
			if ( m_parents[i] != other.m_parents[i] )
			{
				return false;
			}
		}

		return m_node == other.m_node;
	}

	bool GmodPath::operator!=( const GmodPath& other ) const
	{
		return !( *this == other );
	}

	std::optional<std::string> GmodPath::GetNormalAssignmentName( int nodeDepth ) const
	{
		if ( nodeDepth < 0 || nodeDepth >= GetLength() )
		{
			SPDLOG_ERROR( "Node depth is out of range" );
			throw std::out_of_range( "Node depth is out of range" );
		}

		const auto& node = ( *this )[nodeDepth];
		const auto& normalAssignmentNames = node.GetMetadata().GetNormalAssignmentNames();

		if ( normalAssignmentNames.empty() )
		{
			return std::nullopt;
		}

		for ( int i = GetLength() - 1; i >= 0; --i )
		{
			const auto& child = ( *this )[i];
			auto it = normalAssignmentNames.find( child.GetCode() );
			if ( it != normalAssignmentNames.end() )
			{
				return it->second;
			}
		}

		return std::nullopt;
	}

	std::vector<std::pair<int, std::string>> GmodPath::GetCommonNames() const
	{
		std::vector<std::pair<int, std::string>> commonNames;

		for ( int depth = 0; depth < GetLength(); ++depth )
		{
			const auto& node = ( *this )[depth];
			if ( !node.IsLeafNode() && depth != GetLength() - 1 )
			{
				continue;
			}

			std::string name = node.GetMetadata().GetCommonName().value_or( node.GetMetadata().GetName() );
			const auto& normalAssignmentNames = node.GetMetadata().GetNormalAssignmentNames();

			if ( !normalAssignmentNames.empty() )
			{
				for ( int i = GetLength() - 1; i >= depth; --i )
				{
					const auto& child = ( *this )[i];
					auto it = normalAssignmentNames.find( child.GetCode() );
					if ( it != normalAssignmentNames.end() )
					{
						name = it->second;
						break;
					}
				}
			}

			commonNames.emplace_back( depth, name );
		}

		return commonNames;
	}

	std::vector<GmodIndividualizableSet> GmodPath::GetIndividualizableSets() const
	{
		std::vector<GmodIndividualizableSet> result;

		for ( int i = 0; i < GetLength(); ++i )
		{
			const auto& node = ( *this )[i];
			if ( !node.IsIndividualizable() )
			{
				continue;
			}

			std::vector<int> nodes = { i };
			result.emplace_back( nodes, const_cast<GmodPath&>( *this ) );
		}

		return result;
	}

	bool GmodPath::IsIndividualizable() const
	{
		for ( int i = 0; i < GetLength(); ++i )
		{
			if ( ( *this )[i].IsIndividualizable() )
			{
				return true;
			}
		}
		return false;
	}

	bool GmodPath::IsValid( const std::vector<GmodNode>& parents, const GmodNode& node )
	{
		int missingLinkAt;
		return IsValid( parents, node, missingLinkAt );
	}

	bool GmodPath::IsValid( const std::vector<GmodNode>& parents, const GmodNode& node, int& missingLinkAt )
	{
		missingLinkAt = -1;

		if ( parents.empty() )
		{
			return false;
		}

		if ( !parents.front().IsRoot() )
		{
			return false;
		}

		for ( size_t i = 0; i < parents.size(); ++i )
		{
			const auto& parent = parents[i];
			const auto& child = ( i + 1 < parents.size() ) ? parents[i + 1] : node;

			if ( !parent.IsChild( child ) )
			{
				missingLinkAt = static_cast<int>( i );
				return false;
			}
		}

		return true;
	}

	GmodPath GmodPath::Parse( const std::string& item, VisVersion visVersion )
	{
		GmodPath path( std::vector<GmodNode>(), GmodNode(), false );
		if ( !TryParse( item, visVersion, path ) )
		{
			SPDLOG_ERROR( "Failed to parse GmodPath" );
			throw std::invalid_argument( "Failed to parse GmodPath" );
		}
		return path;
	}

	bool GmodPath::TryParse( const std::string& item, VisVersion visVersion, GmodPath& path )
	{
		try
		{
			VIS vis;
			const auto& gmod = vis.GetGmod( visVersion );
			const auto& locations = vis.GetLocations( visVersion );
			return TryParse( item, gmod, locations, path );
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error in GmodPath::TryParse: {}", e.what() );
			return false;
		}
	}

	GmodPath GmodPath::Parse( const std::string& item, const Gmod& gmod, const Locations& locations )
	{
		GmodPath path( std::vector<GmodNode>(), GmodNode(), false );
		if ( !TryParse( item, gmod, locations, path ) )
		{
			SPDLOG_ERROR( "Failed to parse GmodPath" );
			throw std::invalid_argument( "Failed to parse GmodPath" );
		}
		return path;
	}

	bool GmodPath::TryParse( const std::string& item, const Gmod& gmod, const Locations& locations, GmodPath& path )
	{
		auto result = ParseInternal( item, gmod, locations );

		if ( auto* okResult = dynamic_cast<GmodParsePathResult::Ok*>( &result ) )
		{
			path = okResult->path;
			return true;
		}

		return false;
	}

	GmodPath GmodPath::ParseFullPath( const std::string& pathStr, VisVersion visVersion )
	{
		GmodPath path( std::vector<GmodNode>(), GmodNode(), false );
		if ( !TryParseFullPath( pathStr, visVersion, path ) )
		{
			SPDLOG_ERROR( "Failed to parse full GmodPath" );
			throw std::invalid_argument( "Failed to parse full GmodPath" );
		}
		return path;
	}

	bool GmodPath::TryParseFullPath( const std::string& pathStr, VisVersion visVersion, GmodPath& path )
	{
		try
		{
			VIS vis;
			const auto& gmod = vis.GetGmod( visVersion );
			const auto& locations = vis.GetLocations( visVersion );
			return TryParseFullPath( pathStr, gmod, locations, path );
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error in GmodPath::TryParseFullPath: {}", e.what() );
			return false;
		}
	}

	bool GmodPath::TryParseFullPath( std::string_view pathStr, VisVersion visVersion, GmodPath& path )
	{
		try
		{
			VIS vis;
			const auto& gmod = vis.GetGmod( visVersion );
			const auto& locations = vis.GetLocations( visVersion );
			return TryParseFullPath( pathStr, gmod, locations, path );
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error in GmodPath::TryParseFullPath: {}", e.what() );
			return false;
		}
	}

	bool GmodPath::TryParseFullPath( std::string_view pathStr, const Gmod& gmod, const Locations& locations, GmodPath& path )
	{
		auto result = ParseFullPathInternal( pathStr, gmod, locations );

		if ( auto* okResult = dynamic_cast<GmodParsePathResult::Ok*>( &result ) )
		{
			path = okResult->path;
			return true;
		}

		return false;
	}

	GmodParsePathResult GmodPath::ParseFullPathInternal( std::string_view span, const Gmod& gmod, const Locations& locations )
	{
		if ( span.empty() )
			return GmodParsePathResult::Err( "Path is empty" );

		if ( std::all_of( span.begin(), span.end(), []( char c ) { return std::isspace( c ); } ) )
			return GmodParsePathResult::Err( "Path is empty" );

		const std::string& rootCode = gmod.GetRootNode().GetCode();
		if ( !span.starts_with( rootCode ) )
			return GmodParsePathResult::Err( "Path must start with the root node" );

		std::vector<GmodNode> nodes;
		std::string_view remainder = span;

		while ( !remainder.empty() )
		{
			size_t separatorPos = remainder.find( '/' );
			std::string_view part = ( separatorPos == std::string_view::npos )
										? remainder
										: remainder.substr( 0, separatorPos );

			if ( !part.empty() )
			{
				size_t dashPos = part.find( '-' );
				std::string_view nodeCode = ( dashPos == std::string_view::npos )
												? part
												: part.substr( 0, dashPos );

				GmodNode node;
				if ( !gmod.TryGetNode( std::string( nodeCode ), node ) )
					return GmodParsePathResult::Err( "Invalid node code: " + std::string( nodeCode ) );

				if ( dashPos != std::string_view::npos )
				{
					std::string_view locStr = part.substr( dashPos + 1 );
					Location location;

					if ( !locations.TryParse( std::string( locStr ), location ) )
						return GmodParsePathResult::Err( "Invalid location: " + std::string( locStr ) );

					node = node.WithLocation( location );
				}

				nodes.push_back( node );
			}

			if ( separatorPos == std::string_view::npos )
				break;

			remainder = remainder.substr( separatorPos + 1 );
		}

		if ( nodes.empty() )
			return GmodParsePathResult::Err( "No valid nodes found in path" );

		GmodNode endNode = nodes.back();
		nodes.pop_back();

		int missingLinkAt;
		if ( !IsValid( nodes, endNode, missingLinkAt ) )
			return GmodParsePathResult::Err( "Invalid path: missing link at position " + std::to_string( missingLinkAt ) );

		GmodPath path( nodes, endNode );
		return GmodParsePathResult::Ok( path );
	}

	GmodParsePathResult GmodPath::ParseInternal( const std::string& item, const Gmod& gmod, const Locations& locations )
	{
		if ( gmod.GetVisVersion() != locations.GetVisVersion() )
		{
			SPDLOG_ERROR( "Got different VIS versions for Gmod and Locations arguments" );
			throw std::invalid_argument( "Got different VIS versions for Gmod and Locations arguments" );
		}

		if ( item.empty() || std::all_of( item.begin(), item.end(), []( char c ) { return std::isspace( c ); } ) )
			return GmodParsePathResult::Err( "Item is empty" );

		std::string trimmedItem = item;
		trimmedItem.erase( 0, trimmedItem.find_first_not_of( " \t\n\r\f\v" ) );
		trimmedItem.erase( trimmedItem.find_last_not_of( " \t\n\r\f\v" ) + 1 );
		if ( trimmedItem[0] == '/' )
			trimmedItem.erase( 0, 1 );

		std::queue<PathNode> parts;
		std::istringstream iss( trimmedItem );
		std::string partStr;
		while ( std::getline( iss, partStr, '/' ) )
		{
			size_t dashPos = partStr.find( '-' );
			if ( dashPos != std::string::npos )
			{
				std::string code = partStr.substr( 0, dashPos );
				std::string locStr = partStr.substr( dashPos + 1 );

				GmodNode node;
				if ( !gmod.TryGetNode( code, node ) )
					return GmodParsePathResult::Err( "Failed to get GmodNode for " + partStr );

				Location location;
				if ( !locations.TryParse( locStr, location ) )
					return GmodParsePathResult::Err( "Failed to parse location " + locStr );

				parts.emplace( code, std::optional<Location>( location ) );
			}
			else
			{
				GmodNode node;
				if ( !gmod.TryGetNode( partStr, node ) )
					return GmodParsePathResult::Err( "Failed to get GmodNode for " + partStr );

				parts.emplace( partStr );
			}
		}

		if ( parts.empty() )
			return GmodParsePathResult::Err( "Failed to find any parts" );

		bool hasEmptyCode = false;
		std::queue<PathNode> partsCopy = parts;
		while ( !partsCopy.empty() )
		{
			if ( partsCopy.front().code.empty() )
				hasEmptyCode = true;
			partsCopy.pop();
		}

		if ( hasEmptyCode )
			return GmodParsePathResult::Err( "Failed find any parts" );

		PathNode toFind = parts.front();
		parts.pop();

		GmodNode baseNode;
		if ( !gmod.TryGetNode( toFind.code, baseNode ) )
			return GmodParsePathResult::Err( "Failed to find base node" );

		ParseContext context( parts );
		context.toFind = toFind;

		const GmodNode& rootNode = gmod.GetRootNode();
		std::vector<GmodNode> parents = { rootNode };

		std::unordered_map<std::string, Location> locs;
		if ( toFind.location.has_value() )
		{
			locs[toFind.code] = *toFind.location;
		}

		std::queue<PathNode> remainingParts = parts;
		while ( !remainingParts.empty() )
		{
			PathNode part = remainingParts.front();
			remainingParts.pop();

			if ( part.location.has_value() )
			{
				locs[part.code] = *part.location;
			}
		}

		std::function<Gmod::TraversalHandlerResult( const GmodNode& )> visitor =
			[&]( const GmodNode& node ) -> Gmod::TraversalHandlerResult {
			if ( node.GetCode() == toFind.code )
			{
				GmodNode targetNode = node;

				if ( locs.find( node.GetCode() ) != locs.end() )
				{
					targetNode = node.WithLocation( locs[node.GetCode()] );
				}

				context.path = GmodPath( parents, targetNode );
				return Gmod::TraversalHandlerResult::Stop;
			}

			for ( const GmodNode* child : node.GetChildren() )
			{
				parents.push_back( node );

				Gmod::TraversalHandlerResult result = visitor( *child );

				parents.pop_back();

				if ( result == Gmod::TraversalHandlerResult::Stop )
				{
					return result;
				}
			}

			return Gmod::TraversalHandlerResult::Continue;
		};

		visitor( rootNode );

		if ( !context.path.has_value() )
			return GmodParsePathResult::Err( "Failed to find path after traversal" );

		return GmodParsePathResult::Ok( *context.path );
	}

	GmodParsePathResult::Ok::Ok( const GmodPath& path )
		: path( path )
	{
	}

	GmodParsePathResult::Err::Err( const std::string& error )
		: error( error )
	{
		SPDLOG_ERROR( "GmodParsePathResult::Err: {}", error );
	}

	GmodPath::Enumerator::Iterator::Iterator( Enumerator& enumerator, bool end )
		: m_enumerator( enumerator ), m_end( end )
	{
	}

	bool GmodPath::Enumerator::MoveNext()
	{
		if ( m_currentDepth < static_cast<int>( m_path.GetLength() ) - 1 )
		{
			++m_currentDepth;
			const auto& node = m_path[m_currentDepth];
			m_current = { m_currentDepth, std::cref( node ) };
			return true;
		}
		return false;
	}

	std::pair<int, std::reference_wrapper<const GmodNode>> GmodPath::Enumerator::GetCurrent() const
	{
		return m_current;
	}

	GmodPath::Enumerator::Iterator& GmodPath::Enumerator::Iterator::operator++()
	{
		if ( !m_end )
		{
			m_end = !m_enumerator.MoveNext();
		}
		return *this;
	}

	bool GmodPath::Enumerator::Iterator::operator!=( const Iterator& other ) const
	{
		return m_end != other.m_end;
	}

	std::pair<int, std::reference_wrapper<const GmodNode>> GmodPath::Enumerator::Iterator::operator*() const
	{
		return m_enumerator.GetCurrent();
	}

	GmodPath::Enumerator::Enumerator( const GmodPath& path, std::optional<int> fromDepth )
		: m_path( path ), m_currentDepth( fromDepth.value_or( -1 ) ),
		  m_current( -1, std::cref( path.GetNode() ) )
	{
		if ( fromDepth && ( *fromDepth < 0 || *fromDepth > static_cast<int>( path.GetLength() ) ) )
		{
			SPDLOG_ERROR( "fromDepth is out of range" );
			throw std::out_of_range( "fromDepth is out of range" );
		}

		if ( m_currentDepth >= 0 )
		{
			const auto& node = m_path[m_currentDepth];
			m_current = { m_currentDepth, std::cref( node ) };
		}
	}

	GmodPath::Enumerator::Iterator GmodPath::Enumerator::begin()
	{
		MoveNext();
		return Iterator( *this, false );
	}

	GmodPath::Enumerator::Iterator GmodPath::Enumerator::end()
	{
		return Iterator( *this, true );
	}

	std::vector<GmodNode> GmodIndividualizableSet::GetNodes() const
	{
		std::vector<GmodNode> nodes;
		nodes.reserve( m_nodes.size() );
		for ( int index : m_nodes )
		{
			nodes.push_back( ( *m_path )[index] );
		}
		return nodes;
	}

	const std::vector<int>& GmodIndividualizableSet::GetNodeIndices() const
	{
		return m_nodes;
	}

	std::optional<Location> GmodIndividualizableSet::GetLocation() const
	{
		if ( m_nodes.empty() )
		{
			return std::nullopt;
		}
		return ( *m_path )[m_nodes[0]].GetLocation();
	}

	void GmodIndividualizableSet::SetLocation( const std::optional<Location>& location )
	{
		for ( int index : m_nodes )
		{
			if ( location )
			{
				( *m_path )[index] = ( *m_path )[index].WithLocation( *location );
			}
			else
			{
				( *m_path )[index] = ( *m_path )[index].WithoutLocation();
			}
		}
	}

	GmodPath GmodIndividualizableSet::Build()
	{
		if ( !m_path )
		{
			SPDLOG_ERROR( "Tried to build individualizable set twice" );
			throw std::runtime_error( "Tried to build individualizable set twice" );
		}

		GmodPath result = *m_path;
		m_path = nullptr;
		return result;
	}

	std::string GmodIndividualizableSet::ToString() const
	{
		std::stringstream builder;
		for ( size_t i = 0; i < m_nodes.size(); ++i )
		{
			const auto& node = ( *m_path )[m_nodes[i]];
			if ( node.IsLeafNode() || i == m_nodes.size() - 1 )
			{
				if ( i > 0 )
				{
					builder << '/';
				}
				builder << node.ToString();
			}
		}
		return builder.str();
	}

	PathNode::PathNode( const std::string& code, const std::optional<Location>& location )
		: code( code ), location( location )
	{
	}

	ParseContext::ParseContext( std::queue<PathNode> parts )
		: parts( std::move( parts ) ), toFind(), locations(), path( std::nullopt )
	{
		if ( !this->parts.empty() )
		{
			toFind = std::move( this->parts.front() );
			this->parts.pop();
		}
	}

	GmodIndividualizableSet::GmodIndividualizableSet( const std::vector<int>& nodes, GmodPath& path )
		: m_nodes( nodes ), m_path( &path )
	{
		if ( nodes.empty() )
		{
			SPDLOG_ERROR( "GmodIndividualizableSet cannot be empty" );
			throw std::invalid_argument( "GmodIndividualizableSet cannot be empty" );
		}

		for ( int i = 0; i < static_cast<int>( nodes.size() ); ++i )
		{
			const auto& node = ( *m_path )[nodes[i]];
			if ( !node.IsIndividualizable( i == static_cast<int>( nodes.size() ) - 1, nodes.size() > 1 ) )
			{
				SPDLOG_ERROR( "GmodIndividualizableSet nodes must be individualizable" );
				throw std::invalid_argument( "GmodIndividualizableSet nodes must be individualizable" );
			}
		}

		auto location = ( *m_path )[nodes[0]].GetLocation();
		for ( int i = 1; i < static_cast<int>( nodes.size() ); ++i )
		{
			if ( ( *m_path )[nodes[i]].GetLocation() != location )
			{
				SPDLOG_ERROR( "GmodIndividualizableSet nodes have different locations" );
				throw std::invalid_argument( "GmodIndividualizableSet nodes have different locations" );
			}
		}

		if ( std::none_of( nodes.begin(), nodes.end(), [&]( int index ) {
				 const auto& node = ( *m_path )[index];
				 return node == m_path->GetNode() || node.IsLeafNode();
			 } ) )
		{
			SPDLOG_ERROR( "GmodIndividualizableSet has no nodes that are part of the short path" );
			throw std::invalid_argument( "GmodIndividualizableSet has no nodes that are part of the short path" );
		}
	}
}
