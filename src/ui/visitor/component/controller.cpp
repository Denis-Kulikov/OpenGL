#include <ui/visitor/component/controller.hpp>
#include <ui/element/text.hpp>

#include <object/component/template/camera.hpp>
#include <object/component/template/mesh.hpp>
#include <object/component/template/point.hpp>
#include <object/component/template/skeletal_dq_mesh.hpp>
#include <object/component/template/skeletal_matrix_mesh.hpp>
#include <object/component/component_physics.hpp>

#include <object/material/material.hpp>
#include <object/material/shader.hpp>

void DrawTransformSection(Component& c) {
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        glm::vec3 pos = c.GetPosition();
        glm::quat rot = c.GetRotation();
        glm::vec3 scl = c.GetScale();

        float v = 8.0f;

        if (ImGui::DragFloat3("Position", &pos.x, v * 1.6f))
            c.SetPosition(pos);
        if (ImGui::DragFloat3("Scale", &scl.x, v * 0.1f))
            c.SetScale(scl);

        glm::vec3 euler = glm::degrees(glm::eulerAngles(rot));
        if (ImGui::DragFloat3("Rotation", &euler.x, v * 4.0f)) {
            glm::quat newRot = glm::quat(glm::radians(euler));
            c.SetRotation(newRot);
        }
    }
}

void UIVisitorComponentController::Visit(ComponentCamera& c) {
    DrawTransformSection(c);

    if (ImGui::CollapsingHeader("Projection", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Separator();
        ImGui::DragFloat("FOV", &c.camera.PersProj.FOV, 0.1f, 2.5f, 179.0f);
        ImGui::DragFloat("Near Plane", &c.camera.PersProj.zNear, 0.01f, 0.05f, 10.0f);
        ImGui::DragFloat("Far Plane", &c.camera.PersProj.zFar, 1.0f, 10.0f, 10000.0f);

        c.UpdateProj();
    }
}

void UIVisitorComponentController::Visit(ComponentMesh& c) {
    DrawTransformSection(c);

    if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Separator();
        ImGui::Text("Shader: %s", c.mesh->GetShader()->GetName().c_str());
        ImGui::Text("Entries: %d", (int)c.mesh->m_Entries.size());

        if (ImGui::TreeNode("Entries")) {
            int idx = 0;
            for (auto& entry : c.mesh->m_Entries) {
                if (ImGui::TreeNode((void*)(intptr_t)idx, "Entry %d", idx)) {
                    if (entry.material) {
                        if (ImGui::TreeNode("Textures")) {
                            for (auto& tex : entry.material->textureUnits) {
                                ImGui::Text("%s", tex.texture->GetName().c_str());
                                ImGui::Image(
                                    reinterpret_cast<void*>((intptr_t)tex.texture->GetID()),
                                    ImVec2(256, 256),
                                    ImVec2(0, 1),
                                    ImVec2(1, 0)
                                );
                            }
                            ImGui::TreePop();
                        }
                    
                        if (ImGui::TreeNode("Parameters"))  {
                            for (const auto& u : c.mesh->shader->uniforms) {
                                auto it = Material::ConfigurableFields.find(u.first);
                                if (it != Material::ConfigurableFields.end()) {
                                    auto itt = c.mesh->material->values.find(u.first);
                                    if (itt != c.mesh->material->values.end())
                                        c.mesh->material->UniformUIController(u.first);
                                }
                            }
                            ImGui::TreePop();
                        }
                    }
                    ImGui::TreePop();
                }
                idx++;
            }
            ImGui::TreePop();
        }
    }
}

void UIVisitorComponentController::Visit(ComponentPoint& c) {
    DrawTransformSection(c);
}

void UIVisitorComponentController::Visit(ComponentSkeletalDQMesh& c) {
    DrawTransformSection(c);
}

void UIVisitorComponentController::Visit(ComponentSkeletalMatrixMesh& c) {
    DrawTransformSection(c);
}


void UIVisitorComponentController::Visit(ComponentPhysics& c) {
    DrawTransformSection(c);
}
