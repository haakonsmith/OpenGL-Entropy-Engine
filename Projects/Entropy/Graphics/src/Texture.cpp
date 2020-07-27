#include "../Texture.hpp"

#include <iostream>

#include "stb/stb_image.h"

using std::cerr;
using std::endl;

namespace Entropy {
    Texture::Texture(std::string p) : path(std::string(App::texureAssetPath) + "/" + p) {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        // 32 bit loading
        // float* image = stbi_loadf(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
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
        upload(width, height, image, m_8bits);

        stbi_image_free(image);
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

    void Texture::texParameter(GLenum pname, GLint param) { glTexParameteri(GL_TEXTURE_2D, pname, param); }

    void Texture::upload(int width, int height, const void* data, BitDepth bitDepth) {
        switch (bitDepth) {
            case m_32bits:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, data);
                break;
            case m_8bits:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                break;

        }
    }

}  // namespace Entropy
