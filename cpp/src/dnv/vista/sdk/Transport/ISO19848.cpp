/**
 * @file ISO19848.cpp
 * @brief Implementation of ISO 19848 standard data access
 */

#include "dnv/vista/sdk/pch.h"

#include "dnv/vista/sdk/Transport/ISO19848.h"
#include "dnv/vista/sdk/EmbeddedResource.h"
#include "dnv/vista/sdk/Utils/StringUtils.h"
#include "dnv/vista/sdk/Utils/StringBuilderPool.h"
#include "dnv/vista/sdk/Config/ISO19848Constants.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// ISO19848 class
	//=====================================================================

	//----------------------------------------------
	// Singleton access
	//----------------------------------------------

	ISO19848& ISO19848::instance() noexcept
	{
		static ISO19848 s_instance;
		return s_instance;
	}

	//----------------------------------------------
	// Singleton construction
	//----------------------------------------------

	ISO19848::ISO19848() noexcept
		: m_dataChannelTypeNamesDtoCache{ internal::MemoryCacheOptions{ 10, std::chrono::hours( 1 ), std::chrono::hours( 1 ) } },
		  m_dataChannelTypeNamesCache{ internal::MemoryCacheOptions{ 10, std::chrono::hours( 1 ), std::chrono::hours( 1 ) } },
		  m_formatDataTypesDtoCache{ internal::MemoryCacheOptions{ 10, std::chrono::hours( 1 ), std::chrono::hours( 1 ) } },
		  m_formatDataTypesCache{ internal::MemoryCacheOptions{ 10, std::chrono::hours( 1 ), std::chrono::hours( 1 ) } }
	{
	}

	//----------------------------------------------
	// Public interface
	//----------------------------------------------

	DataChannelTypeNames ISO19848::dataChannelTypeNames( ISO19848Version version )
	{
		return m_dataChannelTypeNamesCache.getOrCreate(
			version,
			[this, version]() -> DataChannelTypeNames {
				const auto dto = dataChannelTypeNamesDto( version );

				std::vector<DataChannelTypeName> values;
				values.reserve( dto.values().size() );

				for ( const auto& x : dto.values() )
				{
					values.emplace_back( x.type(), x.description() );
				}

				return DataChannelTypeNames{ std::move( values ) };
			},
			[]( internal::CacheEntry& entry ) {
				entry.size = 1;
				entry.slidingExpiration = std::chrono::hours( 1 );
			} );
	}

	FormatDataTypes ISO19848::formatDataTypes( ISO19848Version version )
	{
		return m_formatDataTypesCache.getOrCreate(
			version,
			[this, version]() -> FormatDataTypes {
				const auto dto = formatDataTypesDto( version );

				std::vector<FormatDataType> values;
				values.reserve( dto.values().size() );

				for ( const auto& x : dto.values() )
				{
					values.emplace_back( x.type(), x.description() );
				}

				return FormatDataTypes{ std::move( values ) };
			},
			[]( internal::CacheEntry& entry ) {
				entry.size = 1;
				entry.slidingExpiration = std::chrono::hours( 1 );
			} );
	}

	//----------------------------------------------
	// DTO access
	//----------------------------------------------

	DataChannelTypeNamesDto ISO19848::dataChannelTypeNamesDto( ISO19848Version version )
	{
		return m_dataChannelTypeNamesDtoCache.getOrCreate(
			version,
			[version]() -> DataChannelTypeNamesDto {
				const auto dto = loadDataChannelTypeNamesDto( version );
				if ( !dto.has_value() )
				{
					throw std::runtime_error( "Invalid state: Failed to load DataChannelTypeNamesDto" );
				}

				return dto.value();
			},
			[]( internal::CacheEntry& entry ) {
				entry.size = 1;
				entry.slidingExpiration = std::chrono::hours( 1 );
			} );
	}

	FormatDataTypesDto ISO19848::formatDataTypesDto( ISO19848Version version )
	{
		return m_formatDataTypesDtoCache.getOrCreate(
			version,
			[version]() -> FormatDataTypesDto {
				const auto dto = loadFormatDataTypesDto( version );
				if ( !dto.has_value() )
				{
					throw std::runtime_error( "Invalid state: Failed to load FormatDataTypesDto" );
				}

				return dto.value();
			},
			[]( internal::CacheEntry& entry ) {
				entry.size = 1;
				entry.slidingExpiration = std::chrono::hours( 1 );
			} );
	}

	//----------------------------------------------
	// Loading
	//----------------------------------------------=

	std::optional<DataChannelTypeNamesDto> ISO19848::loadDataChannelTypeNamesDto( ISO19848Version version )
	{
		switch ( version )
		{
			case ISO19848Version::v2018:
			{
				return EmbeddedResource::dataChannelTypeNames( iso19848::ISO19848_VERSION_2018 );
			}
			case ISO19848Version::v2024:
			{
				return EmbeddedResource::dataChannelTypeNames( iso19848::ISO19848_VERSION_2024 );
			}
			default:
			{
				throw std::invalid_argument( "Invalid ISO19848Version" );
			}
		}
	}

	std::optional<FormatDataTypesDto> ISO19848::loadFormatDataTypesDto( ISO19848Version version )
	{
		switch ( version )
		{
			case ISO19848Version::v2018:
			{
				return EmbeddedResource::formatDataTypes( iso19848::ISO19848_VERSION_2018 );
			}
			case ISO19848Version::v2024:
			{
				return EmbeddedResource::formatDataTypes( iso19848::ISO19848_VERSION_2024 );
			}
			default:
			{
				throw std::invalid_argument( "Invalid ISO19848Version" );
			}
		}
	}

	//=====================================================================
	// DataChannelTypeNames class
	//=====================================================================

	//----------------------------------------------
	// DataChannelTypeNames::ParseResult  class
	//----------------------------------------------

	//-----------------------------
	// Parsing
	//-----------------------------

	DataChannelTypeNames::ParseResult DataChannelTypeNames::parse( std::string_view type ) const
	{
		const auto it = std::find_if( m_values.begin(), m_values.end(),
			[type]( const DataChannelTypeName& x ) { return x.type() == type; } );

		if ( it == m_values.end() )
		{
			return ParseResult{ ParseResult::Invalid{} };
		}

		return ParseResult{ ParseResult::Ok{ *it } };
	}

	//=====================================================================
	// FormatDataType class
	//=====================================================================

	//----------------------------------------------
	// Validation
	//----------------------------------------------

	FormatDataType::ValidateResult FormatDataType::validate( std::string_view value, Value& outValue ) const
	{
		outValue = Value{ Value::String{ value } };

		if ( m_type == iso19848::FORMAT_TYPE_DECIMAL )
		{
			datatypes::Decimal128 d;
			if ( !datatypes::Decimal128::tryParse( value, d ) )
			{
				auto lease = utils::StringBuilderPool::instance();
				auto builder = lease.builder();
				builder.append( "Invalid decimal value - Value='" );
				builder.append( value );
				builder.append( "'" );

				return ValidateResult{ ValidateResult::Invalid{ { lease.toString() } } };
			}

			outValue = Value{ Value::Decimal{ d } };
			return ValidateResult{ ValidateResult::Ok{} };
		}
		else if ( m_type == iso19848::FORMAT_TYPE_DOUBLE )
		{
			double d;
			if ( !utils::tryParseDouble( value, d ) )
			{
				auto lease = utils::StringBuilderPool::instance();
				auto builder = lease.builder();
				builder.append( "Invalid double value - Value='" );
				builder.append( value );
				builder.append( "'" );

				return ValidateResult{ ValidateResult::Invalid{ { lease.toString() } } };
			}

			outValue = Value{ Value::Double{ d } };
			return ValidateResult{ ValidateResult::Ok{} };
		}
		else if ( m_type == iso19848::FORMAT_TYPE_INTEGER )
		{
			int i;
			if ( !utils::tryParseInt( value, i ) )
			{
				auto lease = utils::StringBuilderPool::instance();
				auto builder = lease.builder();
				builder.append( "Invalid integer value - Value='" );
				builder.append( value );
				builder.append( "'" );

				return ValidateResult{ ValidateResult::Invalid{ { lease.toString() } } };
			}

			outValue = Value{ Value::Integer{ i } };
			return ValidateResult{ ValidateResult::Ok{} };
		}
		else if ( m_type == iso19848::FORMAT_TYPE_BOOLEAN )
		{
			bool b;
			if ( !utils::tryParseBool( value, b ) )
			{
				auto lease = utils::StringBuilderPool::instance();
				auto builder = lease.builder();
				builder.append( "Invalid boolean value - Value='" );
				builder.append( value );
				builder.append( "'" );

				return ValidateResult{ ValidateResult::Invalid{ { lease.toString() } } };
			}

			outValue = Value{ Value::Boolean{ b } };
			return ValidateResult{ ValidateResult::Ok{} };
		}
		else if ( m_type == iso19848::FORMAT_TYPE_CHAR )
		{
			if ( value.length() != 1 )
			{
				auto lease = utils::StringBuilderPool::instance();
				auto builder = lease.builder();
				builder.append( "Invalid char value - Value='" );
				builder.append( value );
				builder.append( "'" );

				return ValidateResult{ ValidateResult::Invalid{ { lease.toString() } } };
			}

			outValue = Value{ Value::Char{ value[0] } };
			return ValidateResult{ ValidateResult::Ok{} };
		}
		else if ( m_type == iso19848::FORMAT_TYPE_UNSIGNED_INTEGER )
		{
			if ( value.empty() )
			{
				auto lease = utils::StringBuilderPool::instance();
				auto builder = lease.builder();
				builder.append( "Invalid unsigned integer value - Value='" );
				builder.append( value );
				builder.append( "'" );

				return ValidateResult{ ValidateResult::Invalid{ { lease.toString() } } };
			}

			std::uint32_t ui;
			const auto parseResult = std::from_chars( value.data(), value.data() + value.size(), ui );
			if ( parseResult.ec != std::errc{} || parseResult.ptr != value.data() + value.size() )
			{
				auto lease = utils::StringBuilderPool::instance();
				auto builder = lease.builder();
				builder.append( "Invalid unsigned integer value - Value='" );
				builder.append( value );
				builder.append( "'" );

				return ValidateResult{ ValidateResult::Invalid{ { lease.toString() } } };
			}

			outValue = Value{ Value::UnsignedInteger{ ui } };
			return ValidateResult{ ValidateResult::Ok{} };
		}
		else if ( m_type == iso19848::FORMAT_TYPE_LONG )
		{
			if ( value.empty() )
			{
				auto lease = utils::StringBuilderPool::instance();
				auto builder = lease.builder();
				builder.append( "Invalid long value - Value='" );
				builder.append( value );
				builder.append( "'" );

				return ValidateResult{ ValidateResult::Invalid{ { lease.toString() } } };
			}

			std::int64_t l;
			const auto parseResult = std::from_chars( value.data(), value.data() + value.size(), l );
			if ( parseResult.ec != std::errc{} || parseResult.ptr != value.data() + value.size() )
			{
				auto lease = utils::StringBuilderPool::instance();
				auto builder = lease.builder();
				builder.append( "Invalid long value - Value='" );
				builder.append( value );
				builder.append( "'" );

				return ValidateResult{ ValidateResult::Invalid{ { lease.toString() } } };
			}

			outValue = Value{ Value::Long{ l } };
			return ValidateResult{ ValidateResult::Ok{} };
		}
		else if ( m_type == iso19848::FORMAT_TYPE_STRING )
		{
			return ValidateResult{ ValidateResult::Ok{} };
		}
		else if ( m_type == iso19848::FORMAT_TYPE_DATETIME )
		{
			/*
			 TODO: Implement a dedicated DateTime datatype class with proper ISO 8601 parsing,
			 timezone handling, and cross-platform compatibility.
			 Current implementation uses manual parsing since std::chrono::parse requires C++20
			 and is not available in all compilers. Future DateTime class should provide:
			 - Robust datetime parsing and validation
			 - Timezone awareness and conversion
			 - Multiple format support (ISO 8601, RFC 3339, etc.)
			 - Cross-platform compatibility without C++20 dependencies
			*/

			/* Basic datetime validation and parsing for YYYY-MM-DDTHH:MM:SSZ format */
			std::string dateTimeStr{ value };
			if ( dateTimeStr.length() != 20 ||
				 dateTimeStr[4] != '-' || dateTimeStr[7] != '-' ||
				 dateTimeStr[10] != 'T' || dateTimeStr[13] != ':' ||
				 dateTimeStr[16] != ':' || dateTimeStr[19] != 'Z' )
			{
				auto lease = utils::StringBuilderPool::instance();
				auto builder = lease.builder();
				builder.append( "Invalid datetime value - Value='" );
				builder.append( value );
				builder.append( "'" );

				return ValidateResult{ ValidateResult::Invalid{ { lease.toString() } } };
			}

			try
			{
				/* Parse the datetime string manually */
				int year = std::stoi( dateTimeStr.substr( 0, 4 ) );
				int month = std::stoi( dateTimeStr.substr( 5, 2 ) );
				int day = std::stoi( dateTimeStr.substr( 8, 2 ) );
				int hour = std::stoi( dateTimeStr.substr( 11, 2 ) );
				int minute = std::stoi( dateTimeStr.substr( 14, 2 ) );
				int second = std::stoi( dateTimeStr.substr( 17, 2 ) );

				/* Validate ranges */
				if ( year < 1970 || year > 3000 ||
					 month < 1 || month > 12 ||
					 day < 1 || day > 31 ||
					 hour < 0 || hour > 23 ||
					 minute < 0 || minute > 59 ||
					 second < 0 || second > 59 )
				{
					auto lease = utils::StringBuilderPool::instance();
					auto builder = lease.builder();
					builder.append( "Invalid datetime value - Value='" );
					builder.append( value );
					builder.append( "'" );

					return ValidateResult{ ValidateResult::Invalid{ { lease.toString() } } };
				}

				/* Convert to std::tm */
				std::tm tm = {};
				tm.tm_year = year - 1900;
				tm.tm_mon = month - 1;
				tm.tm_mday = day;
				tm.tm_hour = hour;
				tm.tm_min = minute;
				tm.tm_sec = second;

				/* Convert to time_point */
				auto time_t_val = std::mktime( &tm );
				auto tp = std::chrono::system_clock::from_time_t( time_t_val );

				outValue = Value{ Value::DateTime{ tp } };
				return ValidateResult{ ValidateResult::Ok{} };
			}
			catch ( const std::exception& )
			{
				auto lease = utils::StringBuilderPool::instance();
				auto builder = lease.builder();
				builder.append( "Invalid datetime value - Value='" );
				builder.append( value );
				builder.append( "'" );

				return ValidateResult{ ValidateResult::Invalid{ { lease.toString() } } };
			}
		}
		else
		{
			auto lease = utils::StringBuilderPool::instance();
			auto builder = lease.builder();

			builder.append( "Invalid format type " );
			builder.append( m_type );

			throw std::runtime_error( lease.toString() );
		}
	}

	//=====================================================================
	// FormatDataTypes class
	//=====================================================================

	//----------------------------------------------
	// FormatDataTypes::ParseResult  class
	//----------------------------------------------

	//-----------------------------
	// Parsing
	//-----------------------------

	FormatDataTypes::ParseResult FormatDataTypes::parse( std::string_view type ) const
	{
		const auto it = std::find_if( m_values.begin(), m_values.end(),
			[type]( const FormatDataType& x ) { return x.type() == type; } );

		if ( it == m_values.end() )
		{
			return ParseResult{ ParseResult::Invalid{} };
		}

		return ParseResult{ ParseResult::Ok{ *it } };
	}
}
