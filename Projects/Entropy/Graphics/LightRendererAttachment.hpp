#include <OpenGL/gl3.h>

#include <tuple>
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

#pragma once

namespace Entropy {

    // class OcclusionObject : public virtual Mixin::Geometry2D, public virtual Mixin::Transform {
    //   public:
    //     virtual std::vector<Vertex> getVertices() = 0;
    //     virtual glm::mat4 getModelMatrix() = 0;
    // };

    class LightRendererAttachment {
      protected:
        Ref<VertexBuffer> antiShadowBuffer;
        Ref<VertexBuffer> lightSquareBuffer;

        std::vector<Vertex> squareVerts;

        int antiShadowBufferID = 0;
        int lightSquareBufferID = 0;

        virtual void renderLine(const vec3 &p1, const vec3 &p2) = 0;

        virtual glm::mat4 getViewProjectionMatrix() = 0;

        std::pair<vec2, vec2> findExtremePoints(vec2 observer, std::vector<Vertex> const &polygon, vec2 end) {
            // vec2 closestPoint = std::min_element(polygon.begin(), polygon.end(), [observer](Vertex const& elem,
            // Vertex const& min_elem){
            //     return (glm::distance(vec2(elem.Position), observer) < glm::distance(vec2(min_elem.Position),
            //     observer));
            // })->Position;

            // auto clIndext = std::distance(polygon.begin(),min_element(polygon.begin(),polygon.end()));

            // vec2 x1;
            // vec2 x2;
            // bool found1 = false;

            auto indexofSmallestElement = [](float array[], int size) {
                int index = 0;

                for (int i = 1; i < size; i++) {
                    if (array[i] < array[index]) index = i;
                }

                return index;
            };
            float dist = 10000;
            // vec2 normal;
            vec2 rightMost, leftMost;

            // normal = glm::normalize(glm::rotate(observer, 90.0f));

            // float dists[polygon.size()];
            // vec2 poses[polygon.size()];

            for (size_t i = 0; i < polygon.size(); i++) {
                // projections[i] = glm::proj(polygon[i].xy, normal);
                auto d = (polygon[i].x - observer.x) * (end.y - observer.y) -
                         (polygon[i].y - observer.y) * (end.x - observer.x);

                if (d <= 0) {
                    // auto projLen = glm::length(glm::proj(polygon[i].xy, normal));
                    // auto projLen = glm::orientedAngle(normalize(polygon[i].xy), normalize(observer));
                    // auto projLen = glm::angle(normalize(polygon[i].xy), normalize(observer));
                    auto projLen = atan2((polygon[i].y - observer.y), (polygon[i].x - observer.x)) * -1;

                    // projLen = (observer.y > polygon[i].y) ? projLen : projLen * -1;

                    // dists[i] = projLen;
                    // poses[i] = polygon[i].xy;

                    if (std::min(projLen, dist) != dist) {
                        // LOG("Angle: " << (projLen));
                        dist = projLen;
                        rightMost = polygon[i].xy;
                    }
                }
            }

            dist = 10000;

            // rightMost = poses[indexofSmallestElement(dists, polygon.size())];

            for (size_t i = 0; i < polygon.size(); i++) {
                // projections[i] = glm::proj(polygon[i].xy, normal);
                auto d = (polygon[i].x - observer.x) * (end.y - observer.y) -
                         (polygon[i].y - observer.y) * (end.x - observer.x);

                if (d >= 0) {
                    // auto projLen = glm::length(glm::proj(polygon[i].xy, normal));
                    // auto projLen = glm::orientedAngle(normalize(polygon[i].xy), normalize(observer));
                    // auto projLen = glm::angle(normalize(polygon[i].xy), normalize(observer));
                    auto projLen = atan2((polygon[i].y - observer.y), (polygon[i].x - observer.x));

                    // projLen = (observer.y < polygon[i].y) ? projLen : projLen * -1;

                    if (std::min(projLen, dist) != dist) {
                        dist = projLen;
                        leftMost = polygon[i].xy;
                    }
                }
            }

            // renderLine(vec3((rightMost), 0) * 1.0f, vec3((observer), 0) * 1.0f);
            // renderLine(vec3(normalize(rightMost), 0) * 100.0f, vec3(normalize(observer), 0) * 100.0f);

            // normal = glm::normalize(glm::rotate(observer, -90.0f));

            // for (size_t i = 0; i < polygon.size(); i++) {
            //     // projections[i] = glm::proj(polygon[i].xy, normal);

            //     auto d = (polygon[i].x - observer.x) * (end.y - observer.y) -
            //         (polygon[i].x - observer.y) * (end.x - observer.x);

            //     if (d > 0) {
            //         // auto projLen = glm::length(glm::proj(polygon[i].xy, normal));
            //         // auto projLen = glm::orientedAngle(normalize(polygon[i].xy), normalize(observer));
            //         auto projLen = glm::angle(normalize(polygon[i].xy), normalize(observer));

            //         if (std::max(projLen, dist) != dist) {
            //             dist = projLen;
            //             leftMost = polygon[i].xy;
            //         }

            //         LOG(dist);
            //     }
            // }

            // for (size_t i = 0; i < polygon.size()*2; i++) {
            //     auto P = vec2(polygon[(i - 1) % polygon.size()].Position);
            //     auto N = vec2(polygon[(i + 1) % polygon.size()].Position);
            //     auto A = polygon[(i) % polygon.size()].xy;

            //     auto vec_of_A = A - observer;  // observer-to-A vector
            //     auto vec_of_P = P - observer;
            //     auto vec_of_N = N - observer;

            //     auto productP = vec_of_A.x * vec_of_P.y - vec_of_A.y * vec_of_P.x;
            //     auto productN = vec_of_A.x * vec_of_N.y - vec_of_A.y * vec_of_N.x;

            //     if (sign(productP) == sign(productN)) {
            //         if (found1 && x1 != A)
            //             x2 = A;
            //         else
            //             found1 = true;
            //             x1 = A;
            //         // LOG("EXTEREM " << A.x);
            //     }
            // }

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

      public:
        std::vector<Renderable *> renderables;
        Ref<Shader> shadowShader;
        Ref<Shader> lightShader;

        uint32_t lightVertexCount = 64;
        glm::vec3 pos = glm::vec3(320, 240, 0);

        std::vector<Vertex> getLightMesh() {
            std::vector<Vertex> v;

            for (auto r : renderables) {
                auto verts = r->getVertices();

                for (size_t i = 0; i < verts.size(); i++) {
                    verts[i].Position = glm::vec3(r->getModelMatrix() * glm::vec4(verts[i].Position, 1));

                    v.push_back(verts[i]);
                }

                auto origin = glm::vec3(320, 240, 0);
                auto norm = [](vec2 v1, vec2 v2) { return vec2(v2.x - v1.x, v2.y - v1.y); };
                // LOG(std::get<0>(findExtremePoints(origin, verts)).x);
                // auto x1 = Vertex(vec3(std::get<0>(findExtremePoints(origin, verts, r->getPosition() +
                // vec3(norm(r->getPosition(), origin), 0))), 0));
                auto x1 = Vertex(vec3(std::get<0>(findExtremePoints(origin, verts, r->getPosition())), 0));
                auto x2 = Vertex(vec3(std::get<1>(findExtremePoints(origin, verts, r->getPosition())), 0));

                auto d1 = x1;
                auto d2 = x2;

                renderLine(x1.Position, origin);
                renderLine(x2.Position, origin);
                renderLine(r->getPosition(), origin);

                d1.xy = x1.xy + normalize(norm(vec2(origin), x1.xy)) * 600.0f;
                d2.xy = x2.xy + normalize(norm(vec2(origin), x2.xy)) * 600.0f;

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

                // for (auto vertex : verts) {
                //     c2Ray ray;
                //     ray.p = c2V(origin.x, origin.y);
                //     auto direction = normalize(norm(vec2(origin), vertex.xy));
                //     ray.d = c2Norm(c2Sub(Vector2D(vertex.xy).c2Vector, ray.p));
                //     ray.p = c2Add(ray.p, ray.d);
                //     // ray.d = Vector2D(direction).c2Vector;
                //     ray.t = distance(vertex.xy, vec2(origin));

                //     for (auto poly : renderables) {
                //         if (poly != r) {
                //             c2x pos;

                //             c2Poly polyy;

                //             polyy = poly->poly;
                //             for (size_t i = 0; i < 8; i++)
                //             {
                //                 polyy.verts[i] = Vector2D(glm::vec2(r->getModelMatrix() * glm::vec4((vec2)
                //                 Vector2D(polyy.verts[i]), 0, 1)));
                //             }

                //             c2MakePoly(&polyy);

                //             // pos = c2Transform(Vector2D(poly->getPosition()), poly->getRotation());
                //             pos = c2xIdentity();
                //             c2Raycast cast;
                //             bool hit = c2RaytoPoly(ray, &polyy, 0, &cast);
                //             if (hit) {
                //                 LOG("IMPACT!!!");
                //                 LOG("cast.t: " << cast.t);
                //                 LOG("ray.p.y: " << ray.p.y);
                //                 LOG("ray.p.x: " << ray.p.x);
                //                 renderLine(vec3((vec2)Vector2D(ray.p), 0),
                //                         vec3((vec2)Vector2D(c2Add(ray.p, c2Mulvs(ray.d, cast.t))), 0));
                //                 v.push_back(Vertex(
                //                     vec3((vec2)Vector2D(c2Add(ray.p, c2Mulvs(ray.d, cast.t / pow(10, 24) * 2))),
                //                     0)));

                //             }
                //         }
                //     }
                // }

                // for (size_t i = 0; i < verts.size(); i++) {
                //     v.push_back(verts[i]);

                //     v.push_back(d1);
                //     v.push_back(d2);

                //     // v.push_back(verts[(i + 1) % verts.size()]);
                // }

                // v.insert(v.end(), verts.begin(), verts.end());
                v.push_back(x1);
                v.push_back(x2);
                v.push_back(d1);

                v.push_back(x1);
                v.push_back(x2);
                v.push_back(d2);

                v.push_back(x1);
                v.push_back(d2);
                v.push_back(d1);

                v.push_back(x2);
                v.push_back(d1);
                v.push_back(d2);
            }

            // LOG("MESH\n");
            // for (auto v1 : v) { LOG("X: " << v1.x << ", Y: " << v1.y); }

            return v;
        }

        void renderAntiShadows() {
            auto lightMesh = getLightMesh();
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
            lightShader->uniform3f("light", 0.5,0.5,0);
            GL_LOG("Atrib pointer");
            lightShader->uniformMatrix4fv("VP", getViewProjectionMatrix());
            GL_LOG("Atrib pointer");

            glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);


            glDrawArrays(GL_TRIANGLES, 0, squareVerts.size());
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

            
            glBlendFunc(GL_DST_COLOR, GL_DST_COLOR);
            glBlendEquation(GL_FUNC_SUBTRACT);
            glDrawArrays(GL_TRIANGLES, 0, lightMesh.size());
            
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);


            GL_LOG("draw arrays ");

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            GL_LOG("Render");
        }

        LightRendererAttachment() {
            squareVerts = {
                Vertex(0, 0, 0, 0, 0),
                Vertex(0, 480, 0, 0, 1),
                Vertex(640, 480, 0, 1, 1),

                Vertex(0, 0, 0, 0, 0),
                Vertex(640, 0, 0, 1, 0),
                Vertex(640, 480, 0, 1, 1),
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
        }
        ~LightRendererAttachment() {}
    };

}  // namespace Entropy
