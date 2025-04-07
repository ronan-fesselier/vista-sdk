#include "pch.h"

#include "dnv/vista/sdk/Gmod.h"

#include "dnv/vista/sdk/GmodDto.h"
#include "dnv/vista/sdk/VisVersion.h"
#include "dnv/vista/sdk/GmodPath.h"

namespace dnv::vista::sdk
{
	void Parents::push( const GmodNode* parent )
	{
		m_nodes.push_back( parent );
		if ( m_occurrences.find( parent->GetCode() ) != m_occurrences.end() )
		{
			m_occurrences[parent->GetCode()]++;
		}
		else
		{
			m_occurrences[parent->GetCode()] = 1;
		}
	}

	void Parents::pop()
	{
		if ( m_nodes.empty() )
			return;

		const GmodNode* parent = m_nodes.back();
		m_nodes.pop_back();

		if ( m_occurrences[parent->GetCode()] == 1 )
		{
			m_occurrences.erase( parent->GetCode() );
		}
		else
		{
			m_occurrences[parent->GetCode()]--;
		}
	}

	int Parents::occurrences( const GmodNode& node ) const
	{
		SPDLOG_INFO( "node: {}", node.ToString() );
		auto it = m_occurrences.find( node.GetCode() );
		return it != m_occurrences.end() ? it->second : 0;
	}

	const GmodNode* Parents::lastOrDefault() const
	{
		return m_nodes.empty() ? nullptr : m_nodes.back();
	}

	std::vector<GmodNode> Parents::nodes() const
	{
		std::vector<GmodNode> result;
		result.reserve( m_nodes.size() );
		for ( const auto* node : m_nodes )
		{
			result.push_back( *node );
		}
		return result;
	}

	const std::vector<const GmodNode*>& Parents::nodePointers() const
	{
		return m_nodes;
	}

	Gmod::Gmod( VisVersion version, const GmodDto& dto )
		: m_visVersion( version )
	{
		std::unordered_map<std::string, GmodNode> nodeMap;
		for ( const auto& nodeDto : dto.items )
		{
			GmodNode node( m_visVersion, nodeDto );
			nodeMap.emplace( nodeDto.code, std::move( node ) );
		}

		for ( const auto& relation : dto.relations )
		{
			if ( relation.size() >= 2 )
			{
				const std::string& parentCode = relation[0];
				const std::string& childCode = relation[1];

				auto parentIt = nodeMap.find( parentCode );
				auto childIt = nodeMap.find( childCode );

				if ( parentIt != nodeMap.end() && childIt != nodeMap.end() )
				{
					parentIt->second.AddChild( &childIt->second );
					childIt->second.AddParent( &parentIt->second );
				}
			}
		}

		for ( auto& [code, node] : nodeMap )
		{
			node.Trim();
		}

		auto rootIt = nodeMap.find( "VE" );
		if ( rootIt != nodeMap.end() )
		{
			m_rootNode = rootIt->second;
		}

		std::vector<std::pair<std::string, GmodNode>> nodePairs;
		nodePairs.reserve( nodeMap.size() );
		for ( const auto& [code, node] : nodeMap )
		{
			nodePairs.emplace_back( code, node );
		}

		m_nodeMap = ChdDictionary<GmodNode>( std::move( nodePairs ) );

		SPDLOG_INFO( "Creating Gmod with {} items - dictionary has {} entries",
			dto.items.size(), nodeMap.size() );

		if ( m_nodeMap.isEmpty() )
		{
			SPDLOG_ERROR( "Failed to initialize node dictionary in constructor!" );
		}
	}

	Gmod::Gmod( VisVersion version, const std::unordered_map<std::string, GmodNode>& nodeMap )
		: m_visVersion( version )
	{
		for ( const auto& [code, node] : nodeMap )
		{
			const_cast<GmodNode&>( node ).Trim();
		}

		m_rootNode = nodeMap.at( "VE" );

		std::vector<std::pair<std::string, GmodNode>> nodePairs;
		for ( const auto& [code, node] : nodeMap )
		{
			nodePairs.emplace_back( code, node );
		}

		m_nodeMap = ChdDictionary<GmodNode>( std::move( nodePairs ) );
	}

	Gmod::Gmod( const Gmod& other )
		: m_visVersion( other.m_visVersion ),
		  m_rootNode( other.m_rootNode ),
		  m_nodeMap( other.m_nodeMap )
	{
		SPDLOG_INFO( "Copying Gmod with {} nodes in dictionary",
			m_nodeMap.isEmpty() ? "empty" : "non-empty" );

		if ( other.m_nodeMap.isEmpty() != m_nodeMap.isEmpty() )
		{
			SPDLOG_ERROR( "Dictionary copy failed! Source: {}, Target: {}",
				other.m_nodeMap.isEmpty() ? "empty" : "non-empty",
				m_nodeMap.isEmpty() ? "empty" : "non-empty" );
		}

		SPDLOG_INFO( "Copying Gmod with {} nodes in dictionary",
			other.m_nodeMap.isEmpty() ? "empty" : "non-empty" );

		if ( m_nodeMap.isEmpty() && !other.m_nodeMap.isEmpty() )
		{
			SPDLOG_ERROR( "Dictionary copy failed! Source had data but destination is empty" );
		}
	}

	Gmod::Gmod( Gmod&& other ) noexcept
		: m_visVersion( other.m_visVersion ), m_rootNode( std::move( other.m_rootNode ) ), m_nodeMap( std::move( other.m_nodeMap ) )
	{
		other.m_visVersion = VisVersion::Unknown;
	}

	Gmod::~Gmod()
	{
	}

	Gmod& Gmod::operator=( Gmod&& other ) noexcept
	{
		if ( this != &other )
		{
			m_visVersion = other.m_visVersion;
			m_rootNode = std::move( other.m_rootNode );
			m_nodeMap = std::move( other.m_nodeMap );

			other.m_visVersion = VisVersion::Unknown;
		}
		return *this;
	}

	const GmodNode& Gmod::operator[]( const std::string& key ) const
	{
		static GmodNode nullNode;

		try
		{
			static thread_local std::unordered_map<std::string, GmodNode> nodeCache;

			auto it = nodeCache.find( key );
			if ( it != nodeCache.end() )
			{
				return it->second;
			}

			GmodNode node;
			if ( tryGetNode( key, node ) )
			{
				auto result = nodeCache.insert_or_assign( key, std::move( node ) );
				return result.first->second;
			}

			SPDLOG_WARN( "Node with key '{}' not found", key );
			nodeCache[key] = nullNode;
			return nodeCache[key];
		}
		catch ( const std::exception& ex )
		{
			SPDLOG_ERROR( "Exception in operator[]: {}", ex.what() );
			return nullNode;
		}
		catch ( ... )
		{
			SPDLOG_ERROR( "Unknown exception in operator[]" );
			return nullNode;
		}
	}

	VisVersion Gmod::visVersion() const
	{
		return m_visVersion;
	}

	const GmodNode& Gmod::rootNode() const
	{
		SPDLOG_WARN( "Getting root node: '{}'", m_rootNode.GetCode() );
		return m_rootNode;
	}

	bool Gmod::tryGetNode( const std::string& code, GmodNode& node ) const
	{
		try
		{
			if ( m_nodeMap.isEmpty() )
			{
				SPDLOG_WARN( "TryGetNode: Node dictionary is empty, GMOD may not be properly initialized" );
				SPDLOG_INFO( "GMOD state - VisVersion: {}, Has root node: {}",
					VisVersionExtensions::toVersionString( m_visVersion ),
					m_rootNode.GetCode().empty() ? "no" : "yes" );
				return false;
			}

			return m_nodeMap.tryGetValue( code, &node );
		}
		catch ( const std::exception& ex )
		{
			SPDLOG_ERROR( "Exception in TryGetNode for code {}: {}", code, ex.what() );
			return false;
		}
	}

	bool Gmod::tryGetNode( std::string_view code, GmodNode& node ) const
	{
		try
		{
			if ( code.empty() )
			{
				SPDLOG_WARN( "TryGetNode: Attempted to look up empty node code" );
				return false;
			}

			if ( !m_nodeMap.tryGetValue( code, &node ) )
			{
				SPDLOG_INFO( "TryGetNode: Node '{}' not found in GMOD", code );
				return false;
			}

			SPDLOG_INFO( "TryGetNode: Node '{}' found in GMOD", code );
			return true;
		}
		catch ( const std::exception& ex )
		{
			SPDLOG_ERROR( "Exception in TryGetNode for '{}': {}", code, ex.what() );
			return false;
		}
	}

	GmodPath Gmod::parsePath( const std::string& item ) const
	{
		return GmodPath::Parse( item, m_visVersion );
	}

	bool Gmod::TryParsePath( const std::string& item, std::optional<GmodPath>& path ) const
	{
		SPDLOG_INFO( "TryParsePath: Attempting to parse path: {}", item );
		return GmodPath::TryParse( item, m_visVersion, path );
	}

	GmodPath Gmod::parseFromFullPath( const std::string& item ) const
	{
		return GmodPath::ParseFullPath( item, m_visVersion );
	}

	bool Gmod::tryParseFromFullPath( const std::string& item, std::optional<GmodPath>& path ) const
	{
		GmodPath tempPath;

		if ( GmodPath::TryParseFullPath( item, m_visVersion, tempPath ) )
		{
			path = std::move( tempPath );
			return true;
		}

		return false;
	}

	bool Gmod::traverse( const TraverseHandler& handler, const TraversalOptions& options ) const
	{
		return traverse( rootNode(), handler, options );
	}

	bool Gmod::traverse( const GmodNode& rootNode, const TraverseHandler& handler, const TraversalOptions& options ) const
	{
		TraversalContext context( handler, options.maxTraversalOccurrence, options.maxNodes );
		TraversalHandlerResult result = traverseNode( context, rootNode );

		return result != TraversalHandlerResult::Stop;
	}

	Gmod::TraversalHandlerResult Gmod::traverseNode( TraversalContext& context, const GmodNode& node ) const
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

		bool skipOccurrenceCheck = isProductSelectionAssignment( context.parents.lastOrDefault(), &node );
		if ( !skipOccurrenceCheck )
		{
			int occ = context.parents.occurrences( node );
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

		std::vector<GmodNode> currentPath = context.parents.nodes();

		TraversalHandlerResult result = context.handler( currentPath, node );

		if ( result == TraversalHandlerResult::Stop || result == TraversalHandlerResult::SkipSubtree )
		{
			return result;
		}

		context.parents.push( &node );

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

		context.parents.pop();

		return TraversalHandlerResult::Continue;
	}

	bool Gmod::initializeNodeDictionary( const GmodDto& dto )
	{
		try
		{
			SPDLOG_INFO( "Building GMOD node dictionary from DTO with {} items and {} relations",
				dto.items.size(), dto.relations.size() );

			std::unordered_map<std::string, GmodNode> nodeMap;
			for ( const auto& nodeDto : dto.items )
			{
				GmodNode node( m_visVersion, nodeDto );
				nodeMap.emplace( nodeDto.code, std::move( node ) );
				SPDLOG_INFO( "Created node for code: {}", nodeDto.code );
			}

			for ( const auto& relation : dto.relations )
			{
				if ( relation.size() >= 2 )
				{
					const std::string& parentCode = relation[0];
					const std::string& childCode = relation[1];

					auto parentIt = nodeMap.find( parentCode );
					auto childIt = nodeMap.find( childCode );

					if ( parentIt != nodeMap.end() && childIt != nodeMap.end() )
					{
						parentIt->second.AddChild( &childIt->second );
						childIt->second.AddParent( &parentIt->second );
					}
				}
			}

			for ( auto& [code, node] : nodeMap )
			{
				node.Trim();
			}

			auto rootIt = nodeMap.find( "VE" );
			if ( rootIt != nodeMap.end() )
			{
				m_rootNode = rootIt->second;
			}
			else
			{
				SPDLOG_WARN( "Root node 'VE' not found in GMOD" );
			}

			std::vector<std::pair<std::string, GmodNode>> nodePairs;
			nodePairs.reserve( nodeMap.size() );
			for ( const auto& [code, node] : nodeMap )
			{
				nodePairs.emplace_back( code, node );
			}

			m_nodeMap = ChdDictionary<GmodNode>( std::move( nodePairs ) );
			if ( m_nodeMap.isEmpty() )
			{
				SPDLOG_ERROR( "Node dictionary is empty after initialization!" );
			}

			SPDLOG_INFO( "Successfully built node dictionary with {} entries", nodeMap.size() );

			return !m_nodeMap.isEmpty();
		}
		catch ( const std::exception& ex )
		{
			SPDLOG_ERROR( "Failed to initialize node dictionary: {}", ex.what() );
			return false;
		}
	}

	bool Gmod::isLeafNode( const std::string& fullType )
	{
		return std::find( s_leafTypes.begin(), s_leafTypes.end(), fullType ) != s_leafTypes.end();
	}

	bool Gmod::isLeafNode( const GmodNodeMetadata& metadata )
	{
		return isLeafNode( metadata.GetFullType() );
	}

	bool Gmod::isFunctionNode( const std::string& category )
	{
		return category != "PRODUCT" && category != "ASSET";
	}

	bool Gmod::isFunctionNode( const GmodNodeMetadata& metadata )
	{
		return isFunctionNode( metadata.GetCategory() );
	}

	bool Gmod::isProductSelection( const GmodNodeMetadata& metadata )
	{
		return metadata.GetCategory() == "PRODUCT" && metadata.GetType() == "SELECTION";
	}

	bool Gmod::isProductType( const GmodNodeMetadata& metadata )
	{
		return metadata.GetCategory() == "PRODUCT" && metadata.GetType() == "TYPE";
	}

	bool Gmod::isAsset( const GmodNodeMetadata& metadata )
	{
		return metadata.GetCategory() == "ASSET";
	}

	bool Gmod::isAssetFunctionNode( const GmodNodeMetadata& metadata )
	{
		return metadata.GetCategory() == "ASSET FUNCTION";
	}

	bool Gmod::isProductTypeAssignment( const GmodNode* parent, const GmodNode* child )
	{
		if ( parent == nullptr || child == nullptr )
			return false;

		if ( parent->GetMetadata().GetCategory().find( "FUNCTION" ) == std::string::npos )
			return false;

		if ( child->GetMetadata().GetCategory() != "PRODUCT" || child->GetMetadata().GetType() != "TYPE" )
			return false;

		return true;
	}

	bool Gmod::isProductSelectionAssignment( const GmodNode* parent, const GmodNode* child )
	{
		if ( parent == nullptr || child == nullptr )
			return false;

		if ( parent->GetMetadata().GetCategory().find( "FUNCTION" ) == std::string::npos )
			return false;

		if ( child->GetMetadata().GetCategory().find( "PRODUCT" ) == std::string::npos ||
			 child->GetMetadata().GetType() != "SELECTION" )
			return false;

		return true;
	}

	Gmod::Iterator::Iterator( ChdDictionary<GmodNode>::Iterator innerIt )
		: m_innerIt( std::move( innerIt ) )
	{
	}

	Gmod::Iterator::reference Gmod::Iterator::operator*() const
	{
		return m_innerIt->second;
	}

	Gmod::Iterator::pointer Gmod::Iterator::operator->() const
	{
		return &( m_innerIt->second );
	}

	Gmod::Iterator& Gmod::Iterator::operator++()
	{
		++m_innerIt;
		return *this;
	}

	Gmod::Iterator Gmod::Iterator::operator++( int )
	{
		Iterator tmp = *this;
		++( *this );
		return tmp;
	}

	bool Gmod::Iterator::operator==( const Iterator& other ) const
	{
		return m_innerIt == other.m_innerIt;
	}

	bool Gmod::Iterator::operator!=( const Iterator& other ) const
	{
		return m_innerIt != other.m_innerIt;
	}

	Gmod::Iterator Gmod::begin() const
	{
		return Iterator( m_nodeMap.begin() );
	}

	Gmod::Iterator Gmod::end() const
	{
		return Iterator( m_nodeMap.end() );
	}
}
