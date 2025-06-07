#pragma once
#include "base.hpp"

class GeometryPrimitive : public Geometry {
public:
    ~GeometryPrimitive();

    virtual void Bind() const override;
    virtual void Draw(void* data = nullptr) const override;

    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    GLint numVertices = 0;
    GLint numIndices = 0;
};

