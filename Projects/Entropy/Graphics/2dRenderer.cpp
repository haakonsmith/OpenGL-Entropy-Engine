#include "2dRenderer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace Entropy {

    void m_2dRenderer::addRenderable(Renderable *_renderable) {
        // renderables don't come prepackaged with MVP, so it needs to be
        // created.
        buffer(_renderable);

        objects.push_back(_renderable);
    }

    void m_2dRenderer::removeRenderable(Renderable *renderable) {
        objects.erase(std::remove_if(objects.begin(), objects.end(),
                                     // here comes the C++11 lambda:
                                     [renderable](Renderable *node) { return node == renderable; }),
                      objects.end());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////// Init commands ///////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    void m_2dRenderer::genVertexBuffer(Renderable *_renderable) {
        // Create a buffer object for our Renderable
        glGenBuffers(1, &_renderable->vertexBufferID);

        GL_LOG("add renderable, gen buffer " << _renderable->vertexBufferID);

        glBindBuffer(GL_ARRAY_BUFFER, _renderable->vertexBufferID);

        glBufferData(GL_ARRAY_BUFFER, _renderable->Vertices.size() * sizeof(Vertex), &_renderable->Vertices.front(),
                     GL_STATIC_DRAW);

        GL_LOG("add buffer data ");

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
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // _renderable->vertexBufferID = (true);
    }

    void m_2dRenderer::buffer(Renderable *_renderable) {
        // renderables don't come prepackaged with MVP, so it needs to be
        // created
        transform(_renderable);

        if (!_renderable->vertexBufferINIT) genVertexBuffer(_renderable);

        if (!_renderable->TextureINIT) _renderable->texture = loadTexture("img_test.png");

        if (_renderable->shader == nullptr) _renderable->shader = program;
        GL_LOG("finish buffer ");
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////// Math commands ///////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    void m_2dRenderer::transform(Renderable *_renderable) {
        glm::mat4 MVP;

        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 model = glm::mat4(1.0f);

        _renderable->rotationMatrix =
            glm::rotate(model, glm::radians((float)_renderable->rotation), glm::vec3(0.0f, 0.0f, 1.0f));

        _renderable->translationMatrix = glm::translate(model, (_renderable->getPosition()));

        _renderable->scaleMatrix = glm::scale(model, _renderable->scale);

        _renderable->setModelMatrix(_renderable->translationMatrix * _renderable->rotationMatrix *
                                    _renderable->scaleMatrix);

        // Our ModelViewProjection : multiplication of our 3 matrices
        MVP = projectionMatrix * viewMatrix * _renderable->modelMatrix;  // Remember, matrix multiplication is
                                                                         // the other way around

        _renderable->setMVP(MVP);
    }

    GLuint m_2dRenderer::loadTexture(std::string path) {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *image = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

        if (image == nullptr)  // Error check
        {
            cerr << "Error when loading texture from file: " + path << endl;
        }

        // Create one OpenGL texture
        GLuint textureID;
        glGenTextures(1, &textureID);

        GL_LOG("finish buffer ");

        // "Bind" the newly created texture : all future texture functions will
        // modify this texture
        glBindTexture(GL_TEXTURE_2D, textureID);

        GL_LOG("finish buffer ");

        // Give the image to OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

        GL_LOG("finish buffer ");

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        GL_LOG("finish buffer ");

        // Return the ID of the texture we just created
        return textureID;
    }

    float m_2dRenderer::distToNearestPoint(vec3 point) {
        vector<float> distances;

        distances.reserve(objects.size() * 3);

        for (auto obj : objects) {
            for (size_t i = 0; i < obj->Vertices.size(); i++) {
                vec3 point2 = vec3(obj->getModelMatrix() * vec4(vec3(obj->Vertices[i].Position), 1));

                distances.emplace_back(distance(point, point2));
            }
        }

        return *min_element(distances.begin(), distances.end());
    }

    float m_2dRenderer::distToNearestPoint(vec3 point, Renderable *renderable) {
        vector<float> distances;

        distances.reserve(objects.size() * 3);

        for (auto obj : objects) {
            if (obj != renderable) {
                for (size_t i = 0; i < obj->Vertices.size(); i++) {
                    vec3 point2 = vec3(obj->getModelMatrix() * vec4(vec3(obj->Vertices[i].Position), 1));

                    distances.emplace_back(distance(point, point2));
                }
            }
        }

        return *min_element(distances.begin(), distances.end());
    }

    float m_2dRenderer::distToNearestPoint(Renderable *_renderable) {
        // float distances[_renderable->Vertices.size()/3];
        vector<float> distances;

        for (size_t i = 0; i < _renderable->Vertices.size(); i++) {
            vec3 point = vec3(_renderable->getModelMatrix() * vec4(vec3(_renderable->Vertices[i].Position), 1));

            // distances[i] = distToNearestPoint(point, _renderable);
            distances.emplace_back(distToNearestPoint(point, _renderable));
        }

        // return *min_element(distances, distances + 3);
        return *min_element(distances.begin(), distances.end());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////// Render commands /////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    void m_2dRenderer::renderFrame() {
        for (auto obj : objects) { render(obj); }

        glUseProgram(programID);

        if (debugOutline) {
            for (auto obj : objects) renderOutline(*obj);
        }
    }

    void m_2dRenderer::render(Renderable *_renderable) {
        glBindBuffer(GL_ARRAY_BUFFER, _renderable->vertexBufferID);
        _renderable->shader->bind();
        GL_LOG("bind shader ");

        _renderable->shader->uniformMatrix4fv("MVP", _renderable->MVP);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _renderable->texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        _renderable->shader->uniform1i("myTextureSampler", 0);

        // 1st attribute buffer : Vertices

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

        GL_LOG("Render");
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0,
                     _renderable->Vertices.size());  // Starting from vertex 0; 3
                                                     // Vertices total . 1 RightTriangle
        // LOG(_renderable->Vertices.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }

    void m_2dRenderer::renderLine(const vec3 &p1, const vec3 &p2) {
        Vertex verts[] = {Vertex(screen.localSpace(p1)), Vertex(screen.localSpace(p2))};

        VertexBuffer vbo(0, sizeof(verts), verts, GL_STREAM_DRAW);
        debugLineShader->bind();
        vbo.bind();

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

        glDrawArrays(GL_LINES, 0,
                     2);  // Starting from vertex 0; 3 Vertices total . 1 RightTriangle
        GL_LOG("draw arrays ");

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        GL_LOG("Render");
    }

    void m_2dRenderer::renderOutline(const Renderable &_renderable) {
        debugShader->bind();

        debugShader->uniformMatrix4fv("MVP", _renderable.MVP);
        debugShader->uniform3f("inColor", 1, 1, 0);

        GL_LOG("bind uniform ");

        glBindBuffer(GL_ARRAY_BUFFER, _renderable.vertexBufferID);

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

        glDrawArrays(GL_LINE_LOOP, 0,
                     _renderable.Vertices.size());  // Starting from vertex 0; 3 Vertices
                                                    // total . 1 RightTriangle
        GL_LOG("draw arrays ");

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        GL_LOG("Render");
    }

    void m_2dRenderer::renderCenter(Renderable *_renderable) {}

    void m_2dRenderer::renderQuad(vec3 position, float width, float height, bool hollow, vec3 colour) {
        mat4 MVP = projectionMatrix * viewMatrix *
                   (glm::translate(mat4(1.0f), (position)) * glm::scale(mat4(1.0f), vec3(width, height, 1)));

        debugShader->bind();

        debugShader->uniformMatrix4fv("MVP", MVP);
        debugShader->uniform3f("inColor", colour.x, colour.y, colour.z);

        glBindBuffer(GL_ARRAY_BUFFER, debugQuad->vertexBufferID);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(0,  // attribute 0. No particular reason for 0,
                                  // but must match the layout in the shader.
                              3,               // count
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

        glDrawArrays(hollow ? GL_LINE_LOOP : GL_TRIANGLES, 0, debugQuad->Vertices.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }

    void m_2dRenderer::renderCircle(vec3 position, float radius, bool hollow) {
        builtinCircleShader->bind();

        position.z += 1;

        mat4 MVP = projectionMatrix * viewMatrix * (translate(mat4(1.0f), position) * scale(mat4(1.0f), vec3(radius)));

        builtinCircleShader->uniformMatrix4fv("MVP", MVP);
        builtinCircleShader->uniform1i("hollow", hollow);

        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, debugQuad->vertexBufferID);

        glVertexAttribPointer(0,  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                              3,  // size
                              GL_FLOAT,        // type
                              GL_FALSE,        // normalized?
                              sizeof(Vertex),  // stride
                              (void *)0        // array buffer offset
        );
        GL_LOG("Atrib pointer");

        glVertexAttribPointer(1,  // attribute. No particular reason for 1, but must match the layout in the shader.
                              2,  // size : U+V => 2
                              GL_FLOAT,        // type
                              GL_FALSE,        // normalized?
                              sizeof(Vertex),  // stride
                              (void *)12       // array buffer offset
        );

        GL_LOG("draw arrays ");
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        GL_LOG("Render");
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////// Class commands //////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    m_2dRenderer::m_2dRenderer(Screen &_s) : screen(_s) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        projectionMatrix = glm::ortho(0.0f,                 // left
                                      (float)screen.sizeX,  // right
                                      0.0f,                 // bottom
                                      (float)screen.sizeY,  // top
                                      0.0f,                 // zNear
                                      100.0f                // zFar
        );

        // Camera matrix
        viewMatrix = glm::lookAt(glm::vec3(0, 0, 6),  // Camera is at (2,3,3), in World Space
                                 glm::vec3(0, 0, 0),  // and looks at the origin
                                 glm::vec3(0, 1,
                                           0)  // Head is up (set to 0,-1,0 to look upside-down)
        );

        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);
        GL_LOG("bind vertex array");

        debugQuad = make_shared<Renderable>(Rectangle());

        buffer(debugQuad.get());

        std::cout << "starting renderer init" << std::endl;

        programID =
            LoadShaders("shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader");

        debugCenterShader = LoadShaders("shaders/centerDebug.vertexshader", "shaders/centerDebug.fragmentshader");

        debugShader =
            shared_ptr<Shader>(new Shader("shaders/debug/red.vertexshader", "shaders/debug/red.fragmentshader"));
        debugLineShader =
            shared_ptr<Shader>(new Shader("shaders/debug/line.vertexshader", "shaders/debug/line.fragmentshader"));

        builtinCircleShader = shared_ptr<Shader>(
            new Shader("shaders/Builtin/circle.vertexshader", "shaders/Builtin/circle.fragmentshader"));

        program = shared_ptr<Shader>(
            new Shader("shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader"));
        // instanceShader = shared_ptr<Shader>(new
        // Shader("shaders/Instance.vertexshader",
        // "shaders/Instance.fragmentshader"));

        GL_LOG("Renderer init");

        GLfloat lineWidthRange[2] = {0.0f, 0.0f};
        glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, lineWidthRange);
        LOG(lineWidthRange[1]);

        LOG("!!! HARDWARE INFO !!!");
        LOG("Min line width: " << lineWidthRange[0]);
        LOG("Max line width: " << lineWidthRange[1]);
    }

    m_2dRenderer::~m_2dRenderer() {
        // glDeleteProgram(programID);
        // glDeleteProgram(debugLineShaderID);
        // glDeleteProgram(debugShaderID);
        glDeleteProgram(debugCenterShader);
        glDeleteVertexArrays(1, &VertexArrayID);

        // for (auto v : objects)
        //   if (!dynamic_cast<Renderable>(v))
        //     delete v;

        objects.clear();

        std::cout << "Cleaning up renderer" << std::endl;
    }

}  // namespace Entropy
