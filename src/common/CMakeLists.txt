add_library(common_gui STATIC gui_utils.c)
target_include_directories(common_gui PUBLIC ${PROJECT_SOURCE_DIR}/include ${GTK_INCLUDE_DIRS} ${GLIB_INCLUDE_DIRS})
target_link_libraries(common_gui PUBLIC ${GTK_LIBRARIES} ${GLIB_LIBRARIES} m)
target_compile_options(common_gui PRIVATE ${GTK_CFLAGS_OTHER} ${GLIB_CFLAGS_OTHER})
