/*
 * Copyright 2020, Haakon Smith.
 */

#pragma once
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Entropy {

    namespace App {
        inline constexpr std::string_view shaderAssetPath = "../Resources/Assets/";
    }

    /*
     * Get File extension from File path or File Name
     */
    std::string getFileExtension(std::string filePath);

    GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path);
    class Shader {
      private:
        GLuint shaderID;
        std::string fragmentShaderPath, vertexShaderPath;

        std::unordered_map<const GLchar *, GLuint> cache;

        inline void cachePing(const GLchar *name) {
            if (cache.find(name) == cache.end()) cache[name] = glGetUniformLocation(shaderID, name);
        }

        inline void checkExtensions(const std::string &vertexShader, const std::string &fragmentShader) {
            if (getFileExtension(fragmentShader) != fragmentShaderExtension)
                std::cerr << "!! Warning !! Unkown fragment shader extension." << std::endl;

            if (getFileExtension(vertexShader) != vertexShaderExtension)
                std::cerr << "!! Warning !! Unkown vertex shader extension." << std::endl;
        }

      public:
        bool initialised = false;
        static std::string fragmentShaderExtension, vertexShaderExtension;

        Shader() : Shader("shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader") {}

        Shader(const std::string &vertexShader, const std::string &fragmentShader)
            : initialised(true),
              fragmentShaderPath(std::string(App::shaderAssetPath) + fragmentShader)    ,
              vertexShaderPath(std::string(App::shaderAssetPath) + vertexShader) {

            std::cout << "test" << (std::string(App::shaderAssetPath)) << std::endl;
            shaderID = LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

            checkExtensions(vertexShader, fragmentShader);
        }

        ~Shader() { glDeleteProgram(shaderID); }

        inline void bind() const {
            if (initialised != true) throw std::runtime_error("Cannot bind uninitialised shader.");
            glUseProgram(shaderID);
        }

        inline void uniformMatrix4fv(const GLchar *name, glm::mat4 matrix) {
            cachePing(name);

            glUniformMatrix4fv(cache[name], 1, GL_FALSE, glm::value_ptr(matrix));
        }

        inline void uniform3f(const GLchar *name, float a, float b, float c) {
            cachePing(name);

            glUniform3f(cache[name], a, b, c);
        }

        inline void uniform1i(const GLchar *name, int a) {
            cachePing(name);

            glUniform1i(cache[name], a);
        }

        inline void uniform1f(const GLchar *name, float a) {
            cachePing(name);

            glUniform1f(cache[name], a);
        }
    };

}  // namespace Entropy