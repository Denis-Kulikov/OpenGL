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
#include <entities/templates/mobs/female.hpp>
#include <entities/templates/playable/Ghost.hpp>

#include <threads/thread_pool.hpp>

#include <stb_image_write.h>

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

glm::vec3 quatToEuler(const glm::quat& q) {
    glm::vec3 euler = glm::eulerAngles(q);
    return glm::degrees(euler);
}

void saveFrameAsPNG(const unsigned char* pixelData, int width, int height, const std::string& fileName) {
    stbi_write_png(fileName.c_str(), width, height, 3, pixelData, width * 3);
}
void saveFrameAsJPEG(const unsigned char* pixelData, int width, int height, const std::string& fileName) {
    stbi_write_jpg(fileName.c_str(), width, height, 3, pixelData, 90);
}

void flipVertically(unsigned char* data, int width, int height, int channels) {
    int rowSize = width * channels;
    std::vector<unsigned char> tempRow(rowSize);
    for (int y = 0; y < height / 2; ++y) {
        unsigned char* row1 = data + y * rowSize;
        unsigned char* row2 = data + (height - 1 - y) * rowSize;
        memcpy(tempRow.data(), row1, rowSize);
        memcpy(row1, row2, rowSize);
        memcpy(row2, tempRow.data(), rowSize);
    }
}

Tree* tree;
float GetBranchSwingValue(float frequency = 0.7f, float amplitude = 5.0f) {
    using namespace std::chrono;
    static auto startTime = high_resolution_clock::now();
    
    auto now = high_resolution_clock::now();
    float timeSeconds = duration<float>(now - startTime).count();

    // Значение колеблется от -amplitude до +amplitude
    float swing = std::sin(timeSeconds * frequency * 2.0f * 3.14159f) * amplitude;

    // Центр смещения — 50, итоговый диапазон: [40, 60]
    return 20.0f + swing;
}

void Callback(Scene *scene) {
    TimeManager::Update();
    BulletManager::StepSimulation(TimeManager::GetDeltaTime());
    RenderManager::UpdateCamera();
    RenderManager::pipeline.UpdateView();
    RenderManager::pipeline.UpdatePV();
    RenderManager::pipeline.drawSkybox(*scene->skybox);
    GlobalState::GetPlayer()->MoveForward();

    static ThreadPool threadPool(8, 24);

    // tree->rootComponent->children[0]->SetRotation(glm::vec3(GetBranchSwingValue(), 0.f, 0.f));
    // tree->rootComponent->children[0]->children[0]->children[1]->SetRotation(glm::vec3(GetBranchSwingValue(), 0.f, 0.f));

    for (auto &it : scene->actors) {
        it->Render();
        // std::cout << printVec3(it->rootComponent->GetPosition()) << std::endl;
    }


    if (GlobalState::fScreenshot || GlobalState::FPS) {
        int width = RenderManager::PersProj.Width;
        int height = RenderManager::PersProj.Height;

        std::vector<unsigned char> pixels(width * height * 3);
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
        flipVertically(pixels.data(), width, height, 3);

        if (GlobalState::fScreenshot) {
            char filename[64];
            std::vector<unsigned char> pixelsCopy = pixels;
            snprintf(filename, sizeof(filename), "data/screenshots/screenshot_%04d.png", GlobalState::ScreenshotCounter++);
            threadPool.enqueue([pixelsCopy = std::move(pixelsCopy), width, height, filename]() mutable {
                saveFrameAsPNG(pixelsCopy.data(), width, height, filename);
                std::cout << "Screenshot saved: " << filename << std::endl;
            });
            GlobalState::fScreenshot = false;
        }
        if (GlobalState::FPS) {
            char filename[64];
            std::vector<unsigned char> pixelsCopy = pixels;
            snprintf(filename, sizeof(filename), "data/frames/frame_%04d.jpg", GlobalState::FrameCounter++);
            threadPool.enqueue([pixelsCopy = std::move(pixelsCopy), width, height, filename]() mutable {
                saveFrameAsPNG(pixelsCopy.data(), width, height, filename);
            });
        }
    }

    WindowManager::SwapBuffer();
}

bool fIsInit = false;
Scene *createScene()
{
    Ghost::Initialize();
    Grass::Initialize();
    StoneFloor::Initialize();
    Skybox::Initialize();
    WoodenBox::Initialize();
    Tree::Initialize();
    Female::Initialize();
    BrickSphere::Initialize();


    auto *scene = new Scene();

    Actor *character = new Ghost();
    // character->Teleport(glm::vec3(-10, 3, 5.5));
    // RenderManager::pipeline.camera->camera.yaw = 90;
    // RenderManager::pipeline.camera->camera.pitch = 60;
    scene->pushObject(character);


    // tree = new Tree();
    // tree->Teleport(glm::vec3(0, -0.0, 4));
    // tree->MultiplyScale(glm::vec3(.5));
    // scene->pushObject(tree);


    // auto grass = new Grass();
    // grass->Teleport(glm::vec3(0, -.5, 4));
    // grass->rootComponent->SetScale(glm::vec3(1.5));
    // grass->rootComponent->SetRotation(glm::vec3(90, 0, 0));
    // scene->pushObject(grass);

    // auto cubeX = new WoodenBox();
    // cubeX->Teleport(glm::vec3(0, 2.3, 10));
    // scene->pushObject(cubeX);

    // float boxHeight = 2.0f;
    // float startY = -4.0f;

    // for (int i = 0; i < 5; i++) {
    //     auto box = new WoodenBox();
    //     box->Teleport(glm::vec3(0, startY + i * boxHeight, 10));
    //     scene->pushObject(box);
    // }


    auto sphere = new BrickSphere();
    sphere->Teleport(glm::vec3(5, 3, 10));
    sphere->SetRotation(glm::vec3(45, 0, 0));
    // static_cast<RigidTransform*>(sphere->rootComponent->localTransform)->ApplyImpulse(glm::vec3(-500, 0, 0));
    scene->pushObject(sphere);
    BulletManager::StepSimulation(TimeManager::GetDeltaTime());

    std::cout << printVec3(sphere->rootComponent->GetPosition()) << std::endl;

    auto floor = new StoneFloor();
    floor->Teleport(glm::vec3(0, -5, 10));
    scene->pushObject(floor);

    // auto female = new Female();
    // female->rootComponent->SetRotation(glm::vec3(-90, 180 + 30, 0));
    // female->rootComponent->SetPosition(glm::vec3(3, -0.85, 2.6));
    // female->rootComponent->SetScale(glm::vec3(0.01));
    // scene->pushObject(female);

    scene->skybox = new Skybox();

    GlobalState::SetPlayer(character);

    return scene;
}


int main(int argc, char** argv)
{
    const GLfloat width = 1600, height = 900;

    WindowManager::Initialize(width, height);
    RenderManager::Initialize(70.0f, width, height, 0.1f, 1000.0f);
    BulletManager::Initialize();
    TimeManager::Initialize();

    Scene *scene(createScene());
    WindowManager::SwapBuffer();

    RenderManager::pipeline.UpdatePerspective();
    TimeManager::Update();
    while (GlobalState::fIsAppRunning) {
        Callback(scene);
    }

    WindowManager::Dispose();
    BulletManager::Dispose();

    return 0;
}
