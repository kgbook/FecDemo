set(modules fecal OpenFEC SpdlogHelper)
foreach(item ${modules})
    add_subdirectory(${item})
endforeach()