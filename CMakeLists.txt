cmake_minimum_required(VERSION 3.14)
project(SpaceBattle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_library(space_battle src/Move.cpp src/Rotate.cpp)
target_include_directories(space_battle PUBLIC src)

enable_testing()
add_executable(space_battle_tests tests/test_move_rotate.cpp)
target_link_libraries(space_battle_tests space_battle GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(space_battle_tests)