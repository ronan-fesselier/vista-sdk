#==============================================================================
# dnv::vista::sdk - C API samples
#==============================================================================

file(GLOB_RECURSE DNV_VISTA_SDK_C_API_SAMPLES_SOURCES CONFIGURE_DEPENDS "*.c")

foreach(source IN LISTS DNV_VISTA_SDK_C_API_SAMPLES_SOURCES)
    cmake_path(RELATIVE_PATH source BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" OUTPUT_VARIABLE source_rel)
    string(REPLACE "/" "-" source_id "${source_rel}")
    string(REPLACE ".c" "" source_id "${source_id}")
    set(target "dnv-vista-sdk-sample-c-api-${source_id}")

    add_executable(${target} ${source})

    set_target_properties(${target}
        PROPERTIES
            C_STANDARD          11
            C_STANDARD_REQUIRED ON
            DEBUG_POSTFIX       "-d"
    )

    target_link_libraries(${target}
        PRIVATE
            dnv::vista::sdk::c
    )
endforeach()
