function(GetCPM)
    set(CPM_PATH "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")

    if(NOT EXISTS ${CPM_PATH})
        file(DOWNLOAD
            https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake
            ${CPM_PATH}
            # Remove EXPECTED_HASH entirely for now
        )
    endif()

    include(${CPM_PATH})
endfunction()
