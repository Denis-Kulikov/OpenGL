#include <entities/templates/playable/Ghost.hpp>
#include <entities/templates/mobs/Female.hpp>
#include <game/gameManager.hpp>
#include <object/sphere.hpp>
#include <object/sphere_wire.hpp>
#include <object/cube.hpp>

#include <threads/threads.hpp>
#include <game/gameManager.hpp> 
#include <mesh/mesh.hpp> 
#include <mesh/custom_mesh.hpp> 

#include <entities/templates/decor/wooden_box.hpp>

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
    GameManager::render.drawSkybox(*scene->skybox);

    for (auto& it : scene->pawns) {
        it->MoveForward();
        if (it->rootComponent != nullptr) {
            it->rootComponent->Move(glm::vec3(0, 0, -GameManager::Time.GetCurrentTime() / 10));
        }
        if (it->GetMesh() != nullptr) { // pawn - использует сетку со скелетной анимацией
            it->GetMesh()->set_transform(*it->GetTransform());
            std::vector<aiMatrix4x4> Transforms;
            it->GetMesh()->BoneTransform(GameManager::Time.GetCurrentTime(), Transforms);
            Actor::Actor_rdata data = {&Transforms, it->GetMesh()};
            GameManager::render.clearRender();
            // if (it->rootComponent != nullptr)
            //     (*it->rootComponent->GetTransform())->SetPosition(it->GetTransform()->GetWorldPos());
            it->Render(&data);
        } else {
            it->Render(nullptr);
        }
    }
    for (auto& it : scene->shapes) {
        it->Render();
    }

    scene->btManager.StepSimulation(GameManager::Time.GetDeltaTime());

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
    const std::size_t sphere_lvl = 20;
    const std::size_t sphere_wire_lvl = 10;

    Sphere<10>::initializeGeometry();
    Cube::initializeGeometry();
    Sprite::initializeGeometry();
    Line::initializeGeometry();
    Cube_wire::initializeGeometry();
    Sphere<sphere_lvl>::initializeGeometry();
    Sphere_wire<sphere_wire_lvl>::initializeGeometry();
    // Female::Initialize();
    WoodenBox::Initialize();

    auto *scene = new Scene();
    Cube* cube = new Cube("img/skybox.png");
    Ghost *character = new Ghost();

    Sprite* sprite = new Sprite("floor", "shaders/sprite_fs.glsl","shaders/sprite_vs.glsl", "img/grass.png");
    Cube* box = new Cube("img/box.jpg");
    Sphere<sphere_lvl>* sph = new Sphere<sphere_lvl>("img/brick_wall_10_diff_1k.jpg");
    Line* line = new Line();
    Sphere_wire<sphere_wire_lvl>* sph_w = new Sphere_wire<sphere_wire_lvl>();
    Cube_wire* bone_box = new Cube_wire();

    Shape* primitive = new Shape(sprite);
    Shape* primitive_cube = new Shape(box);
    Shape* primitive_sph = new Shape(sph);
    Shape* primitive_bbox = new Shape(bone_box);
    Shape* primitive_sph_w = new Shape(sph_w);
    Shape* primitive_line = new Shape(line);
    // Female* female1 = new Female(&scene->btManager);
    // Female* female2 = new Female(&scene->btManager);

    WoodenBox* woodenBox = new WoodenBox(&scene->btManager);

    scene->skybox = cube;
    scene->pushObject(character);
    //scene->pushObject(female1);
    //scene->pushObject(female2);
    scene->pushObject(woodenBox);
    scene->pushObject(primitive);
    scene->pushObject(primitive_cube);
    scene->pushObject(primitive_sph);
    scene->pushObject(primitive_bbox);
    scene->pushObject(primitive_sph_w);
    scene->pushObject(primitive_line);

    objectTransform transform;
    float scale = 5;
    // transform.SetWorldPos(0.0, -10.0, -10.0);
    // transform.SetRotate(-90.0, 0.0, 0.0);
    // transform.SetScale(glm::vec3(scale, scale, scale));
    // *female1->GetTransform() = transform;

    // *woodenBox->GetTransform() = transform;

    // transform.SetWorldPos(5.0, -10.0, -10.0);
    // *female2->GetTransform() = transform;
        
    scale = 15;
    transform.SetWorldPos(5.0, -10.0, -10.0);
    transform.SetRotate(90.0, 0.0, 0.0);
    transform.SetScale(glm::vec3(scale, scale, scale));
    *primitive->GetTransform() = transform;
    
    transform.SetRotate(0.0, 0.0, 0.0);


    scale = 1;
    transform.SetWorldPos(8.0, -10.0 + scale / 2, -10.0);
    transform.SetScale(glm::vec3(scale, scale, scale));
    *primitive_cube->GetTransform() = transform;

    scale = 5;
    transform.SetWorldPos(9.0, 0.0 + scale / 2, -10.0);
    transform.SetScale(glm::vec3(scale, scale, scale));
    *primitive_sph->GetTransform() = transform;
    sph->SetColor(glm::vec3(1, 0, 1));

    scale = 1;
    transform.SetWorldPos(10.0, -10.0 + scale / 2, -10.0);
    transform.SetScale(glm::vec3(scale, scale, scale));
    *primitive_bbox->GetTransform() = transform;
    bone_box->SetColor(glm::vec3(1, 0, 0));

    scale = 1;
    transform.SetWorldPos(11.0, -10.0 + scale / 2, -10.0);
    transform.SetScale(glm::vec3(scale, scale, scale));
    *primitive_sph_w->GetTransform() = transform;
    sph_w->SetColor(glm::vec3(0, 0, 1));

    *primitive_line->GetTransform() = line->setPoints(glm::vec3(8.0, -10.0 + scale / 2, -9.0), glm::vec3(11.0, -10.0 + scale / 2, -9.0));
    line->SetColor(glm::vec3(0, 1, 0));
    line->SetWidth(3);

    //female1->rootComponent->GetLocalTransform()->SetPosition(female1->GetTransform()->GetWorldPos());
    //female2->rootComponent->GetLocalTransform()->SetPosition(female2->GetTransform()->GetWorldPos());
    
    glm::vec3 pos(0, 0, 0);
    Transform transformSpawn;
    transformSpawn.SetPosition(pos);
    // woodenBox->rootComponent->GetLocalTransform()->SetPosition(pos);
    woodenBox->Spawn(&transformSpawn);

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

    while (!GameManager::IsEnd) {
        callback(scene.get());
    }

    glfwDestroyWindow(GameManager::window);
    glfwTerminate();

    return 0;
}
