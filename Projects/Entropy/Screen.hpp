#include "Shared.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#pragma once

namespace Entropy {

    class Screen {
        friend class BaseApplication;

      public:
        Screen(unsigned int _x, unsigned int _y) : x(_x), y(_y) {}
        union {
            struct {
                unsigned int x, y;
            };
            struct {
                unsigned int sizeX, sizeY;
            };
            glm::vec2 size;
        };

        ~Screen() {}

        /*
         * Gets -1 to 1 float local space esque coordinates.
         */
        glm::vec3 localSpace(glm::vec3 worldSpaceCoords) {
            return glm::vec3(localSpace(glm::vec2(worldSpaceCoords)), worldSpaceCoords.z);
        }
        glm::vec2 localSpace(glm::vec2 worldSpaceCoords) {
            worldSpaceCoords.x = ((worldSpaceCoords.x * 2) / sizeX) - 1;
            worldSpaceCoords.y = ((worldSpaceCoords.y * 2) / sizeY) - 1;

            return worldSpaceCoords;
        }

        glm::vec3 worldSpace(glm::vec3 localSpaceCoords) {
            return glm::vec3(worldSpace(glm::vec2(localSpaceCoords)), localSpaceCoords.z);
        }

        glm::vec2 worldSpace(glm::vec2 localSpaceCoords) {
            localSpaceCoords.x = (localSpaceCoords.x * sizeX + sizeX) / 2;
            localSpaceCoords.y = (localSpaceCoords.y * sizeY + sizeY) / 2;

            return localSpaceCoords;
        }
    };

    namespace App {
        extern Screen screen;
    };

}  // namespace Entropy