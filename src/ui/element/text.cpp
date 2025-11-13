#include <ui/element/element.hpp>
#include <format>

class UIText : public UIElement {
public:
    UIText(const std::string& name)
        : UIElement(name) {}

    template<typename... Args>
    void SetText(std::string_view fmt_str, Args&&... args) {
        text = std::vformat(fmt_str, std::make_format_args(std::forward<Args>(args)...));
    }

    void Draw() override {
        if (!visible) return;
        ImGui::TextUnformatted(text.c_str());
    }

private:
    std::string text;
};
