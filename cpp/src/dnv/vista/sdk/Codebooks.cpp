/**
 * @file Codebooks.cpp
 * @brief Implementation of Vessel Information Structure (VIS) codebooks
 */

#include "pch.h"

#include "dnv/vista/sdk/Config.h"
#include "dnv/vista/sdk/Codebooks.h"
#include "dnv/vista/sdk/CodebooksDto.h"
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
			const auto index = static_cast<size_t>( codebook.name() ) - 1;
			m_codebooks[index] = std::move( codebook );
		}

		const CodebookDto detailDto( std::string( CODEBOOK_NAME_DETAIL ), CodebookDto::ValuesMap{} );
		constexpr auto detailIndex = static_cast<size_t>( CodebookName::Detail ) - 1;
		m_codebooks[detailIndex] = Codebook( detailDto );
	}
}
