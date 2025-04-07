/*
	Copyright @kzu
	License MIT
	https://github.com/devlooped/ThisAssembly/blob/main/src/EmbeddedResource.cs
*/

#pragma once

#include "ISO19848Dtos.h"

namespace dnv::vista::sdk
{
	struct CodebooksDto;
	struct GmodDto;
	struct GmodVersioningDto;
	struct LocationsDto;
	/**
	 * @brief Utility class for accessing embedded resources
	 */
	class EmbeddedResource
	{
	public:
		/**
		 * @brief Get all available VIS versions
		 * @return List of VIS version strings
		 */
		static std::vector<std::string> GetVisVersions();

		/**
		 * @brief Get GMOD for specific VIS version
		 * @param visVersion VIS version string
		 * @return GMOD DTO, or nullptr if not found
		 */
		static std::optional<GmodDto> GetGmod( const std::string& visVersion );

		/**
		 * @brief Get codebooks for specific VIS version
		 * @param visVersion VIS version string
		 * @return Codebooks DTO, or nullptr if not found
		 */
		static std::optional<CodebooksDto> GetCodebooks( const std::string& visVersion );

		/**
		 * @brief Get all GMOD versioning data
		 * @return Dictionary of versioning DTOs by version string
		 */
		static std::optional<std::unordered_map<std::string, GmodVersioningDto>> GetGmodVersioning();

		/**
		 * @brief Get locations for specific VIS version
		 * @param visVersion VIS version string
		 * @return Locations DTO, or nullptr if not found
		 */
		static std::optional<LocationsDto> GetLocations( const std::string& visVersion );

		/**
		 * @brief Get data channel type names for specific version
		 * @param version Version string
		 * @return DataChannelTypeNamesDto, or nullptr if not found
		 */
		static std::optional<DataChannelTypeNamesDto> GetDataChannelTypeNames( const std::string& version );

		/**
		 * @brief Get format data types for specific version
		 * @param version Version string
		 * @return FormatDataTypesDto, or nullptr if not found
		 */
		static std::optional<FormatDataTypesDto> GetFormatDataTypes( const std::string& version );

		// private:
		/**
		 * @brief Get resource stream for specific resource name
		 * @param resourceName Resource name
		 * @return Memory stream containing resource data
		 */
		static std::shared_ptr<std::istream> GetStream( const std::string& resourceName );

		/**
		 * @brief Get all embedded resource names
		 * @return Array of resource names
		 */
		static std::vector<std::string> GetResourceNames();

		/**
		 * @brief Get decompressed stream for gzipped resource
		 * @param resourceName Compressed resource name
		 * @return Decompressed stream
		 */
		static std::shared_ptr<std::istream> GetDecompressedStream( const std::string& resourceName );
	};
}
