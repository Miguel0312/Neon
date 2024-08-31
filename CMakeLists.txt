cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(neon)

add_executable(neon src/main.cpp
  src/utils/color.cpp
  src/utils/image.cpp
  src/thirdparty/stb/stb_image_write.cpp)

target_include_directories(neon
  PRIVATE include)
