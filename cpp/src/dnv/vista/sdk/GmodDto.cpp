/**
 * @file GmodDto.cpp
 * @brief Implementation of Generic Product Model (GMOD) data transfer objects
 */

#include "pch.h"

#include "dnv/vista/sdk/GmodDto.h"

#include "dnv/vista/sdk/Config.h"

namespace dnv::vista::sdk
{
	namespace
	{
		//=====================================================================
		// JSON parsing helper functions
		//=====================================================================

		static constexpr std::string_view UNKNOWN_CODE = "[unknown code]";
		static constexpr std::string_view UNKNOWN_VERSION = "[unknown version]";

		std::string_view extractNameHint( const nlohmann::json& json ) noexcept
		{
			try
			{
				if ( json.contains( GMOD_DTO_KEY_CODE ) && json.at( GMOD_DTO_KEY_CODE ).is_string() )
				{
					const auto& str = json.at( GMOD_DTO_KEY_CODE ).get_ref<const std::string&>();
					return std::string_view{ str };
				}
				return UNKNOWN_CODE;
			}
			catch ( ... )
			{
				return UNKNOWN_CODE;
			}
		}

		std::string_view extractVisHint( const nlohmann::json& json ) noexcept
		{
			try
			{
				if ( json.contains( GMOD_DTO_KEY_VIS_RELEASE ) && json.at( GMOD_DTO_KEY_VIS_RELEASE ).is_string() )
				{
					const auto& str = json.at( GMOD_DTO_KEY_VIS_RELEASE ).get_ref<const std::string&>();
					return std::string_view{ str };
				}
				return UNKNOWN_VERSION;
			}
			catch ( ... )
			{
				return UNKNOWN_VERSION;
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
		const auto codeHint = extractNameHint( json );

		try
		{
			if ( !json.contains( GMOD_DTO_KEY_CODE ) || !json.at( GMOD_DTO_KEY_CODE ).is_string() )
			{
				SPDLOG_ERROR( "GMOD Node JSON missing required '{}' field or not a string",
					GMOD_DTO_KEY_CODE );
				return std::nullopt;
			}
			if ( !json.contains( GMOD_DTO_KEY_CATEGORY ) || !json.at( GMOD_DTO_KEY_CATEGORY ).is_string() )
			{
				SPDLOG_ERROR( "GMOD Node JSON missing required '{}' field or not a string",
					GMOD_DTO_KEY_CATEGORY );
				return std::nullopt;
			}
			if ( !json.contains( GMOD_DTO_KEY_TYPE ) || !json.at( GMOD_DTO_KEY_TYPE ).is_string() )
			{
				SPDLOG_ERROR( "GMOD Node JSON missing required '{}' field or not a string",
					GMOD_DTO_KEY_TYPE );
				return std::nullopt;
			}

			std::string tempCode = json.at( GMOD_DTO_KEY_CODE ).get<std::string>();
			std::string tempCategory = json.at( GMOD_DTO_KEY_CATEGORY ).get<std::string>();
			std::string tempType = json.at( GMOD_DTO_KEY_TYPE ).get<std::string>();

			std::string tempName;
			if ( json.contains( GMOD_DTO_KEY_NAME ) )
			{
				if ( json.at( GMOD_DTO_KEY_NAME ).is_string() )
				{
					tempName = json.at( GMOD_DTO_KEY_NAME ).get<std::string>();
				}
				else
				{
					SPDLOG_ERROR( "GMOD Node JSON (code='{}') field '{}' is present but not a string",
						tempCode, GMOD_DTO_KEY_NAME );
					return std::nullopt;
				}
			}
			else
			{
				SPDLOG_WARN( "GMOD Node JSON (code='{}') missing '{}' field. Defaulting name to empty string.",
					tempCode, GMOD_DTO_KEY_NAME );
				tempName = "";
			}

			if ( tempCode.empty() )
			{
				SPDLOG_WARN( "Empty code field found in GMOD node" );
			}
			if ( tempCategory.empty() )
			{
				SPDLOG_WARN( "Empty category field found in GMOD node code='{}'", tempCode );
			}
			if ( tempType.empty() )
			{
				SPDLOG_WARN( "Empty type field found in GMOD node code='{}'", tempCode );
			}
			if ( tempName.empty() )
			{
				SPDLOG_WARN( "Empty name field used for GMOD node code='{}'", tempCode );
			}

			std::optional<std::string> tempCommonName = std::nullopt;
			if ( json.contains( GMOD_DTO_KEY_COMMON_NAME ) )
			{
				if ( json.at( GMOD_DTO_KEY_COMMON_NAME ).is_string() )
				{
					tempCommonName = json.at( GMOD_DTO_KEY_COMMON_NAME ).get<std::string>();
				}
				else if ( !json.at( GMOD_DTO_KEY_COMMON_NAME ).is_null() )
				{
					SPDLOG_WARN( "GMOD Node code='{}' has non-string '{}'",
						tempCode, GMOD_DTO_KEY_COMMON_NAME );
				}
			}

			std::optional<std::string> tempDefinition = std::nullopt;
			if ( json.contains( GMOD_DTO_KEY_DEFINITION ) )
			{
				if ( json.at( GMOD_DTO_KEY_DEFINITION ).is_string() )
				{
					tempDefinition = json.at( GMOD_DTO_KEY_DEFINITION ).get<std::string>();
				}
				else if ( !json.at( GMOD_DTO_KEY_DEFINITION ).is_null() )
				{
					SPDLOG_WARN( "GMOD Node code='{}' has non-string '{}'",
						tempCode, GMOD_DTO_KEY_DEFINITION );
				}
			}

			std::optional<std::string> tempCommonDefinition = std::nullopt;
			if ( json.contains( GMOD_DTO_KEY_COMMON_DEFINITION ) )
			{
				if ( json.at( GMOD_DTO_KEY_COMMON_DEFINITION ).is_string() )
				{
					tempCommonDefinition = json.at( GMOD_DTO_KEY_COMMON_DEFINITION ).get<std::string>();
				}
				else if ( !json.at( GMOD_DTO_KEY_COMMON_DEFINITION ).is_null() )
				{
					SPDLOG_WARN( "GMOD Node code='{}' has non-string '{}'",
						tempCode, GMOD_DTO_KEY_COMMON_DEFINITION );
				}
			}

			std::optional<bool> tempInstallSubstructure = std::nullopt;
			if ( json.contains( GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE ) )
			{
				if ( json.at( GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE ).is_boolean() )
				{
					tempInstallSubstructure = json.at( GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE ).get<bool>();
				}
				else if ( !json.at( GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE ).is_null() )
				{
					SPDLOG_WARN( "GMOD Node code='{}' has non-bool '{}'",
						tempCode, GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE );
				}
			}

			std::optional<NormalAssignmentNamesMap> tempNormalAssignmentNames = std::nullopt;
			if ( json.contains( GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES ) )
			{
				if ( json.at( GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES ).is_object() )
				{
					try
					{
						auto assignments = json.at( GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES ).get<NormalAssignmentNamesMap>();
						if ( !assignments.empty() )
						{
							tempNormalAssignmentNames = std::move( assignments );
						}
					}
					catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
					{
						SPDLOG_WARN( "GMOD Node code='{}' failed to parse '{}' object: {}",
							tempCode, GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES, ex.what() );
					}
				}
				else if ( !json.at( GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES ).is_null() )
				{
					SPDLOG_WARN( "GMOD Node code='{}' has non-object '{}'",
						tempCode, GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES );
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
			SPDLOG_ERROR( "JSON exception during GmodNodeDto parsing (hint: code='{}'): {}",
				codeHint, ex.what() );
			return std::nullopt;
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			SPDLOG_ERROR( "Standard exception during GmodNodeDto parsing (hint: code='{}'): {}",
				codeHint, ex.what() );
			return std::nullopt;
		}
	}

	GmodNodeDto GmodNodeDto::fromJson( const nlohmann::json& json )
	{
		const auto codeHint = extractNameHint( json );
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
		if ( !j.contains( GMOD_DTO_KEY_CATEGORY ) || !j.at( GMOD_DTO_KEY_CATEGORY ).is_string() )
		{
			throw nlohmann::json::parse_error::create( 101, 0u,
				fmt::format( "GMOD Node JSON missing required '{}' field or not a string",
					GMOD_DTO_KEY_CATEGORY ),
				&j );
		}
		if ( !j.contains( GMOD_DTO_KEY_TYPE ) || !j.at( GMOD_DTO_KEY_TYPE ).is_string() )
		{
			throw nlohmann::json::parse_error::create( 102, 0u,
				fmt::format( "GMOD Node JSON missing required '{}' field or not a string",
					GMOD_DTO_KEY_TYPE ),
				&j );
		}
		if ( !j.contains( GMOD_DTO_KEY_CODE ) || !j.at( GMOD_DTO_KEY_CODE ).is_string() )
		{
			throw nlohmann::json::parse_error::create( 103, 0u,
				fmt::format( "GMOD Node JSON missing required '{}' field or not a string",
					GMOD_DTO_KEY_CODE ),
				&j );
		}

		/* Extract required fields with temporary variables */
		std::string tempCategory = j.at( GMOD_DTO_KEY_CATEGORY ).get<std::string>();
		std::string tempType = j.at( GMOD_DTO_KEY_TYPE ).get<std::string>();
		std::string tempCode = j.at( GMOD_DTO_KEY_CODE ).get<std::string>();

		/* SPECIAL CASE: Name field handling - matches tryFromJson logic */
		std::string tempName;
		if ( j.contains( GMOD_DTO_KEY_NAME ) )
		{
			if ( j.at( GMOD_DTO_KEY_NAME ).is_string() )
			{
				tempName = j.at( GMOD_DTO_KEY_NAME ).get<std::string>();
			}
			else if ( j.at( GMOD_DTO_KEY_NAME ).is_null() )
			{
				SPDLOG_WARN( "GMOD Node JSON (code='{}') has null '{}' field. Defaulting name to empty string.",
					tempCode, GMOD_DTO_KEY_NAME );
				tempName = "";
			}
			else
			{
				SPDLOG_WARN( "GMOD Node JSON (code='{}') has non-string '{}' field. Defaulting name to empty string.",
					tempCode, GMOD_DTO_KEY_NAME );
				tempName = "";
			}
		}
		else
		{
			SPDLOG_WARN( "GMOD Node JSON (code='{}') missing '{}' field. Defaulting name to empty string.",
				tempCode, GMOD_DTO_KEY_NAME );
			tempName = "";
		}

		/* Optional fields with robust validation (matches tryFromJson pattern) */
		std::optional<std::string> tempCommonName = std::nullopt;
		if ( j.contains( GMOD_DTO_KEY_COMMON_NAME ) )
		{
			if ( j.at( GMOD_DTO_KEY_COMMON_NAME ).is_string() )
			{
				tempCommonName = j.at( GMOD_DTO_KEY_COMMON_NAME ).get<std::string>();
			}
			else if ( !j.at( GMOD_DTO_KEY_COMMON_NAME ).is_null() )
			{
				SPDLOG_WARN( "GMOD Node code='{}' has non-string '{}' in from_json",
					tempCode, GMOD_DTO_KEY_COMMON_NAME );
			}
		}

		std::optional<std::string> tempDefinition = std::nullopt;
		if ( j.contains( GMOD_DTO_KEY_DEFINITION ) )
		{
			if ( j.at( GMOD_DTO_KEY_DEFINITION ).is_string() )
			{
				tempDefinition = j.at( GMOD_DTO_KEY_DEFINITION ).get<std::string>();
			}
			else if ( !j.at( GMOD_DTO_KEY_DEFINITION ).is_null() )
			{
				SPDLOG_WARN( "GMOD Node code='{}' has non-string '{}' in from_json",
					tempCode, GMOD_DTO_KEY_DEFINITION );
			}
		}

		std::optional<std::string> tempCommonDefinition = std::nullopt;
		if ( j.contains( GMOD_DTO_KEY_COMMON_DEFINITION ) )
		{
			if ( j.at( GMOD_DTO_KEY_COMMON_DEFINITION ).is_string() )
			{
				tempCommonDefinition = j.at( GMOD_DTO_KEY_COMMON_DEFINITION ).get<std::string>();
			}
			else if ( !j.at( GMOD_DTO_KEY_COMMON_DEFINITION ).is_null() )
			{
				SPDLOG_WARN( "GMOD Node code='{}' has non-string '{}' in from_json",
					tempCode, GMOD_DTO_KEY_COMMON_DEFINITION );
			}
		}

		std::optional<bool> tempInstallSubstructure = std::nullopt;
		if ( j.contains( GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE ) )
		{
			if ( j.at( GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE ).is_boolean() )
			{
				tempInstallSubstructure = j.at( GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE ).get<bool>();
			}
			else if ( !j.at( GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE ).is_null() )
			{
				SPDLOG_WARN( "GMOD Node code='{}' has non-bool '{}' in from_json",
					tempCode, GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE );
			}
		}

		std::optional<GmodNodeDto::NormalAssignmentNamesMap> tempNormalAssignmentNames = std::nullopt;
		if ( j.contains( GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES ) )
		{
			if ( j.at( GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES ).is_object() )
			{
				try
				{
					auto assignments = j.at( GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES ).get<GmodNodeDto::NormalAssignmentNamesMap>();
					if ( !assignments.empty() )
					{
						tempNormalAssignmentNames = std::move( assignments );
					}
				}
				catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
				{
					SPDLOG_WARN( "GMOD Node code='{}' failed to parse '{}' object in from_json: {}",
						tempCode, GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES, ex.what() );
				}
			}
			else if ( !j.at( GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES ).is_null() )
			{
				SPDLOG_WARN( "GMOD Node code='{}' has non-object '{}' in from_json",
					tempCode, GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES );
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
			{ GMOD_DTO_KEY_CATEGORY, dto.category() },
			{ GMOD_DTO_KEY_TYPE, dto.type() },
			{ GMOD_DTO_KEY_CODE, dto.code() },
			{ GMOD_DTO_KEY_NAME, dto.name() } };

		if ( dto.commonName().has_value() )
		{
			j[GMOD_DTO_KEY_COMMON_NAME] = dto.commonName().value();
		}
		if ( dto.definition().has_value() )
		{
			j[GMOD_DTO_KEY_DEFINITION] = dto.definition().value();
		}
		if ( dto.commonDefinition().has_value() )
		{
			j[GMOD_DTO_KEY_COMMON_DEFINITION] = dto.commonDefinition().value();
		}
		if ( dto.installSubstructure().has_value() )
		{
			j[GMOD_DTO_KEY_INSTALL_SUBSTRUCTURE] = dto.installSubstructure().value();
		}
		if ( dto.normalAssignmentNames().has_value() )
		{
			j[GMOD_DTO_KEY_NORMAL_ASSIGNMENT_NAMES] = dto.normalAssignmentNames().value();
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
		const auto visHint = extractVisHint( json );

		try
		{
			if ( !json.contains( GMOD_DTO_KEY_VIS_RELEASE ) || !json.at( GMOD_DTO_KEY_VIS_RELEASE ).is_string() )
			{
				SPDLOG_ERROR( "GMOD JSON missing required '{}' field or not a string",
					GMOD_DTO_KEY_VIS_RELEASE );
				return std::nullopt;
			}

			std::string tempVisVersion = json.at( GMOD_DTO_KEY_VIS_RELEASE ).get<std::string>();

			Items tempItems;
			size_t totalItems = 0;
			size_t successCount = 0;

			if ( json.contains( GMOD_DTO_KEY_ITEMS ) )
			{
				if ( !json.at( GMOD_DTO_KEY_ITEMS ).is_array() )
				{
					SPDLOG_WARN( "'{}' field is not an array for VIS version {}",
						GMOD_DTO_KEY_ITEMS, std::string_view{ tempVisVersion } );
				}
				else
				{
					const auto& itemsArray = json.at( GMOD_DTO_KEY_ITEMS );
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
							SPDLOG_WARN( "Skipping malformed GMOD node during GmodDto parsing for VIS version {}",
								std::string_view{ tempVisVersion } );
						}
					}

					SPDLOG_DEBUG( "Successfully parsed {}/{} GMOD nodes", successCount, totalItems );

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
				SPDLOG_WARN( "No '{}' array found in GmodDto for VIS version {}",
					GMOD_DTO_KEY_ITEMS, std::string_view{ tempVisVersion } );
			}

			Relations tempRelations;
			size_t relationCount = 0;
			size_t validRelationCount = 0;

			if ( json.contains( GMOD_DTO_KEY_RELATIONS ) )
			{
				if ( !json.at( GMOD_DTO_KEY_RELATIONS ).is_array() )
				{
					SPDLOG_WARN( "'{}' field is not an array for VIS version {}",
						GMOD_DTO_KEY_RELATIONS, std::string_view{ tempVisVersion } );
				}
				else
				{
					const auto& relationsArray = json.at( GMOD_DTO_KEY_RELATIONS );
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
									SPDLOG_WARN( "Non-string value found in relation entry for VIS version {}",
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
							SPDLOG_WARN( "Non-array entry found in '{}' array for VIS version {}",
								GMOD_DTO_KEY_RELATIONS, std::string_view{ tempVisVersion } );
						}
					}

					SPDLOG_DEBUG( "Successfully parsed {}/{} GMOD relations", validRelationCount, relationCount );

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
				SPDLOG_WARN( "No '{}' array found in GmodDto for VIS version {}",
					GMOD_DTO_KEY_RELATIONS, std::string_view{ tempVisVersion } );
			}

			if ( tempItems.size() > 10000 )
			{
				[[maybe_unused]] const size_t approxMemoryUsage = ( tempItems.size() * sizeof( GmodNodeDto ) + tempRelations.size() * 24 ) / ( 1024 * 1024 );
				SPDLOG_DEBUG( "Large GMOD model loaded: ~{} MB estimated memory usage", approxMemoryUsage );
			}

			/* Construct the final DTO using successfully parsed data */
			GmodDto resultDto( std::move( tempVisVersion ), std::move( tempItems ), std::move( tempRelations ) );

			return resultDto;
		}
		catch ( [[maybe_unused]] const nlohmann::json::exception& ex )
		{
			SPDLOG_ERROR( "JSON exception during GmodDto parsing (hint: visRelease='{}'): {}",
				visHint, ex.what() );
			return std::nullopt;
		}
		catch ( [[maybe_unused]] const std::exception& ex )
		{
			SPDLOG_ERROR( "Standard exception during GmodDto parsing (hint: visRelease='{}'): {}",
				visHint, ex.what() );
			return std::nullopt;
		}
	}

	GmodDto GmodDto::fromJson( const nlohmann::json& json )
	{
		const auto visHint = extractVisHint( json );
		auto dtoOpt = GmodDto::tryFromJson( json );
		if ( !dtoOpt.has_value() )
		{
			throw std::invalid_argument( fmt::format( "Failed to deserialize GmodDto from JSON (hint: visRelease='{}')",
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
		if ( !j.contains( GMOD_DTO_KEY_VIS_RELEASE ) || !j.at( GMOD_DTO_KEY_VIS_RELEASE ).is_string() )
		{
			throw nlohmann::json::parse_error::create( 201, 0u,
				fmt::format( "GMOD JSON missing required '{}' field or not a string",
					GMOD_DTO_KEY_VIS_RELEASE ),
				&j );
		}
		if ( !j.contains( GMOD_DTO_KEY_ITEMS ) || !j.at( GMOD_DTO_KEY_ITEMS ).is_array() )
		{
			throw nlohmann::json::parse_error::create( 202, 0u,
				fmt::format( "GMOD JSON missing required '{}' field or not an array",
					GMOD_DTO_KEY_ITEMS ),
				&j );
		}
		if ( !j.contains( GMOD_DTO_KEY_RELATIONS ) || !j.at( GMOD_DTO_KEY_RELATIONS ).is_array() )
		{
			throw nlohmann::json::parse_error::create( 203, 0u,
				fmt::format( "GMOD JSON missing required '{}' field or not an array",
					GMOD_DTO_KEY_RELATIONS ),
				&j );
		}

		/* Extract required fields with temporary variables */
		std::string tempVisVersion = j.at( GMOD_DTO_KEY_VIS_RELEASE ).get<std::string>();

		GmodDto::Items tempItems;
		if ( j.contains( GMOD_DTO_KEY_ITEMS ) && j.at( GMOD_DTO_KEY_ITEMS ).is_array() )
		{
			tempItems = j.at( GMOD_DTO_KEY_ITEMS ).get<GmodDto::Items>();
		}

		GmodDto::Relations tempRelations;
		if ( j.contains( GMOD_DTO_KEY_RELATIONS ) && j.at( GMOD_DTO_KEY_RELATIONS ).is_array() )
		{
			tempRelations = j.at( GMOD_DTO_KEY_RELATIONS ).get<GmodDto::Relations>();
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
			{ GMOD_DTO_KEY_VIS_RELEASE, dto.visVersion() },
			{ GMOD_DTO_KEY_ITEMS, dto.items() },
			{ GMOD_DTO_KEY_RELATIONS, dto.relations() } };
	}
}
