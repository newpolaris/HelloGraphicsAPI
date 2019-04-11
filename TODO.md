```
#if 0
{
    // Load the coordinate
    gl::VertexAttribPointer(m_aPositionHandle, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(FLOAT), vtxs);
    gl::VertexAttribPointer(m_aTexHandle, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(FLOAT), &vtxs[3]);

    if (bTriangleFan)
        gl::DrawArrays(GL_TRIANGLE_FAN, 0, 4);
    else {
        std::array<uint16_t, 6> indices = { 0, 1, 2, 0, 2, 3 };
        gl::DrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, indices.data()); 
    }
}

void SubDrawTexture(float* vtxs)
{
    // Load the coordinate
    glVertexAttribPointer(m_aPositionHandle, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), vtxs);
    glVertexAttribPointer(m_aTexHandle, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), &vtxs[3]);

    float vNormalVector[12] = { 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f };
    glVertexAttribPointer(m_aNormalVector, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), vNormalVector);

    // Drawing
    if (bTriangleFan)
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    else {
        std::array<uint16_t, 6> indices = { 0, 1, 2, 0, 2, 3 };
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, indices.data()); 
    }
}

{
    std::array<uint16_t, 6> indices = { 0, 1, 2, 0, 2, 3 };
    GL_CHECK(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, indices.data()));

    const GLbyte* offset = 0;
    const GLuint fanCount = 4;
    const GLuint count = (fanCount - 2) * 3;

    GLuint ibo = 0;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, params.numIndices * params.sizeofIndices, params.indices, GL_STATIC_DRAW);

    glDrawElementsBaseVertex(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, nullptr, offset);
}
#endif

```