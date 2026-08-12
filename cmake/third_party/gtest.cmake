include(CTest)

if(ZUU_JSON_BUILD_TESTS)
    enable_testing()

    find_package(GTest CONFIG QUIET)
    if(NOT GTest_FOUND AND NOT TARGET GTest::gtest AND NOT TARGET gtest)
        include(FetchContent)
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        v1.15.2
        )
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()

    if(TARGET gtest AND NOT TARGET GTest::gtest)
        add_library(GTest::gtest ALIAS gtest)
    endif()
endif()