
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "Location.hpp"
#include "glm/gtc/matrix_transform.hpp"

using glm::mat4;
using glm::vec3;

#pragma once

namespace Entropy {

    struct Transform : public Location {
        vec3 scale;
        float rotation;

        mat4 modelMatrix;
        mat4 scaleMatrix, translationMatrix, rotationMatrix;


        Transform() : rotation(0), scale(vec3(10)), Location(vec3(0)) { compute(); }
        Transform(vec3 _position, float _rotation = 0, vec3 _scale = vec3(10)) : rotation(_rotation), scale(_scale), Location(_position) { compute(); }
        
        inline void compute() {
            // Model matrix : an identity matrix (model will be at the origin)
            glm::mat4 model = glm::mat4(1.0f);

            rotationMatrix = glm::rotate(model, glm::radians((float) rotation), glm::vec3(0.0f, 0.0f, 1.0f));

            translationMatrix = glm::translate(model, (position));

            scaleMatrix = glm::scale(model, scale);

            setModelMatrix(translationMatrix * rotationMatrix * scaleMatrix);
        }

        inline void setScale(const vec3 &v) { scale = v; }
        inline void setScale(float x, float y) { setScale(vec3(x, y, scale.z)); }
        inline void setScale(float x, float y, float z) { setScale(vec3(x, y, z)); }
        inline void setScale(const vec2 &v) { setScale(vec3(v, scale.z)); }
        inline vec3 getScale() const { return scale; }

        inline float getRotation() const { return rotation; }
        inline void setRotation(float r) { rotation = r; }

        inline void setModelMatrix(mat4 m) { modelMatrix = m; }
        inline mat4 getModelMatrix() const { return modelMatrix; }
    };

}  // namespace Entropy
