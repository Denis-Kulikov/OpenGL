#pragma once
#include <vector>

struct BoneNode {
    BoneNode(int BoneIndex, BoneNode* Parent)
        : Index(BoneIndex), Parent(Parent) {}
    BoneNode() {}

    BoneNode* Parent = nullptr;
    std::vector<BoneNode> Children;
    int Index;
};