#include "pch.h"

#include "dnv/vista/sdk/ILocalIdBuilder.h"
#include "dnv/vista/sdk/LocalIdBuilder.h"

namespace dnv::vista::sdk
{
	template <typename TBuilder, typename TResult>
	bool ILocalIdBuilder<TBuilder, TResult>::TryParse(
		const std::string& localIdStr,
		ParsingErrors& errors,
		std::optional<TBuilder>& localId )
	{
		std::optional<LocalIdBuilder> builder;
		if ( !LocalIdBuilder::TryParse( localIdStr, errors, builder ) )
		{
			localId = std::nullopt;
			return false;
		}

		if ( !builder.has_value() )
		{
			localId = std::nullopt;
			return false;
		}

		localId = static_cast<TBuilder>( *builder );
		return true;
	}

	template class ILocalIdBuilder<LocalIdBuilder, LocalId>;
}
