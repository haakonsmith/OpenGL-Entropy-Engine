
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#pragma once

namespace Entropy
{


    class Texture
    {
    private:
        GLuint textureID;
        GLenum textureSlot;
    public:
        Texture(/* args */) {
            
        }
        ~Texture();

        inline void bind() {
            glBindTexture(textureSlot, textureID);
        }
    };
    
} // namespace Entropy
