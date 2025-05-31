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
	// Codebooks class
	//=====================================================================

	//----------------------------------------------
	// Construction / destruction
	//----------------------------------------------

	Codebooks::Codebooks( VisVersion version, const CodebooksDto& dto )
		: m_visVersion{ version },
		  m_codebooks{}
	{
		const auto& items = dto.items();

		for ( const auto& typeDto : items )
		{
			Codebook codebook( typeDto );
			const auto name = codebook.name();
			const auto index = static_cast<size_t>( name ) - 1;

			m_codebooks[index] = std::move( codebook );
		}

		const CodebookDto s_detailDto( "detail", CodebookDto::ValuesMap{} );
		constexpr auto detailIndex = static_cast<size_t>( CodebookName::Detail ) - 1;
		m_codebooks[detailIndex] = Codebook( s_detailDto );
	}

	//----------------------------------------------
	// Lookup operators
	//----------------------------------------------

	const Codebook& Codebooks::operator[]( CodebookName name ) const noexcept
	{
		return m_codebooks[static_cast<size_t>( name ) - 1];
	}

	//----------------------------------------------
	// Range-based iterator
	//----------------------------------------------

	Codebooks::Iterator Codebooks::begin() const noexcept
	{
		return m_codebooks.data();
	}

	Codebooks::Iterator Codebooks::end() const noexcept
	{
		return m_codebooks.data() + NUM_CODEBOOKS;
	}

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	VisVersion Codebooks::visVersion() const noexcept
	{
		return m_visVersion;
	}

	const Codebook& Codebooks::codebook( CodebookName name ) const
	{
		const auto rawValue = static_cast<size_t>( name );
		if ( rawValue == 0 || rawValue > NUM_CODEBOOKS ) [[unlikely]]
		{
			throw std::invalid_argument( "Invalid codebook name: " + std::to_string( rawValue ) );
		}

		const auto index = rawValue - 1;

		return m_codebooks[index];
	}

	//----------------------------------------------
	// Tag creation
	//----------------------------------------------

	std::optional<MetadataTag> Codebooks::tryCreateTag( CodebookName name, const std::string_view value ) const
	{
		return m_codebooks[static_cast<size_t>( name ) - 1].tryCreateTag( value );
	}

	MetadataTag Codebooks::createTag( CodebookName name, const std::string& value ) const
	{
		return m_codebooks[static_cast<size_t>( name ) - 1].createTag( value );
	}
}
