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

		// for ( const auto& [key, node] : m_nodeMap )
		//{
		//  SPDLOG_INFO( "Node added to GMOD map: {}", key );
		//}
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

		// for ( const auto& [key, node] : m_nodeMap )
		//{
		//	SPDLOG_INFO( "Node added to GMOD map: {}", key );
		//}
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
		return m_nodeMap[key];
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
		path = std::nullopt;

		try
		{
			if ( item.empty() )
			{
				SPDLOG_WARN( "TryParsePath: Input path is empty." );
				return false;
			}

			std::vector<std::string> parts;
			size_t start = 0, end = 0;

			while ( ( end = item.find( '/', start ) ) != std::string::npos )
			{
				if ( end > start )
					parts.push_back( item.substr( start, end - start ) );
				start = end + 1;
			}

			if ( start < item.size() )
				parts.push_back( item.substr( start ) );

			if ( parts.empty() )
			{
				SPDLOG_WARN( "TryParsePath: No valid parts found in the input path: {}", item );
				return false;
			}

			std::vector<GmodNode> nodes;
			for ( const auto& part : parts )
			{
				GmodNode node;
				if ( !TryGetNode( part, node ) )
				{
					SPDLOG_WARN( "TryParsePath: Node '{}' not found in GMOD.", part );
					return false;
				}
				nodes.push_back( node );
			}

			if ( nodes.empty() )
			{
				SPDLOG_WARN( "TryParsePath: No valid nodes resolved from the input path: {}", item );
				return false;
			}

			GmodNode targetNode = nodes.back();
			nodes.pop_back();
			path = GmodPath( nodes, targetNode );

			SPDLOG_INFO( "TryParsePath: Successfully parsed path: {}", item );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "TryParsePath: Exception occurred: {}", e.what() );
			return false;
		}
		catch ( ... )
		{
			SPDLOG_ERROR( "TryParsePath: Unknown exception occurred." );
			return false;
		}
	}

	GmodPath Gmod::ParseFromFullPath( const std::string& item ) const
	{
		return GmodPath::ParseFullPath( item, m_visVersion );
	}

	bool Gmod::TryParseFromFullPath( const std::string& item, std::optional<GmodPath>& path ) const
	{
		if ( !path.has_value() )
		{
			path.emplace();
		}

		auto p = path.value();
		return GmodPath::TryParseFullPath( item, m_visVersion, p );
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
