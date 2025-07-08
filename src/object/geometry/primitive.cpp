#include <object/geometry/primitive.hpp>

GeometryPrimitive::~GeometryPrimitive() {
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    if (EBO != 0) glDeleteBuffers(1, &EBO);
}

void GeometryPrimitive::Bind() const {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void GeometryPrimitive::Draw(void* data) const {
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}
