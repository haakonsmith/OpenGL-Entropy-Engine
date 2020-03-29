/*
 * Copyright 2020, Haakon Smith.
 */

#include <array>

#include "../Shared.hpp"
#include "BufferObject.hpp"
#include "Renderable.hpp"
#include "glm/glm.hpp"

using namespace glm;

#pragma once

namespace Entropy {

    template <uint32 C>
    class RenderInstance {
      private:
        uint32 bo = 0;

      public:
        std::array<vec3, C> PositionData;
        Scope<BufferObject<GL_ARRAY_BUFFER>> PositionBuffer;
        Ref<Renderable> Instanced;

        RenderInstance(Ref<Renderable> instanced) : Instanced(instanced) {
            PositionData.fill(vec3(0));
            PositionBuffer = std::unique_ptr<BufferObject<GL_ARRAY_BUFFER>>(
                new BufferObject<GL_ARRAY_BUFFER>(
                    bo, C * sizeof(vec3), PositionData.data(), GL_STREAM_DRAW));
            GL_LOG("instance creation");
        }
        ~RenderInstance() {}

        void update(const std::array<vec3, C> &data) {
            PositionData = data;

            PositionBuffer->SubBuffer(0, C * sizeof(vec3), PositionData.data());
        }

        uint32 getCount() { return C; }
    };

}  // namespace Entropy
