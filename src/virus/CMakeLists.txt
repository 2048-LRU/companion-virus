add_executable(mediaplayer core.c ui.c)
target_include_directories(mediaplayer PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(mediaplayer PRIVATE common_gui ${GLIB_LIBRARIES} ${GTK_LIBRARIES})
target_compile_options(mediaplayer PRIVATE ${GLIB_CFLAGS_OTHER} ${GTK_CFLAGS_OTHER})

install(TARGETS mediaplayer RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
