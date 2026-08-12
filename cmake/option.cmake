option(ZUU_JSON_BUILD_TESTS "Build unit tests" OFF)
option(ZUU_JSON_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(ZUU_JSON_BUILD_EXAMPLES "Build examples" OFF)
option(ZUU_JSON_BUILD_FUZZERS "Build libFuzzer-based fuzz targets (requires Clang)" OFF)

option(ZUU_JSON_ENABLE_CLANG_FORMAT "Enable clang-format target" OFF)
option(ZUU_JSON_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
option(ZUU_JSON_ENABLE_DOXYGEN "Enable doxygen" OFF)

option(ZUU_JSON_ENABLE_SANITIZERS "Enable sanitizers" OFF)
option(ZUU_JSON_ENABLE_COVERAGE "Enable coverage" OFF)

option(ZUU_JSON_ENABLE_IPO "Enable interprocedural optimization" ON)
option(ZUU_JSON_ENABLE_NATIVE_OPTIMIZATION "Enable native CPU optimizations" OFF)
option(ZUU_JSON_ENABLE_AVX2 "Enable AVX2 in release" OFF)

if(WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_definitions(_DISABLE_STRING_ANNOTATION _DISABLE_VECTOR_ANNOTATION)
endif()