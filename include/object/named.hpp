#pragma once
#include <string>

class Named {
public:
    Named(const std::string& name)
        : name(name)
    {}

    const std::string GetName() { return name; }
    void SetName(const std::string& new_name) { name = new_name; }

protected:
    std::string name;
};
