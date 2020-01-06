add_library (acc account.cpp)

target_include_directories (acc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (acc)
