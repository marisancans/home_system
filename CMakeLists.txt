cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(home_system_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# options
option(WITH_GUI "Build with gui" ON)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")



add_subdirectory("lib")
set(LIBS ${LIBS} uSockets uWebSockets dwarfs)

# include std extra libs
set(LIBS ${LIBS} pthread)

# for uWebSockets
add_definitions(-DUWS_NO_ZLIB)

# GUI
if(WITH_GUI)
    add_definitions(-DWITH_GUI)
    set(LIBS ${LIBS} gui)

    # opencv - only with gui for now
    find_package(OpenCV REQUIRED CONFIG)
    set(LIBS ${LIBS} opencv_core opencv_highgui opencv_imgproc opencv_imgcodecs)

    # glfw
    find_package(glfw3 REQUIRED)
    include_directories(${GLFW_INCLUDE_DIRS})
    link_libraries(${GLFW_LIBRARY_DIRS})

    # opengl
    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIRS})

    # glew
    find_package(GLEW REQUIRED)
    include_directories(${GLEW_INCLUDE_DIRS})
    set(LIBS ${LIBS} glfw GLEW GL GLU)
endif(WITH_GUI)



set(SOURCES
   ${SRC_DIR}/main.cpp
   ${SRC_DIR}/json.h
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBS})
