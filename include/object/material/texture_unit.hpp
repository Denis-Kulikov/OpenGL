#pragma once
#include <GL/glew.h>
#include <string>
#include <unordered_map>

class Texture;

class TextureUnit {
public:
    static const GLuint NotActivated = 0xFFFF;

    enum TextureType {
        ALBEDO,
        BASE_COLOR,
        SPECULAR,
        NORMALS,
        HEIGHT,
        EMISSIVE,
        SHADOW_MAP_CUBE,
        SHADOW_MAP_DIR
    };

    inline static const std::unordered_map<TextureType, std::string> Types = {
        { ALBEDO,  "albedoMap"  },
        { SPECULAR, "specularMap" },
        { NORMALS,  "normalMap"   },
        { HEIGHT,   "heightMap"   },
        { EMISSIVE, "emissiveMap" },
        { SHADOW_MAP_CUBE, "shadowCubeMap" },
        { SHADOW_MAP_DIR, "shadowMapDirLight" }
    };

    TextureUnit(Texture* texture, TextureType type);
    void Bind() const;

    Texture* texture;
    TextureType type;
    GLuint unit = NotActivated;
};
