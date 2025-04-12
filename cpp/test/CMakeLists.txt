cmake_minimum_required(VERSION 3.24)

find_package(spdlog CONFIG REQUIRED)
find_package(GTest REQUIRED)
include(GoogleTest)

set(TESTS
	ImoNumberTests
	ISOStringTests
	CodebookTests
	CodebooksTests
	GmodTests
	LocalIdTests
)

foreach(TEST ${TESTS})
if(NOT TARGET ${TEST})
	add_executable(${TEST} ${TEST}.cpp)

	target_precompile_headers(${TEST} PRIVATE pch.h)

	target_link_libraries(${TEST} PRIVATE
		vista-sdk
		GTest::gtest_main
		GTest::gtest
		spdlog::spdlog_header_only
	)

	target_include_directories(${TEST} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

	add_test(
		NAME ${TEST}
		COMMAND ${TEST}
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/cpp/cmake/bin/$<CONFIG>"
	)
endif()
endforeach()

if(NOT TARGET CopyTestData)
  add_custom_target(CopyTestData ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	  ${CMAKE_CURRENT_SOURCE_DIR}/testdata ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/testdata
	COMMENT "Copying 'testdata' directory to the test execution directory"
  )
endif()
