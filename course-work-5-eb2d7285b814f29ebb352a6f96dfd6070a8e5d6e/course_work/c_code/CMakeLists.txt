cmake_minimum_required(VERSION 3.16)
project("C Code")

set(CMAKE_C_FLAGS "")
set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_EXE_LINKER_FLAGS "")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wdisabled-optimization -Wdiv-by-zero -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wfloat-equal -Wint-to-pointer-cast")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-include-dirs -Wnested-externs")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-main -Woverflow -Wparentheses -Wpointer-arith")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpointer-to-int-cast -Wredundant-decls -Wshadow")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wstrict-prototypes -Wtrigraphs -Wundef -Wunused-parameter")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wvariadic-macros -Wvla -Wwrite-strings -fpie")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-asynchronous-unwind-tables -fno-unwind-tables")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-stack-protector -fno-builtin -nostdinc -nostdlib")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing -Wl,--build-id=none -std=c11 -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s -O0")

set(CMAKE_EXE_LINKER_FLAGS " -s -T ${CMAKE_CURRENT_SOURCE_DIR}/linker_script.ld -Wl,--nmagic")

enable_language(C)

# Dummy target to make code insight work
add_executable(C_Code
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)

add_custom_target(c_code
        COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/build_c_code.sh
            ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src
        COMMENT "Building C code"
)
