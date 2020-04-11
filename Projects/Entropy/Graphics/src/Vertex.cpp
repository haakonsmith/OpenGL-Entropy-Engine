#include "../Vertex.hpp"

#include <OpenGL/gl3.h>

namespace Entropy {

    void Vertex::assertLayout() {
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        
        // 0th attribute buffer : Vertices
        glVertexAttribPointer(0,  // Attribute 0. No particular reason for 0,
                                  // but must match the layout in the shader.
                              3,               // Count. Size = 3 * 4 = 12
                              GL_FLOAT,        // Type
                              GL_FALSE,        // Normalized?
                              sizeof(Vertex),  // stride
                              (void *)0        // Array buffer offset
        );
        // 1nd attribute buffer : UVs
        glVertexAttribPointer(1,  // attribute. No particular reason for 1, but
                                  // must match the layout in the shader.
                              2,               // Count : U+V => 2
                              GL_FLOAT,        // Type
                              GL_FALSE,        // Normalized?
                              sizeof(Vertex),  // Stride
                              (void *)12       // Array buffer offset, same as size of previous.
        );
    }
}  // namespace Entropy
