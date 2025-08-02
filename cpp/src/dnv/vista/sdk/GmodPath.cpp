/**
 * @file GmodPath.cpp
 * @brief Implementation of GmodPath and related classes for representing paths in the Generic Product Model (GMOD).
 */

#include "pch.h"

#include "dnv/vista/sdk/GmodPath.h"

#include "dnv/vista/sdk/utils/StringUtils.h"
#include "dnv/vista/sdk/utils/StringBuilderPool.h"

#include "dnv/vista/sdk/GmodTraversal.h"
#include "dnv/vista/sdk/Locations.h"
#include "dnv/vista/sdk/VIS.h"

namespace dnv::vista::sdk
{
	namespace internal
	{
		struct LocationSetsVisitor
		{
			size_t currentParentStart;

			LocationSetsVisitor()
				: currentParentStart{ std::numeric_limits<size_t>().max() } {}

			std::optional<std::tuple<size_t, size_t, std::optional<Location>>> visit(
				const GmodNode& node,
				size_t i,
				const std::vector<GmodNode*>& pathParents,
				const GmodNode& pathTargetNode )
			{
				bool isParent = Gmod::isPotentialParent( node.metadata().type() );
				bool isTargetNode = ( static_cast<size_t>( i ) == pathParents.size() );

				if ( currentParentStart == std::numeric_limits<size_t>().max() )
				{
					if ( isParent )
					{
						currentParentStart = i;
					}
					if ( node.isIndividualizable( isTargetNode ) )
					{
						return std::make_tuple( i, i, node.location() );
					}
				}
				else
				{
					if ( isParent || isTargetNode )
					{
						std::optional<std::tuple<size_t, size_t, std::optional<Location>>> nodes = std::nullopt;

						if ( currentParentStart + 1 == i )
						{
							if ( node.isIndividualizable( isTargetNode ) )
								nodes = std::make_tuple( i, i, node.location() );
						}
						else
						{
							size_t skippedOne = std::numeric_limits<size_t>().max();
							bool hasComposition = false;

							for ( size_t j = currentParentStart + 1; j <= i; ++j )
							{
								const GmodNode* setNode = ( j < pathParents.size() ) ? pathParents[j] : &pathTargetNode;

								if ( !setNode->isIndividualizable( j == pathParents.size(), true ) )
								{
									if ( nodes.has_value() )
									{
										skippedOne = j;
									}

									continue;
								}

								if ( nodes.has_value() &&
									 std::get<2>( nodes.value() ).has_value() &&
									 setNode->location().has_value() &&
									 std::get<2>( nodes.value() ) != setNode->location() )
								{
									throw std::runtime_error( "Mapping error: different locations in the same nodeset" );
								}

								if ( skippedOne != std::numeric_limits<size_t>().max() )
								{
									throw std::runtime_error( "Can't skip in the middle of individualizable set" );
								}

								if ( setNode->isFunctionComposition() )
								{
									hasComposition = true;
								}

								auto location = nodes.has_value() && std::get<2>( nodes.value() ).has_value() ? std::get<2>( nodes.value() ) : setNode->location();
								size_t start = nodes.has_value() ? std::get<0>( nodes.value() ) : j;
								size_t end = j;
								nodes = std::make_tuple( start, end, location );
							}

							if ( nodes.has_value() && std::get<0>( nodes.value() ) == std::get<1>( nodes.value() ) && hasComposition )
							{
								nodes = std::nullopt;
							}
						}

						currentParentStart = i;
						if ( nodes.has_value() )
						{
							bool hasLeafNode = false;
							size_t startIdx = std::get<0>( nodes.value() );
							size_t endIdx = std::get<1>( nodes.value() );

							for ( size_t j = startIdx; j <= endIdx; ++j )
							{
								const GmodNode* setNode = ( j < pathParents.size() ) ? pathParents[j] : &pathTargetNode;
								if ( setNode->isLeafNode() || j == pathParents.size() )
								{
									hasLeafNode = true;

									break;
								}
							}
							if ( hasLeafNode )
							{
								return nodes;
							}
						}
					}

					if ( isTargetNode && node.isIndividualizable( isTargetNode ) )
					{
						return std::make_tuple( i, i, node.location() );
					}
				}

				return std::nullopt;
			}
		};
	}

	//=====================================================================
	// GmodPath class
	//=====================================================================

	//----------------------------------------------
	// Construction
	//----------------------------------------------

	GmodPath::GmodPath( const Gmod& gmod, GmodNode node, std::vector<GmodNode> parents, bool skipVerify )
		: m_visVersion{ node.visVersion() },
		  m_gmod{ &gmod },
		  m_node{ std::move( node ) },
		  m_parents{ std::move( parents ) }
	{
		if ( !m_gmod )
		{
			throw std::invalid_argument( "GmodPath constructor: gmod reference is null" );
		}

		if ( !m_node.has_value() )
		{
			throw std::invalid_argument( "GmodPath constructor: node is not valid" );
		}

		if ( skipVerify || m_parents.empty() )
		{
			return;
		}
	}

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	std::vector<GmodIndividualizableSet> GmodPath::individualizableSets() const
	{
		std::vector<GmodIndividualizableSet> result;
		result.reserve( 8 );

		internal::LocationSetsVisitor locationSetsVisitor;
		std::vector<GmodNode*> tempParents;
		tempParents.reserve( m_parents.size() );

		for ( const auto& parent : m_parents )
		{
			tempParents.push_back( const_cast<GmodNode*>( &parent ) );
		}

		for ( size_t i = 0; i < length(); ++i )
		{
			const GmodNode& node = ( i < m_parents.size() ) ? m_parents[i] : *m_node;

			auto set = locationSetsVisitor.visit( node, i, tempParents, *m_node );
			if ( !set.has_value() )
			{
				continue;
			}

			const auto& [startIdx, endIdx, location] = set.value();

			if ( startIdx == endIdx )
			{
				result.emplace_back( std::vector<int>{ static_cast<int>( startIdx ) }, *this );

				continue;
			}

			std::vector<int> nodes;
			nodes.reserve( endIdx - startIdx + 1 );
			for ( size_t j = startIdx; j <= endIdx; ++j )
			{
				nodes.push_back( static_cast<int>( j ) );
			}

			result.emplace_back( std::move( nodes ), *this );
		}

		return result;
	}

	std::optional<std::string> GmodPath::normalAssignmentName( size_t nodeDepth ) const
	{
		if ( nodeDepth >= length() )
		{
			return std::nullopt;
		}

		const GmodNode& nodeInPath = ( *this )[nodeDepth];

		const auto& normalAssignmentNames = nodeInPath.metadata().normalAssignmentNames();
		if ( normalAssignmentNames.empty() )
		{
			return std::nullopt;
		}

		if ( length() == 0 )
		{
			return std::nullopt;
		}

		for ( size_t i = length() - 1;; --i )
		{
			const GmodNode& child = ( *this )[i];
			auto it = normalAssignmentNames.find( child.code() );
			if ( it != normalAssignmentNames.end() )
			{
				return it->second;
			}

			if ( i == 0 )
			{
				break;
			}
		}

		return std::nullopt;
	}

	std::vector<std::pair<size_t, std::string>> GmodPath::commonNames() const
	{
		std::vector<std::pair<size_t, std::string>> result;

		auto enumerator = this->enumerator();
		while ( enumerator.next() )
		{
			const auto& [depth, nodePtr] = enumerator.current();

			if ( !nodePtr )
			{
				continue;
			}

			const GmodNode& node = *nodePtr;
			bool isTarget = ( depth == m_parents.size() );

			if ( !( node.isLeafNode() || isTarget ) || !node.isFunctionNode() )
			{
				continue;
			}

			std::string name;
			const auto& commonName = node.metadata().commonName();
			if ( commonName.has_value() && !commonName->empty() )
			{
				name = *commonName;
			}
			else
			{
				name = node.metadata().name();
			}

			const auto& normalAssignmentNames = node.metadata().normalAssignmentNames();

			if ( !normalAssignmentNames.empty() )
			{
				if ( m_node.has_value() )
				{
					auto nodeCodeIt = normalAssignmentNames.find( m_node->code() );
					if ( nodeCodeIt != normalAssignmentNames.end() )
					{
						name = nodeCodeIt->second;
					}
				}

				if ( !m_parents.empty() && depth < m_parents.size() )
				{
					for ( size_t i = m_parents.size() - 1; i >= depth; --i )
					{
						const GmodNode& parent = m_parents[i];
						auto parentCodeIt = normalAssignmentNames.find( parent.code() );
						if ( parentCodeIt != normalAssignmentNames.end() )
						{
							name = parentCodeIt->second;

							break;
						}

						if ( i == depth )
						{
							break;
						}
					}
				}
			}

			result.emplace_back( depth, name );
		}

		return result;
	}

	//----------------------------------------------
	// State inspection methods
	//----------------------------------------------

	bool GmodPath::isValid( const std::vector<GmodNode*>& parents, const GmodNode& node, int& missingLinkAt )
	{
		missingLinkAt = -1;

		if ( parents.empty() )
		{
			return false;
		}

		if ( !parents.empty() && !parents[0]->isRoot() )
		{
			return false;
		}

		for ( size_t i = 0; i < parents.size(); ++i )
		{
			const GmodNode* parent = parents[i];
			const GmodNode* child = ( i + 1 < parents.size() ) ? parents[i + 1] : &node;

			if ( !parent->isChild( *child ) )
			{
				missingLinkAt = static_cast<int>( i );

				return false;
			}
		}

		return true;
	}

	bool GmodPath::isIndividualizable() const
	{
		if ( !m_node.has_value() || length() == 0 )
		{
			return false;
		}

		internal::LocationSetsVisitor locationSetsVisitor;
		std::vector<GmodNode*> tempParents;
		tempParents.reserve( m_parents.size() );

		for ( const auto& parent : m_parents )
		{
			tempParents.push_back( const_cast<GmodNode*>( &parent ) );
		}

		for ( size_t i = 0; i < length(); ++i )
		{
			const GmodNode& node = ( i < m_parents.size() ) ? m_parents[i] : *m_node;

			auto set = locationSetsVisitor.visit( node, i, tempParents, *m_node );
			if ( set.has_value() )
			{
				return true;
			}
		}

		return false;
	}

	//----------------------------------------------
	// Path manipulation methods
	//----------------------------------------------

	GmodPath GmodPath::withoutLocations() const
	{
		if ( !m_gmod || !m_node.has_value() )
		{
			return GmodPath{};
		}

		std::vector<GmodNode> newParents;
		newParents.reserve( m_parents.size() );

		for ( const GmodNode& parent : m_parents )
		{
			newParents.push_back( parent.withoutLocation() );
		}

		return GmodPath( *m_gmod, m_node->withoutLocation(), std::move( newParents ) );
	}

	//----------------------------------------------
	// Private static parsing methods
	//----------------------------------------------

	GmodParsePathResult GmodPath::parseFullPathInternal(
		std::string_view item, const Gmod& gmod, const Locations& locations )
	{
		if ( item.empty() )
		{
			return GmodParsePathResult::Error{ "Item is empty" };
		}

		if ( !utils::startsWith( item, gmod.rootNode().code() ) )
		{
			return GmodParsePathResult::Error{ "Path must start with {}" + gmod.rootNode().code() };
		}

		const size_t estimatedSegments = item.length() / 3;
		std::vector<GmodNode> nodes;
		nodes.reserve( estimatedSegments );

		for ( const auto& segment : utils::splitView( item, '/' ) )
		{
			if ( segment.empty() )
			{
				continue;
			}

			const auto dashPos = segment.find( '-' );

			if ( dashPos != std::string_view::npos )
			{
				const std::string_view codePart = segment.substr( 0, dashPos );
				const std::string_view locationPart = segment.substr( dashPos + 1 );

				const GmodNode* nodePtr;
				if ( !gmod.tryGetNode( codePart, nodePtr ) )
				{
					return GmodParsePathResult::Error{ "Node lookup failed" };
				}

				Location parsedLocation;
				if ( !locations.tryParse( locationPart, parsedLocation ) )
				{
					return GmodParsePathResult::Error{ "Location parse failed" };
				}

				nodes.emplace_back( std::move( nodePtr->withLocation( std::move( parsedLocation ) ) ) );
			}
			else
			{
				const GmodNode* nodePtr;
				if ( !gmod.tryGetNode( segment, nodePtr ) )
				{
					return GmodParsePathResult::Error{ "Node lookup failed" };
				}

				nodes.emplace_back( std::move( *nodePtr ) );
			}
		}

		if ( nodes.empty() )
		{
			return GmodParsePathResult::Error{ "No nodes found" };
		}

		GmodNode endNode = std::move( nodes.back() );
		nodes.pop_back();

		if ( nodes.empty() )
		{
			return GmodParsePathResult::Ok{ GmodPath( gmod, std::move( endNode ), std::move( nodes ), true ) };
		}

		if ( !nodes[0].isRoot() )
		{
			return GmodParsePathResult::Error{ "Sequence of nodes are invalid" };
		}

		int missingLinkAt = -1;
		std::vector<GmodNode*> nodePointers;
		nodePointers.reserve( nodes.size() );
		for ( auto& node : nodes )
		{
			nodePointers.push_back( &node );
		}

		if ( !isValid( nodePointers, endNode, missingLinkAt ) )
		{
			return GmodParsePathResult::Error{ "Sequence of nodes are invalid" };
		}

		bool hasLocations = endNode.location().has_value();
		for ( const auto& node : nodes )
		{
			if ( node.location().has_value() )
			{
				hasLocations = true;

				break;
			}
		}

		if ( !hasLocations )
		{
			return GmodParsePathResult::Ok{ GmodPath( gmod, std::move( endNode ), std::move( nodes ), true ) };
		}

		internal::LocationSetsVisitor locationSetsVisitor;
		std::optional<size_t> prevNonNullLocation;

		constexpr size_t MAX_SETS = 16;
		std::pair<size_t, size_t> sets[MAX_SETS];
		size_t setCounter = 0;

		constexpr size_t MAX_PARENTS = 16;
		GmodNode* tempParents[MAX_PARENTS];
		size_t tempParentsSize = 0;

		thread_local std::vector<GmodNode*> tempParentsView;

		for ( size_t i = 0; i < nodes.size() + 1; ++i )
		{
			const GmodNode& n = ( i < nodes.size() ) ? nodes[i] : endNode;

			tempParentsSize = 0;
			for ( auto& node : nodes )
			{
				if ( tempParentsSize < MAX_PARENTS )
				{
					tempParents[tempParentsSize++] = &node;
				}
			}

			tempParentsView.clear();
			tempParentsView.assign( tempParents, tempParents + tempParentsSize );
			auto set = locationSetsVisitor.visit( n, i, tempParentsView, endNode );
			if ( !set.has_value() )
			{
				if ( !prevNonNullLocation.has_value() && n.location().has_value() )
				{
					prevNonNullLocation = i;
				}

				continue;
			}

			const auto& [setStart, setEnd, location] = set.value();

			if ( prevNonNullLocation.has_value() )
			{
				for ( size_t j = prevNonNullLocation.value(); j < setStart; ++j )
				{
					const GmodNode& pn = ( j < nodes.size() ) ? nodes[j] : endNode;
					if ( pn.location().has_value() )
					{
						return GmodParsePathResult::Error{ "Expected all nodes in the set to be without individualization" };
					}
				}
			}
			prevNonNullLocation = std::nullopt;

			if ( setCounter < MAX_SETS )
			{
				sets[setCounter++] = { setStart, setEnd };
			}

			if ( setStart == setEnd )
			{
				continue;
			}

			for ( size_t j = setStart; j <= setEnd; ++j )
			{
				if ( j < nodes.size() )
				{
					nodes[j] = nodes[j].tryWithLocation( location );
				}
				else
				{
					endNode = endNode.tryWithLocation( location );
				}
			}
		}

		std::pair<size_t, size_t> currentSet = { SIZE_MAX, SIZE_MAX };
		size_t currentSetIndex = 0;

		for ( size_t i = 0; i < nodes.size() + 1; ++i )
		{
			while ( currentSetIndex < setCounter && ( currentSet.second == SIZE_MAX || currentSet.second < i ) )
				currentSet = sets[currentSetIndex++];

			bool insideSet = ( currentSet.first != SIZE_MAX && i >= currentSet.first && i <= currentSet.second );
			const GmodNode& n = ( i < nodes.size() ) ? nodes[i] : endNode;

			if ( insideSet )
			{
				const GmodNode& expectedLocationNode = ( currentSet.second < nodes.size() ) ? nodes[currentSet.second] : endNode;
				if ( n.location() != expectedLocationNode.location() )
				{
					return GmodParsePathResult::Error{ "Expected all nodes in the set to be individualized the same" };
				}
			}
			else
			{
				if ( n.location().has_value() )
				{
					return GmodParsePathResult::Error{ "Expected all nodes in the set to be without individualization" };
				}
			}
		}

		return GmodParsePathResult::Ok{ GmodPath( gmod, std::move( endNode ), std::move( nodes ), true ) };
	}

	GmodParsePathResult GmodPath::parseInternal( std::string_view item, const Gmod& gmod, const Locations& locations )
	{
		if ( gmod.visVersion() != locations.visVersion() )
		{
			return GmodParsePathResult::Error{ "Got different VIS versions for Gmod and Locations arguments" };
		}

		if ( item.empty() )
		{
			return GmodParsePathResult::Error{ "Item is empty" };
		}

		const size_t start = item.find_first_not_of( constants::NULL_OR_WHITESPACE );
		if ( start == std::string_view::npos )
		{
			return GmodParsePathResult::Error{ "Item is empty" };
		}

		const size_t end = item.find_last_not_of( constants::NULL_OR_WHITESPACE ) + 1;
		item = item.substr( start, end - start );

		if ( !item.empty() && item[0] == '/' )
		{
			item = item.substr( 1 );
		}

		struct PathNode
		{
			std::string_view code;
			std::optional<Location> location;
		};

		thread_local PathNode pathNodes[32];
		size_t pathNodeCount = 0;

		for ( const auto& segment : utils::splitView( item, '/' ) )
		{
			if ( segment.empty() || pathNodeCount >= 32 )
			{
				continue;
			}

			const size_t dashPos = segment.find( '-' );

			if ( dashPos != std::string_view::npos )
			{
				const std::string_view codePart = segment.substr( 0, dashPos );
				const std::string_view locationPart = segment.substr( dashPos + 1 );

				const GmodNode* tempNode = nullptr;
				if ( !gmod.tryGetNode( codePart, tempNode ) || !tempNode )
				{
					return GmodParsePathResult::Error{ "Failed to get GmodNode for " + std::string{ segment } };
				}

				Location parsedLocation;
				if ( !locations.tryParse( locationPart, parsedLocation ) )
				{
					return GmodParsePathResult::Error{ "Failed to parse location " + std::string{ locationPart } };
				}

				pathNodes[pathNodeCount++] = PathNode{ codePart, parsedLocation };
			}
			else
			{
				const GmodNode* tempNode = nullptr;
				if ( !gmod.tryGetNode( segment, tempNode ) || !tempNode )
				{
					return GmodParsePathResult::Error{ "Failed to get GmodNode for " + std::string{ segment } };
				}

				pathNodes[pathNodeCount++] = PathNode{ segment, std::nullopt };
			}
		}

		if ( pathNodeCount == 0 )
		{
			return GmodParsePathResult::Error{ "Failed find any parts" };
		}

		const PathNode& toFind = pathNodes[0];
		size_t currentIndex = 1;

		const GmodNode* baseNode = nullptr;
		if ( !gmod.tryGetNode( toFind.code, baseNode ) || !baseNode )
		{
			return GmodParsePathResult::Error{ "Failed to find base node" };
		}

		struct ParseContext
		{
			const PathNode* pathNodes;
			size_t pathNodeCount;
			size_t currentIndex;
			std::string_view currentCode;
			std::optional<Location> currentLocation;
			std::optional<utils::StringMap<Location>> locations;
			std::optional<GmodPath> path;
			const Gmod* gmod;
		};

		ParseContext context{
			pathNodes,
			pathNodeCount,
			currentIndex,
			toFind.code,
			toFind.location,
			std::nullopt,
			std::nullopt,
			&gmod };

		TraverseHandlerWithState<ParseContext> handler = []( ParseContext& ctx, const std::vector<const GmodNode*>& parents, const GmodNode& current ) -> TraversalHandlerResult {
			bool found = ( current.code() == ctx.currentCode );

			if ( !found && Gmod::isLeafNode( current.metadata() ) )
			{
				return TraversalHandlerResult::SkipSubtree;
			}

			if ( !found )
			{
				return TraversalHandlerResult::Continue;
			}

			if ( ctx.currentLocation.has_value() )
			{
				if ( !ctx.locations.has_value() )
				{
					ctx.locations = utils::StringMap<Location>{};
				}
				ctx.locations->emplace( std::string{ ctx.currentCode }, ctx.currentLocation.value() );
			}

			if ( ctx.currentIndex < ctx.pathNodeCount )
			{
				const PathNode& nextNode = ctx.pathNodes[ctx.currentIndex++];
				ctx.currentCode = nextNode.code;
				ctx.currentLocation = nextNode.location;

				return TraversalHandlerResult::Continue;
			}

			std::vector<GmodNode> pathParents;
			pathParents.reserve( parents.size() );

			for ( const GmodNode* parent : parents )
			{
				if ( ctx.locations.has_value() )
				{
					auto locationIt = ctx.locations->find( std::string{ parent->code() } );
					if ( locationIt != ctx.locations->end() )
					{
						pathParents.emplace_back( parent->withLocation( locationIt->second ) );

						continue;
					}
				}
				pathParents.emplace_back( *parent );
			}

			GmodNode endNode = ctx.currentLocation.has_value() ? current.withLocation( ctx.currentLocation.value() ) : current;

			const GmodNode* startNode = nullptr;
			if ( !pathParents.empty() && pathParents[0].parents().size() == 1 )
			{
				startNode = pathParents[0].parents()[0];
			}
			else if ( endNode.parents().size() == 1 )
			{
				startNode = endNode.parents()[0];
			}

			if ( startNode == nullptr || startNode->parents().size() > 1 )
			{
				return TraversalHandlerResult::Stop;
			}

			while ( startNode->parents().size() == 1 )
			{
				pathParents.insert( pathParents.begin(), *startNode );
				startNode = startNode->parents()[0];
				if ( startNode->parents().size() > 1 )
				{
					return TraversalHandlerResult::Stop;
				}
			}

			pathParents.insert( pathParents.begin(), ctx.gmod->rootNode() );

			internal::LocationSetsVisitor visitor;
			for ( size_t i = 0; i < pathParents.size() + 1; ++i )
			{
				GmodNode& n = ( i < pathParents.size() ) ? pathParents[i] : endNode;

				std::vector<GmodNode*> tempParentsView;
				tempParentsView.reserve( pathParents.size() );
				for ( auto& p : pathParents )
				{
					tempParentsView.push_back( &p );
				}

				auto set = visitor.visit( n, i, tempParentsView, endNode );
				if ( !set.has_value() )
				{
					if ( n.location().has_value() )
					{
						return TraversalHandlerResult::Stop;
					}

					continue;
				}

				const auto& [setStart, setEnd, location] = set.value();
				if ( setStart == setEnd )
				{
					continue;
				}

				for ( size_t j = setStart; j <= setEnd; ++j )
				{
					if ( j < pathParents.size() )
					{
						pathParents[j] = pathParents[j].tryWithLocation( location );
					}
					else
					{
						endNode = endNode.tryWithLocation( location );
					}
				}
			}

			ctx.path = GmodPath( *ctx.gmod, std::move( endNode ), std::move( pathParents ), true );
			return TraversalHandlerResult::Stop;
		};

		GmodTraversal::traverse( context, *baseNode, handler );

		if ( !context.path.has_value() )
		{
			return GmodParsePathResult::Error{ "Failed to find path after traversal" };
		}

		return GmodParsePathResult::Ok{ std::move( context.path.value() ) };
	}

	//----------------------------------------------
	// GmodPath::enumerator
	//----------------------------------------------

	//----------------------------
	// Construction
	//----------------------------

	GmodPath::Enumerator::Enumerator( const GmodPath* pathInst, size_t startIndex )
		: m_pathInstance{ pathInst },
		  m_currentIndex{ std::numeric_limits<size_t>::max() },
		  m_current{ std::numeric_limits<size_t>::max(), nullptr }
	{
		if ( startIndex != std::numeric_limits<size_t>::max() )
		{
			if ( startIndex > 0 )
			{
				m_currentIndex = startIndex - 1;
			}
		}
	}

	//=====================================================================
	// GmodIndividualizableSet class
	//=====================================================================

	GmodIndividualizableSet::GmodIndividualizableSet( const std::vector<int>& nodeIndices, const GmodPath& sourcePath )
		: m_nodeIndices{ nodeIndices },
		  m_path{ sourcePath },
		  m_isBuilt{ false }
	{
		if ( m_nodeIndices.empty() )
		{
			throw std::invalid_argument( "GmodIndividualizableSet constructor: nodeIndices cannot be empty." );
		}

		for ( int nodeIdx : m_nodeIndices )
		{
			if ( static_cast<size_t>( nodeIdx ) >= m_path.length() || nodeIdx < 0 )
			{
				throw std::out_of_range( "GmodIndividualizableSet constructor: Node index " +
										 std::to_string( nodeIdx ) + " is out of bounds for path length " +
										 std::to_string( m_path.length() ) + "." );
			}

			const GmodNode& currentNode = m_path[static_cast<size_t>( nodeIdx )];

			bool isTargetNode = ( static_cast<size_t>( nodeIdx ) == m_path.length() - 1 );
			bool isInSet = ( m_nodeIndices.size() > 1 );
			if ( !currentNode.isIndividualizable( isTargetNode, isInSet ) )
			{
				throw std::invalid_argument( "GmodIndividualizableSet constructor: Node '" +
											 std::string{ currentNode.code().data() } + "' (at index " +
											 std::to_string( nodeIdx ) + ") is not individualizable in the given context." );
			}
		}

		if ( m_nodeIndices.size() > 1 )
		{
			const GmodNode& firstNode = m_path[static_cast<size_t>( m_nodeIndices[0] )];
			std::optional<Location> expectedLocation = firstNode.location();

			for ( size_t i = 1; i < m_nodeIndices.size(); ++i )
			{
				int currentIdx = m_nodeIndices[i];
				const GmodNode& currentNode = m_path[static_cast<size_t>( currentIdx )];
				if ( currentNode.location() != expectedLocation )
				{
					throw std::invalid_argument( "GmodIndividualizableSet constructor: Nodes have different locations. Node '" +
												 std::string{ currentNode.code().data() } + "' (at index " +
												 std::to_string( currentIdx ) + ") has location while first node in set had different or no location." );
				}
			}
		}

		bool foundPartOfShortPath = false;
		for ( int nodeIdx : m_nodeIndices )
		{
			const GmodNode& currentNode = m_path[static_cast<size_t>( nodeIdx )];
			if ( currentNode == m_path.node() || currentNode.isLeafNode() )
			{
				foundPartOfShortPath = true;

				break;
			}
		}

		if ( !foundPartOfShortPath )
		{
			throw std::invalid_argument( "GmodIndividualizableSet constructor: No nodes in the set are part of the short path (final node or leaf node)." );
		}
	}

	//----------------------------------------------
	// Build
	//----------------------------------------------

	GmodPath GmodIndividualizableSet::build()
	{
		if ( m_isBuilt )
		{
			throw std::runtime_error( "Tried to build individualizable set twice" );
		}

		m_isBuilt = true;

		return std::move( m_path );
	}

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	std::vector<GmodNode*> GmodIndividualizableSet::nodes() const
	{
		std::vector<GmodNode*> result;
		result.reserve( m_nodeIndices.size() );

		for ( int nodeIdx : m_nodeIndices )
		{
			if ( nodeIdx < 0 || static_cast<size_t>( nodeIdx ) >= m_path.length() )
			{
				throw std::out_of_range( "Node index out of bounds" );
			}

			result.push_back( const_cast<GmodNode*>( &m_path[static_cast<size_t>( nodeIdx )] ) );
		}

		return result;
	}

	void GmodIndividualizableSet::setLocation( const std::optional<Location>& location )
	{
		for ( size_t i = 0; i < m_nodeIndices.size(); ++i )
		{
			int nodeIdx = m_nodeIndices[i];

			if ( nodeIdx < 0 || static_cast<size_t>( nodeIdx ) >= m_path.length() )
			{
				continue;
			}

			GmodNode& nodeInPath = const_cast<GmodPath&>( m_path )[static_cast<size_t>( nodeIdx )];

			if ( !location.has_value() )
			{
				nodeInPath = nodeInPath.withoutLocation();
			}
			else
			{
				nodeInPath = nodeInPath.withLocation( location.value() );
			}
		}
	}

	std::string GmodIndividualizableSet::toString() const
	{
		auto lease = utils::StringBuilderPool::instance();
		bool firstNodeAppended = false;

		for ( size_t j = 0; j < m_nodeIndices.size(); ++j )
		{
			int nodeIdx = m_nodeIndices[j];
			if ( nodeIdx < 0 || static_cast<size_t>( nodeIdx ) >= m_path.length() )
			{
				continue;
			}

			const GmodNode& currentNode = m_path[static_cast<size_t>( nodeIdx )];

			if ( currentNode.isLeafNode() || j == m_nodeIndices.size() - 1 )
			{
				if ( firstNodeAppended )
				{
					lease.Builder().push_back( '/' );
				}

				lease.Builder().append( currentNode.toString() );
				firstNodeAppended = true;
			}
		}

		return lease.toString();
	}
}
