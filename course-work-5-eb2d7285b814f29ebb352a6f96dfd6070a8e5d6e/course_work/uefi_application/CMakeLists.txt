cmake_minimum_required(VERSION 3.16)
project("UEFI Application")

set(APP_NAME "CourseWork")

add_executable(application
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/print_functions.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.c
)

target_include_directories(application PRIVATE
        ${CMAKE_SOURCE_DIR}/edk2/MdeModulePkg/Include
        ${CMAKE_SOURCE_DIR}/edk2/MdePkg/Include
        ${CMAKE_SOURCE_DIR}/edk2/MdePkg/Include/X64
)

add_custom_target(${APP_NAME}_build
        COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/build.sh ${APP_NAME}
        COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/copy_binary.sh ${APP_NAME}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/scripts
        DEPENDS trampoline
        DEPENDS c_code
        COMMENT "Building ${APP_NAME} binary"
)

add_custom_target(${APP_NAME}_run
        COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/run.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/scripts
        DEPENDS ${APP_NAME}_build
        COMMENT "Launching QEMU"
)

set(INIT FALSE CACHE BOOL "Has edk been initialized")
if (NOT INIT)
    add_custom_target(EDK_init
            COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/init_edk_tools.sh
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/scripts
            COMMENT "Initializing EDK tools"
    )

    add_custom_target(OVMF
            COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/rebuild_ovmf.sh && bash ${CMAKE_SOURCE_DIR}/scripts/update_ovmf.sh
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/scripts
            COMMENT "Launching QEMU"
    )

    add_custom_target(AddApplicationToPackage
            COMMAND sed -i '/.*${APP_NAME}.inf/d' MdeModulePkg.dsc
            COMMAND sed -i '/.*HelloWorld.inf/a \ \ MdeModulePkg\/Application\/${APP_NAME}\/${APP_NAME}.inf' MdeModulePkg.dsc
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/edk2/MdeModulePkg
            COMMENT "Adding application to package"
    )

    add_dependencies(${APP_NAME}_build
            EDK_init
            AddApplicationToPackage
    )

    add_dependencies(${APP_NAME}_run
            OVMF
    )

    set(INIT TRUE CACHE BOOL "Has edk been initialized" FORCE)
endif ()
