#include <entities/templates/playable/Ghost.hpp>
#include <entities/templates/mobs/Female.hpp>
#include <game/gameManager.hpp>
#include <object/sphere.hpp>
#include <object/cube.hpp>

#include <threads/threads.hpp>
#include <game/gameManager.hpp> 
#include <mesh/mesh.hpp> 
#include <mesh/custom_mesh.hpp> 

#include <entities/templates/mobs/Female.hpp>
#include <stb_image.h>
#include <stb_image_write.h>

std::chrono::milliseconds totalTime(0);

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

    for (auto& it : scene->primitives) {
        it->Render();
            // it->GetMesh()->set_transform(*it->GetTransform());
            // std::vector<aiMatrix4x4> *Transforms = new std::vector<aiMatrix4x4>;
            // it->GetMesh()->BoneTransform(GameManager::Time.GetCurrentTime(), *Transforms);
            // GameManager::render.clearRender();
            // Actor::Actor_rdata data = {Transforms, it->GetMesh()};
            // it->Render(&data);
            // GameManager::render.clearRender();
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

Scene *createScene()
{
    Cube::initializeGeometry();
    Sprite::initializeGeometry();
    Female::Initialize();

    auto *scene = new Scene();
    // Cube* cube = new Cube("img/skybox.png");
    Ghost *character = new Ghost();
    // Sprite* sprite = new Sprite("floor", "shaders/sprite_fs.glsl","shaders/sprite_vs.glsl", "img/grass.png");

    const std::size_t SIZE = 3072;
    vec3i size(SIZE, SIZE, SIZE);
    std::vector<bool> vec(SIZE * SIZE * SIZE);
    //int* vec = new int[SIZE * SIZE * SIZE];


    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            for (int z = 0; z < size.z; ++z) {
                vec[z + y * size.z + x * size.z * size.y] = x / 2 == 0;
            }
        }
    }

    CustomMesh *cmesh = new CustomMesh(size, vec);
    Primitive_mesh *obj = new Primitive_mesh(cmesh);

    scene->pushObject(character);
    scene->pushObject(obj);

    objectTransform transform;
    float scale = 4 / SIZE;
    transform.SetWorldPos(0.0, -5.0, -10.0);
    transform.SetRotate(0.0, 0.0, 0.0);
    transform.SetScale(glm::vec3(scale, scale, scale));
    *obj->GetTransform() = transform;

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
