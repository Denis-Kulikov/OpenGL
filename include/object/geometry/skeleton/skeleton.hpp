#pragma once
#include <glm/glm.hpp>
#include <string>
#include "animation/skeletal_animation.hpp"
#include "bone/bone_node.hpp"

class Skeleton {
public:
    void BuildBoneTree(const aiNode* node) {
        glm::mat4 transform;
        std::string nodeName(node->mName.C_Str());
        int boneIndex = -1;

        auto it = BoneMap.find(nodeName);
        if (it != BoneMap.end()) {
            boneIndex = it->second;
            BoneInvers[boneIndex] = glm::mat4(1.0f);
            transform = BoneLocal[boneIndex];
        } else {
            transform = glm::mat4(1.0f);
        }
        
        BoneTree = BoneNode(boneIndex, nullptr);
        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            BuildBoneTreeRecursive(node->mChildren[i], BoneTree, transform);
        }
    }

private:
    void BuildBoneTreeRecursive(const aiNode* node, BoneNode& parent, const glm::mat4& parentTransform) {
        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            glm::mat4 transform;
            const aiNode* childNode = node->mChildren[i];
            std::string childName(childNode->mName.C_Str());

            int boneIndex = -1;
            auto it = BoneMap.find(childName);
            if (it != BoneMap.end()) {
                boneIndex = it->second;
                BoneInvers[boneIndex] = glm::inverse(parentTransform);
                transform = parentTransform * BoneLocal[boneIndex];
            } else {
                transform = parentTransform;
            }

            parent.Children.emplace_back(boneIndex, &parent);
            BoneNode& newChild = parent.Children.back();

            BuildBoneTreeRecursive(childNode, newChild, transform);
        }
    }

public:
    std::unordered_map<std::string, int> BoneMap;
    std::unordered_map<std::string, SkeletalAnimation> AnimationMap;
    std::vector<glm::mat4> BoneLocal;
    std::vector<glm::mat4> BoneInvers;
    BoneNode BoneTree;
};
