#pragma once

#include "CodebooksDto.h"
#include "GmodVersioning.h"
#include "Codebooks.h"
#include "Gmod.h"

namespace dnv::vista::sdk
{
	class GmodNode;
	class GmodPath;
	class LocalId;
	class LocalIdBuilder;
	class Locations;

	enum class VisVersion;

	struct GmodDto;
	struct GmodVersioningDto;
	struct LocationsDto;

	/**
	 * @brief Interface for VIS (Vessel Information Structure) operations
	 */
	class IVIS
	{
	public:
		virtual ~IVIS() = default;

		/**
		 * @brief Get the GMOD for a specific VIS version
		 * @param visVersion The VIS version
		 * @return The GMOD (Generic Model) for the specified version
		 */
		virtual Gmod GetGmod( VisVersion visVersion ) const = 0;

		/**
		 * @brief Get the codebooks for a specific VIS version
		 * @param visversion The VIS version
		 * @return The codebooks for the specified version
		 */
		virtual Codebooks GetCodebooks( VisVersion visversion ) = 0;

		/**
		 * @brief Get the locations for a specific VIS version
		 * @param visversion The VIS version
		 * @return The locations for the specified version
		 */
		virtual Locations GetLocations( VisVersion visversion ) = 0;

		/**
		 * @brief Get a map of codebooks for multiple VIS versions
		 * @param visVersions The VIS versions to retrieve codebooks for
		 * @return Map of VIS versions to their corresponding codebooks
		 */
		virtual std::unordered_map<VisVersion, Codebooks> GetCodebooksMap(
			const std::vector<VisVersion>& visVersions ) = 0;

		/**
		 * @brief Get a map of GMODs for multiple VIS versions
		 * @param visVersions The VIS versions to retrieve GMODs for
		 * @return Map of VIS versions to their corresponding GMODs
		 */
		virtual std::unordered_map<VisVersion, Gmod> GetGmodsMap(
			const std::vector<VisVersion>& visVersions ) = 0;

		/**
		 * @brief Get a map of locations for multiple VIS versions
		 * @param visVersions The VIS versions to retrieve locations for
		 * @return Map of VIS versions to their corresponding locations
		 */
		virtual std::unordered_map<VisVersion, Locations> GetLocationsMap(
			const std::vector<VisVersion>& visVersions ) = 0;

		/**
		 * @brief Get all available VIS versions
		 * @return Vector of all supported VIS versions
		 */
		virtual std::vector<VisVersion> GetVisVersions() = 0;

		/**
		 * @brief Convert a GMOD node from one VIS version to another
		 * @param sourceVersion The source VIS version
		 * @param sourceNode The node to convert
		 * @param targetVersion The target VIS version
		 * @return The converted node, or none if conversion is not possible
		 */
		virtual std::optional<GmodNode> ConvertNode(
			VisVersion sourceVersion, const GmodNode& sourceNode, VisVersion targetVersion ) = 0;

		/**
		 * @brief Convert a GMOD path from one VIS version to another
		 * @param sourceVersion The source VIS version
		 * @param sourcePath The path to convert
		 * @param targetVersion The target VIS version
		 * @return The converted path, or none if conversion is not possible
		 */
		virtual std::optional<GmodPath> ConvertPath(
			VisVersion sourceVersion, const GmodPath& sourcePath, VisVersion targetVersion ) = 0;
	};

	/**
	 * @brief Implementation of the IVIS interface for Vessel Information Structure operations
	 */
	class VIS : public IVIS
	{
	public:
		/**
		 * @brief The latest supported VIS version
		 */
		static const VisVersion LatestVisVersion;

		/**
		 * @brief Get the singleton instance of VIS
		 * @return Reference to the singleton instance
		 */
		static VIS& Instance();

		/**
		 * @brief Constructor
		 */
		VIS();

		/**
		 * @brief Virtual destructor
		 */
		virtual ~VIS() = default;

		/**
		 * @brief Get the GMOD for a specific VIS version
		 * @param visVersion The VIS version
		 * @return The GMOD for the specified version
		 * @throws std::invalid_argument If the VIS version is invalid
		 */
		Gmod GetGmod( VisVersion visVersion ) const override;

		/**
		 * @brief Get the codebooks for a specific VIS version
		 * @param visversion The VIS version
		 * @return The codebooks for the specified version
		 */
		Codebooks GetCodebooks( VisVersion visversion ) override;

		/**
		 * @brief Get the locations for a specific VIS version
		 * @param visversion The VIS version
		 * @return The locations for the specified version
		 */
		Locations GetLocations( VisVersion visversion ) override;

		/**
		 * @brief Get a map of codebooks for multiple VIS versions
		 * @param visVersions The VIS versions to retrieve codebooks for
		 * @return Map of VIS versions to their corresponding codebooks
		 * @throws std::invalid_argument If any VIS version is invalid
		 */
		std::unordered_map<VisVersion, Codebooks> GetCodebooksMap(
			const std::vector<VisVersion>& visVersions ) override;

		/**
		 * @brief Get a map of GMODs for multiple VIS versions
		 * @param visVersions The VIS versions to retrieve GMODs for
		 * @return Map of VIS versions to their corresponding GMODs
		 * @throws std::invalid_argument If any VIS version is invalid
		 */
		std::unordered_map<VisVersion, Gmod> GetGmodsMap(
			const std::vector<VisVersion>& visVersions ) override;

		/**
		 * @brief Get a map of locations for multiple VIS versions
		 * @param visVersions The VIS versions to retrieve locations for
		 * @return Map of VIS versions to their corresponding locations
		 * @throws std::invalid_argument If any VIS version is invalid
		 */
		std::unordered_map<VisVersion, Locations> GetLocationsMap(
			const std::vector<VisVersion>& visVersions ) override;

		/**
		 * @brief Get all available VIS versions
		 * @return Vector of all supported VIS versions
		 */
		std::vector<VisVersion> GetVisVersions() override;

		/**
		 * @brief Convert a GMOD node from one VIS version to another
		 * @param sourceVersion The source VIS version
		 * @param sourceNode The node to convert
		 * @param targetVersion The target VIS version
		 * @return The converted node, or none if conversion is not possible
		 */
		std::optional<GmodNode> ConvertNode(
			VisVersion sourceVersion, const GmodNode& sourceNode, VisVersion targetVersion ) override;

		/**
		 * @brief Convert a GMOD path from one VIS version to another
		 * @param sourceVersion The source VIS version
		 * @param sourcePath The path to convert
		 * @param targetVersion The target VIS version
		 * @return The converted path, or none if conversion is not possible
		 */
		std::optional<GmodPath> ConvertPath(
			VisVersion sourceVersion, const GmodPath& sourcePath, VisVersion targetVersion ) override;

		/**
		 * @brief Convert a GMOD node to a different VIS version
		 * @param sourceNode The node to convert
		 * @param targetVersion The target VIS version
		 * @param sourceParent Optional parent node for context
		 * @return The converted node, or none if conversion is not possible
		 */
		std::optional<GmodNode> ConvertNode(
			const GmodNode& sourceNode, VisVersion targetVersion, const GmodNode* sourceParent = nullptr );

		/**
		 * @brief Convert a GMOD path to a different VIS version
		 * @param sourcePath The path to convert
		 * @param targetVersion The target VIS version
		 * @return The converted path, or none if conversion is not possible
		 */
		std::optional<GmodPath> ConvertPath(
			const GmodPath& sourcePath, VisVersion targetVersion );

		/**
		 * @brief Convert a LocalIdBuilder to a different VIS version
		 * @param sourceLocalId The LocalIdBuilder to convert
		 * @param targetVersion The target VIS version
		 * @return The converted LocalIdBuilder, or none if conversion is not possible
		 */
		std::optional<LocalIdBuilder> ConvertLocalId(
			const LocalIdBuilder& sourceLocalId, VisVersion targetVersion );

		/**
		 * @brief Convert a LocalId to a different VIS version
		 * @param sourceLocalId The LocalId to convert
		 * @param targetVersion The target VIS version
		 * @return The converted LocalId, or none if conversion is not possible
		 */
		std::optional<LocalId> ConvertLocalId(
			const LocalId& sourceLocalId, VisVersion targetVersion );

		/**
		 * @brief Check if a string matches ISO local ID format
		 * @param value The string to check
		 * @return True if the string matches ISO local ID format
		 */
		static bool MatchISOLocalIdString( const std::string& value );

		/**
		 * @brief Check if a character is valid for ISO strings
		 * @param c The character to check
		 * @return True if the character is valid for ISO strings
		 */
		static bool IsISOString( char c );

		/**
		 * @brief Check if a string contains only valid ISO characters
		 * @param value The string to check
		 * @return True if the string contains only valid ISO characters
		 */
		static bool IsISOString( const std::string& value );

		/**
		 * @brief Check if a string view contains only valid ISO characters
		 * @param value The string view to check
		 * @return True if the string view contains only valid ISO characters
		 */
		static bool IsISOString( const std::string_view value );

		/**
		 * @brief Check if a string is a valid ISO local ID string
		 * @param value The string to check
		 * @return True if the string is a valid ISO local ID string
		 */
		static bool IsISOLocalIdString( const std::string& value );

	private:
		/**
		 * @brief Simple template-based cache implementation
		 * @tparam K The key type
		 * @tparam V The value type
		 */
		template <typename K, typename V>
		class Cache
		{
		public:
			/**
			 * @brief Get a value from cache or create it if not present
			 * @param key The cache key
			 * @param factory Function to create the value if not in cache
			 * @return Reference to the cached value
			 */
			V& GetOrCreate( const K& key, std::function<V()> factory ) const;

		private:
			mutable std::unordered_map<K, V> m_cache;
		};

		Cache<VisVersion, GmodDto> m_gmodDtoCache;
		Cache<VisVersion, Gmod> m_gmodCache;
		Cache<VisVersion, CodebooksDto> m_codebooksDtoCache;
		Cache<VisVersion, Codebooks> m_codebooksCache;
		Cache<VisVersion, LocationsDto> m_locationsDtoCache;
		Cache<VisVersion, Locations> m_locationsCache;
		Cache<std::string, std::unordered_map<std::string, GmodVersioningDto>> m_gmodVersioningDtoCache;
		Cache<std::string, GmodVersioning> m_gmodVersioningCache;
		static const std::string m_versioning;

		/**
		 * @brief Get the GMOD DTO for a specific VIS version
		 * @param visVersion The VIS version
		 * @return The GMOD DTO
		 * @throws std::runtime_error If the GMOD cannot be loaded
		 */
		GmodDto GetGmodDto( VisVersion visVersion ) const;

		/**
		 * @brief Load the GMOD DTO for a specific VIS version
		 * @param visVersion The VIS version
		 * @return The GMOD DTO, or none if it cannot be loaded
		 */
		static std::optional<GmodDto> LoadGmodDto( VisVersion visVersion );

		/**
		 * @brief Get the GMOD versioning DTOs
		 * @return Map of versioning DTOs
		 * @throws std::invalid_argument If the versioning data cannot be loaded
		 */
		std::unordered_map<std::string, GmodVersioningDto> GetGmodVersioningDto();

		/**
		 * @brief Get the GMOD versioning
		 * @return The GMOD versioning object
		 */
		GmodVersioning GetGmodVersioning();

		/**
		 * @brief Get the codebooks DTO for a specific VIS version
		 * @param visVersion The VIS version
		 * @return The codebooks DTO
		 * @throws std::runtime_error If the codebooks cannot be loaded
		 */
		CodebooksDto GetCodebooksDto( VisVersion visVersion );

		/**
		 * @brief Get the locations DTO for a specific VIS version
		 * @param visVersion The VIS version
		 * @return The locations DTO
		 * @throws std::runtime_error If the locations cannot be loaded
		 */
		LocationsDto GetLocationsDto( VisVersion visVersion );

		/**
		 * @brief Check if a character code is valid for ISO strings
		 * @param code The ASCII code to check
		 * @return True if the code is valid for ISO strings
		 */
		static bool MatchAsciiDecimal( int code );
	};
}
