#include "pch.h"

#include "dnv/vista/sdk/GmodNode.h"

#include "dnv/vista/sdk/Gmod.h"
#include "dnv/vista/sdk/GmodDto.h"
#include "dnv/vista/sdk/GmodPath.h"
#include "dnv/vista/sdk/ParsingErrors.h"
#include "dnv/vista/sdk/VIS.h"
#include "dnv/vista/sdk/VisVersion.h"

namespace dnv::vista::sdk
{
	GmodNodeMetadata::GmodNodeMetadata( const std::string& category,
		const std::string& type,
		const std::string& name,
		const std::optional<std::string>& commonName,
		const std::optional<std::string>& definition,
		const std::optional<std::string>& commonDefinition,
		const std::optional<bool>& installSubstructure,
		const std::unordered_map<std::string, std::string>& normalAssignmentNames )
		: m_category( category ), m_type( type ), m_name( name ), m_commonName( commonName ), m_definition( definition ), m_commonDefinition( commonDefinition ), m_installSubstructure( installSubstructure ), m_normalAssignmentNames( normalAssignmentNames )
	{
		m_fullType = category + " " + type;
	}

	const std::string& GmodNodeMetadata::category() const
	{
		return m_category;
	}

	const std::string& GmodNodeMetadata::type() const
	{
		return m_type;
	}

	const std::string& GmodNodeMetadata::name() const
	{
		return m_name;
	}

	const std::optional<std::string>& GmodNodeMetadata::commonName() const
	{
		return m_commonName;
	}

	const std::optional<std::string>& GmodNodeMetadata::definition() const
	{
		return m_definition;
	}

	const std::optional<std::string>& GmodNodeMetadata::commonDefinition() const
	{
		return m_commonDefinition;
	}

	const std::optional<bool>& GmodNodeMetadata::installSubstructure() const
	{
		return m_installSubstructure;
	}

	const std::unordered_map<std::string, std::string>& GmodNodeMetadata::normalAssignmentNames() const
	{
		return m_normalAssignmentNames;
	}

	const std::string& GmodNodeMetadata::fullType() const
	{
		return m_fullType;
	}

	bool GmodNodeMetadata::operator==( const GmodNodeMetadata& other ) const
	{
		return m_category == other.m_category &&
			   m_type == other.m_type &&
			   m_name == other.m_name &&
			   m_commonName == other.m_commonName &&
			   m_definition == other.m_definition &&
			   m_commonDefinition == other.m_commonDefinition &&
			   m_installSubstructure == other.m_installSubstructure &&
			   m_normalAssignmentNames == other.m_normalAssignmentNames;
	}

	bool GmodNodeMetadata::operator!=( const GmodNodeMetadata& other ) const
	{
		return !( *this == other );
	}

	GmodNode::GmodNode()
		: m_code( "" ), m_location( std::nullopt ), m_visVersion( VisVersion::v3_8a ), // TODO
		  m_metadata( "", "", "", std::nullopt, std::nullopt, std::nullopt, std::nullopt, {} ),
		  m_children(), m_parents(), m_childrenSet()
	{
	}

	GmodNode::GmodNode( VisVersion version, const GmodNodeDto& dto )
		: m_code( dto.code ), m_location( std::nullopt ),
		  m_visVersion( version ), m_metadata(
									   dto.category,
									   dto.type,
									   dto.name,
									   dto.commonName,
									   dto.definition,
									   dto.commonDefinition,
									   dto.installSubstructure,
									   dto.normalAssignmentNames ? *dto.normalAssignmentNames : std::unordered_map<std::string, std::string>() ),
		  m_children(), m_parents(), m_childrenSet()
	{
	}

	GmodNode::GmodNode( const GmodNode& other )
		: m_code( other.m_code ),
		  m_location( other.m_location ),
		  m_visVersion( other.m_visVersion ),
		  m_metadata( other.m_metadata ),
		  m_childrenSet( other.m_childrenSet )
	{
		m_children.reserve( other.m_children.size() );
		for ( const auto child : other.m_children )
		{
			if ( child != nullptr )
			{
				m_children.push_back( child );
			}
		}

		m_parents.reserve( other.m_parents.size() );
		for ( const auto parent : other.m_parents )
		{
			if ( parent != nullptr )
			{
				m_parents.push_back( parent );
			}
		}
	}

	const std::string& GmodNode::code() const
	{
		return m_code;
	}

	const std::optional<Location>& GmodNode::location() const
	{
		return m_location;
	}

	VisVersion GmodNode::visVersion() const
	{
		return m_visVersion;
	}

	const GmodNodeMetadata& GmodNode::metadata() const
	{
		return m_metadata;
	}

	const std::vector<GmodNode*>& GmodNode::children() const
	{
		return m_children;
	}

	const std::vector<GmodNode*>& GmodNode::parents() const
	{
		return m_parents;
	}

	GmodNode GmodNode::withoutLocation() const
	{
		if ( !m_location.has_value() )
			return *this;

		GmodNode result = *this;
		result.m_location = std::nullopt;
		return result;
	}

	GmodNode GmodNode::withLocation( const std::string& locationStr ) const
	{
		Locations locations = VIS::instance().locations( m_visVersion );
		Location location = locations.parse( locationStr );

		GmodNode result = *this;
		result.m_location = location;
		return result;
	}

	GmodNode GmodNode::tryWithLocation( const std::string& locationStr ) const
	{
		Locations locations = VIS::instance().locations( m_visVersion );
		Location location;

		if ( !locations.tryParse( std::string_view( locationStr ), location ) )
			return *this;

		return withLocation( locationStr );
	}

	GmodNode GmodNode::tryWithLocation( const std::string& locationStr, ParsingErrors& errors ) const
	{
		Locations locations = VIS::instance().locations( m_visVersion );
		Location location;

		if ( !locations.tryParse( std::string_view( locationStr ), location, errors ) )
			return *this;

		return withLocation( locationStr );
	}

	GmodNode GmodNode::withLocation( const Location& location ) const
	{
		GmodNode result = *this;
		result.m_location = location;
		return result;
	}

	GmodNode GmodNode::tryWithLocation( const std::optional<Location>& location ) const
	{
		if ( !location.has_value() )
			return *this;

		return withLocation( *location );
	}

	bool GmodNode::isIndividualizable( bool isTargetNode, bool isInSet ) const
	{
		if ( m_metadata.type() == "GROUP" )
			return false;
		if ( m_metadata.type() == "SELECTION" )
			return false;
		if ( isProductType() )
			return false;
		if ( m_metadata.category() == "ASSET" && m_metadata.type() == "TYPE" )
			return false;
		if ( isFunctionComposition() )
			return m_code.back() == 'i' || isInSet || isTargetNode;
		return true;
	}

	bool GmodNode::isFunctionComposition() const
	{
		return ( m_metadata.category() == "ASSET FUNCTION" || m_metadata.category() == "PRODUCT FUNCTION" ) && m_metadata.type() == "COMPOSITION";
	}

	bool GmodNode::isMappable() const
	{
		if ( productType() != nullptr )
			return false;
		if ( productSelection() != nullptr )
			return false;
		if ( isProductSelection() )
			return false;
		if ( isAsset() )
			return false;

		char lastChar = m_code.back();
		return lastChar != 'a' && lastChar != 's';
	}

	bool GmodNode::isProductSelection() const
	{
		return Gmod::isProductSelection( m_metadata );
	}

	bool GmodNode::isProductType() const
	{
		return Gmod::isProductType( m_metadata );
	}

	bool GmodNode::isAsset() const
	{
		return Gmod::isAsset( m_metadata );
	}

	bool GmodNode::isLeafNode() const
	{
		return Gmod::isLeafNode( m_metadata );
	}

	bool GmodNode::isFunctionNode() const
	{
		return Gmod::isFunctionNode( m_metadata );
	}

	bool GmodNode::isAssetFunctionNode() const
	{
		return Gmod::isAssetFunctionNode( m_metadata );
	}

	bool GmodNode::isRoot() const
	{
		return m_code == "VE";
	}

	bool GmodNode::isChild( const GmodNode& node ) const
	{
		return isChild( node.m_code );
	}

	bool GmodNode::isChild( const std::string& code ) const
	{
		return m_childrenSet.find( code ) != m_childrenSet.end();
	}

	const GmodNode* GmodNode::productType() const
	{
		if ( m_children.size() != 1 )
			return nullptr;

		if ( m_metadata.category().find( "FUNCTION" ) == std::string::npos )
			return nullptr;

		const GmodNode* child = m_children[0];
		if ( child == nullptr )
			return nullptr;

		if ( child->m_metadata.category() != "PRODUCT" )
			return nullptr;

		if ( child->m_metadata.type() != "TYPE" )
			return nullptr;

		return child;
	}

	const GmodNode* GmodNode::productSelection() const
	{
		if ( m_children.size() != 1 )
			return nullptr;

		if ( m_metadata.category().find( "FUNCTION" ) == std::string::npos )
			return nullptr;

		const GmodNode* child = m_children[0];
		if ( child == nullptr )
			return nullptr;

		if ( child->m_metadata.category().find( "PRODUCT" ) == std::string::npos )
			return nullptr;

		if ( child->m_metadata.type() != "SELECTION" )
			return nullptr;

		return child;
	}

	bool GmodNode::operator==( const GmodNode& other ) const
	{
		return m_code == other.m_code && m_location == other.m_location;
	}

	bool GmodNode::operator!=( const GmodNode& other ) const
	{
		return !( *this == other );
	}

	std::string GmodNode::toString() const
	{
		if ( m_location.has_value() )
		{
			return m_code + "-" + m_location->toString();
		}
		return m_code;
	}

	void GmodNode::toString( std::stringstream& builder ) const
	{
		builder << m_code;

		if ( m_location.has_value() )
		{
			builder << "-" << m_location->toString();
		}

		SPDLOG_INFO( "GmodNode::ToString: {}", builder.str() );
	}

	void GmodNode::addChild( GmodNode* child )
	{
		m_children.push_back( child );
	}

	void GmodNode::addParent( GmodNode* parent )
	{
		m_parents.push_back( parent );
	}

	void GmodNode::trim()
	{
		m_children.shrink_to_fit();
		m_parents.shrink_to_fit();

		m_childrenSet.clear();
		for ( const auto& child : m_children )
		{
			m_childrenSet.insert( child->code() );
		}
	}
}
