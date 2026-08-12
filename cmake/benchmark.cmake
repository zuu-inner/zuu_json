function(add_benchmark_target target_name src_dir)
    if(NOT ZUU_JSON_BUILD_BENCHMARKS)
        return()
    endif()

    file(GLOB BM_SRCS
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/${src_dir}/*.cpp"
    )

    add_executable(zuu_${target_name}
        "${CMAKE_SOURCE_DIR}/benchmarks/${target_name}.cpp"
        ${BM_SRCS}
    )

    target_link_libraries(zuu_${target_name} PRIVATE
        zuu_json::zuu_json
        benchmark::benchmark
    )

    target_include_directories(zuu_${target_name} PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/internal"
        "${CMAKE_SOURCE_DIR}/benchmarks/${src_dir}"
    )

    target_compile_definitions(zuu_${target_name} PRIVATE
        ZUU_JSON_SAMPLES_DIR="${CMAKE_SOURCE_DIR}/assets/samples"
    )

    zuu_json_enable_warning(zuu_${target_name})
    zuu_json_enable_release_optimization(zuu_${target_name})
    zuu_json_enable_clang_tidy(zuu_${target_name})

	string(TIMESTAMP BM_TIME "%Y%m%d%H%M%S")

    add_custom_target(run_${target_name}
        COMMAND
            $<TARGET_FILE:zuu_${target_name}>
            --benchmark_min_warmup_time=2
            --benchmark_out=${CMAKE_SOURCE_DIR}/out/results/${src_dir}_${BM_TIME}.json
            --benchmark_out_format=json
        DEPENDS zuu_${target_name}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running benchmark: ${target_name}"
        VERBATIM
    )
endfunction()