cmake_minimum_required(VERSION 3.14)
project(Aupt)

include(${CMAKE_SOURCE_DIR}/cmake/GetCPM.cmake)
GetCPM()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

CPMAddPackage("gh:SFML/SFML#3.0.1")

add_library(core)
target_include_directories(core PUBLIC include/core)
target_sources(core
    PRIVATE
        src/core/window.cpp
				src/core/renderer.cpp
)

target_link_libraries(core PUBLIC SFML::Graphics)

# Engine executable
add_executable(engine)
target_include_directories(engine PUBLIC include)

target_sources(engine
    PRIVATE
        src/engine/main.cpp
        # Add other engine sources
)
target_link_libraries(engine PRIVATE core)