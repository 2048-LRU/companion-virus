find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

file(GLOB list_files "*.c")
list(REMOVE_ITEM list_files "${CMAKE_CURRENT_SOURCE_DIR}/gui_utils.c")

foreach(file_path ${list_files})
    get_filename_component(filename ${file_path} NAME_WE)
    add_executable(${filename} ${file_path} gui_utils.c)

    target_include_directories(${filename} PRIVATE ${GTK_INCLUDE_DIRS})
    target_compile_options(${filename} PRIVATE ${GTK_CFLAGS_OTHER})
    target_link_libraries(${filename} PRIVATE ${GTK_LIBRARIES} m gd crypto)
endforeach()