/**
 * @file UniversalId.h
 * @brief
 * @details

 */

#pragma once

#include "IUniversalId.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// Forward declarations
	//=====================================================================

	class IUniversalIdBuilder;
	class UniversalIdBuilder;
	class LocalId;
	class ParsingErrors;

	//=====================================================================
	// UniversalId Class
	//=====================================================================

	class UniversalId final : public IUniversalId
	{
	public:
		//----------------------------------------------
		// Construction / destruction
		//----------------------------------------------

		explicit UniversalId( const UniversalIdBuilder& builder );

		/** @brief Default constructor. */
		UniversalId() = delete;

		/** @brief Copy constructor */
		UniversalId( const UniversalId& ) = delete;

		/** @brief Move constructor */
		UniversalId( UniversalId&& ) noexcept = default;

		/** @brief Destructor */
		virtual ~UniversalId() = default;

		//----------------------------------------------
		// Assignment operators
		//----------------------------------------------

		/** @brief Copy assignment operator */
		UniversalId& operator=( const UniversalId& ) = delete;

		/** @brief Move assignment operator */
		UniversalId& operator=( UniversalId&& ) noexcept = default;

		//----------------------------------------------
		// Operators
		//----------------------------------------------

		/**
		 * Equality operator.
		 * @param other The other UniversalId to compare.
		 * @return True if equal, false otherwise.
		 */
		bool operator==( const UniversalId& other ) const;

		/**
		 * Inequality operator.
		 * @param other The other UniversalId to compare.
		 * @return True if not equal, false otherwise.
		 */
		virtual bool operator!=( const UniversalId& other ) const;

		//----------------------------------------------
		// Accessors
		//----------------------------------------------

		/**
		 * Gets the IMO number.
		 * @return The IMO number.
		 * @throws std::runtime_error If the IMO number is invalid.
		 */
		virtual const ImoNumber& imoNumber() const override;

		/**
		 * Gets the local ID.
		 * @return The local ID.
		 */
		virtual const LocalId& localId() const override;

		[[nodiscard]] size_t hashCode() const noexcept;

		//----------------------------------------------
		// Conversion and comparison
		//----------------------------------------------

		/**
		 * Converts the UniversalId to a string representation.
		 * @return The string representation.
		 */
		[[nodiscard]] virtual std::string toString() const override;

		/**
		 * Checks equality with another UniversalId.
		 * @param other The other UniversalId to compare.
		 * @return True if equal, false otherwise.
		 */
		virtual bool equals( const UniversalId& other ) const;

		//----------------------------------------------
		// Static parsing methods
		//----------------------------------------------

		/**
		 * Parses a UniversalId from a string.
		 * @param universalIdStr The string representation of the UniversalId.
		 * @return The parsed UniversalId.
		 * @throws std::invalid_argument If parsing fails.
		 */
		static UniversalId parse( const std::string& universalIdStr );

		/**
		 * Tries to parse a UniversalId from a string.
		 * @param universalIdStr The string representation of the UniversalId.
		 * @param errors Container for parsing errors.
		 * @param universalId The parsed UniversalId if successful.
		 * @return True if parsing succeeded, false otherwise.
		 */
		static bool tryParse( const std::string& universalIdStr, ParsingErrors& errors, std::optional<UniversalId>& universalId );

	private:
		//----------------------------------------------
		// Private member variables
		//----------------------------------------------

		ImoNumber m_imoNumber;
		std::shared_ptr<IUniversalIdBuilder> m_builder;
		LocalId m_localId;
	};
}
