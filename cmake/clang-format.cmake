find_program(CLANG_FORMAT_EXE NAMES clang-format)

if(CLANG_FORMAT_EXE AND ZUU_JSON_ENABLE_CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCE_FILES CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/benchmarks/*.cpp"
        "${CMAKE_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_SOURCE_DIR}/internal/*.hpp"
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/tests/fuzz/*.cpp"
        "${CMAKE_SOURCE_DIR}/tests/integration/*.cpp"
        "${CMAKE_SOURCE_DIR}/tests/unit/*.cpp"
        "${CMAKE_SOURCE_DIR}/example/*.cpp"
    )

    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT_EXE} --dry-run --Werror -style=file ${ALL_SOURCE_FILES}
        COMMENT "Checking source formatting"
    )

    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${ALL_SOURCE_FILES}
        COMMENT "Formatting source files with clang-format"
    )
endif()

function(zuu_json_enable_clang_format target_name)
	if(NOT ZUU_JSON_ENABLE_CLANG_FORMAT)
        return()
    endif()

    if(CLANG_FORMAT_EXE)
        set_target_properties(${target_name} PROPERTIES
            CXX_CLANG_FORMAT "${CLANG_FORMAT_EXE}"
        )
    endif()
endfunction()
