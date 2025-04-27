#include <entities/templates/decor/config.hpp>
#include <object/sphere.hpp>
#include <object/component/template/point.hpp>
#include <functional>

Config::Config()
    : Pawn(std::string(""))
{
    Transform *transform = new Transform();
    rootComponent = CreateComponent<ComponentPoint>(transform);
    rootComponent->SetPosition(glm::vec3(0));
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
    height = 512;
    width = 512;
    num_frames = 4;

    // const int   numTurns     = 5;        // Кол-во витков спирали
    // const int   stepsPerTurn = 60;       // Точек на один виток
    // const float maxRadius    = 1000.0f;  // Максимальный радиус спирали
    // const float heightStart  = 500.0f;   // Начальная высота в центре
    // const float heightEnd    = 100.0f;   // Конечная высота на краю
    // const float cubeSize     = 50.0f;

    // const int totalSteps = numTurns * stepsPerTurn;

    // for (int step = 0; step < totalSteps; ++step) {
    //     float t = float(step) / float(totalSteps - 1); // от 0 до 1
    //     float angle = glm::two_pi<float>() * numTurns * t;
    //     float radius = t * maxRadius;
    //     float height = glm::mix(heightStart, heightEnd, t); // линейный спад

    //     glm::vec3 pos {
    //         radius * cos(angle),
    //         height,
    //         radius * sin(angle)
    //     };

    //     cube c;
    //     c.SetPosition(pos);
    //     c.SetScale(glm::vec3(cubeSize)); // равномерный куб

    //     // Поворот вдоль спирали
    //     glm::quat rot = glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));
    //     c.SetRotation(rot);

    //     add(c);
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
