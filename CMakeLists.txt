cmake_minimum_required(VERSION 3.14)

project(rtaudio-test LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.11)

add_subdirectory(external/rtaudio)

add_executable(rtaudio-test main.cpp)
target_include_directories(rtaudio-test
    PRIVATE external/rtaudio)
target_link_libraries(rtaudio-test
    PRIVATE rtaudio)
