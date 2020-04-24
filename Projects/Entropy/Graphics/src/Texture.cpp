#include "../Texture.hpp"

#include <iostream>

#include "stb/stb_image.h"

using std::cerr;
using std::endl;

namespace Entropy {
    Texture::Texture(std::string p) : path(p) {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* image = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

        if (image == nullptr)  // Error check
        {
            cerr << "Error when loading texture from file: " + path << endl;
        }

        // Create one OpenGL texture
        obj_ = createBlank();

        // "Bind" the newly created texture : all future texture functions will
        // modify this texture
        bind();

        setPoorFiltering();

        // Give the image to OpenGL
        upload(width, height, image);
    }

    GLuint Texture::createBlank() {
        GLuint textureID;
        glGenTextures(1, &textureID);
        return textureID;
    }

    void Texture::setPoorFiltering() {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

    void Texture::upload(int width, int height, const void* data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
}  // namespace Entropy
