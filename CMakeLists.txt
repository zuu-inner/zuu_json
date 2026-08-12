cmake_minimum_required(VERSION 3.20)
project(
	zuu_json
	VERSION 0.1.0
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

include(cmake/target.cmake)

if(ZUU_JSON_ENABLE_CLANG_FORMAT)
    include(cmake/clang-format.cmake)
endif()

if(ZUU_JSON_ENABLE_CLANG_TIDY)
    include(cmake/clang-tidy.cmake)
endif()

if(ZUU_JSON_ENABLE_DOXYGEN)
    include(cmake/doxygen.cmake)
endif()

if(ZUU_JSON_BUILD_TESTS)
    include(cmake/testing.cmake)
endif()

if(ZUU_JSON_BUILD_BENCHMARKS)
    include(cmake/benchmark.cmake)
endif()

if(ZUU_JSON_BUILD_EXAMPLES)
    include(cmake/example.cmake)
endif()

if(ZUU_JSON_BUILD_FUZZERS)
    include(cmake/fuzzing.cmake)
endif()