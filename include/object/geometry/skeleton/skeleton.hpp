#pragma once
#include <glm/glm.hpp>
#include <string>
#include "animation/skeletal_animation.hpp"
#include "bone/bone_node.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/dual_quaternion.hpp>

namespace tinygltf {
    class Model;
};

class Skeleton {
public:
    void BuildBoneTree(const aiNode* node);
    void BuildBoneTree(const tinygltf::Model& model, const std::unordered_map<int, int>& BoneIndexesMap, int nodeIndex);

private:
    void BuildBoneTreeRecursive(const aiNode* node, BoneNode& parent);
    void BuildBoneTreeRecursive(const tinygltf::Model& model, const std::unordered_map<int, int>& BoneIndexesMap,
                                int nodeIndex, BoneNode* parent);

public:
    std::unordered_map<std::string, int> BoneMap;
    std::unordered_map<std::string, SkeletalAnimation> AnimationMap;
    std::vector<glm::mat4x4> BoneLocal;
    std::vector<glm::dualquat> inverseBind;
    BoneNode BoneTree;
};
