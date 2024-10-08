#include <entities/templates/playable/Ghost.hpp>
#include <entities/templates/mobs/Female.hpp>
#include <game/gameManager.hpp>
// #include <object/sphere.hpp>
// #include <object/cube.hpp>
// #include <object/cube_simple.hpp>

#include <threads/threads.hpp>
#include <game/gameManager.hpp> 
// #include <mesh/mesh.hpp> 
#include <mesh/custom_mesh.hpp> 

#include <entities/templates/mobs/Female.hpp>
#include <stb_image.h>
#include <stb_image_write.h>

#include <data/bit_array.hpp>
#include <data/bit_big_array.hpp>

#include <omp.h>


std::chrono::milliseconds totalTime(0);

const float SCALE = 8;
const std::size_t PART = 16;
const ull_I SIZE = 256;
vec3i size(SIZE, SIZE, SIZE);
BitBigArray vec(SIZE * SIZE * SIZE, PART);


void swapBuffer() {
    // glfwSwapBuffers(GameManager::window);
    // glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void callback(Scene *scene) {
    static time_t prev = time(0);
    static int frame = 0;
    static const int targetFPS = 144;
    static const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    static std::chrono::steady_clock::time_point frameStart;
    std::chrono::steady_clock::time_point frameEnd = std::chrono::steady_clock::now();
    std::this_thread::sleep_until(frameStart + frameDuration);
    frameStart = std::chrono::steady_clock::now();

    GameManager::Time.Update();
    GameManager::render.GetPV();
    // GameManager::UpdateCamera();

    GameManager::Time.Update();
    std::cout << "Start (" << frame << "): " << GameManager::Time.GetCurrentTime() << std::endl;

    for (auto& it : scene->actors) {
        (reinterpret_cast<Pawn*>(it))->MoveForward();
    }

    objectTransform transform;
    float scale = SCALE / SIZE;
    transform.SetWorldPos(0, 0, SCALE);
    transform.SetRotate(0.0, 20.0 * frame, 0.0);
    transform.SetScale(glm::vec3(scale, scale, scale));
        
    for (int i = 0; i < PART; ++i) {
        CustomMesh cmesh(size, vec, i);
        Primitive_mesh obj(&cmesh);
        *obj.GetTransform() = transform;
        obj.Render();
    }

    GameManager::Time.Update();
    std::cout << "Write (" << frame << "): " << GameManager::Time.GetCurrentTime() << std::endl;


    std::vector<float> depthBuffer(GameManager::width * GameManager::height);
    std::vector<unsigned char> pixels(GameManager::width * GameManager::height * 3);

    glReadPixels(0, 0, GameManager::width, GameManager::height, GL_DEPTH_COMPONENT, GL_FLOAT, depthBuffer.data());
    glReadPixels(0, 0, GameManager::width, GameManager::height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    #pragma omp parallel
    {
        #pragma omp single
        {
            std::string dir_name("data");
            // Task 1: swapBuffer(), должен выполняться родительским потоком
            #pragma omp task
            {
                if (omp_get_thread_num() == 0) {
                    swapBuffer();
                }
            }

            // Task 2: запись данных в файл
            #pragma omp task
            {
                if (frame < 1000) {
                    std::string file_name(dir_name + "/data_" + std::to_string(frame) + ".txt");
                    std::ofstream out(file_name, std::ios::app);
                    for (int i = 0; i < GameManager::width; i++) {
                        for (int j = 0; j < GameManager::height; j++) {
                            if (depthBuffer[j * GameManager::width + i] < 1.f) {
                                out << " "; // Пустой пиксель
                            } else {
                                out << "#"; // Пиксель с объектом
                            }
                        }
                        out << std::endl;
                    }
                    out.close();
                }
            }

            // Task 3: сохранение скриншота
            #pragma omp task
            {
                if (frame < 1000) {
                    std::string file_name(dir_name + "/screenshot_" + std::to_string(frame) + ".png");
                    stbi_write_png(file_name.c_str(), GameManager::width, GameManager::height, 3, pixels.data(), GameManager::width * 3);
                }
            }
        }
    }

    GameManager::Time.Update();
    std::cout << "End (" << frame << "): " << GameManager::Time.GetCurrentTime() << std::endl;

    ++frame;
}

void generateMobiusStrip(long long int size, BitBigArray &data, float width, float radius) {
    long long int centerX = size / 2;
    long long int centerY = size / 2;
    long long int centerZ = size / 2;

    for (long long int x = 0; x < size; ++x) {
        for (long long int y = 0; y < size; ++y) {
            for (long long int z = 0; z < size; ++z) {
                float u = ((float)x / size) * 2.0f * M_PI;  // Параметр u
                float v = ((float)y / size) * 2.0f - 1.0f;  // Параметр v (от -1 до 1)

                // Параметрическое уравнение Мёбиусова кольца
                float X = (radius + v * cos(u / 2)) * cos(u);
                float Y = (radius + v * cos(u / 2)) * sin(u);
                float Z = v * sin(u / 2);

                // Преобразование координат в систему с центром в центре куба
                float dx = X + centerX - x;
                float dy = Y + centerY - y;
                float dz = Z + centerZ - z;

                // Если расстояние до точки меньше допустимого, заполняем воксель
                if (sqrt(dx * dx + dy * dy + dz * dz) < width)
                    data.set(x + y * size + z * size * size, 1);
            }
        }
    }
}

void generateTorus(long long int size, BitBigArray &data) {
    long long int centerX = size / 2;
    long long int centerY = size / 2;
    long long int centerZ = size / 2;

    float R = 0.6f * size;
    float r = 0.2f * size;

    for (long long int z = 0; z < size; ++z) {
        for (long long int y = 0; y < size; ++y) {
            for (long long int x = 0; x < size; ++x) {
                float dx = x - centerX;
                float dy = y - centerY;
                float dz = z - centerZ;

                float distanceFromAxis = sqrt(dx * dx + dy * dy);
                float torusEquation = pow((distanceFromAxis - R), 2) + dz * dz - r * r;

                if (torusEquation <= 0.0f)
                    data.set(x + y * size + z * size * size, 1);
            }
        }
    }
}

void generateSphere(long long int size, BitBigArray &data, float radius) {
    long long int centerX = size / 2;
    long long int centerY = size / 2;
    long long int centerZ = size / 2;

    #pragma omp parallel for collapse(2)
    for (long long int z = 0; z < size; ++z) {
        for (long long int y = 0; y < size; ++y) {
            for (long long int x = 0; x < size; ++x) {
                float dist = sqrt(pow(x - centerX, 2) + pow(y - centerY, 2) + pow(z - centerZ, 2));
                if (dist <= radius)
                    data.set(x + y * size + z * size * size, 1);
            }
        }
    }
}

Scene *createScene()
{
    auto *scene = new Scene();
    Ghost *character = new Ghost();
    scene->pushObject(character);
    GameManager::PushPlayer(character);
    GameManager::render.pipeline.camera.OwnerTransformPtr = character->GetTransform();

    swapBuffer();

    std::cout << "Start (" << "generate" << "): " << GameManager::Time.GetCurrentTime() << std::endl;
    // generateSphere(SIZE, vec, SIZE / 2);
    generateTorus(SIZE, vec);
    std::cout << "End (" << "generateSphere" << "): " << GameManager::Time.GetCurrentTime() << std::endl;

    return scene;
}


int main(int argc, char** argv)
{
    const int width = SIZE, height = SIZE;
    GameManager::InitializeGLFW(width, height);
    GameManager::InitializeObjects();

    std::unique_ptr<Scene> scene(createScene());

    while (!GameManager::IsEnd) {
        callback(scene.get());
    }

    glfwDestroyWindow(GameManager::window);
    glfwTerminate();

    return 0;
}
