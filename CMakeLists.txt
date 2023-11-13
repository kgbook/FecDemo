cmake_minimum_required(VERSION 3.18)
project(FecDemo)

include(cmake/module_define.cmake)

option(ENABLE_OPENFEC "enable OpenFEC or not?" ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fPIC)

include_directories(include)

set(modules app workflow local_stream network fec utils third_party)
foreach(item ${modules})
    add_subdirectory(${item})
endforeach()
