
#include <vector>

#include "../Graphics/AtribLayout.hpp"
#include "../Graphics/Shader.hpp"
#include "../Graphics/Shapes/Shape.hpp"
#include "../Graphics/Texture.hpp"
#include "../Graphics/Vertex.hpp"
#include "../Graphics/VertexBuffer.hpp"
#include "Cute/cute_c2.hpp"
#include "Entity.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

using glm::mat4;
using glm::vec2;
using glm::vec3;

#pragma once

namespace Entropy {
    /////////////////////////////////////////////////////////////////////////
    ////////////////////////// Translation ////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////

    struct Translation {
        union {
            vec2 position2D;
            vec3 position;
        };

        Translation(const vec3 &p) : position(p) {}
        Translation() : position(vec3(0)) {}
    };

    /////////////////////////////////////////////////////////////////////////
    ////////////////////////// Transform ///////////////////////////////////
    ///////////////////////////////////////////////////////////////////////

    struct Transform : public Translation {
        vec3  scale;
        float rotation;

        mat4 modelMatrix;
        mat4 scaleMatrix, translationMatrix, rotationMatrix;

        Transform() : rotation(0), scale(vec3(10)), Translation(vec3(0)) { compute(); }
        Transform(vec3 _position, float _rotation = 0, vec3 _scale = vec3(10))
            : rotation(_rotation), scale(_scale), Translation(_position) {
            compute();
        }

        inline void compute() {
            // Model matrix : an identity matrix (model will be at the origin)
            glm::mat4 model = glm::mat4(1.0f);

            rotationMatrix = glm::rotate(model, glm::radians((float)rotation), glm::vec3(0.0f, 0.0f, 1.0f));

            translationMatrix = glm::translate(model, (position));

            scaleMatrix = glm::scale(model, scale);

            setModelMatrix(translationMatrix * rotationMatrix * scaleMatrix);
        }

        inline void setScale(const vec3 &v) { scale = v; }
        inline void setScale(float x, float y) { setScale(vec3(x, y, scale.z)); }
        inline void setScale(float x, float y, float z) { setScale(vec3(x, y, z)); }
        inline void setScale(const vec2 &v) { setScale(vec3(v, scale.z)); }
        inline vec3 getScale() const { return scale; }

        inline float getRotation() const { return rotation; }
        inline void  setRotation(float r) { rotation = r; }

        inline void setModelMatrix(mat4 m) { modelMatrix = m; }
        inline mat4 getModelMatrix() const { return modelMatrix; }
    };

    /////////////////////////////////////////////////////////////////////////
    ////////////////////////// Colliders ///////////////////////////////////
    ///////////////////////////////////////////////////////////////////////

    using Manifold = c2Manifold;

    struct BoundingBox {
        float width, height;

        BoundingBox() : width(10), height(10) {}
        BoundingBox(const vec2 &size) : width(size.x), height(size.y) {}
    };

    ///////////////////////////////////////////////////////////////////////////
    ////////////////////////// AABBCollider //////////////////////////////////
    /////////////////////////////////////////////////////////////////////////

    struct AABBCollider {
        BoundingBox boundingBox;

        AABBCollider(const vec2 &bounds) : boundingBox(bounds) {}

        static c2AABB getTransformedAABB(const BoundingBox &boundingBox, const vec2 &position) {
            c2AABB aabb;
            aabb.min = c2V((-boundingBox.width + position.x), (-boundingBox.height + position.y));
            aabb.max = c2V((boundingBox.width + position.x), (boundingBox.height + position.y));
            return aabb;
        }

        bool getIntersection(const Translation &transformA, const AABBCollider &colliderB,
                             const Translation &transformB) {
            return c2AABBtoAABB(getTransformedAABB(this->boundingBox, transformA.position2D),
                                getTransformedAABB(colliderB.boundingBox, transformB.position2D));
        }

        // always points from shape A (This shape) to shape B (first and second shapes passed into
        // any of the c2***to***Manifold functions)
        Manifold getCollisionManifold(const Translation &transformA, const AABBCollider &colliderB,
                                      const Translation &transformB) {
            Manifold manifold;
            c2AABBtoAABBManifold(getTransformedAABB(this->boundingBox, transformA.position2D),
                                 getTransformedAABB(colliderB.boundingBox, transformB.position2D), &manifold);
            return manifold;
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    ////////////////////////// Render data ///////////////////////////////////
    /////////////////////////////////////////////////////////////////////////

    /**
     * Mesh containing VBO
     */
    struct Mesh2D {
        bool castsShadow = true;

        Ref<VertexBuffer> vertexBuffer;
        VertexArray       arrayBuffer;

        std::vector<Vertex> vertices;

        Mesh2D() {
            vertices.push_back(Vertex(-1.0f, -1.0f, 0.0f, 0, 0));
            vertices.push_back(Vertex(1.0f, -1.0f, 0.0f, 1, 0));
            vertices.push_back(Vertex(1.0f, 1.0f, 0.0f, 1, 1));

            generateBuffer();
        }

        Mesh2D(const std::vector<Vertex> &_vertices) : vertices(_vertices) { generateBuffer(); }
        Mesh2D(Mesh2D &&other) = default;
        Mesh2D &operator=(Mesh2D &&other) = default;

        ~Mesh2D() { vertices.clear(); }

        void update() { updateBuffer(); }

      private:
        size_t rendererID = 0;

        void generateBuffer() {
            arrayBuffer.bind();

            vertexBuffer = std::make_shared<VertexBuffer>(rendererID, vertices.size() * sizeof(Vertex), vertices.data(),
                                                          GL_STATIC_DRAW);

            Vertex::assertLayout();
            arrayBuffer.unbind();
        }

        void updateBuffer() { vertexBuffer->subBuffer(0, vertices.size() * sizeof(Vertex), vertices.data()); }
    };

    namespace Builder {
        static void renderData(Entropy::Entity entity) {
            entity.emplace<Mesh2D>();
            entity.emplace<Texture>("img_test.png");
            entity.emplace<Ref<Shader>>(std::make_shared<Shader>("shaders/Builtin/default.vertexshader",
                                                                 "shaders/Builtin/default.fragmentshader"));
        }

        static void renderData(Entropy::Entity entity, const Shape &shape) {
            entity.emplace<Mesh2D>(shape.Vertices);
            entity.emplace<Texture>("img_test.png");
            entity.emplace<Ref<Shader>>(std::make_shared<Shader>("shaders/Builtin/default.vertexshader",
                                                                 "shaders/Builtin/default.fragmentshader"));
        }
    }  // namespace Builder

    // struct RenderData {
    //     size_t rendererID = 0;
    //     Ref<Shader> shader;
    //     VertexArray arrayBuffer;
    //     Texture texture;
    //     Ref<VertexBuffer> vertexBuffer;

    //     vector<Vertex> vertices;

    //     RenderData() : arrayBuffer(), texture("img_test.png") {
    //         vertices.push_back(Vertex(-1.0f, -1.0f, 0.0f, 0, 0));
    //         vertices.push_back(Vertex(1.0f, -1.0f, 0.0f, 1, 0));
    //         vertices.push_back(Vertex(1.0f, 1.0f, 0.0f, 1, 1));

    //         arrayBuffer.bind();

    //         vertexBuffer = std::make_shared<VertexBuffer>(rendererID, vertices.size() * sizeof(Vertex),
    //         vertices.data(),
    //                                                       GL_STATIC_DRAW);

    //         Vertex::assertLayout();
    //         arrayBuffer.unbind();

    //         shader = std::make_shared<Shader>("shaders/Builtin/default.vertexshader",
    //                                           "shaders/Builtin/default.fragmentshader");
    //     }

    //     RenderData(const vector<Vertex> &_vertices) : vertices(_vertices), arrayBuffer(), texture("img_test.png") {
    //         arrayBuffer.bind();

    //         vertexBuffer = std::make_shared<VertexBuffer>(bufferObject, vertices.size() * sizeof(Vertex),
    //                                                       vertices.data(), GL_STATIC_DRAW);

    //         Vertex::assertLayout();
    //         arrayBuffer.unbind();

    //         shader = std::make_shared<Shader>("shaders/Builtin/default.vertexshader",
    //                                           "shaders/Builtin/default.fragmentshader");
    //     }

    //     void setTexture(std::string path) { texture = Texture(path); }
    // };
}  // namespace Entropy
