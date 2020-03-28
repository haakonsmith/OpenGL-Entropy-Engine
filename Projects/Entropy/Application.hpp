/*
 * Copyright 2020, Haakon Smith.
 * 
 * Base application. Declares basic GLFW wrapper and game loop.
 */

#ifndef BASEAPPLICATION
#define BASEAPPLICATION
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <iostream>
#else
#error "BaseApplication included twice"
#endif

namespace Entropy {
class BaseApplication {
private:
  void initWindow(unsigned int SCREEN_WIDTH, unsigned int SCREEN_HEIGHT);

public:
  static void glfwError(int id, const char *description)
  {
    std::cout << description << std::endl;
  }

  static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

  GLFWwindow *window;
  virtual void init() { std::cout << "base" << std::endl; }
  void mainLoop();
  virtual void loop() { glfwPollEvents(); }

  bool initialized = false;

  BaseApplication();
  ~BaseApplication();
};

} // namespace Entropy
