#include <entities/templates/decor/tree.hpp>
#include <managers/render_manager.hpp> 
#include <object/component/template/component_physics.hpp>
#include <object/component/template/point.hpp>

Tree::Tree()
{
    std::cout << name << std::endl;

    // ComponentShape *root = CreateComponent<ComponentShape>(new Transform());
    // root->shape = RenderManager::primitives.cube;
    // root->SetScale(glm::vec3(0.3, .5, 0.4));
    // root->SetRotation(glm::vec3(0.0, 0, -90.0));
    // root->material = Material::Find("Tree");

    // ComponentShape *box = CreateComponent<ComponentShape>(new Transform());
    // root->AddChild(box);
    // box->shape = RenderManager::primitives.cube;
    // box->SetPosition(glm::vec3(0.0, .5, 0.0));
    // box->SetScale(glm::vec3(0.3, .5, 0.4));
    // box->SetRotation(glm::vec3(0.0, 15, 0.0));
    // box->material = Material::Find("Tree");

    ComponentShape *root = CreateComponent<ComponentShape>(new Transform());
    root->shape = RenderManager::primitives.cube;
    root->SetScale(glm::vec3(0.2, 2.0, 0.2));
    root->material = Material::Find("Tree");

// добавление веток 1
    ComponentPoint *point_1_1 = CreateComponent<ComponentPoint>(new Transform());
    root->AddChild(point_1_1);
    point_1_1->SetPosition(glm::vec3(0.0, 1.0, 0));


    ComponentShape *branch_1_1 = CreateComponent<ComponentShape>(new Transform());
    point_1_1->AddChild(branch_1_1);
    branch_1_1->shape = RenderManager::primitives.cube;
    branch_1_1->SetPosition(glm::vec3(0.0, 1.0, 0));
    branch_1_1->SetScale(glm::vec3(0.15, 2, 0.15));
    branch_1_1->material = Material::Find("Tree");   

// листья
    ComponentPoint *point_leaf_1 = CreateComponent<ComponentPoint>(new Transform());
    branch_1_1->AddChild(point_leaf_1);
    point_leaf_1->SetPosition(glm::vec3(0.0, 1.0, 0));

    ComponentShape *branch_3_2 = CreateComponent<ComponentShape>(new Transform());
    point_leaf_1->AddChild(branch_3_2);
    branch_3_2->shape = RenderManager::primitives.cube;
    branch_3_2->SetPosition(glm::vec3(0.0, 0.75 / 2, 0));
    branch_3_2->SetScale(glm::vec3(0.1, 0.75, 0.1));
    branch_3_2->material = Material::Find("Tree"); 

    ComponentShape *leaf1 = CreateComponent<ComponentShape>(new Transform());
    branch_3_2->AddChild(leaf1);
    leaf1->shape = RenderManager::primitives.sphere;
    leaf1->SetPosition(glm::vec3(0.0, 0.75 / 2 + 0.7 / 2 + 0., -0.0));
    leaf1->SetScale(glm::vec3(0.8, 0.7, 0.8));
    leaf1->material = Material::Find("Leaf"); 
    ComponentShape *leaf11 = CreateComponent<ComponentShape>(new Transform());
    branch_3_2->AddChild(leaf11);
    leaf11->shape = RenderManager::primitives.sphere;
    leaf11->SetPosition(glm::vec3(-0.1, 0.75 / 2 + 0.7 / 2, 0.2));
    leaf11->SetScale(glm::vec3(0.8, 0.7, 0.8));
    leaf11->material = Material::Find("Leaf");
    ComponentShape *leaf111 = CreateComponent<ComponentShape>(new Transform());
    branch_3_2->AddChild(leaf111);
    leaf111->shape = RenderManager::primitives.sphere;
    leaf111->SetPosition(glm::vec3(0.2, 0.75 / 2 + 0.7 / 2, 0.1));
    leaf111->SetScale(glm::vec3(0.9, 0.9, 0.9));
    leaf111->material = Material::Find("Leaf");

//     //branch_1_1->SetGlobalTransform(branch_1_1->globalTransform->GetMatrix());

// // 1
    ComponentPoint *point_1_2 = CreateComponent<ComponentPoint>(new Transform());
    branch_1_1->AddChild(point_1_2);
    point_1_2->SetPosition(glm::vec3(0.0, 1., 0));

    ComponentShape *branch_1_2 = CreateComponent<ComponentShape>(new Transform());
    point_1_2->AddChild(branch_1_2);
    branch_1_2->shape = RenderManager::primitives.cube;
    branch_1_2->SetPosition(glm::vec3(0.0, 1.0, 0));
    branch_1_2->SetScale(glm::vec3(0.1, 2., 0.1));
    branch_1_2->material = Material::Find("Tree");

// // 2
    ComponentPoint *point_2_1 = CreateComponent<ComponentPoint>(new Transform());
    branch_1_2->AddChild(point_2_1);
    point_2_1->SetPosition(glm::vec3(0.0, 1.0, 0));

    ComponentShape *branch_2_1 = CreateComponent<ComponentShape>(new Transform());
    point_2_1->AddChild(branch_2_1);
    branch_2_1->shape = RenderManager::primitives.cube;
    branch_2_1->SetPosition(glm::vec3(0.0, 0.5, 0));
    branch_2_1->SetScale(glm::vec3(0.1, 1, 0.1));
    branch_2_1->material = Material::Find("Tree");  


    ComponentPoint *point_2_2 = CreateComponent<ComponentPoint>(new Transform());
    branch_1_2->AddChild(point_2_2);
    point_2_2->SetPosition(glm::vec3(0.0, 1., 0));

    ComponentShape *branch_2_2 = CreateComponent<ComponentShape>(new Transform());
    point_2_2->AddChild(branch_2_2);
    branch_2_2->shape = RenderManager::primitives.cube;
    branch_2_2->SetPosition(glm::vec3(0.0, .5, 0));
    branch_2_2->SetScale(glm::vec3(0.1, 1., 0.1));
    branch_2_2->material = Material::Find("Tree"); 

    glm::vec3 basePos = glm::vec3(0.0f, .6f, 0.0f);
    ComponentShape* leaf2 = CreateComponent<ComponentShape>(new Transform());
    branch_2_2->AddChild(leaf2);
    leaf2->shape = RenderManager::primitives.sphere;
    leaf2->SetPosition(basePos);
    leaf2->SetScale(glm::vec3(0.9f, 0.7f, 1.1f));
    leaf2->material = Material::Find("Leaf");

    ComponentShape* leaf2a = CreateComponent<ComponentShape>(new Transform());
    branch_2_2->AddChild(leaf2a);
    leaf2a->shape = RenderManager::primitives.sphere;
    leaf2a->SetPosition(basePos + glm::vec3(0.3f, 0.2f, -0.2f));
    leaf2a->SetScale(glm::vec3(0.8f, 0.6f, 0.9f));
    leaf2a->material = Material::Find("Leaf");
    ComponentShape* leaf2b = CreateComponent<ComponentShape>(new Transform());
    branch_2_2->AddChild(leaf2b);
    leaf2b->shape = RenderManager::primitives.sphere;
    leaf2b->SetPosition(basePos + glm::vec3(-0.2f, 0.1f, 0.3f));
    leaf2b->SetScale(glm::vec3(0.85f, 0.7f, 0.85f));
    leaf2b->material = Material::Find("Leaf");

    

// // 3
    ComponentPoint *point_3_1 = CreateComponent<ComponentPoint>(new Transform());
    branch_2_1->AddChild(point_3_1);
    point_3_1->SetPosition(glm::vec3(0.0, .5, 0));

    ComponentShape *branch_3_1 = CreateComponent<ComponentShape>(new Transform());
    point_3_1->AddChild(branch_3_1);
    branch_3_1->shape = RenderManager::primitives.cube;
    branch_3_1->SetPosition(glm::vec3(0.0, .5, 0));
    branch_3_1->SetScale(glm::vec3(0.1, 1., 0.1));
    branch_3_1->material = Material::Find("Tree");  

// Дополнительные листья вокруг basePos3
    glm::vec3 basePos3 = glm::vec3(0.0f, .6f, 0.0f);
ComponentShape* leaf3a = CreateComponent<ComponentShape>(new Transform());
branch_3_1->AddChild(leaf3a);
leaf3a->shape = RenderManager::primitives.sphere;
leaf3a->SetPosition(basePos3 + glm::vec3(0.4f, 0.2f, -0.3f));
leaf3a->SetScale(glm::vec3(1.0f, 0.9f, 1.2f));
leaf3a->material = Material::Find("Leaf");

ComponentShape* leaf3b = CreateComponent<ComponentShape>(new Transform());
branch_3_1->AddChild(leaf3b);
leaf3b->shape = RenderManager::primitives.sphere;
leaf3b->SetPosition(basePos3 + glm::vec3(-0.3f, 0.1f, 0.4f));
leaf3b->SetScale(glm::vec3(1.1f, 0.8f, 1.0f));
leaf3b->material = Material::Find("Leaf");

ComponentShape* leaf3c = CreateComponent<ComponentShape>(new Transform());
branch_3_1->AddChild(leaf3c);
leaf3c->shape = RenderManager::primitives.sphere;
leaf3c->SetPosition(basePos3 + glm::vec3(0.2f, 0.3f, 0.3f));
leaf3c->SetScale(glm::vec3(0.9f, 0.9f, 1.1f));
leaf3c->material = Material::Find("Leaf");

// вращение
    point_1_1->SetRotation(glm::vec3(30.0, 0, 0));
    point_1_2->SetRotation(glm::vec3(30.0, 0, 0));

    point_leaf_1->SetRotation(glm::vec3(-45.0, 30, 0));


    point_2_1->SetRotation(glm::vec3(45.0, -30, 0));
    point_2_2->SetRotation(glm::vec3(65.0, 120, 0));

    point_3_1->SetRotation(glm::vec3(-45.0, 0, 0));


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
