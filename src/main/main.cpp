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
// #include <object/component/template/component_physics.hpp>

#include <stb_image_write.h>

#include <iomanip> 
void printMat4(const glm::mat4& mat) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            std::cout << std::setw(10) << mat[col][row] << " ";
        }
        std::cout << std::endl;
    }
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


WoodenBox* cube;
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

glm::vec3 SpinTopRotation(float time, float spinSpeed, float tiltSpeed, float tiltAmount) {
    // Время — накопленное, т.е. time += deltaTime каждый кадр

    // Основное вращение вокруг вертикальной оси (обычно Y)
    float yaw = time * spinSpeed; // вращение вокруг Y

    // Наклон, который описывает круг на X и Z
    float pitch = sin(time * tiltSpeed) * tiltAmount; // наклон вперёд-назад (X)
    float roll  = cos(time * tiltSpeed) * tiltAmount; // наклон влево-вправо (Z)

    return glm::vec3(pitch, yaw, roll);
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

    // cube->rootComponent->SetRotation(SpinTopRotation(TimeManager::GetCurrentTime(), 180.0f, 3.0f, 45.0f));

    for (auto &it : scene->actors)
        it->Render();


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
                saveFrameAsJPEG(pixelsCopy.data(), width, height, filename);
            });
        }
    }

    WindowManager::SwapBuffer();
}

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
    // character->Teleport(glm::vec3(9, 6, 0));
    scene->pushObject(character);

    // auto grass = new Grass();
    // grass->Teleport(glm::vec3(-1, -1, 5));
    // grass->rootComponent->SetScale(glm::vec3(3));
    // grass->rootComponent->SetRotation(glm::vec3(90, 0, 0));
    // scene->pushObject(grass);

    cube = new WoodenBox();
    // cube->Teleport(glm::vec3(0, -0.5, 4 + 2));
    cube->Teleport(glm::vec3(0, -1, 4));
    // cube->rootComponent->SetRotation(glm::vec3(20, 30, 75));
    scene->pushObject(cube);

//     auto sphere = new BrickSphere();
//     sphere->Teleport(glm::vec3(0, 1, -6));
//     auto body = static_cast<RigidTransform*>(sphere->rootComponent->localTransform);
//     glm::vec3 impulseDir = glm::normalize(glm::vec3(0.0f, -0.2f, 1.0f));
//     body->ApplyImpulse(impulseDir * 100.0f);
//     scene->pushObject(sphere);

// const int stackHeight = 5;
// for (int i = 0; i < stackHeight; ++i) {
//     auto box = new WoodenBox();
//     float y = 0.01f + i * 2.01f;
//     box->Teleport(glm::vec3(0.0f, y, 12.0f));
//     scene->pushObject(box);
// }



    // auto floor = new StoneFloor();
    // floor->Teleport(glm::vec3(0, 0, 10));
    // // floor->rootComponent->SetScale(glm::vec3(0.5));
    // scene->pushObject(floor);

    // auto female = new Female();
    // female->rootComponent->SetRotation(glm::vec3(-90, 180 + 30, 0));
    // female->rootComponent->SetPosition(glm::vec3(0, -1 + 0.1, 4.2 - 2.6));
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
    // GlobalState::FPS = 65;

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
