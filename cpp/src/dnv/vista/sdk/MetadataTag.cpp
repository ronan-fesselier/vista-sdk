#include "pch.h"

#include "dnv/vista/sdk/MetadataTag.h"

namespace dnv::vista::sdk
{
	MetadataTag::MetadataTag( CodebookName name, const std::string& value, bool isCustom )
		: m_name( name ), m_value( value ), m_isCustom( isCustom )
	{
	}

	CodebookName MetadataTag::GetName() const
	{
		return m_name;
	}

	const std::string& MetadataTag::GetValue() const
	{
		return m_value;
	}

	bool MetadataTag::IsCustom() const
	{
		return m_isCustom;
	}

	char MetadataTag::GetPrefix() const
	{
		return m_isCustom ? '~' : '-';
	}

	MetadataTag::operator std::string() const
	{
		return m_value;
	}

	bool MetadataTag::operator==( const MetadataTag& other ) const
	{
		if ( m_name != other.m_name )
			throw std::invalid_argument( "Can't compare tags with different names" );

		return m_value == other.m_value;
	}

	bool MetadataTag::operator!=( const MetadataTag& other ) const
	{
		return !( *this == other );
	}

	size_t MetadataTag::GetHashCode() const
	{
		return std::hash<std::string>{}( m_value );
	}

	std::string MetadataTag::ToString() const
	{
		return m_value;
	}

	void MetadataTag::ToString( std::ostringstream& builder, char separator ) const
	{
		std::string prefix;

		switch ( m_name )
		{
			case CodebookName::Position:
				prefix = "pos";
				break;
			case CodebookName::Quantity:
				prefix = "qty";
				break;
			case CodebookName::Calculation:
				prefix = "calc";
				break;
			case CodebookName::State:
				prefix = "state";
				break;
			case CodebookName::Content:
				prefix = "cnt";
				break;
			case CodebookName::Command:
				prefix = "cmd";
				break;
			case CodebookName::Type:
				prefix = "type";
				break;
			case CodebookName::FunctionalServices:
				prefix = "funct.svc";
				break;
			case CodebookName::MaintenanceCategory:
				prefix = "maint.cat";
				break;
			case CodebookName::ActivityType:
				prefix = "act.type";
				break;
			case CodebookName::Detail:
				prefix = "detail";
				break;
			default:
				throw std::invalid_argument( "Unknown metadata tag: " +
											 std::to_string( static_cast<int>( m_name ) ) );
		}

		auto append = [&builder]( const std::string& str ) {
			builder << str;
		};

		append( prefix );
		append( IsCustom() ? std::to_string( '~' ) : std::to_string( '-' ) );
		append( m_value );
		append( std::to_string( separator ) );
	}
}
