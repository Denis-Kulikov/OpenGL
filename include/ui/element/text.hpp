#include <ui/element/element.hpp>
#include <stdlib.h>
#include <format>

class UIText : public UIElement {
public:
    UIText(const std::string& name)
        : UIElement(name) {}

    template<typename... Args>
    void SetText(std::string_view fmt_str, Args&&... args) {
        auto format_args = std::make_format_args(args...);
        text = std::vformat(fmt_str, format_args);
    }

    void Draw() override {
        if (!visible) return;
        ImGui::TextUnformatted(text.c_str());
    }

private:
    std::string text;
};
