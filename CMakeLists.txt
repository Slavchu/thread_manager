cmake_minimum_required(VERSION 3.14.0)
project(slavchoo_threadManager VERSION 0.1.0 LANGUAGES C CXX)

if (NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(STATUS "This project has a top-level one called [${CMAKE_PROJECT_NAME}]")
else()
    message(STATUS "This project is a top-level one")
endif()

add_library(slavchoo_threadManager STATIC src/thread_manager.cpp)
target_include_directories(slavchoo_threadManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
