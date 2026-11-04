cmake_minimum_required(VERSION 3.20)

project(
    HangmanGame
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(
    HangmanCore
    STATIC

    core/HangmanGame.cpp
    core/WordBank.cpp
)

add_executable(
    HangmanTests

    tests/TestMain.cpp
    tests/ResultTest.cpp
    tests/WordBankTest.cpp
    tests/HangmanGameTest.cpp
    tests/TerminalUITest.cpp

    ui/TerminalUI.cpp
)

add_executable(
    HangmanApp

    main.cpp
    app/HangmanApp.cpp

    ui/TerminalUI.cpp
)

enable_testing()

target_link_libraries(
    HangmanApp
    PRIVATE
    HangmanCore
)

target_link_libraries(
    HangmanTests
    PRIVATE
    HangmanCore
)

target_include_directories(
    HangmanTests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/vendor
)

include(tests/vendor/Catch.cmake)

catch_discover_tests(
    HangmanTests
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    PROPERTIES TIMEOUT 5
)

file(
    COPY hangman_data
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)


target_include_directories(HangmanCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


target_compile_features(
    HangmanCore
    PUBLIC
    cxx_std_23
)

target_compile_options(
    HangmanApp
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

target_compile_options(
    HangmanCore
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
