#include "thirdparty/glad/glad.h"
#include "utils/image.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <scene/scene.h>
#include <sstream>
#include <thread>
#include <utils/visualizer.h>

namespace Neon {
Visualizer::Visualizer(Scene *scene) : m_scene(scene) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  m_window =
      glfwCreateWindow(scene->getCamera()->getWidth(),
                       scene->getCamera()->getHeight(), "Neon", NULL, NULL);

  if (m_window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(m_window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
  }

  glViewport(0, 0, scene->getCamera()->getWidth(),
             scene->getCamera()->getHeight());

  float borderColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  colorToBytes(m_scene->getPixels(), m_textureData);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scene->getCamera()->getWidth(),
               scene->getCamera()->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
               m_textureData.data());

  m_vertices[0] = -1.0f, m_vertices[1] = -1.0f, m_vertices[2] = 0.0f,
  m_vertices[3] = 0.0f, m_vertices[4] = 1.0f;
  m_vertices[5] = -1.0f, m_vertices[6] = 1.0f, m_vertices[7] = 0.0f,
  m_vertices[8] = 0.0f, m_vertices[9] = 0.0f;
  m_vertices[10] = 1.0f, m_vertices[11] = 1.0f, m_vertices[12] = 0.0f,
  m_vertices[13] = 1.0f, m_vertices[14] = 0.0f;
  m_vertices[15] = 1.0f, m_vertices[16] = -1.0f, m_vertices[17] = 0.0f,
  m_vertices[18] = 1.0f, m_vertices[19] = 1.0f;

  m_indices[0] = 0, m_indices[1] = 1, m_indices[2] = 2;
  m_indices[3] = 0, m_indices[4] = 3, m_indices[5] = 2;

  glGenVertexArrays(1, &m_vertexArray);
  glBindVertexArray(m_vertexArray);

  glGenBuffers(1, &m_vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), m_vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &m_elementBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), m_indices,
               GL_STATIC_DRAW);

  m_shaderProgram = compileProgram("../src/shaders/shader.vert",
                                   "../src/shaders/shader.frag");

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

Visualizer::~Visualizer() {
  // TODO: delete the buffers, programs, etc.
  glfwTerminate();
}

void Visualizer::render() {
  std::thread t(&Scene::render, m_scene);

  while (!glfwWindowShouldClose(m_window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    colorToBytes(m_scene->getPixels(), m_textureData);
    glTextureSubImage2D(m_texture, 0, 0, 0, m_scene->getCamera()->getWidth(),
                        m_scene->getCamera()->getHeight(), GL_RGB,
                        GL_UNSIGNED_BYTE, m_textureData.data());

    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_vertexArray);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwPollEvents();
    glfwSwapBuffers(m_window);
  }

  t.join();
}

unsigned int Visualizer::loadShader(const std::string &filename, GLint type) {
  unsigned int shader = glCreateShader(type);

  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cout << "ERROR: file not found " << filename << std::endl;
    return -1;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  std::string fileContents = buffer.str();
  const char *shaderCode = fileContents.c_str();
  glShaderSource(shader, 1, &shaderCode, NULL);
  glCompileShader(shader);

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "ERROR: unsuccessful compilation of shader " << filename
              << ": " << infoLog << std::endl;
    return -1;
  }

  return shader;
}

unsigned int Visualizer::compileProgram(const std::string &vertShaderPath,
                                        const std::string &fragShaderPath) {
  unsigned int vShader = loadShader(vertShaderPath, GL_VERTEX_SHADER);
  unsigned int fShader = loadShader(fragShaderPath, GL_FRAGMENT_SHADER);

  unsigned int program = glCreateProgram();
  glAttachShader(program, vShader);
  glAttachShader(program, fShader);
  glLinkProgram(program);

  int success = true;
  char infoLog[512];
  glGetShaderiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cout << "ERROR: unsuccessful compilation of program " << infoLog
              << std::endl;
    return -1;
  }

  glDeleteShader(vShader);
  glDeleteShader(fShader);

  return program;
}
} // namespace Neon
