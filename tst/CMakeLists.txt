PROJECT(${CMAKE_PROJECT_NAME}_tst)

file(GLOB_RECURSE TEST_FILES LIST_DIRECTORIES false *.h *.cpp)

set(TEST_SOURCE ${TEST_FILES})
SET (TEST_LIBRARIES ${PROJECT_NAME}_LINKED_LIST)


include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


mark_as_advanced(
        BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
        gmock_build_tests gtest_build_samples gtest_build_tests
        gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)


ADD_TEST(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
# add_executable(${PROJECT_NAME} ${TEST_FILES})
