#pragma once

#include <string>
#include <vector>

class DynamicCompiler {
public:
    bool compile(const std::string &filename);

    void* getSymbol(const std::string &symbolName);

    DynamicCompiler();
    ~DynamicCompiler();

private:
    struct Impl;
    Impl* impl;
};
