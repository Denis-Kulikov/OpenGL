#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

class Geometry {
public:
    int GetType() {return type;}
    virtual void Bind() const = 0;
    virtual void Draw(void* data = nullptr) const = 0;

    enum Type {
        PRIMITIVE,
        MESH
    };

protected:
    void SetType(int new_type) {type = new_type;};

private:
    int type;
};
