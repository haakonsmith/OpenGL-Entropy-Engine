#include "../Texture.hpp"

#include "stb/stb_image.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace Entropy
{
    GLuint Texture::loadTexture(std::string path) {
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


        // "Bind" the newly created texture : all future texture functions will
        // modify this texture
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Give the image to OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Return the ID of the texture we just created
        return textureID;
    }
} // namespace Entropy
