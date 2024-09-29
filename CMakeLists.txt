cmake_minimum_required(VERSION 3.29)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_STANDARD C++17)

set(CMAKE_SYSTEM_NAME "Linux")
set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu;/usr/include")

find_package(OpenGL REQUIRED)

project(neon)

add_executable(neon src/main.cpp
  src/math/boundingBox.cpp
  src/math/matrix.cpp
  src/math/transform.cpp
  src/reflection/dielectric.cpp
  src/reflection/lambertian.cpp
  src/reflection/metallic.cpp
  src/scene/accelerators/octree.cpp
  src/scene/camera.cpp
  src/scene/integrators/normalIntegrator.cpp
  src/scene/integrators/whiteBoxIntegrator.cpp
  src/scene/integrators/whittedIntegrator.cpp
  src/scene/lights/areaLight.cpp
  src/scene/scene.cpp
  src/scene/sceneParser.cpp
  src/scene/sphere.cpp
  src/scene/triangle.cpp
  src/utils/color.cpp
  src/utils/image.cpp
  src/utils/objectFactory.cpp
  src/utils/sampling/discretePDF.cpp
  src/utils/sampling/independentSampler.cpp
  src/utils/sampling/warp.cpp
  src/utils/threadPool.cpp
  src/utils/visualizer.cpp
  src/thirdparty/glad.c
  src/thirdparty/stb/stb_image_write.cpp)

target_compile_options(neon PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(neon glfw)

target_include_directories(neon
  PRIVATE include)
