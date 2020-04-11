#include <OpenGL/gl3.h>

#include <future>
#include <string>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include "../Shared.hpp"
#include "Cute/cute_c2.hpp"
#include "Renderable.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include "VertexBuffer.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/projection.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

using glm::vec2;
using glm::vec3;
using glm::vec4;

#pragma once

namespace Entropy {

    // class OcclusionObject : public virtual Mixin::Geometry2D, public virtual Mixin::Transform {
    //   public:
    //     virtual std::vector<Vertex> getVertices() = 0;
    //     virtual glm::mat4 getModelMatrix() = 0;
    // };

    struct Light {
        vec3 position;
        float intensity;
        vec4 colour;

        Light() : position(vec3(0)), intensity(0), colour(vec4(1)) {}
        Light(vec3 const &v) : position(v), intensity(0), colour(vec4(1)) {}
    };

    class LightRendererAttachment {
      protected:
        Ref<VertexBuffer> antiShadowBuffer;
        Ref<VertexBuffer> lightSquareBuffer;

        std::vector<Vertex> squareVerts;

        int antiShadowBufferID = 0;
        int lightSquareBufferID = 0;

        virtual void renderLine(const vec3 &p1, const vec3 &p2) = 0;

        virtual glm::mat4 getViewProjectionMatrix() = 0;

        static std::pair<vec2, vec2> findExtremePoints(vec2 observer, std::vector<Vertex> const &polygon, vec2 end) {
            PROFILE_FUNCTION();

            auto calculateD = [observer, end](vec2 const &pos) {
                PROFILE_SCOPE("calculateD");
                return (pos.x - observer.x) * (end.y - observer.y) - (pos.y - observer.y) * (end.x - observer.x);
            };

            float dist = 10000;
            float d = 0;
            vec2 rightMost, leftMost;

            // get rightmost extreme
            for (size_t i = 0; i < polygon.size(); i++) {
                PROFILE_SCOPE("findRightMost");
                d = calculateD(polygon[i].xy);

                if (d <= 0) {
                    auto projLen = atan2((polygon[i].y - observer.y), (polygon[i].x - observer.x)) * -1;

                    if (std::min(projLen, dist) != dist) {
                        dist = projLen;
                        rightMost = polygon[i].xy;
                    }
                }
            }

            dist = 10000;

            // get leftmost extreme
            for (size_t i = 0; i < polygon.size(); i++) {
                PROFILE_SCOPE("findLeftMost");
                d = calculateD(polygon[i].xy);

                if (d >= 0) {
                    auto projLen = atan2((polygon[i].y - observer.y), (polygon[i].x - observer.x));

                    if (std::min(projLen, dist) != dist) {
                        dist = projLen;
                        leftMost = polygon[i].xy;
                    }
                }
            }

            return std::make_pair(leftMost, rightMost);
        }

        std::vector<glm::vec2> computeEdgeNormals(std::vector<Vertex2D> const &vertices) {
            std::vector<glm::vec2> normals;

            for (size_t i = 0; i < vertices.size(); i++) {
                auto normal = vertices[i].position - vertices[(i + 1) % vertices.size()].position;
                normal = glm::rotate((vec2)normal, 90.0f);
                normals.push_back(normal);
            }
            return normals;
        }

        std::vector<glm::vec2> computeVertexNormals(std::vector<glm::vec2> const &normals) {
            std::vector<glm::vec2> vNormals;

            for (size_t i = 0; i < normals.size(); i++) {
                auto normal = glm::normalize(normals[i] + normals[(i + 1) % normals.size()]);
                vNormals.push_back(normal);
            }
            return vNormals;
        }

        static std::vector<Vertex> computeLineOfSightVertices(Light *light, Renderable *r) {
            std::vector<Vertex> shadowMesh;

            PROFILE_FUNCTION();
            auto verts = r->getVertices();

            for (size_t i = 0; i < verts.size(); i++) {
                PROFILE_SCOPE("ModelMatrixTransform");

                verts[i].Position = glm::vec3(r->getModelMatrix() * glm::vec4(verts[i].Position, 1));

                // add renderable itself to shadowMap
                shadowMesh.push_back(verts[i]);
            }

            const auto origin = (light->position);

            auto norm = [](vec2 v1, vec2 v2) { return vec2(v2.x - v1.x, v2.y - v1.y); };

            auto extremePoints = findExtremePoints(origin, verts, r->getPosition());
            auto x1 = Vertex(vec3(std::get<0>(extremePoints), 0));
            auto x2 = Vertex(vec3(std::get<1>(extremePoints), 0));

            auto d1 = x1;
            auto d2 = x2;

            d1.xy = x1.xy + normalize(norm(vec2(origin), x1.xy)) * 2600.0f;
            d2.xy = x2.xy + normalize(norm(vec2(origin), x2.xy)) * 2600.0f;

            {
                PROFILE_SCOPE("assembleMesh");
                shadowMesh.reserve(12);
                shadowMesh.push_back(x1);
                shadowMesh.push_back(x2);
                shadowMesh.push_back(d1);

                shadowMesh.push_back(x1);
                shadowMesh.push_back(x2);
                shadowMesh.push_back(d2);

                shadowMesh.push_back(x1);
                shadowMesh.push_back(d2);
                shadowMesh.push_back(d1);

                shadowMesh.push_back(x2);
                shadowMesh.push_back(d1);
                shadowMesh.push_back(d2);
            }

            return shadowMesh;
        }

      public:
        std::vector<Renderable *> renderables;
        std::vector<Light *> lights;
        Ref<Shader> shadowShader;
        Ref<Shader> finalShader;
        Ref<Shader> lightShader;

        virtual void createRenderTarget(string name) = 0;

        virtual void bindRenderTarget(string name) = 0;
        virtual void bindRenderTexture(string name) = 0;

        uint32_t lightVertexCount = 64;
        glm::vec3 pos = glm::vec3(320, 240, 0);

        bool first = true;

        std::vector<Vertex> getShadowMesh() {
            PROFILE_FUNCTION();

            std::vector<Vertex> shadowMesh;

            // std::vector<std::future<std::vector<Vertex>>> meshes;

            // for (auto light : lights) {
            //     for (auto r : renderables) {
            //         meshes.push_back(std::async(computeLineOfSightVertices, light, r));
            //     }
            // }
            // for (size_t i = 0; i < meshes.size(); i++) {
            //     auto m = meshes[i].get();
            //     shadowMesh.insert(shadowMesh.end(), m.begin(), m.end());
            // }

            for (auto light : lights) {
                for (auto r : renderables) {
                    auto m = (computeLineOfSightVertices(light, r));
                    shadowMesh.insert(shadowMesh.end(), m.begin(), m.end());
                }
            }

            return shadowMesh;
        }

        void addLight(Light *light) { lights.push_back(light); }

        void renderAntiShadows() {
            PROFILE_FUNCTION();

            if (first) {
                createRenderTarget(std::string("name"));
                first = false;
            }

            bindRenderTarget("name");
            glClear(GL_COLOR_BUFFER_BIT);

            auto lightMesh = getShadowMesh();
            GL_LOG("Atrib pointer");

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);

            lightSquareBuffer->bind();

            glVertexAttribPointer(0,  // attribute 0. No particular reason for 0,
                                      // but must match the layout in the shader.
                                  3,               // size
                                  GL_FLOAT,        // type
                                  GL_FALSE,        // normalized?
                                  sizeof(Vertex),  // stride
                                  (void *)0        // array buffer offset
            );

            GL_LOG("Atrib pointer");
            // 2nd attribute buffer : UVs
            GL_LOG("draw arrays ");
            glVertexAttribPointer(1,  // attribute. No particular reason for 1, but
                                      // must match the layout in the shader.
                                  2,               // size : U+V => 2
                                  GL_FLOAT,        // type
                                  GL_FALSE,        // normalized?
                                  sizeof(Vertex),  // stride
                                  (void *)12       // array buffer offset
            );
            GL_LOG("add buffer data ");

            lightShader->bind();
            GL_LOG("Atrib pointer");

            lightShader->uniformMatrix4fv("VP", getViewProjectionMatrix());
            GL_LOG("Atrib pointer");

            glBlendFunc(GL_SRC_COLOR, GL_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);

            for (auto light : lights) {
                lightShader->uniform3f("light", 0.5, 0.5, 0);
                GL_LOG("Atrib pointer");
                glDrawArrays(GL_TRIANGLES, 0, squareVerts.size());
            }

            GL_LOG("Atrib pointer");

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);

            shadowShader->bind();
            GL_LOG("Atrib pointer");

            shadowShader->uniformMatrix4fv("VP", getViewProjectionMatrix());

            antiShadowBuffer->bind();
            GL_LOG("Atrib pointer");
            antiShadowBuffer->subBuffer(0, lightVertexCount * sizeof(Vertex), lightMesh.data());
            GL_LOG("Atrib pointer");

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(0,  // attribute 0. No particular reason for 0,
                                      // but must match the layout in the shader.
                                  3,               // size
                                  GL_FLOAT,        // type
                                  GL_FALSE,        // normalized?
                                  sizeof(Vertex),  // stride
                                  (void *)0        // array buffer offset
            );

            GL_LOG("Atrib pointer");
            // 2nd attribute buffer : UVs
            GL_LOG("draw arrays ");
            glVertexAttribPointer(1,  // attribute. No particular reason for 1, but
                                      // must match the layout in the shader.
                                  2,               // size : U+V => 2
                                  GL_FLOAT,        // type
                                  GL_FALSE,        // normalized?
                                  sizeof(Vertex),  // stride
                                  (void *)12       // array buffer offset
            );
            GL_LOG("add buffer data ");

            glBlendFunc(GL_DST_COLOR, GL_DST_ALPHA);
            glBlendEquation(GL_FUNC_SUBTRACT);
            glDrawArrays(GL_TRIANGLES, 0, lightMesh.size());

            GL_LOG("draw arrays ");

            GL_LOG("Render");
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, 640 * 2, 480 * 2);
            GL_LOG("Atrib pointer");

            finalShader->bind();
            GL_LOG("Atrib pointer");

            lightSquareBuffer->bind();
            GL_LOG("Atrib pointer");
            finalShader->uniformMatrix4fv("VP", getViewProjectionMatrix());
            GL_LOG("Atrib pointer");
            finalShader->uniform1i("texSampler", 0);

            bindRenderTexture("name");

            Vertex::assertLayout();
            GL_LOG("add buffer data ");
            // glBlendFunc(GL_DST_COLOR, GL_ZERO);
            // glBlendEquation(GL_MIN);
            glBlendFunc(GL_DST_COLOR, GL_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);
            // glFlush();
            glDrawArrays(GL_TRIANGLES, 0, squareVerts.size());

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
        }

        LightRendererAttachment() {
            squareVerts = {
                Vertex(0, 0, 0, 0, 0), Vertex(0, 480, 0, 0, 1), Vertex(640, 480, 0, 1, 1),

                Vertex(0, 0, 0, 0, 0), Vertex(640, 0, 0, 1, 0), Vertex(640, 480, 0, 1, 1),
            };

            LOG("LIGHT INIT");
            antiShadowBuffer = std::make_shared<VertexBuffer>(antiShadowBufferID, lightVertexCount * sizeof(Vertex),
                                                              nullptr, GL_STREAM_DRAW);

            lightSquareBuffer = std::make_shared<VertexBuffer>(lightSquareBufferID, squareVerts.size() * sizeof(Vertex),
                                                               squareVerts.data(), GL_STATIC_DRAW);
            shadowShader = std::make_shared<Shader>("shaders/Builtin/Lighting/mesh.vertexshader",
                                                    "shaders/Builtin/Lighting/shadow.fragmentshader");

            lightShader = std::make_shared<Shader>("shaders/Builtin/Lighting/mesh.vertexshader",
                                                   "shaders/Builtin/Lighting/light.fragmentshader");

            finalShader = std::make_shared<Shader>("shaders/Builtin/Lighting/mesh.vertexshader",
                                                   "shaders/Builtin/Lighting/final.fragmentshader");
        }
        ~LightRendererAttachment() {}
    };

}  // namespace Entropy
