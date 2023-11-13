clear_local_var()
set(local_module OpenFEC)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR} local_source)
set(local_dependency openfec SpdlogHelper utils) # case-sensitive
set(local_export_header ${CMAKE_CURRENT_LIST_DIR})
build_shared_library()