cmake_minimum_required(VERSION 3.20)
project(
	zuu_json
	VERSION 0.2.0
	LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(cmake/credit.cmake)
include(cmake/variable.cmake)
include(cmake/option.cmake)
include(cmake/config.cmake)
include(cmake/output.cmake)
include(cmake/generator.cmake)

include(cmake/warning.cmake)
include(cmake/optimization.cmake)
include(cmake/sanitizer.cmake)
include(cmake/coverage.cmake)

include(cmake/clang-format.cmake)

include(cmake/clang-tidy.cmake)

if(ZUU_JSON_ENABLE_DOXYGEN)
    include(cmake/doxyfile.cmake)
endif()

if(ZUU_JSON_BUILD_TESTS)
    include(cmake/third_party/gtest.cmake)
    include(cmake/testing.cmake)
endif()

if(ZUU_JSON_BUILD_BENCHMARKS)
    include(cmake/third_party/benchmark.cmake)
    include(cmake/benchmark.cmake)
endif()

if(ZUU_JSON_BUILD_EXAMPLES)
    include(cmake/example.cmake)
endif()

if(ZUU_JSON_BUILD_FUZZERS)
    include(cmake/fuzzing.cmake)
endif()

include(cmake/target.cmake)