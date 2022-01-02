cmake_minimum_required(VERSION 3.16)
project(Trampoline)

add_custom_target(trampoline
        COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/update_assembly.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/scripts
        COMMENT "Building and packing trampoline code"
)
