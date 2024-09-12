#include <entities/templates/playable/Wilson.hpp>
#include <entities/templates/playable/Ghost.hpp>
#include <entities/templates/mobs/spider.hpp>
#include <entities/templates/decor/wave.hpp>
#include <entities/templates/decor/grass.hpp>
#include <game/gameManager.hpp>
#include <object/sphere.hpp>
#include <object/cube.hpp>


std::chrono::milliseconds totalTime(0);

float randomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

Vector3<GLfloat> generateRandomPoint() {
    float MIN_X = -30.0, MAX_X = 30.0;
    float MIN_Y = -3.0, MAX_Y = 3.0;
    float MIN_Z = -64.0, MAX_Z = 2.0;
    float x = randomFloat(MIN_X, MAX_X);
    float y = randomFloat(MIN_Y, MAX_Y);
    float z = randomFloat(MIN_Z, MAX_Z);
    return Vector3<GLfloat>(x, y, z);
}

Scene *createScene()
{
    Ghost *character = new Ghost();
    auto *scene = new Scene();

    character->updateAnimation("stand");
    scene->pushActor(character);

    Sphere<10>::initializeGeometry();
    Cube::initializeGeometry();

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
    GameManager::threads->setScene(scene.get());
    GameManager::threads->start();

    glfwDestroyWindow(GameManager::window);
    glfwTerminate();

    return 0;
}
