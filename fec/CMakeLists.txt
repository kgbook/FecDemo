include_directories(${CMAKE_CURRENT_LIST_DIR})

if (ENABLE_OPENFEC)
    add_subdirectory(OpenFEC)
endif ()

clear_local_var()
set(local_module fec)
aux_source_directory(. local_source)
set(local_dependency fecal SpdlogHelper utils)
set(local_export_header ${CMAKE_CURRENT_LIST_DIR})

if (ENABLE_OPENFEC)
    set(local_dependency ${local_dependency} OpenFEC)
    set(local_cflags ${local_cflags} -DENABLE_OPENFEC)
endif ()

build_shared_library()