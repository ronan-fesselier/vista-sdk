/**
 * @file GmodVersioningDto.cpp
 * @brief Implementation of GMOD versioning data transfer objects
 */

#include "pch.h"

#include "dnv/vista/sdk/GmodVersioningDto.h"

#include "dnv/vista/sdk/config/DtoKeys.h"

namespace dnv::vista::sdk
{
	namespace GmodVersioningDtoInternal
	{
		//=====================================================================
		// JSON parsing helper functions
		//=====================================================================

		std::string_view extractVisHint( const nlohmann::json& json ) noexcept
		{
			try
			{
				if ( json.contains( dto::GMODVERSIONING_DTO_KEY_VIS_RELEASE ) && json.at( dto::GMODVERSIONING_DTO_KEY_VIS_RELEASE ).is_string() )
				{
					const auto& str = json.at( dto::GMODVERSIONING_DTO_KEY_VIS_RELEASE ).get_ref<const std::string&>();

					return std::string_view{ str };
				}

				return dto::GMODVERSIONING_DTO_UNKNOWN_VERSION;
			}
			catch ( ... )
			{
				return dto::GMODVERSIONING_DTO_UNKNOWN_VERSION;
			}
		}

		std::string_view extractOldAssignmentHint( const nlohmann::json& json ) noexcept
		{
			try
			{
				if ( json.contains( dto::GMODVERSIONING_DTO_KEY_OLD_ASSIGNMENT ) && json.at( dto::GMODVERSIONING_DTO_KEY_OLD_ASSIGNMENT ).is_string() )
				{
					const auto& str = json.at( dto::GMODVERSIONING_DTO_KEY_OLD_ASSIGNMENT ).get_ref<const std::string&>();

					return std::string_view{ str };
				}

				return dto::GMODVERSIONING_DTO_UNKNOWN_OLD_ASSIGNMENT;
			}
			catch ( ... )
			{
				return dto::GMODVERSIONING_DTO_UNKNOWN_OLD_ASSIGNMENT;
			}
		}

		std::string_view extractSourceHint( const nlohmann::json& json ) noexcept
		{
			try
			{
				if ( json.contains( dto::GMODVERSIONING_DTO_KEY_SOURCE ) && json.at( dto::GMODVERSIONING_DTO_KEY_SOURCE ).is_string() )
				{
					const auto& str = json.at( dto::GMODVERSIONING_DTO_KEY_SOURCE ).get_ref<const std::string&>();

					return std::string_view{ str };
				}

				return dto::GMODVERSIONING_DTO_UNKNOWN_SOURCE;
			}
			catch ( ... )
			{
				return dto::GMODVERSIONING_DTO_UNKNOWN_SOURCE;
			}
		}
	}

	//=====================================================================
	// GMOD versioning assignement data transfer objects
	//=====================================================================

	//----------------------------------------------
	// Serialization
	//----------------------------------------------

	std::optional<GmodVersioningAssignmentChangeDto> GmodVersioningAssignmentChangeDto::tryFromJson( const nlohmann::json& json )
	{
		[[maybe_unused]] const auto oldAssignmentHint = GmodVersioningDtoInternal::extractOldAssignmentHint( json );

		try
		{
			if ( !json.is_object() )
			{
				fmt::print( stderr, "ERROR: JSON value for GmodVersioningAssignmentChangeDto is not an object\n" );

				return std::nullopt;
			}

			GmodVersioningAssignmentChangeDto dto = json.get<GmodVersioningAssignmentChangeDto>();

			return std::optional<GmodVersioningAssignmentChangeDto>{ std::move( dto ) };
		}
		catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
		{
			fmt::print( stderr, "ERROR: JSON exception during GmodVersioningAssignmentChangeDto parsing (hint: oldAssignment='{}'): {}\n",
				oldAssignmentHint, ex.what() );

			return std::nullopt;
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			fmt::print( stderr, "ERROR: Standard exception during GmodVersioningAssignmentChangeDto parsing (hint: oldAssignment='{}'): {}\n",
				oldAssignmentHint, ex.what() );

			return std::nullopt;
		}
	}

	GmodVersioningAssignmentChangeDto GmodVersioningAssignmentChangeDto::fromJson( const nlohmann::json& json )
	{
		const auto oldAssignmentHint = GmodVersioningDtoInternal::extractOldAssignmentHint( json );
		auto dtoOpt = GmodVersioningAssignmentChangeDto::tryFromJson( json );
		if ( !dtoOpt.has_value() )
		{
			throw std::invalid_argument(
				fmt::format(
					"Failed to deserialize GmodVersioningAssignmentChangeDto from JSON (hint: oldAssignment='{}')",
					oldAssignmentHint ) );
		}

		return dtoOpt.value();
	}

	nlohmann::json GmodVersioningAssignmentChangeDto::toJson() const
	{
		nlohmann::json result;
		to_json( result, *this );

		return result;
	}

	//----------------------------------------------
	// Private serialization methods
	//----------------------------------------------

	void from_json( const nlohmann::json& j, GmodVersioningAssignmentChangeDto& dto )
	{
		if ( !j.contains( dto::GMODVERSIONING_DTO_KEY_OLD_ASSIGNMENT ) || !j.at( dto::GMODVERSIONING_DTO_KEY_OLD_ASSIGNMENT ).is_string() )
		{
			throw nlohmann::json::parse_error::create(
				101, 0u,
				fmt::format(
					"GmodVersioningAssignmentChangeDto JSON missing required '{}' field or not a string",
					dto::GMODVERSIONING_DTO_KEY_OLD_ASSIGNMENT ),
				nullptr );
		}
		if ( !j.contains( dto::GMODVERSIONING_DTO_KEY_CURRENT_ASSIGNMENT ) || !j.at( dto::GMODVERSIONING_DTO_KEY_CURRENT_ASSIGNMENT ).is_string() )
		{
			throw nlohmann::json::parse_error::create(
				101, 0u,
				fmt::format(
					"GmodVersioningAssignmentChangeDto JSON missing required '{}' field or not a string",
					dto::GMODVERSIONING_DTO_KEY_CURRENT_ASSIGNMENT ),
				nullptr );
		}

		dto.m_oldAssignment = j.at( dto::GMODVERSIONING_DTO_KEY_OLD_ASSIGNMENT ).get<std::string>();
		dto.m_currentAssignment = j.at( dto::GMODVERSIONING_DTO_KEY_CURRENT_ASSIGNMENT ).get<std::string>();

		if ( dto.m_oldAssignment.empty() )
		{
			fmt::print( stderr, "WARN: Empty 'oldAssignment' field found in GmodVersioningAssignmentChangeDto\n" );
		}
		if ( dto.m_currentAssignment.empty() )
		{
			fmt::print( stderr, "WARN: Empty 'currentAssignment' field found in GmodVersioningAssignmentChangeDto\n" );
		}
	}

	void to_json( nlohmann::json& j, const GmodVersioningAssignmentChangeDto& dto )
	{
		j = nlohmann::json{
			{ dto::GMODVERSIONING_DTO_KEY_OLD_ASSIGNMENT, dto.m_oldAssignment },
			{ dto::GMODVERSIONING_DTO_KEY_CURRENT_ASSIGNMENT, dto.m_currentAssignment } };
	}

	//=====================================================================
	// GMOD Node Conversion Transfer Object
	//=====================================================================

	//----------------------------------------------
	// Serialization
	//----------------------------------------------

	std::optional<GmodNodeConversionDto> GmodNodeConversionDto::tryFromJson( const nlohmann::json& json )
	{
		[[maybe_unused]] const auto sourceHint = GmodVersioningDtoInternal::extractSourceHint( json );

		try
		{
			if ( !json.is_object() )
			{
				fmt::print( stderr, "ERROR: JSON value for GmodNodeConversionDto is not an object\n" );

				return std::nullopt;
			}

			GmodNodeConversionDto dto = json.get<GmodNodeConversionDto>();
			return std::optional<GmodNodeConversionDto>{ std::move( dto ) };
		}
		catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
		{
			fmt::print( stderr, "ERROR: JSON exception during GmodNodeConversionDto parsing (hint: source='{}'): {}\n",
				sourceHint, ex.what() );

			return std::nullopt;
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			fmt::print( stderr, "ERROR: Standard exception during GmodNodeConversionDto parsing (hint: source='{}'): {}\n",
				sourceHint, ex.what() );

			return std::nullopt;
		}
	}

	GmodNodeConversionDto GmodNodeConversionDto::fromJson( const nlohmann::json& json )
	{
		const auto sourceHint = GmodVersioningDtoInternal::extractSourceHint( json );
		auto dtoOpt = GmodNodeConversionDto::tryFromJson( json );
		if ( !dtoOpt.has_value() )
		{
			throw std::invalid_argument(
				fmt::format(
					"Failed to deserialize GmodNodeConversionDto from JSON (hint: source='{}')",
					sourceHint ) );
		}

		return dtoOpt.value();
	}

	nlohmann::json GmodNodeConversionDto::toJson() const
	{
		nlohmann::json result;
		to_json( result, *this );

		return result;
	}

	//----------------------------------------------
	// Private serialization methods
	//----------------------------------------------

	void from_json( const nlohmann::json& j, GmodNodeConversionDto& dto )
	{
		if ( j.contains( dto::GMODVERSIONING_DTO_KEY_OPERATIONS ) )
		{
			if ( !j.at( dto::GMODVERSIONING_DTO_KEY_OPERATIONS ).is_array() )
			{
				throw nlohmann::json::type_error::create(
					302,
					fmt::format(
						"GmodNodeConversionDto JSON field '{}' is not an array",
						dto::GMODVERSIONING_DTO_KEY_OPERATIONS ),
					nullptr );
			}
			dto.m_operations.clear();
			const auto& opsArray = j.at( dto::GMODVERSIONING_DTO_KEY_OPERATIONS );

			dto.m_operations.reserve( opsArray.size() + opsArray.size() / 8 );
			for ( const auto& op : opsArray )
			{
				if ( !op.is_string() )
				{
					throw nlohmann::json::type_error::create(
						302,
						fmt::format(
							"GmodNodeConversionDto JSON field '{}' contains non-string element",
							dto::GMODVERSIONING_DTO_KEY_OPERATIONS ),
						nullptr );
				}
				dto.m_operations.insert( op.get<std::string>() );
			}
		}
		else
		{
			dto.m_operations.clear();
			fmt::print( stderr, "WARN: GmodNodeConversionDto JSON missing optional '{}' field\n", dto::GMODVERSIONING_DTO_KEY_OPERATIONS );
		}

		if ( j.contains( dto::GMODVERSIONING_DTO_KEY_SOURCE ) )
		{
			if ( !j.at( dto::GMODVERSIONING_DTO_KEY_SOURCE ).is_string() )
			{
				throw nlohmann::json::type_error::create(
					302,
					fmt::format(
						"GmodNodeConversionDto JSON field '{}' is not a string",
						dto::GMODVERSIONING_DTO_KEY_SOURCE ),
					nullptr );
			}
			dto.m_source = j.at( dto::GMODVERSIONING_DTO_KEY_SOURCE ).get<std::string>();
		}
		else
		{
			dto.m_source.clear();
			fmt::print( stderr, "WARN: GmodNodeConversionDto JSON missing optional '{}' field\n", dto::GMODVERSIONING_DTO_KEY_SOURCE );
		}

		if ( j.contains( dto::GMODVERSIONING_DTO_KEY_TARGET ) )
		{
			if ( !j.at( dto::GMODVERSIONING_DTO_KEY_TARGET ).is_string() )
			{
				throw nlohmann::json::type_error::create(
					302,
					fmt::format(
						"GmodNodeConversionDto JSON field '{}' is not a string",
						dto::GMODVERSIONING_DTO_KEY_TARGET ),
					nullptr );
			}
			dto.m_target = j.at( dto::GMODVERSIONING_DTO_KEY_TARGET ).get<std::string>();
		}
		else
		{
			dto.m_target.clear();
			fmt::print( stderr, "WARN: GmodNodeConversionDto JSON missing optional '{}' field\n", dto::GMODVERSIONING_DTO_KEY_TARGET );
		}

		if ( j.contains( dto::GMODVERSIONING_DTO_KEY_OLD_ASSIGNMENT ) )
		{
			if ( !j.at( dto::GMODVERSIONING_DTO_KEY_OLD_ASSIGNMENT ).is_string() )
			{
				throw nlohmann::json::type_error::create(
					302,
					fmt::format(
						"GmodNodeConversionDto JSON field '{}' is not a string",
						dto::GMODVERSIONING_DTO_KEY_OLD_ASSIGNMENT ),
					nullptr );
			}
			dto.m_oldAssignment = j.at( dto::GMODVERSIONING_DTO_KEY_OLD_ASSIGNMENT ).get<std::string>();
		}
		else
		{
			dto.m_oldAssignment.clear();
		}

		if ( j.contains( dto::GMODVERSIONING_DTO_KEY_NEW_ASSIGNMENT ) )
		{
			if ( !j.at( dto::GMODVERSIONING_DTO_KEY_NEW_ASSIGNMENT ).is_string() )
			{
				throw nlohmann::json::type_error::create(
					302,
					fmt::format(
						"GmodNodeConversionDto JSON field '{}' is not a string", dto::GMODVERSIONING_DTO_KEY_NEW_ASSIGNMENT ),
					nullptr );
			}
			dto.m_newAssignment = j.at( dto::GMODVERSIONING_DTO_KEY_NEW_ASSIGNMENT ).get<std::string>();
		}
		else
		{
			dto.m_newAssignment.clear();
		}

		if ( j.contains( dto::GMODVERSIONING_DTO_KEY_DELETE_ASSIGNMENT ) )
		{
			if ( !j.at( dto::GMODVERSIONING_DTO_KEY_DELETE_ASSIGNMENT ).is_boolean() )
			{
				throw nlohmann::json::type_error::create(
					302,
					fmt::format(
						"GmodNodeConversionDto JSON field '{}' is not a boolean",
						dto::GMODVERSIONING_DTO_KEY_DELETE_ASSIGNMENT ),
					nullptr );
			}
			dto.m_deleteAssignment = j.at( dto::GMODVERSIONING_DTO_KEY_DELETE_ASSIGNMENT ).get<bool>();
		}
		else
		{
			dto.m_deleteAssignment = false;
		}

		if ( dto.m_operations.empty() )
		{
			fmt::print( stderr, "WARN: Node conversion has no operations: source={}, target={}\n", dto.m_source, dto.m_target );
		}
		if ( dto.m_source.empty() && dto.m_target.empty() )
		{
			fmt::print( stderr, "WARN: Node conversion has empty source and target\n" );
		}
	}

	void to_json( nlohmann::json& j, const GmodNodeConversionDto& dto )
	{
		j = nlohmann::json{
			{ dto::GMODVERSIONING_DTO_KEY_OPERATIONS, dto.m_operations },
			{ dto::GMODVERSIONING_DTO_KEY_SOURCE, dto.m_source },
			{ dto::GMODVERSIONING_DTO_KEY_TARGET, dto.m_target },
			{ dto::GMODVERSIONING_DTO_KEY_OLD_ASSIGNMENT, dto.m_oldAssignment },
			{ dto::GMODVERSIONING_DTO_KEY_NEW_ASSIGNMENT, dto.m_newAssignment },
			{ dto::GMODVERSIONING_DTO_KEY_DELETE_ASSIGNMENT, dto.m_deleteAssignment } };
	}

	//=====================================================================
	// GMOD Versioning Data Transfer Object
	//=====================================================================

	//----------------------------------------------
	// Serialization
	//----------------------------------------------

	std::optional<GmodVersioningDto> GmodVersioningDto::tryFromJson( const nlohmann::json& json )
	{
		[[maybe_unused]] const auto visHint = GmodVersioningDtoInternal::extractVisHint( json );

		try
		{
			if ( !json.is_object() )
			{
				fmt::print( stderr, "ERROR: JSON value for GmodVersioningDto is not an object\n" );
				return std::nullopt;
			}

			GmodVersioningDto dto = json.get<GmodVersioningDto>();

			return std::optional<GmodVersioningDto>{ std::move( dto ) };
		}
		catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
		{
			fmt::print( stderr, "ERROR: JSON exception during GmodVersioningDto parsing (hint: visRelease='{}'): {}\n",
				visHint, ex.what() );

			return std::nullopt;
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			fmt::print( stderr, "ERROR: Standard exception during GmodVersioningDto parsing (hint: visRelease='{}'): {}\n",
				visHint, ex.what() );

			return std::nullopt;
		}
	}

	GmodVersioningDto GmodVersioningDto::fromJson( const nlohmann::json& json )
	{
		const auto visHint = GmodVersioningDtoInternal::extractVisHint( json );
		auto dtoOpt = GmodVersioningDto::tryFromJson( json );
		if ( !dtoOpt.has_value() )
		{
			throw std::invalid_argument( fmt::format( "Failed to deserialize GmodVersioningDto from JSON (hint: visRelease='{}')",
				visHint ) );
		}

		return std::move( dtoOpt ).value();
	}

	nlohmann::json GmodVersioningDto::toJson() const
	{
		nlohmann::json result;
		to_json( result, *this );

		return result;
	}

	//----------------------------------------------
	// Private serialization methods
	//----------------------------------------------

	void from_json( const nlohmann::json& j, GmodVersioningDto& dto )
	{
		if ( !j.contains( dto::GMODVERSIONING_DTO_KEY_VIS_RELEASE ) || !j.at( dto::GMODVERSIONING_DTO_KEY_VIS_RELEASE ).is_string() )
		{
			throw nlohmann::json::parse_error::create(
				101, 0u,
				fmt::format(
					"GmodVersioningDto JSON missing required '{}' field or not a string",
					dto::GMODVERSIONING_DTO_KEY_VIS_RELEASE ),
				nullptr );
		}
		dto.m_visVersion = j.at( dto::GMODVERSIONING_DTO_KEY_VIS_RELEASE ).get<std::string>();

		dto.m_items.clear();
		if ( j.contains( dto::GMODVERSIONING_DTO_KEY_ITEMS ) )
		{
			if ( !j.at( dto::GMODVERSIONING_DTO_KEY_ITEMS ).is_object() )
			{
				throw nlohmann::json::type_error::create(
					302,
					fmt::format(
						"GmodVersioningDto JSON field '{}' is not an object",
						dto::GMODVERSIONING_DTO_KEY_ITEMS ),
					nullptr );
			}

			const auto& itemsObj = j.at( dto::GMODVERSIONING_DTO_KEY_ITEMS );
			size_t itemCount = itemsObj.size();

			if ( itemCount > 10000 )
			{
			}

			size_t successCount = 0;

			dto.m_items.reserve( itemCount + itemCount / 8 );

			for ( const auto& [key, value] : itemsObj.items() )
			{
				try
				{
					GmodNodeConversionDto nodeDto = value.get<GmodNodeConversionDto>();

					dto.m_items.emplace( key, std::move( nodeDto ) );
					successCount++;
				}
				catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
				{
					fmt::print( stderr, "ERROR: Error parsing conversion item '{}': {}\n", key, ex.what() );
				}
				catch ( [[maybe_unused]] const std::exception& ex )
				{
					fmt::print( stderr, "ERROR: Standard exception parsing conversion item '{}': {}\n", key, ex.what() );
				}
			}

			/* If parsing failed for more than 10% of items, rebuild the map to potentially save memory */
			if ( itemCount > 0 && successCount < itemCount * 9 / 10 )
			{
				if ( dto.m_items.bucket_count() > dto.m_items.size() * 4 / 3 )
				{
					GmodVersioningDto::ItemsMap optimizedMap;
					optimizedMap.reserve( dto.m_items.size() );
					optimizedMap = std::move( dto.m_items );
					dto.m_items = std::move( optimizedMap );
				}
			}
		}
		else
		{
			fmt::print(
				stderr,
				"WARN: No '{}' object found in GMOD versioning data for VIS version {}\n",
				dto::GMODVERSIONING_DTO_KEY_ITEMS, dto.m_visVersion );
		}
	}

	void to_json( nlohmann::json& j, const GmodVersioningDto& dto )
	{
		j = nlohmann::json{ { dto::GMODVERSIONING_DTO_KEY_VIS_RELEASE, dto.m_visVersion } };

		if ( !dto.m_items.empty() )
		{
			j[dto::GMODVERSIONING_DTO_KEY_ITEMS] = dto.m_items;

			size_t emptyOperationsCount = 0;
			for ( const auto& [key, value] : dto.m_items )
			{
				if ( value.operations().empty() )
				{
					emptyOperationsCount++;
				}
			}
			if ( emptyOperationsCount > 0 )
			{
				fmt::print(
					stderr,
					"WARN: {} nodes have no operations defined during serialization\n",
					emptyOperationsCount );
			}
		}
		else
		{
			j[dto::GMODVERSIONING_DTO_KEY_ITEMS] = nlohmann::json::object();
		}
	}
}
