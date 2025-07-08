#pragma once
#include <assimp/scene.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "skeletal_animation.hpp"
#include <object/transform/transform.hpp>
#include <object/component/component.hpp>
#include <managers/time_manager.hpp>
#include "../skeleton.hpp"

class Animator {
public:
    Animator(const Skeleton& skeleton);

    void SetAnimationAny();
    void SetAnimation(const std::string& animationName);
    void ApplyAnimation(std::vector<glm::mat4x3>& transforms, float deltaTime);
    void ApplyAnimationDQ(std::vector<glm::dualquat>& dualQuats, float deltaTime);

private:
    void ReadNodeHierarchy(const BoneNode& node, const glm::mat4& parentTransform,
                           std::vector<glm::mat4x3>& transforms, float AnimationTime) const;
    void ReadNodeHierarchyDQ(const BoneNode& node, const glm::dualquat& parentTransform,
                             const glm::quat& parentRot, const glm::vec3& parentTrans,
                             std::vector<glm::dualquat>& dualQuats, float AnimationTime) const;


private:
    const Skeleton& skeleton;
    const SkeletalAnimation* animation = nullptr;
    float startTime;
    float animationTime;
};
