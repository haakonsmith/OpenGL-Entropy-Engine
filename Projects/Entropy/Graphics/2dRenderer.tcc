namespace Entropy {
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////// Render commands /////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <uint32 C>
    void m_2dRenderer::renderInstance(const RenderInstance<C> &Instance, uint32 renderCount) {
        glBindBuffer(GL_ARRAY_BUFFER, Instance.Instanced->vertexBufferID);

        Instance.Instanced->shader->Bind();
        GL_LOG("bind shader ");

        Instance.Instanced->shader->UniformMatrix4fv("MVP", Instance.Instanced->MVP);
        GL_LOG("bind shader ");

        glActiveTexture(GL_TEXTURE0);
        GL_LOG("Active Texture");
        glBindTexture(GL_TEXTURE_2D, Instance.Instanced->texture);
        GL_LOG("Bind Texture");

        // Set our "myTextureSampler" sampler to use Texture Unit 0
        Instance.Instanced->shader->Uniform1i("myTextureSampler", 0);
        GL_LOG("Use shader");

        // 1st attribute buffer : Vertices

        GL_LOG("Atrib pointer");

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0,  // attribute 0. No particular reason for 0, but must match
                                  // the layout in the shader.
                              3,               // count
                              GL_FLOAT,        // type
                              GL_FALSE,        // normalized?
                              sizeof(Vertex),  // stride
                              (void *)0        // array buffer offset
        );
        GL_LOG("Atrib ptr");

        glEnableVertexAttribArray(1);
        GL_LOG("Atrib ptr");
        glVertexAttribPointer(1,  // attribute. No particular reason for 1, but must match the
                                  // layout in the shader.
                              2,               // size : U+V => 2
                              GL_FLOAT,        // type
                              GL_FALSE,        // normalized?
                              sizeof(Vertex),  // stride
                              (void *)12       // array buffer offset
        );
        GL_LOG("Atrib ptr");

        glEnableVertexAttribArray(2);
        GL_LOG("Atrib instance");
        Instance.PositionBuffer->Bind();
        GL_LOG("Bind instance position buffer");
        glVertexAttribPointer(2,  // attribute 0. No particular reason for 0, but must match
                                  // the layout in the shader.
                              3,             // count
                              GL_FLOAT,      // type
                              GL_FALSE,      // normalized?
                              sizeof(vec3),  // stride
                              (void *)0      // array buffer offset
        );

        GL_LOG("Render");
        glVertexAttribDivisor(0, 0);  // particles vertices : always reuse
                                      // the same 4 vertices -> 0
        glVertexAttribDivisor(1, 0);  // positions : one per quad (its center) -> 1
        glVertexAttribDivisor(2, 1);  // color : one per quad -> 1

        glDrawArraysInstanced(GL_TRIANGLES, 0, Instance.Instanced->Vertices.size(),
                              (renderCount < C) ? renderCount : C);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }
}  // namespace Entropy
