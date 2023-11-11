cmake_minimum_required(VERSION 3.26)
project(FecDemo)

include(cmake/module_define.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fPIC)

set(modules app workflow local_stream network fec utils third_party)
foreach(item ${modules})
    add_subdirectory(${item})
endforeach()
