#include <object/mesh/skeleton/skeleton.hpp>
#include <tiny_gltf.h>

void PrintBoneTree(const BoneNode& node, int depth = 0) {
    // Печатаем отступы в зависимости от уровня
    for (int i = 0; i < depth; ++i)
        std::cout << "  ";

    std::cout << "Bone index: " << node.Index << "\n";

    // Рекурсивно печатаем детей
    for (const auto& child : node.Children) {
        PrintBoneTree(child, depth + 1);
    }
}


void Skeleton::BuildBoneTree(const aiNode* node) {
    std::string nodeName(node->mName.C_Str());
    int boneIndex = -1;
    auto it = BoneMap.find(nodeName);
    if (it != BoneMap.end()) {
        boneIndex = it->second;
    }
    
    BoneTree = BoneNode(boneIndex, nullptr);
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        BuildBoneTreeRecursive(node->mChildren[i], BoneTree);
    }
}

void Skeleton::BuildBoneTreeRecursive(const aiNode* node, BoneNode& parent) {
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        const aiNode* childNode = node->mChildren[i];
        std::string childName(childNode->mName.C_Str());

        auto it = BoneMap.find(childName);
        if (it != BoneMap.end()) {
            int boneIndex = -1;
            boneIndex = it->second;
            parent.Children.emplace_back(boneIndex, &parent);
            BoneNode& newChild = parent.Children.back();
            BuildBoneTreeRecursive(childNode, newChild);
        } else {
            BuildBoneTreeRecursive(childNode, parent);
        }
    }
}

void Skeleton::BuildBoneTree(const tinygltf::Model& model, const std::unordered_map<int, int>& BoneIndexesMap, int nodeIndex) {
    const auto& node = model.nodes[nodeIndex];

    auto it = BoneIndexesMap.find(nodeIndex);
    if (it != BoneIndexesMap.end()) {
        BoneMap.insert({node.name, it->second});
        BoneTree = BoneNode(it->second, nullptr);
    } else {
        BoneTree = BoneNode(-1, nullptr);
    }

    for (int childIndex : node.children) {
        BuildBoneTreeRecursive(model, BoneIndexesMap, childIndex, &BoneTree);
    }

    // PrintBoneTree(BoneTree);
}

void Skeleton::BuildBoneTreeRecursive(const tinygltf::Model& model, const std::unordered_map<int, int>& BoneIndexesMap,
                                      int nodeIndex, BoneNode* parent) {
    const auto& node = model.nodes[nodeIndex];

    BoneNode* nodePtr;
    auto it = BoneIndexesMap.find(nodeIndex);
    if (it != BoneIndexesMap.end()) {
        BoneMap.insert({node.name, it->second});
        parent->Children.emplace_back(it->second, parent);
        nodePtr = &parent->Children.back();
    } else {
        nodePtr = parent;
    }

    for (int childIndex : node.children) {
        BuildBoneTreeRecursive(model, BoneIndexesMap, childIndex, nodePtr);
    }
}
