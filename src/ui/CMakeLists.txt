find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)
pkg_check_modules(GLIB REQUIRED glib-2.0)

file(GLOB UI_SOURCES "*.c")
list(REMOVE_ITEM UI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/gui_utils.c")

foreach(file_path ${UI_SOURCES})
    get_filename_component(filename ${file_path} NAME_WE)
    
    if(filename STREQUAL "mediaplayer_ui")
        continue()
    endif()
    
    add_executable(${filename} ${file_path} gui_utils.c)

    target_include_directories(${filename} PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${GTK_INCLUDE_DIRS}
        ${GLIB_INCLUDE_DIRS}
    )

    target_compile_options(${filename} PRIVATE ${GTK_CFLAGS_OTHER})
    
    target_link_libraries(${filename} PRIVATE
        companion_core
        ${GTK_LIBRARIES}
        ${GLIB_LIBRARIES}
        m
    )
endforeach()
