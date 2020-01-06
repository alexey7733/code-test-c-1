set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable (symbioDemo main.cpp ui.cpp)

target_link_libraries (symbioDemo LINK_PUBLIC eng acc Qt5::Widgets)
