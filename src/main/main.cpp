#include <managers/global_state.hpp> 
#include <managers/render_manager.hpp> 
#include <managers/time_manager.hpp> 
#include <managers/window_manager.hpp> 
#include <managers/bullet_manager.hpp>

#include <object/scene.hpp>
#include <entities/templates/decor/grass.hpp>
#include <entities/templates/decor/stone_floor.hpp>
#include <entities/templates/decor/brick_sphere.hpp>
#include <entities/templates/decor/wooden_box.hpp>
#include <entities/templates/decor/tree.hpp>
#include <entities/templates/decor/skybox.hpp>
#include <entities/templates/decor/test_dq.hpp>

#include <entities/templates/mobs/female.hpp>
#include <entities/templates/mobs/dual_quat_skining.hpp>
#include <entities/templates/mobs/matrix_skining.hpp>
#include <entities/templates/playable/Ghost.hpp>

#include <stb_image_write.h>

glm::vec3 ExtractTranslation(const glm::dualquat& dq) {
    glm::quat t_quat = dq.dual * glm::conjugate(dq.real);
    return 2.0f * glm::vec3(t_quat.x, t_quat.y, t_quat.z);
}

void PrintMatrix(const glm::mat4x3& matrix) {
    for (int row = 0; row < 3; ++row) {
        std::cout << "| ";
        for (int col = 0; col < 4; ++col) {
            std::cout << matrix[col][row] << "\t";
        }
        std::cout << "|\n";
    }
    std::cout << std::endl;
}
void PrintMatrix(const glm::mat4& matrix) {
    for (int row = 0; row < 4; ++row) {
        std::cout << "| ";
        for (int col = 0; col < 4; ++col) {
            std::cout << matrix[col][row] << "\t";
        }
        std::cout << "|\n";
    }
    std::cout << std::endl;
}

std::string printVec3(const glm::vec3& v) {
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
}

std::string printQuat(const glm::quat& q) {
    return "(" + std::to_string(q.x) + ", " + std::to_string(q.y) + ", " + std::to_string(q.z) + ", " + std::to_string(q.w) + ")";
}

glm::vec3 quatToEuler(const glm::quat& q) {
    glm::vec3 euler = glm::eulerAngles(q);
    return glm::degrees(euler);
}

void Callback(Scene *scene) {
    TimeManager::Update();
    BulletManager::StepSimulation(TimeManager::GetDeltaTime());
    RenderManager::UpdateCamera();
    RenderManager::pipeline.UpdateView();
    RenderManager::pipeline.UpdatePV();
    RenderManager::bufferManager.Update();
    // RenderManager::pipeline.drawSkybox(*scene->skybox);
    GlobalState::GetPlayer()->MoveForward();

    for (auto &it : scene->actors) {
        if (it->rootComponent) {
            it->rootComponent->UpdateTree(TimeManager::GetDeltaTime());
        }
        it->Render();
    }

    WindowManager::SwapBuffer();
}

bool fIsInit = false;
Scene *createScene()
{
    Ghost::Initialize();
    Female::Initialize();
    DualQuatSkining::Initialize();
    MatrixSkining::Initialize();

    auto *scene = new Scene();

    Actor *character = new Ghost();
    character->Teleport(glm::vec3(0, 2, 0));
    scene->pushObject(character);

    auto female = new Female();
    female->SetRotation(glm::vec3(0, 0, 0));
    female->Teleport(glm::vec3(-3, 0.0, 4.0));
    female->SetScale(glm::vec3(0.03));
    scene->pushObject(female);

    auto ms = new MatrixSkining();
    ms->SetRotation(glm::vec3(-90, 0, 0));
    ms->Teleport(glm::vec3(4, 0.0, 7.0));
    ms->SetScale(glm::vec3(0.05));
    scene->pushObject(ms);

    auto dq = new DualQuatSkining();
    dq->SetRotation(glm::vec3(-90, 0, 0));
    dq->Teleport(glm::vec3(0, 0.0, 7.0));
    dq->SetScale(glm::vec3(0.05));
    scene->pushObject(dq);


    // scene->skybox = new Skybox();

    GlobalState::SetPlayer(character);

    return scene;
}

int main(int argc, char** argv)
{
    const GLfloat width = 1600, height = 900;

    WindowManager::Initialize(width, height);
    RenderManager::Initialize(70.0f, width, height, 0.1f, 3000.0f);
    BulletManager::Initialize();
    TimeManager::Initialize();

    Scene *scene(createScene());

    while (GlobalState::fIsAppRunning) {
        Callback(scene);
    }

    WindowManager::Dispose();
    BulletManager::Dispose();

    return 0;
}
