#pragma once
#include "base.hpp"

class GeometryPrimitive : public Geometry {
public:
    ~GeometryPrimitive() {
        if (VAO != 0) glDeleteVertexArrays(1, &VAO);
        if (VBO != 0) glDeleteBuffers(1, &VBO);
        if (EBO != 0) glDeleteBuffers(1, &EBO);
    }

    virtual void Bind() const override {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    }

    virtual void Draw(void* data = nullptr) const override {
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    }

    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    GLint numVertices = 0;
    GLint numIndices = 0;
};

