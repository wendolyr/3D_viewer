cmake_minimum_required(VERSION 3.5)
project(viewer)

set(CMAKE_CXX_STANDARD 20)    # C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets OpenGLWidgets REQUIRED)
find_package(Curses REQUIRED)

add_executable(viewer
    main.cc
    view/MainWidget.cc
    view/OpenGLWidget.cc
    view/CyclicDoubleSpinBox.cc
)

add_compile_options(-Wall -Wextra -Werror)

target_include_directories(viewer PRIVATE ${CURSES_INCLUDE_DIR})
# Линковка всех библиотек и зависимостей
target_link_libraries(viewer
    PRIVATE 
    ${CURSES_LIBRARIES} # ncurses
    Qt6::Widgets    # Qt6 (или Qt5::Widgets)
    Qt6::OpenGLWidgets
)

set_target_properties(viewer PROPERTIES
    AUTOMOC ON
)