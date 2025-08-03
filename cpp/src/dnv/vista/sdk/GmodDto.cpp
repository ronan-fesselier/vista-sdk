/**
 * @file GmodDto.cpp
 * @brief Implementation of Generic Product Model (GMOD) data transfer objects
 */

#include "pch.h"

#include "dnv/vista/sdk/GmodDto.h"

#include "dnv/vista/sdk/config/DtoKeys.h"

namespace dnv::vista::sdk
{
	namespace GmodDtoInternal
	{
		//=====================================================================
		// JSON parsing helper functions
		//=====================================================================

		std::string_view extractVisHint( const nlohmann::json& json ) noexcept
		{
			try
			{
				if ( json.contains( dto::GMOD_DTO_KEY_VIS_RELEASE ) && json.at( dto::GMOD_DTO_KEY_VIS_RELEASE ).is_string() )
				{
					const auto& str = json.at( dto::GMOD_DTO_KEY_VIS_RELEASE ).get_ref<const std::string&>();

					return std::string_view{ str };
				}

				return dto::GMOD_DTO_UNKNOWN_VERSION;
			}
			catch ( ... )
			{
				return dto::GMOD_DTO_UNKNOWN_VERSION;
			}
		}

		std::string_view extractNameHint( const nlohmann::json& json ) noexcept
		{
			try
			{
				if ( json.contains( dto::GMOD_DTO_KEY_CODE ) && json.at( dto::GMOD_DTO_KEY_CODE ).is_string() )
				{
					const auto& str = json.at( dto::GMOD_DTO_KEY_CODE ).get_ref<const std::string&>();

					return std::string_view{ str };
				}

				return dto::GMOD_DTO_UNKNOWN_CODE;
			}
			catch ( ... )
			{
				return dto::GMOD_DTO_UNKNOWN_CODE;
			}
		}
	}

	//=====================================================================
	// GMOD Node Data Transfer Object
	//=====================================================================

	//----------------------------------------------
	// Serialization
	//----------------------------------------------

	std::optional<GmodNodeDto> GmodNodeDto::tryFromJson( const nlohmann::json& json )
	{
		[[maybe_unused]] const auto codeHint = GmodDtoInternal::extractNameHint( json );

		try
		{
			if ( !json.contains( dto::GMOD_DTO_KEY_CODE ) || !json.at( dto::GMOD_DTO_KEY_CODE ).is_string() )
			{
				fmt::print( stderr, "ERROR: GMOD Node JSON missing required '{}' field or not a string\n",
					dto::GMOD_DTO_KEY_CODE );

				return std::nullopt;
			}
			if ( !json.contains( dto::GMOD_DTO_KEY_CATEGORY ) || !json.at( dto::GMOD_DTO_KEY_CATEGORY ).is_string() )
			{
				fmt::print( stderr, "ERROR: GMOD Node JSON missing required '{}' field or not a string\n",
					dto::GMOD_DTO_KEY_CATEGORY );

				return std::nullopt;
			}
			if ( !json.contains( dto::GMOD_DTO_KEY_TYPE ) || !json.at( dto::GMOD_DTO_KEY_TYPE ).is_string() )
			{
				fmt::print( stderr, "ERROR: GMOD Node JSON missing required '{}' field or not a string\n",
					dto::GMOD_DTO_KEY_TYPE );

				return std::nullopt;
			}

			std::string tempCode = json.at( dto::GMOD_DTO_KEY_CODE ).get<std::string>();
			std::string tempCategory = json.at( dto::GMOD_DTO_KEY_CATEGORY ).get<std::string>();
			std::string tempType = json.at( dto::GMOD_DTO_KEY_TYPE ).get<std::string>();

			std::string tempName;
			if ( json.contains( dto::GMOD_DTO_KEY_NAME ) )
			{
				if ( json.at( dto::GMOD_DTO_KEY_NAME ).is_string() )
				{
					tempName = json.at( dto::GMOD_DTO_KEY_NAME ).get<std::string>();
				}
				else
				{
					fmt::print( stderr, "ERROR: GMOD Node JSON (code='{}') field '{}' is present but not a string\n",
						tempCode, dto::GMOD_DTO_KEY_NAME );

					return std::nullopt;
				}
			}
			else
			{
				fmt::print( stderr, "WARN: GMOD Node JSON (code='{}') missing '{}' field. Defaulting name to empty string.\n",
					tempCode, dto::GMOD_DTO_KEY_NAME );

				tempName = "";
			}

			if ( tempCode.empty() )
			{
				fmt::print( stderr, "WARN: Empty code field found in GMOD node\n" );
			}
			if ( tempCategory.empty() )
			{
				fmt::print( stderr, "WARN: Empty category field found in GMOD node code='{}'\n", tempCode );
			}
			if ( tempType.empty() )
			{
				fmt::print( stderr, "WARN: Empty type field found in GMOD node code='{}'\n", tempCode );
			}
			if ( tempName.empty() )
			{
				fmt::print( stderr, "WARN: Empty name field used for GMOD node code='{}'\n", tempCode );
			}

			std::optional<std::string> tempCommonName = std::nullopt;
			if ( json.contains( dto::GMOD_DTO_KEY_COMMON_NAME ) )
			{
				if ( json.at( dto::GMOD_DTO_KEY_COMMON_NAME ).is_string() )
				{
					tempCommonName = json.at( dto::GMOD_DTO_KEY_COMMON_NAME ).get<std::string>();
				}
				else if ( !json.at( dto::GMOD_DTO_KEY_COMMON_NAME ).is_null() )
				{
					fmt::print( stderr, "WARN: GMOD Node code='{}' has non-string '{}'\n",
						tempCode, dto::GMOD_DTO_KEY_COMMON_NAME );
				}
			}

			std::optional<std::string> tempDefinition = std::nullopt;
			if ( json.contains( dto::GMOD_DTO_KEY_DEFINITION ) )
			{
				if ( json.at( dto::GMOD_DTO_KEY_DEFINITION ).is_string() )
				{
					tempDefinition = json.at( dto::GMOD_DTO_KEY_DEFINITION ).get<std::string>();
				}
				else if ( !json.at( dto::GMOD_DTO_KEY_DEFINITION ).is_null() )
				{
					fmt::print( stderr, "WARN: GMOD Node code='{}' has non-string '{}'\n",
						tempCode, dto::GMOD_DTO_KEY_DEFINITION );
				}
			}

			std::optional<std::string> tempCommonDefinition = std::nullopt;
			if ( json.contains( dto::GMOD_DTO_KEY_COMMON_DEFINITION ) )
			{
				if ( json.at( dto::GMOD_DTO_KEY_COMMON_DEFINITION ).is_string() )
				{
					tempCommonDefinition = json.at( dto::GMOD_DTO_KEY_COMMON_DEFINITION ).get<std::string>();
				}
				else if ( !json.at( dto::GMOD_DTO_KEY_COMMON_DEFINITION ).is_null() )
				{
					fmt::print( stderr, "WARN: GMOD Node code='{}' has non-string '{}'\n",
						tempCode, dto::GMOD_DTO_KEY_COMMON_DEFINITION );
				}
			}

			std::optional<bool> tempInstallSubstructure = std::nullopt;
			if ( json.contains( dto::GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE ) )
			{
				if ( json.at( dto::GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE ).is_boolean() )
				{
					tempInstallSubstructure = json.at( dto::GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE ).get<bool>();
				}
				else if ( !json.at( dto::GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE ).is_null() )
				{
					fmt::print( stderr, "WARN: GMOD Node code='{}' has non-bool '{}'\n",
						tempCode, dto::GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE );
				}
			}

			std::optional<NormalAssignmentNamesMap> tempNormalAssignmentNames = std::nullopt;
			if ( json.contains( dto::GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES ) )
			{
				if ( json.at( dto::GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES ).is_object() )
				{
					try
					{
						auto assignments = json.at( dto::GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES ).get<NormalAssignmentNamesMap>();
						if ( !assignments.empty() )
						{
							tempNormalAssignmentNames = std::move( assignments );
						}
					}
					catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
					{
						fmt::print( stderr, "WARN: GMOD Node code='{}' failed to parse '{}' object: {}\n",
							tempCode, dto::GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES, ex.what() );
					}
				}
				else if ( !json.at( dto::GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES ).is_null() )
				{
					fmt::print( stderr, "WARN: GMOD Node code='{}' has non-object '{}'\n",
						tempCode, dto::GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES );
				}
			}

			/* Construct the final DTO using successfully parsed data */
			GmodNodeDto result(
				std::move( tempCategory ),
				std::move( tempType ),
				std::move( tempCode ),
				std::move( tempName ),
				std::move( tempCommonName ),
				std::move( tempDefinition ),
				std::move( tempCommonDefinition ),
				tempInstallSubstructure,
				std::move( tempNormalAssignmentNames ) );

			return result;
		}
		catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
		{
			fmt::print( stderr, "ERROR: JSON exception during GmodNodeDto parsing (hint: code='{}'): {}\n",
				codeHint, ex.what() );

			return std::nullopt;
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			fmt::print( stderr, "ERROR: Standard exception during GmodNodeDto parsing (hint: code='{}'): {}\n",
				codeHint, ex.what() );

			return std::nullopt;
		}
	}

	GmodNodeDto GmodNodeDto::fromJson( const nlohmann::json& json )
	{
		const auto codeHint = GmodDtoInternal::extractNameHint( json );
		auto dtoOpt = GmodNodeDto::tryFromJson( json );
		if ( !dtoOpt.has_value() )
		{
			throw std::invalid_argument(
				fmt::format(
					"Failed to deserialize GmodNodeDto from JSON (hint: code='{}')",
					codeHint ) );
		}

		return std::move( dtoOpt.value() );
	}

	nlohmann::json GmodNodeDto::toJson() const
	{
		nlohmann::json result;
		to_json( result, *this );

		return result;
	}

	//----------------------------------------------
	// Private serialization methods
	//----------------------------------------------

	void from_json( const nlohmann::json& j, GmodNodeDto& dto )
	{
		/* ADL hook for nlohmann::json deserialization. */
		if ( !j.contains( dto::GMOD_DTO_KEY_CATEGORY ) || !j.at( dto::GMOD_DTO_KEY_CATEGORY ).is_string() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u,
				fmt::format( "GMOD Node JSON missing required '{}' field or not a string",
					dto::GMOD_DTO_KEY_CATEGORY ),
				&j );
		}
		if ( !j.contains( dto::GMOD_DTO_KEY_TYPE ) || !j.at( dto::GMOD_DTO_KEY_TYPE ).is_string() )
		{
			throw nlohmann::json::parse_error::create( 102, 0u,
				fmt::format( "GMOD Node JSON missing required '{}' field or not a string",
					dto::GMOD_DTO_KEY_TYPE ),
				&j );
		}
		if ( !j.contains( dto::GMOD_DTO_KEY_CODE ) || !j.at( dto::GMOD_DTO_KEY_CODE ).is_string() )
		{
			throw nlohmann::json::parse_error::create( 103, 0u,
				fmt::format( "GMOD Node JSON missing required '{}' field or not a string",
					dto::GMOD_DTO_KEY_CODE ),
				&j );
		}

		/* Extract required fields with temporary variables */
		std::string tempCategory = j.at( dto::GMOD_DTO_KEY_CATEGORY ).get<std::string>();
		std::string tempType = j.at( dto::GMOD_DTO_KEY_TYPE ).get<std::string>();
		std::string tempCode = j.at( dto::GMOD_DTO_KEY_CODE ).get<std::string>();

		/* SPECIAL CASE: Name field handling - matches tryFromJson logic */
		std::string tempName;
		if ( j.contains( dto::GMOD_DTO_KEY_NAME ) )
		{
			if ( j.at( dto::GMOD_DTO_KEY_NAME ).is_string() )
			{
				tempName = j.at( dto::GMOD_DTO_KEY_NAME ).get<std::string>();
			}
			else if ( j.at( dto::GMOD_DTO_KEY_NAME ).is_null() )
			{
				fmt::print( stderr, "WARN: GMOD Node JSON (code='{}') has null '{}' field. Defaulting name to empty string.\n",
					tempCode, dto::GMOD_DTO_KEY_NAME );
				tempName = "";
			}
			else
			{
				fmt::print( stderr, "WARN: GMOD Node JSON (code='{}') has non-string '{}' field. Defaulting name to empty string.\n",
					tempCode, dto::GMOD_DTO_KEY_NAME );
				tempName = "";
			}
		}
		else
		{
			fmt::print( stderr, "WARN: GMOD Node JSON (code='{}') missing '{}' field. Defaulting name to empty string.\n",
				tempCode, dto::GMOD_DTO_KEY_NAME );
			tempName = "";
		}

		/* Optional fields with robust validation (matches tryFromJson pattern) */
		std::optional<std::string> tempCommonName = std::nullopt;
		if ( j.contains( dto::GMOD_DTO_KEY_COMMON_NAME ) )
		{
			if ( j.at( dto::GMOD_DTO_KEY_COMMON_NAME ).is_string() )
			{
				tempCommonName = j.at( dto::GMOD_DTO_KEY_COMMON_NAME ).get<std::string>();
			}
			else if ( !j.at( dto::GMOD_DTO_KEY_COMMON_NAME ).is_null() )
			{
				fmt::print( stderr, "WARN: GMOD Node code='{}' has non-string '{}' in from_json\n",
					tempCode, dto::GMOD_DTO_KEY_COMMON_NAME );
			}
		}

		std::optional<std::string> tempDefinition = std::nullopt;
		if ( j.contains( dto::GMOD_DTO_KEY_DEFINITION ) )
		{
			if ( j.at( dto::GMOD_DTO_KEY_DEFINITION ).is_string() )
			{
				tempDefinition = j.at( dto::GMOD_DTO_KEY_DEFINITION ).get<std::string>();
			}
			else if ( !j.at( dto::GMOD_DTO_KEY_DEFINITION ).is_null() )
			{
				fmt::print( stderr, "WARN: GMOD Node code='{}' has non-string '{}' in from_json\n",
					tempCode, dto::GMOD_DTO_KEY_DEFINITION );
			}
		}

		std::optional<std::string> tempCommonDefinition = std::nullopt;
		if ( j.contains( dto::GMOD_DTO_KEY_COMMON_DEFINITION ) )
		{
			if ( j.at( dto::GMOD_DTO_KEY_COMMON_DEFINITION ).is_string() )
			{
				tempCommonDefinition = j.at( dto::GMOD_DTO_KEY_COMMON_DEFINITION ).get<std::string>();
			}
			else if ( !j.at( dto::GMOD_DTO_KEY_COMMON_DEFINITION ).is_null() )
			{
				fmt::print( stderr, "WARN: GMOD Node code='{}' has non-string '{}' in from_json\n",
					tempCode, dto::GMOD_DTO_KEY_COMMON_DEFINITION );
			}
		}

		std::optional<bool> tempInstallSubstructure = std::nullopt;
		if ( j.contains( dto::GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE ) )
		{
			if ( j.at( dto::GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE ).is_boolean() )
			{
				tempInstallSubstructure = j.at( dto::GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE ).get<bool>();
			}
			else if ( !j.at( dto::GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE ).is_null() )
			{
				fmt::print( stderr, "WARN: GMOD Node code='{}' has non-bool '{}' in from_json\n",
					tempCode, dto::GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE );
			}
		}

		std::optional<GmodNodeDto::NormalAssignmentNamesMap> tempNormalAssignmentNames = std::nullopt;
		if ( j.contains( dto::GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES ) )
		{
			if ( j.at( dto::GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES ).is_object() )
			{
				try
				{
					auto assignments = j.at( dto::GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES ).get<GmodNodeDto::NormalAssignmentNamesMap>();
					if ( !assignments.empty() )
					{
						tempNormalAssignmentNames = std::move( assignments );
					}
				}
				catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
				{
					fmt::print( stderr, "WARN: GMOD Node code='{}' failed to parse '{}' object in from_json: {}\n",
						tempCode, dto::GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES, ex.what() );
				}
			}
			else if ( !j.at( dto::GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES ).is_null() )
			{
				fmt::print( stderr, "WARN: GMOD Node code='{}' has non-object '{}' in from_json\n",
					tempCode, dto::GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES );
			}
		}

		/* Assign all fields to DTO using successfully parsed data */
		dto.m_category = std::move( tempCategory );
		dto.m_type = std::move( tempType );
		dto.m_code = std::move( tempCode );
		dto.m_name = std::move( tempName );
		dto.m_commonName = std::move( tempCommonName );
		dto.m_definition = std::move( tempDefinition );
		dto.m_commonDefinition = std::move( tempCommonDefinition );
		dto.m_installSubstructure = tempInstallSubstructure;
		dto.m_normalAssignmentNames = std::move( tempNormalAssignmentNames );
	}

	void to_json( nlohmann::json& j, const GmodNodeDto& dto )
	{
		/* ADL hook for nlohmann::json serialization. */
		j = {
			{ dto::GMOD_DTO_KEY_CATEGORY, dto.category() },
			{ dto::GMOD_DTO_KEY_TYPE, dto.type() },
			{ dto::GMOD_DTO_KEY_CODE, dto.code() },
			{ dto::GMOD_DTO_KEY_NAME, dto.name() } };

		if ( dto.commonName().has_value() )
		{
			j[dto::GMOD_DTO_KEY_COMMON_NAME] = dto.commonName().value();
		}
		if ( dto.definition().has_value() )
		{
			j[dto::GMOD_DTO_KEY_DEFINITION] = dto.definition().value();
		}
		if ( dto.commonDefinition().has_value() )
		{
			j[dto::GMOD_DTO_KEY_COMMON_DEFINITION] = dto.commonDefinition().value();
		}
		if ( dto.installSubstructure().has_value() )
		{
			j[dto::GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE] = dto.installSubstructure().value();
		}
		if ( dto.normalAssignmentNames().has_value() )
		{
			j[dto::GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES] = dto.normalAssignmentNames().value();
		}
	}

	//=====================================================================
	// GMOD Data Transfer Object
	//=====================================================================

	//----------------------------------------------
	// Serialization
	//----------------------------------------------

	std::optional<GmodDto> GmodDto::tryFromJson( const nlohmann::json& json )
	{
		[[maybe_unused]] const auto visHint = GmodDtoInternal::extractVisHint( json );

		try
		{
			if ( !json.contains( dto::GMOD_DTO_KEY_VIS_RELEASE ) || !json.at( dto::GMOD_DTO_KEY_VIS_RELEASE ).is_string() )
			{
				fmt::print( stderr, "ERROR: GMOD JSON missing required '{}' field or not a string\n",
					dto::GMOD_DTO_KEY_VIS_RELEASE );

				return std::nullopt;
			}

			std::string tempVisVersion = json.at( dto::GMOD_DTO_KEY_VIS_RELEASE ).get<std::string>();

			Items tempItems;
			size_t totalItems = 0;
			size_t successCount = 0;

			if ( json.contains( dto::GMOD_DTO_KEY_ITEMS ) )
			{
				if ( !json.at( dto::GMOD_DTO_KEY_ITEMS ).is_array() )
				{
					fmt::print( stderr, "WARN: '{}' field is not an array for VIS version {}\n",
						dto::GMOD_DTO_KEY_ITEMS, std::string_view{ tempVisVersion } );
				}
				else
				{
					const auto& itemsArray = json.at( dto::GMOD_DTO_KEY_ITEMS );
					totalItems = itemsArray.size();
					tempItems.reserve( totalItems + totalItems / 8 );

					for ( const auto& itemJson : itemsArray )
					{
						auto nodeOpt = GmodNodeDto::tryFromJson( itemJson );
						if ( nodeOpt.has_value() )
						{
							tempItems.emplace_back( std::move( *nodeOpt ) );
							successCount++;
						}
						else
						{
							fmt::print( stderr, "WARN: Skipping malformed GMOD node during GmodDto parsing for VIS version {}\n",
								std::string_view{ tempVisVersion } );
						}
					}

					/* If parsing failed for more than 10% of items, shrink the vector to potentially save memory */
					if ( totalItems > 0 && successCount < totalItems * 9 / 10 )
					{
						if ( tempItems.capacity() > tempItems.size() * 4 / 3 )
						{
							tempItems.shrink_to_fit();
						}
					}
				}
			}
			else
			{
				fmt::print( stderr, "WARN: No '{}' array found in GmodDto for VIS version {}\n",
					dto::GMOD_DTO_KEY_ITEMS, std::string_view{ tempVisVersion } );
			}

			Relations tempRelations;
			size_t relationCount = 0;
			size_t validRelationCount = 0;

			if ( json.contains( dto::GMOD_DTO_KEY_RELATIONS ) )
			{
				if ( !json.at( dto::GMOD_DTO_KEY_RELATIONS ).is_array() )
				{
					fmt::print( stderr, "WARN: '{}' field is not an array for VIS version {}\n",
						dto::GMOD_DTO_KEY_RELATIONS, std::string_view{ tempVisVersion } );
				}
				else
				{
					const auto& relationsArray = json.at( dto::GMOD_DTO_KEY_RELATIONS );
					relationCount = relationsArray.size();
					tempRelations.reserve( relationCount + relationCount / 8 );

					for ( const auto& relationJson : relationsArray )
					{
						if ( relationJson.is_array() )
						{
							Relation relationPair;
							relationPair.reserve( relationJson.size() + 1 );
							bool validPair = true;

							for ( const auto& relJson : relationJson )
							{
								if ( relJson.is_string() )
								{
									relationPair.emplace_back( relJson.get<std::string>() );
								}
								else
								{
									fmt::print( stderr, "WARN: Non-string value found in relation entry for VIS version {}\n",
										std::string_view{ tempVisVersion } );
									validPair = false;
									break;
								}
							}

							if ( validPair && !relationPair.empty() )
							{
								tempRelations.emplace_back( std::move( relationPair ) );
								validRelationCount++;
							}
						}
						else
						{
							fmt::print( stderr, "WARN: Non-array entry found in '{}' array for VIS version {}\n",
								dto::GMOD_DTO_KEY_RELATIONS, std::string_view{ tempVisVersion } );
						}
					}

					/* If parsing failed for more than 10% of relations, shrink the vector to potentially save memory */
					if ( relationCount > 0 && validRelationCount < relationCount * 9 / 10 )
					{
						if ( tempRelations.capacity() > tempRelations.size() * 4 / 3 )
						{
							tempRelations.shrink_to_fit();
						}
					}
				}
			}
			else
			{
				fmt::print( stderr, "WARN: No '{}' array found in GmodDto for VIS version {}\n",
					dto::GMOD_DTO_KEY_RELATIONS, std::string_view{ tempVisVersion } );
			}

			if ( tempItems.size() > 10000 )
			{
			}

			/* Construct the final DTO using successfully parsed data */
			GmodDto resultDto( std::move( tempVisVersion ), std::move( tempItems ), std::move( tempRelations ) );

			return resultDto;
		}
		catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
		{
			fmt::print( stderr, "ERROR: JSON exception during GmodDto parsing (hint: visRelease='{}'): {}\n",
				visHint, ex.what() );
			return std::nullopt;
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			fmt::print( stderr, "ERROR: Standard exception during GmodDto parsing (hint: visRelease='{}'): {}\n",
				visHint, ex.what() );
			return std::nullopt;
		}
	}

	GmodDto GmodDto::fromJson( const nlohmann::json& json )
	{
		const auto visHint = GmodDtoInternal::extractVisHint( json );
		auto dtoOpt = GmodDto::tryFromJson( json );
		if ( !dtoOpt.has_value() )
		{
			throw std::invalid_argument(
				fmt::format(
					"Failed to deserialize GmodDto from JSON (hint: visRelease='{}')",
					visHint ) );
		}

		return std::move( dtoOpt.value() );
	}

	nlohmann::json GmodDto::toJson() const
	{
		nlohmann::json result;
		to_json( result, *this );

		return result;
	}

	//-------------------------------------------------------------------
	// Private serialization methods
	//-------------------------------------------------------------------

	void from_json( const nlohmann::json& j, GmodDto& dto )
	{
		/* ADL hook for nlohmann::json deserialization. */
		if ( !j.contains( dto::GMOD_DTO_KEY_VIS_RELEASE ) || !j.at( dto::GMOD_DTO_KEY_VIS_RELEASE ).is_string() )
		{
			throw nlohmann::json::parse_error::create( 201, 0u,
				fmt::format( "GMOD JSON missing required '{}' field or not a string",
					dto::GMOD_DTO_KEY_VIS_RELEASE ),
				&j );
		}
		if ( !j.contains( dto::GMOD_DTO_KEY_ITEMS ) || !j.at( dto::GMOD_DTO_KEY_ITEMS ).is_array() )
		{
			throw nlohmann::json::parse_error::create( 202, 0u,
				fmt::format( "GMOD JSON missing required '{}' field or not an array",
					dto::GMOD_DTO_KEY_ITEMS ),
				&j );
		}
		if ( !j.contains( dto::GMOD_DTO_KEY_RELATIONS ) || !j.at( dto::GMOD_DTO_KEY_RELATIONS ).is_array() )
		{
			throw nlohmann::json::parse_error::create( 203, 0u,
				fmt::format( "GMOD JSON missing required '{}' field or not an array",
					dto::GMOD_DTO_KEY_RELATIONS ),
				&j );
		}

		/* Extract required fields with temporary variables */
		std::string tempVisVersion = j.at( dto::GMOD_DTO_KEY_VIS_RELEASE ).get<std::string>();

		GmodDto::Items tempItems;
		if ( j.contains( dto::GMOD_DTO_KEY_ITEMS ) && j.at( dto::GMOD_DTO_KEY_ITEMS ).is_array() )
		{
			tempItems = j.at( dto::GMOD_DTO_KEY_ITEMS ).get<GmodDto::Items>();
		}

		GmodDto::Relations tempRelations;
		if ( j.contains( dto::GMOD_DTO_KEY_RELATIONS ) && j.at( dto::GMOD_DTO_KEY_RELATIONS ).is_array() )
		{
			tempRelations = j.at( dto::GMOD_DTO_KEY_RELATIONS ).get<GmodDto::Relations>();
		}

		/* Assign all fields to DTO using successfully parsed data */
		dto.m_visVersion = std::move( tempVisVersion );
		dto.m_items = std::move( tempItems );
		dto.m_relations = std::move( tempRelations );
	}

	void to_json( nlohmann::json& j, const GmodDto& dto )
	{
		/* ADL hook for nlohmann::json serialization. */
		j = {
			{ dto::GMOD_DTO_KEY_VIS_RELEASE, dto.visVersion() },
			{ dto::GMOD_DTO_KEY_ITEMS, dto.items() },
			{ dto::GMOD_DTO_KEY_RELATIONS, dto.relations() } };
	}
}
