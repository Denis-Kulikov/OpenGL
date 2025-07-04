#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "skeletal_animation.hpp"
#include "../skeleton.hpp"
#include <object/transform/transform.hpp>
#include <object/component/component.hpp>
#include <managers/time_manager.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

class Animator {
public:
    Animator(const Skeleton& skeleton);

    void SetAnimation(const std::string& animationName);
    void ApplyAnimation(std::vector<glm::mat4x3>& transforms, float deltaTime);
    void ApplyAnimationDQ(std::vector<glm::quat>& rotations, std::vector<glm::quat>& duals, float deltaTime);

private:
    void ReadNodeHierarchy(const BoneNode& node, const glm::mat4& parentTransform,
                           std::vector<glm::mat4x3>& transforms, float AnimationTime) const;
    void ReadNodeHierarchyDQ(const BoneNode& node, 
                             const glm::quat& parentRot, const glm::vec3& parentDuals,
                             std::vector<glm::quat>& rotations, std::vector<glm::quat>& duals,
                             float AnimationTime) const;


private:
    const Skeleton& skeleton;
    const SkeletalAnimation* animation = nullptr;
    float startTime;
    float animationTime;
};
