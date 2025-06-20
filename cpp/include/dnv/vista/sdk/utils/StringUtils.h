/**
 * @file StringUtils.h
 * @brief String utilities and heterogeneous lookup support for Vista SDK
 * @details Contains performance-critical string utilities including
 *          heterogeneous lookup functors for zero-copy string_view operations.
 */

#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <unordered_map>
#include <unordered_set>

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

		[[nodiscard]] inline size_t operator()( std::string_view sv ) const noexcept
		{
			return std::hash<std::string_view>{}( sv );
		}

		[[nodiscard]] inline size_t operator()( const std::string& s ) const noexcept
		{
			return std::hash<std::string_view>{}( s );
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

		[[nodiscard]] inline bool operator()( const std::string& lhs, const std::string& rhs ) const noexcept
		{
			return lhs == rhs;
		}

		[[nodiscard]] inline bool operator()( const std::string& lhs, std::string_view rhs ) const noexcept
		{
			return lhs == rhs;
		}

		[[nodiscard]] inline bool operator()( std::string_view lhs, const std::string& rhs ) const noexcept
		{
			return lhs == rhs;
		}

		[[nodiscard]] inline bool operator()( std::string_view lhs, std::string_view rhs ) const noexcept
		{
			return lhs == rhs;
		}
	};

	//=====================================================================
	// Type aliases for heterogeneous containers
	//=====================================================================

	/**
	 * @brief Unordered map with heterogeneous lookup support
	 * @tparam T Value type
	 */
	template <typename T>
	using StringMap = std::unordered_map<std::string, T, StringViewHash, StringViewEqual>;

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
	[[nodiscard]] inline bool endsWith( std::string_view str, std::string_view suffix ) noexcept
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
	[[nodiscard]] inline bool startsWith( std::string_view str, std::string_view prefix ) noexcept
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
	[[nodiscard]] inline bool contains( std::string_view str, std::string_view substr ) noexcept
	{
		return str.find( substr ) != std::string_view::npos;
	}

	/**
	 * @brief Fast case-sensitive string comparison
	 * @param lhs First string
	 * @param rhs Second string
	 * @return True if strings are exactly equal
	 */
	[[nodiscard]] inline bool equals( std::string_view lhs, std::string_view rhs ) noexcept
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
			[]( char a, char b ) {
				return std::tolower( static_cast<unsigned char>( a ) ) ==
					   std::tolower( static_cast<unsigned char>( b ) );
			} );
	}
}
