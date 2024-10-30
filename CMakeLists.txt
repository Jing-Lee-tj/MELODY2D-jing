# # The source code is a multi-body meshless method developed by mollon.
# # You can use this file to build it.
# #-------------------------------------------------------------------------
# #    Contributing author:
# #    Jing Lee (MATE, Shanghai)
# #-------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20.0)
project(MELODY2D LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Code)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/melody2d)
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

