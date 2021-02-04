cmake_minimum_required (VERSION 3.8)

project (Galaxy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
  include_directories(win)  
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_compile_definitions(_USE_MATH_DEFINES)
endif()

add_executable(ThreeBody ThreeBody.cpp screen.h vec2.h)
add_executable(Sun-Earth-Moon Sun-Earth-Moon.cpp screen.h vec2.h)
add_executable(Collision Collision.cpp screen.h vec2.h)

