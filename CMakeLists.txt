# https://google.github.io/googletest/quickstart-cmake.html

cmake_minimum_required(VERSION 3.14)
project(spatial_implementation)

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(
  googletest)

enable_testing()

add_executable(
  quad_test
  quad_test.cpp
)

target_include_directories(quad_test "/usr/local/include")

target_link_libraries(
  quad_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(quad_test)
