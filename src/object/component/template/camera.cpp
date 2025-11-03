#include <object/component/template/camera.hpp>

void ComponentCamera::UpdatePV() {
    PV = Proj * View;
}

void ComponentCamera::UpdateProj(bool perspective) {
    if (perspective)
        UpdatePerspective();
    else
        UpdateOrthographic();
}

void ComponentCamera::UpdateView() {
    auto& c = camera;
    
    View = LookAtLH(
        GetGlobalPosition(),
        GetGlobalPosition() + c.Params.Target,
        c.Params.Up
    );
}

void ComponentCamera::UpdatePerspective() {
    auto& c = camera;

    Proj = PerspectiveLH(
        glm::radians(c.PersProj.FOV),
        static_cast<float>(c.PersProj.Width) / c.PersProj.Height,
        c.PersProj.zNear,
        c.PersProj.zFar
    );

    PV = Proj * View;
}

void ComponentCamera::UpdateOrthographic() {
    const auto& c = camera;

    float halfW = c.PersProj.Width / 2.0f;
    float halfH = c.PersProj.Height / 2.0f;

    Proj = OrthoLH(
        -halfW, halfW,
        -halfH, halfH,
        c.PersProj.zNear, c.PersProj.zFar
    );

    PV = Proj * View;
}
