// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <random>
#include <string>

#define GL_SILENCE_DEPRECATION


#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <cstring>
#include <chrono>
#include <thread>
#include <unistd.h>

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



// #include <GLUT/glut.h>

// declare window must go before initializers load
// GLFWwindow *window;

// the order in which these are included does matter
// Include custom initializers
// #include <main/initializers.hpp>

// #include <game/open-lib.hpp>

// TODO: clean up


using namespace glm;

// include shader loader

// // include 3d model loader
// #include <main/objloader.hpp>

double to_the(double item, int power = 2)
{
    double number = 1;
    for (size_t i = 0; i <= power; i++)
    {
        number = number * item;
    }
    return number;
}

double distance(double x_1, double y_1, double x_2, double y_2)
{
    double distance = sqrt(to_the(x_1 - x_2, 2) + to_the(y_1 - y_2, 2));
    return distance;
}



// bool loadModel(char name[], std::unique_ptr<Obj> object)
// {
//     // Read our .obj file
//     std::vector<glm::vec3> vertices;
//     std::vector<glm::vec2> uvs;
//     std::vector<glm::vec3> normals; // Won't be used at the moment.
//     bool res = loadOBJ("cube.obj", vertices, uvs, normals);
    
//     if (!res) {
//         printf("error loading blender model");
//         return false;
//     }
    
//     else
//     {
//         object->vertices = vertices;
//         object->normals = normals;
//         object->uvs = uvs;
//         return true;
//     }
    
    
// }

// #include <Entropy/Application.hpp>
// #include <Entropy/2dRenderer.hpp>

#define NDEBUG
#include <Entropy.hpp>
#include "src/Player.hpp"

#include <type_traits> 


using namespace Entropy;
using namespace std;




bool done = false;

class Trespass : public Entropy::BaseApplication
{

        double MouseXPos, MouseYPos;

        int state;
    
        Entropy::m_2dRenderer* renderer;

        Renderable* tri;

        Entropy::PhysicsEngine* world;

        shared_ptr<Player> player;

        int i = 0;

    public:

        void init() override {
            player = make_shared<Player>();

            player->setPosition(vec3(320,240,0));

            player->scale = vec3(1,1,1);


            // Ensure we can capture the escape key being pressed below
            glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

            renderer = new Entropy::m_2dRenderer(640, 480);

            std::vector<GLfloat> vertices = {
                -1.0f, -1.0f, 1.0f, // x,y,z vertex 1
                1.0f, -1.0f, 0.0f,  // x,y,z vertex 2
                1.0f, 1.0f, 0.0f,   // x,y,z vertex 3
            };

            tri = new Renderable(vertices);

            

            // renderer->add_renderable(tri);
            renderer->add_renderable(player.get());
            // renderer->add_renderable(new Renderable(vertices, glm::vec3(100,100,-10)));

            world = new Entropy::PhysicsEngine();

            world->addObject(player.get());
        }

        void loop() override {
            glfwSetTime(0);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            tri->position.x = i;

            // renderer->transform(tri);

            renderer->transform(player.get());

            renderer->renderFrame();

            glfwSwapBuffers(window);
            glfwPollEvents();

            auto time = glfwGetTime();
            if (time < 0.016)
                usleep((0.016 - time) * 1000000);

            i++;

            glfwGetCursorPos(window, &MouseXPos, &MouseYPos);


            MouseYPos = (MouseYPos - 480) * -1;

            player->rotation = glm::degrees(atan2((MouseYPos - player->getPosition().y), (MouseXPos - player->getPosition().x)) * -1) * -1 + 45 + 2;
            
            state = glfwGetKey(window, GLFW_KEY_W);
            if (state == GLFW_PRESS)
            {
                player->velocity.y = 1000;
            }
            state = glfwGetKey(window, GLFW_KEY_S);
            if (state == GLFW_PRESS)
            {
                player->velocity.y = -1000;
            }
            state = glfwGetKey(window, GLFW_KEY_A);
            if (state == GLFW_PRESS)
            {
                player->velocity.x = -1000;
            }
            state = glfwGetKey(window, GLFW_KEY_D);
            if (state == GLFW_PRESS)
            {
                player->velocity.x = 1000;
            }

            world->timeStep(glfwGetTime());
        }

        Trespass() : Entropy::BaseApplication() {
            init();

            std::cout << "Finished custom initialization. " << std::endl;

            mainLoop();
        }
        ~Trespass() {
            // delete player;
            // delete tri;
            delete world;
            delete renderer;
        }
};


int main() {

    Trespass* app = new Trespass();
    delete app;
}


// int oldmain(void)
// {
  


//     // Dark blue background
//     glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

//     GLuint VertexArrayID;
//     glGenVertexArrays(1, &VertexArrayID);
//     glBindVertexArray(VertexArrayID);

//     // This will identify our vertex buffer
//     GLuint worldbuffer;
    

//     GLuint programID = LoadShaders("shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader");

    
//     // std::unique_ptr<Obj> cube(new Obj(programID, GL_STATIC_DRAW));
  
//     // std::unique_ptr<Triangle> triangle(new Triangle(programID, GL_STATIC_DRAW));
//         std::unique_ptr<RightTriangle> triangle2(new RightTriangle(programID, worldbuffer));

//         std::unique_ptr<GameObject> box(new GameObject(glm::vec3(40, 40, 1), 60, 60));

//         box->add_shape("tri", TRIANGLE, programID, worldbuffer);

//         auto room = std::make_shared<Room>(SCREEN_WIDTH, SCREEN_HEIGHT);
//         room->add("tri", std::move(box));

//         // player object
//         std::unique_ptr<GameObject> objk(new GameObject(glm::vec3(1, 1, 1), 10, 10, TRIANGLE));
//         objk->add_shape("tri", TRIANGLE, programID, worldbuffer);

//         std::unique_ptr<Player>
//             player(new Player(std::move(objk), room));

//         std::shared_ptr<GameObject> bullet_ob(new GameObject(glm::vec3(1, 1, 1)));
//         bullet_ob->add_shape("tri", TRIANGLE, programID, worldbuffer);

//         player->bullet_object = bullet_ob;
//         player->shader = programID;
    

    
//     triangle2->buffer();
//     // room->buffer();
//     // player->buffer();

//     auto t_start = std::chrono::high_resolution_clock::now();

//     double lastTime = glfwGetTime();
//     int nbFrames = 0;
//     int movecounter = 0;

//     float thingX = 1;
//     float thingY = 1;

//     int lastMouseState = GLFW_RELEASE;

//     // loop if not close event
//     while (!glfwWindowShouldClose(window))
//     {

//         // Measure speed
//         double currentTime = glfwGetTime();
//         nbFrames++;
//         if (currentTime - lastTime >= 1.0)
//         { // If last prinf() was more than 1 sec ago
//             // printf and reset timer
//             printf("%f ms/frame\n", 1000.0 / double(nbFrames));
//             std::cout << double(nbFrames) << " frames\n";
//             nbFrames = 0;
//             lastTime += 1.0;
            
//         }
//         // std::printf(glGetError());
//         usleep(8000);

//         // Clear the screen to black
//         glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT);

//         // Calculate transformation
//         auto t_now = std::chrono::high_resolution_clock::now();
//         float my_time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

//         // triangle->transform(player->rotation, glm::vec3(player->x, player->y, 1));
//         // triangle->draw();

//         #include <game/Loop.hpp>    

//         glfwSwapBuffers(window);
//         glfwPollEvents();

//         double xMousePos, yMousePos;
//         glfwGetCursorPos(window, &xMousePos, &yMousePos);

//         if (
//             xMousePos > 0 && xMousePos < SCREEN_WIDTH 
//             &&
//             yMousePos > 0 && yMousePos < SCREEN_HEIGHT
//         ) 
//         {
//             // std::cout << xMousePos << "     " << yMousePos << "\n";

//             // convert y axis to bottem left corner coordinate system
//             yMousePos = (yMousePos - SCREEN_HEIGHT) * -1;

//             // calculate player rotation
//             player->rotation = glm::degrees(atan2((yMousePos - player->y), (xMousePos - player->x)) * -1) *-1;
//         }

        

//         GLuint state;
//         // keys:
//             // get state of left mouse key
//             state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

//             // if left mouse key is pressed down
//             if (state == GLFW_PRESS)
//             {
//                 lastMouseState = GLFW_PRESS;
//                 // std::cout << "yes";
//                 // player->shoot();
//             }
            
//             if (state == GLFW_RELEASE) {
                
//                 if (lastMouseState == GLFW_PRESS) {
//                     std::cout << "yes\n";
//                     player->shoot(worldbuffer);
//                 }
                
//                 lastMouseState = GLFW_RELEASE;
//             }
            

//             state = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);

            
//             if (state == GLFW_PRESS) {
//                 player->speed = 5;
//             }
            
//             else{
//                 player->speed = 1;
//             }

//             // Measure speed
//             // currentTime = glfwGetTime();
//             // nbFrames++;
//             if (currentTime - lastTime >= (0.6 - player->speed) - 20)
//             { // If last prinf() was more than 1 sec ago
             

//                 glfwGetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

                
//                 state = glfwGetKey(window, GLFW_KEY_W);

//                 if (state == GLFW_PRESS && player->boundary(MOVE::UP) < room->boundary(MOVE::UP))
//                 {
//                     player->move(MOVE::UP);
//                 }

//                 state = glfwGetKey(window, GLFW_KEY_S);

//                 if (state == GLFW_PRESS && player->boundary(MOVE::DOWN) > room->boundary(MOVE::DOWN) * -1)
//                 {
//                     player->move(MOVE::DOWN);
//                 }

//                 state = glfwGetKey(window, GLFW_KEY_A);

//                 if (state == GLFW_PRESS && player->boundary(MOVE::LEFT) > room->boundary(MOVE::LEFT))
//                 {
//                     player->move(MOVE::LEFT);
//                 }

//                 state = glfwGetKey(window, GLFW_KEY_D);

//                 if (state == GLFW_PRESS && player->boundary(MOVE::RIGHT) < room->boundary(MOVE::RIGHT))
//                 {
//                     player->move(MOVE::RIGHT);
//                 }
//             }
//             state = glfwGetKey(window, GLFW_KEY_E);

//             if (state == GLFW_PRESS)
//             {
//                 player->move(MOVE::ROTATE);
//             }

//             // get state of escape key
//             state = glfwGetKey(window, GLFW_KEY_ESCAPE);

//             // if escape key is pressed down quit
//             if (state == GLFW_PRESS)
//             {
//                 glfwSetWindowShouldClose(window, GLFW_TRUE);
//             }
//     }

//     glDeleteBuffers(1, &worldbuffer);
//     glDeleteProgram(programID);
//     glDeleteVertexArrays(1, &VertexArrayID);
//     // Close OpenGL window and terminate GLFW
//     glfwTerminate();

//     return 0;
// }
