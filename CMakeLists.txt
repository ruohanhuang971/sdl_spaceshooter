cmake_minimum_required(VERSION 3.10)
project(Sdl_spaceshooter VERSION 0.1.0 LANGUAGES C CXX)

# C++ specifications
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler flags
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra - Wpedantic)
endif()

# set compiler output location
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

# set output name based on OS
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# find all the libraries
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(${TARGET} "src/main.cpp" "src/Game.cpp" "src/Scene.cpp" "src/SceneMain.cpp" "src/SceneTitle.cpp" "src/SceneEnd.cpp")

# target_include_directories(${TARGET} PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR})

target_link_libraries(${TARGET} 
                            ${SDL2_LIBRARIES} SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer SDL2_ttf::SDL2_ttf)    