#pragma once

#include "../object/sprite.hpp"
#include "../entities/components/bone.hpp"
// #include "../game/gameTime.hpp"
#include <filesystem>
#include <pugixml.hpp>

#define MY_ACTOR_TEST true

#if MY_ACTOR_TEST
#include "../render/render.hpp"
#include "../object/sphere.hpp"
#include "../object/line.hpp"
#endif


namespace fs = std::filesystem;

namespace STATE
{
    enum
    {
        DEFAULT,
        STAND,
        GO,
        ACTION,
        GET_HIT
    };
}

constexpr size_t myHash(const char* s)
{
    size_t hash = 0;
    while (*s) {
        hash = (hash * 31) + *s;
        ++s;
    }
    
    return hash;
}

class Actor 
{
public:
    Actor(const std::string &path, const size_t SkeletSize)
    {
        loadActor(path);
        trans.Rotate = Vector3<GLfloat>(0.0, 0.0, 180);
        direction = Vector3<GLfloat>();


        globalFlip = new GLfloat[SkeletSize];
        components = new Component[SkeletSize];
        animations = new Animation*[SkeletSize];
        for (int i = 0; i < SkeletSize; i++) {
            animations[i] = nullptr;
        }
        animations[0] = new Animation();
        globalFlip[0] = 0;

        #if MY_ACTOR_TEST
        spherePos = new Vector3<GLfloat>[SkeletSize]();

        mySphere = sphere(std::string("mySphere"), "shaders/sphere_fs.glsl", "shaders/sphere_vs.glsl", nullptr, 10);
        sphereTransform.Rotate = Vector3<GLfloat>(0.0, 0.0, 0.0);
        sphereTransform.Scale = Vector3<GLfloat>(0.1, 0.2, 0.1);
        
        myLine = line(std::string("myLine"), Vector3<GLfloat>(1.0, 0.0, 0.0));
        #endif
    }
    
    ~Actor() {}

    virtual size_t GetSkeletSize() = 0;
    virtual Bone *GetSkelet() = 0;
    virtual std::map<std::string, Sprite> *GetSprites() = 0;
    virtual std::string *GetName() = 0;


    std::vector<Component*> getActorComponents(Bone *_parent, size_t &n, const GLfloat duration)
    {
        std::vector<Component*> ActorComponents;

        auto it = _parent->Animations.find("stan");

        const size_t parentNumber = n++;
        const objectTransform ParentSprite = components[parentNumber].transform;
        const GLfloat globalScaleX = ParentSprite.Scale.x / animations[parentNumber]->spriteScale.x;
        const GLfloat globalScaleY = ParentSprite.Scale.y / animations[parentNumber]->spriteScale.y;

        for (const auto &it : _parent->children) {
            const Animation &animation_Num = *animations[n]; // ссылка из-за векторов в Motion
            const objectTransform component = animation_Num.transform;

            Motion::anchorDirection = globalFlip[n]; // нужно для GetFlip
            const GLfloat flipMotion = component.Rotate.x + animation_Num.motion.GetFlip(AnimationTimeStart, duration);
            const Vector3<GLfloat> scaleMotion = animation_Num.motion.GetScale(AnimationTimeStart, duration);
            const Vector3<GLfloat> offetMotion = animation_Num.motion.GetOffset(AnimationTimeStart, duration);

            globalFlip[n] = globalFlip[parentNumber] + flipMotion;
            GLfloat parentFlipAngle = globalFlip[parentNumber];
            GLfloat flipAngle = globalFlip[n];

            objectTransform component_Num;

            component_Num.WorldPos.x = ParentSprite.WorldPos.x + offetMotion.x;
            component_Num.WorldPos.y = ParentSprite.WorldPos.y + offetMotion.y;
            component_Num.WorldPos.z = ParentSprite.WorldPos.z + component.WorldPos.z;

            component_Num.Rotate.y = component.Rotate.y + ParentSprite.Rotate.y;
            component_Num.Rotate.z = component.Rotate.z + ParentSprite.Rotate.z + flipMotion;

            component_Num.Scale.x = component.Scale.x * animation_Num.spriteScale.x * globalScaleX * scaleMotion.x;
            component_Num.Scale.y = component.Scale.y * animation_Num.spriteScale.y * globalScaleY * scaleMotion.y;

            components[n].transform = component_Num;

            Vector3<GLfloat> direction = Vector3<GLfloat>(cos(ToRadian(parentFlipAngle)), sin(ToRadian(parentFlipAngle)), 0.0f); // размещение в координатах родительского спрайта
            components[n].transform.Move(component.WorldPos.x * ParentSprite.Scale.x, direction);
            parentFlipAngle += 90;
            direction = Vector3<GLfloat>(cos(ToRadian(parentFlipAngle)), sin(ToRadian(parentFlipAngle)), 0.0f);
            components[n].transform.Move(component.WorldPos.y * ParentSprite.Scale.y, direction);

            #if MY_ACTOR_TEST
            render->PushGeometry(mySphere.GetGeometry());
            sphereTransform.WorldPos = components[n].transform.WorldPos;
            spherePos[n] = sphereTransform.WorldPos;
            if (render != nullptr) render->drawObject(&sphereTransform, &mySphere);
            render->PushGeometry(myLine.GetGeometry());
            myLine.setPoints(spherePos[n], spherePos[parentNumber]);
            if (render != nullptr) render->drawObject(&myLine.trans, &myLine);
            spherePos[0] = spherePos[1];
            render->PushGeometry(Sprite().GetGeometry());
            #endif

            direction = Vector3<GLfloat>(cos(ToRadian(flipAngle)), sin(ToRadian(flipAngle)), 0.0f); // трансформация относительно anchor
            components[n].transform.Move(-animation_Num.anchorPoint.x * globalScaleX, direction);
            flipAngle += 90;
            direction = Vector3<GLfloat>(cos(ToRadian(flipAngle)), sin(ToRadian(flipAngle)), 0.0f);
            components[n].transform.Move(-animation_Num.anchorPoint.y * globalScaleY, direction);

            ActorComponents.push_back(&components[n]);
            std::vector<Component*> componentsToAdd = getActorComponents(it, n, duration);
            ActorComponents.insert(ActorComponents.end(), componentsToAdd.begin(), componentsToAdd.end());
        }

        return ActorComponents;
    }

    std::vector<Component*> getActorComponents()
    {
        if (components == nullptr) {
            std::vector<Component*> ActorComponents;
            return ActorComponents;
        }
        
        size_t n = 0;
        animations[0]->transform = components[0].transform = trans; // Updating the skelet position
        GLfloat duration = Animation::GetDuration(*GetName(), animation);

        return getActorComponents(GetSkelet(), n, duration);
    }


    void updateAnimationRecursive(Bone *_parent, const std::string &animationName, size_t &n)
    {
        n++;
        for (auto it : _parent->children) {
            if (it->Animations.find(animationName) != it->Animations.end()) {
                animations[n] = &it->Animations[animationName];
                components[n].sprite = animations[n]->sprite; 
            } else {
                std::cout << "Error: " << __FUNCTION__ << " " << animationName << " sprite not found" << std::endl;
            }

            updateAnimationRecursive(it, animationName, n);
        }
    }

    void updateAnimation(const std::string &animationName)
    {
        if (animation == animationName) return;
        if (GetSkelet()->Animations.find(animationName) != GetSkelet()->Animations.end()) {
            animation = animationName;
            animations[0] = &GetSkelet()->Animations[animationName];
        }

        size_t n = 0;
        updateAnimationRecursive(GetSkelet(), animationName, n);
    }


    static void parseNodeAnimation(pugi::xml_node &nodeAnimation, const std::string &animationName, Animation &newAnimation, std::map<std::string, Sprite> &Sprites) {
        std::string spriteName = nodeAnimation.attribute("sprite").as_string();
        auto sprite = Sprites.find(spriteName); 
        if (sprite != Sprites.end()) {
            newAnimation.sprite = &(sprite->second); 
            newAnimation.spriteScale = sprite->second.Scale; 
        } else {
            std::cout << "Error Actor.parseAnimation: not found " << spriteName << std::endl;
        }

        objectTransform _transform;
        Vector3<GLfloat> v;

        v.x = std::stof(nodeAnimation.attribute("x").value());
        v.y = std::stof(nodeAnimation.attribute("y").value());
        v.z = std::stof(nodeAnimation.attribute("z").value()) / 10;
        _transform.WorldPos = v;

        v.x = std::stof(nodeAnimation.attribute("width").value());
        v.y = std::stof(nodeAnimation.attribute("height").value());
        v.z = 0.0;
        _transform.Scale = v; 

        v.x = std::stof(nodeAnimation.attribute("flip").value());
        v.y = 180.0 * (nodeAnimation.attribute("mirrorX") != 0);
        v.z = std::stof(nodeAnimation.attribute("rotate").value());
        _transform.Rotate = v;

        newAnimation.transform = _transform;

        v.x = std::stof(nodeAnimation.attribute("tangent").value());
        v.y = std::stof(nodeAnimation.attribute("radius").value());
        v.z = 0.0;
        newAnimation.anchorPoint = v;
    }

    static void parseNodeMotion(pugi::xml_node &nodeMotion, const std::string &animationName, Animation &newAnimation, const std::string &ActorName) {
        constexpr std::size_t addHash      = myHash("add");
        constexpr std::size_t multiplyHash = myHash("multiply");
        constexpr std::size_t sinHash      = myHash("sin");
        constexpr std::size_t cosHash      = myHash("cos");
        constexpr std::size_t timeHash     = myHash("time");

        for (pugi::xml_node childMotion : nodeMotion.children()) {
            float end = childMotion.attribute("end") ? std::stof(childMotion.attribute("end").value()) : Animation::GetDuration(ActorName, animationName);

            for (pugi::xml_node childFrame : childMotion.children()) {
                if (std::string(childFrame.name()) == "Flip") {
                    std::vector<Motion::rule> rules;
                    for (pugi::xml_node childRules : childFrame.children()) {
                        std::string arg = childRules.attribute("arg") ? childRules.attribute("arg").as_string() : "";
                        float factor = childRules.attribute("factor") ? std::stof(childRules.attribute("factor").value()) : 1.0;

                        if (!arg.empty()) {
                            if (arg == "time") {
                                rules.push_back({ 1.0, Motion::FUNTIONS::TIME });
                            } else {
                                float add = std::stof(childRules.attribute("arg").value());
                                rules.push_back({ add, Motion::FUNTIONS::ADD });
                            }
                        }

                        switch (myHash(childRules.name()))
                        {
                        case addHash:
                            break;

                        case multiplyHash:
                            rules.push_back({ factor, Motion::FUNTIONS::MULTIPLY });
                            break;

                        case sinHash:
                            rules.push_back({ factor, Motion::FUNTIONS::SIN });
                            break;

                        case cosHash:
                            rules.push_back({ factor, Motion::FUNTIONS::COS });
                            break;

                        case timeHash:
                            rules.push_back({ factor, Motion::FUNTIONS::TIME });
                            break;
                        
                        default:
                            break;
                        }
                    }

                    std::pair<float, std::vector<Motion::rule>> rule(end, rules);
                    newAnimation.motion.ruleFlip.push_back(rule);
                }
            
                if (std::string(childFrame.name()) == "Scale") {
                    std::vector<Motion::rule> rules;
                    std::string args[2];
                    float factor[2];
                    for (pugi::xml_node childRules : childFrame.children()) {
                        args[X] = childRules.attribute("width") ? childRules.attribute("width").as_string() : "";
                        args[Y] = childRules.attribute("height") ? childRules.attribute("height").as_string() : "";
                        factor[X] = childRules.attribute("width_factor") ? std::stof(childRules.attribute("width_factor").value()) : 1.0;
                        factor[Y] = childRules.attribute("height_factor") ? std::stof(childRules.attribute("height_factor").value()) : 1.0;

                        if (args[X].empty() ^ args[Y].empty()) continue;

                        if ((!args[X].empty()) && (!args[Y].empty())) {
                            if (args[X] == "time") {
                                rules.push_back({ 1.0, Motion::FUNTIONS::TIME });
                            } else {
                                float add = std::stof(childRules.attribute("width").value());
                                rules.push_back({ add, Motion::FUNTIONS::ADD });
                            }

                            if (args[Y] == "time") {
                                rules.push_back({ 1.0, Motion::FUNTIONS::TIME });
                            } else {
                                float add = std::stof(childRules.attribute("height").value());
                                rules.push_back({ add, Motion::FUNTIONS::ADD });
                            }
                        }


                        for (size_t i = 0; i < 2; i++) {
                            std::string ruleName;
                            if (childRules.name() == "pair") {
                                if (i == 0) {
                                    ruleName = childRules.attribute("xfun").as_string();
                                } else {
                                    ruleName = childRules.attribute("yfun").as_string();
                                }
                            } else {
                                ruleName = childRules.name();
                            }

                            switch (myHash(ruleName.c_str()))
                            {
                            case addHash:
                                break;

                            case multiplyHash:
                                rules.push_back({ factor[i], Motion::FUNTIONS::MULTIPLY });
                                break;

                            case sinHash:
                                rules.push_back({ factor[i], Motion::FUNTIONS::SIN });
                                break;

                            case cosHash:
                                rules.push_back({ factor[i], Motion::FUNTIONS::COS });
                                break;

                            case timeHash:
                                rules.push_back({ factor[i], Motion::FUNTIONS::TIME });
                                break;
                            
                            default:
                                break;
                            }
                        }
                    }

                    std::cout << rules.size() << std::endl;
                    std::pair<float, std::vector<Motion::rule>> rule(end, rules);
                    newAnimation.motion.ruleScale.push_back(rule);
                }
            
                if (std::string(childFrame.name()) == "Move") {
                    std::vector<Motion::rule> rules;
                    std::string args[2];
                    float factor[2];
                    for (pugi::xml_node childRules : childFrame.children()) {
                        args[X] = childRules.attribute("x") ? childRules.attribute("x").as_string() : "";
                        args[Y] = childRules.attribute("y") ? childRules.attribute("y").as_string() : "";
                        if (childRules.attribute("factor")) {
                            factor[X] = factor[Y] = std::stof(childRules.attribute("factor").value());
                        } else {
                            factor[X] = childRules.attribute("x_factor") ? std::stof(childRules.attribute("x_factor").value()) : 1.0;
                            factor[Y] = childRules.attribute("y_factor") ? std::stof(childRules.attribute("y_factor").value()) : 1.0;
                        }

                        if (args[X].empty() ^ args[Y].empty()) continue;

                        if ((!args[X].empty()) && (!args[Y].empty())) {
                            if (args[X] == "time") {
                                rules.push_back({ 1.0, Motion::FUNTIONS::TIME });
                            } else {
                                float add = std::stof(childRules.attribute("x").value());
                                rules.push_back({ add, Motion::FUNTIONS::ADD });
                            }

                            if (args[Y] == "time") {
                                rules.push_back({ 1.0, Motion::FUNTIONS::TIME });
                            } else {
                                float add = std::stof(childRules.attribute("y").value());
                                rules.push_back({ add, Motion::FUNTIONS::ADD });
                            }
                        }


                        for (size_t i = 0; i < 2; i++) {
                            std::string ruleName;
                            if (std::string(childRules.name()) == "pair") {
                                if (i == 0) {
                                    ruleName = childRules.attribute("xfun").as_string();
                                } else {
                                    ruleName = childRules.attribute("yfun").as_string();
                                }
                            } else {
                                ruleName = std::string(childRules.name());
                            }

                            switch (myHash(ruleName.c_str()))
                            {
                            case addHash:
                                break;

                            case multiplyHash:
                                rules.push_back({ factor[i], Motion::FUNTIONS::MULTIPLY });
                                break;

                            case sinHash:
                                rules.push_back({ factor[i], Motion::FUNTIONS::SIN });
                                break;

                            case cosHash:
                                rules.push_back({ factor[i], Motion::FUNTIONS::COS });
                                break;

                            case timeHash:
                                rules.push_back({ factor[i], Motion::FUNTIONS::TIME });
                                break;
                            
                            default:
                                break;
                            }
                        }
                    }

                    std::cout << rules.size() << std::endl;
                    std::pair<float, std::vector<Motion::rule>> rule(end, rules);
                    newAnimation.motion.ruleOffset.push_back(rule);
                }
            }
        }
        nodeMotion = nodeMotion.next_sibling();
    }

    template<typename Derived>
    static void parseAnimation(pugi::xml_node &nodeAnimation, pugi::xml_node &nodeMotion, Bone *_bone, const std::string &animationName) {
        for (int i = 0; i < _bone->children.size(); i++) {
            Animation newAnimation;
            std::string boneName = _bone->children[i]->name;
            pugi::xml_node node = nodeAnimation.child(boneName.c_str());
                
            if (nodeMotion && (nodeMotion.name() == boneName))
                parseNodeMotion(nodeMotion, animationName, newAnimation, Derived::name);

            parseNodeAnimation(node, animationName, newAnimation, Derived::Sprites);
            _bone->children[i]->Animations.insert({animationName, newAnimation});

            parseAnimation<Derived>(node, nodeMotion, _bone->children[i], animationName);
        }
    }

    template<typename Derived>
    static bool loadAnimation(const std::string &_path, const std::string &_name)
    {
        std::string animationPath = std::string("assets/entities/") + _path + std::string("/models/animations/") + _name + std::string(".xml");
        std::string motionPath = std::string("assets/entities/") + _path + std::string("/models/motions/") + _name + std::string(".xml");
        pugi::xml_document docAnimation;
        pugi::xml_document docMotion;
        pugi::xml_node nodeAnimation;
        pugi::xml_node nodeMotion;
        pugi::xml_parse_result parseAnimationResult = docAnimation.load_file(animationPath.c_str());
        pugi::xml_parse_result parseMotionResult = docMotion.load_file(motionPath.c_str());

        if (!parseAnimationResult) {
            std::cout << "Error Actor.loadAnimation: file not found (" << animationPath << ")" << std::endl;
            return false;
        }

        nodeAnimation = docAnimation.child("animation");
        std::string animationName = nodeAnimation.attribute("name").as_string();
        
        objectTransform _transform;
        Vector3<GLfloat> v;

        v.x = std::stof(nodeAnimation.attribute("x").value());
        v.y = std::stof(nodeAnimation.attribute("y").value());
        v.z = std::stof(nodeAnimation.attribute("z").value());
        _transform.SetWorldPos(v.x, v.y, v.z);

        v.x = std::stof(nodeAnimation.attribute("width").value());
        v.y = std::stof(nodeAnimation.attribute("height").value());
        _transform.SetScale(v.x, v.y, 0.0);
        
        v.z = std::stof(nodeAnimation.attribute("flip").value());
        _transform.SetRotate(0.0, 0.0, v.z);

        if (nodeAnimation.attribute("mirrorX")) {
            _transform.SetRotate(0.0, 180.0, v.z);
        } 

        Animation newAnimation;
        newAnimation.sprite = nullptr;
        newAnimation.spriteScale = Vector3<GLfloat>(1.0, 1.0, 1.0);
        newAnimation.transform = objectTransform();
        Derived::skelet.Animations.insert({animationName, newAnimation});


        nodeMotion = docMotion.child("motion");
        if (parseMotionResult) {
            float duration = std::stof(nodeMotion.attribute("duration").value());
            Animation::PushDuration(Derived::name, animationName, duration);
            nodeMotion = nodeMotion.first_child();
        } else {
            Animation::PushDuration(Derived::name, animationName, 0.0);
        }

        parseAnimation<Derived>(nodeAnimation, nodeMotion, &Derived::skelet, animationName);

        return true;
    }

    template<typename Derived>
    static bool loadSprites(const std::string &path)
    {
        std::string full_path = "entities/" + path + "/models/sprites/";

        for (const auto &entry : fs::directory_iterator("assets/" + full_path)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                if ((filename.size() > 4) && (filename.substr(filename.size() - 4) == ".jpg" || filename.substr(filename.size() - 4) == ".png")) {
                    std::string spriteName = filename.substr(0, filename.size() - 4);
                    Sprite sprite(spriteName, "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl", (full_path + filename).c_str());
                    Derived::Sprites.insert({spriteName, sprite});
                }
            }
        }

        return true;
    }

    template<typename Derived>
    static bool loadSkelet(const std::string &path)
    {
        Derived::skeletSize = Derived::skelet.createSkelet(path, "skelet");

        return true;
    }

    template<typename Derived>
    static void Initialize(std::string path, std::string _name, std::vector<std::string> _animations)
    {
        Derived::name = _name;
        loadSkelet<Derived>(path);
        loadSprites<Derived>(path);
        for (const auto &it : _animations)
            loadAnimation<Derived>(path, it);
    }

    bool loadActor(const std::string &path)
    {
        std::string full_path = std::string("assets/entities/") + path + "/actor.xml";
        pugi::xml_document doc;
        pugi::xml_node word;
        pugi::xml_parse_result parse_result = doc.load_file(full_path.c_str());

        if (!parse_result) {
            std::cout << "Error " << GetName() << ".loadActor: file not found (" << full_path << ")" << std::endl;
            return false;
        }

        // *GetName() = doc.child("character").attribute("name").value();

        Vector3<GLfloat> v;

        v.x = std::stof(doc.child("character").child("objectTransform").child("worldPos").attribute("x").value());
        v.y = std::stof(doc.child("character").child("objectTransform").child("worldPos").attribute("y").value());
        v.z = std::stof(doc.child("character").child("objectTransform").child("worldPos").attribute("z").value());
        trans.SetWorldPos(v.x, v.y, v.z);

        v.x = std::stof(doc.child("character").child("objectTransform").child("rotate").attribute("deg").value());
        trans.SetRotate(0.0, 0.0, v.x);

        v.x = std::stof(doc.child("character").child("objectTransform").child("scale").attribute("x").value());
        v.y = std::stof(doc.child("character").child("objectTransform").child("scale").attribute("y").value());
        trans.SetScale(v.x, v.y, 0.0);

        return true;
    }


    objectTransform *GetTransform()
    {
        return &trans;
    }

    Vector3<GLfloat> GetDirection()
    {
        return direction;
    }

    void SetDirection(const Vector3<GLfloat> &_direction)
    {
        direction = _direction;
    }

    std::string GetAnimationByAction()
    {
        switch (state) {
        case 0:
            return std::string("stand");
        default:
            return std::string("stand");
        }
    }

    std::string GetAnimation(const float CurrentTime)
    {
        switch (state) {
        case STATE::STAND:
            if (direction.Length() != 0) {
                SetState(STATE::GO, CurrentTime);
            }
            return std::string("stand");

        case STATE::GO: 
            if (direction.Length() == 0) {
                SetState(STATE::STAND, CurrentTime);
            }
            return std::string("stand_2");

        case STATE::ACTION:
            return GetAnimationByAction();

        case STATE::GET_HIT:
            return std::string("stand");

        default:
            return std::string("stand");
        }
    }

    void SetState(int _state, const float CurrentTime)
    {
        state = _state;
        AnimationTimeStart = CurrentTime;
    }

    static void PushTime(const float _time)
    {
        Motion::PushTime(_time);
    }


    #if MY_ACTOR_TEST
    void PushRender(Render *_render)
    {
        render = _render;
    }

    Render *render = nullptr;
    Vector3<GLfloat> *spherePos = nullptr;
    objectTransform sphereTransform;
    sphere mySphere;
    line myLine;
    #endif


protected:
    int state = STATE::STAND;
    float AnimationTimeStart = 0.0;
    GLfloat *globalFlip = nullptr;
    Component *components = nullptr;
    Animation **animations = nullptr;
    Vector3<GLfloat> direction;
    objectTransform trans;
    std::string animation;
    
    inline static size_t skeletSize = 0;
    inline static Bone skelet; // можно сделать статический массив костей и новую переменную хранящую структуру
    static inline std::string name = "NoName";
    inline static std::map<std::string, Sprite> Sprites;
};
