/**
 * @file Codebooks.h
 * @brief Container for Vessel Information Structure (VIS) codebooks
 * @details Provides access to standardized codebooks as defined in ISO 19848,
 *          with support for retrieving codebook entries, validating values, and
 *          creating metadata tags. This container is immutable after construction.
 * @see ISO 19848 - Ships and marine technology - Standard data for shipboard machinery and equipment
 */

#pragma once

#include "Codebook.h"
#include "CodebookName.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// Forward declarations
	//=====================================================================

	class MetadataTag;
	class CodebooksDto;
	enum class VisVersion;

	//=====================================================================
	// Constants
	//=====================================================================

	/**
	 * @brief Number of codebooks based on the enum values.
	 * @details Size accommodates 0-based indexing derived from 1-based enum values.
	 *          Example: CodebookName::Position (value 1) maps to index 0.
	 *          The value is derived from the last enumerator in CodebookName.
	 */
	static constexpr size_t NUM_CODEBOOKS = static_cast<size_t>( CodebookName::Detail );

	/**
	 * @class Codebooks
	 * @brief Container for all codebooks in a specific VIS version.
	 * @details Provides access to standard codebooks defined in ISO 19848,
	 *          supports iteration over codebooks and metadata tag creation.
	 *          This container is immutable after construction.
	 */
	class Codebooks final
	{
	public:
		//=====================================================================
		// Iterator
		//=====================================================================

		//=====================================================================
		// Construction / destruction
		//=====================================================================

		/**
		 * @brief Construct codebooks from a Data Transfer Object (DTO).
		 * @details Initializes the container with codebooks based on the provided DTO
		 *          for the specified VIS version. Ensures the 'detail' codebook exists.
		 * @param version The `VisVersion` these codebooks belong to.
		 * @param dto The `CodebooksDto` containing the raw codebook data.
		 */
		explicit Codebooks( VisVersion version, const CodebooksDto& dto );

		/** @brief Default constructor */
		Codebooks() = default;

		/** @brief Copy constructor */
		Codebooks( const Codebooks& ) = delete;

		/** @brief Move constructor */
		Codebooks( Codebooks&& ) noexcept = default;

		/** @brief Destructor */
		~Codebooks() = default;

		//=====================================================================
		// Assignment operators
		//=====================================================================

		/** @brief Copy assignment operator */
		Codebooks& operator=( const Codebooks& ) = delete;

		/** @brief Move assignment operator */
		Codebooks& operator=( Codebooks&& ) noexcept = default;

		//=====================================================================
		// Operators
		//=====================================================================

		/**
		 * @brief Access a codebook by name using the array index operator.
		 * @details Provides direct access to a specific codebook.
		 * @param name The `CodebookName` enum value identifying the desired codebook.
		 * @return A const reference to the requested `Codebook`.
		 * @throws std::invalid_argument If the `name` enum value is invalid or out of the expected range.
		 */
		[[nodiscard]] const Codebook& operator[]( CodebookName name ) const;

		//=====================================================================
		// Accessors
		//=====================================================================

		/**
		 * @brief Get a codebook by name (alternative accessor).
		 * @details Provides direct access to a specific codebook. Functionally equivalent to `operator[]`.
		 * @param name The `CodebookName` enum value identifying the desired codebook.
		 * @return A const reference to the requested `Codebook`.
		 * @throws std::invalid_argument If the `name` enum value is invalid or out of the expected range.
		 * @see operator[]
		 */
		[[nodiscard]] const Codebook& codebook( CodebookName name ) const;

		/**
		 * @brief Get the VIS version associated with these codebooks.
		 * @return The `VisVersion` enum value.
		 */
		[[nodiscard]] VisVersion visVersion() const;

		//=====================================================================
		// Tag creation
		//=====================================================================

		/**
		 * @brief Try to create a metadata tag using the appropriate codebook for validation.
		 * @details Attempts to validate the `value` against the codebook specified by `name`.
		 *          If validation succeeds, a `MetadataTag` is created and returned.
		 * @param name The `CodebookName` identifying which codebook to use for validation.
		 * @param value The string value for the tag. Can be empty.
		 * @return An `std::optional<MetadataTag>` containing the created tag if the value is valid
		 *         (or if the codebook allows any value, like 'detail'), otherwise `std::nullopt`.
		 * @throws std::invalid_argument If the `name` enum value is invalid (propagated from `operator[]`).
		 */
		[[nodiscard]] std::optional<MetadataTag> tryCreateTag( CodebookName name, const std::string_view value ) const;

		/**
		 * @brief Create a metadata tag using the appropriate codebook, throwing on failure.
		 * @details Validates the `value` against the codebook specified by `name`.
		 *          If validation succeeds, a `MetadataTag` is created and returned.
		 *          If validation fails, an exception is thrown.
		 * @param name The `CodebookName` identifying which codebook to use for validation.
		 * @param value The string value for the tag. Cannot be empty if validation is required.
		 * @return The created `MetadataTag`.
		 * @throws std::invalid_argument If the `value` is invalid for the specified codebook,
		 *         or if the `name` enum value is invalid.
		 */
		[[nodiscard]] MetadataTag createTag( CodebookName name, const std::string& value ) const;

	private:
		//=====================================================================
		// Private member variables
		//=====================================================================

		/** @brief The VIS version these codebooks belong to. */
		VisVersion m_visVersion{};

		/** @brief Fixed-size array holding all codebooks, indexed by (CodebookName - 1). */
		std::array<Codebook, NUM_CODEBOOKS> m_codebooks{};
	};
}
