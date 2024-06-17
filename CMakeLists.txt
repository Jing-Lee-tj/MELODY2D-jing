# # The project aims to accelerate Melody's calculations using CUDA technology.
# #-------------------------------------------------------------------------
# #    Contributing author:
# #    Jing Lee (MATE, Shanghai)
# #-------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20.0)
project(MELODY2D-CUDA LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CUDAToolkit REQUIRED)
# cuda architectures
if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
  set(CMAKE_CUDA_ARCHITECTURES 75;89)
endif()

add_subdirectory(Code)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/melody2d-cuda)
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

