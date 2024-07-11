#include <entities/actor.hpp>

Actor::Actor(const std::string &path, const size_t SkeletSize)
    : animationInfo(SkeletSize)
{
    loadActor(path);
    transform.Rotate = Vector3<GLfloat>(0.0, 0.0, 180);

    #if MY_ACTOR_TEST
    spherePos = new Vector3<GLfloat>[SkeletSize]();

    mySphere = sphere<10>(std::string("mySphere"), "shaders/sphere_fs.glsl", "shaders/sphere_vs.glsl", nullptr);
    sphereTransform.Rotate = Vector3<GLfloat>(0.0, 0.0, 0.0);
    sphereTransform.Scale = Vector3<GLfloat>(0.1, 0.2, 0.1);
    
    myLine = line(std::string("myLine"), Vector3<GLfloat>(1.0, 0.0, 0.0));
    #endif
}

Actor::~Actor() {};

std::vector<Component*> Actor::getActorComponents(const Bone &_parent, size_t &n, const GLfloat duration)
{
    std::vector<Component*> ActorComponents;

    const size_t parentNumber = n++;
    const objectTransform ParentSprite = animationInfo.components[parentNumber].transform;
    const GLfloat globalScaleX = ParentSprite.Scale.x / animationInfo.animations[parentNumber].spriteScale.x;
    const GLfloat globalScaleY = ParentSprite.Scale.y / animationInfo.animations[parentNumber].spriteScale.y;

    for (const auto &it : _parent.children) {
        const Animation &animation_Num = animationInfo.animations[n]; // ссылка из-за векторов в Motion
        const objectTransform component = animation_Num.transform;

        const GLfloat offetMotion[2] = {animationInfo.transforms[n].offset[X], animationInfo.transforms[n].offset[Y]};
        const GLfloat flipMotion     = component.Rotate.x + animationInfo.transforms[n].flip;
        const GLfloat scaleMotion[2] = {1 + animationInfo.transforms[n].scale[X], 1 + animationInfo.transforms[n].scale[Y]};

        animationInfo.globalFlip[n] = animationInfo.globalFlip[parentNumber] + flipMotion;
        GLfloat parentFlipAngle = animationInfo.globalFlip[parentNumber];
        GLfloat flipAngle = animationInfo.globalFlip[n];

        objectTransform component_Num;

        component_Num.WorldPos.x = ParentSprite.WorldPos.x + offetMotion[0];
        component_Num.WorldPos.y = ParentSprite.WorldPos.y + offetMotion[1];
        component_Num.WorldPos.z = ParentSprite.WorldPos.z + component.WorldPos.z;

        component_Num.Rotate.x = ParentSprite.Rotate.x;
        component_Num.Rotate.y = component.Rotate.y + ParentSprite.Rotate.y;
        component_Num.Rotate.z = component.Rotate.z + ParentSprite.Rotate.z + flipMotion;

        component_Num.Scale.x = component.Scale.x * animation_Num.spriteScale.x * globalScaleX * scaleMotion[0];
        component_Num.Scale.y = component.Scale.y * animation_Num.spriteScale.y * globalScaleY * scaleMotion[1];

        animationInfo.components[n].transform = component_Num;

        Vector3<GLfloat> direction = Vector3<GLfloat>(cos(ToRadian(parentFlipAngle)), sin(ToRadian(parentFlipAngle)), 0.0f); // размещение в координатах родительского спрайта
        animationInfo.components[n].transform.Move(component.WorldPos.x * ParentSprite.Scale.x, direction);
        parentFlipAngle += 90;
        direction = Vector3<GLfloat>(cos(ToRadian(parentFlipAngle)), sin(ToRadian(parentFlipAngle)), 0.0f);
        animationInfo.components[n].transform.Move(component.WorldPos.y * ParentSprite.Scale.y, direction);

        #if MY_ACTOR_TEST
        render->PushGeometry(mySphere.GetGeometry());
        sphereTransform.WorldPos = animationInfo.components[n].transform.WorldPos;
        spherePos[n] = sphereTransform.WorldPos;
        if (render != nullptr) render->drawObject(render->pipeline.GetTransform(sphereTransform), &mySphere);
        render->PushGeometry(myLine.GetGeometry());
        myLine.setPoints(spherePos[n], spherePos[parentNumber]);
        if (render != nullptr) render->drawObject(render->pipeline.GetTransform(myLine.transform), &myLine);
        spherePos[0] = spherePos[1];
        render->PushGeometry(Sprite().GetGeometry());
        #endif

        direction = Vector3<GLfloat>(cos(ToRadian(flipAngle)), sin(ToRadian(flipAngle)), 0.0f); // трансформация относительно anchor
        animationInfo.components[n].transform.Move(-animation_Num.anchorPoint.x * globalScaleX, direction);
        flipAngle += 90;
        direction = Vector3<GLfloat>(cos(ToRadian(flipAngle)), sin(ToRadian(flipAngle)), 0.0f);
        animationInfo.components[n].transform.Move(-animation_Num.anchorPoint.y * globalScaleY, direction);

        ActorComponents.push_back(&animationInfo.components[n]);
        std::vector<Component*> componentsToAdd = getActorComponents(it, n, duration);
        ActorComponents.insert(ActorComponents.end(), componentsToAdd.begin(), componentsToAdd.end());
    }

    return ActorComponents;
}

std::vector<Component*> Actor::getActorComponents()
{
    if (animationInfo.components.size() == 0) {
        std::vector<Component*> ActorComponents;
        return ActorComponents;
    }
    
    size_t n = 0;
    animationInfo.animations[0].transform = animationInfo.components[0].transform = transform; // Updating the skelet position
    GLfloat duration = Animation::GetDuration(*GetName(), &animationInfo.animation[0]);
    motionPtr->PushTime( std::fmod((GetTime() - animationInfo.AnimationTimeStart) / 1e9, *motionPtr->FindUniformFloat("duration")) );
    motionPtr->PushTransform(&animationInfo.transforms[0]);
    (*motionFunPtr)();

    return getActorComponents(*GetSkelet(), n, duration);
}


void Actor::updateAnimationRecursive(Bone &_parent, const std::string &animationName, size_t &n)
{
    n++;
    for (auto& it : _parent.children) {
        if (it.Animations.find(animationName) != it.Animations.end()) {
            animationInfo.animations[n] = it.Animations[animationName];
            animationInfo.components[n].sprite = animationInfo.animations[n].sprite; 
        } else {
            std::cout << "Error: " << __FUNCTION__ << " " << animationName << " sprite not found" << std::endl;
        }

        updateAnimationRecursive(it, animationName, n);
    }
}

void Actor::updateAnimation(const std::string &animationName)
{
    if (animationInfo.animation == animationName) return;
    if (GetSkelet()->Animations.find(animationName) != GetSkelet()->Animations.end()) {
        std::pair<float, Motion::FunType> *motionPairPtr = &motionPtr->function[animationName];
        motionPtr->PushDuration(motionPairPtr->first);
        motionFunPtr = &motionPairPtr->second;
        animationInfo.animation = animationName;
        animationInfo.animations[0] = GetSkelet()->Animations[animationName];
    }

    size_t n = 0;
    updateAnimationRecursive(*GetSkelet(), animationName, n);
}


void Actor::parseNodeAnimation(pugi::xml_node &nodeAnimation, const std::string &animationName, Animation &newAnimation, std::map<std::string, Sprite> &Sprites) {
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

bool Actor::loadActor(const std::string &path)
{
    std::string full_path = std::string("assets/entities/") + path + "/actor.xml";
    pugi::xml_document doc;
    pugi::xml_node word;
    pugi::xml_parse_result parse_result = doc.load_file(full_path.c_str());

    if (!parse_result) {
        std::cout << "Error " << GetName() << ".loadActor: file not found (" << full_path << ")" << std::endl;
        return false;
    }

    Vector3<GLfloat> v;

    v.x = std::stof(doc.child("character").child("objectTransform").child("worldPos").attribute("x").value());
    v.y = std::stof(doc.child("character").child("objectTransform").child("worldPos").attribute("y").value());
    v.z = std::stof(doc.child("character").child("objectTransform").child("worldPos").attribute("z").value());
    transform.SetWorldPos(v.x, v.y, v.z);

    v.x = std::stof(doc.child("character").child("objectTransform").child("rotate").attribute("deg").value());
    transform.SetRotate(0.0, 0.0, v.x);

    v.x = std::stof(doc.child("character").child("objectTransform").child("scale").attribute("x").value());
    v.y = std::stof(doc.child("character").child("objectTransform").child("scale").attribute("y").value());
    transform.SetScale(v.x, v.y, 0.0);

    return true;
}


objectTransform *Actor::GetTransform()
{
    return &transform;
}

Vector3<GLfloat> Actor::GetDirection() const
{
    return direction;
}

void Actor::SetDirection(const Vector3<GLfloat> &_direction)
{
    direction = _direction;
}

//Vector3<GLfloat> Actor::GetTarget() const
//{
//    return target;
//}
//
//void Actor::SetTarget(const Vector3<GLfloat> &_target)
//{
//    target = _target;
//}

std::string Actor::GetAnimationByAction()
{
    switch (state) {
    case 0:
        return std::string("stand");
    default:
        return std::string("stand");
    }
}

std::string Actor::GetAnimation(const float CurrentTime)
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
        // return std::string("stand_2");
        return std::string("stand");

    case STATE::ACTION:
        return GetAnimationByAction();

    case STATE::GET_HIT:
        return std::string("stand");

    default:
        return std::string("stand");
    }
}

void Actor::SetState(int _state, const float CurrentTime)
{
    if (state != _state) {
        animationInfo.AnimationTimeStart = CurrentTime;
        state = _state;
    }
}

void Actor::PushTime(const float _time)
{
    time = _time;
}

float Actor::GetTime() const
{
    return time;
}
