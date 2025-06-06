/**
 * @file GmodNode.cpp
 * @brief Implementation of the GmodNode and GmodNodeMetadata classes
 */

#include "pch.h"

#include "dnv/vista/sdk/GmodNode.h"

#include "dnv/vista/sdk/Gmod.h"
#include "dnv/vista/sdk/ParsingErrors.h"
#include "dnv/vista/sdk/VIS.h"
#include "dnv/vista/sdk/VISVersion.h"

namespace dnv::vista::sdk
{
	namespace
	{
		//=====================================================================
		// Constants
		//=====================================================================

		static constexpr std::string_view NODE_CATEGORY_PRODUCT = "PRODUCT";
		static constexpr std::string_view NODE_CATEGORY_VALUE_FUNCTION = "FUNCTION";
		static constexpr std::string_view NODE_CATEGORY_ASSET = "ASSET";
		static constexpr std::string_view NODE_CATEGORY_PRODUCT_FUNCTION = "PRODUCT FUNCTION";
		static constexpr std::string_view NODE_CATEGORY_ASSET_FUNCTION = "ASSET FUNCTION";

		static constexpr std::string_view NODE_TYPE_GROUP = "GROUP";
		static constexpr std::string_view NODE_TYPE_COMPOSITION = "COMPOSITION";

		static constexpr std::string_view NODE_TYPE_VALUE_TYPE = "TYPE";
		static constexpr std::string_view NODE_TYPE_VALUE_SELECTION = "SELECTION";
	}

	//=====================================================================
	// GmodNodeMetadata class
	//=====================================================================

	//----------------------------------------------
	// Construction / destruction
	//----------------------------------------------

	GmodNodeMetadata::GmodNodeMetadata(
		const std::string& category,
		const std::string& type,
		const std::string& name,
		const std::optional<std::string>& commonName,
		const std::optional<std::string>& definition,
		const std::optional<std::string>& commonDefinition,
		const std::optional<bool>& installSubstructure,
		const std::unordered_map<std::string, std::string>& normalAssignmentNames )
		: m_category{ category },
		  m_type{ type },
		  m_name{ name },
		  m_commonName{ commonName },
		  m_definition{ definition },
		  m_commonDefinition{ commonDefinition },
		  m_installSubstructure{ installSubstructure },
		  m_normalAssignmentNames{ normalAssignmentNames },
		  m_fullType{ category + " " + type }
	{
	}

	GmodNodeMetadata::GmodNodeMetadata( const GmodNodeMetadata& other )
		: m_category{ other.m_category },
		  m_type{ other.m_type },
		  m_name{ other.m_name },
		  m_commonName{ other.m_commonName },
		  m_definition{ other.m_definition },
		  m_commonDefinition{ other.m_commonDefinition },
		  m_installSubstructure{ other.m_installSubstructure },
		  m_normalAssignmentNames{ other.m_normalAssignmentNames },
		  m_fullType{ other.m_fullType }
	{
	}

	GmodNodeMetadata& GmodNodeMetadata::operator=( const GmodNodeMetadata& other )
	{
		if ( this == &other )
		{
			return *this;
		}

		m_category = other.m_category;
		m_type = other.m_type;
		m_name = other.m_name;
		m_commonName = other.m_commonName;
		m_definition = other.m_definition;
		m_commonDefinition = other.m_commonDefinition;
		m_installSubstructure = other.m_installSubstructure;
		m_normalAssignmentNames = other.m_normalAssignmentNames;
		m_fullType = other.m_fullType;

		return *this;
	}

	//----------------------------------------------
	// Operators
	//----------------------------------------------

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

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	const std::string& GmodNodeMetadata::category() const
	{
		return m_category;
	}

	const std::string& GmodNodeMetadata::type() const
	{
		return m_type;
	}

	const std::string& GmodNodeMetadata::fullType() const
	{
		return m_fullType;
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

	//=====================================================================
	// GmodNode class
	//=====================================================================

	//----------------------------------------------
	// Construction / destruction
	//----------------------------------------------

	GmodNode::GmodNode( VisVersion version, const GmodNodeDto& dto )
		: m_code{ dto.code() },
		  m_location{ std::nullopt },
		  m_visVersion{ version },
		  m_metadata{
			  dto.category(),
			  dto.type(),
			  dto.name(),
			  dto.commonName(),
			  dto.definition(),
			  dto.commonDefinition(),
			  dto.installSubstructure(),
			  dto.normalAssignmentNames().has_value() ? *dto.normalAssignmentNames() : std::unordered_map<std::string, std::string>() },
		  m_children{},
		  m_parents{},
		  m_childrenSet{}
	{
		m_children.reserve( 8 );
		m_parents.reserve( 4 );
		m_childrenSet.reserve( 8 );
	}

	GmodNode::GmodNode( const GmodNode& other )
		: m_code{ other.m_code },
		  m_location{ other.m_location },
		  m_visVersion{ other.m_visVersion },
		  m_metadata{ other.m_metadata },
		  m_children{ other.m_children },
		  m_parents{ other.m_parents },
		  m_childrenSet{ other.m_childrenSet },
		  m_cachedProductType{ other.m_cachedProductType },
		  m_cachedProductSelection{ other.m_cachedProductSelection }
	{
	}

	//----------------------------------------------
	// Assignment operators
	//----------------------------------------------

	GmodNode& GmodNode::operator=( const GmodNode& other )
	{
		if ( this == &other )
		{
			return *this;
		}

		m_code = other.m_code;
		m_location = other.m_location;
		m_visVersion = other.m_visVersion;
		m_metadata = other.m_metadata;
		m_children = other.m_children;
		m_parents = other.m_parents;
		m_childrenSet = other.m_childrenSet;
		m_cachedProductType = other.m_cachedProductType;
		m_cachedProductSelection = other.m_cachedProductSelection;

		return *this;
	}

	//----------------------------------------------
	// Operators
	//----------------------------------------------

	bool GmodNode::operator==( const GmodNode& other ) const
	{
		if ( m_code != other.m_code )
		{
			return false;
		}

		if ( m_location.has_value() )
		{
			if ( !other.m_location.has_value() )
			{
				return false;
			}
			if ( m_location.value() != other.m_location.value() )
			{
				return false;
			}
		}
		else
		{
			if ( other.m_location.has_value() )
			{
				return false;
			}
		}

		return true;
	}

	bool GmodNode::equals( const GmodNode& other ) const
	{
		return ( *this == other );
	}

	bool GmodNode::operator!=( const GmodNode& other ) const
	{
		return !( *this == other );
	}

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

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

	size_t GmodNode::hashCode() const noexcept
	{
		size_t hash = std::hash<std::string>{}( m_code );

		if ( m_location.has_value() )
		{
			hash ^= ( std::hash<std::string>{}( m_location->toString() ) ) + 0x9e3779b9 + ( hash << 6 ) + ( hash >> 2 );
		}

		return hash;
	}

	//----------------------------------------------
	// Relationship accessors
	//----------------------------------------------

	const std::vector<GmodNode*>& GmodNode::children() const
	{
		return m_children;
	}

	const std::vector<GmodNode*>& GmodNode::parents() const
	{
		return m_parents;
	}

	const GmodNode* GmodNode::productType() const
	{
		if ( m_children.size() != 1 )
		{
			return nullptr;
		}

		if ( m_metadata.category().find( NODE_CATEGORY_VALUE_FUNCTION ) == std::string::npos )
		{
			SPDLOG_WARN( "Product type check failed: expected FUNCTION category, found {}", m_metadata.category() );
			return nullptr;
		}

		const GmodNode* child = m_children[0];
		if ( !child )
		{
			SPDLOG_WARN( "Product type check failed: child is null" );
			return nullptr;
		}

		if ( child->m_metadata.category() != NODE_CATEGORY_PRODUCT )
		{
			SPDLOG_WARN( "Product type check failed: expected PRODUCT category, found {}", child->m_metadata.category() );
			return nullptr;
		}

		if ( child->m_metadata.type() != NODE_TYPE_VALUE_TYPE )
		{
			SPDLOG_WARN( "Product type check failed: expected TYPE type, found {}", child->m_metadata.type() );
			return nullptr;
		}

		return child;
	}

	const GmodNode* GmodNode::productSelection() const
	{
		if ( m_children.size() != 1 )
		{
			SPDLOG_WARN( "Product selection check failed: expected 1 child, found {}", m_children.size() );
			return nullptr;
		}

		if ( m_metadata.category().find( NODE_CATEGORY_VALUE_FUNCTION ) == std::string::npos )
		{
			SPDLOG_WARN( "Product selection check failed: current node category '{}' does not contain '{}'", m_metadata.category(), NODE_CATEGORY_VALUE_FUNCTION );
			return nullptr;
		}

		const GmodNode* child = m_children[0];
		if ( !child )
		{
			SPDLOG_WARN( "Product selection check failed: child pointer is null" );
			return nullptr;
		}

		if ( child->m_metadata.category().find( NODE_CATEGORY_PRODUCT ) == std::string::npos )
		{
			SPDLOG_WARN( "Product selection check failed: child category '{}' does not contain '{}'", child->m_metadata.category(), NODE_CATEGORY_PRODUCT );
			return nullptr;
		}

		if ( child->m_metadata.type() != NODE_TYPE_VALUE_SELECTION )
		{
			SPDLOG_WARN( "Product selection check failed: child type '{}' is not '{}'", child->m_metadata.type(), NODE_TYPE_VALUE_SELECTION );
			return nullptr;
		}

		return child;
	}

	//----------------------------------------------
	// Node location methods
	//----------------------------------------------

	GmodNode GmodNode::withoutLocation() const
	{
		if ( !m_location.has_value() )
		{
			return *this;
		}

		GmodNode result = *this;
		result.m_location = std::nullopt;

		return result;
	}

	GmodNode GmodNode::withLocation( const Location& location ) const
	{
		GmodNode result = *this;
		result.m_location = location;

		return result;
	}

	GmodNode GmodNode::withLocation( std::string_view locationStr ) const
	{
		Locations locations = VIS::instance().locations( m_visVersion );
		Location location = locations.parse( locationStr );

		return withLocation( location );
	}

	GmodNode GmodNode::tryWithLocation( std::string_view locationStr ) const
	{
		Locations locations = VIS::instance().locations( m_visVersion );
		Location parsedLocation;

		if ( !locations.tryParse( locationStr, parsedLocation ) )
		{
			return *this;
		}

		return withLocation( parsedLocation );
	}

	GmodNode GmodNode::tryWithLocation( std::string_view locationStr, ParsingErrors& errors ) const
	{
		auto locations = VIS::instance().locations( m_visVersion );

		Location location;
		if ( !locations.tryParse( locationStr, location, errors ) )
		{
			return *this;
		}

		return withLocation( location );
	}

	GmodNode GmodNode::tryWithLocation( const std::optional<Location>& location ) const
	{
		if ( !location.has_value() )
		{
			return *this;
		}

		return withLocation( location.value() );
	}

	//----------------------------------------------
	// Node type checking methods
	//----------------------------------------------

	bool GmodNode::isIndividualizable( bool isTargetNode, bool isInSet ) const
	{
		if ( m_metadata.type() == NODE_TYPE_GROUP )
		{
			return false;
		}
		if ( m_metadata.type() == NODE_TYPE_VALUE_SELECTION )
		{
			return false;
		}
		if ( isProductType() )
		{
			return false;
		}
		if ( m_metadata.category() == NODE_CATEGORY_ASSET && m_metadata.type() == NODE_TYPE_VALUE_TYPE )
		{
			return false;
		}
		if ( isFunctionComposition() )
		{
			if ( m_code.empty() )
			{
				SPDLOG_WARN( "isIndividualizable: Code is empty, cannot check last character for 'i'. Node: {}", m_code );
				return false;
			}

			return m_code.back() == 'i' || isInSet || isTargetNode;
		}

		return true;
	}

	bool GmodNode::isFunctionComposition() const
	{
		return ( m_metadata.category() == NODE_CATEGORY_ASSET_FUNCTION ||
				   m_metadata.category() == NODE_CATEGORY_PRODUCT_FUNCTION ) &&
			   m_metadata.type() == NODE_TYPE_COMPOSITION;
	}

	bool GmodNode::isMappable() const noexcept
	{
		if ( !m_cachedProductType.has_value() )
		{
			if ( m_children.size() == 1 &&
				 m_metadata.category().find( NODE_CATEGORY_VALUE_FUNCTION ) != std::string::npos )
			{
				const GmodNode* child = m_children[0];
				if ( child &&
					 child->m_metadata.category() == NODE_CATEGORY_PRODUCT &&
					 child->m_metadata.type() == NODE_TYPE_VALUE_TYPE )
				{
					m_cachedProductType = child;
				}
				else
				{
					m_cachedProductType = nullptr;
				}
			}
			else
			{
				m_cachedProductType = nullptr;
			}
		}

		if ( m_cachedProductType.value() != nullptr )
		{
			return false;
		}

		if ( !m_cachedProductSelection.has_value() )
		{
			if ( m_children.size() == 1 &&
				 m_metadata.category().find( NODE_CATEGORY_VALUE_FUNCTION ) != std::string::npos )
			{
				const GmodNode* child = m_children[0];
				if ( child &&
					 child->m_metadata.category().find( NODE_CATEGORY_PRODUCT ) != std::string::npos &&
					 child->m_metadata.type() == NODE_TYPE_VALUE_SELECTION )
				{
					m_cachedProductSelection = child;
				}
				else
				{
					m_cachedProductSelection = nullptr;
				}
			}
			else
			{
				m_cachedProductSelection = nullptr;
			}
		}

		if ( m_cachedProductSelection.value() != nullptr )
		{
			return false;
		}

		if ( m_metadata.category().find( NODE_CATEGORY_PRODUCT ) != std::string::npos && m_metadata.type() == NODE_TYPE_VALUE_SELECTION )
		{
			return false;
		}

		if ( m_metadata.category() == NODE_CATEGORY_ASSET )
		{
			return false;
		}

		if ( m_code.empty() )
		{
			return false;
		}

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

	bool GmodNode::isRoot() const noexcept
	{
		return m_code == "VE";
	}

	//----------------------------------------------
	// Node relationship query methods
	//----------------------------------------------

	bool GmodNode::isChild( const GmodNode& node ) const
	{
		return isChild( node.m_code );
	}

	bool GmodNode::isChild( const std::string& code ) const
	{
		if ( m_children.size() <= 8 )
		{
			for ( const auto* child : m_children )
			{
				if ( child->code() == code )
				{
					return true;
				}
			}

			return false;
		}

		return m_childrenSet.contains( code );
	}

	//----------------------------------------------
	// Utility methods
	//----------------------------------------------

	std::string GmodNode::toString() const
	{
		return m_location.has_value() ? fmt::format( "{}-{}", m_code, m_location->toString() ) : m_code;
	}

	void GmodNode::toString( std::stringstream& builder ) const
	{
		builder << m_code;

		if ( m_location.has_value() )
		{
			builder << "-" << m_location->toString();
		}
	}

	//----------------------------------------------
	// Relationship management methods
	//----------------------------------------------

	void GmodNode::addChild( GmodNode* child )
	{
		if ( !child )
		{
			return;
		}

		const std::string& childCode = child->code();
		if ( m_childrenSet.contains( childCode ) )
		{
			return;
		}

		m_children.push_back( child );
		m_childrenSet.emplace( childCode );

		m_cachedProductType.reset();
		m_cachedProductSelection.reset();
	}

	void GmodNode::addParent( GmodNode* parent )
	{
		if ( !parent )
		{
			return;
		}

		m_parents.push_back( parent );
	}

	void GmodNode::trim()
	{
		m_children.shrink_to_fit();
		m_parents.shrink_to_fit();

		if ( m_childrenSet.size() != m_children.size() )
		{
			m_childrenSet.clear();
			m_childrenSet.reserve( m_children.size() );

			for ( const auto* child : m_children )
			{
				m_childrenSet.emplace( child->code() );
			}
		}
	}
}
