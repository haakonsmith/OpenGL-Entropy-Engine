// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <random>
#include <string>

#define GL_SILENCE_DEPRECATION

#include <math.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <cstring>
#include <functional>
#include <map>
#include <thread>
#include <vector>

#define PI 3.1415926535897932384626433832795028841971693993

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include <OpenGL/gl3ext.h>

#else
    #include <GL/glew.h>
#endif

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Include GLFW
#include <GLFW/glfw3.h>

#include <type_traits>

using namespace glm;

#include <Entropy.hpp>

#include "Entt/single_include/entt/entt.hpp"
#include "src/Bullet.hpp"
#include "src/Player.hpp"

using namespace Entropy;
using namespace std;

bool done = false;

class Trespass : public Entropy::BaseApplication {
    std::unique_ptr<Entropy::m_2dRenderer> renderer;
    std::unique_ptr<PhysicsEngine>         world;

    Entropy::Entity player = Entropy::Entity(registry);

    Entropy::Entity quad = Entropy::Entity(registry);
    Entropy::Entity light = Entropy::Entity(registry);

    entt::entity tri = registry->create();

    entt::entity topWall    = registry->create();
    entt::entity bottomWall = registry->create();
    entt::entity leftWall   = registry->create();
    entt::entity rightWall  = registry->create();

    int    state;
    double previousFrameTime;

    dvec2 mouse_position;

  public:
    void init() override {
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

        renderer = make_unique<Entropy::m_2dRenderer>(getScreen(), *registry.get());
        world = make_unique<PhysicsEngine>(*renderer, getScreen(), *registry.get());


        player.emplace<Transform>(Transform({240, 100, 0}));
        player.emplace<PhysicsData>(PhysicsData(vec3(0), vec3(0), 5));
        player.emplace<AABBCollider>(AABBCollider({10, 10}));
        Builder::renderData(player);

        quad.emplace<AABBCollider>(AABBCollider({10, 10}));
        quad.emplace<Transform>(Transform({320.0f, 280.0f, 0.0f}, 0.0f, {10, 10, 0}));
        Builder::renderData(quad, Rectangle());

        light.emplace<Light>(vec3(320,240,0));

        registry->emplace<Transform>(tri, Transform({320.0f, 240.0f, 0.0f}, 0.0f, {320, 240, 0}));
        registry->emplace<RenderData>(tri, RenderData(Rectangle().Vertices)).setTexture("floor.png");

        //////////////////////////////
        //////////// Walls //////////
        ////////////////////////////
        registry->emplace<AABBCollider>(leftWall, AABBCollider({20, 240}));
        registry->emplace<Transform>(leftWall, Transform({-20.0f, 240.0f, 0.0f}, 0.0f, {0, 0, 0}));

        registry->emplace<AABBCollider>(topWall, AABBCollider({320, 20}));
        registry->emplace<Transform>(topWall, Transform({320.0f, 500.0f, 0.0f}, 0.0f, {0, 0, 0}));

        registry->emplace<AABBCollider>(rightWall, AABBCollider({20, 240}));
        registry->emplace<Transform>(rightWall, Transform({660.0f, 240.0f, 0.0f}, 0.0f, {0, 0, 0}));

        registry->emplace<AABBCollider>(bottomWall, AABBCollider({320, 20}));
        registry->emplace<Transform>(bottomWall, Transform({320.0f, -20.0f, 0.0f}, 0.0f, {0, 0, 0}));

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    }

    void loop() override {
        // LOG("FPS: " << App::profiler.fps);
        PROFILE_FUNCTION();
        App::profiler.newFrame();

        previousFrameTime = glfwGetTime();

        glfwSetTime(0);

        glfwGetCursorPos(window, &mouse_position.x, &mouse_position.y);
        mouse_position.y = (mouse_position.y - 480) * -1;
        glfwSetTime(0);
        renderer->beginLayer("scene");

        glClear(GL_COLOR_BUFFER_BIT);

        state = glfwGetKey(window, GLFW_KEY_W);
        if (state == GLFW_PRESS) { player.get<PhysicsData>().velocity.y += 40; }
        state = glfwGetKey(window, GLFW_KEY_S);
        if (state == GLFW_PRESS) { player.get<PhysicsData>().velocity.y += -40; }
        state = glfwGetKey(window, GLFW_KEY_A);
        if (state == GLFW_PRESS) { player.get<PhysicsData>().velocity.x += -40; }
        state = glfwGetKey(window, GLFW_KEY_D);
        if (state == GLFW_PRESS) { player.get<PhysicsData>().velocity.x += 40; }

        player.get<Transform>().rotation =
            glm::degrees(atan2((mouse_position.y - player.get<Transform>().position.y),
                               (mouse_position.x - player.get<Transform>().position.x)) *
                         -1) *
                -1 +
            45;

        player.get<Transform>().compute();

        renderer->render(tri);
        // for (size_t i = 0; i < 1000; i++)
        // {
        //     for (size_t j = 0; j < 100; j++)
        //     {
        //         renderer->renderQuad({j*2,i*2,0}, 20, 20, false, {1,1,0});
        //     }

        // }

        renderer->render(player);
        renderer->render(quad);
        world->timeStep(previousFrameTime);

        renderer->endLayer();
        renderer->beginLayer("light");
        renderer->renderLights();
        renderer->endLayer();

        renderer->blendLayers("light", "scene", renderer->mergeShader);
        PROFILE_CALL(glfwSwapBuffers(window));
        glfwPollEvents();

        App::profiler.endFrame();
    }

    Trespass() : Entropy::BaseApplication() {
        init();

        std::cout << "Finished custom initialization. " << std::endl;

        mainLoop();
    }
    ~Trespass() {
        // delete player;
        // delete tri;
        // delete world;
        // delete renderer;
    }
};

// class Trespass : public Entropy::BaseApplication {
//     double MouseXPos, MouseYPos;

//     int state;

//     Entropy::m_2dRenderer* renderer;

//     shared_ptr<Renderable> tri;

//     Entropy::PhysicsEngine* world;

//     shared_ptr<Player> player;

//     shared_ptr<GameObject> quad;
//     shared_ptr<PhysicsObject> leftwall;
//     shared_ptr<PhysicsObject> rightwall;
//     shared_ptr<PhysicsObject> topwall;
//     shared_ptr<PhysicsObject> bottomwall;

//     shared_ptr<Light> light;

//     double previousFrameTime;

//     int i = 0;

//     std::chrono::time_point<std::chrono::high_resolution_clock> t_start;
//     std::chrono::time_point<std::chrono::high_resolution_clock> ent_start;

//   public:
//     void init() override {
// t_start = std::chrono::high_resolution_clock::now();
// ent_start = std::chrono::high_resolution_clock::now();
// player = make_shared<Player>();

// player->setPosition(vec3(320, 240, 0));

// Ensure we can capture the escape key being pressed below
// glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

// renderer = new Entropy::m_2dRenderer(getScreen(), registry);
// // renderer->drawOutline(true);
// std::vector<Vertex> vertices = {
//     Vertex(-1.0f, -1.0f, 0.0f),  // x,y,z vertex 1
//     Vertex(1.0f, -1.0f, 0.0f),   // x,y,z vertex 2
//     Vertex(1.0f, 1.0f, 0.0f),    // x,y,z vertex 3
//     Vertex(-1.0f, 1.0f, 0.0f),   // x,y,z vertex 3
// };

// tri = shared_ptr<Renderable>(new Renderable(Rectangle()));

// tri->setPosition(vec3(320.0f, 240.0f, 0.0f));
// tri->setScale(vec3(320, 240, 0));
// tri->setTexture("floor.png");
// tri->castsShadow = false;

// quad = make_shared<GameObject>(Rectangle());

// quad->setPosition(vec3(350, 320, 0));

// quad->collider.boundingBox.width = 10;
// quad->collider.boundingBox.height = 10;
// quad->castsShadow = true;

// player->collider.boundingBox.width = 10;
// player->collider.boundingBox.height = 10;

// player->data.physicsType = ACTIVE;
// quad->shader = make_shared<Entropy::Shader>("shaders/SimpleVertexShader.vertexshader",
// "shaders/test.fragmentshader");
// // renderer->drawOutline(true);

// light = shared_ptr<Light>(new Light());

// light->position = vec3(380, 240, 0);
// light->intensity = 10;
// light->colour = vec3(1, 0, 1);

// renderer->addLight(light.get());
// player->castsShadow = true;
// // light->position = vec3(320,240,0);

// quad->setScale(vec3(10.0f, 10.0f, 0.1f));
// renderer->addRenderable(quad.get());
// renderer->addRenderable(tri.get());
// player->setScale(vec3(10.0f, 10.0f, 0.1f));
// renderer->addRenderable(player.get());

// leftwall = make_shared<PhysicsObject>();
// leftwall->collider.boundingBox.height = 240;
// leftwall->collider.boundingBox.width = 20;
// leftwall->setPosition(vec3(-10, 240, 0));

// rightwall = make_shared<PhysicsObject>();
// rightwall->collider.boundingBox.height = 240;
// rightwall->collider.boundingBox.width = 20;
// rightwall->setPosition(vec3(650, 240, 0));

// topwall = make_shared<PhysicsObject>();
// topwall->collider.boundingBox.height = 20;
// topwall->collider.boundingBox.width = 320;
// topwall->setPosition(vec3(320, 490, 0));

// bottomwall = make_shared<PhysicsObject>();
// bottomwall->collider.boundingBox.height = 20;
// bottomwall->collider.boundingBox.width = 320;
// bottomwall->setPosition(vec3(320, -10, 0));

// world = new Entropy::PhysicsEngine(*renderer, getScreen());

// world->addObject(player.get());
// world->addObject(quad.get());
// world->addObject(leftwall.get());
// world->addObject(rightwall.get());
// world->addObject(topwall.get());
// world->addObject(bottomwall.get());
// world->debug = true;

// player->renderer = renderer;
// player->world = world;

//     glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
// }

// void loop() override {
//     LOG("FPS: " << App::profiler.fps);
//     PROFILE_FUNCTION();
//     App::profiler.newFrame();
//     {
//         PROFILE_SCOPE("Main");

// light->position.x += 1;

// previousFrameTime = glfwGetTime();

// glfwSetTime(0);
// glfwGetCursorPos(window, &MouseXPos, &MouseYPos);

// glClear(GL_COLOR_BUFFER_BIT);
// renderer->beginLayer("scene");

// glClear(GL_COLOR_BUFFER_BIT);
// glClearColor(0.0, 0.0, 0.0, 0.0);

// player->transform.compute();
// renderer->renderCircle(vec3(320, 240, 0), 10);
// renderer->renderQuad(vec3(200, 240, 0), 10, 10);
// renderer->renderLine(vec3(0), player->getPosition());

// c2Ray ray;

// ray.t = 200;
// ray.d = c2V(-0.5, -0.5);
// ray.p = c2V(320, 240);

// auto dist = renderer->rayCollisionCheck(ray);

// MouseYPos = (MouseYPos - 480) * -1;

// renderer->renderLine(vec3(320, 240, 0), vec3((vec2(320, 240) + (vec2(ray.d.x, ray.d.y) * dist)), 0));

// world->timeStep(previousFrameTime);
// renderer->renderLine((player->data.velocity) + player->getPosition(), player->getPosition());

// player->transform.rotation =
//     glm::degrees(atan2((MouseYPos - player->getPosition().y), (MouseXPos - player->getPosition().x)) * -1) *
//         -1 +
//     45;

// state = glfwGetKey(window, GLFW_KEY_W);
// if (state == GLFW_PRESS) { player->data.velocity.y = 100; }
// state = glfwGetKey(window, GLFW_KEY_S);
// if (state == GLFW_PRESS) { player->data.velocity.y = -100; }
// state = glfwGetKey(window, GLFW_KEY_A);
// if (state == GLFW_PRESS) { player->data.velocity.x = -100; }
// state = glfwGetKey(window, GLFW_KEY_D);
// if (state == GLFW_PRESS) { player->data.velocity.x = 100; }

// auto t_now = std::chrono::high_resolution_clock::now();

// state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
// if (std::chrono::duration_cast<std::chrono::duration<float>>(t_now - ent_start).count() > 0.3) {
//     player->createEnemy(vec3(rand() % 440 + 100, rand() % 220 + 100, 0));
//     ent_start = t_now;
// }
// if (state == GLFW_PRESS &&
//     std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count() > 0.05) {
//     t_start = t_now;
//     player->shouldCreate = true;
//     player->shootBullet();
// }

// player->update();

// renderer->renderFrame();
// renderer->endLayer();

// renderer->beginLayer("light");
// renderer->renderLights();
// renderer->endLayer();

// renderer->blendLayers("light", "scene", renderer->mergeShader);

//             {
//                 PROFILE_SCOPE("GPU Renderering");
//                 // glFinish();
//                 glfwSwapBuffers(window);
//             }
//             PROFILE_CALL(glfwPollEvents());
//         }
//         App::profiler.endFrame();
//     }

//     Trespass() : Entropy::BaseApplication() {
//         init();

//         std::cout << "Finished custom initialization. " << std::endl;

//         mainLoop();
//     }
//     ~Trespass() {
//         // delete player;
//         // delete tri;
//         delete world;
//         delete renderer;
//     }
// };

int main() {
    Trespass* app = new Trespass();
    delete app;
}