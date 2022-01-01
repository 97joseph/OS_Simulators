# Simple UEFI OS Loader
## Description

Todo detailed description of the project 

## Dependencies
    
    apt install build-essential cmake uuid-dev iasl nasm python3-distutils git 
## How to build
Building using cmake build tools

    ./configure.sh
    mkdir __build && cd __build && cmake ..
    make efi_app_build

Manually building

    ./configure.sh
    cd scripts && ./build.sh

In the case of manually building result image is placed on the path:
        
    edk2/Build/MdeModule/DEBUG_GCC5/X64/MdeModulePkg/Application/CourseWork/CourseWork/OUTPUT/CourseWork.efi

## How to run
Build and run in virtual environment (qemu)

    ./configure.sh
    mkdir __build && cd __build && cmake ..
    make efi_app_run

Manually building and running in virtual environment (qemu)

    ./configure.sh
    cd scripts && ./build.sh && ./copy_efi.sh && ./run.sh

## Restrictions
If you gcc compiler version is 10 or greater you need to follow these commands
        
    sudo rm /usr/bin/gcc
    sudo ln -s /usr/bin/gcc-<version> /usr/bin/gcc


## Todo

- #### CI/CD
 
    Static and dynamic analysis, testing, autobuilding on Github Actions or other CI/CD platform
 
- #### Tests
  Choosing testing environment for common functions and UEFI specified functions

- #### Documentation
   Creating Doxygen documentation and autobuilding HTML docs from Doxygen comments and auto deploy it to GithubPages 