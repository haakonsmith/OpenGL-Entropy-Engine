
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

using glm::vec2;
using glm::vec3;

#pragma once

namespace Entropy {

    namespace Mixin {

        class Transform {
          public:
            virtual void setPosition(const vec3 &v) = 0;
            virtual void setPosition(const vec2 &v) = 0;
            virtual void setPosition(float x, float y) = 0;
            virtual void setPosition(float x, float y, float z) = 0;
            virtual vec3 getPosition() = 0;

            virtual void setScale(const vec3 &v) = 0;
            virtual void setScale(float x, float y) = 0;
            virtual void setScale(float x, float y, float z) = 0;
            virtual void setScale(const vec2 &v) = 0;
            virtual vec3 getScale() = 0;

            virtual void setRotation(float r) = 0;
            virtual float getRotation() = 0;
        };

    }  // namespace Mixin

    struct Position : public virtual Mixin::Transform{
        vec3 position;
        vec3 scale;
        float rotation;

        inline void setPosition(const vec3 &v) override { position = v; }
        inline void setPosition(const vec2 &v) override { setPosition(vec3(v, position.z)); }
        inline void setPosition(float x, float y) override { setPosition(vec3(x, y, position.z)); }
        inline void setPosition(float x, float y, float z) override { setPosition(vec3(x, y, z)); }
        inline vec3 getPosition() override { return position; }

        inline void setScale(const vec3 &v) override { scale = v; }
        inline void setScale(float x, float y) override { setScale(vec3(x, y, scale.z)); }
        inline void setScale(float x, float y, float z) override { setScale(vec3(x, y, z)); }
        inline void setScale(const vec2 &v) override { setScale(vec3(v, scale.z)); }
        inline vec3 getScale() override { return scale; }

        inline float getRotation() override { return rotation; }
        inline void setRotation(float r) override { rotation = r; }
    };

}  // namespace Entropy
