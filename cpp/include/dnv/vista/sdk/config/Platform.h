/**
 * @file Platform.h
 * @brief Cross-platform compiler support and hardware optimization macros
 * @details Provides SIMD intrinsics, force inline directives, and compiler-specific
 *          feature detection for maximum performance across MSVC, GCC, and Clang
 */

#pragma once

//=====================================================================
// Cross-platform compiler support
//=====================================================================

//----------------------------------------------
// Compiler intrinsics headers
//----------------------------------------------

/** @brief Platform-specific intrinsics headers for SIMD and hardware optimization support */
#ifdef _MSC_VER
#	include <intrin.h>
#elif defined( __GNUC__ ) || defined( __clang__ )
#	include <immintrin.h>
#	include <x86intrin.h>
#endif

//----------------------------------------------
// Cross-compiler performance macros
//----------------------------------------------

/** @brief Cross-compiler force inline directive for performance-critical functions */
#if defined( _MSC_VER )
#	define VISTA_SDK_CPP_FORCE_INLINE __forceinline
#elif defined( __GNUC__ ) || defined( __clang__ )
#	define VISTA_SDK_CPP_FORCE_INLINE __attribute__( ( always_inline ) ) inline
#else
#	define VISTA_SDK_CPP_FORCE_INLINE inline
#endif

//----------------------------------------------
// Compiler-specific C++20 feature support
//----------------------------------------------

/** @brief Conditional constexpr support for GCC 11.x which has incomplete constexpr std::string support */
#if defined( __GNUC__ ) && __GNUC__ >= 11 && __GNUC__ < 12
#	define VISTA_SDK_CPP_CONDITIONAL_CONSTEXPR
#else
#	define VISTA_SDK_CPP_CONDITIONAL_CONSTEXPR constexpr
#endif

/** @brief No unique address attribute for zero-cost empty member optimization */
#if defined( _MSC_VER ) && _MSC_VER >= 1928
#	define VISTA_SDK_CPP_NO_UNIQUE_ADDRESS [[no_unique_address]]
#elif defined( __has_cpp_attribute ) && __has_cpp_attribute( no_unique_address ) >= 201803L
#	define VISTA_SDK_CPP_NO_UNIQUE_ADDRESS [[no_unique_address]]
#	if __cpp_lib_no_unique_address >= 201803L
#		define VISTA_SDK_CPP_NO_UNIQUE_ADDRESS [[no_unique_address]]
#	else
#		define VISTA_SDK_CPP_NO_UNIQUE_ADDRESS
#	endif
#else
#	define VISTA_SDK_CPP_NO_UNIQUE_ADDRESS
#endif
