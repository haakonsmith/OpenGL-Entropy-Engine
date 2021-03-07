#include <OpenGL/gl3.h>

#include <future>

#include "../pch.gch"

#define GLM_ENABLE_EXPERIMENTAL
#include "../Components/Light.hpp"
#include "../Shared.hpp"
#include "Cute/cute_c2.hpp"
#include "Renderable.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include "VertexBuffer.hpp"

using glm::vec2;
using glm::vec3;
using glm::vec4;

#pragma once

namespace Entropy {

    class LightRendererAttachment {
      protected:
        Ref<VertexBuffer> antiShadowBuffer;
        Ref<VertexBuffer> lightSquareBuffer;

        std::vector<Vertex> squareVerts;

        entt::registry &registry;

        int antiShadowBufferID  = 0;
        int lightSquareBufferID = 0;

        virtual void renderLine(const vec3 &p1, const vec3 &p2) = 0;
        virtual void renderQuad(vec3 position, float width, float height, bool hollow = false,
                                vec3 color = vec3(1, 0, 0))     = 0;

        virtual glm::mat4 getViewProjectionMatrix() = 0;

      public:
        float rayCollisionCheck(c2Ray ray) {
            std::vector<c2Poly> polys;

            auto meshGroup = registry.group<Mesh2D, Transform>();

            meshGroup.each([&](auto entity, auto &mesh, auto &transform) {
                if (mesh.castsShadow) {
                    c2Poly poly;

                    poly.count = mesh.vertices.size();

                    std::transform(mesh.vertices.begin(), mesh.vertices.end(), std::begin(poly.verts),
                                   [&](const Vertex2D &a) {
                                       auto t = transform.getModelMatrix() * vec4(a.position.glmVector, 0, 1);

                                       return Vector2D(vec2(t)).c2Vector;
                                   });

                    c2MakePoly(&poly);

                    polys.push_back(poly);
                }
            });

            float dist = ray.t;

            for (auto poly : polys) {
                c2Raycast cast;

                // LOG(poly.count);
                // LOG(poly.verts[0].x);

                if (c2RaytoPoly(ray, &poly, 0, &cast)) dist = std::min(dist, cast.t);
            }

            return dist;
        }

        void drawRayLines() {
            auto meshGroup = registry.view<Mesh2D, Transform>();

            for (auto entity : meshGroup) {
                auto [mesh, transform] = meshGroup.get<Mesh2D, Transform>(entity);
                drawRayLine(mesh, transform);
            }
        }

        void drawRayLine(const Mesh2D &mesh, const Transform &transform) {
            auto                norm  = [](vec2 v1, vec2 v2) { return vec2(v2.x - v1.x, v2.y - v1.y); };
            std::vector<Vertex> verts = mesh.vertices;

            for (size_t i = 0; i < verts.size(); i++) {
                PROFILE_SCOPE("ModelMatrixTransform");

                verts[i].Position = glm::vec3(transform.getModelMatrix() * glm::vec4(verts[i].Position, 1));
            }

            for (auto &vert : verts) {
                c2Ray ray;

                auto direction = normalize(norm(vec2(320, 240), vert.Position));

                c2v vertPos = c2V(vert.Position.x, vert.Position.y);

                ray.t = 200;
                ray.p = c2V(320, 240);
                ray.d = c2V(direction.x, direction.y);

                auto dist = rayCollisionCheck(ray);

                // LOG(ray.d.x);
                // LOG(ray.d.y);

                // LOG(ray.p.x);
                // LOG(Vector2D(vert.Position).c2Vector.y);

                // LOG(c2Sub(vertPos, ray.p).x);
                // LOG(c2Sub(vertPos, ray.p).y);

                // LOG(dist);

                auto point = vec3((vec2(320, 240) + (vec2(ray.d.x, ray.d.y) * dist)), 0);
                renderLine(vec3(320, 240, 0), point);
            }
        }

        std::vector<Vertex> getLightMesh(const Light &light) {
            std::vector<Vertex> lightMesh;
            auto                norm = [](vec2 v1, vec2 v2) { return vec2(v2.x - v1.x, v2.y - v1.y); };

            auto meshGroup = registry.view<Mesh2D, Transform>();

            for (auto entity : meshGroup) {
                auto [mesh, transform] = meshGroup.get<Mesh2D, Transform>(entity);

                auto verts = mesh.vertices;

                for (size_t i = 0; i < verts.size(); i++) {
                    PROFILE_SCOPE("ModelMatrixTransform");

                    verts[i].Position = glm::vec3(transform.getModelMatrix() * glm::vec4(verts[i].Position, 1));
                }
                lightMesh.reserve(mesh.vertices.size() * 2);

                for (auto &vert : verts) {
                    c2Ray ray;

                    auto direction = normalize(norm(light.position, vert.Position));
                    auto offset    = rotate(direction, radians(90.0f)) / 100.0f;

                    c2v vertPos = c2V(vert.Position.x, vert.Position.y);

                    // ray.t = distance(light->position, vert.Position);
                    ray.t = 1000;
                    ray.p = c2V(light.position.x, light.position.y);
                    ray.d = c2V(direction.x + offset.x, direction.y + offset.y);
                    // ray.d = c2V(direction.x, direction.y);

                    auto dist  = rayCollisionCheck(ray);
                    auto point = vec3((vec2(light.position) + (vec2(ray.d.x, ray.d.y) * dist)), 0);
                    renderLine(light.position, point);

                    lightMesh.push_back(Vertex(point));

                    ray.d = c2V(direction.x - offset.x, direction.y - offset.y);
                    dist  = rayCollisionCheck(ray);
                    point = vec3((vec2(light.position) + (vec2(ray.d.x, ray.d.y) * dist)), 0);

                    lightMesh.push_back(Vertex(point));
                }
            }

            std::array<Vertex, 4> customs = {
                Vertex(vec3(0, 0, 0), vec2(0, 0)), Vertex(vec3(App::screen.x, 0, 0), vec2(1, 0)),
                Vertex(vec3(App::screen.x, 480, 0), vec2(1, 1)), Vertex(vec3(0, App::screen.x, 0), vec2(0, 1))};

            for (auto &vert : customs) {
                c2Ray ray;

                auto direction = normalize(norm(light.position, vert.Position));
                auto offset    = rotate(direction, glm::radians(90.0f)) / 100000.0f;

                c2v vertPos = c2V(vert.Position.x, vert.Position.y);

                ray.t = 2000;
                ray.p = c2V(light.position.x, light.position.y);
                ray.d = c2V(direction.x + offset.x, direction.y + offset.y);

                auto dist = rayCollisionCheck(ray);

                auto point = vec3((vec2(light.position) + (vec2(ray.d.x, ray.d.y) * dist)), 0);

                lightMesh.push_back(Vertex(point));
            }

            std::sort(lightMesh.begin(), lightMesh.end(), [light](auto a, auto b) {
                vec2 observer = vec2(light.position);
                return atan2((a.x - observer.x), -(a.y - observer.y)) > atan2((b.x - observer.x), -(b.y - observer.y));
            });

            lightMesh.insert(lightMesh.begin(), lightMesh.back());
            lightMesh.insert(lightMesh.begin(), Vertex(light.position, (light.position)));

            return lightMesh;
        }

        Ref<Shader> shadowShader;
        Ref<Shader> finalShader;
        Ref<Shader> lightShader;
        Ref<Shader> objectShader;

        virtual void createRenderTarget(string name) = 0;

        virtual void bindRenderTarget(string name)               = 0;
        virtual void bindRenderTexture(string name, GLenum slot) = 0;

        uint32_t lightVertexCount = 600;

        VertexArray vertexArray;

        void renderLights() {
            PROFILE_FUNCTION();

            // renderQuad({App::screen.size, 0}, App::screen.sizeX, App::screen.sizeY, false, vec3(0));

            glBlendEquation(GL_MAX);
            glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX);

            lightShader->bind();
            GL_LOG("Bind Shader");

            lightShader->uniformMatrix4fv("VP", getViewProjectionMatrix());
            GL_LOG("Set VP matirx");

            auto lightView = registry.view<Light>();
            auto meshView  = registry.view<Transform, Mesh2D>();

            antiShadowBuffer->bind();
            GL_LOG("Bind VBO");

            Vertex::assertLayout();
            GL_LOG("Assert buffer layout");

            future<void> futures[lightView.size()];

            lightView.each([&](auto entity, auto &light) {
                // futures[i] = std::async(std::launch::async, &PhysicsEngine::checkForCollisions, this,);
                
                // vertexArray.bind();
                PROFILE_SCOPE("renderLight");
                lightShader->uniformMatrix4fv("VP", getViewProjectionMatrix());
                auto mesh = getLightMesh(light);
                antiShadowBuffer->subBuffer(0, lightVertexCount * sizeof(Vertex), mesh.data());
                GL_LOG("Update VBO");

                auto lp = App::screen.screenScale(light.position);

                lightShader->uniform3f("light", lp.x, lp.y, 0);
                lightShader->uniform3f("lightColour", light.colour.r, light.colour.g, light.colour.b);
                lightShader->uniform1f("intensity", light.intensity);
                GL_LOG("Uniform setting");

                glDrawArrays(GL_TRIANGLE_FAN, 0, mesh.size());
                GL_LOG("Draw call");

                meshView.each([&](auto objEntity, auto &transform, auto &mesh) {
                    lightShader->uniformMatrix4fv("VP", getViewProjectionMatrix() * transform.getModelMatrix());
                    mesh.arrayBuffer.bind();

                    glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());
                });
            });

            // Render objects over light
            // registry.view<Transform, Mesh2D>().each([&](auto entity, auto &transform, auto &mesh) {
            //     lightShader->uniformMatrix4fv("VP", getViewProjectionMatrix() * transform.getModelMatrix());
            //     mesh.arrayBuffer.bind();

            //     glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());
            // });

            // Render objects over light
            // lightView.each([&](auto entity, auto &light) {
            //     auto lp = App::screen.screenScale(light.position);

            //     lightShader->uniform3f("light", lp.x, lp.y, 0);
            //     lightShader->uniform3f("lightColour", light.colour.r, light.colour.g, light.colour.b);
            //     lightShader->uniform1f("intensity", light.intensity);
            //     GL_LOG("Uniform setting");

            //     registry.view<Transform, Mesh2D>().each([&](auto objEntity, auto &transform, auto &mesh) {
            //         lightShader->uniformMatrix4fv("VP", getViewProjectionMatrix() * transform.getModelMatrix());
            //         mesh.arrayBuffer.bind();

            //         glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());
            //     });
            // });

            glBlendEquation(GL_FUNC_ADD);
            vertexArray.bind();
        }

        LightRendererAttachment(entt::registry &_registry) : registry(_registry) {
            squareVerts = {
                Vertex(0, 0, 0, 0, 0), Vertex(0, 480, 0, 0, 1), Vertex(640, 480, 0, 1, 1),

                Vertex(0, 0, 0, 0, 0), Vertex(640, 0, 0, 1, 0), Vertex(640, 480, 0, 1, 1),
            };

            LOG("LIGHT INIT");
            antiShadowBuffer = std::make_shared<VertexBuffer>(antiShadowBufferID, lightVertexCount * sizeof(Vertex),
                                                              nullptr, GL_STREAM_DRAW);

            lightSquareBuffer = std::make_shared<VertexBuffer>(lightSquareBufferID, squareVerts.size() * sizeof(Vertex),
                                                               squareVerts.data(), GL_STATIC_DRAW);
            shadowShader      = std::make_shared<Shader>("shaders/Builtin/Lighting/mesh.vertexshader",
                                                    "shaders/Builtin/Lighting/shadow.fragmentshader");

            lightShader = std::make_shared<Shader>("shaders/Builtin/Lighting/mesh.vertexshader",
                                                   "shaders/Builtin/Lighting/light.fragmentshader");

            finalShader = std::make_shared<Shader>("shaders/Builtin/Lighting/mesh.vertexshader",
                                                   "shaders/Builtin/Lighting/final.fragmentshader");

            objectShader = std::make_shared<Shader>("shaders/Builtin/Lighting/mesh.vertexshader",
                                                    "shaders/Builtin/Lighting/final.fragmentshader");
        }
        ~LightRendererAttachment() {}
    };

}  // namespace Entropy
