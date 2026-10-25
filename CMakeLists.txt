cmake_minimum_required(VERSION 3.28)

project(SlidingNumberPuzzle LANGUAGES CXX)

option(BUILD_SFML_GUI "Build the SFML graphical version" ON)

add_executable(SlidingNumberPuzzleConsole
    NumberSlideGame.cpp
)
target_compile_features(SlidingNumberPuzzleConsole PRIVATE cxx_std_17)

if(BUILD_SFML_GUI)
    add_executable(SlidingNumberGame
        SFMLApp.cpp
    )
    target_compile_features(SlidingNumberGame PRIVATE cxx_std_17)

    include(FetchContent)

    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.1.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
    )

    FetchContent_MakeAvailable(SFML)
    target_link_libraries(SlidingNumberGame PRIVATE SFML::Graphics)
endif()