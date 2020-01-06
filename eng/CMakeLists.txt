add_library (eng eng.cpp)

target_include_directories (eng PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (eng acc db)
