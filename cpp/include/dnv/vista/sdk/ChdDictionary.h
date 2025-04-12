#pragma once

namespace dnv::vista::sdk
{
	/**
	 * @brief Internal utilities for the ChdDictionary class
	 */
	namespace internal
	{
		/**
		 * @brief Helper class for throwing standardized exceptions
		 */
		class ThrowHelper
		{
		public:
			/**
			 * @brief Throw an exception for key not found
			 * @param key The key that was not found
			 * @throws std::out_of_range Always
			 */
			[[noreturn]] static void ThrowKeyNotFoundException( std::string_view key );

			/**
			 * @brief Throw an exception for invalid operation
			 * @throws std::invalid_argument Always
			 */
			[[noreturn]] static void ThrowInvalidOperationException();
		};

		/**
		 * @brief Hash function utilities for the CHD algorithm
		 */
		class Hashing
		{
		public:
			/**
			 * @brief FNV-1a hash function
			 * @param hash The current hash value
			 * @param ch The character to hash
			 * @return The updated hash value
			 */
			static inline uint32_t Fnv( uint32_t hash, uint8_t ch )
			{
				return ( ch ^ hash ) * 0x01000193;
			}

			/**
			 * @brief Seed mixing function for CHD algorithm
			 * @param seed The seed value
			 * @param hash The hash value
			 * @param size The table size
			 * @return The final table index
			 */
			static inline uint32_t Seed( uint32_t seed, uint32_t hash, uint64_t size )
			{
				uint32_t x = seed + hash;
				x ^= x >> 12;
				x ^= x << 25;
				x ^= x >> 27;

				return static_cast<uint32_t>( ( static_cast<uint64_t>( x ) * 0x2545F4914F6CDD1DUL ) & ( size - 1 ) );
			}
		};
	}

	/**
	 * @brief Perfect hashing dictionary using CHD (Compress, Hash, Displace) algorithm
	 *
	 * ChdDictionary provides O(1) lookup time with minimal memory overhead for read-only
	 * dictionaries. It uses a two-level perfect hashing scheme that guarantees no collisions.
	 * This implementation exactly matches the C# version's behavior.
	 *
	 * @tparam TValue The type of values stored in the dictionary
	 */
	template <typename TValue>
	class ChdDictionary
	{
	private:
		/** @brief Table of key-value pairs */
		std::vector<std::pair<std::string, TValue>> m_table;

		/** @brief Seeds for hash functions */
		std::vector<int> m_seeds;

	public:
		/**
		 * @brief Default constructor
		 */
		ChdDictionary() = default;

		/**
		 * @brief Construct from a collection of key-value pairs
		 * @param items The items to store in the dictionary
		 */
		explicit ChdDictionary( const std::vector<std::pair<std::string, TValue>>& items );

		/**
		 * @brief Access value by key
		 * @param key The key to look up
		 * @return Reference to the value
		 * @throws std::out_of_range If the key is not found
		 */
		TValue& operator[]( std::string_view key );

		/**
		 * @brief Access value by key (const version)
		 * @param key The key to look up
		 * @return Const reference to the value
		 * @throws std::out_of_range If the key is not found
		 */
		const TValue& operator[]( std::string_view key ) const;

		/**
		 * @brief Try to get a value by key
		 * @param key The key to look up
		 * @param[out] value Pointer to store the value if found
		 * @return True if the key was found via perfect hash match, false otherwise
		 */
		bool TryGetValue( std::string_view key, TValue* value ) const;

		/**
		 * @brief Iterator for ChdDictionary key-value pairs
		 */
		class Iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = std::pair<std::string, TValue>;
			using difference_type = std::ptrdiff_t;
			using pointer = const value_type*;
			using reference = const value_type&;

			/**
			 * @brief Default constructor
			 */
			Iterator() = default;

			/**
			 * @brief Construct with table reference and position
			 * @param table Pointer to the key-value table
			 * @param index Starting index in the table
			 */
			explicit Iterator( const std::vector<std::pair<std::string, TValue>>* table, int index );

			/**
			 * @brief Dereference operator
			 * @return Reference to the current key-value pair
			 * @throws std::invalid_argument If iterator is out of range
			 */
			reference operator*() const;

			/**
			 * @brief Arrow operator
			 * @return Pointer to the current key-value pair
			 * @throws std::invalid_argument If iterator is out of range
			 */
			pointer operator->() const;

			/**
			 * @brief Pre-increment operator
			 * @return Reference to this iterator after increment
			 */
			Iterator& operator++();

			/**
			 * @brief Post-increment operator
			 * @return Copy of iterator before increment
			 */
			Iterator operator++( int );

			/**
			 * @brief Equality comparison
			 * @param other Iterator to compare with
			 * @return True if iterators are equal
			 */
			bool operator==( const Iterator& other ) const;

			/**
			 * @brief Inequality comparison
			 * @param other Iterator to compare with
			 * @return True if iterators are not equal
			 */
			bool operator!=( const Iterator& other ) const;

			/**
			 * @brief Reset the iterator to its initial state
			 */
			void Reset();

		private:
			/** @brief Pointer to the key-value table */
			const std::vector<std::pair<std::string, TValue>>* m_table;

			/** @brief Current position in the table */
			int m_index;

			/** @brief Cached current key-value pair */
			mutable std::pair<std::string, TValue> m_current;
		};

		/**
		 * @brief Get iterator to the beginning
		 * @return Iterator to the first element
		 */
		Iterator begin() const;

		/**
		 * @brief Get iterator to the end
		 * @return Iterator past the last element
		 */
		Iterator end() const;

		/**
		 * @brief Alias for Iterator
		 */
		using Enumerator = Iterator;

		/**
		 * @brief Get enumerator for iteration
		 * @return Iterator to the beginning
		 */
		Enumerator GetEnumerator() const;

	private:
		/**
		 * @brief Hash a key to a 32-bit integer
		 * @param key The key to hash
		 * @return The hash value
		 */
		static uint32_t Hash( std::string_view key );
	};
}
