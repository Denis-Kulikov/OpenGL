#include <entities/templates/mobs/Female.hpp>
#include <object/component/template/shape.hpp>
#include <object/component/template/mesh.hpp>
#include <object/transform/transform.hpp>


Female::Female()
{
    Transform *transform = new Transform();
    ComponentShape *shape = CreateComponent<ComponentShape>(transform);
    shape->shape = mesh;
    rootComponent = shape;
    rootComponent->material = Material::Find("female");

    // ComponentCollaider *collider = CreateComponent<ComponentCollaider>();
    /*
    glm::vec3 scale(1);
    btSphereShape *colliderShape = new btSphereShape(scale.x);
    RigidTransform *rigidShape = new RigidTransform(btManager, colliderShape, 0.5);

    Transform *shapeTransform = new Transform();
    rigidShape->SetScale(scale);
    
    // glm::quat rotationt = glm::angleAxis(glm::radians(30.0f), glm::vec3(1, 0, 0));
    // rigidShape->SetScale(scale);
    // rigidShape->SetRotation(rotationt);
    // rigidShape->SetScale(scale);

    ComponentPhysic *shape = CreateComponent<ComponentPhysic>(rigidShape);
    // shape->shape = new Sphere<20>("img/brick_wall_10_diff_1k.jpg");

    rootComponent = shape;


    Transform *cmeshTransform = new Transform();
    Transform *meshTransform = new Transform();

    // float scaleMesh = 5.0f / scale.x;
    float scaleMesh = 3.0f;
    glm::quat rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1, 0, 0));
    meshTransform->SetPosition(glm::vec3(0.0, -1.0, 0.0));
    meshTransform->SetRotation(rotation);
    meshTransform->SetScale(glm::vec3(scaleMesh));

    ComponentMesh *cmesh = CreateComponent<ComponentMesh>(meshTransform);
    cmesh->mesh = Female::mesh;

    rootComponent->AddChild(cmesh);
    */
}

Female::~Female()
{}

void Female::Initialize()
{
    Female::name = "Female";
    // std::string path("assets/model/female/female.glb");
    std::string path("assets/model/chick/source/CHICKEN.glb");
    mesh = new GeometryMesh(path);

    auto shader_cube = Shader::Create("sprite", "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl");
    // auto texture_wooden_box = Texture::Create("wooden_box", "img/box.jpg");
    
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

    auto material_wooden_box = Material::Create("female", shader_cube, init_sprite, apply_sprite);
    material_wooden_box->SetTexture(mesh->m_Textures);
    std::cout << mesh->m_Textures.size() << std::endl;
}

std::string Female::GetName() const {
    return Female::name;
}
