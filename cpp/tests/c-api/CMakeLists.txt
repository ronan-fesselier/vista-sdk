#==============================================================================
# dnv::vista::sdk::c - Tests
#==============================================================================

if(NOT DNV_VISTA_SDK_BUILD_C_API)
    return()
endif()

file(GLOB_RECURSE DNV_VISTA_SDK_C_API_TESTS_SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(dnv-vista-sdk-tests-c-api ${DNV_VISTA_SDK_C_API_TESTS_SOURCES})

set_target_properties(dnv-vista-sdk-tests-c-api
    PROPERTIES
        CXX_STANDARD          20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS        OFF
        DEBUG_POSTFIX         "-d"
)

target_compile_definitions(dnv-vista-sdk-tests-c-api
    PRIVATE
        DOCTEST_CONFIG_USE_STD_HEADERS
)

target_link_libraries(dnv-vista-sdk-tests-c-api
    PRIVATE
        doctest::doctest_with_main
        dnv::vista::sdk::c
        dnv::vista::sdk::testdata
        dnv::vista::sdk::warnings
)

doctest_discover_tests(dnv-vista-sdk-tests-c-api)
