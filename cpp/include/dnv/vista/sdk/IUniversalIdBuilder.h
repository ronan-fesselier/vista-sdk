#pragma once

#include "ImoNumber.h"
#include "LocalIdBuilder.h"

namespace dnv::vista::sdk
{
	/**
	 * Interface for building universal identifiers.
	 */
	class UniversalIdBuilder;
	class IUniversalIdBuilder
	{
	public:
		virtual ~IUniversalIdBuilder() = default;

		/**
		 * Checks if the builder is in a valid state.
		 * @return True if valid, false otherwise.
		 */
		virtual bool IsValid() const = 0;

		/**
		 * Gets the IMO number.
		 * @return The optional IMO number.
		 */
		virtual std::optional<ImoNumber> GetImoNumber() const = 0;

		/**
		 * Gets the local ID builder.
		 * @return The optional local ID builder.
		 */
		virtual std::optional<LocalIdBuilder> GetLocalId() const = 0;

		/**
		 * Sets the local ID.
		 * @param localId The local ID builder.
		 * @return A new UniversalIdBuilder instance with the updated local ID.
		 */
		virtual UniversalIdBuilder WithLocalId( const LocalIdBuilder& localId ) = 0;

		/**
		 * Removes the local ID.
		 * @return A new UniversalIdBuilder instance without the local ID.
		 */
		virtual UniversalIdBuilder WithoutLocalId() = 0;

		/**
		 * Tries to set the local ID.
		 * @param localId The optional local ID builder.
		 * @return A new UniversalIdBuilder instance with the updated local ID.
		 */
		virtual UniversalIdBuilder TryWithLocalId( const std::optional<LocalIdBuilder>& localId ) = 0;

		/**
		 * Tries to set the local ID.
		 * @param localId The optional local ID builder.
		 * @param succeeded Output parameter indicating success.
		 * @return A new UniversalIdBuilder instance with the updated local ID.
		 */
		virtual UniversalIdBuilder TryWithLocalId( const std::optional<LocalIdBuilder>& localId, bool& succeeded ) = 0;

		/**
		 * Sets the IMO number.
		 * @param imoNumber The IMO number.
		 * @return A new UniversalIdBuilder instance with the updated IMO number.
		 */
		virtual UniversalIdBuilder WithImoNumber( const ImoNumber& imoNumber ) = 0;

		/**
		 * Tries to set the IMO number.
		 * @param imoNumber The optional IMO number.
		 * @return A new UniversalIdBuilder instance with the updated IMO number.
		 */
		virtual UniversalIdBuilder TryWithImoNumber( const std::optional<ImoNumber>& imoNumber ) = 0;

		/**
		 * Tries to set the IMO number.
		 * @param imoNumber The optional IMO number.
		 * @param succeeded Output parameter indicating success.
		 * @return A new UniversalIdBuilder instance with the updated IMO number.
		 */
		virtual UniversalIdBuilder TryWithImoNumber( const std::optional<ImoNumber>& imoNumber, bool& succeeded ) = 0;

		/**
		 * Removes the IMO number.
		 * @return A new UniversalIdBuilder instance without the IMO number.
		 */
		virtual UniversalIdBuilder WithoutImoNumber() = 0;

		/**
		 * Converts the builder to a string representation.
		 * @return The string representation.
		 */
		virtual std::string ToString() const = 0;
	};
}
