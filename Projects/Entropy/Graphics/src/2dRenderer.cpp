#include "../2dRenderer.hpp"

#include "stb/stb_image.h"

namespace Entropy {

    void m_2dRenderer::addRenderable(Renderable *_renderable) {
        // renderables don't come prepackaged with MVP, so it needs to be
        // created.
        buffer(_renderable);

        if (_renderable->castsShadow) renderables.push_back(_renderable);
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

        _renderable->arrayBuffer.bind();

        glBindBuffer(GL_ARRAY_BUFFER, _renderable->vertexBufferID);

        glBufferData(GL_ARRAY_BUFFER, _renderable->Vertices.size() * sizeof(Vertex), &_renderable->Vertices.front(),
                     GL_STATIC_DRAW);

        GL_LOG("add buffer data ");

        Vertex::assertLayout();
        _renderable->arrayBuffer.unbind();
        GL_LOG("add buffer data ");
        // glBindBuffer(GL_ARRAY_BUFFER, 0);
        // _renderable->vertexBufferID = (true);
    }

    void m_2dRenderer::buffer(Renderable *_renderable) {
        _renderable->transform.compute();

        if (!_renderable->vertexBufferINIT) genVertexBuffer(_renderable);

        if (!_renderable->TextureINIT) _renderable->setTexture("bullet.png");

        if (_renderable->shader == nullptr) _renderable->shader = program;
        GL_LOG("finish buffer ");
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////// Math commands ///////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

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
        PROFILE_FUNCTION();

        // bindRenderTarget("scene");
        // glClear(GL_COLOR_BUFFER_BIT);

        for (auto obj : objects) { render(obj); }

        unbindRenderTarget();

        // glViewport(0, 0, 640 * 2, 480 * 2);
        vertexArray.bind();

        program->bind();

        if (debugOutline) {
            for (auto obj : objects) renderOutline(*obj);
        }
    }

    void m_2dRenderer::render(Renderable *_renderable) {
        PROFILE_FUNCTION();
        _renderable->shader->bind();

        _renderable->shader->uniformMatrix4fv("MVP",
                                              projectionMatrix * viewMatrix * _renderable->transform.modelMatrix);

        // Bind texture in Texture Unit 0
        _renderable->texture.bind();

        // Set texture sampler to use Texture Unit 0
        _renderable->shader->uniform1i("myTextureSampler", 0);

        _renderable->arrayBuffer.bind();

        glDrawArrays(GL_TRIANGLES, 0, _renderable->Vertices.size());

        vertexArray.bind();
    }

    void m_2dRenderer::renderLine(const vec3 &p1, const vec3 &p2) {
        // PROFILE_FUNCTION();
        // vertexArray.bind();
        // Vertex verts[] = {Vertex(screen.localSpace(p1)), Vertex(screen.localSpace(p2))};

        // VertexBuffer vbo(0, sizeof(verts), verts, GL_STREAM_DRAW);
        // debugLineShader->bind();
        // vbo.bind();

        // Vertex::assertLayout();

        // glDrawArrays(GL_LINES, 0, 2);  // Starting from vertex 0; 3 Vertices total . 1 RightTriangle
        // GL_LOG("draw arrays ");
    }

    void m_2dRenderer::render(entt::entity entity, entt::registry& context) {
        PROFILE_FUNCTION();

        auto& transform = context.get<Transform>(entity);
        auto& renderData = context.get<RenderData>(entity);
        renderData.shader->bind();

        renderData.shader->uniformMatrix4fv("MVP",
                                              projectionMatrix * viewMatrix * transform.modelMatrix);

        // Bind texture in Texture Unit 0
        renderData.texture.bind();

        // Set texture sampler to use Texture Unit 0
        renderData.shader->uniform1i("myTextureSampler", 0);

        renderData.arrayBuffer.bind();

        glDrawArrays(GL_TRIANGLES, 0, renderData.Vertices.size());

        vertexArray.bind();
    }

    void m_2dRenderer::renderOutline(const Renderable &_renderable) {
        PROFILE_FUNCTION();
        debugShader->bind();

        debugShader->uniformMatrix4fv("MVP", getViewProjectionMatrix() * _renderable.transform.modelMatrix);
        debugShader->uniform3f("inColor", 1, 1, 0);

        _renderable.arrayBuffer.bind();

        glDrawArrays(GL_LINE_LOOP, 0,
                     _renderable.Vertices.size());  // Starting from vertex 0; 3 Vertices

        GL_LOG("Render");
    }

    void m_2dRenderer::renderCenter(Renderable *_renderable) {}

    void m_2dRenderer::renderQuad(vec3 position, float width, float height, bool hollow, vec3 colour) {
        PROFILE_FUNCTION();

        const mat4 MVP = getViewProjectionMatrix() *
                         (glm::translate(mat4(1.0f), (position)) * glm::scale(mat4(1.0f), vec3(width, height, 1)));

        debugShader->bind();

        debugShader->uniformMatrix4fv("MVP", MVP);
        debugShader->uniform3f("inColor", colour.x, colour.y, colour.z);

        debugQuad->arrayBuffer.bind();

        glDrawArrays(hollow ? GL_LINE_LOOP : GL_TRIANGLES, 0, debugQuad->Vertices.size());
    }

    void m_2dRenderer::renderCircle(vec3 position, float radius, bool hollow) {
        PROFILE_FUNCTION();
        builtinCircleShader->bind();

        position.z += 1;

        mat4 MVP = getViewProjectionMatrix() * (translate(mat4(1.0f), position) * scale(mat4(1.0f), vec3(radius)));

        builtinCircleShader->uniformMatrix4fv("MVP", MVP);
        builtinCircleShader->uniform1i("hollow", hollow);

        debugQuad->arrayBuffer.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        vertexArray.bind();
        GL_LOG("Render");
    }

    void m_2dRenderer::compositeTextures(Texture &texture1, Texture &texture2, shared_ptr<Shader> layerMergeShader) {
        PROFILE_FUNCTION();
        const mat4 MVP = getViewProjectionMatrix() *
                         (glm::translate(mat4(1.0f), (vec3(320, 240, 0))) * glm::scale(mat4(1.0f), vec3(320, 240, 1)));

        texture1.bind(GL_TEXTURE0);
        texture2.bind(GL_TEXTURE1);
        GL_LOG("bind vertex array");

        layerMergeShader->bind();
        GL_LOG("bind vertex array");

        layerMergeShader->uniformMatrix4fv("VP", MVP);
        layerMergeShader->uniform1i("texSampler", 0);
        layerMergeShader->uniform1i("texSampler1", 1);

        debugQuad->arrayBuffer.bind();
        GL_LOG("bind vertex array");

        glDrawArrays(GL_TRIANGLES, 0, debugQuad->Vertices.size());
        GL_LOG("bind vertex array");
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////// Class commands //////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    m_2dRenderer::m_2dRenderer(Screen &_s) : screen(_s), LightRendererAttachment() {
        glEnable(GL_BLEND);
        glScissor(0, 0, screen.sizeX * 2, screen.sizeY * 2);
        glViewport(0, 0, screen.sizeX * 2, screen.sizeY * 2);
        glEnable(GL_SCISSOR_TEST);
        glEnable(GL_MULTISAMPLE);  
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

        viewProjectionMatrix = projectionMatrix * viewMatrix;

        vertexArray.bind();
        GL_LOG("bind vertex array");

        debugQuad = make_shared<Renderable>(Rectangle());

        createRenderTarget("scene");
        createRenderTarget("light");

        buffer(debugQuad.get());

        std::cout << "starting renderer init" << std::endl;

        debugShader =
            shared_ptr<Shader>(new Shader("shaders/debug/red.vertexshader", "shaders/debug/red.fragmentshader"));
        debugLineShader =
            shared_ptr<Shader>(new Shader("shaders/debug/line.vertexshader", "shaders/debug/line.fragmentshader"));

        builtinCircleShader = shared_ptr<Shader>(
            new Shader("shaders/Builtin/circle.vertexshader", "shaders/Builtin/circle.fragmentshader"));

        program = shared_ptr<Shader>(
            new Shader("shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader"));

        mergeShader = shared_ptr<Shader>(
            new Shader("shaders/Builtin/Lighting/mesh.vertexshader", "shaders/Builtin/Lighting/merge.fragmentshader"));
        // instanceShader = shared_ptr<Shader>(new
        // Shader("shaders/Instance.vertexshader",
        // "shaders/Instance.fragmentshader"));

        GL_LOG("Renderer init");

        GLfloat lineWidthRange[2] = {0.0f, 0.0f};
        glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, lineWidthRange);

        LOG("!!! HARDWARE INFO !!!");
        LOG("Min line width: " << lineWidthRange[0]);
        LOG("Max line width: " << lineWidthRange[1]);
    }

    m_2dRenderer::~m_2dRenderer() {
        objects.clear();

        std::cout << "Cleaning up renderer" << std::endl;
    }

}  // namespace Entropy
