add_executable(mediaplayer_test mediaplayer_test.c)
target_include_directories(mediaplayer_test PRIVATE ${PROJECT_SOURCE_DIR}/include ${GLIB_INCLUDE_DIRS} ${GIO_INCLUDE_DIRS})
target_sources(mediaplayer_test PRIVATE ${PROJECT_SOURCE_DIR}/src/virus/core.c)
target_link_libraries(mediaplayer_test PRIVATE common_gui ${GLIB_LIBRARIES} ${GIO_LIBRARIES} m)
target_compile_options(mediaplayer_test PRIVATE ${GLIB_CFLAGS_OTHER})

add_test(NAME mediaplayer COMMAND mediaplayer_test)
