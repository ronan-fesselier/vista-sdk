/**
 * @file StringUtils.h
 * @brief String utilities and heterogeneous lookup support for Vista SDK
 * @details Contains performance-critical string utilities including
 *          heterogeneous lookup functors for zero-copy string_view operations.
 */

#pragma once

#include "dnv/vista/sdk/Config.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// Heterogeneous lookup functors for zero-copy string operations
	//=====================================================================

	/**
	 * @brief Hash functor supporting both std::string and std::string_view
	 * @details Enables heterogeneous lookup in unordered containers,
	 *          allowing direct string_view lookups without string construction.
	 */
	struct StringViewHash
	{
		using is_transparent = void;

		static constexpr std::hash<std::string_view> s_hasher{};

		[[nodiscard]] inline size_t operator()( std::string_view sv ) const noexcept
		{
			return s_hasher( sv );
		}

		[[nodiscard]] inline size_t operator()( const std::string& s ) const noexcept
		{
			return s_hasher( std::string_view{ s.data(), s.size() } );
		}

		[[nodiscard]] inline size_t operator()( const char* s ) const noexcept
		{
			return s_hasher( std::string_view{ s } );
		}
	};

	/**
	 * @brief Equality functor supporting both std::string and std::string_view
	 * @details Enables heterogeneous lookup in unordered containers,
	 *          providing all comparison overloads for string types.
	 */
	struct StringViewEqual
	{
		using is_transparent = void;

		[[nodiscard]] VISTA_SDK_CPP_CONDITIONAL_CONSTEXPR inline bool operator()( const std::string& lhs, const std::string& rhs ) const noexcept
		{
			return lhs.size() == rhs.size() && lhs == rhs;
		}

		[[nodiscard]] VISTA_SDK_CPP_CONDITIONAL_CONSTEXPR inline bool operator()( const std::string& lhs, std::string_view rhs ) const noexcept
		{
			return lhs.size() == rhs.size() && lhs == rhs;
		}

		[[nodiscard]] VISTA_SDK_CPP_CONDITIONAL_CONSTEXPR inline bool operator()( std::string_view lhs, const std::string& rhs ) const noexcept
		{
			return lhs.size() == rhs.size() && lhs == rhs;
		}

		[[nodiscard]] constexpr inline bool operator()( std::string_view lhs, std::string_view rhs ) const noexcept
		{
			return lhs.size() == rhs.size() && lhs == rhs;
		}

		[[nodiscard]] VISTA_SDK_CPP_CONDITIONAL_CONSTEXPR inline bool operator()( const char* lhs, const std::string& rhs ) const noexcept
		{
			std::string_view lhs_view{ lhs };
			return lhs_view.size() == rhs.size() && lhs_view == rhs;
		}

		[[nodiscard]] VISTA_SDK_CPP_CONDITIONAL_CONSTEXPR inline bool operator()( const std::string& lhs, const char* rhs ) const noexcept
		{
			std::string_view rhs_view{ rhs };
			return lhs.size() == rhs_view.size() && lhs == rhs_view;
		}

		[[nodiscard]] constexpr inline bool operator()( const char* lhs, std::string_view rhs ) const noexcept
		{
			std::string_view lhs_view{ lhs };
			return lhs_view.size() == rhs.size() && lhs_view == rhs;
		}

		[[nodiscard]] constexpr inline bool operator()( std::string_view lhs, const char* rhs ) const noexcept
		{
			std::string_view rhs_view{ rhs };
			return lhs.size() == rhs_view.size() && lhs == rhs_view;
		}

		[[nodiscard]] constexpr inline bool operator()( const char* lhs, const char* rhs ) const noexcept
		{
			std::string_view lhs_view{ lhs };
			std::string_view rhs_view{ rhs };
			return lhs_view.size() == rhs_view.size() && lhs_view == rhs_view;
		}
	};

	//=====================================================================
	// StringMap class
	//=====================================================================

	/**
	 * @brief Enhanced unordered map with full heterogeneous support
	 * @tparam T Value type
	 */
	template <typename T>
	class StringMap : public std::unordered_map<std::string, T, StringViewHash, StringViewEqual>
	{
		using Base = std::unordered_map<std::string, T, StringViewHash, StringViewEqual>;

	public:
		using Base::Base;
		using Base::operator[];
		using Base::try_emplace;

		/**
		 * @brief Heterogeneous operator[] for string_view
		 * @param key String view key
		 * @return Reference to the mapped value
		 */
		inline T& operator[]( std::string_view key )
		{
			return this->try_emplace( std::string{ key }, T{} ).first->second;
		}

		/**
		 * @brief Heterogeneous operator[] for const char*
		 * @param key C-string key
		 * @return Reference to the mapped value
		 */
		inline T& operator[]( const char* key )
		{
			return this->try_emplace( std::string{ key }, T{} ).first->second;
		}

		/**
		 * @brief Heterogeneous try_emplace for string_view
		 * @param key String view key
		 * @param args Arguments to construct the value
		 * @return Pair of iterator and bool indicating insertion
		 */
		template <typename... Args>
		inline std::pair<typename Base::iterator, bool> try_emplace( std::string_view key, Args&&... args )
		{
			return Base::try_emplace( std::string{ key }, std::forward<Args>( args )... );
		}

		/**
		 * @brief Heterogeneous try_emplace for const char*
		 * @param key C-string key
		 * @param args Arguments to construct the value
		 * @return Pair of iterator and bool indicating insertion
		 */
		template <typename... Args>
		inline std::pair<typename Base::iterator, bool> try_emplace( const char* key, Args&&... args )
		{
			return Base::try_emplace( std::string{ key }, std::forward<Args>( args )... );
		}
	};

	//=====================================================================
	// Type aliases for heterogeneous containers
	//=====================================================================

	/**
	 * @brief Unordered set with heterogeneous lookup support
	 */
	using StringSet = std::unordered_set<std::string, StringViewHash, StringViewEqual>;

	//=====================================================================
	// Performance-critical string utilities
	//=====================================================================

	/**
	 * @brief Fast check if string ends with suffix
	 * @param str String to check
	 * @param suffix Suffix to find
	 * @return True if str ends with suffix
	 */
	[[nodiscard]] constexpr inline bool endsWith( std::string_view str, std::string_view suffix ) noexcept
	{
		return str.size() >= suffix.size() &&
			   str.compare( str.size() - suffix.size(), suffix.size(), suffix ) == 0;
	}

	/**
	 * @brief Fast check if string starts with prefix
	 * @param str String to check
	 * @param prefix Prefix to find
	 * @return True if str starts with prefix
	 */
	[[nodiscard]] constexpr inline bool startsWith( std::string_view str, std::string_view prefix ) noexcept
	{
		return str.size() >= prefix.size() &&
			   str.compare( 0, prefix.size(), prefix ) == 0;
	}

	/**
	 * @brief Fast check if string contains substring
	 * @param str String to check
	 * @param substr Substring to find
	 * @return True if str contains substr
	 */
	[[nodiscard]] constexpr inline bool contains( std::string_view str, std::string_view substr ) noexcept
	{
		return str.find( substr ) != std::string_view::npos;
	}

	/**
	 * @brief Fast case-sensitive string comparison
	 * @param lhs First string
	 * @param rhs Second string
	 * @return True if strings are exactly equal
	 */
	[[nodiscard]] constexpr inline bool equals( std::string_view lhs, std::string_view rhs ) noexcept
	{
		return lhs == rhs;
	}

	/**
	 * @brief Fast case-insensitive string comparison
	 * @param lhs First string
	 * @param rhs Second string
	 * @return True if strings are equal (case-insensitive)
	 */
	[[nodiscard]] inline bool iequals( std::string_view lhs, std::string_view rhs ) noexcept
	{
		if ( lhs.size() != rhs.size() )
		{
			return false;
		}

		return std::equal( lhs.begin(), lhs.end(), rhs.begin(),
			[]( char a, char b ) noexcept {
				return std::tolower( static_cast<unsigned char>( a ) ) ==
					   std::tolower( static_cast<unsigned char>( b ) );
			} );
	}
}
