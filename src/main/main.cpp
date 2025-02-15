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

void MoveInCircle(Ghost *character, float radius, int step, int max_steps) {
    if (!character) return;
    std::cout << "step: " << step << std::endl;

    // Вычисляем угол поворота на каждом шаге
    float angleStep = glm::two_pi<float>() / max_steps;

    // Вычисляем текущий угол
    float angle = angleStep * step;

    // Новая позиция на окружности
    float x = radius * cos(angle);
    float z = radius * sin(angle);
    float y = character->GetTransform()->GetWorldPos().y;

    character->GetTransform()->SetWorldPos(glm::vec3(x, y, z));

    // Направление на центр
    glm::vec3 direction = glm::normalize(character->GetTransform()->GetWorldPos());

    // Вычисляем угол поворота вокруг Y-оси
    float rotationY = atan2(direction.x, direction.z);
    rotationY = glm::degrees(rotationY);
    rotationY -= 90;
    character->SetYaw(-(rotationY));
}


void callback(Scene *scene) {
    static int frame = 0;
    int MAX_FRAME = 40;

    if (frame >= MAX_FRAME) {
        GameManager::IsEnd = true;
        return;
    }

    // Rendering
    GameManager::Time.Update();
    auto start_render = GameManager::Time.GetCurrentTime();
    MoveInCircle(character, 1000, frame, MAX_FRAME);
    GameManager::UpdateCamera();
    GameManager::render.GetPV();
    config->RenderScene();


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

    #pragma omp parallel
    {
        #pragma omp single
        {
            const std::string dir_name("data");

            // Task: Запись depth map в текстовый файл (буферизированная запись)
            #pragma omp task
            {
                if (frame < MAX_FRAME) {
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
            #pragma omp task
            {
                if (frame < MAX_FRAME) {
                    const std::string file_name = dir_name + "/screenshot_" + std::to_string(frame) + ".png";

                    // Оптимизированная запись через память
                    std::vector<unsigned char> compressedData;
                    compressedData.reserve(GameManager::width * GameManager::height * 3);

                    auto pngWriteCallback = [](void *context, void *data, int size) {
                        auto *vec = static_cast<std::vector<unsigned char>*>(context);
                        vec->insert(vec->end(), static_cast<unsigned char*>(data), static_cast<unsigned char*>(data) + size);
                    };

                    // stbi_write_png_to_func(pngWriteCallback, &compressedData, 
                    //                        GameManager::width, GameManager::height, 3, 
                    //                        pixels.data(), GameManager::width * 3);

                    // Записываем PNG в файл одним вызовом
                    std::ofstream out(file_name, std::ios::binary);
                    out.write(reinterpret_cast<char*>(compressedData.data()), compressedData.size());
                }
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
    character->GetTransform()->SetWorldPos(glm::vec3(400, 100, 0));
    scene->pushObject(character);

    config = new Config();
    config->CreateScene();

    GameManager::PushPlayer(character);
    GameManager::render.pipeline.camera.OwnerTransformPtr = character->GetTransform();

    swapBuffer();

    return scene;
}


int main(int argc, char** argv)
{
    int size = 3000;
    const int width = size, height = size;
    GameManager::InitializeGLFW(width, height);

    Scene *scene(createScene());

    while (!GameManager::IsEnd) {
        callback(scene);
    }

    std::cout << "Rendering time:\t" << time_render << std::endl;
    std::cout << "Saving time:\t" << time_save << std::endl;
    std::cout << "Total time:\t" << GameManager::Time.GetCurrentTime() << std::endl;

    glfwDestroyWindow(GameManager::window);
    glfwTerminate();

    return 0;
}
