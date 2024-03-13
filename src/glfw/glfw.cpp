#include <render/glfw.hpp>
#include <render/render.hpp>
#include <object/scene.hpp>
#include <object/sphere.hpp>
#include <object/line.hpp>
#include <object/cube_bone.hpp>
#include <lib-project/try.hpp>
#include <game/gameManager.hpp>
#include <entities/actor.hpp>
#include <entities/pawn.hpp>
#include <entities/character.hpp>
#include <entities/templates/playable/Wilson.hpp>
// #include <entities/templates/playable/spider.hpp>

#include <chrono>
#include <ctime>

// Bone Wilson::Skelet;
// Bone Spider::Skelet;

// Character *character = nullptr;
Wilson *character = nullptr;
// Spider *spider = nullptr;

// Растеризация. Проекция перспективы. Скелетная анимация 2D моделей.

time_t prev = 0;
int frame = 0;

bool RenderSceneCB(Render *render, Scene *scene)
{
    for (std::vector<Component>::iterator it = scene->getIterator(); it != scene->component.end(); it++)
        GameManager::render->drawObject(it->transform, *it->sprite);

    // std::vector<Component*> ActorComponents = character->getActorComponents(&character->skelet);

    // character->UpdateCameraPos();

    // for (auto it : ActorComponents) {
    //     if (it->sprite == nullptr) continue;
    //     // it->trans.print();
    //     GameManager::render->drawObject(it->transform, *it->sprite);
    // }

    // if (spider != nullptr) {
    //     spider->MoveTowards(character, 0.006);
    //     ActorComponents = spider->getActorComponents(&spider->skelet);
    //     for (auto it : ActorComponents) {
    //         if (it->sprite == nullptr) continue;
    //         // it->trans.print();
    //         GameManager::render->drawObject(it->transform, *it->sprite);
    //     }
    // }

    
    frame++;
    if ((time(0) - prev) > 3) {
        std::cout << "FPS: " << frame / 3 << std::endl;
        prev = time(0);
        frame = 0;
    } 

    return GameManager::IsEnd;
}

Scene *createScene()
{
    auto *scene = new Scene(std::string("Main scene"));

    Vector3<GLfloat> whiteColor(1, 1, 1);
    Vector3<GLfloat> redColor(1, 0, 0);
    Sprite *mySprite;
    objectTransform _trans;

    mySprite = new Sprite(std::string("Grass"), _trans, "shaders/Sprite_fs.glsl", "shaders/Sprite_vs.glsl", "img/grass.png");
    objectTransform transformGrass;
    transformGrass.Move(0, -3, 2);
    transformGrass.SetRotate(90, 0, 0);
    transformGrass.SetScale(5, 5, 0);
    Component component(transformGrass, mySprite);
    scene->pushObject(component);
    
    // mySprite = new Sprite(std::string("Wilson"), _trans, "shaders/Sprite_fs.glsl", "shaders/Sprite_vs.glsl", "img/Wilson.png");
    // mySprite->trans.Move(1, -1 + (mySprite->trans.Scale.y), 2);
    // scene->pushObject(mySprite);


    // mySprite = new Sprite(std::string("chess1"), _trans, "shaders/Sprite_fs.glsl", "shaders/Sprite_vs.glsl", "img/chess.jpg");
    // mySprite->trans.Move(0, 1, 1);
    // scene->pushObject(mySprite);

    // sphere *mySphere = new sphere(std::string("MySphere1"), _trans, "shaders/sphere_fs.glsl", "shaders/base_vs.glsl", nullptr, 4);
    // mySphere->trans.Move(-1, 0, 2);
    // mySphere->trans.SetScale(0.1, 0.1, 0.1);
    // scene->pushObject(mySphere);
    // mySphere = new sphere(std::string("MySphere1"), _trans, "shaders/sphere_fs.glsl", "shaders/base_vs.glsl", nullptr, 4);
    // mySphere->trans.Move(1, 1, 1);
    // mySphere->trans.SetScale(0.1, 0.1, 0.1);
    // scene->pushObject(mySphere);


    // mySprite = new Sprite(std::string("chess1"), _trans, "shaders/Sprite_fs.glsl", "shaders/Sprite_vs.glsl", "img/chess.jpg");
    // mySprite->trans.Move(0, 1, 2);
    // scene->pushObject(mySprite);

    // sphere *mySphere = new sphere(std::string("MySphere"), _trans, "shaders/sphere_fs.glsl", "shaders/base_vs.glsl", nullptr, 4);
    // mySphere->trans.Move(-3, -3, 7);
    // scene->pushObject(mySphere);

    // cube_bone *myCube_bone = new cube_bone(std::string("MyCube"), _trans, whiteColor);
    // myCube_bone->trans.Move(0, -1, 3);
    // scene->pushObject(myCube_bone);


    // line *myLine = new line(std::string("MyLine"), _trans, redColor);
    // // myLine->trans.Move(0, 0, 1);
    // myLine->setPoints(Vector3<GLfloat>(1, 1, 1), Vector3<GLfloat>(-1, 0, 2));
    // scene->pushObject(myLine);


    // Actor<Wilson>::loadSprites(&Wilson::Sprites, "player/Wilson");


    // std::string path("player/Wilson");
    // Actor::loadSprites(path);
    // character = new Character(path);
    character = new Wilson();
    // character.skelet.createSkelet(path, "skelet");
    // character->loadAnimation(path, "stand");
    GameManager::PushPlayerTransform(&character->trans);
    // character->createCamera(GameManager::width, GameManager::height);

    // std::cout << "Address: " << &Wilson::Sprites << std::endl;

    // spider = new Spider();


    GameManager::render->SetCamera(character->camera);
    GameManager::PushCamera(character->camera);
    // GameManager::PushCamera(&pawn->trans);

    return scene;
}
