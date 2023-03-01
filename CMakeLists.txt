cmake_minimum_required(VERSION 3.20)
project(garand LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

# Requirements:
# - fmt
# - SDL2
# - SDL2PP
# - imgui
find_package(fmt CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2PP REQUIRED)
find_package(imgui CONFIG REQUIRED)
file(GLOB sources src/*.cpp)

add_executable(garand ${sources})
target_include_directories(garand PRIVATE ${SDL2PP_INCLUDE_DIRS})
target_link_libraries(garand
    PRIVATE
    fmt::fmt
    SDL2::SDL2main
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
    ${SDL2PP_LIBRARIES}
    imgui::imgui
)