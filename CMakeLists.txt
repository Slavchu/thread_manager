cmake_minimum_required(VERSION 3.14.0)

add_library(slavchoo_thread_manager src/thread_manager.cpp include/thread_manager.hpp)
target_include_directories(slavchoo_thread_manager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

