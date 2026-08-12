function(zuu_json_enable_clang_tidy target_name)
	if(NOT ZUU_JSON_ENABLE_CLANG_TIDY)
        return()
    endif()

	find_program(CLANG_TIDY_EXE NAMES clang-tidy)
    if(CLANG_TIDY_EXE)
	    set_target_properties(${target_name} PROPERTIES
            CXX_CLANG_TIDY "${CLANG_TIDY_EXE}"
        )
    endif()
endfunction()