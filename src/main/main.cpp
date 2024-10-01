#include <entities/templates/playable/Ghost.hpp>
#include <entities/templates/mobs/Female.hpp>
#include <game/gameManager.hpp>
#include <object/sphere.hpp>
#include <object/cube.hpp>
#include <object/sphere_wire.hpp>

#include <threads/threads.hpp>
#include <game/gameManager.hpp> 
#include <mesh/mesh.hpp> 
#include <mesh/primitive_mesh.hpp>

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
    GameManager::render.GetPV();
    GameManager::UpdateCamera();
    GameManager::render.drawSkybox(*scene->skybox);

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
    }
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
    const std::size_t sphere_lvl = 6;
    const std::size_t sphere_wire_lvl = 2;

    Cube::initializeGeometry();
    Sprite::initializeGeometry();
    Line::initializeGeometry();
    Cube_wire::initializeGeometry();
    Sphere<sphere_lvl>::initializeGeometry();
    Sphere_wire<sphere_wire_lvl>::initializeGeometry();
    Female::Initialize();

    auto *scene = new Scene();
    Cube* cube = new Cube("img/skybox.png");
    Ghost *character = new Ghost();

    Sprite* sprite = new Sprite("floor", "shaders/sprite_fs.glsl","shaders/sprite_vs.glsl", "img/grass.png");
    Cube* box = new Cube("img/box.jpg");
    Sphere<sphere_lvl>* sph = new Sphere<sphere_lvl>();
    Line* line = new Line();
    Sphere_wire<sphere_wire_lvl>* sph_w = new Sphere_wire<sphere_wire_lvl>();
    Cube_wire* bone_box = new Cube_wire();

    Primitive_mesh* primitive = new Primitive_mesh(sprite);
    Primitive_mesh* primitive_cube = new Primitive_mesh(box);
    Primitive_mesh* primitive_sph = new Primitive_mesh(sph);
    Primitive_mesh* primitive_bbox = new Primitive_mesh(bone_box);
    Primitive_mesh* primitive_sph_w = new Primitive_mesh(sph_w);
    Primitive_mesh* primitive_line = new Primitive_mesh(line);
    Female* female1 = new Female;
    Female* female2 = new Female;

    scene->skybox = cube;
    scene->pushObject(character);
    scene->pushObject(female1);
    scene->pushObject(female2);
    scene->pushObject(primitive);
    scene->pushObject(primitive_cube);
    scene->pushObject(primitive_sph);
    scene->pushObject(primitive_bbox);
    scene->pushObject(primitive_sph_w);
    scene->pushObject(primitive_line);

    objectTransform transform;
    float scale = 5;
    transform.SetWorldPos(0.0, -10.0, -10.0);
    transform.SetRotate(-90.0, 0.0, 0.0);
    transform.SetScale(glm::vec3(scale, scale, scale));
    *female1->GetTransform() = transform;

    transform.SetWorldPos(5.0, -10.0, -10.0);
    *female2->GetTransform() = transform;
        
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

    scale = 1;
    transform.SetWorldPos(9.0, -10.0 + scale / 2, -10.0);
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

    GameManager::PushPlayer(character);
    GameManager::render.pipeline.camera.OwnerTransformPtr = character->GetTransform();

    return scene;
}


int main(int argc, char** argv)
{
    const int width = 1600, height = 960;
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
