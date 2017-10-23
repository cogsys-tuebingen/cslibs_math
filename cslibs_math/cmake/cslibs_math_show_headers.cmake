function("${PROJECT_NAME}_show_headers")
    file(GLOB_RECURSE ALL_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS *.hpp *.h)
    add_custom_target(${PROJECT_NAME}_show_header SOURCES ${ALL_HEADERS})
endfunction()
