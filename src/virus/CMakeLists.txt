add_executable(mediaplayer core.c ui.c)
target_include_directories(mediaplayer PRIVATE ${GIO_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(mediaplayer PRIVATE common_gui ${GLIB_LIBRARIES} ${GTK_LIBRARIES} ${GIO_LIBRARIES})
target_compile_options(mediaplayer PRIVATE ${GLIB_CFLAGS_OTHER} ${GTK_CFLAGS_OTHER})

add_executable(mediaplayer_test ${PROJECT_SOURCE_DIR}/test/virus/mediaplayer_test.c core.c)
target_include_directories(mediaplayer_test PRIVATE ${PROJECT_SOURCE_DIR}/include ${GLIB_INCLUDE_DIRS} ${GIO_INCLUDE_DIRS})
target_link_libraries(mediaplayer_test PRIVATE common_gui ${GLIB_LIBRARIES} ${GIO_LIBRARIES} m)
target_compile_options(mediaplayer_test PRIVATE ${GLIB_CFLAGS_OTHER})

add_test(NAME mediaplayer COMMAND mediaplayer_test)

install(TARGETS mediaplayer RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
