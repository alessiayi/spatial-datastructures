# https://google.github.io/googletest/quickstart-cmake.html

cmake_minimum_required(VERSION 3.14)
project(spatial_implementation)

set(CMAKE_CXX_STANDARD 17)

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

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS geometry) 

include_directories(${Boost_INCLUDE_DIRS})

add_executable(
  quad_test
  quad_test.cpp
)

target_link_libraries(
  quad_test
  gtest_main
  ${Boost_LIBRARIES}
)

include(GoogleTest)
gtest_discover_tests(quad_test)
