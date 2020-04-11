#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include <iosfwd>
#include <string>
#include <vector>

#include "../Components/Transform.hpp"
#include "../Mixins/Geometry.hpp"
#include "../Mixins/Transform.hpp"
#include "../Shared.hpp"
#include "AtribLayout.hpp"
#include "Shader.hpp"
#include "Shapes/Shape.hpp"
#include "Texture.hpp"
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
        GLuint vao = 0;

      public:
        bool castsShadow = true;
        bool cleanVBO = true;

        string name;
        Ref<Shader> shader;

        Texture texture;

        Transform transform;

        VertexArray arrayBuffer;

        inline vec3 getPosition() const { return transform.position; }
        inline void setPosition(vec3 const& v) { transform.position = v; }
        inline void setScale(vec3 const& v) { transform.scale = v; }
        inline mat4 getModelMatrix() const { return transform.modelMatrix; }

        bool TextureINIT = false, UVBufferINIT = false, vertexBufferINIT = false;

        bool isLight = false;

        vector<struct Vertex> Vertices;

        inline std::vector<Vertex> getVertices() { return Vertices; }

        void setTexture(std::string path) {
            texture = Texture(path);
            GL_LOG("Set Texture");
            TextureINIT = true;
        }

        // create default Renderable
        Renderable()
            : transform(),
              arrayBuffer(),
              Polygon2D(vector<Vertex2D>({Vertex2D(-1, -1), Vertex2D(1, -1), Vertex2D(1, 1)})) {
            name = "Renderable";

            Vertices.push_back(Vertex(-1.0f, -1.0f, 0.0f, 0, 0));
            Vertices.push_back(Vertex(1.0f, -1.0f, 0.0f, 1, 0));
            Vertices.push_back(Vertex(1.0f, 1.0f, 0.0f, 1, 1));
        }

        Renderable(Shape shape) : arrayBuffer(), transform() {
            name = "Renderable";
            Vertices = shape.Vertices;
        }

        Renderable(vector<Vertex> _vertices) : arrayBuffer(), transform() {
            name = "Renderable";
            Vertices = _vertices;
        }

        ~Renderable() {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            if (cleanVBO) glDeleteBuffers(1, &vertexBufferID);
            // glDeleteBuffers(1, &UVBufferID);
            // glDeleteTextures(1, &texture);
        }
    };

}  // namespace Entropy
