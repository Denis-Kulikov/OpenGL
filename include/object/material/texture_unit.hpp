#pragma once
#include "texture.hpp"

class TextureUnit {
public:
    enum TextureType {
        DIFFUSE,
        BASE_COLOR,
        SPECULAR,
        NORMALS,
        HEIGHT,
        EMISSIVE
    };

    inline static const std::unordered_map<TextureType, std::string> Types = {
        { DIFFUSE,  "diffuseMap"  },
        { SPECULAR, "specularMap" },
        { NORMALS,  "normalMap"   },
        { HEIGHT,   "heightMap"   },
        { EMISSIVE, "emissiveMap" }
    };

    TextureUnit(Texture* texture, TextureType type);
    void Bind() const;

    Texture* texture;
    TextureType type;
    GLuint unit;
};
