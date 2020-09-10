# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

cmake_minimum_required(VERSION 3.8)
project(MISSILE_COMMAND)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "PredefinedTargets")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

set(CMAKE_CONFIGURATION_TYPES 
    Release 
    Debug 
    CACHE TYPE INTERNAL FORCE )

set(SOURCE_PROJECT_DIR Source) 
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/ThirdParty) 
set(BINARY_DIR ${CMAKE_BINARY_DIR}/Binaries)


# Project source files
set(PROJECT_SOURCE_FILES 
    "${SOURCE_PROJECT_DIR}/AntiMissile.cpp"
    "${SOURCE_PROJECT_DIR}/AntiMissile.h"
    "${SOURCE_PROJECT_DIR}/Base.cpp"
    "${SOURCE_PROJECT_DIR}/Base.h"
    "${SOURCE_PROJECT_DIR}/City.cpp"
    "${SOURCE_PROJECT_DIR}/City.h"
    "${SOURCE_PROJECT_DIR}/Enemy.cpp"
    "${SOURCE_PROJECT_DIR}/Enemy.h"
    "${SOURCE_PROJECT_DIR}/EnemyMissile.cpp"
    "${SOURCE_PROJECT_DIR}/EnemyMissile.h"
    "${SOURCE_PROJECT_DIR}/Factory.cpp"
    "${SOURCE_PROJECT_DIR}/Factory.h"
    "${SOURCE_PROJECT_DIR}/Game.cpp"
    "${SOURCE_PROJECT_DIR}/Game.h"
    "${SOURCE_PROJECT_DIR}/GameInterface.h"
    "${SOURCE_PROJECT_DIR}/GameObject.cpp"
    "${SOURCE_PROJECT_DIR}/GameObject.h"
    "${SOURCE_PROJECT_DIR}/Main.cpp"
    "${SOURCE_PROJECT_DIR}/MathLibrary.h"
    "${SOURCE_PROJECT_DIR}/Missile.cpp"
    "${SOURCE_PROJECT_DIR}/Missile.h"
    "${SOURCE_PROJECT_DIR}/Player.cpp"
    "${SOURCE_PROJECT_DIR}/Player.h"
    "${SOURCE_PROJECT_DIR}/Singleton.h"
)

# Organize filters based on directory structure
source_group( TREE ${CMAKE_SOURCE_DIR} FILES PROJECT_SOURCE_FILES)
# Set Debug binary files directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BINARY_DIR}/Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${BINARY_DIR}/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${BINARY_DIR}/Release)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BINARY_DIR}/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${BINARY_DIR}/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BINARY_DIR}/Debug)

add_executable(MissileCommand WIN32 ${PROJECT_SOURCE_FILES})


target_link_libraries(
MissileCommand 
    optimized
    "${THIRD_PARTY_DIR}/EngineX/lib/EngineX.lib"
    debug
    "${THIRD_PARTY_DIR}/EngineX/lib/EngineXd.lib"
    general
    "${THIRD_PARTY_DIR}/SDL/2.0.3/lib/x86/SDL2.lib"
    "${THIRD_PARTY_DIR}/SDL/gfx-1.0.1/lib/SDL2-gfx.lib"
    "${THIRD_PARTY_DIR}/SDL/ttf-2.0.12/lib/SDL2-ttf.lib"
    winmm.lib
)

target_include_directories(
MissileCommand
    PUBLIC
        ${SOURCE_PROJECT_DIR}
        "${THIRD_PARTY_DIR}/EngineX/include"
        "${THIRD_PARTY_DIR}/SDL/2.0.3/include"
        "${THIRD_PARTY_DIR}/SDL/gfx-1.0.1"
        "${THIRD_PARTY_DIR}/SDL/ttf-2.0.12/include"
)

set_property(TARGET MissileCommand PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:MissileCommand>)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MissileCommand)

add_custom_command(
TARGET MissileCommand 
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${THIRD_PARTY_DIR}/SDL/2.0.3/lib/x86/SDL2.dll"
        "${THIRD_PARTY_DIR}/SDL/gfx-1.0.1/lib/SDL2-gfx.dll"
        "${THIRD_PARTY_DIR}/SDL/ttf-2.0.12/lib/SDL2-ttf.dll"
        "${THIRD_PARTY_DIR}/FreeType/lib/libfreetype-6.dll"
        "${THIRD_PARTY_DIR}/ZLib/lib/zlib1.dll"
    $<TARGET_FILE_DIR:MissileCommand>)

    
add_custom_command( TARGET MissileCommand  PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets/Fonts/
    $<TARGET_FILE_DIR:MissileCommand>)