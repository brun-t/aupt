set(CPM_PATH "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")
set(CPM_VERSION v0.42.0)

get_filename_component(CPM_PATH ${CPM_PATH} ABSOLUTE)

if(NOT EXISTS ${CPM_PATH})
    file(DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/${CPM_VERSION}/CPM.cmake
        ${CPM_PATH}
        # Remove EXPECTED_HASH entirely for now
        )
endif()

include(${CPM_PATH})
