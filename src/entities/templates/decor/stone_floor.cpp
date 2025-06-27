#include <entities/templates/decor/stone_floor.hpp>
#include <managers/render_manager.hpp> 
#include <object/component/template/component_physics.hpp>

StoneFloor::StoneFloor()
{
    std::cout << name << std::endl;

    btScalar mass = 0.0f;
    glm::vec3 scale(10, 10, 0.001f);
    btCollisionShape* colliderShape = new btBoxShape(btVector3(scale.x / 2, scale.y / 2, scale.z / 2));
    RigidTransform * rigidBody = new RigidTransform(colliderShape, mass, scale);
    ComponentPhysics *body = CreateComponent<ComponentPhysics>(rigidBody);
    body->SetRotation({-90, 0, 0});

    Transform *transform = new Transform();
    ComponentShape *shape = CreateComponent<ComponentShape>(transform);
    body->AddChild(shape);
    shape->shape = RenderManager::primitives.sprite;
    shape->SetScale(scale);
    shape->SetRotation({-90, 0, 0});
    shape->material = Material::Find("stone_floor");

    rootComponent = body;
}

StoneFloor::~StoneFloor() {}

void StoneFloor::Initialize()
{
    StoneFloor::name = "StoneFloor";
    
    auto shader = Shader::Create("sprite", "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl");
    auto texture = Texture::Create("stone_floor", "img/floor.jpg");
    
    auto init_sprite = new Material::InitFunction([](Material& m) {
        std::vector<std::string> str = {"gWorld", "textureSampler"};
        for (auto s : str) {
            auto it = m.values.find(s);
            if (it != m.values.end()) {
                delete static_cast<glm::mat4*>(it->second.second);
                m.values.erase(it);
            }
        }
        m.values["gWorld"] = {glGetUniformLocation(m.GetShader()->GetID(), "gWorld"), new glm::mat4};
        m.values["textureSampler"] = {glGetUniformLocation(m.GetShader()->GetID(), "textureSampler"), nullptr};
    });
    auto apply_sprite = new Material::ApplyFunction([](const Material& m) {
        glActiveTexture(GL_TEXTURE0);

        GLint texLoc = m.values.at("textureSampler").first;
        glUniform1i(texLoc, 0);
        GLint matLoc = m.values.at("gWorld").first;
        glm::mat4* matPtr = static_cast<glm::mat4*>(m.values.at("gWorld").second);
        glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(*matPtr));

    });

    auto material = Material::Create("stone_floor", shader, init_sprite, apply_sprite);
    material->PushTexture(texture);
}

std::string StoneFloor::GetName() const {
    return StoneFloor::name;
}
