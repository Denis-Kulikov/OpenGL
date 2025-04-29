#include <entities/templates/playable/Ghost.hpp>
#include <entities/templates/mobs/Female.hpp>
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

#include <dynamic_compiler/dynamic_compiler.hpp>


std::chrono::milliseconds totalTime(0);
Config* config = nullptr;
Pawn *character = nullptr;

float time_render = 0;
float time_save = 0;

void swapBuffer() {
    glfwSwapBuffers(GameManager::window);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

void draw(Scene *scene) {
    static int frame = 1;

    // if (frame % 10 == 0)
    //     std::cout << "Frame: " << frame << std::endl;

    GameManager::Time.Update();
    auto start_render = GameManager::Time.GetCurrentTime();
    GameManager::UpdateCamera();
    GameManager::render.GetPV();
    GameManager::callbackData.player->MoveForward();
    GameManager::render.drawSkybox(*scene->skybox);

    for (auto &it : scene->shapes) {
        it->Render();
    }

    swapBuffer();

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

    scene->skybox = new Cube("img/skybox.png");

    auto cube = new Cube("img/box.jpg");
    auto cube_shape = new Shape(cube);
    cube_shape->GetTransform()->SetWorldPos(glm::vec3(0, 0, 2));

    scene->pushObject(cube_shape);

    GameManager::PushPlayer(reinterpret_cast<Character*>(character));
    GameManager::render.pipeline.camera.OwnerTransformPtr = character->GetTransform();

    // config = new Config();
    // config->CreateScene();

    swapBuffer();

    return scene;
}


int main(int argc, char** argv)
{
    DynamicCompiler compiler;
    std::cout << "DynamicCompiler" << std::endl;

    if (!compiler.compile("script.cpp")) {
        std::cerr << "Compilation failed." << std::endl;
        return 1;
    }

    // Ищем и вызываем функцию hello
    using HelloFunc = void (*)();
    auto *hellofunc = reinterpret_cast<HelloFunc>(compiler.getSymbol("hello"));
    if (!hellofunc) {
        std::cerr << "Function 'hello' not found." << std::endl;
        return 1;
    }

    hellofunc(); // Вызывает hello()

    
    if (!compiler.compile("create_scene.cpp")) {
        std::cerr << "Compilation failed." << std::endl;
        return 1;
    }
    std::cout << "compiler.compile" << std::endl;

    using CreateScene = Scene *(*)();
    auto *func = reinterpret_cast<CreateScene>(compiler.getSymbol("CreateScene"));
    if (!func) {
        std::cerr << "Function 'CreateScene' not found." << std::endl;
        return 1;
    }

    std::cout << "compiler.getSymbol" << std::endl;

    Scene *scene(func());
    std::cout << "scene(func())" << std::endl;
    character = scene->pawns[0];
    std::cout << "scene->pawns[0]" << std::endl;


    const int width = 1600, height = 900;
    GameManager::InitializeGLFW(width, height);

    // Scene *scene(createScene());
    // ThreadPool pool(15, 100);

    while (!GameManager::IsEnd) {
        draw(scene);
    }

    // for (int i = 0; i < config->GetFrames(); ++i) {
        // for (int j = 0; j < config->GetFrames() / 2 + 1; ++j) {
        //     drow(scene, pool, 0, j);
        // }

        // for (int j = 0; j < config->GetFrames() / 2 + 1; ++j) {
        //     drow(scene, pool, 15, j);
        // }
    // }

    std::cout << "Total time:\t" << GameManager::Time.GetCurrentTime() << std::endl;

    // pool.shutdown();

    glfwDestroyWindow(GameManager::window);
    glfwTerminate();

    return 0;
}
