#include <render/glfw.hpp>
#include <render/render.hpp>
#include <object/scene.hpp>
#include <object/sphere.hpp>
#include <object/line.hpp>
#include <object/cube_bone.hpp>
#include <lib-project/try.hpp>
#include <entities/actor.hpp>
#include <game/gameManager.hpp>

Actor *actor = nullptr;

// Растеризация. Проекция перспективы. Скелетная анимация 2D моделей.

bool RenderSceneCB(Render *render, Scene *scene)
{
    for (std::vector<Sprite*>::iterator it = scene->getIterator(); it != scene->sprites.end(); it++) render->drawObject(**it);

    std::vector<Sprite*> ActorComponents = actor->getActorComponents(&actor->skelet);

    for (auto it : ActorComponents) {
        if (it == nullptr) continue;
        // it->trans.print();
        render->drawObject(*it);
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

    // mySprite = new Sprite(std::string("Grass"), _trans, "shaders/Sprite_fs.glsl", "shaders/Sprite_vs.glsl", "img/grass.png");
    // mySprite->trans.Move(1, -1, 2);
    // mySprite->trans.SetRotate(90, 0, 0);
    // mySprite->trans.SetScale(3, 3, 0);
    // scene->pushObject(mySprite);
    
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


    std::string path("player/Wilson");
    Actor::loadSprites(path);
    actor = new Actor(path);
    actor->skelet.createSkelet(path, "skelet", &Actor::sprites);
    actor->loadAnimation(path, "stand");

    return scene;
}
