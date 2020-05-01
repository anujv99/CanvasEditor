cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Application)
project(Paint)

add_subdirectory(app)
add_subdirectory(paint)

set(VS_STARTUP_PROJECT Paint)