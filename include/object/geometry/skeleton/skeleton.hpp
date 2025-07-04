#pragma once
#include <glm/glm.hpp>
#include <string>
#include "animation/skeletal_animation.hpp"
#include "bone/bone_node.hpp"

class Skeleton {
public:
    void BuildBoneTree(const aiNode* node) {
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

private:
    void BuildBoneTreeRecursive(const aiNode* node, BoneNode& parent) {
        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            const aiNode* childNode = node->mChildren[i];
            std::string childName(childNode->mName.C_Str());

            int boneIndex = -1;
            auto it = BoneMap.find(childName);
            if (it != BoneMap.end()) {
                boneIndex = it->second;
            }

            parent.Children.emplace_back(boneIndex, &parent);
            BoneNode& newChild = parent.Children.back();

            BuildBoneTreeRecursive(childNode, newChild);
        }
    }

public:
    std::unordered_map<std::string, int> BoneMap;
    std::unordered_map<std::string, SkeletalAnimation> AnimationMap;
    std::vector<glm::mat4x4> BoneLocal;
    std::vector<glm::quat> inverseBindRots, inverseBindDuals;
    BoneNode BoneTree;
};
