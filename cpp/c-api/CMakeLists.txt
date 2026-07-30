#==============================================================================
# dnv-vista-sdk-c - C API
#==============================================================================

#----------------------------------------------
# Library
#----------------------------------------------

add_library(dnv-vista-sdk-c
    src/core/codebook_name.cpp
    src/core/codebook.cpp
    src/core/codebooks.cpp
    src/core/metadata_tag.cpp
    src/core/vis.cpp

    src/common.cpp
    src/error.cpp
    src/version.cpp
)
add_library(dnv::vista::sdk::c ALIAS dnv-vista-sdk-c)

target_include_directories(dnv-vista-sdk-c
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

target_link_libraries(dnv-vista-sdk-c
    PRIVATE
        dnv::vista::sdk
)

target_compile_features(dnv-vista-sdk-c PRIVATE cxx_std_20)

#----------------------------------------------
# Export header
#----------------------------------------------

include(GenerateExportHeader)

generate_export_header(dnv-vista-sdk-c
    EXPORT_MACRO_NAME     DNV_VISTA_SDK_C_API
    EXPORT_FILE_NAME      "${CMAKE_CURRENT_BINARY_DIR}/include/dnv/vista/sdk/c/Export.h"
)

#----------------------------------------------
# Shared library properties
#----------------------------------------------

if(BUILD_SHARED_LIBS)
    set_target_properties(dnv-vista-sdk-c
        PROPERTIES
            CXX_VISIBILITY_PRESET     hidden
            VISIBILITY_INLINES_HIDDEN ON
            VERSION                   ${PROJECT_VERSION}
            SOVERSION                 ${PROJECT_VERSION_MAJOR}
    )

    if(WIN32)
        set(DNV_VISTA_SDK_RC_FILE_DESCRIPTION  "Vista SDK C API - C wrapper for the DNV Vista SDK C++ library")
        set(DNV_VISTA_SDK_RC_INTERNAL_NAME     "dnv-vista-sdk-c")
        set(DNV_VISTA_SDK_RC_ORIGINAL_FILENAME "dnv-vista-sdk-c.dll")

        configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/dnvVistaSdk.rc.in"
            "${CMAKE_CURRENT_BINARY_DIR}/dnvVistaSdkC.rc"
            @ONLY
        )

        target_sources(dnv-vista-sdk-c
            PRIVATE
                "${CMAKE_CURRENT_BINARY_DIR}/dnvVistaSdkC.rc"
        )
    endif()
endif()
