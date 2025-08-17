cmake_minimum_required(VERSION 3.14)
project(Aupt)

include(${CMAKE_SOURCE_DIR}/cmake/GetCPM.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/EmbedAsset.cmake)

GetCPM()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

CPMAddPackage("gh:SFML/SFML#3.0.1")
CPMAddPackage("gh:g-truc/glm#master")
CPMAddPackage("gh:skypjack/entt#master")

add_library(core)
target_include_directories(core PUBLIC include/core)
target_sources(core
    PRIVATE
        src/core/window.cpp
				src/core/renderer.cpp
				src/core/stage.cpp
)

EmbedAsset(
  "${CMAKE_SOURCE_DIR}/embeds/Roboto/Roboto-VariableFont_wdth-wght.ttf"
  "${CMAKE_SOURCE_DIR}/include/core/embeds.hpp"
  Roboto_font
)

add_dependencies(core ${Roboto_font_TARGET})
target_link_libraries(core PUBLIC SFML::Graphics glm::glm EnTT::EnTT)

# Engine executable
add_executable(engine)
target_include_directories(engine PUBLIC include)

target_sources(engine
    PRIVATE
        src/engine/main.cpp
        # Add other engine sources
)
target_link_libraries(engine PRIVATE core)