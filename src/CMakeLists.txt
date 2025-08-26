cmake_minimum_required(VERSION 3.5)
project(viewer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets OpenGLWidgets REQUIRED)

set(SHADERS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/view/shaders")
set(SHADERS_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(viewer
    main.cc
    view/main_widget.cc
    view/opengl_widget.cc
    view/cyclic_double_spin_box.cc
    view/shader_manager.cc
    view/builder/color_widget_builder.cc
    view/builder/visual_settings_builder.cc
    controller/facade.cc
    model/common_objects.cc
    model/figure.cc
    model/file_manager.cc
    model/strategy.cc
)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -Wshadow)

target_link_libraries(viewer
    PRIVATE 
    Qt6::Widgets
    Qt6::OpenGLWidgets
)

set_target_properties(viewer PROPERTIES
    AUTOMOC ON
)

add_custom_command(TARGET viewer PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SHADERS_SOURCE_DIR}
    ${SHADERS_BINARY_DIR}
)