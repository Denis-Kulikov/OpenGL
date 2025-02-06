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
#include <entities/templates/decor/floor.hpp>

#include <entities/templates/mobs/Female.hpp>
#include <stb_image.h>
#include <stb_image_write.h>

std::chrono::milliseconds totalTime(0);

#include <glm/gtx/euler_angles.hpp>
glm::vec3 QuatToEuler(glm::quat q) {
    float yaw, pitch, roll;
    glm::extractEulerAngleYXZ(glm::toMat4(q), yaw, pitch, roll);
    return glm::degrees(glm::vec3(pitch, yaw, roll));
}

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

    static bool flag1 = true;
    static bool flag2 = true;
    for (auto& it : scene->pawns) {
        it->MoveForward();
        if (it->rootComponent != nullptr && (it->GetName() == "WoodenBox")) {
            glm::quat rotationZ = glm::angleAxis(glm::radians(GameManager::Time.GetDeltaTime() * 20), glm::vec3(0, 1, 0));
            // it->rootComponent->Move(glm::vec3(0, 0, -GameManager::Time.GetCurrentTime() / 100));

            float speed = 2.0f; // Скорость вращения
            float radius = 3.0f; // Радиус движения
            float angle = GameManager::Time.GetCurrentTime() * speed;

            glm::vec3 circularPosition = glm::vec3(sinf(angle) * radius, 0, cosf(angle) * radius);
            it->rootComponent->Move(circularPosition, GameManager::Time.GetDeltaTime() * 2.01f);
            it->rootComponent->Rotate(rotationZ);

            Transform transformSpawn;
            glm::vec3 pos(0, 10, -15);
            // glm::quat rotation = glm::angleAxis(glm::radians(0.1f), glm::vec3(0, 1, 0));
            transformSpawn.SetPosition(pos);
            // transformSpawn.SetRotation(rotation);
            transformSpawn.UpdateMatrix();
            // it->Spawn();
            // it->rootComponent->Spawn(transformSpawn.GetMatrix());
            // it->rootComponent->globalTransform->SetMatrix(transformSpawn.GetMatrix());
            // auto position = it->rootComponent->globalTransform->GetPosition();
            // std::cout << position.x << " " << position.y << " " << position.z << std::endl;
        }
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
    // std::cout << std::endl;

    scene->btManager.StepSimulation(GameManager::Time.GetDeltaTime());

    // BulletManager &btManager = scene->btManager;
    // btDynamicsWorld *world = btManager.dynamicsWorld;
    // if (!world) {
    //     std::cerr << "Error: dynamicsWorld is null!" << std::endl;
    //     return;
    // }
    // int numObjects = world->getNumCollisionObjects();
    // for (int i = 0; i < numObjects; i++) {
    //     btCollisionObject *obj = world->getCollisionObjectArray()[i];
    //     btRigidBody *body = btRigidBody::upcast(obj);
    //     if (body && body->getMotionState()) {
    //         btTransform transform;
    //         body->getMotionState()->getWorldTransform(transform);
    //         btVector3 position = transform.getOrigin();
    //         btQuaternion rotation = transform.getRotation();  // Получаем кватернион поворота

    //         // Конвертация кватерниона в угол (радианы) и ось
    //         btVector3 axis = btVector3(0, 0, 0);
    //         btScalar angle = rotation.getAngle(); // Угол в радианах
    //         if (angle > 0.0001f) { // Проверка, что угол не нулевой
    //             axis = rotation.getAxis();
    //         }

    //         // Вывод в градусах
    //         float angleDegrees = angle * (180.0f / glm::pi<float>());

    //         std::cout << "Object " << i << ": Position = ("
    //                 << position.x() << ", " << position.y() << ", " << position.z() << ")"
    //                 << ", Rotation Axis = (" << axis.x() << ", " << axis.y() << ", " << axis.z() << ")"
    //                 << ", Angle = " << angleDegrees << " degrees"
    //                 << std::endl;
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
    const std::size_t sphere_lvl = 20;
    const std::size_t sphere_wire_lvl = 10;

{
    Sphere<10>::initializeGeometry();
    Cube::initializeGeometry();
    Sprite::initializeGeometry();
    Line::initializeGeometry();
    Cube_wire::initializeGeometry();
    Sphere<sphere_lvl>::initializeGeometry();
    Sphere_wire<sphere_wire_lvl>::initializeGeometry();

    Female::Initialize();
    WoodenBox::Initialize();
    Floor::Initialize();
}

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
    Female* female1 = new Female(&scene->btManager);
    // Female* female2 = new Female(&scene->btManager);

    WoodenBox* woodenBox = new WoodenBox(&scene->btManager);
    Floor* floor = new Floor(&scene->btManager);

    {
    scene->skybox = cube;
    scene->pushObject(character);
    scene->pushObject(female1);
    //scene->pushObject(female2);
    scene->pushObject(woodenBox);
    scene->pushObject(floor);

    scene->pushObject(primitive);
    scene->pushObject(primitive_cube);
    scene->pushObject(primitive_sph);
    scene->pushObject(primitive_bbox);
    scene->pushObject(primitive_sph_w);
    scene->pushObject(primitive_line);
    }

    objectTransform transform;
    float scale = 5;
    {
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
    transform.SetWorldPos(8.0, -10.0 + scale, -10.0);
    transform.SetScale(glm::vec3(scale, scale, scale));
    *primitive_cube->GetTransform() = transform;

    scale = 1;
    transform.SetWorldPos(10.0, -10.0 + scale, -10.0);
    transform.SetScale(glm::vec3(scale, scale, scale));
    *primitive_bbox->GetTransform() = transform;
    bone_box->SetColor(glm::vec3(1, 0, 0));

    scale = 1;
    transform.SetWorldPos(12.0, -10.0 + scale, -10.0);
    transform.SetScale(glm::vec3(scale, scale, scale));
    *primitive_sph_w->GetTransform() = transform;
    sph_w->SetColor(glm::vec3(0, 0, 1));

    scale = 1;
    transform.SetWorldPos(14.0, -10.0 + scale, -10.0);
    transform.SetScale(glm::vec3(scale, scale, scale));
    *primitive_sph->GetTransform() = transform;
    sph->SetColor(glm::vec3(1, 0, 1));

    *primitive_line->GetTransform() = line->setPoints(glm::vec3(8.0, -10.0 + scale, -9.0), glm::vec3(11.0, -10.0 + scale, -9.0));
    line->SetColor(glm::vec3(0, 1, 0));
    line->SetWidth(3);
    }

{
    Transform transformSpawn;
    glm::quat rotationX = glm::angleAxis(glm::radians(-45.0f), glm::vec3(1, 0, 0)); // 45° вокруг X
    transformSpawn.SetPosition(glm::vec3(0.0, 0.0, -10.0));
    transformSpawn.SetPosition(glm::vec3(2.0, -10.0 + 3 + 5, -15.0));
    // transformSpawn.SetRotation(rotationX);
    woodenBox->Spawn(transformSpawn);
    // woodenBox->rootComponent->UpdateMatrixTree();
}
{
    Transform transformSpawn;
    transformSpawn.SetPosition(glm::vec3(2, 10, 0));
    female1->Spawn(transformSpawn);
}
{
    Transform transformSpawn;
    auto rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(1, 0, 0)); // Без поворота
    transformSpawn.SetPosition(glm::vec3(0, -9, 0));
    // transformSpawn.SetRotation(rotation);
    floor->Spawn(transformSpawn);
    // floor->rootComponent->UpdateMatrixTree();
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

    while (!GameManager::IsEnd) {
        callback(scene.get());
    }

    glfwDestroyWindow(GameManager::window);
    glfwTerminate();

    return 0;
}
