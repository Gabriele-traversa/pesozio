# cmake per main.cpp
# cmake_minimum_required(VERSION 3.28)
# project(CMakeSFMLProject LANGUAGES CXX)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# include(FetchContent)
# FetchContent_Declare(SFML
#     GIT_REPOSITORY https://github.com/SFML/SFML.git
#     GIT_TAG 3.0.2
#     GIT_SHALLOW ON
#     EXCLUDE_FROM_ALL
#     SYSTEM)
# FetchContent_MakeAvailable(SFML)

# add_executable(main src/main_text_echo.cpp
#     src/main_text_editor.cpp )
# target_compile_features(main PRIVATE cxx_std_17)
# target_link_libraries(main PRIVATE SFML::Graphics)

# set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)

cmake_minimum_required(VERSION 4.0)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(main_text_echo src/main_text_echo.cpp)
target_link_libraries(main_text_echo PRIVATE SFML::Graphics)

add_executable(main_text_editor src/main_text_editor.cpp)
target_link_libraries(main_text_editor PRIVATE SFML::Graphics)

