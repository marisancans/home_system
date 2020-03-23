cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(uWebSockets)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(SOURCES
    "${INC_DIR}/f2/function2.hpp"
    "${INC_DIR}/App.h"
    "${INC_DIR}/App.cpp"
    "${INC_DIR}/AsyncSocketData.h"
    "${INC_DIR}/AsyncSocket.h"
    "${INC_DIR}/HttpContextData.h"
    "${INC_DIR}/HttpContext.h"
    "${INC_DIR}/HttpParser.h"
    "${INC_DIR}/HttpResponseData.h"
    "${INC_DIR}/HttpResponse.h"
    "${INC_DIR}/HttpRouter.h"
    "${INC_DIR}/LoopData.h"
    "${INC_DIR}/Loop.h"
    "${INC_DIR}/PerMessageDeflate.h"
    "${INC_DIR}/TopicTree.h"
    "${INC_DIR}/Utilities.h"
    "${INC_DIR}/WebSocketContextData.h"
    "${INC_DIR}/WebSocketContext.h"
    "${INC_DIR}/WebSocketData.h"
    "${INC_DIR}/WebSocketExtensions.h"
    "${INC_DIR}/WebSocket.h"
    "${INC_DIR}/WebSocketHandshake.h"
    "${INC_DIR}/WebSocketProtocol.h"
)

add_definitions(-DLIBUS_NO_SSL)
add_definitions(-DUWS_NO_ZLIB)

add_library(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} uSockets)
target_include_directories (${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

