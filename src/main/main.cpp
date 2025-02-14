#include <entities/templates/playable/Ghost.hpp>
#include <entities/templates/mobs/Female.hpp>
#include <game/gameManager.hpp>
#include <object/sphere.hpp>
#include <object/sphere_wire.hpp>
#include <object/cube.hpp>

#include <threads/threads.hpp>
#include <game/gameManager.hpp> 
// #include <mesh/mesh.hpp> 

#include <entities/templates/decor/wooden_box.hpp>
#include <entities/templates/decor/floor.hpp>
#include <entities/templates/decor/config.hpp>

#include <entities/templates/mobs/Female.hpp>
#include <stb_image.h>
#include <stb_image_write.h>

#include <omp.h>


std::chrono::milliseconds totalTime(0);
Config* config = nullptr;

#include <glm/gtx/euler_angles.hpp>
glm::vec3 QuatToEuler(glm::quat q) {
    float yaw, pitch, roll;
    glm::extractEulerAngleYXZ(glm::toMat4(q), yaw, pitch, roll);
    return glm::degrees(glm::vec3(pitch, yaw, roll));
}

// void swapBuffer() {
//     // glfwSwapBuffers(GameManager::window);
//     // glfwPollEvents();
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

//     // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// }

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

    static bool flag1 = true;
    static bool flag2 = true;
    for (auto& it : scene->pawns) {
        it->MoveForward();
        // if (it->rootComponent != nullptr && (it->GetName() == "WoodenBox")) {
        //     glm::quat rotationZ = glm::angleAxis(glm::radians(GameManager::Time.GetDeltaTime() * 20), glm::vec3(0, 1, 0));
        //     // it->rootComponent->Move(glm::vec3(0, 0, -GameManager::Time.GetCurrentTime() / 100));

        //     float speed = 2.0f; // Скорость вращения
        //     float radius = 3.0f; // Радиус движения
        //     float angle = GameManager::Time.GetCurrentTime() * speed;

        //     glm::vec3 circularPosition = glm::vec3(sinf(angle) * radius, 0, cosf(angle) * radius);
        //     it->rootComponent->Move(circularPosition, GameManager::Time.GetDeltaTime() * 2.01f);
        //     it->rootComponent->Rotate(rotationZ);

        //     Transform transformSpawn;
        //     glm::vec3 pos(0, 10, -15);
        //     // glm::quat rotation = glm::angleAxis(glm::radians(0.1f), glm::vec3(0, 1, 0));
        //     transformSpawn.SetPosition(pos);
        //     // transformSpawn.SetRotation(rotation);
        //     transformSpawn.UpdateMatrix();
        //     // it->Spawn();
        //     // it->rootComponent->Spawn(transformSpawn.GetMatrix());
        //     // it->rootComponent->globalTransform->SetMatrix(transformSpawn.GetMatrix());
        //     // auto position = it->rootComponent->globalTransform->GetPosition();
        //     // std::cout << position.x << " " << position.y << " " << position.z << std::endl;
        // }
        // if (flag1 && (it->rootComponent != nullptr) && (it->GetName() == "WoodenBox")) {
        //     flag1 = false;
        //     glm::vec3 pos(0, 0, -15);
        //     Transform transformSpawn;
        //     transformSpawn.SetPosition(pos);
        //     transformSpawn.UpdateMatrix();
        //     it->rootComponent->SetTransform(&transformSpawn);
        // }
        // if (flag1 && (it->rootComponent != nullptr) && (it->GetName() == "Floor")) {
            // it->rootComponent->SetPosition(glm::vec3());
        // }
        // if (flag2 && (it->rootComponent != nullptr) && (it->GetName() == "Female")) {
        //     flag2 = false;
        //     std::cout << "+++" << std::endl;
        //     it->rootComponent->SetPosition(glm::vec3(0, 0, 0));
        // }
        // if (it->GetName() == "Female") {
        //     static int count = 0;
        //     if (count % 10 == 0)
        //         it->rootComponent->Move(glm::vec3(0.1, 0.0, 0.0));
        //     ++count;
        // }

        // if (it->rootComponent != nullptr) {
        //     auto position = it->rootComponent->globalTransform->GetPosition();
        //     std::cout << position.x << " " << position.y << " " << position.z << std::endl;
        // }

        // if (it->GetMesh() != nullptr) { // pawn - использует сетку со скелетной анимацией
        //     // it->GetMesh()->set_transform(*it->GetTransform());
        //     // std::vector<aiMatrix4x4> Transforms;
        //     // it->GetMesh()->BoneTransform(GameManager::Time.GetCurrentTime(), Transforms);
        //     // Actor::Actor_rdata data = {&Transforms, it->GetMesh()};
        //     // GameManager::render.clearRender();
        //     // it->Render(&data);
        //     it->rootComponent->Render();
        // } else {
        //     it->Render(nullptr);
        // }
        it->Render();
    }
    for (auto& it : scene->shapes) {
        it->Render();
    }

    config->RenderScene();

    // scene->btManager.StepSimulation(GameManager::Time.GetDeltaTime());

    // std::vector<float> depthBuffer(GameManager::width * GameManager::height);
    // std::vector<unsigned char> pixels(GameManager::width * GameManager::height * 3);


    // glFinish(); // Ожидание конца рендеринга

    // glReadPixels(0, 0, GameManager::width, GameManager::height, GL_DEPTH_COMPONENT, GL_FLOAT, depthBuffer.data());
    // glReadPixels(0, 0, GameManager::width, GameManager::height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    // #pragma omp parallel
    // {
    //     #pragma omp single
    //     {
    //         int MAX_FRAME = 10;
    //         std::string dir_name("data");
    //         // Task 1: swapBuffer(), должен выполняться родительским потоком
    //         #pragma omp task
    //         {
    //             if (omp_get_thread_num() == 0) {
    //                 swapBuffer();
    //             }
    //         }
    //         // Task 2: запись данных в файл
    //         #pragma omp task
    //         {
    //             if (frame < MAX_FRAME) {
    //                 std::string file_name(dir_name + "/data_" + std::to_string(frame) + ".txt");
    //                 std::ofstream out(file_name, std::ios::app);
    //                 for (int i = 0; i < GameManager::width; i++) {
    //                     for (int j = 0; j < GameManager::height; j++) {
    //                         if (depthBuffer[j * GameManager::width + i] < 1.f) {
    //                             out << " "; // Пустой пиксель
    //                         } else {
    //                             out << "#"; // Пиксель с объектом
    //                         }
    //                     }
    //                     out << std::endl;
    //                 }
    //                 out.close();
    //             }
    //         }
    //         // Task 3: сохранение скриншота
    //         #pragma omp task
    //         {
    //             if (frame < MAX_FRAME) {
    //                 std::string file_name(dir_name + "/screenshot_" + std::to_string(frame) + ".png");
    //                 stbi_write_png(file_name.c_str(), GameManager::width, GameManager::height, 3, pixels.data(), GameManager::width * 3);
    //             }
    //         }
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
    Config::Initialize();

    auto *scene = new Scene();
    Ghost *character = new Ghost();

    Cube *skybox = new Cube("img/skybox.png");

    config = new Config();

    config->CreateScene();

    scene->skybox = skybox;
    scene->pushObject(character);
    // scene->pushObject(config);
    
    GameManager::PushPlayer(character);
    GameManager::render.pipeline.camera.OwnerTransformPtr = character->GetTransform();

    return scene;
}


int main(int argc, char** argv)
{
    const int width = 1600, height = 900;
    GameManager::InitializeGLFW(width, height);

    Scene *scene(createScene());

    while (!GameManager::IsEnd) {
        callback(scene);
    }

    glfwDestroyWindow(GameManager::window);
    glfwTerminate();

    return 0;
}
