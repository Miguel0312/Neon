cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_STANDARD C++17)

project(neon)

add_executable(neon src/main.cpp
  src/scene/camera.cpp
  src/scene/sphere.cpp
  src/utils/color.cpp
  src/utils/image.cpp
  src/thirdparty/stb/stb_image_write.cpp)

target_compile_options(neon PRIVATE -Wall -Wextra -Wpedantic)

target_include_directories(neon
  PRIVATE include)
