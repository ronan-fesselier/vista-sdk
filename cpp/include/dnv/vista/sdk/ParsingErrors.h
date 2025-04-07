#pragma once

namespace dnv::vista::sdk
{
	/**
	 * @brief Represents a collection of parsing errors.
	 *
	 * This class is used to store and manage errors encountered during parsing operations.
	 */
	class ParsingErrors
	{
	public:
		/**
		 * @brief Represents a single parsing error entry.
		 *
		 * Each error entry consists of a type (string) and a message (string).
		 */
		using ErrorEntry = std::tuple<std::string, std::string>;

		/**
		 * @brief A static instance representing an empty set of parsing errors.
		 */
		static const ParsingErrors Empty;

		/**
		 * @brief Constructs a ParsingErrors object.
		 * @param errors A vector of error entries (default is an empty vector).
		 */
		ParsingErrors( const std::vector<ErrorEntry>& errors = {} );

		/**
		 * @brief Checks if there are any errors.
		 * @return True if there are errors, false otherwise.
		 */
		bool HasErrors() const;

		/**
		 * @brief Checks if there is an error of a specific type.
		 * @param type The type of error to check for.
		 * @return True if an error of the specified type exists, false otherwise.
		 */
		bool HasErrorType( const std::string& type ) const;

		/**
		 * @brief Converts the parsing errors to a string representation.
		 * @return A string representation of the parsing errors.
		 */
		std::string ToString() const;

		/**
		 * @brief Equality operator.
		 * @param other The other ParsingErrors object to compare.
		 * @return True if the two ParsingErrors objects are equal, false otherwise.
		 */
		bool operator==( const ParsingErrors& other ) const;

		/**
		 * @brief Inequality operator.
		 * @param other The other ParsingErrors object to compare.
		 * @return True if the two ParsingErrors objects are not equal, false otherwise.
		 */
		bool operator!=( const ParsingErrors& other ) const;

		/**
		 * @brief Iterator for traversing parsing errors.
		 *
		 * Provides a forward iterator for iterating over the error entries in the ParsingErrors object.
		 */
		class Iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag; ///< Iterator category.
			using value_type = ErrorEntry;						 ///< Value type of the iterator.
			using difference_type = std::ptrdiff_t;				 ///< Difference type for the iterator.
			using pointer = const ErrorEntry*;					 ///< Pointer type for the iterator.
			using reference = const ErrorEntry&;				 ///< Reference type for the iterator.

			/**
			 * @brief Constructs an iterator.
			 * @param data Pointer to the vector of error entries.
			 * @param index The starting index for the iterator.
			 */
			Iterator( const std::vector<ErrorEntry>* data, size_t index );

			/**
			 * @brief Dereference operator.
			 * @return A reference to the current error entry.
			 */
			reference operator*() const;

			/**
			 * @brief Member access operator.
			 * @return A pointer to the current error entry.
			 */
			pointer operator->() const;

			/**
			 * @brief Pre-increment operator.
			 * @return A reference to the incremented iterator.
			 */
			Iterator& operator++();

			/**
			 * @brief Post-increment operator.
			 * @return A copy of the iterator before incrementing.
			 */
			Iterator operator++( int );

			/**
			 * @brief Equality operator.
			 * @param other The other iterator to compare.
			 * @return True if the iterators are equal, false otherwise.
			 */
			bool operator==( const Iterator& other ) const;

			/**
			 * @brief Inequality operator.
			 * @param other The other iterator to compare.
			 * @return True if the iterators are not equal, false otherwise.
			 */
			bool operator!=( const Iterator& other ) const;

		private:
			const std::vector<ErrorEntry>* _data; ///< Pointer to the vector of error entries.
			size_t _index;						  ///< Current index of the iterator.
		};

		/**
		 * @brief Gets an iterator to the beginning of the error entries.
		 * @return An iterator to the first error entry.
		 */
		Iterator begin() const;

		/**
		 * @brief Gets an iterator to the end of the error entries.
		 * @return An iterator past the last error entry.
		 */
		Iterator end() const;

	private:
		std::vector<ErrorEntry> _errors; ///< Vector of error entries.
	};
}
