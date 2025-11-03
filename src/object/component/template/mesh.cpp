#include <managers/render/render.hpp>
#include <object/component/template/mesh.hpp>
#include <scene/shadow/shadow_map.hpp>
#include <object/material/material.hpp>
#include <object/material/shader.hpp>
#include <managers/window/window_manager.hpp> 
#include <managers/window/window.hpp> 
#include <scene/scene.hpp>


void ComponentMesh::Render() const {
    auto model_mats4x4 = glm::mat4(
        glm::vec4(GetMatrix()[0], 0.0f),
        glm::vec4(GetMatrix()[1], 0.0f),
        glm::vec4(GetMatrix()[2], 0.0f),
        glm::vec4(GetMatrix()[3], 1.0f)
    );

    mesh->material->Set("Model", model_mats4x4);
    mesh->material->Set("dirLightSpaceMatrix", WindowManager::curWindow->GetScene()->shadow.directionalLight.lightSpaceMatrix);

    mesh->material->Set("hasSpecularMap", bool(false));
    mesh->material->Set("roughness", float(0.6f));
    mesh->material->Set("metallic", float(0.02f));
    mesh->material->Set("ambientStrength", float(0.25f));
    mesh->material->Set("specularStrength", float(0.02f));

    mesh->material->Set("lightPos", WindowManager::curWindow->GetScene()->shadow.pointLights.GetPosition());
    mesh->material->Set("farPlanes", WindowManager::curWindow->GetScene()->shadow.pointLights.GetFar());
    mesh->material->Set("nearPlanes", WindowManager::curWindow->GetScene()->shadow.pointLights.GetNear());
    
    mesh->Bind();

    for (const auto& m : mesh->m_Entries) {
        m.material->Bind(mesh->shader);
        m.Draw();
    }
}

void ComponentMesh::RenderShadowPass(const glm::mat4& shadowProj, ShadowMapType type) const {
    if (castsShadow && mesh != nullptr) {
        Shader* shader = nullptr;
        GLuint mLoc, pLoc, lLoc, fLoc;
        auto model = glm::mat4(
            glm::vec4(GetMatrix()[0], 0.0f),
            glm::vec4(GetMatrix()[1], 0.0f),
            glm::vec4(GetMatrix()[2], 0.0f),
            glm::vec4(GetMatrix()[3], 1.0f)
        );

        switch (type)
        {
        case ShadowMapType::ORTHO:
            shader = Shader::Find("ShadowMapOrtho");
            shader->Bind();
            
            mLoc = shader->FindUniform("Model")->location;
            pLoc = shader->FindUniform("ShadowProj")->location;

            glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(shadowProj));
            break;

        case ShadowMapType::PERSPECTIVE:
            shader = Shader::Find("ShadowMapPerspective");
            shader->Bind();
            
            mLoc = shader->FindUniform("Model")->location;
            pLoc = shader->FindUniform("ShadowProj")->location;
            lLoc = shader->FindUniform("lightPos")->location;
            fLoc = shader->FindUniform("farPlane")->location;

            glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(shadowProj));
            glUniform3fv(lLoc, 1, glm::value_ptr(WindowManager::curWindow->GetScene()->lighting.DataSSBO.pointLights[0].position));
            glUniform1f(fLoc, WindowManager::curWindow->GetScene()->shadow.pointLights.GetFar());
            break;
        
        default:
            return;
        }

        mesh->BindGeometry();

        for (const auto& m : mesh->m_Entries)
            m.Draw();
    }
}