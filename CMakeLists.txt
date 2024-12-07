# MaizeMix

cmake_minimum_required(VERSION 3.27)
project(MaizeMix)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(MIX_BUILD_SHARED_LIBS "Build Maize Mix as a shared library" OFF)
option(MIX_BUILD_TEST "Build test project" OFF)
option(MIX_BUILD_SANDBOX "Build sandbox project" OFF)

# import sfml
FetchContent_Declare(sfml GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.6.1)

if (MIX_BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON)
endif ()

if (MIX_BUILD_SANDBOX)
    set(SFML_BUILD_WINDOW ON)
    set(SFML_BUILD_GRAPHICS ON)
    set(SFML_BUILD_NETWORK OFF)
else ()
    set(SFML_BUILD_WINDOW OFF)
    set(SFML_BUILD_GRAPHICS OFF)
    set(SFML_BUILD_NETWORK OFF)
endif ()

set(SFML_BUILD_AUDIO ON)

FetchContent_MakeAvailable(sfml)

# build it as a static or shared library
if (MIX_BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
else()
    set(LIB_TYPE STATIC)
endif()

add_library(MaizeMix ${LIB_TYPE}
        src/MaizeMix/Helper/AudioClips/Clip.h
        src/MaizeMix/Helper/AudioSpecification.h
        src/MaizeMix/Helper/AudioManager.cpp
        src/MaizeMix/Helper/AudioManager.h
        src/MaizeMix/Helper/AudioClips/SoundBuffer.cpp
        src/MaizeMix/Helper/AudioClips/SoundBuffer.h
        src/MaizeMix/Helper/AudioClips/SoundReference.h
        src/MaizeMix/Helper/AudioClips/SoundReference.cpp
        src/MaizeMix/Helper/Music.cpp
        src/MaizeMix/Helper/Music.h
        src/MaizeMix/AudioClip.cpp
        src/MaizeMix/AudioClip.h
        src/MaizeMix/AudioEngine.cpp
        src/MaizeMix/AudioEngine.h
        src/MaizeMix.h
)

target_include_directories(MaizeMix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(MaizeMix PUBLIC sfml-audio)

# add test directory if tests are enabled
if (NOT MIX_BUILD_SHARED_LIBS AND MIX_BUILD_TEST)
    add_subdirectory(test)
endif()

# add sandbox directory if sandbox are enabled
if (NOT MIX_BUILD_SHARED_LIBS AND MIX_BUILD_SANDBOX)
    add_subdirectory(sandbox)
endif()
