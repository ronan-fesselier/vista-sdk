#include "pch.h"

#include "dnv/vista/sdk/Gmod.h"

#include "dnv/vista/sdk/GmodDto.h"
#include "dnv/vista/sdk/VisVersion.h"
#include "dnv/vista/sdk/GmodPath.h"

namespace dnv::vista::sdk
{
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
			const std::string& parentCode = relation[0];
			const std::string& childCode = relation[1];

			GmodNode& parentNode = nodeMap[parentCode];
			GmodNode& childNode = nodeMap[childCode];

			parentNode.AddChild( &childNode );
			childNode.AddParent( &parentNode );
		}

		for ( auto& [code, node] : nodeMap )
		{
			node.Trim();
		}

		m_rootNode = nodeMap["VE"];

		std::vector<std::pair<std::string, GmodNode>> nodePairs;
		for ( const auto& [code, node] : nodeMap )
		{
			nodePairs.emplace_back( code, node );
		}

		m_nodeMap = ChdDictionary<GmodNode>( std::move( nodePairs ) );
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

	VisVersion Gmod::GetVisVersion() const
	{
		return m_visVersion;
	}

	const GmodNode& Gmod::GetRootNode() const
	{
		return m_rootNode;
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
			if ( TryGetNode( key, node ) )
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

	bool Gmod::TryGetNode( const std::string& code, GmodNode& node ) const
	{
		return m_nodeMap.TryGetValue( code, &node );
	}

	bool Gmod::TryGetNode( std::string_view code, GmodNode& node ) const
	{
		if ( !m_nodeMap.TryGetValue( code, &node ) )
		{
			SPDLOG_WARN( "TryGetNode: Node '{}' not found in GMOD.", code );
			return false;
		}
		SPDLOG_INFO( "TryGetNode: Node '{}' found in GMOD.", code );
		return true;
	}

	GmodPath Gmod::ParsePath( const std::string& item ) const
	{
		return GmodPath::Parse( item, m_visVersion );
	}

	bool Gmod::TryParsePath( const std::string& item, std::optional<GmodPath>& path ) const
	{
		try
		{
			if ( item.empty() )
			{
				return false;
			}

			if ( item.find( '/' ) == std::string::npos )
			{
				GmodNode node;
				if ( TryGetNode( item, node ) )
				{
					GmodPath tempPath;
					if ( GmodPath::TryParse( item, m_visVersion, tempPath ) )
					{
						path = std::move( tempPath );
						return true;
					}
					else
					{
						SPDLOG_WARN( "TryParsePath: Failed to create path for code: {}", item );
						return false;
					}
				}
				else
				{
					SPDLOG_WARN( "TryParsePath: Node not found for code: {}", item );
					return false;
				}
			}

			try
			{
				path = GmodPath::Parse( item, m_visVersion );
				return true;
			}
			catch ( const std::exception& ex )
			{
				SPDLOG_ERROR( "TryParsePath: Exception in Parse: {}", ex.what() );
				return false;
			}
		}
		catch ( const std::exception& ex )
		{
			SPDLOG_ERROR( "TryParsePath: Exception occurred: {}", ex.what() );
			return false;
		}
		catch ( ... )
		{
			SPDLOG_ERROR( "TryParsePath: Unknown exception occurred" );
			return false;
		}
	}

	GmodPath Gmod::ParseFromFullPath( const std::string& item ) const
	{
		return GmodPath::ParseFullPath( item, m_visVersion );
	}

	bool Gmod::TryParseFromFullPath( const std::string& item, std::optional<GmodPath>& path ) const
	{
		GmodPath tempPath;

		if ( GmodPath::TryParseFullPath( item, m_visVersion, tempPath ) )
		{
			path = std::move( tempPath );
			return true;
		}

		return false;
	}

	bool Gmod::Traverse( const TraverseHandler& handler, const TraversalOptions& options ) const
	{
		return TraverseFrom( GetRootNode(), handler, options );
	}

	bool Gmod::TraverseFrom( const GmodNode& startNode, const TraverseHandler& handler, const TraversalOptions& options ) const
	{
		struct StackItem
		{
			std::string nodeCode;
			std::vector<std::string> parentCodes;
		};

		std::stack<StackItem> stack;

		stack.push( { startNode.GetCode(), {} } );

		while ( !stack.empty() )
		{
			auto item = stack.top();
			stack.pop();

			GmodNode currentNode;
			if ( !TryGetNode( item.nodeCode, currentNode ) )
			{
				continue;
			}

			int occurrences = 0;
			for ( const auto& code : item.parentCodes )
			{
				if ( code == item.nodeCode )
				{
					occurrences++;
					if ( occurrences >= options.MaxOccurrence )
					{
						break;
					}
				}
			}

			if ( occurrences >= options.MaxOccurrence )
			{
				continue;
			}

			std::vector<GmodNode> parentNodes;
			parentNodes.reserve( item.parentCodes.size() );

			for ( const auto& parentCode : item.parentCodes )
			{
				GmodNode parentNode;
				if ( TryGetNode( parentCode, parentNode ) )
				{
					parentNodes.push_back( std::move( parentNode ) );
				}
			}

			auto result = handler( parentNodes, currentNode );

			if ( result == TraversalHandlerResult::Stop )
			{
				return false;
			}

			if ( result != TraversalHandlerResult::SkipSubtree )
			{
				std::vector<std::string> newParentCodes = item.parentCodes;
				newParentCodes.push_back( item.nodeCode );

				const auto& children = currentNode.GetChildren();

				for ( size_t i = 0; i < children.size(); ++i )
				{
					const GmodNode* child = nullptr;
					if ( i < children.size() )
					{
						child = children[children.size() - 1 - i];
					}

					if ( child )
					{
						stack.push( { child->GetCode(), newParentCodes } );
					}
				}
			}
		}

		return true;
	}

	bool Gmod::IsPotentialParent( const std::string& type )
	{
		return std::find( s_potentialParentScopeTypes.begin(), s_potentialParentScopeTypes.end(), type ) !=
			   s_potentialParentScopeTypes.end();
	}

	bool Gmod::IsLeafNode( const std::string& fullType )
	{
		return std::find( s_leafTypes.begin(), s_leafTypes.end(), fullType ) != s_leafTypes.end();
	}

	bool Gmod::IsLeafNode( const GmodNodeMetadata& metadata )
	{
		return IsLeafNode( metadata.GetFullType() );
	}

	bool Gmod::IsFunctionNode( const std::string& category )
	{
		return category != "PRODUCT" && category != "ASSET";
	}

	bool Gmod::IsFunctionNode( const GmodNodeMetadata& metadata )
	{
		return IsFunctionNode( metadata.GetCategory() );
	}

	bool Gmod::IsProductSelection( const GmodNodeMetadata& metadata )
	{
		return metadata.GetCategory() == "PRODUCT" && metadata.GetType() == "SELECTION";
	}

	bool Gmod::IsProductType( const GmodNodeMetadata& metadata )
	{
		return metadata.GetCategory() == "PRODUCT" && metadata.GetType() == "TYPE";
	}

	bool Gmod::IsAsset( const GmodNodeMetadata& metadata )
	{
		return metadata.GetCategory() == "ASSET";
	}

	bool Gmod::IsAssetFunctionNode( const GmodNodeMetadata& metadata )
	{
		return metadata.GetCategory() == "ASSET FUNCTION";
	}

	bool Gmod::IsProductTypeAssignment( const GmodNode* parent, const GmodNode* child )
	{
		if ( parent == nullptr || child == nullptr )
			return false;

		if ( parent->GetMetadata().GetCategory().find( "FUNCTION" ) == std::string::npos )
			return false;

		if ( child->GetMetadata().GetCategory() != "PRODUCT" || child->GetMetadata().GetType() != "TYPE" )
			return false;

		return true;
	}

	bool Gmod::IsProductSelectionAssignment( const GmodNode* parent, const GmodNode* child )
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
