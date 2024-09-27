#ifndef SCENE_PARSER_H
#define SCENE_PARSER_H

#include "scene/scene.h"
#include "thirdparty/argparse/argparse.h"
#include <thirdparty/toml++/toml.h>

namespace Neon {
class SceneParser final {
public:
  SceneParser() = delete;

  SceneParser(const std::string &configFile);

  SceneParser(const std::string &configFile,
              const argparse::ArgumentParser &args);

  ~SceneParser() { m_singleton = nullptr; };

  void renderScene();

  BSDF *getMaterial(const std::string &name) const;

  Shape *getShape(const std::string &name) const;

  bool isOk() const { return m_isOk; }

  static SceneParser *getSingleton() { return m_singleton; }

private:
  bool m_isOk = true;
  Scene m_scene;
  toml::parse_result m_config;

  void parseCamera();

  void parseIntegrator();

  void parseSampler();

  void parseAccelerator();

  void parseMaterials();

  void parseShapes();

  void parseLights();

  std::map<std::string, std::unique_ptr<BSDF>> m_materials;
  std::map<std::string, Shape *> m_shapes;

  static SceneParser *m_singleton;
};
} // namespace Neon

#endif
