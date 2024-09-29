#include "scene/sceneParser.h"
#include "scene/accelerators/accelerator.h"
#include "scene/integrators/integrator.h"
#include "scene/scene.h"
#include "thirdparty/argparse/argparse.h"
#include "utils/objectFactory.h"
#include "utils/utils.h"
#include "utils/visualizer.h"
#include <filesystem>
#include <iostream>
#include <memory>

namespace Neon {
SceneParser *SceneParser::m_singleton = nullptr;

SceneParser::SceneParser(const std::string &configFile) {
  std::ifstream file(configFile);
  if (!file.is_open()) {
    std::cout << "ERROR: file not found " << configFile << std::endl;
    m_isOk = false;
    return;
  }

  std::filesystem::path configFilePath(configFile);
  configFilePath.replace_extension(".png");
  m_scene.setFilename(configFilePath.string());

  m_config = toml::parse_file(configFile);

  m_scene.setSampleCount(
      m_config["samplesPerPixel"].value_or<unsigned int>(64));

  m_singleton = this;

  parseCamera();
  parseIntegrator();
  parseSampler();
  parseAccelerator();
  parseMaterials();
  parseShapes();
  parseLights();
}

SceneParser::SceneParser(const std::string &configFile,
                         const argparse::ArgumentParser &args)
    : SceneParser(configFile) {
  if (args.is_used("-j")) {
    m_scene.setRenderingThreadsCount(args.get<int>("-j"));
  }
}

void SceneParser::renderScene() {
  Visualizer visualizer(&m_scene);

  visualizer.render();
}

BSDF *SceneParser::getMaterial(const std::string &name) const {
  auto it = m_materials.find(name);
  if (it == m_materials.end()) {
    std::cout << "ERROR: no material found with name " << name << std::endl;
    return nullptr;
  }
  return it->second.get();
}

Shape *SceneParser::getShape(const std::string &name) const {
  auto it = m_shapes.find(name);
  if (it == m_shapes.end()) {
    std::cout << "ERROR: no shape found with name " << name << std::endl;
    return nullptr;
  }
  return it->second;
}

void SceneParser::parseCamera() {
  auto cameraNode = m_config.get("camera");
  if (cameraNode == nullptr) {
    std::cout << "ERROR: No camera specified in config file " << std::endl;
    m_isOk = false;
    return;
  }

  // TODO: implement this part on the camera file to be consistent with the
  // other classes
  // TODO: gracefully handle missing keys
  std::unique_ptr<Camera> camera = std::make_unique<Camera>();
  auto cameraTable = cameraNode->as_table();
  camera->setWidth(cameraTable->at("width").as_integer()->get());
  camera->setHeight(cameraTable->at("height").as_integer()->get());
  camera->setVerticalFOV(
      cameraTable->at("verticalFOV").as_floating_point()->get());
  camera->setFocalLength(
      cameraTable->at("focalLength").as_floating_point()->get());
  camera->setLensRadius(
      cameraTable->at("lensRadius").as_floating_point()->get());

  camera->setCenter(parsePoint(cameraTable->at("center").as_array()));
  camera->setForward(parseVector(cameraTable->at("forward").as_array()));
  camera->setUp(parseVector(cameraTable->at("up").as_array()));

  m_scene.setCamera(camera);
}

void SceneParser::parseIntegrator() {
  auto table = m_config.get("integrator")->as_table();
  std::unique_ptr<Integrator> integrator(
      (Integrator *)ObjectFactory::instantiate(
          table->at("type").as_string()->get(), table));

  m_scene.setIntegrator(integrator);
}

void SceneParser::parseSampler() {
  auto table = m_config.get("sampler")->as_table();
  std::unique_ptr<Sampler> sampler((Sampler *)ObjectFactory::instantiate(
      table->at("type").as_string()->get(), table));

  m_scene.setSampler(sampler);
}

void SceneParser::parseAccelerator() {
  auto table = m_config.get("accelerator")->as_table();
  std::unique_ptr<Accelerator> accelerator(
      (Accelerator *)ObjectFactory::instantiate(
          table->at("type").as_string()->get(), table));

  m_scene.setAccelerator(accelerator);
}

void SceneParser::parseMaterials() {
  auto materialsArray = m_config.get("materials")->as_array();
  for (auto &&material : *materialsArray) {
    auto materialTable = material.as_table();
    std::unique_ptr<BSDF> newMaterial((BSDF *)ObjectFactory::instantiate(
        materialTable->at("type").as_string()->get(), materialTable));

    m_materials[materialTable->at("name").as_string()->get()] =
        std::move(newMaterial);
  }

  std::cout << "Loaded " << m_materials.size() << " materials" << std::endl;
}

void SceneParser::parseShapes() {
  auto shapesArray = m_config.get("shapes")->as_array();
  int cnt = 0;

  for (auto &&shape : *shapesArray) {
    auto materialTable = shape.as_table();
    std::unique_ptr<Shape> newShape((Shape *)ObjectFactory::instantiate(
        materialTable->at("type").as_string()->get(), materialTable));

    auto name = materialTable->get("name");
    if (name != nullptr) {
      m_shapes[name->as_string()->get()] = newShape.get();
    }

    m_scene.addShape(newShape);
    cnt++;
  }

  std::cout << "Loaded " << cnt << " shapes/meshes" << std::endl;
}

void SceneParser::parseLights() {
  auto lightsNode = m_config.get("lights");
  int cnt = 0;

  if (lightsNode == nullptr) {
    return;
  }

  auto lightsArray = lightsNode->as_array();

  for (auto &&light : *lightsArray) {
    auto materialTable = light.as_table();
    std::unique_ptr<Light> newLight((Light *)ObjectFactory::instantiate(
        materialTable->at("type").as_string()->get(), materialTable));

    m_scene.addLight(newLight);
    cnt++;
  }

  std::cout << "Loaded " << cnt << " lights" << std::endl;
}
} // namespace Neon