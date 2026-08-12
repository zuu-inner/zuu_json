function(add_test_target type_test src_dir)
	if(ZUU_JSON_BUILD_TESTS)
		file(GLOB TEST_SRCS
			CONFIGURE_DEPENDS
			"${CMAKE_CURRENT_SOURCE_DIR}/tests/${type_test}/${src_dir}/*.cpp"
		)

		add_executable(zuu_${src_dir}
			"${TEST_SRCS}"
		)

		target_link_libraries(zuu_${src_dir} PRIVATE
			zuu_json::zuu_json
			GTest::gtest
			GTest::gtest_main
		)
		target_include_directories(zuu_${src_dir} PRIVATE
			"${CMAKE_SOURCE_DIR}/include"
			"${CMAKE_SOURCE_DIR}/internal"
			"${CMAKE_SOURCE_DIR}/tests/${type_test}/${src_dir}"
		)

		zuu_json_enable_warning(zuu_${src_dir})
		zuu_json_enable_sanitizer(zuu_${src_dir})
		zuu_json_enable_coverage(zuu_${src_dir})
		zuu_json_enable_clang_tidy(zuu_${src_dir})

		add_test(NAME zuu_${src_dir} COMMAND zuu_${src_dir})
	endif()
endfunction()