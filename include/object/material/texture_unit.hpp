#include "texture.hpp"
#include "shader.hpp"

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


    TextureUnit(Texture* Texture, TextureType type)
        : Texture(Texture), type(type)
    {}
    
    void Bind() const {
        glActiveTexture(GL_TEXTURE0 + unit);
        Texture->Bind();
    }

    bool Link(Shader* shader, GLuint i) {
        const auto& texName = Types.find(type);
        if (texName == Types.end())
            return false;

        const auto& loc = shader->uniforms.find(texName->second);
        if (loc == shader->uniforms.end())
            return false;

        unit = i;
        glUniform1i(loc->second.location, unit);
    
        return true;
    }

    Texture* Texture;
    TextureType type;
    GLuint unit;
};
