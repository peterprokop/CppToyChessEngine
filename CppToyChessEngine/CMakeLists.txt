cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(CppToyChessEngine)
add_executable(
  chess
  main.cpp
  PieceType.cpp
  Move.cpp
  Coordinate.cpp
  ChessBoardHelpers.cpp
)

# Gtest stuff is from https://google.github.io/googletest/quickstart-cmake.html
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  chess_tests
  ChessTests.cpp
  PieceType.cpp
  Move.cpp
  Coordinate.cpp
  ChessBoardHelpers.cpp
)
target_link_libraries(
  chess_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(chess_tests)
