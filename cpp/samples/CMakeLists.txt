#==============================================================================
# dnv::vista::sdk - Samples
#==============================================================================

if(DNV_VISTA_SDK_BUILD_C_API)
    add_subdirectory(c-api)
endif()

add_subdirectory(core)
add_subdirectory(query)
add_subdirectory(transport)
add_subdirectory(types)
add_subdirectory(utils)
