#include <entities/templates/playable/Ghost.hpp>
#include <entities/templates/mobs/Female.hpp>
#include <game/gameManager.hpp>
#include <object/sphere.hpp>
#include <object/cube.hpp>
#include <object/cube_simple.hpp>

#include <threads/threads.hpp>
#include <game/gameManager.hpp> 
#include <mesh/mesh.hpp> 
#include <mesh/custom_mesh.hpp> 

#include <entities/templates/mobs/Female.hpp>
#include <stb_image.h>
#include <stb_image_write.h>

#include <data/bit_array.hpp>
#include <data/bit_big_array.hpp>

std::chrono::milliseconds totalTime(0);



const std::size_t PART = 4;
const std::size_t SIZE = 128;
vec3i size(SIZE, SIZE, SIZE);
BitBigArray vec(SIZE * SIZE * SIZE, PART);


void swapBuffer() {
    glfwSwapBuffers(GameManager::window);
    glfwPollEvents();
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
    GameManager::UpdateCamera();

    // static CubeSimple cube;
    // GameManager::render.drawSkybox(*scene->skybox);

    for (auto& it : scene->actors) {
        (reinterpret_cast<Pawn*>(it))->MoveForward();
        if (it->GetMesh() != nullptr) {
            it->GetMesh()->set_transform(*it->GetTransform());
            std::vector<aiMatrix4x4> *Transforms = new std::vector<aiMatrix4x4>;
            it->GetMesh()->BoneTransform(GameManager::Time.GetCurrentTime(), *Transforms);
            GameManager::render.clearRender();
            Actor::Actor_rdata data = {Transforms, it->GetMesh()};
            it->Render(&data);
            GameManager::render.clearRender();
        }
    }

    // CubeSimple::CubeSimple_rdata data = {
    //     SIZE,
    //     GameManager::render.pipeline.GetTransform(objectTransform()),
    //     &vec
    // };

    // cube.Render(&data);

    for (auto& it : scene->primitives) {
        it->Render();
    }

//     static time_t prev_rotate = time(0);
//     if ((time(0) - prev_rotate) > 0.75) {
//         scene->actors[1]->GetTransform()->AddRotate(glm::vec3(0, 20, 0));

//     std::vector<float> depthBuffer(GameManager::width * GameManager::height);
//     std::vector<unsigned char> pixels(GameManager::width * GameManager::height * 4); // RGBA формат
// glReadPixels(0, 0, GameManager::width, GameManager::height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
// stbi_write_png("screenshot_with_alpha.png", GameManager::width, GameManager::height, 4, pixels.data(), GameManager::width * 4);

//     glReadPixels(0, 0, GameManager::width, GameManager::height, GL_DEPTH_COMPONENT, GL_FLOAT, depthBuffer.data());
// std::ofstream out("data.txt", std::ios::app);
//     for (int i = 0; i < GameManager::width; i++) {
//         for (int j = 0; j < GameManager::height; j++) {
//             if (depthBuffer[j * GameManager::width + i] < 1.f) {
//                 out << "1 "; // Пустой пиксель
//             } else {
//                 out << "0 "; // Пиксель с объектом
//             }
//             // out  << depthBuffer[j * GameManager::width + i] << " "; // Значение глубины для каждого пикселя
//         }
//         out << std::endl; // Перенос строки после каждого ряда
//     }
// out.close();   
//         prev_rotate = time(0);
//     }

    // for (auto& it : scene->sprites) {
    //     it->Render(&GameManager::render.pipeline.GetTransform(it. ->GetTransform()));
    // }


    // std::vector<unsigned char> pixels(GameManager::width * GameManager::height * 3);

    // glReadPixels(0, 0, GameManager::width, GameManager::height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    // stbi_write_png("screenshot.png", GameManager::width, GameManager::height, 3, pixels.data(), GameManager::width * 3);

// Перевернем изображение по вертикали
// for (int y = 0; y < GameManager::height / 2; ++y) {
//     for (int x = 0; x < GameManager::width * 3; ++x) {
//         std::swap(pixels[y * GameManager::width * 3 + x], pixels[(GameManager::width - 1 - y) * GameManager::width * 3 + x]);
//     }
// }


     

    swapBuffer();

    frame++;
    if ((time(0) - prev) > 3) {
        std::cout << "FPS: " << frame / 3 << std::endl;
        prev = time(0);
        frame = 0;
    }
}

void generateMobiusStrip(int size, std::vector<bool> &data, float width, float radius) {
    int centerX = size / 2;
    int centerY = size / 2;
    int centerZ = size / 2;

    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            for (int z = 0; z < size; ++z) {
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
                if (sqrt(dx * dx + dy * dy + dz * dz) < width) {
                    data[x + y * size + z * size * size] = 1;
                } else {
                    data[x + y * size + z * size * size] = 0;
                }
            }
        }
    }
}

void generateTorus(int size, std::vector<bool> &data, float R, float r) {
    int centerX = size / 2;
    int centerY = size / 2;
    int centerZ = size / 2;

    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            for (int z = 0; z < size; ++z) {
                // Преобразование координат в систему с центром в центре куба
                float dx = x - centerX;
                float dy = y - centerY;
                float dz = z - centerZ;

                // Вычисление расстояний
                float distanceFromAxis = sqrt(dx * dx + dy * dy);
                float torusEquation = pow((distanceFromAxis - R), 2) + dz * dz - r * r;

                // Если точка удовлетворяет уравнению тора, заполняем воксель
                if (torusEquation <= 0.0f) {
                    data[x + y * size + z * size * size] = 1;
                } else {
                    data[x + y * size + z * size * size] = 0;
                }
            }
        }
    }
}

void generateSphere(int size, BitBigArray &data, float radius) {
    int centerX = size / 2;
    int centerY = size / 2;
    int centerZ = size / 2;

    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            for (int z = 0; z < size; ++z) {
                float dist = sqrt(pow(x - centerX, 2) + pow(y - centerY, 2) + pow(z - centerZ, 2));
                if (dist <= radius) {
                    data.setBit(x + y * size + z * size * size, 1);
                } else {
                    data.setBit(x + y * size + z * size * size, 0);
                }
            }
        }
    }
}

Scene *createScene()
{
    // Cube::initializeGeometry();
    // CubeSimple::initializeGeometry();
    // Sprite::initializeGeometry();
    // Female::Initialize();

    auto *scene = new Scene();
    // Cube* cube = new Cube("img/skybox.png");
    Ghost *character = new Ghost();
    // Sprite* sprite = new Sprite("floor", "shaders/sprite_fs.glsl","shaders/sprite_vs.glsl", "img/grass.png");

    // ull_I s = 3072ull * 3072ull * 3072ull;
    // BitBigArray array(s, 24);
    // bool* vecb = new bool[3072 * 3072 * (3072 / 24)];
    // bool *bigVec[24];
    // for (int i = 0; i < 24; ++i) {
    //     bigVec[i] = new bool[3072 * 3072 * (3072 / 24)];
    // }

    generateSphere(SIZE, vec, SIZE / 4);
    // generateTorus(SIZE, vec, 300.0f, 100.0f);
    // generateMobiusStrip(SIZE, vec, 50.0f, 300.0f);
    // for (int x = 0; x < size.x; ++x) {
    //     for (int y = 0; y < size.y; ++y) {
    //         for (int z = 0; z < size.z; ++z) {
    //             vec[z + y * size.z + x * size.z * size.y] = (z / 2) == 0;
    //         }
    //     }
    // }

    scene->pushObject(character);
    for (int i = 0; i < PART; ++i) {
        CustomMesh *cmesh = new CustomMesh(size, vec, i);
        Primitive_mesh *obj = new Primitive_mesh(cmesh);
        scene->pushObject(obj);

        objectTransform transform;
        float scale = 8.0 / SIZE;
        transform.SetWorldPos(0.0, -5.0, -0.0);
        transform.SetRotate(0.0, 0.0, 0.0);
        transform.SetScale(glm::vec3(scale, scale, scale));
        *obj->GetTransform() = transform;
    }

    GameManager::PushPlayer(character);
    GameManager::render.pipeline.camera.OwnerTransformPtr = character->GetTransform();

    return scene;
}


int main(int argc, char** argv)
{
    const int width = 1900, height = 1280;
    GameManager::InitializeGLFW(width, height);
    GameManager::InitializeObjects();

    std::unique_ptr<Scene> scene(createScene());
    // GameManager::threads->setScene(scene.get());
    // GameManager::threads->start();

    while (!GameManager::IsEnd) {
        callback(scene.get());
    }

    glfwDestroyWindow(GameManager::window);
    glfwTerminate();

    return 0;
}
