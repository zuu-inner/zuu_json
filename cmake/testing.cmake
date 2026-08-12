function(add_test_target type_test target_name src_dir)
	if(ZUU_JSON_BUILD_TESTS)
		file(GLOB TEST_SRCS
			CONFIGURE_DEPENDS
			"${CMAKE_CURRENT_SOURCE_DIR}/tests/${type_test}/${src_dir}/*.cpp"
		)

		add_executable(zuu_${target_name}
			"${CMAKE_SOURCE_DIR}/tests/${type_test}/${target_name}.cpp"
			"${TEST_SRCS}"
		)

		target_link_libraries(zuu_${target_name} PRIVATE
			zuu_json::zuu_json
			GTest::gtest
		)
		target_include_directories(zuu_${target_name} PRIVATE
			"${CMAKE_SOURCE_DIR}/include"
			"${CMAKE_SOURCE_DIR}/internal"
			"${CMAKE_SOURCE_DIR}/tests/${type_test}/${src_dir}"
		)

		zuu_json_enable_warnings(zuu_${target_name})
		zuu_json_enable_sanitizers(zuu_${target_name})
		zuu_json_enable_coverage(zuu_${target_name})
		zuu_json_enable_clang_tidy(zuu_${target_name})

		add_test(NAME zuu_${target_name} COMMAND zuu_${target_name})
	endif()
endfunction()