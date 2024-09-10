#ifndef NEON_VISUALIZER_H
#define NEON_VISUALIZER_H

#include "scene/scene.h"
#include "thirdparty/glad/glad.h"
#include <GLFW/glfw3.h>

namespace Neon {
class Visualizer final {
public:
  Visualizer(Scene *scene);

  ~Visualizer();

  void render();

private:
  Scene *m_scene;
  GLFWwindow *m_window;
  float m_vertices[20];
  unsigned int m_indices[6];
  unsigned int m_vertexBuffer, m_vertexArray, m_elementBuffer;
  unsigned int m_shaderProgram;
  unsigned int m_texture;
  std::vector<unsigned char> m_textureData;

  unsigned int loadShader(const std::string &filename, GLint type);
  unsigned int compileProgram(const std::string &vertShaderPath,
                              const std::string &fragShaderPath);
};
} // namespace Neon

#endif
