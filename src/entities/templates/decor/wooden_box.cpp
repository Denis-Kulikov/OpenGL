#include <entities/templates/decor/wooden_box.hpp>
#include <managers/render_manager.hpp> 
#include <object/component/template/component_physics.hpp>

WoodenBox::WoodenBox()
{
    std::cout << name << std::endl;

    glm::vec3 scale(1.f);
    btScalar mass = 200.0f;
    btCollisionShape* colliderShape = new btBoxShape(btVector3(
        scale.x / 2, 
        scale.y / 2, 
        scale.z / 2
    ));
    
    RigidTransform* rigidBody = new RigidTransform(
        colliderShape, 
        mass, 
        scale
    );

    ComponentPhysics *body = CreateComponent<ComponentPhysics>(rigidBody);

    ComponentShape *shape1 = CreateComponent<ComponentShape>(new Transform());
    body->AddChild(shape1);
    shape1->shape = RenderManager::primitives.cube;
    shape1->SetScale(scale);
    shape1->material = Material::Find("WoodenBox");

    rootComponent = body;
}

WoodenBox::~WoodenBox() {}

void WoodenBox::Initialize()
{
    WoodenBox::name = "WoodenBox";
    auto shader_cube = Shader::Create("cube", "shaders/cube_fs.glsl", "shaders/cube_vs.glsl");
    auto texture_wooden_box = Texture::Create("wooden_box", "img/box.jpg");
    
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

    auto material_wooden_box = Material::Create("WoodenBox", shader_cube, init_sprite, apply_sprite);
    material_wooden_box->PushTexture(texture_wooden_box);
}

std::string WoodenBox::GetName() const {
    return WoodenBox::name;
}
