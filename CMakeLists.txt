cmake_minimum_required(VERSION 2.8)
project(spatial_implementation)

set(CMAKE_CXX_STANDARD 11)

if (NOT TARGET gtest_main)
    add_subdirectory(googletest)
endif ()

include_directories(${gtest_SOURCE_DIR}/include ${gmock_SOURCE_DIR}/include)

# enable_testing()

add_executable(
  quad_test
  quad_test.cpp
)

target_link_libraries(
  quad_test
  gtest_main
  gtest
  gmock
)