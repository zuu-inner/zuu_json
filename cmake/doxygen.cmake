if(ZUU_JSON_ENABLE_DOXYGEN)
    find_program(DOXYGEN_EXE NAMES doxygen)

    if(DOXYGEN_EXE)
		add_custom_target(doxygen-docs
			COMMAND ${DOXYGEN_EXE} ${DOXYFILE_OUT}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen"
			VERBATIM
		)
    endif()
endif()