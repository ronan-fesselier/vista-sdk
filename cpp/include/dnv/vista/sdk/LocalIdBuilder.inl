/**
 * @file LocalIdBuilder.inl
 * @brief Inline implementations for performance-critical LocalIdBuilder operations
 */

#pragma once

#include "CodebookName.h"
#include "utils/StringBuilderPool.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// LocalIdBuilder class
	//=====================================================================

	//----------------------------------------------
	// Assignment operators
	//----------------------------------------------

	inline LocalIdBuilder& LocalIdBuilder::operator=( LocalIdBuilder&& other ) noexcept
	{
		if ( this != &other )
		{
			m_visVersion = std::move( other.m_visVersion );
			m_verboseMode = other.m_verboseMode;
			m_items = std::move( other.m_items );
			m_quantity = std::move( other.m_quantity );
			m_content = std::move( other.m_content );
			m_calculation = std::move( other.m_calculation );
			m_state = std::move( other.m_state );
			m_command = std::move( other.m_command );
			m_type = std::move( other.m_type );
			m_position = std::move( other.m_position );
			m_detail = std::move( other.m_detail );
		}
		return *this;
	}

	//----------------------------------------------
	// Operators
	//----------------------------------------------

	inline bool LocalIdBuilder::operator==( const LocalIdBuilder& other ) const
	{
		return equals( other );
	}

	inline bool LocalIdBuilder::operator!=( const LocalIdBuilder& other ) const
	{
		return !equals( other );
	}

	inline bool LocalIdBuilder::equals( const LocalIdBuilder& other ) const
	{
		if ( m_visVersion != other.m_visVersion )
		{
			throw std::invalid_argument( "Cant compare local IDs from different VIS versions" );
		}

		return m_items.primaryItem() == other.m_items.primaryItem() &&
			   m_items.secondaryItem() == other.m_items.secondaryItem() &&
			   m_quantity == other.m_quantity &&
			   m_calculation == other.m_calculation &&
			   m_content == other.m_content &&
			   m_position == other.m_position &&
			   m_state == other.m_state &&
			   m_command == other.m_command &&
			   m_type == other.m_type &&
			   m_detail == other.m_detail;
	}

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	inline std::optional<VisVersion> LocalIdBuilder::visVersion() const noexcept
	{
		return m_visVersion;
	}

	inline const std::optional<GmodPath>& LocalIdBuilder::primaryItem() const noexcept
	{
		return m_items.primaryItem();
	}

	inline const std::optional<GmodPath>& LocalIdBuilder::secondaryItem() const noexcept
	{
		return m_items.secondaryItem();
	}

	inline std::vector<MetadataTag> LocalIdBuilder::metadataTags() const noexcept
	{
		std::vector<MetadataTag> tags;
		tags.reserve( 8 );

		if ( m_quantity.has_value() )
		{
			tags.push_back( *m_quantity );
		}
		if ( m_calculation.has_value() )
		{
			tags.push_back( *m_calculation );
		}
		if ( m_content.has_value() )
		{
			tags.push_back( *m_content );
		}
		if ( m_position.has_value() )
		{
			tags.push_back( *m_position );
		}
		if ( m_state.has_value() )
		{
			tags.push_back( *m_state );
		}
		if ( m_command.has_value() )
		{
			tags.push_back( *m_command );
		}
		if ( m_type.has_value() )
		{
			tags.push_back( *m_type );
		}
		if ( m_detail.has_value() )
		{
			tags.push_back( *m_detail );
		}

		return tags;
	}

	VISTA_SDK_CPP_FORCE_INLINE size_t LocalIdBuilder::hashCode() const noexcept
	{
		size_t hash = constants::FNV_OFFSET_BASIS;

		auto addToHash = [&hash]( size_t value ) {
			hash = hash * 31 + value;
		};

		if ( m_items.primaryItem().has_value() )
		{
			addToHash( m_items.primaryItem()->hashCode() );
		}
		if ( m_items.secondaryItem().has_value() )
		{
			addToHash( m_items.secondaryItem()->hashCode() );
		}
		if ( m_quantity.has_value() )
		{
			addToHash( m_quantity->hashCode() );
		}
		if ( m_calculation.has_value() )
		{
			addToHash( m_calculation->hashCode() );
		}
		if ( m_content.has_value() )
		{
			addToHash( m_content->hashCode() );
		}
		if ( m_position.has_value() )
		{
			addToHash( m_position->hashCode() );
		}
		if ( m_state.has_value() )
		{
			addToHash( m_state->hashCode() );
		}
		if ( m_command.has_value() )
		{
			addToHash( m_command->hashCode() );
		}
		if ( m_type.has_value() )
		{
			addToHash( m_type->hashCode() );
		}
		if ( m_detail.has_value() )
		{
			addToHash( m_detail->hashCode() );
		}

		return hash;
	}

	//----------------------------------------------
	// State inspection methods
	//----------------------------------------------

	inline bool LocalIdBuilder::isValid() const noexcept
	{
		if ( !m_visVersion.has_value() )
		{
			return false;
		}

		if ( !m_items.primaryItem().has_value() )
		{
			return false;
		}

		if ( !m_quantity.has_value() &&
			 !m_calculation.has_value() &&
			 !m_content.has_value() &&
			 !m_position.has_value() &&
			 !m_state.has_value() &&
			 !m_command.has_value() &&
			 !m_type.has_value() &&
			 !m_detail.has_value() )
		{
			return false;
		}

		return true;
	}

	inline bool LocalIdBuilder::isEmpty() const noexcept
	{
		bool itemsEmpty = !m_items.primaryItem().has_value() &&
						  !m_items.secondaryItem().has_value();

		bool metadataEmpty = !m_quantity.has_value() &&
							 !m_content.has_value() &&
							 !m_calculation.has_value() &&
							 !m_state.has_value() &&
							 !m_command.has_value() &&
							 !m_type.has_value() &&
							 !m_position.has_value() &&
							 !m_detail.has_value();

		return itemsEmpty && metadataEmpty;
	}

	inline bool LocalIdBuilder::isVerboseMode() const noexcept
	{
		return m_verboseMode;
	}

	//----------------------------------------------
	// Metadata inspection methods
	//----------------------------------------------

	inline bool LocalIdBuilder::hasCustomTag() const noexcept
	{
		return ( m_quantity.has_value() && m_quantity->isCustom() ) ||
			   ( m_content.has_value() && m_content->isCustom() ) ||
			   ( m_calculation.has_value() && m_calculation->isCustom() ) ||
			   ( m_state.has_value() && m_state->isCustom() ) ||
			   ( m_command.has_value() && m_command->isCustom() ) ||
			   ( m_type.has_value() && m_type->isCustom() ) ||
			   ( m_position.has_value() && m_position->isCustom() ) ||
			   ( m_detail.has_value() && m_detail->isCustom() );
	}

	inline bool LocalIdBuilder::isEmptyMetadata() const noexcept
	{
		return !m_quantity.has_value() &&
			   !m_content.has_value() &&
			   !m_calculation.has_value() &&
			   !m_state.has_value() &&
			   !m_command.has_value() &&
			   !m_type.has_value() &&
			   !m_position.has_value() &&
			   !m_detail.has_value();
	}

	inline const LocalIdItems& LocalIdBuilder::items() const noexcept
	{
		return m_items;
	}

	inline const std::optional<MetadataTag>& LocalIdBuilder::quantity() const noexcept
	{
		return m_quantity;
	}

	inline const std::optional<MetadataTag>& LocalIdBuilder::content() const noexcept
	{
		return m_content;
	}

	inline const std::optional<MetadataTag>& LocalIdBuilder::calculation() const noexcept
	{
		return m_calculation;
	}

	inline const std::optional<MetadataTag>& LocalIdBuilder::state() const noexcept
	{
		return m_state;
	}

	inline const std::optional<MetadataTag>& LocalIdBuilder::command() const noexcept
	{
		return m_command;
	}

	inline const std::optional<MetadataTag>& LocalIdBuilder::type() const noexcept
	{
		return m_type;
	}

	inline const std::optional<MetadataTag>& LocalIdBuilder::position() const noexcept
	{
		return m_position;
	}

	inline const std::optional<MetadataTag>& LocalIdBuilder::detail() const noexcept
	{
		return m_detail;
	}

	//----------------------------------------------
	// String conversion
	//----------------------------------------------

	inline std::string LocalIdBuilder::toString() const
	{
		/* LocalId format: /dnv-v2/vis-{version}/{primary-item}[/sec/{secondary-item}][~{description}]/meta/{metadata-tags} */
		auto lease = utils::StringBuilderPool::instance();
		auto builder = lease.Builder();

		toString( builder );

		return lease.toString();
	}

	template <typename StringBuilder>
	inline void LocalIdBuilder::toString( StringBuilder& builder ) const
	{
		if ( !m_visVersion.has_value() )
		{
			throw std::invalid_argument( "No VisVersion configured on LocalId" );
		}

		/* Naming rule prefix: "/dnv-v2" */
		builder.append( "/" );
		builder.append( namingRule );
		builder.append( "/" );

		/* VIS version: "vis-{major}-{minor}{patch}" */
		builder.append( VisVersionExtensions::toVersionString( *m_visVersion ) );
		builder.append( "/" );

		/* Items section: primary item [+ secondary item] [+ description] */
		m_items.append( std::back_inserter( builder ), m_verboseMode );

		/* Metadata section prefix: "/meta" */
		builder.append( "meta/" );

		/* Metadata tags: {prefix}{separator}{value} */
		auto appendMetadata = [&builder]( const std::optional<MetadataTag>& tag ) {
			if ( tag.has_value() )
			{
				const auto prefix = CodebookNames::toPrefix( tag->name() );
				builder.append( prefix );
				builder.push_back( tag->prefix() );
				builder.append( tag->value() );
				builder.push_back( '/' );
			}
		};

		appendMetadata( m_quantity );
		appendMetadata( m_content );
		appendMetadata( m_calculation );
		appendMetadata( m_state );
		appendMetadata( m_command );
		appendMetadata( m_type );
		appendMetadata( m_position );
		appendMetadata( m_detail );

		/* Cleanup trailing slash */
		if ( builder.size() > 0 && builder[builder.size() - 1] == '/' )
		{
			builder.resize( builder.size() - 1 );
		}
	}
}
