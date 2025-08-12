# ==============================================================================
# VISTA-SDK-CPP - C++ Library CMake Sources
# ==============================================================================

set(PUBLIC_HEADERS
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Config/AlgorithmConstants.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Config/CodebookConstants.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Config/DateTimeConstants.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Config/Decimal128Constants.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Config/DtoKeysConstants.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Config/GmodConstants.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Config/ISO19848Constants.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Config/LocalIdConstants.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Config/LocationsConstants.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Config/Platform.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Config/UniversalIdConstants.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Config/version.h

	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/DataTypes/DateTimeISO8601.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/DataTypes/DateTimeISO8601.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/DataTypes/Decimal128.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/DataTypes/Decimal128.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/DataTypes/Int128.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/DataTypes/Int128.inl

	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Internal/HashMap.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Internal/MemoryCache.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Internal/PathConversionCache.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Internal/ChdDictionary.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Internal/ChdDictionary.inl

	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Transport/ISO19848.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Transport/ISO19848.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Transport/ISO19848Dtos.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Transport/ISO19848Dtos.inl

	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Utils/Hashing.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Utils/StringBuilderPool.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Utils/StringMap.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Utils/StringUtils.h

	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Codebook.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Codebook.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Codebooks.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Codebooks.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/CodebookName.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/CodebookName.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/CodebooksDto.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/CodebooksDto.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/EmbeddedResource.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/EmbeddedResource.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Exceptions.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Gmod.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Gmod.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/GmodDto.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/GmodDto.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/GmodNode.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/GmodNode.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/GmodPath.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/GmodPath.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/GmodVersioning.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/GmodVersioning.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/GmodVersioningDto.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/GmodVersioningDto.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/ImoNumber.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/ImoNumber.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/LocalId.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/LocalId.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/LocalIdBuilder.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/LocalIdItems.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/LocalIdItems.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/LocalIdParsingErrorBuilder.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/LocalIdParsingErrorBuilder.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/LocationBuilder.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/LocationBuilder.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/LocationParsingErrorBuilder.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/LocationParsingErrorBuilder.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Locations.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/Locations.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/LocationsDto.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/LocationsDto.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/MetadataTag.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/MetadataTag.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/ParsingErrors.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/ParsingErrors.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/UniversalIdBuilder.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/UniversalIdBuilder.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/UniversalId.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/UniversalId.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/VIS.h
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/VIS.inl
	${VISTA_SDK_CPP_INCLUDE_DIR}/dnv/vista/sdk/VISVersion.h
)

set(PRIVATE_HEADERS
)

set(PRIVATE_SOURCES
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/DataTypes/DateTimeISO8601.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/DataTypes/Decimal128.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/DataTypes/Int128.cpp

	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/Transport/ISO19848.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/Transport/ISO19848Dtos.cpp

	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/Codebook.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/Codebooks.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/CodebooksDto.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/EmbeddedResource.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/Gmod.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/GmodDto.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/GmodNode.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/GmodPath.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/GmodVersioning.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/GmodVersioningDto.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/ImoNumber.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/LocalId.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/LocalIdBuilder.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/LocalIdParsingErrorBuilder.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/LocationBuilder.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/LocationParsingErrorBuilder.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/Locations.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/LocationsDto.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/MetadataTag.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/ParsingErrors.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/UniversalIdBuilder.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/UniversalId.cpp
	${VISTA_SDK_CPP_SOURCE_DIR}/dnv/vista/sdk/VIS.cpp
)
