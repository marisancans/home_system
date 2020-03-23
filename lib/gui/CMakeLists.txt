cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(gui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    imgui/gui.h
    imgui/imconfig.h
    imgui/imgui.h
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_impl_glfw.h
    imgui/imgui_impl_glfw.cpp
    imgui/imgui_impl_opengl3.cpp
    imgui/imgui_internal.h
    imgui/imgui_widgets.cpp
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h

    widgets/image.h
    widgets/log.cpp
    widgets/log.h
)

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

