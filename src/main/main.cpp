#include <entities/templates/playable/Ghost.hpp>
#include <entities/templates/mobs/Female.hpp>
#include <game/gameManager.hpp>
#include <object/scene.hpp>
#include <object/sphere.hpp>
#include <object/sphere_wire.hpp>
#include <object/cube.hpp>

#include <threads/thread_pool.hpp>
#include <game/gameManager.hpp> 

#include <entities/templates/decor/config.hpp>

#include <entities/templates/mobs/Female.hpp>
#include <stb_image.h>
#include <stb_image_write.h>

#include <omp.h>


std::chrono::milliseconds totalTime(0);
Config* config = nullptr;
Ghost *character = nullptr;

float time_render = 0;
float time_save = 0;

void swapBuffer() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void saveDepthMap(const std::vector<float>& depthBuffer, const std::string& filename) {
    std::string buffer;
    buffer.reserve(GameManager::width * (GameManager::height + 1));
    for (int j = 0; j < GameManager::height; ++j) {
        for (int i = 0; i < GameManager::width; ++i) {
            buffer += (depthBuffer[j * GameManager::width + i] < 1.f ? ' ' : '#');
        }
        buffer += '\n';
    }
    std::ofstream out(filename, std::ios::binary);
    out.write(buffer.data(), buffer.size());
}

void saveScreenshot(const std::vector<unsigned char>& pixels, const std::string& filename) {
    std::vector<unsigned char> compressed;
    auto callback = [](void* context, void* data, int size) {
        auto* vec = static_cast<std::vector<unsigned char>*>(context);
        vec->insert(vec->end(), (unsigned char*)data, (unsigned char*)data + size);
    };

    stbi_write_png_to_func(callback, &compressed,
        GameManager::width, GameManager::height, 3,
        pixels.data(), GameManager::width * 3);

    std::ofstream out(filename, std::ios::binary);
    out.write(reinterpret_cast<char*>(compressed.data()), compressed.size());
}

void MoveInGridArc(Ghost* character, float radius, int i, int j, int max_steps) {
    if (!character) return;

    int horizontalSteps = max_steps;
    int verticalSteps = max_steps / 2 + 1;

    // Углы по горизонтали (0 .. 360°)
    double angleH = glm::two_pi<double>() * (i % horizontalSteps) / horizontalSteps;

    double t = double(j) / (verticalSteps - 1);
    double angleV = glm::radians(90.0f) * (1.0f - 2.0f * t); // от +90° до -90°

    float x = radius * cos(angleV) * cos(angleH);
    float y = radius * sin(angleV);
    float z = radius * cos(angleV) * sin(angleH);

    glm::vec3 pos = glm::vec3(x, y, z);
    character->GetTransform()->SetWorldPos(pos);

    // Поворот к центру
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 direction = glm::normalize(center - pos);

    float yaw   = glm::degrees(atan2(direction.x, direction.z));
    float pitch = glm::degrees(asin(direction.y));  // Вычисляем угол наклона вверх/вниз

    // std::cout << "yaw: " << yaw << std::endl;
    // std::cout << "pitch: " << pitch << std::endl;

    if (std::fabs(pitch) == 90)
        yaw -= 180;

    character->SetYaw(yaw);
    character->SetPitch(pitch);
}

void drow(Scene *scene, ThreadPool& pool, int i, int j) {
    static int frame = 1;

    if (frame % 10 == 0)
        std::cout << "Frame: " << frame << std::endl;

    GameManager::Time.Update();
    auto start_render = GameManager::Time.GetCurrentTime();
    MoveInGridArc(character, config->GetWidth() + 1, i, j, config->GetFrames());
    GameManager::UpdateCamera();
    GameManager::render.GetPV();
    config->RenderScene();

    std::vector<float> depthBuffer(GameManager::width * GameManager::height);
    std::vector<unsigned char> pixels(GameManager::width * GameManager::height * 3);

    glFinish(); // Ожидание конца рендеринга
    GameManager::Time.Update();
    auto end_render = GameManager::Time.GetCurrentTime();
    time_render += end_render - start_render;

    GameManager::Time.Update();
    auto start_save = GameManager::Time.GetCurrentTime();

    glReadPixels(0, 0, GameManager::width, GameManager::height, GL_DEPTH_COMPONENT, GL_FLOAT, depthBuffer.data());
    glReadPixels(0, 0, GameManager::width, GameManager::height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    while (!pool.available()) {
        std::chrono::milliseconds timespan(100);
        std::this_thread::sleep_for(timespan);
    }

    // pool.enqueue([=]() {
    //     saveDepthMap(depthBuffer, "data/screenshot_" + std::to_string(i) + "_" + std::to_string(j) + ".txt");
    // });

    pool.enqueue([=]() {
        saveScreenshot(pixels, "data/screenshot_" + std::to_string(i) + "_" + std::to_string(j) + ".png");
    });

    swapBuffer();

    GameManager::Time.Update();
    auto end_save = GameManager::Time.GetCurrentTime();
    time_save += end_save - start_save;

    ++frame;
}

Scene *createScene()
{
    Cube::initializeGeometry();
    Sprite::initializeGeometry();
    Config::Initialize();

    auto *scene = new Scene();

    character = new Ghost();
    scene->pushObject(character);

    GameManager::PushPlayer(character);
    GameManager::render.pipeline.camera.OwnerTransformPtr = character->GetTransform();

    config = new Config();
    config->CreateScene();

    swapBuffer();

    return scene;
}


int main(int argc, char** argv)
{
    int size = 3000;
    const int width = size, height = size;
    GameManager::InitializeGLFW(width, height);

    Scene *scene(createScene());
    ThreadPool pool(15, 100);

    // for (int i = 0; i < config->GetFrames(); ++i) {
        for (int j = 0; j < config->GetFrames() / 2 + 1; ++j) {
            drow(scene, pool, 0, j);
        }

        for (int j = 0; j < config->GetFrames() / 2 + 1; ++j) {
            drow(scene, pool, 15, j);
        }
    // }

    // std::cout << "Rendering time:\t" << time_render << std::endl;
    // std::cout << "Saving time:\t" << time_save << std::endl;
    std::cout << "Total time:\t" << GameManager::Time.GetCurrentTime() << std::endl;

    pool.shutdown();

    glfwDestroyWindow(GameManager::window);
    glfwTerminate();

    return 0;
}
