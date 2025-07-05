#include <managers/render_manager.hpp>
#include <object/component/template_dual_quat/shape.hpp>

std::string printQuat(const glm::quat& q);

void ComponentDQShape::Render() const {
    // std::cout << "dq_real: " << material->values["dq_real"].first << " " << printQuat(globalTransform->dq_real) << std::endl;
    // std::cout << "dq_dual: " << material->values["dq_dual"].first << " " << printQuat(globalTransform->dq_dual) << std::endl;
    // std::cout << std::endl;
    material->UpdateValue("PV", RenderManager::pipeline.PV);
    material->UpdateValue("gDQ", globalTransform->dq);
    material->Bind();
    if (!material->GetTexture().empty())
        material->GetTexture()[0]->Bind();
    shape->Bind();
    shape->Draw();
}
