#include <algorithm>
#include <scene/shadow/shadow_point_light.hpp>
#include <scene/scene.hpp>

#include <stb_image_write.h>

static float LinearizeDepth(float depth, float kNearPlane, float kFarPlane)
{
    float z = depth * 2.0f - 1.0f; // обратно в clip-space
    return (2.0f * kNearPlane * kFarPlane) / (kFarPlane + kNearPlane - z * (kFarPlane - kNearPlane));
}

void SaveDepthCubeMap(GLuint cubeMap, int size, const std::string& baseName, float kNearPlane, float kFarPlane)
{
    std::vector<float> depthData(size * size);
    std::vector<unsigned char> image(size * size);

    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

    for (int face = 0; face < 6; ++face)
    {
        glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_DEPTH_COMPONENT, GL_FLOAT, depthData.data());

        for (int i = 0; i < size * size; ++i)
        {
            float d = depthData[i];

            if (d >= 1.0f)
                d = kFarPlane;

            float linearDepth = LinearizeDepth(d, kNearPlane, kFarPlane);

            float normalized = (linearDepth - kNearPlane) / (kFarPlane - kNearPlane);
            normalized = 1.0f - normalized; // инвертируем, чтобы ближе = светлее

            image[i] = static_cast<unsigned char>(std::clamp(normalized, 0.0f, 1.0f) * 255.0f);
        }

        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size / 2 - 1; ++x) {
                std::swap(image[y * size + x], image[y * size + size - x - 1]);
            }
        }

        std::string fileName = baseName + "_face" + std::to_string(face) + ".png";
        stbi_write_png(fileName.c_str(), size, size, 1, image.data(), size);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

// Код из статьи https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows
ShadowPointLight::ShadowPointLight(GLsizei SHADOW_SIZE)
    : ShadowMap(SHADOW_SIZE, SHADOW_SIZE)
{
    glGenFramebuffers(1, &FBO);

    glGenTextures(1, &map);
    glBindTexture(GL_TEXTURE_CUBE_MAP, map);
    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
                     SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);  
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, map, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    near = 2.0f;
    far = 16.0f;

    Texture::Create("ShadowMapPoint", map, 1.0f);
}

void ShadowPointLight::ShadowPass(const Scene* scene, const glm::vec3 lightPos) {
    std::vector<glm::mat4> shadowTransforms;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, GetNear(), GetFar());
    shadowTransforms.reserve(6);
    lightPosition = lightPos;

    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1, 0, 0), glm::vec3(0,-1, 0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1, 0, 0), glm::vec3(0,-1, 0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0, 1, 0), glm::vec3(0, 0, 1)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0,-1, 0), glm::vec3(0, 0,-1)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0, 0, 1), glm::vec3(0,-1, 0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0, 0,-1), glm::vec3(0,-1, 0)));


    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    for (int i = 0; i < 6; ++i) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, map, 0);
        glClear(GL_DEPTH_BUFFER_BIT);
        for (const auto &it : scene->actors) 
            it->RenderShadowPass(shadowTransforms[i], SHADOW_CUBE_MAP);
    }

    static bool f = true;
    if (f) {
        SaveDepthCubeMap(map, SHADOW_WIDTH, "cube_map", GetNear(), GetFar());
        f = false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
