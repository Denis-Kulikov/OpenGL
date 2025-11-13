#pragma once
#include <string>
#include <vector>
#include <imgui.h>

class UIElement {
public:
    UIElement(const std::string& name = "UIElement")
        : name(name), visible(true) {}

    // virtual ~UIElement() = default;

    virtual void Draw() = 0;
    virtual void Update(float dt) {}
    virtual void SetVisible(bool v) { visible = v; }
    bool IsVisible() const { return visible; }

    std::string name;

protected:
    bool visible;
};
