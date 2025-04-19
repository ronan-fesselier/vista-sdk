#pragma once

namespace dnv::vista::sdk
{
	/**
	 * @brief Internal utilities for the ChdDictionary class
	 */
	namespace internal
	{
		/**
		 * @brief Check if the CPU supports SSE4.2 instructions
		 * @return true if SSE4.2 is supported, false otherwise
		 */
		bool hasSSE42Support();

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
			[[noreturn]] static void throwKeyNotFoundException( std::string_view key );

			/**
			 * @brief Throw an exception for invalid operation
			 * @throws std::invalid_argument Always
			 */
			[[noreturn]] static void throwInvalidOperationException();
		};

		/**
		 * @brief Hash function utilities for the CHD algorithm
		 */
		class Hashing
		{
		public:
			/**
			 * @brief FNV-1a / Fowler–Noll–Vo hash function (https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function)
			 * @param hash The current hash value
			 * @param ch The character to hash
			 * @return The updated hash value
			 */
			static uint32_t Fnv1a( uint32_t hash, uint8_t ch );

			/**
			 * @brief Seed mixing function for CHD algorithm
			 * @param seed The seed value
			 * @param hash The hash value
			 * @param size The table size
			 * @return The final table index
			 */
			static uint32_t seed( uint32_t seed, uint32_t hash, uint64_t size );
		};
	}

	/**
	 * @brief Perfect hashing dictionary using CHD (Compress, Hash, Displace) algorithm
	 *
	 * ChdDictionary provides O(1) lookup time with minimal memory overhead for read-only
	 * dictionaries. It uses a two-level perfect hashing scheme that guarantees no collisions.
	 *
	 * @tparam TValue The type of values stored in the dictionary
	 */
	template <typename TValue>
	class ChdDictionary
	{
	public:
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

			reference operator*() const;
			pointer operator->() const;
			Iterator& operator++();
			Iterator operator++( int );
			bool operator==( const Iterator& other ) const;
			bool operator!=( const Iterator& other ) const;

			void reset();

		private:
			/** @brief Pointer to the key-value table */
			const std::vector<std::pair<std::string, TValue>>* m_table = nullptr;

			/** @brief Current position in the table */
			int m_index = 0;

			/** @brief Cached current key-value pair */
			mutable std::pair<std::string, TValue> m_current;
		};

		using Enumerator = Iterator;

		ChdDictionary() = default;
		explicit ChdDictionary( const std::vector<std::pair<std::string, TValue>>& items );
		ChdDictionary( const ChdDictionary& other );
		ChdDictionary( ChdDictionary&& other ) noexcept;
		ChdDictionary& operator=( const ChdDictionary& other );
		ChdDictionary& operator=( ChdDictionary&& other ) noexcept;

		TValue& operator[]( std::string_view key );
		const TValue& operator[]( std::string_view key ) const;
		bool tryGetValue( std::string_view key, TValue* value ) const;

		bool isEmpty() const;

		Iterator begin() const;
		Iterator end() const;
		Enumerator enumerator() const;

	private:
		/**
		 * @brief Hash a key to a 32-bit integer
		 * @param key The key to hash
		 * @return The hash value
		 */
		static uint32_t hash( std::string_view key );

		/** @brief Table of key-value pairs */
		std::vector<std::pair<std::string, TValue>> m_table;

		/** @brief Seeds for hash functions */
		std::vector<int> m_seeds;
	};
}

#include "ChdDictionary.hpp"
