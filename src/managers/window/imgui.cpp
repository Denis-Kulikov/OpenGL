#include <managers/window/imgui.hpp>
#include <managers/global.hpp> 
#include <managers/render/render.hpp> 
#include <managers/time.hpp> 
#include <managers/window/window.hpp> 
#include <GLFW/glfw3.h>

void ImGuiManager::Initialize(const std::string& version_GLSL) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(WindowManager::window, true);
    ImGui_ImplOpenGL3_Init(version_GLSL.c_str());
}

void ImGuiManager::Dispose()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

ImGuiIO& ImGuiManager::GetIO() {
    return ImGui::GetIO();
}

void ImGuiManager::NewFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::Render(Scene* scene) {
    ImGuiManager::NewFrame();

    DrawMainMenu();
    DrawSceneHierarchy(scene);
    DrawInspector(scene);
    DrawResources();
    DrawPerformance();

    ImGuiManager::Render();
}

void ImGuiManager::Render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::DrawMainMenu() {}
void ImGuiManager::DrawSceneHierarchy(Scene* scene) {
    ImGui::Begin("Scene Hierarchy");
    for (auto actor : scene->actors) {
        ImGui::PushID(actor);
        bool selected = (selectedActor == actor);
        if (ImGui::Selectable(actor->GetName().c_str(), selected)) {
            if (selectedActor == actor)
                selectedActor = nullptr; // повторный клик — снять выделение
            else
                selectedActor = actor;
        }
        ImGui::PopID();
    }
    ImGui::End();
}

void ImGuiManager::DrawInspector(Scene* scene) {
    if (!selectedActor) return;

    ImGui::Begin("Inspector");
    ImGui::Text("Name: %s", selectedActor->GetName().c_str());
    
    Component* comp = selectedActor->rootComponent;
    if (!comp) {
        ImGui::TextDisabled("No Transform component");
        ImGui::End();
        return;
    }

    glm::vec3 pos = comp->GetPosition();
    glm::quat rot = comp->GetRotation();
    glm::vec3 scl = comp->GetScale();

    float v = 8.0f;

    if (ImGui::DragFloat3("Position", &pos.x, v * 0.1f))
        comp->SetPosition(pos);
    if (ImGui::DragFloat3("Scale", &scl.x, v * 0.1f))
        comp->SetScale(scl);

    glm::vec3 euler = glm::degrees(glm::eulerAngles(rot));
    if (ImGui::DragFloat3("Rotation", &euler.x, v * 1.0f)) {
        glm::quat newRot = glm::quat(glm::radians(euler));
        comp->SetRotation(newRot);
    }

    ImGui::End();
}


template<typename Map>
void ShowResourceMapKeys(const char* title, Map const& m) {
    ImGui::Text("%s (%zu)", title, m.size());
    ImGui::Separator();
    ImGui::BeginChild(title, ImVec2(0, 200), true);
    for (auto const& kv : m) {
        const std::string& key = kv.first;
        if (ImGui::Selectable(key.c_str())) {
            // можно сохранять выбранный ресурс где-то глобально/в статике
            // например: selectedResourceName = key;
        }
    }
    ImGui::EndChild();
}

void ShowResourceMapKeys(const char* title, std::unordered_map<std::string, Texture> const& m)
{
    ImGui::Text("%s (%zu)", title, m.size());
    ImGui::Separator();
    ImGui::BeginChild(title, ImVec2(0, 500), true);

    for (auto const& kv : m) {
        const std::string& key = kv.first;
        const Texture& tex = kv.second;

        ImGui::PushID(key.c_str());

        if (ImGui::CollapsingHeader(key.c_str())) {
            ImGui::Text("OpenGL ID: %u", tex.GetID());

            ImGui::Image(
                reinterpret_cast<void*>((intptr_t)tex.GetID()),
                ImVec2(128, 128),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );
        }

        ImGui::PopID();
    }

    ImGui::EndChild();
}


void ImGuiManager::DrawResources() {
    if (ImGui::CollapsingHeader("Resources")) {
        ShowResourceMapKeys("Textures", Texture::cache);
        ShowResourceMapKeys("Shaders", Shader::cache);
        ShowResourceMapKeys("Meshes", Mesh::cache);
    }
}
void ImGuiManager::DrawPerformance() {}
