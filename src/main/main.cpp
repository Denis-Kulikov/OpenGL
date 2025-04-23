#include <entities/templates/playable/Ghost.hpp>
#include <entities/templates/mobs/Female.hpp>
#include <game/gameManager.hpp>
#include <object/sphere.hpp>
#include <object/sphere_wire.hpp>
#include <object/cube.hpp>

#include <threads/threads.hpp>
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

    std::cout << "x = " << x << " y = " << y << " z = " << z << std::endl;
    std::cout << "yaw = " << yaw << " pitch = " << pitch << std::endl;

    character->SetYaw(yaw);
    character->SetPitch(pitch);
}

void drow(Scene *scene, int i, int j) {
    static int frame = 0;

    std::cout << "Frame: " << frame << std::endl;
    // Rendering
    GameManager::Time.Update();
    auto start_render = GameManager::Time.GetCurrentTime();
    MoveInGridArc(character, config->GetWidth() + 1, i, j, config->GetFrames());
    GameManager::UpdateCamera();
    // GameManager::render.GetPV_Perspective();
    GameManager::render.GetPV();
    config->RenderScene();

    scene->shapes[0]->Render();


    // Saving
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

    swapBuffer();

    //#pragma omp parallel
    {
        //#pragma omp single
        {
            const std::string dir_name("data");

            // Task: Запись depth map в текстовый файл (буферизированная запись)
            //#pragma omp task
            {
                // if (frame < MAX_FRAME) {
                if (false) {
                    const std::string file_name = dir_name + "/data_" + std::to_string(frame) + ".txt";
                    
                    // Используем std::string для буферизации
                    std::string buffer;
                    buffer.reserve(GameManager::width * (GameManager::height + 1)); // Оптимизация выделения памяти

                    for (int j = 0; j < GameManager::height; j++) {
                        for (int i = 0; i < GameManager::width; i++) {
                            buffer += (depthBuffer[j * GameManager::width + i] < 1.f ? ' ' : '#');
                        }
                        buffer += '\n';
                    }

                    // Запись в файл одним вызовом
                    std::ofstream out(file_name, std::ios::binary);
                    out.write(buffer.data(), buffer.size());
                }
            }

            // Task: Сохранение скриншота в PNG с буферизированной записью
            //#pragma omp task
            {
                const std::string file_name = dir_name + "/screenshot_" + std::to_string(i) + "_" + std::to_string(j) + ".png";

                // Оптимизированная запись через память
                std::vector<unsigned char> compressedData;
                compressedData.reserve(GameManager::width * GameManager::height * 3);

                auto pngWriteCallback = [](void *context, void *data, int size) {
                    auto *vec = static_cast<std::vector<unsigned char>*>(context);
                    vec->insert(vec->end(), static_cast<unsigned char*>(data), static_cast<unsigned char*>(data) + size);
                };

                stbi_write_png_to_func(pngWriteCallback, &compressedData, 
                                        GameManager::width, GameManager::height, 3, 
                                        pixels.data(), GameManager::width * 3);

                // Записываем PNG в файл одним вызовом
                std::ofstream out(file_name, std::ios::binary);
                out.write(reinterpret_cast<char*>(compressedData.data()), compressedData.size());
            }
        }
    }

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

    auto cube = new Shape(new Cube());
    cube->GetTransform()->SetWorldPos(glm::vec3(0));
    cube->GetTransform()->SetScale(glm::vec3(500));
    scene->pushObject(cube);

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
    // int size = 640;
    const int width = size, height = size;
    GameManager::InitializeGLFW(width, height);

    Scene *scene(createScene());

    // for (int i = 0; i < config->GetFrames(); ++i) {
        for (int j = 0; j < config->GetFrames() / 2 + 1; ++j) {
            drow(scene, 0, j);
            // drow(scene, i, j);
        }
    // }

    std::cout << "Rendering time:\t" << time_render << std::endl;
    std::cout << "Saving time:\t" << time_save << std::endl;
    std::cout << "Total time:\t" << GameManager::Time.GetCurrentTime() << std::endl;

    glfwDestroyWindow(GameManager::window);
    glfwTerminate();

    return 0;
}
