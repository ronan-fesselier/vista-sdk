#include "pch.h"

#include "dnv/vista/sdk/LocalId.h"
#include "dnv/vista/sdk/GmodPath.h"
#include "dnv/vista/sdk/LocalIdBuilder.h"
#include "dnv/vista/sdk/MetadataTag.h"
#include "dnv/vista/sdk/ParsingErrors.h"
#include "dnv/vista/sdk/VisVersion.h"

namespace dnv::vista::sdk
{
	const std::string LocalId::NamingRule = "dnv-v2";

	LocalId::LocalId( const LocalIdBuilder& builder )
		: m_builder( builder )
	{
		if ( m_builder.IsEmpty() )
			throw std::invalid_argument( "LocalId cannot be constructed from empty LocalIdBuilder" );
		if ( !m_builder.IsValid() )
			throw std::invalid_argument( "LocalId cannot be constructed from invalid LocalIdBuilder" );
	}

	const LocalIdBuilder& LocalId::GetBuilder() const
	{
		return m_builder;
	}

	VisVersion LocalId::GetVisVersion() const
	{
		return *m_builder.GetVisVersion();
	}

	bool LocalId::GetVerboseMode() const
	{
		return m_builder.GetVerboseMode();
	}

	const GmodPath& LocalId::GetPrimaryItem() const
	{
		return *m_builder.GetPrimaryItem();
	}

	std::optional<GmodPath> LocalId::GetSecondaryItem() const
	{
		return m_builder.GetSecondaryItem();
	}

	std::vector<MetadataTag> LocalId::GetMetadataTags() const
	{
		return m_builder.GetMetadataTags();
	}

	std::optional<MetadataTag> LocalId::GetQuantity() const
	{
		return m_builder.GetQuantity();
	}

	std::optional<MetadataTag> LocalId::GetContent() const
	{
		return m_builder.GetContent();
	}

	std::optional<MetadataTag> LocalId::GetCalculation() const
	{
		return m_builder.GetCalculation();
	}

	std::optional<MetadataTag> LocalId::GetState() const
	{
		return m_builder.GetState();
	}

	std::optional<MetadataTag> LocalId::GetCommand() const
	{
		return m_builder.GetCommand();
	}

	std::optional<MetadataTag> LocalId::GetType() const
	{
		return m_builder.GetType();
	}

	std::optional<MetadataTag> LocalId::GetPosition() const
	{
		return m_builder.GetPosition();
	}

	std::optional<MetadataTag> LocalId::GetDetail() const
	{
		return m_builder.GetDetail();
	}

	bool LocalId::HasCustomTag() const
	{
		return m_builder.HasCustomTag();
	}

	std::string LocalId::ToString() const
	{
		return m_builder.ToString();
	}

	LocalId LocalId::Parse( const std::string& localIdStr )
	{
		return LocalIdBuilder::Parse( localIdStr ).Build();
	}

	bool LocalId::TryParse( const std::string& localIdStr, ParsingErrors& errors, std::optional<LocalId>& localId )
	{
		std::optional<LocalIdBuilder> localIdBuilder;
		if ( !LocalIdBuilder::TryParse( localIdStr, errors, localIdBuilder ) )
		{
			localId = std::nullopt;
			return false;
		}

		localId.emplace( localIdBuilder->Build() );
		return true;
	}

	bool LocalId::operator==( const LocalId& other ) const
	{
		return m_builder == other.m_builder;
	}

	bool LocalId::operator!=( const LocalId& other ) const
	{
		return !( *this == other );
	}

	size_t LocalId::GetHashCode() const
	{
		return m_builder.GetHashCode();
	}
}
