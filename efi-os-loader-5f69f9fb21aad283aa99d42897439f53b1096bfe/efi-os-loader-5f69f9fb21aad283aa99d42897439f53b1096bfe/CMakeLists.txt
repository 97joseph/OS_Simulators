cmake_minimum_required(VERSION 3.16)

project(CourseWork VERSION 0.1.0)

# Uncomment this strings if you need autocomplete in your IDE

#file(GLOB_RECURSE src src/*.c)
#file(GLOB_RECURSE hdrs src/*.h)
#
#add_executable(application
#        ${src}
#        ${hdrs}
#        )
#
#target_include_directories(application PUBLIC
#        ${CMAKE_CURRENT_SOURCE_DIR}/edk2/MdeModulePkg/Include
#        ${CMAKE_CURRENT_SOURCE_DIR}/edk2/MdePkg/Include
#        ${CMAKE_CURRENT_SOURCE_DIR}/edk2/MdePkg/Include/X64
#        ${CMAKE_CURRENT_SOURCE_DIR}/src
#        )

add_custom_target(efi_app_build
        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/scripts/build.sh
        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/scripts/copy_efi.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/scripts
        )

add_custom_target(efi_app_run
        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/scripts
        DEPENDS efi_app_build
        )
