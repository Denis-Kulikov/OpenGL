#include <scene/shadow/shadow_directional_light.hpp>
#include <scene/scene.hpp>

// Код из статьи https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
ShadowDirectionalLight::ShadowDirectionalLight(GLsizei SHADOW_WIDTH, GLsizei SHADOW_HEIGHT)
    : ShadowMap(SHADOW_WIDTH, SHADOW_HEIGHT)
{
    glGenFramebuffers(1, &FBO); 

    glGenTextures(1, &map);
    glBindTexture(GL_TEXTURE_2D, map);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, map, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    near = 2.f;
    far = 24.0f;

    Texture::Create("ShadowMapDirLight", map, SHADOW_WIDTH / SHADOW_HEIGHT);
}

#include <stb_image_write.h>

void SaveDepthMap(GLuint Map, int size, const std::string& Name)
{
    std::vector<float> depthData(size * size);
    std::vector<unsigned char> image(size * size);

    glBindTexture(GL_TEXTURE_2D, Map);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, depthData.data());

    for (int i = 0; i < size * size; ++i) {
        image[i] = static_cast<unsigned char>(depthData[i] * 255.0f);
    }
    
    stbi_write_png(Name.c_str(), size, size, 1, image.data(), size);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ShadowDirectionalLight::ShadowPass(const Scene* scene, const DirectionalLight* light, const glm::vec3& offset) {
    glm::vec3 target   = glm::vec3(0.0f);
    glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 lightDir = glm::normalize(light->direction);
    glm::vec3 lightPos = lightDir * 10.0f;;

    glm::mat4 lightView = LookAtLH(lightPos, target, up);

    float orthoSize = 20.0f;
    glm::mat4 lightProjection = OrthoLH(
        -orthoSize, orthoSize,
        -orthoSize, orthoSize,
        GetNear(), GetFar()
    );

    lightSpaceMatrix = lightProjection * lightView;

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    for (const auto &it : scene->actors) 
        it->RenderShadowPass(lightSpaceMatrix, ORTHO);

    // SaveDepthMap(map, SHADOW_WIDTH, "map.png");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
