#include <managers/bullet.hpp>
#include <managers/global.hpp> 
#include <managers/time.hpp> 
#include <managers/render/render.hpp> 
#include <managers/window/window_manager.hpp> 
#include <managers/window/imgui.hpp> 

#include <scene/scene.hpp>
// #include <entities/templates/decor/grass.hpp>
#include <entities/templates/decor/house.hpp>
#include <entities/templates/decor/stone_floor.hpp>
// #include <entities/templates/decor/brick_sphere.hpp>
#include <entities/templates/decor/wooden_box.hpp>
// #include <entities/templates/decor/tree.hpp>
#include <entities/templates/decor/skybox.hpp>
// #include <entities/templates/decor/test_dq.hpp>

#include <entities/templates/mobs/female.hpp>
#include <entities/templates/mobs/dual_quat_skining.hpp>
#include <entities/templates/mobs/matrix_skining.hpp>
#include <entities/templates/playable/Ghost.hpp>

#include <stb_image_write.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <locale>
#include <codecvt>



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

void createScene(Scene *scene)
{
    Ghost::Initialize();
    Skybox::Initialize();
    StoneFloor::Initialize();
    WoodenBox::Initialize();
    House::Initialize();
    Female::Initialize();
    // DualQuatSkining::Initialize();
    MatrixSkining::Initialize();

    scene->skybox = new Skybox();

    Actor *character = new Ghost();
    // character->Teleport(glm::vec3(0, 2, 0));
    scene->pushObject(character);

    auto female = new Female();
    // female->SetRotation(glm::vec3(0, 0, 0));
    female->Teleport(glm::vec3(4, 0.0, 3.0));
    // female->SetScale(glm::vec3(0.06));
    scene->pushObject(female);

    auto female2 = new Female();
    // female2->SetRotation(glm::vec3(0, 0, 0));
    female2->Teleport(glm::vec3(0, 5.0, 0.0));
    // female2->SetScale(glm::vec3(0.06));
    scene->pushObject(female2);


    auto stoneFloor = new StoneFloor();
    stoneFloor->Teleport(glm::vec3(0, -0.5, 0.0));
    stoneFloor->SetRotation(glm::vec3(-90, 0.0, 0.0));
    scene->pushObject(stoneFloor);

    // float f = 0;
    // for (int i = 0; i < 1; i++) {
    //     auto wBox = new WoodenBox();
    //     wBox->Teleport(glm::vec3(-3, 2.0 + f, -4.0));
    //     scene->pushObject(wBox);
    //     f += 2.5;
    // }

        // auto wBox = new WoodenBox();
        // wBox->Teleport(glm::vec3(0.0f, 8.0f, 1.0f));
        // scene->pushObject(wBox);

    // auto house = new House();
    // house->Teleport(glm::vec3(0.0f, 8.0f, 1.0f));
    // house->SetScale(glm::vec3(20.0f));
    // scene->pushObject(house);


    // auto ms = new MatrixSkining();
    // // ms->SetRotation(glm::vec3(-90, 0, 0));
    // ms->Teleport(glm::vec3(4, 1.0, 8.0));
    // // ms->SetScale(glm::vec3(0.05));
    // // ms->SetScale(glm::vec3(50));
    // scene->pushObject(ms);

    // auto dq = new DualQuatSkining();
    // dq->SetRotation(glm::vec3(-90, 0, 0));
    // dq->Teleport(glm::vec3(0, 0.0, 8.0));
    // dq->SetScale(glm::vec3(0.05));
    // scene->pushObject(dq);

    GlobalState::SetPlayer(character);
}

int main(int argc, char** argv)
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::locale::global(std::locale("en_US.UTF-8"));

    const GLfloat width = 1920, height = 992;

    WindowManager::Initialize(width, height);
    RenderManager::Initialize();
    BulletManager::Initialize();
    TimeManager::Initialize();
    ImGuiManager::Initialize();

    WindowManager::curWindow->scene = new Scene();
    createScene(WindowManager::curWindow->scene);

    // WindowManager::SetWindow(WindowManager::windows.at("Engine2").get());
    // WindowManager::curWindow->scene = new Scene();
    // ComponentCamera* camera = new ComponentCamera();
    // camera->camera.SetPerspectiveProj(90.0f, WindowManager::curWindow->GetWidth(), WindowManager::curWindow->GetHeight(), 0.5f, 512.0f);
    // WindowManager::curWindow->scene->SetCamera(camera);
    // glEnable(GL_DEPTH_TEST);
    // glClearColor(0.12f, 0.12f, 0.12f, 0.0f);

    while (GlobalState::fIsAppRunning) {
        TimeManager::Update();
        // WindowManager::SetWindow(WindowManager::windows.at("Engine").get());
        WindowManager::curWindow->scene->Callback(TimeManager::GetDeltaTime());
        ImGuiManager::Render(WindowManager::curWindow->scene);
        WindowManager::SwapBuffer();
        
        // WindowManager::SetWindow(WindowManager::windows.at("Engine2").get());
        // WindowManager::curWindow->scene->Callback(TimeManager::GetDeltaTime());
        // ImGuiManager::Render(WindowManager::curWindow->scene);
        // WindowManager::SwapBuffer();
    }

    ImGuiManager::Dispose();
    WindowManager::Dispose();
    BulletManager::Dispose();

    return 0;
}
