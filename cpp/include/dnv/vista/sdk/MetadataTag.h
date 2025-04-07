#pragma once

#include "CodebookName.h"

namespace dnv::vista::sdk
{
	/**
	 * @brief Represents a metadata tag in the VIS system.
	 *
	 * A metadata tag consists of a name, value, and an optional flag indicating
	 * whether it is a custom tag. It is used to store and manage metadata
	 * associated with various entities in the system.
	 */
	class MetadataTag
	{
	public:
		/**
		 * @brief Constructs a MetadataTag object.
		 * @param name The name of the metadata tag (from the CodebookName enumeration).
		 * @param value The value of the metadata tag.
		 * @param isCustom Indicates whether the tag is custom (default is false).
		 */
		MetadataTag( CodebookName name, const std::string& value, bool isCustom = false );

		/**
		 * @brief Gets the name of the metadata tag.
		 * @return The name of the metadata tag.
		 */
		CodebookName GetName() const;

		/**
		 * @brief Gets the value of the metadata tag.
		 * @return A reference to the string value of the metadata tag.
		 */
		const std::string& GetValue() const;

		/**
		 * @brief Checks if the metadata tag is custom.
		 * @return True if the tag is custom, false otherwise.
		 */
		bool IsCustom() const;

		/**
		 * @brief Gets the prefix character for the metadata tag.
		 * @return '~' if the tag is custom, '-' otherwise.
		 */
		char GetPrefix() const;

		/**
		 * @brief Implicit conversion to a string.
		 * @return The value of the metadata tag as a string.
		 */
		operator std::string() const;

		/**
		 * @brief Equality operator.
		 * @param other The other MetadataTag to compare.
		 * @return True if the tags are equal, false otherwise.
		 * @throws std::invalid_argument If the names of the tags are different.
		 */
		bool operator==( const MetadataTag& other ) const;

		/**
		 * @brief Inequality operator.
		 * @param other The other MetadataTag to compare.
		 * @return True if the tags are not equal, false otherwise.
		 */
		bool operator!=( const MetadataTag& other ) const;

		/**
		 * @brief Gets the hash code of the metadata tag.
		 * @return The hash code of the tag's value.
		 */
		size_t GetHashCode() const;

		/**
		 * @brief Converts the metadata tag to a string representation.
		 * @return A string representation of the metadata tag.
		 */
		std::string ToString() const;

		/**
		 * @brief Appends the metadata tag to a string stream.
		 * @param builder The string stream to append to.
		 * @param separator The separator character to use (default is '/').
		 */
		void ToString( std::ostringstream& builder, char separator = '/' ) const;

	private:
		CodebookName m_name; ///< The name of the metadata tag.
		std::string m_value; ///< The value of the metadata tag.
		bool m_isCustom;	 ///< Indicates whether the tag is custom.
	};
}
