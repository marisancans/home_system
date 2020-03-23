cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(uSockets)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    "crypto/openssl.c"
    "crypto/wolfssl.c"
    "eventing/epoll_kqueue.c"
    "eventing/gcd.c"
    "eventing/libuv.c"
    "internal/eventing/epoll_kqueue.h"
    "internal/eventing/gcd.h"
    "internal/eventing/libuv.h"
    "internal/internal.h"
    "internal/loop_data.h"
    "internal/networking/bsd.h"
    "context.c"
    "libusockets.h"
    "loop.c"
    "socket.c"
)

add_definitions(-DLIBUS_NO_SSL)

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

