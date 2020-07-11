#include <vector>

#include "../Graphics/AtribLayout.hpp"
#include "../Graphics/Shader.hpp"
#include "../Graphics/Texture.hpp"
#include "../Graphics/Vertex.hpp"
#include "../Graphics/VertexBuffer.hpp"

#pragma once

namespace Entropy {

    struct RenderData {
        Ref<Shader> shader;
        VertexArray arrayBuffer;
        Texture texture;
        Ref<VertexBuffer> vertexBuffer;

        vector<struct Vertex> Vertices;
    };

}  // namespace Entropy
