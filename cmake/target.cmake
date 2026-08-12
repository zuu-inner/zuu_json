set(ZUU_JSON_LIBRARY_TARGET zuu_json)

add_library(${ZUU_JSON_LIBRARY_TARGET})

target_sources(${ZUU_JSON_LIBRARY_TARGET}
    PRIVATE
        # will filled
)

target_include_directories(${ZUU_JSON_LIBRARY_TARGET}
    PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/internal"
)

target_compile_features(${ZUU_JSON_LIBRARY_TARGET} PUBLIC cxx_std_23)

zuu_json_enable_warning(${ZUU_JSON_LIBRARY_TARGET})
zuu_json_enable_release_optimization(${ZUU_JSON_LIBRARY_TARGET})
zuu_json_enable_sanitizer(${ZUU_JSON_LIBRARY_TARGET})
zuu_json_enable_coverage(${ZUU_JSON_LIBRARY_TARGET})

if(ZUU_JSON_BUILD_FUZZERS)
    target_compile_options(${ZUU_JSON_LIBRARY_TARGET} PRIVATE
        -fsanitize-coverage=inline-8bit-counters,pc-table,trace-cmp
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )
endif()

if(CLANG_TIDY_EXE)
    set_target_properties(${ZUU_JSON_LIBRARY_TARGET} PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_CMD}"
    )
endif()

add_library(zuu_json::zuu_json ALIAS ${ZUU_JSON_LIBRARY_TARGET})