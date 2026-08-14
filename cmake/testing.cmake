function(add_test_target type_test src_dir)
	if(ZUU_JSON_BUILD_TESTS)
		file(GLOB TEST_SRCS
			CONFIGURE_DEPENDS
			"${CMAKE_CURRENT_SOURCE_DIR}/tests/${type_test}/${src_dir}/*.cpp"
		)

		string(REPLACE "/" "_" target_name "zuu_${src_dir}")

		add_executable(${target_name}
			"${TEST_SRCS}"
		)

		target_link_libraries(${target_name} PRIVATE
			zuu_json::zuu_json
			GTest::gtest
			GTest::gtest_main
		)
		target_include_directories(${target_name} PRIVATE
			"${CMAKE_SOURCE_DIR}/include"
			"${CMAKE_SOURCE_DIR}/internal"
			"${CMAKE_SOURCE_DIR}/tests/${type_test}/${src_dir}"
		)

		zuu_json_enable_warning(${target_name})
		zuu_json_enable_sanitizer(${target_name})
		zuu_json_enable_coverage(${target_name})
		zuu_json_enable_clang_tidy(${target_name})

		add_test(NAME ${target_name} COMMAND ${target_name})
	endif()
endfunction()