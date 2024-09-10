cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_STANDARD C++17)

set(CMAKE_SYSTEM_NAME "Linux")
set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu")

find_package(OpenGL REQUIRED)

message("${OPENGL_LIBRARIES}")

project(neon)

add_executable(neon src/main.cpp
  src/scene/camera.cpp
  src/scene/integrators/normalIntegrator.cpp
  src/scene/scene.cpp
  src/scene/sphere.cpp
  src/utils/color.cpp
  src/utils/image.cpp
  src/utils/visualizer.cpp
  src/thirdparty/glad.c
  src/thirdparty/stb/stb_image_write.cpp)

target_compile_options(neon PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(neon glfw)

target_include_directories(neon
  PRIVATE include)
