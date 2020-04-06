#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include <iostream>
#include <string>
#include <vector>

#include "../Mixins/Geometry.hpp"
#include "../Mixins/Transform.hpp"
#include "../Shared.hpp"
#include "Shader.hpp"
#include "Shapes/Shape.hpp"
#include "Vertex.hpp"
#include "glm/glm.hpp"

using namespace glm;
using namespace std;

#pragma once

namespace Entropy {
    class Renderable : public Polygon2D {
        friend class m_2dRenderer;

      protected:
        GLuint vertexBufferID = 0;
        GLuint TextureID = 0;
        vec3 position;
        vec3 scale;

      public:
        bool castsShadow = true;
        bool cleanVBO = true;
        string name;
        shared_ptr<Shader> shader;
        double rotation;
        mat4 MVP;
        mat4 modelMatrix;
        mat4 scaleMatrix, translationMatrix, rotationMatrix;

        GLuint texture;

        bool TextureINIT = false, UVBufferINIT = false, vertexBufferINIT = false;

        bool isLight = false;

        vector<struct Vertex> Vertices;

        short id;

        // Getters and Setters
        // clang-format off
        inline void setPosition(const vec3 &v) { position = v; }
        inline void setPosition(const vec2 &v) { setPosition(vec3(v, position.z)); }
        inline void setPosition(float x, float y) { setPosition(vec3(x, y, position.z)); }
        inline void setPosition(float x, float y, float z) { setPosition(vec3(x, y, z)); }
        inline vec3 getPosition() { return position; }

        inline void setScale(const vec3 &v) { scale = v; }
        inline void setScale(float x, float y) { setScale(vec3(x, y, scale.z)); }
        inline void setScale(float x, float y, float z) { setScale(vec3(x, y, z)); }
        inline void setScale(const vec2 &v) { setScale(vec3(v, scale.z)); }
        inline vec3 getScale() { return scale; }

        inline float getRotation() { return rotation; }
        inline void setRotation(float r) { rotation = r; }
    
        virtual void setMVP(mat4 mvp) { MVP = mvp; }
        virtual mat4 getMVP() { return MVP; }

        inline std::vector<Vertex> getVertices() {return Vertices;}

        virtual void setModelMatrix(mat4 m) { modelMatrix = m; }
        virtual mat4 getModelMatrix() { return modelMatrix; }
        // clang-format on

        void setTexture(GLuint tex) {
            texture = tex;
            TextureINIT = true;
        }

        // create default Renderable
        Renderable()
            : position(0, 0, 0),
              scale(1, 1, 1),
              rotation(0),
              Polygon2D(vector<Vertex2D>({Vertex2D(-1, -1), Vertex2D(1, -1), Vertex2D(1, 1)})) {
            name = "Renderable";

            Vertices.push_back(Vertex(-1.0f, -1.0f, 0.0f, 0, 0));
            Vertices.push_back(Vertex(1.0f, -1.0f, 0.0f, 1, 0));
            Vertices.push_back(Vertex(1.0f, 1.0f, 0.0f, 1, 1));
        }

        Renderable(Shape shape) : position(0, 0, 0), scale(1, 1, 1), rotation(0) {
            name = "Renderable";
            Vertices = shape.Vertices;
        }

        Renderable(vector<Vertex> _vertices) : position(0, 0, 0), scale(1, 1, 1), rotation(0) {
            name = "Renderable";
            Vertices = _vertices;
        }

        // change Renderable Position
        Renderable(vector<Vertex> _vertices, vec3 _position) : scale(1, 1, 1), rotation(0) {
            Vertices = _vertices;
            position = _position;
            name = "Renderable";
        }

        ~Renderable() {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            if (cleanVBO) glDeleteBuffers(1, &vertexBufferID);
            // glDeleteBuffers(1, &UVBufferID);
            glDeleteTextures(1, &texture);
        }
    };

}  // namespace Entropy
