#include <entities/templates/decor/config.hpp>
#include <object/sphere.hpp>
#include <object/component/template/point.hpp>
#include <functional>

Config::Config()
    : Pawn(std::string(""))
{
    Transform *transform = new Transform();
    rootComponent = CreateComponent<ComponentPoint>(transform);
    rootComponent->SetPosition(glm::vec3(-50.f * 15 / 2, 250.f, -50.f * 15 / 2));
    // rootComponent = CreateComponent<ComponentShape>(transform);
    // dynamic_cast<ComponentShape*>(rootComponent)->shape = new Cube("img/box.jpg");
}

Config::~Config() {}

void Config::Initialize()
{
    Config::name = "Config";
    sprite_obj::Initialize();
    cube_obj::Initialize();
    sphere_obj::Initialize();
}

void Config::Deinitialization() {
    Sphere<sphere_quality>::initializeGeometry();
    sprite_obj::Deinitialization();
    cube_obj::Deinitialization();
    sphere_obj::Deinitialization();
}

std::string Config::GetName() const { return Config::name; }
Mesh *Config::GetMesh() const { return nullptr; }

void Config::DestroyBuffers() {

}

void Config::CreateBuffers(GLfloat width, GLfloat height) {
    GameManager::render.pipeline.camera.PersProj.Width  = scene.width;
    GameManager::render.pipeline.camera.PersProj.Height = scene.height;

    GameManager::width  = scene.width;
    GameManager::height = scene.height;

    glBindFramebuffer(GL_FRAMEBUFFER, GameManager::framebuffer);

    // Создание текстуры для рендеринга
    glBindTexture(GL_TEXTURE_2D, GameManager::texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GameManager::texture, 0);

    // Создание depth буфера
    glBindRenderbuffer(GL_RENDERBUFFER, GameManager::depthbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, GameManager::depthbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete" << std::endl;
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, width, height);
}

void Config::UpdateCanvas() {
    if (scene.width != lastWidth || scene.height != lastHeight) {
        DestroyBuffers(); // удалить предыдущие ресурсы
        CreateBuffers(scene.width, scene.height); // создать новые
        lastWidth = scene.width;
        lastHeight = scene.height;
    }
}


// ### ### ###
#define new_sprite     new sprite_obj()
#define new_cube       new cube_obj()
#define new_sphere     new sphere_obj()
#define new_mesh(PATH) new mesh_obj(PATH)

#define sprite   sprite_obj
#define cube     cube_obj
#define sphere   sphere_obj
#define mesh     mesh_obj

// #define fun(name, ...) auto name = [](__VA_ARGS__)
#define fun(name, ...) std::function<void(__VA_ARGS__)> name = [&]( __VA_ARGS__ )

using namespace glm;

Config::ConfigScene::ConfigScene() {
    height = 3000;
    width = 3000;
    num_frames = 6;
    int gridSize = 20;     // Размер сетки
    float spacing = 25.0f;  // Расстояние между спиралями

    cube Cube1;
    Cube1.SetPosition(vec3(0, 0, 0));
    Cube1.SetScale(vec3(500, 500, 500));
    // Cube1.SetPosition(vec3(0, 1000, 0));
    // Cube1.SetScale(vec3(1000, 10, 10));

    // cube Cube2;
    // Cube2.SetPosition(vec3(0, -1000, 0));
    // Cube2.SetScale(vec3(1000, 10, 10));

    // cube Cube3;
    // Cube3.SetPosition(vec3(500, 0, 0));
    // Cube3.SetScale(vec3(10, 1000, 10));

    // cube Cube4;
    // Cube4.SetPosition(vec3(-500, 0, 0));
    // Cube4.SetScale(vec3(10, 1000, 10));

    add(Cube1);
    // add(Cube2);
    // add(Cube3);
    // add(Cube4);

    // fun(BuildSpiral, int step, int maxSteps, cube parent) {
    //     if (step >= maxSteps) return;

    //     float angle = glm::radians(step * 30.0f);
    //     float radius = 2.0f;
    //     float heightStep = 2.0f;

    //     cube childCube;
    //     glm::vec3 position(radius * cos(angle), step * heightStep, radius * sin(angle));
    //     childCube.SetPosition(position);

    //     glm::quat rotation = glm::angleAxis(-angle, glm::vec3(0, 1, 0));
    //     childCube.SetRotation(rotation);

    //     childCube.attach(parent);
    //     add(childCube);

    //     BuildSpiral(step + 1, maxSteps, childCube);
    // };

    // for (int x = 0; x < gridSize; x++) {
    //     for (int y = 0; y < gridSize; y++) {
    //         cube rootCube;
    //         rootCube.SetPosition(vec3(x * spacing, 0.0f, y * spacing));
    //         add(rootCube);

    //         BuildSpiral(0, 100, rootCube);
    //     }
    // }
}

// Config::ConfigScene::ConfigScene() {
//     cube cube1;
//     cube cube2;
//     cube1.SetPosition(vec3(1.f));
//     cube2.SetPosition(vec3(1.f));
//     cube2.attach(cube1);
//     add(cube1);
//     add(cube2);

//     fun(foo, int step, int max_step, cube obj) {
//         if (step >= max_step) return;

//         cube cube_child;
//         cube_child.SetPosition(vec3(step));
//         cube_child.attach(obj);
//         add(cube_child);

//         foo(step + 1, max_step, cube_child);
//     };

//     foo(0, 5, cube2);
// }

    // mesh model = new_mesh("assets/model/female/female.glb");
    // add(model);
