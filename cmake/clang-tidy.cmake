function(zuu_json_enable_clang_tidy target_name)
	if(NOT ZUU_JSON_ENABLE_SANITIZERS)
        return()
    endif()

	find_program(CLANG_TIDY_EXE NAMES clang-tidy)

	set_target_properties(${target_name} PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_CMD}"
    )
endfunction()