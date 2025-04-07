#include "pch.h"

#include "dnv/vista/sdk/ILocalId.h"
#include "dnv/vista/sdk/ParsingErrors.h"
#include "dnv/vista/sdk/VisVersion.h"
#include "dnv/vista/sdk/GmodPath.h"
#include "dnv/vista/sdk/MetadataTag.h"

namespace dnv::vista::sdk
{
	template <typename T>
	T ILocalId<T>::Parse( const std::string& localIdStr )
	{
		ParsingErrors errors;
		std::optional<T> localId;
		if ( !TryParse( localIdStr, errors, localId ) )
		{
			throw std::invalid_argument( "Failed to parse LocalId: " + errors.ToString() );
		}
		return *localId;
	}

	template <typename T>
	bool ILocalId<T>::TryParse( const std::string& localIdStr, ParsingErrors& errors, std::optional<T>& localId )
	{
		try
		{
			localId = T( localIdStr );
			return true;
		}
		catch ( const std::exception& e )
		{
			(void)e;
			localId.reset();
			return false;
		}
	}
}
