#include <entities/templates/decor/tree.hpp>
#include <managers/render_manager.hpp> 
#include <object/component/template/component_physics.hpp>
#include <object/component/template/point.hpp>

Tree::Tree()
{
    std::cout << name << std::endl;
    ComponentShape *root = CreateComponent<ComponentShape>(new Transform());
    root->shape = RenderManager::primitives.cube;
    root->SetPosition(glm::vec3(0.0, 0.0, 0));
    root->SetRotation(glm::vec3(0.0, 0, 0));
    root->SetScale(glm::vec3(0.2, 1.0, 0.2));
    root->material = Material::Find("Tree");

//
    ComponentPoint *point_1_1 = CreateComponent<ComponentPoint>(new Transform());
    root->AddChild(point_1_1);
    point_1_1->SetPosition(glm::vec3(0.0, 1.0, 0));
    point_1_1->SetScale(glm::vec3(1));

    ComponentShape *branch_1_1 = CreateComponent<ComponentShape>(new Transform());
    point_1_1->AddChild(branch_1_1);
    branch_1_1->shape = RenderManager::primitives.cube;
    branch_1_1->SetPosition(glm::vec3(0.0, 1.5, 0));
    branch_1_1->SetRotation(glm::vec3(0.0, 0, 0));
    branch_1_1->SetScale(glm::vec3(0.1, 0.5, 0.1));
    branch_1_1->material = Material::Find("Tree");   
    
    point_1_1->SetRotation(glm::vec3(10.0, 0, 0));

    // Transform *transform1 = new Transform();
    // ComponentShape *shape1 = CreateComponent<ComponentShape>(transform1);
    // shape1->shape = RenderManager::primitives.cube;
    // shape1->SetPosition(glm::vec3(0.0, -0.5, 0));
    // shape1->SetRotation(glm::vec3(0.0, 45, 0));
    // shape1->SetScale(glm::vec3(0.4, 0.2, 0.4));
    // shape1->material = Material::Find("WoodenBox");

    // Transform *transform2 = new Transform();
    // ComponentShape *shape2 = CreateComponent<ComponentShape>(transform2);
    // shape2->shape = RenderManager::primitives.cube;
    // shape1->AddChild(shape2);
    // shape2->SetPosition(glm::vec3(0.0, 1.0, 0.0));
    // shape2->SetRotation(glm::vec3(0.0, 0, 0));
    // // shape2->SetScale(glm::vec3(0.5));
    // // shape2->SetScale(glm::vec3(1.0, 1, 1));
    // shape2->SetScale(glm::vec3(1));
    // shape2->material = Material::Find("WoodenBox");

    // Transform *transform3 = new Transform();
    // ComponentShape *shape3 = CreateComponent<ComponentShape>(transform3);
    // shape3->shape = RenderManager::primitives.cube;
    // shape2->AddChild(shape3);
    // shape3->SetPosition(glm::vec3(0.0, 3, 0.0));
    // // shape3->SetRotation(glm::vec3(0, 45, 0));
    // shape3->SetScale(glm::vec3(0.1, 0.5, 0.1));
    // shape3->material = Material::Find("WoodenBox");

    // // shape2->SetScale(glm::vec3(1, 0.5, 1));
    // // shape2->SetPosition(glm::vec3(1, 1.5, 1));
    // shape2->SetRotation(glm::vec3(30.0, 0, 0));

    // rootComponent = shape1;

    // ComponentShape *shape2 = CreateComponent<ComponentShape>(new Transform());
    // shape2->shape = RenderManager::primitives.sphere;
    // shape1->AddChild(shape2);
    // shape2->SetPosition(glm::vec3(0.0, 1.0, 0.0));
    // shape2->SetRotation(glm::vec3(0.0, 0, 0));
    // // shape2->SetScale(glm::vec3(0.5));
    // // shape2->SetScale(glm::vec3(1.0, 1, 1));
    // shape2->SetScale(glm::vec3(1));
    // shape2->material = Material::Find("Leaf");

    // Transform *transform3 = new Transform();
    // ComponentShape *shape3 = CreateComponent<ComponentShape>(transform3);
    // shape3->shape = RenderManager::primitives.cube;
    // shape2->AddChild(shape3);
    // shape3->SetPosition(glm::vec3(0.0, 3, 0.0));
    // // shape3->SetRotation(glm::vec3(0, 45, 0));
    // shape3->SetScale(glm::vec3(2.0, 1, 1.0));
    // shape3->material = Material::Find("WoodenBox");

    rootComponent = root;
}

Tree::~Tree() {}

void Tree::Initialize()
{
    Tree::name = "Tree";
    auto shader_tree = Shader::Create("cube", "shaders/cube_fs.glsl", "shaders/cube_vs.glsl");
    auto shader_leaf = Shader::Create("sprite", "shaders/sprite_fs.glsl", "shaders/sprite_vs.glsl");
    auto texture_tree = Texture::Create("tree", "img/tree.jpg");
    auto texture_leafs = Texture::Create("leafs", "img/leaf.jpg");
    
    auto init_tree = new Material::InitFunction([](Material& m) {
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
    auto apply_tree = new Material::ApplyFunction([](const Material& m) {
        glActiveTexture(GL_TEXTURE0);

        GLint texLoc = m.values.at("textureSampler").first;
        glUniform1i(texLoc, 0);
        GLint matLoc = m.values.at("gWorld").first;
        glm::mat4* matPtr = static_cast<glm::mat4*>(m.values.at("gWorld").second);
        glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(*matPtr));
    });

    auto init_leaf = init_tree;
    auto apply_leaf = apply_tree;

    auto material_tree = Material::Create("Tree", shader_tree, init_tree, apply_tree);
    auto material_leaf = Material::Create("Leaf", shader_leaf, init_leaf, apply_leaf);
    material_tree->PushTexture(texture_tree);
    material_leaf->PushTexture(texture_leafs);
}

std::string Tree::GetName() const {
    return Tree::name;
}
