#include <entities/templates/decor/test_dq.hpp>
#include <managers/render_manager.hpp> 
#include <object/component/template_dual_quat/shape.hpp>

TestDQ::TestDQ()
{
    std::cout << name << std::endl;

    ComponentDQShape *root = new ComponentDQShape(new TransformDualQuat());
    root->shape = RenderManager::primitives.cube;
    // root->SetScale(glm::vec3(0.3, .5, 0.4));
    root->SetRotation(glm::vec3(0.0, 30.f, 0.0));
    root->material = Material::Find("TestDQ");

    ComponentDQShape *shape1 = new ComponentDQShape(new TransformDualQuat());
    root->AddChild(shape1);
    shape1->shape = RenderManager::primitives.cube;
    shape1->SetPosition(glm::vec3(0.5, 1.0, 0.25));
    // root->SetRotation(glm::vec3(0.0, 0, -90.0));
    shape1->material = Material::Find("TestDQ");

    // ComponentShape *box = CreateComponent<ComponentShape>(new Transform());
    // root->AddChild(box);
    // box->shape = RenderManager::primitives.cube;
    // box->SetPosition(glm::vec3(0.0, .5, 0.0));
    // box->SetScale(glm::vec3(0.3, .5, 0.4));
    // box->SetRotation(glm::vec3(0.0, 15, 0.0));
    // box->material = Material::Find("TestDQ");


    rootDualQuat = root;
    // rootComponent = root;
}

TestDQ::~TestDQ() {}

void TestDQ::Initialize()
{
    TestDQ::name = "TestDQ";
    auto shader_tree = Shader::Create("DQ_shader", "shaders/cube_fs.glsl", "shaders/dual_quat_vs.glsl");
    auto texture_tree = Texture::Create("tree", "img/tree.jpg");

    glm::mat4 mm;
    glm::vec4 qq;

    auto init_tree = new Material::InitFunction([](Material& m) {
        std::vector<std::string> str = {"PV" ,"gDQ", "textureSampler"};
        for (auto s : str) {
            auto it = m.values.find(s);
            if (it != m.values.end()) {
                delete static_cast<glm::mat4*>(it->second.second);
                m.values.erase(it);
            }
        }
        m.values["PV"] = {glGetUniformLocation(m.GetShader()->GetID(), "PV"), new glm::mat4};
        m.values["gDQ"] = {glGetUniformLocation(m.GetShader()->GetID(), "gDQ"), new glm::dualquat};
        assert(m.values["gDQ"].first >= 0 );
        m.values["textureSampler"] = {glGetUniformLocation(m.GetShader()->GetID(), "textureSampler"), nullptr};
    });
    auto apply_tree = new Material::ApplyFunction([](const Material& m) {
        glActiveTexture(GL_TEXTURE0);

        GLint texLoc = m.values.at("textureSampler").first;
        glUniform1i(texLoc, 0);

        glm::mat4* PV = static_cast<glm::mat4*>(m.values.at("PV").second);
        glm::dualquat* dq = static_cast<glm::dualquat*>(m.values.at("gDQ").second);
        glUniformMatrix4fv(m.values.at("PV").first, 1, GL_FALSE, glm::value_ptr(*PV));
        glUniform4fv(m.values.at("gDQ").first, 2, glm::value_ptr(dq->real));
    });

    auto init_leaf = init_tree;
    auto apply_leaf = apply_tree;

    auto material_tree = Material::Create("TestDQ", shader_tree, init_tree, apply_tree);
    material_tree->PushTexture(texture_tree);
}

std::string TestDQ::GetName() const {
    return TestDQ::name;
}
