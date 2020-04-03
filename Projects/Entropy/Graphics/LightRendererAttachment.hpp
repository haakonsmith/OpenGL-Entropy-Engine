#include <OpenGL/gl3.h>

#include <tuple>
#include <vector>

#include "../Shared.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include "VertexBuffer.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "Cute/cute_c2.hpp"

using glm::vec2;

#pragma once

namespace Entropy {
    class OcclusionVertices {
      public:
        std::vector<Vertex> &vertices;
        unsigned int count;

        // OcclusionVertices(Vertex v[]) : vertices(v) {}
        ~OcclusionVertices();
    };

    class OcclusionObject {
      public:
        virtual std::vector<Vertex> getVertices() = 0;
        virtual glm::mat4 getModelMatrix() = 0;
    };

    class LightRendererAttachment {
      protected:
        Ref<VertexBuffer> antiShadowBuffer;

        int antiShadowBufferID = 0;

        virtual glm::mat4 getViewProjectionMatrix() = 0;

        std::pair<vec2, vec2> findExtremePoints(vec2 observer, std::vector<Vertex> const &polygon) {
            // vec2 closestPoint = std::min_element(polygon.begin(), polygon.end(), [observer](Vertex const& elem,
            // Vertex const& min_elem){
            //     return (glm::distance(vec2(elem.Position), observer) < glm::distance(vec2(min_elem.Position),
            //     observer));
            // })->Position;

            // auto clIndext = std::distance(polygon.begin(),min_element(polygon.begin(),polygon.end()));

            vec2 x1;
            vec2 x2;
            bool found1 = false;

            for (size_t i = 0; i < polygon.size()*2; i++) {
                auto P = vec2(polygon[(i - 1) % polygon.size()].Position);
                auto N = vec2(polygon[(i + 1) % polygon.size()].Position);
                auto A = polygon[(i) % polygon.size()].xy;

                auto vec_of_A = A - observer;  // observer-to-A vector
                auto vec_of_P = P - observer;
                auto vec_of_N = N - observer;

                auto productP = vec_of_A.x * vec_of_P.y - vec_of_A.y * vec_of_P.x;
                auto productN = vec_of_A.x * vec_of_N.y - vec_of_A.y * vec_of_N.x;

                if (sign(productP) == sign(productN)) {
                    if (found1 && x1 != A)
                        x2 = A;
                    else
                        found1 = true;
                        x1 = A;
                    LOG("EXTEREM " << A.x);
                }
            }

            return std::make_pair(x1,x2);
        }

        std::vector<glm::vec2> computeEdgeNormals(std::vector<Vertex> const& vertices) {
            std::vector<glm::vec2> normals;

            for (size_t i = 0; i < vertices.size(); i++)
            {
                auto normal = vertices[i].xy - vertices[(i+1) % vertices.size()].xy;
                normal = glm::rotate(normal, 90.0f);
                normals.push_back(normal);
            }
            return normals;
        }

        std::vector<glm::vec2> computeVertexNormals(std::vector<glm::vec2> const& normals) {
            std::vector<glm::vec2> vNormals;

            for (size_t i = 0; i < normals.size(); i++)
            {
                auto normal = glm::normalize(normals[i] + normals[(i+1) % normals.size()]);
                vNormals.push_back(normal);
            }
            return vNormals;
        }

      public:
        std::vector<OcclusionObject *> renderables;
        Ref<Shader> lightShader;

        uint32_t lightVertexCount = 64;
        glm::vec3 pos = glm::vec3(320, 240, 0);

        std::vector<Vertex> getLightMesh() {
            std::vector<Vertex> v;

            for (auto r : renderables) {
                auto verts = r->getVertices();

                for (size_t i = 0; i < verts.size(); i++) {
                    verts[i].Position = glm::vec3(r->getModelMatrix() * glm::vec4(verts[i].Position, 1));
                }

                auto origin = glm::vec3(320, 240, 0);
                // LOG(std::get<0>(findExtremePoints(origin, verts)).x);
                // auto x1 = Vertex(vec3(std::get<0>(findExtremePoints(origin, verts)), 0));
                // auto x2 = Vertex(vec3(std::get<1>(findExtremePoints(origin, verts)), 0));

                // auto d1 = Vertex(vec3(std::get<0>(findExtremePoints(origin, verts)), 0));
                // auto d2 = Vertex(vec3(std::get<1>(findExtremePoints(origin, verts)), 0));

                auto norm = [](vec2 v1, vec2 v2) {return vec2(v2.x - v1.x, v2.y - v1.y);};

                // d1.xy += norm(vec2(origin), x1.xy) * 20;
                // d2.xy += norm(vec2(origin), x2.xy) * 20;

                // // d1.xy = vec2(0);
                // // d2.xy = vec2(0);

                // v.push_back(d1);
                // v.push_back(x1);
                // v.push_back(x2);

                // v.push_back(x2);
                // v.push_back(d2);
                // v.push_back(d1);

                // v.push_back(x1);
                // v.push_back(d2);
                // v.push_back(d1);

                for (auto vertex : verts) {
                    c2Ray ray;
                    ray.p = c2V(origin.x, origin.y);
                    auto direction = norm(vec2(origin), vertex.xy);
                    ray.d = c2V(direction.x, direction.y);
                    ray.t = distance(vertex.xy, vec2(origin));

                    for (auto poly : renderables)
                    {
                        if (poly != r)
                        {
                            c2Poly p;
                            p.count = poly->getVertices().size();
                            p.verts = *(poly->getVertices().data());
                            p.norms = computeVertexNormals(computeEdgeNormals(poly->getVertices())).data();

                            
                        }
                        
                    }
                    
                }


                // for (size_t i = 0; i < verts.size(); i++) {
                //     v.push_back(verts[i]);

                //     v.push_back(x1);
                //     v.push_back(x2);

                //     // v.push_back(verts[(i + 1) % verts.size()]);
                // }

                // v.insert(v.end(), verts.begin(), verts.end());
            }

            LOG("MESH\n");
            for (auto v1 : v) { LOG("X: " << v1.x << ", Y: " << v1.y); }

            return v;
        }

        void renderAntiShadows() {
            auto lightMesh = getLightMesh();
            GL_LOG("Atrib pointer");

            lightShader->bind();
            GL_LOG("Atrib pointer");

            lightShader->uniformMatrix4fv("VP", getViewProjectionMatrix());

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

            // glDrawArrays(GL_TRIANGLES, 0, 3);
            glDrawArrays(GL_TRIANGLES, 0, lightMesh.size());

            GL_LOG("draw arrays ");

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            GL_LOG("Render");
        }

        LightRendererAttachment() {
            LOG("LIGHT INIT");
            antiShadowBuffer = std::make_shared<VertexBuffer>(antiShadowBufferID, lightVertexCount * sizeof(Vertex),
                                                              nullptr, GL_STREAM_DRAW);
            lightShader = std::make_shared<Shader>("shaders/Builtin/Lighting/light.vertexshader",
                                                   "shaders/Builtin/Lighting/light.fragmentshader");
        }
        ~LightRendererAttachment() {}
    };

}  // namespace Entropy
