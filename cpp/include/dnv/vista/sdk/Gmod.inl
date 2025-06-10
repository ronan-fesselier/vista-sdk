/**
 * @file Gmod.inl
 * @brief Inline implementations for performance-critical Gmod operations
 */

#include "GmodConstants.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// Gmod class
	//=====================================================================

	//----------------------------------------------
	// Lookup operators
	//----------------------------------------------

	inline const GmodNode& Gmod::operator[]( std::string_view key ) const
	{
		const GmodNode* nodePtr = nullptr;
		bool found = m_nodeMap.tryGetValue( key, nodePtr );
		if ( found && nodePtr != nullptr )
		{
			return *nodePtr;
		}

		throw std::out_of_range( fmt::format( "Key not found in Gmod node map: {}", key ) );
	}

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	inline VisVersion Gmod::visVersion() const
	{
		return m_visVersion;
	}

	inline const GmodNode& Gmod::rootNode() const
	{
		if ( !m_rootNode )
		{
			throw std::runtime_error( "Root node is not initialized or 'VE' was not found." );
		}

		return *m_rootNode;
	}

	//----------------------------------------------
	// Static state inspection methods
	//----------------------------------------------

	inline bool Gmod::isPotentialParent( const std::string& type )
	{
		return type == NODE_TYPE_SELECTION ||
			   type == NODE_TYPE_GROUP ||
			   type == NODE_TYPE_LEAF;
	}

	inline bool Gmod::isLeafNode( const GmodNodeMetadata& metadata )
	{
		const auto& fullType = metadata.fullType();

		return fullType == NODE_FULLTYPE_ASSET_FUNCTION_LEAF ||
			   fullType == NODE_FULLTYPE_PRODUCT_FUNCTION_LEAF;
	}

	inline bool Gmod::isFunctionNode( const GmodNodeMetadata& metadata )
	{
		const auto& category = metadata.category();

		return category != NODE_CATEGORY_PRODUCT &&
			   category != NODE_CATEGORY_ASSET;
	}

	inline bool Gmod::isProductSelection( const GmodNodeMetadata& metadata )
	{
		return metadata.category() == NODE_CATEGORY_PRODUCT &&
			   metadata.type() == NODE_TYPE_SELECTION;
	}

	inline bool Gmod::isProductType( const GmodNodeMetadata& metadata )
	{
		return metadata.category() == NODE_CATEGORY_PRODUCT &&
			   metadata.type() == NODE_TYPE_TYPE;
	}

	inline bool Gmod::isAsset( const GmodNodeMetadata& metadata )
	{
		return metadata.category() == NODE_CATEGORY_ASSET;
	}

	inline bool Gmod::isAssetFunctionNode( const GmodNodeMetadata& metadata )
	{
		return metadata.category() == NODE_CATEGORY_ASSET_FUNCTION;
	}

	inline bool Gmod::isProductTypeAssignment( const GmodNode* parent, const GmodNode* child ) noexcept
	{
		if ( !parent || !child )
		{
			return false;
		}

		const auto& parentCategory = parent->metadata().category();
		const auto& childCategory = child->metadata().category();
		const auto& childType = child->metadata().type();

		if ( parentCategory.find( NODE_CATEGORY_FUNCTION ) == std::string_view::npos )
		{
			return false;
		}

		return childCategory == NODE_CATEGORY_PRODUCT &&
			   childType == NODE_TYPE_TYPE;
	}

	inline bool Gmod::isProductSelectionAssignment( const GmodNode* parent, const GmodNode* child )
	{
		if ( !parent || !child )
		{
			return false;
		}

		const auto& parentCategory = parent->metadata().category();
		const auto& childCategory = child->metadata().category();
		const auto& childType = child->metadata().type();

		if ( parentCategory.find( NODE_CATEGORY_FUNCTION ) == std::string_view::npos )
		{
			return false;
		}

		return childCategory.find( NODE_CATEGORY_PRODUCT ) != std::string_view::npos &&
			   childType == NODE_TYPE_SELECTION;
	}
}
