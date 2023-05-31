cmake_minimum_required(VERSION 3.20)
project(vix LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(VMwareVIX REQUIRED)
find_package(NAPI REQUIRED)

add_subdirectory(src)
