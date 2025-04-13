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

	const std::string& GmodNodeMetadata::GetCategory() const
	{
		return m_category;
	}

	const std::string& GmodNodeMetadata::GetType() const
	{
		return m_type;
	}

	const std::string& GmodNodeMetadata::GetName() const
	{
		return m_name;
	}

	const std::optional<std::string>& GmodNodeMetadata::GetCommonName() const
	{
		return m_commonName;
	}

	const std::optional<std::string>& GmodNodeMetadata::GetDefinition() const
	{
		return m_definition;
	}

	const std::optional<std::string>& GmodNodeMetadata::GetCommonDefinition() const
	{
		return m_commonDefinition;
	}

	const std::optional<bool>& GmodNodeMetadata::GetInstallSubstructure() const
	{
		return m_installSubstructure;
	}

	const std::unordered_map<std::string, std::string>& GmodNodeMetadata::GetNormalAssignmentNames() const
	{
		return m_normalAssignmentNames;
	}

	const std::string& GmodNodeMetadata::GetFullType() const
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
		: m_code( "" ), m_location( std::nullopt ), m_visVersion( VisVersion::v3_8a ),
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

	const std::string& GmodNode::GetCode() const
	{
		return m_code;
	}

	const std::optional<Location>& GmodNode::GetLocation() const
	{
		return m_location;
	}

	VisVersion GmodNode::GetVisVersion() const
	{
		return m_visVersion;
	}

	const GmodNodeMetadata& GmodNode::GetMetadata() const
	{
		return m_metadata;
	}

	const std::vector<GmodNode*>& GmodNode::GetChildren() const
	{
		return m_children;
	}

	const std::vector<GmodNode*>& GmodNode::GetParents() const
	{
		return m_parents;
	}

	GmodNode GmodNode::WithoutLocation() const
	{
		if ( !m_location.has_value() )
			return *this;

		GmodNode result = *this;
		result.m_location = std::nullopt;
		return result;
	}

	GmodNode GmodNode::WithLocation( const std::string& locationStr ) const
	{
		Locations locations = VIS::Instance().GetLocations( m_visVersion );
		Location location = locations.Parse( locationStr );

		GmodNode result = *this;
		result.m_location = location;
		return result;
	}

	GmodNode GmodNode::TryWithLocation( const std::string& locationStr ) const
	{
		Locations locations = VIS::Instance().GetLocations( m_visVersion );
		Location location;

		if ( !locations.TryParse( std::string_view( locationStr ), location ) )
			return *this;

		return WithLocation( locationStr );
	}

	GmodNode GmodNode::TryWithLocation( const std::string& locationStr, ParsingErrors& errors ) const
	{
		Locations locations = VIS::Instance().GetLocations( m_visVersion );
		Location location;

		if ( !locations.TryParse( std::string_view( locationStr ), location, errors ) )
			return *this;

		return WithLocation( locationStr );
	}

	GmodNode GmodNode::WithLocation( const Location& location ) const
	{
		GmodNode result = *this;
		result.m_location = location;
		return result;
	}

	GmodNode GmodNode::TryWithLocation( const std::optional<Location>& location ) const
	{
		if ( !location.has_value() )
			return *this;

		return WithLocation( *location );
	}

	bool GmodNode::IsIndividualizable( bool isTargetNode, bool isInSet ) const
	{
		if ( m_metadata.GetType() == "GROUP" )
			return false;
		if ( m_metadata.GetType() == "SELECTION" )
			return false;
		if ( IsProductType() )
			return false;
		if ( m_metadata.GetCategory() == "ASSET" && m_metadata.GetType() == "TYPE" )
			return false;
		if ( IsFunctionComposition() )
			return m_code.back() == 'i' || isInSet || isTargetNode;
		return true;
	}

	bool GmodNode::IsFunctionComposition() const
	{
		return ( m_metadata.GetCategory() == "ASSET FUNCTION" || m_metadata.GetCategory() == "PRODUCT FUNCTION" ) && m_metadata.GetType() == "COMPOSITION";
	}

	bool GmodNode::IsMappable() const
	{
		if ( ProductType() != nullptr )
			return false;
		if ( ProductSelection() != nullptr )
			return false;
		if ( IsProductSelection() )
			return false;
		if ( IsAsset() )
			return false;

		char lastChar = m_code.back();
		return lastChar != 'a' && lastChar != 's';
	}

	bool GmodNode::IsProductSelection() const
	{
		return Gmod::IsProductSelection( m_metadata );
	}

	bool GmodNode::IsProductType() const
	{
		return Gmod::IsProductType( m_metadata );
	}

	bool GmodNode::IsAsset() const
	{
		return Gmod::IsAsset( m_metadata );
	}

	const GmodNode* GmodNode::ProductType() const
	{
		if ( m_children.size() != 1 )
			return nullptr;

		if ( m_metadata.GetCategory().find( "FUNCTION" ) == std::string::npos )
			return nullptr;

		const GmodNode* child = m_children[0];
		if ( child == nullptr )
			return nullptr;

		if ( child->m_metadata.GetCategory() != "PRODUCT" )
			return nullptr;

		if ( child->m_metadata.GetType() != "TYPE" )
			return nullptr;

		return child;
	}

	const GmodNode* GmodNode::ProductSelection() const
	{
		if ( m_children.size() != 1 )
			return nullptr;

		if ( m_metadata.GetCategory().find( "FUNCTION" ) == std::string::npos )
			return nullptr;

		const GmodNode* child = m_children[0];
		if ( child == nullptr )
			return nullptr;

		if ( child->m_metadata.GetCategory().find( "PRODUCT" ) == std::string::npos )
			return nullptr;

		if ( child->m_metadata.GetType() != "SELECTION" )
			return nullptr;

		return child;
	}

	bool GmodNode::IsChild( const GmodNode& node ) const
	{
		return IsChild( node.m_code );
	}

	bool GmodNode::IsChild( const std::string& code ) const
	{
		return m_childrenSet.find( code ) != m_childrenSet.end();
	}

	bool GmodNode::IsLeafNode() const
	{
		return Gmod::IsLeafNode( m_metadata );
	}

	bool GmodNode::IsFunctionNode() const
	{
		return Gmod::IsFunctionNode( m_metadata );
	}

	bool GmodNode::IsAssetFunctionNode() const
	{
		return Gmod::IsAssetFunctionNode( m_metadata );
	}

	bool GmodNode::IsRoot() const
	{
		return m_code == "VE";
	}

	bool GmodNode::operator==( const GmodNode& other ) const
	{
		return m_code == other.m_code && m_location == other.m_location;
	}

	bool GmodNode::operator!=( const GmodNode& other ) const
	{
		return !( *this == other );
	}

	std::string GmodNode::ToString() const
	{
		if ( !m_location.has_value() )
			return m_code;
		return m_code + "-" + m_location->ToString();
	}

	void GmodNode::ToString( std::stringstream& builder ) const
	{
		if ( !m_location.has_value() )
		{
			builder << m_code;
		}
		else
		{
			builder << m_code << "-" << m_location->ToString();
		}
	}

	void GmodNode::AddChild( GmodNode* child )
	{
		m_children.push_back( child );
	}

	void GmodNode::AddParent( GmodNode* parent )
	{
		m_parents.push_back( parent );
	}

	void GmodNode::Trim()
	{
		m_children.shrink_to_fit();
		m_parents.shrink_to_fit();

		m_childrenSet.clear();
		for ( const auto& child : m_children )
		{
			m_childrenSet.insert( child->GetCode() );
		}
	}
}
