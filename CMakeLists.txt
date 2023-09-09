# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

cmake_minimum_required(VERSION 3.24)
project(MISSILE_COMMAND)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "PredefinedTargets")
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_PROJECT_DIR ${CMAKE_SOURCE_DIR}/Source) 
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/ThirdParty) 
set(BINARY_DIR ${CMAKE_BINARY_DIR}/Binaries)
file(GLOB_RECURSE PROJECT_SOURCE_FILES "${SOURCE_PROJECT_DIR}/*.cpp" "${SOURCE_PROJECT_DIR}/*.h")
source_group(TREE "${SOURCE_PROJECT_DIR}" PREFIX "Source Files/" FILES ${PROJECT_SOURCE_FILES})

# Setup ZeronEngine
set(ZERON_TESTS OFF)
set(ZERON_SANDBOX OFF)
set(ZERON_UNIFY_OUTPUT_DIR ON)
add_subdirectory(${THIRD_PARTY_DIR}/ZeronEngine)
include("${ZERON_CMAKE_UTILS}/ZeronProjectUtils.cmake")
include("${ZERON_CMAKE_UTILS}/ZeronShaderUtils.cmake")
include("${ZERON_CMAKE_UTILS}/ZeronAndroidUtils.cmake")
zeron_configure_compiler()

# Create Project
zeron_create_project(MissileCommand "${PROJECT_SOURCE_FILES}")
target_link_libraries(MissileCommand ZeronPlatform ZeronRender)
target_include_directories(MissileCommand PUBLIC ${SOURCE_PROJECT_DIR})

# Platfom specific setup
set(ZERON_ANDROID_CONFIG_SCREEN_ORIENTATION "landscape")
zeron_android_configure_package(MissileCommand "com.zeron.missileCommand" MissileCommand ${SOURCE_PROJECT_DIR})

# Asset setup
zeron_add_shader_directory(${CMAKE_SOURCE_DIR}/Shaders)
zeron_add_project_asset_directory("${CMAKE_SOURCE_DIR}/Assets" "Resources")

zeron_compile_shaders(MissileCommand "Resources/Shaders")
zeron_copy_assets(MissileCommand "Resources")
