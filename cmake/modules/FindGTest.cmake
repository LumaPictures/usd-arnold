# Simple module to find Google Test.

find_path(GTEST_INCLUDE_DIR gtest/gtest.h
          DOC "Google Test include directory")

find_library(GMOCK_LIBRARY libgmock.a)
find_library(GMOCK_MAIN_LIBRARY libgmock_main.a)
find_library(GTEST_LIBRARY libgtest.a)
find_library(GTEST_MAIN_LIBRARY libgtest_main.a)

set(GTEST_LIBRARIES "${GMOCK_LIBRARY};${GMOCK_MAIN_LIBRARY};${GTEST_LIBRARY};${GTEST_MAIN_LIBRARY}")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
        GTest
        REQUIRED_VARS
        GTEST_INCLUDE_DIR
        GMOCK_LIBRARY
        GMOCK_MAIN_LIBRARY
        GTEST_LIBRARY
        GTEST_MAIN_LIBRARY)
