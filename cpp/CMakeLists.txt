add_subdirectory(cmake)

if(VISTA_SDK_COPY_RESOURCES AND TARGET CopyResources)
	message(STATUS "Resources will be copied to ${VISTA_SDK_OUTPUT_DIR}/resources")
endif()

if(VISTA_SDK_BUILD_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()
