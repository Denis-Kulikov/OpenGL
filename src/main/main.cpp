#include <entities/templates/playable/Ghost.hpp>
#include <entities/templates/mobs/Female.hpp>
#include <game/gameManager.hpp>
#include <object/sphere.hpp>
#include <object/cube.hpp>

#include <threads/threads.hpp>
#include <game/gameManager.hpp> 
#include <mesh/mesh.hpp> 

#include <entities/templates/mobs/Female.hpp>


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
    GameManager::render->GetPV();
    GameManager::UpdateCamera();
    GameManager::render->drawSkybox(*scene->skybox);

    for (auto& it : scene->actors) {
        (reinterpret_cast<Pawn*>(it))->MoveForward();
        if (it->GetMesh() != nullptr) {
            it->GetMesh()->set_transform(*it->GetTransform());
            std::vector<aiMatrix4x4> *Transforms = new std::vector<aiMatrix4x4>;
            it->GetMesh()->BoneTransform(GameManager::Time.GetCurrentTime(), *Transforms);
            GameManager::render->clearRender();
            Actor::Actor_rdata data = {Transforms, it->GetMesh()};
            it->Render(&data);
            GameManager::render->clearRender();
        }
    }

    // for (auto& it : scene->sprites) {
    //     it->Render(&GameManager::render->pipeline.GetTransform(it. ->GetTransform()));
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
    Sphere<10>::initializeGeometry();
    Cube::initializeGeometry();
    Sprite::initializeGeometry();
    Female::Initialize();

    auto *scene = new Scene();
    Cube* cube = new Cube("img/skybox.png");
    Ghost *character = new Ghost();
    Sprite* sprite = new Sprite("floor", "shaders/sprite_fs.glsl","shaders/sprite_vs.glsl", "img/grass.png");
    Female* female1 = new Female;
    Female* female2 = new Female;

    scene->skybox = cube;
    scene->pushObject(character);
    scene->pushObject(female1);
    scene->pushObject(female2);
    scene->pushObject(sprite);

    objectTransform transform;
    float scale = 5;
    transform.SetWorldPos(0.0, -10.0, -10.0);
    transform.SetRotate(-90.0, 0.0, 0.0);
    transform.SetScale(glm::vec3(scale, scale, scale));
    *female1->GetTransform() = transform;

    transform.SetWorldPos(5.0, -10.0, -10.0);
    *female2->GetTransform() = transform;
        
    scale = 10;
    transform.SetWorldPos(0.0, -10.0, -10.0);
    transform.SetRotate(90.0, 0.0, 0.0);
    transform.MultiplyScale(glm::vec3(scale, scale, scale));

    GameManager::PushPlayer(character);
    GameManager::render->pipeline.camera->OwnerTransformPtr = character->GetTransform();

    return scene;
}


int main(int argc, char** argv)
{
    const int width = 1600, height = 960;
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
