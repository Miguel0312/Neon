#include "math/point.h"
#include "reflection/lambertian.h"
#include "reflection/metallic.h"
#include "scene/accelerators/accelerator.h"
#include "scene/accelerators/octree.h"
#include "scene/camera.h"
#include "scene/integrators/whittedIntegrator.h"
#include "scene/lights/areaLight.h"
#include "scene/scene.h"
#include "scene/sceneParser.h"
#include "scene/shape.h"
#include "scene/sphere.h"
#include "scene/triangle.h"
#include "thirdparty/argparse/argparse.h"
#include "utils/sampling/independentSampler.h"
#include "utils/visualizer.h"
#include <math/vector.h>
#include <memory>
#include <utils/color.h>
#include <utils/image.h>

#define WIDTH 800
#define HEIGHT 600

using namespace Neon;

int main(int argc, char *argv[]) {
  argparse::ArgumentParser argParser("Neon", "0.1.0");

  argParser.add_argument("sceneFile");
  argParser.add_argument("-o", "--output").default_value("images/image.png");
  argParser.add_argument("-j").scan<'i', int>();

  argParser.parse_args(argc, argv);

  SceneParser sceneParser(argParser.get("sceneFile"));

  sceneParser.renderScene();

  /*std::unique_ptr<Camera> camera = std::make_unique<Camera>();
  camera->setWidth(WIDTH), camera->setHeight(HEIGHT);
  camera->setCenter({0, 0, 2});

  std::unique_ptr<Integrator> integrator =
      std::make_unique<WhittedIntegrator>(10);

  std::unique_ptr<Sampler> sampler = std::make_unique<IndependentSampler>();
  std::unique_ptr<Accelerator> accelerator = std::make_unique<Octree>();

  Lambertian groundMaterial = Lambertian(Color(0.8, 0.8, 0.0));
  Lambertian centerMaterial = Lambertian(Color(0.1, 0.2, 0.5));
  Metallic leftMaterial = Metallic(Color(0.8, 0.8, 0.8));
  Metallic rightMaterial = Metallic(Color(0.8, 0.6, 0.2));
  Metallic lightMaterial = Metallic(Color(1.0, 1.0, 1.0f));

  std::unique_ptr<Shape> sphere1 =
      std::make_unique<Sphere>(Point3f(0, 0, -0.2), 0.5f, &centerMaterial);
  std::unique_ptr<Shape> sphere2 =
      std::make_unique<Sphere>(Point3f(-1.0, 0.0, 0.0), 0.5f, &leftMaterial);
  std::unique_ptr<Shape> sphere3 =
      std::make_unique<Sphere>(Point3f(1, 0, 0.0), 0.5f, &rightMaterial);
  std::unique_ptr<Shape> ground = std::make_unique<Sphere>(
      Point3f(0, -100.5, 0.0), 100.0f, &groundMaterial);
  std::unique_ptr<Shape> lightSphere1 =
      std::make_unique<Sphere>(Point3f(-1, 1, 0.2), 0.3f, &lightMaterial);
  std::unique_ptr<Shape> lightSphere2 =
      std::make_unique<Sphere>(Point3f(1.5, 1, 0.2), 0.3f, &groundMaterial);
  std::unique_ptr<Shape> triangle = std::make_unique<Triangle>(
      Point3f(0.0f, -1.0f, 0.0f), Point3f(0.0f, 0.0f, 0.0f),
      Point3f(1.0f, -1.0f, 1.0f), &groundMaterial);

  std::unique_ptr<Light> light1 =
      std::make_unique<AreaLight>(lightSphere1.get(), Color(1.0f));
  std::unique_ptr<Light> light2 =
      std::make_unique<AreaLight>(lightSphere2.get(), Color(1.0f));

  Scene scene;

  if (argParser.is_used("--output")) {
    scene.setFilename(argParser.get<std::string>("--output"));
  }
  if (argParser.is_used("-j")) {
    scene.setRenderingThreadsCount(argParser.get<int>("-j"));
  }

  scene.addShape(sphere1);
  scene.addShape(sphere2);
  scene.addShape(sphere3);
  scene.addShape(ground);
  //  scene.addShape(triangle);
  scene.addShape(lightSphere1);
  scene.addShape(lightSphere2);

  scene.setCamera(camera);
  scene.setIntegrator(integrator);
  scene.setSampler(sampler);
  scene.setAccelerator(accelerator);
  scene.addLight(light1);
  scene.addLight(light2);
  scene.setSampleCount(64);

  Visualizer visualizer(&scene);

  visualizer.render();*/

  return 0;
}
