#include <vector>

#include "../Graphics/AtribLayout.hpp"
#include "../Graphics/Shader.hpp"
#include "../Graphics/Texture.hpp"
#include "../Graphics/Vertex.hpp"
#include "../Graphics/VertexBuffer.hpp"

#pragma once

namespace Entropy {

    struct RenderData {
        size_t bufferObject = 0;
        Ref<Shader> shader;
        VertexArray arrayBuffer;
        Texture texture;
        Ref<VertexBuffer> vertexBuffer;

        vector<struct Vertex> vertices;

        RenderData() : arrayBuffer(), texture("img_test.png") {
            vertices.push_back(Vertex(-1.0f, -1.0f, 0.0f, 0, 0));
            vertices.push_back(Vertex(1.0f, -1.0f, 0.0f, 1, 0));
            vertices.push_back(Vertex(1.0f, 1.0f, 0.0f, 1, 1));

            arrayBuffer.bind();

            vertexBuffer = std::make_shared<VertexBuffer>(bufferObject, vertices.size() * sizeof(Vertex), vertices.data(),
                                                     GL_STATIC_DRAW);

            Vertex::assertLayout();
            arrayBuffer.unbind();

            shader = std::make_shared<Shader>("shaders/Builtin/default.vertexshader", "shaders/Builtin/default.fragmentshader");
        }
    };

}  // namespace Entropy
