#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <assimp/scene.h>
#include "skeletal_animation.hpp"
#include "../skeleton.hpp"
#include <object/transform/transform.hpp>
#include <object/component/component.hpp>
#include <managers/time_manager.hpp>

class Animator {
public:
    Animator(const Skeleton& skeleton)
        : skeleton(skeleton)
    {}

    void SetAnimation(const std::string& animationName) {
        auto it = skeleton.AnimationMap.find(animationName);
        if (it == skeleton.AnimationMap.end()) {
            std::cerr << "Animation not found: " << animationName << std::endl;
            animation = nullptr;
            return;
        }

        animation = &it->second;
        startTime = TimeManager::GetCurrentTime();
    }

    bool HasAnimation() const { return animation != nullptr; }

    void ApplyAnimation(std::vector<glm::mat4x4>& transforms, glm::mat4x4 parentTransform) const {
        transforms.resize(skeleton.BoneLocal.size());
        if (!HasAnimation()) {
            ApplyTPose(transforms, parentTransform);
            return;
        }

        float currentTime = TimeManager::GetCurrentTime();
        float TicksPerSecond = (float)(animation->TicksPerSecond != 0 ? animation->TicksPerSecond : 25.0f);
        float TimeInSeconds = currentTime - startTime;
        float TimeInTicks = TimeInSeconds * TicksPerSecond;
        float AnimationTime = fmod(TimeInTicks, (float)animation->Duration);

        ReadNodeHierarchy(skeleton.BoneTree, parentTransform, transforms, AnimationTime);
    }

private:
    void Animator::ReadNodeHierarchy(const BoneNode& node, const glm::mat4& parentTransform,
                                    std::vector<glm::mat4x4>& transforms, float AnimationTime) const
    {
        glm::mat4 localTransform = glm::mat4(1.0f);

        if (node.Index >= 0) {
            auto it = animation->Tracks.find(node.Index);
            if (it != animation->Tracks.end()) {
                const auto& track = it->second;
                glm::vec3 scaling = track.CalcInterpolatedScaling(AnimationTime);
                glm::quat rotation = track.CalcInterpolatedRotation(AnimationTime);
                glm::vec3 translation = track.CalcInterpolatedPosition(AnimationTime);

                glm::mat4 T = glm::translate(glm::mat4(1.0f), translation);
                glm::mat4 R = glm::toMat4(rotation);
                glm::mat4 S = glm::scale(glm::mat4(1.0f), scaling);

                localTransform = T * R * S;
            }

            glm::mat4 globalTransform = parentTransform * localTransform;
            transforms[node.Index] = globalTransform * skeleton.BoneLocal[node.Index];

            for (const auto& child : node.Children) {
                ReadNodeHierarchy(child, globalTransform, transforms, AnimationTime);
            }
        } else {
            for (const auto& child : node.Children) {
                ReadNodeHierarchy(child, parentTransform, transforms, AnimationTime);
            }
        }
    }

    void ApplyTPose(std::vector<glm::mat4x4>& transforms, const glm::mat4x4& parentTransform) const {
        transforms.resize(skeleton.BoneLocal.size());
        ApplyTPoseRecursive(skeleton.BoneTree, parentTransform, transforms);
    }

    void ApplyTPoseRecursive(const BoneNode& node, const glm::mat4x4& parentTransform, std::vector<glm::mat4x4>& transforms) const {
        if (node.Index >= 0) {
            transforms[node.Index] = parentTransform ;
        } 
        
        for (const auto& child : node.Children) {
            ApplyTPoseRecursive(child, parentTransform, transforms);
        }
    }

private:
    const Skeleton& skeleton;
    const SkeletalAnimation* animation = nullptr;
    std::vector<glm::mat4x4>* transforms;
    float startTime;
};
