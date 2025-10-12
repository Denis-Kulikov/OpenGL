#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/scene.h>

#include <iostream>

namespace tinygltf {
    class Model;
    class Animation;
};

class BoneKeyframes {
private:
    template<typename T>
    unsigned int FindKeyframeIndex(float AnimationTime, const std::vector<std::pair<double, T>>& keyframes) const {
        if (keyframes.size() < 2)
            return 0;

        int left = 0;
        int right = keyframes.size() - 2;

        while (left <= right) {
            int mid = (left + right) / 2;
            if (AnimationTime < keyframes[mid + 1].first)
                right = mid - 1;
            else
                left = mid + 1;
        }

        return left;
    }

public:
    unsigned int FindPosition(float AnimationTime) const {
        return FindKeyframeIndex(AnimationTime, Positions);
    }
    unsigned int FindRotation(float AnimationTime) const {
        return FindKeyframeIndex(AnimationTime, Rotations);
    }
    unsigned int FindScaling(float AnimationTime) const {
        return FindKeyframeIndex(AnimationTime, Scales);
    }

    glm::vec3 CalcInterpolatedPosition(float AnimationTime) const;
    glm::quat CalcInterpolatedRotation(float AnimationTime) const;
    glm::vec3 CalcInterpolatedScaling(float AnimationTime) const;

    std::vector<std::pair<double, glm::vec3>> Positions;
    std::vector<std::pair<double, glm::quat>> Rotations;
    std::vector<std::pair<double, glm::vec3>> Scales;
};

class SkeletalAnimation {
public:
    double Duration = 0.0;              // в тиках
    double TicksPerSecond = 25.0;       // default if not specified

    std::unordered_map<int, BoneKeyframes> Tracks;

    explicit SkeletalAnimation(const aiAnimation* anim, const std::unordered_map<std::string, int>& BoneMap);
    explicit SkeletalAnimation(const tinygltf::Model& model, const tinygltf::Animation& anim,
                  const std::unordered_map<int, int>& NodeToBoneIndex);
};
