#pragma once

#include "GmodPath.h"

namespace dnv::vista::sdk
{
	/**
	 * @brief Structure representing primary and secondary items for a LocalId
	 */
	class LocalIdItems
	{
	private:
		std::optional<GmodPath> m_primaryItem;
		std::optional<GmodPath> m_secondaryItem;

	public:
		/**
		 * @brief Default constructor
		 */
		LocalIdItems() = default;
		LocalIdItems& operator=( const LocalIdItems& other );
		LocalIdItems( const LocalIdItems& other );

		/**
		 * @brief Equality operator
		 * @param other The other LocalIdItems to compare
		 * @return true if equal, false otherwise
		 */
		bool operator==( const LocalIdItems& other ) const;

		/**
		 * @brief Inequality operator
		 * @param other The other LocalIdItems to compare
		 * @return true if not equal, false otherwise
		 */
		bool operator!=( const LocalIdItems& other ) const;

		/**
		 * @brief Constructor with primary and secondary items
		 * @param primaryItem Optional primary item
		 * @param secondaryItem Optional secondary item
		 */
		LocalIdItems(
			const std::optional<GmodPath>& primaryItem,
			const std::optional<GmodPath>& secondaryItem );

		/**
		 * @brief Get primary item
		 * @return Optional primary item
		 */
		const std::optional<GmodPath>& GetPrimaryItem() const;

		/**
		 * @brief Get secondary item
		 * @return Optional secondary item
		 */
		const std::optional<GmodPath>& GetSecondaryItem() const;

		/**
		 * @brief Append items to string builder
		 * @param builder String stream to append to
		 * @param verboseMode Whether to include verbose output
		 */
		void Append( std::stringstream& builder, bool verboseMode ) const;

	private:
		/**
		 * @brief Append common name with location
		 * @param builder String stream to append to
		 * @param commonName Common name to append
		 * @param location Optional location string
		 */
		static void AppendCommonName( std::stringstream& builder,
			const std::string& commonName,
			const std::optional<std::string>& location );
	};
}
