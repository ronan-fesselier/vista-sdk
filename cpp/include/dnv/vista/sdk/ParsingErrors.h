/**
 * @file ParsingErrors.h
 * @brief Defines the ParsingErrors class for managing parsing error collections.
 */

#pragma once

namespace dnv::vista::sdk
{
	//=====================================================================
	// ParsingErrors class
	//=====================================================================

	/**
	 * @brief Represents a collection of parsing errors.
	 *
	 * This class is used to store and manage errors encountered during parsing operations.
	 */
	class ParsingErrors final
	{
	public:
		//----------------------------------------------
		// Types definitions
		//----------------------------------------------

		/** @brief Represents a single parsing error entry. */
		using ErrorEntry = std::pair<std::string, std::string>;

		//----------------------------------------------
		// Static members
		//----------------------------------------------

		/** @brief A static instance representing an empty set of parsing errors. */
		static const ParsingErrors Empty;

	private:
		//----------------------------------------------
		// Forward declarations
		//----------------------------------------------

		class Enumerator;

		//----------------------------------------------
		// Friends access
		//----------------------------------------------

		friend class LocalIdParsingErrorBuilder;
		friend class LocationParsingErrorBuilder;

		//----------------------------------------------
		// Construction / destruction
		//----------------------------------------------

		/**
		 * @brief Internal constructor for creating ParsingErrors with error entries.
		 * @param errors A vector of error entries.
		 */
		ParsingErrors( const std::vector<ErrorEntry>& errors );

	public:
		/** @brief Default constructor */
		ParsingErrors();

		/** @brief Copy constructor */
		ParsingErrors( const ParsingErrors& ) = default;

		/** @brief Move constructor */
		ParsingErrors( ParsingErrors&& ) noexcept = default;

		/** @brief Destructor */
		~ParsingErrors() = default;

		//----------------------------------------------
		// Assignment operators
		//----------------------------------------------

		/** @brief Copy assignment operator */
		ParsingErrors& operator=( const ParsingErrors& ) = default;

		/** @brief Move assignment operator */
		ParsingErrors& operator=( ParsingErrors&& ) noexcept = default;

		//----------------------------------------------
		// Operators
		//----------------------------------------------

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

		//----------------------------------------------
		// Public methods
		//----------------------------------------------

		/**
		 * @brief Checks if there are any errors.
		 * @return True if there are errors, false otherwise.
		 */
		bool hasErrors() const;

		/**
		 * @brief Checks if there is an error of a specific type.
		 * @param type The type of error to check for.
		 * @return True if an error of the specified type exists, false otherwise.
		 */
		bool hasErrorType( const std::string& type ) const;

		/**
		 * @brief Checks if this ParsingErrors object is equal to another.
		 * @param other The other ParsingErrors object to compare.
		 * @return True if the two ParsingErrors objects are equal, false otherwise.
		 */
		bool equals( const ParsingErrors& other ) const;

		/**
		 * @brief Checks if this ParsingErrors object is equal to another object.
		 * @param obj The other object to compare (as void pointer).
		 * @return True if the objects are equal, false otherwise.
		 */
		bool equals( const void* obj ) const;

		/**
		 * @brief Gets the number of error entries.
		 * @return The count of error entries.
		 */
		size_t count() const;

		/**
		 * @brief Gets the hash code for this ParsingErrors object.
		 * @return The hash code (can be negative).
		 */
		[[nodiscard]] size_t hashCode() const noexcept;

		/**
		 * @brief Converts the parsing errors to a string representation.
		 * @return A string representation of the parsing errors.
		 */
		std::string toString() const;

		//----------------------------
		// Enumeration
		//----------------------------

		/**
		 * @brief Gets an enumerator for the error entries.
		 * @return An enumerator for iterating through the errors.
		 */
		Enumerator enumerator() const;

	private:
		//----------------------------------------------
		// Private member variables
		//----------------------------------------------

		std::vector<ErrorEntry> m_errors;

	private:
		//----------------------------------------------
		// ParsingErrors enumerator
		//----------------------------------------------

		/**
		 * @brief Enumerator for traversing parsing errors.
		 *
		 * Provides enumeration functionality for iterating through error entries.
		 * The enumerator starts positioned before the first element.
		 */
		class Enumerator final
		{
		public:
			//----------------------------
			// Construction / destruction
			//----------------------------

			/**
			 * @brief Constructs an enumerator for the given error data.
			 * @param data Pointer to the vector of error entries to enumerate.
			 */
			Enumerator( const std::vector<ErrorEntry>* data );

			/** @brief Default constructor */
			Enumerator() = delete;

			/** @brief Copy constructor */
			Enumerator( const Enumerator& ) = default;

			/** @brief Move constructor */
			Enumerator( Enumerator&& ) noexcept = default;

			/** @brief Destructor */
			~Enumerator() = default;

			//----------------------------
			// Assignment operators
			//----------------------------

			/** @brief Copy assignment operator */
			Enumerator& operator=( const Enumerator& ) = default;

			/** @brief Move assignment operator */
			Enumerator& operator=( Enumerator&& ) noexcept = default;

			//----------------------------
			// Enumeration methods
			//----------------------------

			/**
			 * @brief Advances the enumerator to the next element.
			 * @return True if the enumerator successfully moved to the next element;
			 *         false if the enumerator has passed the end of the collection.
			 */
			bool next();

			/**
			 * @brief Gets the current element.
			 * @return The current error entry.
			 */
			const ErrorEntry& current() const;

			/**
			 * @brief Resets the enumerator to its initial position.
			 */
			void reset();

		private:
			//----------------------------
			// Private member variables
			//----------------------------

			const std::vector<ErrorEntry>* m_data;
			size_t m_index;
			ErrorEntry m_current;
		};
	};
}
