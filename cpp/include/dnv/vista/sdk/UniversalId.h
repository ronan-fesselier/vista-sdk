#pragma once

#include "IUniversalId.h"

namespace dnv::vista::sdk
{
	class IUniversalIdBuilder;
	class LocalId;
	class ParsingErrors;
	/**
	 * Represents a Universal Identifier.
	 */
	class UniversalId : public IUniversalId
	{
	public:
		/**
		 * Constructs a UniversalId from a builder.
		 * @param builder The builder used to construct the UniversalId.
		 * @throws std::invalid_argument If the builder is in an invalid state.
		 */
		explicit UniversalId( const std::shared_ptr<IUniversalIdBuilder>& builder );

		/**
		 * Gets the IMO number.
		 * @return The IMO number.
		 * @throws std::runtime_error If the IMO number is invalid.
		 */
		const ImoNumber& GetImoNumber() const override;

		/**
		 * Gets the local ID.
		 * @return The local ID.
		 */
		const LocalId& GetLocalId() const override;

		/**
		 * Checks equality with another UniversalId.
		 * @param other The other UniversalId to compare.
		 * @return True if equal, false otherwise.
		 */
		bool Equals( const UniversalId& other ) const;

		/**
		 * Parses a UniversalId from a string.
		 * @param universalIdStr The string representation of the UniversalId.
		 * @return The parsed UniversalId.
		 * @throws std::invalid_argument If parsing fails.
		 */
		static UniversalId Parse( const std::string& universalIdStr );

		/**
		 * Tries to parse a UniversalId from a string.
		 * @param universalIdStr The string representation of the UniversalId.
		 * @param errors Container for parsing errors.
		 * @param universalId The parsed UniversalId if successful.
		 * @return True if parsing succeeded, false otherwise.
		 */
		static bool TryParse( const std::string& universalIdStr, ParsingErrors& errors, std::unique_ptr<UniversalId>& universalId );

		/**
		 * Converts the UniversalId to a string representation.
		 * @return The string representation.
		 */
		std::string ToString() const;

		/**
		 * Gets the hash code of the UniversalId.
		 * @return The hash code.
		 */
		size_t GetHashCode() const;

	private:
		std::shared_ptr<IUniversalIdBuilder> m_builder;
		LocalId m_localId;
		ImoNumber m_imoNumber;
	};
}
