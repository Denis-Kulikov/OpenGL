#include <ui/element/element.hpp>
#include <memory>

class UIWindow : public UIElement {
public:
    UIWindow(const std::string& name)
        : UIElement(name), open(true) {}

    void Draw() override {
        if (!open || !visible) return;
        if (ImGui::Begin(name.c_str(), &open)) {
            for (auto& e : elements)
                e->Draw();
        }
        ImGui::End();
    }

    void AddElement(std::shared_ptr<UIElement> element) {
        elements.push_back(element);
    }

private:
    bool open;
    std::vector<std::shared_ptr<UIElement>> elements;
};
