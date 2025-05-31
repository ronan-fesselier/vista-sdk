/**
 * @file Codebooks.cpp
 * @brief Implementation of Vessel Information Structure (VIS) codebooks
 */

#include "pch.h"

#include "dnv/vista/sdk/Codebooks.h"

#include "dnv/vista/sdk/CodebooksDto.h"
#include "dnv/vista/sdk/MetadataTag.h"
#include "dnv/vista/sdk/VISVersion.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// Construction / destruction
	//=====================================================================

	Codebooks::Codebooks( VisVersion version, const CodebooksDto& dto )
		: m_visVersion{ version },
		  m_codebooks{}
	{
		for ( const auto& typeDto : dto.items() )
		{
			Codebook temp( typeDto );
			const auto index = static_cast<size_t>( temp.name() ) - 1;
			m_codebooks[index] = std::move( temp );
		}

		constexpr auto detailIndex = static_cast<size_t>( CodebookName::Detail ) - 1;

		static const std::unordered_map<std::string, std::vector<std::string>> s_emptyValues{};
		static const CodebookDto s_detailDto{ "detail", s_emptyValues };

		m_codebooks[detailIndex] = Codebook( s_detailDto );
	}

	//=====================================================================
	// Operators
	//=====================================================================

	const Codebook& Codebooks::operator[]( CodebookName name ) const
	{
		auto index = static_cast<size_t>( name ) - 1;

#ifdef NDEBUG
		[[assume( index < NUM_CODEBOOKS )]];
#else
		if ( index >= NUM_CODEBOOKS ) [[unlikely]]
		{
			throw std::invalid_argument( "Invalid codebook name" );
		}
#endif

		return m_codebooks[index];
	}

	//=====================================================================
	// Accessors
	//=====================================================================

	const Codebook& Codebooks::codebook( CodebookName name ) const
	{
		return ( *this )[name];
	}

	VisVersion Codebooks::visVersion() const
	{
		return m_visVersion;
	}

	//=====================================================================
	// Tag creation
	//=====================================================================

	std::optional<MetadataTag> Codebooks::tryCreateTag( CodebookName name, const std::string_view value ) const
	{
		return ( *this )[name].tryCreateTag( value );
	}

	MetadataTag Codebooks::createTag( CodebookName name, const std::string& value ) const
	{
		return ( *this )[name].createTag( value );
	}
}
