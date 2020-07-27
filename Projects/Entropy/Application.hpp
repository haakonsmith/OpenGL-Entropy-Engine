/*
 * Copyright 2020, Haakon Smith.
 *
 * Base application. Declares basic GLFW wrapper, game loop, and registry.
 */

#ifndef BASEAPPLICATION
#define BASEAPPLICATION
#define GL_SILENCE_DEPRECATION
#include "GLFW/glfw3.h"
#include "pch.gch"



#include <iosfwd>

#include "Screen.hpp"
#include "Shared.hpp"
#else
#error "BaseApplication included twice"
#endif

#pragma once

namespace Entropy {

    class BaseApplication {
      protected:

        void initWindow(unsigned int SCREEN_WIDTH, unsigned int SCREEN_HEIGHT);

        // entt::registry real_registry;

      public:
        std::shared_ptr<entt::registry> registry = std::shared_ptr<entt::registry>(new entt::registry());

        static void glfwError(int id, const char *description) { std::cout << description << std::endl; }

        static Screen &getScreen() { return App::screen; }

        static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

        GLFWwindow *window;
        virtual void init() { std::cout << "base" << std::endl; }
        void mainLoop();
        virtual void loop() { glfwPollEvents(); }

        bool initialized = false;

        BaseApplication();
        ~BaseApplication();
    };

}  // namespace Entropy
