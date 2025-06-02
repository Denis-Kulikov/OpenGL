#include <entities/templates/decor/brick_sphere.hpp>
#include <managers/render_manager.hpp> 

BrickSphere::BrickSphere()
{
    std::cout << name << std::endl;
    Transform *transform1 = new Transform();
    ComponentShape *shape1 = CreateComponent<ComponentShape>(transform1);
    shape1->shape = sphere;
    shape1->material = Material::Find("BrickSphere");

    rootComponent = shape1;
}

BrickSphere::~BrickSphere() {}

void BrickSphere::Initialize()
{
    BrickSphere::name = "BrickSphere";
    sphere = RenderManager::primitives.sphere;
    auto shader = Shader::Create("sprite", "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl");
    // auto texture_wooden_box = Texture::Create("brick_wall", "img/white.png");
    auto texture_wooden_box = Texture::Create("brick_wall", "img/brick_wall_10_diff_1k.jpg");
    
    cube = RenderManager::primitives.cube;
    auto shader_cube = Shader::Create("cube", "shaders/cube_fs.glsl", "shaders/cube_vs.glsl");


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
        glUseProgram(m.GetShader()->GetID());
        glActiveTexture(GL_TEXTURE0);

        GLint texLoc = m.values.at("textureSampler").first;
        glUniform1i(texLoc, 0);
        GLint matLoc = m.values.at("gWorld").first;
        glm::mat4* matPtr = static_cast<glm::mat4*>(m.values.at("gWorld").second);
        glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(*matPtr));

    });

    auto material_wooden_box = Material::Create("BrickSphere", shader, init_sprite, apply_sprite);
    material_wooden_box->PushTexture(texture_wooden_box);
}

std::string BrickSphere::GetName() const {
    return BrickSphere::name;
}
