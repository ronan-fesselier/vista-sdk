#pragma once

#include "ILocalIdBuilder.h"
#include "LocalIdBuilder.h"

namespace dnv::vista::sdk
{
	template <typename TBuilder, typename TResult>
	TBuilder ILocalIdBuilder<TBuilder, TResult>::parse( const std::string& localIdStr )
	{
		ParsingErrors errors;
		std::optional<TBuilder> builder;
		if ( !tryParse( localIdStr, errors, builder ) )
		{
			SPDLOG_ERROR( "Failed to parse LocalId: {}", localIdStr );
			throw std::invalid_argument( "Failed to parse LocalId: " + errors.toString() );
		}

		if ( !builder.has_value() )
		{
			SPDLOG_ERROR( "Builder is null after successful parsing" );
			throw std::runtime_error( "Internal error: Builder is null after successful parsing" );
		}

		return builder.value();
	}

	template <typename TBuilder, typename TResult>
	bool ILocalIdBuilder<TBuilder, TResult>::tryParse( const std::string& localIdStr, std::optional<TBuilder>& localId )
	{
		ParsingErrors errors;
		return tryParse( localIdStr, errors, localId );
	}

	template <typename TBuilder, typename TResult>
	bool ILocalIdBuilder<TBuilder, TResult>::tryParse(
		const std::string& localIdStr,
		ParsingErrors& errors,
		std::optional<TBuilder>& localId )
	{
		SPDLOG_INFO( "Attempting to parse LocalId string: {}", localIdStr );

		std::optional<LocalIdBuilder> builder;
		if ( !LocalIdBuilder::tryParse( localIdStr, errors, builder ) )
		{
			SPDLOG_ERROR( "LocalId parsing failed in base builder" );
			localId = std::nullopt;
			return false;
		}

		if ( !builder.has_value() )
		{
			SPDLOG_INFO( "LocalId builder has no value after successful parsing" );
			localId = std::nullopt;
			return false;
		}

		localId = static_cast<TBuilder>( *builder );
		SPDLOG_INFO( "Successfully parsed and converted LocalId builder" );
		return true;
	}
}
