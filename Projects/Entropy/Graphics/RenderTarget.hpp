/*
 * Copyright 2020, Haakon Smith.
 */

#include "Texture.hpp"

namespace Entropy {

    struct RenderTarget {
        GLuint FrameBuffer;
        Texture texture;
    };
}  // namespace Entropy
