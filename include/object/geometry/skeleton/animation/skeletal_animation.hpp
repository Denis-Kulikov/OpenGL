#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/scene.h>

#include <iostream>

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

    glm::vec3 CalcInterpolatedPosition(float AnimationTime) const {
        if (Positions.empty())
            return glm::vec3(0.0f);
        
        if (Positions.size() == 1 || AnimationTime <= Positions.front().first)
            return Positions.front().second;

        if (AnimationTime >= Positions.back().first)
            return Positions.back().second;

        unsigned int PositionIndex = FindPosition(AnimationTime);
        unsigned int NextPositionIndex = PositionIndex + 1;

        if (NextPositionIndex >= Positions.size()) // страховка
            return Positions.back().second;

        float StartTime = static_cast<float>(Positions[PositionIndex].first);
        float EndTime   = static_cast<float>(Positions[NextPositionIndex].first);
        float DeltaTime = EndTime - StartTime;

        if (DeltaTime <= 0.0f)
            return Positions[PositionIndex].second;

        float Factor = (AnimationTime - StartTime) / DeltaTime;
        Factor = std::clamp(Factor, 0.0f, 1.0f);

        const auto& Start = Positions[PositionIndex].second;
        const auto& End = Positions[NextPositionIndex].second;

        return glm::mix(Start, End, Factor);
    }
    glm::quat CalcInterpolatedRotation(float AnimationTime) const {
        if (Rotations.empty())
            return glm::quat(0, 0, 0, 0);
        
        if (Rotations.size() == 1 || AnimationTime <= Rotations.front().first)
            return Rotations.front().second;

        if (AnimationTime >= Rotations.back().first)
            return Rotations.back().second;

        unsigned int RotationIndex = FindRotation(AnimationTime);
        unsigned int NextRotationIndex = RotationIndex + 1;

        if (NextRotationIndex >= Rotations.size())
            return Rotations.back().second;

        float StartTime = static_cast<float>(Rotations[RotationIndex].first);
        float EndTime   = static_cast<float>(Rotations[NextRotationIndex].first);
        float DeltaTime = EndTime - StartTime;

        if (DeltaTime <= 0.0f)
            return Rotations[RotationIndex].second;

        float Factor = std::clamp((AnimationTime - StartTime) / DeltaTime, 0.0f, 1.0f);

        const auto& Start = Rotations[RotationIndex].second;
        const auto& End   = Rotations[NextRotationIndex].second;

        return glm::normalize(glm::slerp(Start, End, Factor));
    }
    glm::vec3 CalcInterpolatedScaling(float AnimationTime) const {
        if (Scales.empty())
            return glm::vec3(0.0f);
        
        if (Scales.size() == 1 || AnimationTime <= Scales.front().first)
            return Scales.front().second;

        if (AnimationTime >= Scales.back().first)
            return Scales.back().second;

        unsigned int PositionIndex = FindScaling(AnimationTime);
        unsigned int NextPositionIndex = PositionIndex + 1;

        if (NextPositionIndex >= Scales.size()) // страховка
            return Scales.back().second;

        float StartTime = static_cast<float>(Scales[PositionIndex].first);
        float EndTime   = static_cast<float>(Scales[NextPositionIndex].first);
        float DeltaTime = EndTime - StartTime;

        if (DeltaTime <= 0.0f)
            return Scales[PositionIndex].second;

        float Factor = (AnimationTime - StartTime) / DeltaTime;
        Factor = std::clamp(Factor, 0.0f, 1.0f);

        const auto& Start = Scales[PositionIndex].second;
        const auto& End = Scales[NextPositionIndex].second;

        return glm::mix(Start, End, Factor);
    }

    std::vector<std::pair<double, glm::vec3>> Positions;
    std::vector<std::pair<double, glm::quat>> Rotations;
    std::vector<std::pair<double, glm::vec3>> Scales;
};

class SkeletalAnimation {
public:
    double Duration = 0.0;              // в тиках
    double TicksPerSecond = 25.0;       // default if not specified

    // Каналы: имя кости -> ключевые кадры
    std::unordered_map<int, BoneKeyframes> Tracks;

    explicit SkeletalAnimation(const aiAnimation* anim, const std::unordered_map<std::string, int>& BoneMap) {
        Duration = anim->mDuration;
        TicksPerSecond = anim->mTicksPerSecond != 0.0 ? anim->mTicksPerSecond : 25.0;

        std::cout << "Duration = " << Duration << std::endl;
        std::cout << "TicksPerSecond = " << TicksPerSecond << std::endl;

        for (unsigned int i = 0; i < anim->mNumChannels; ++i) {
            aiNodeAnim* channel = anim->mChannels[i];
            std::string boneName(channel->mNodeName.C_Str());
            BoneKeyframes keyframes;

            for (unsigned int j = 0; j < channel->mNumPositionKeys; ++j) {
                auto& key = channel->mPositionKeys[j];
                keyframes.Positions.emplace_back(key.mTime, glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z));
            }
            for (unsigned int j = 0; j < channel->mNumRotationKeys; ++j) {
                auto& key = channel->mRotationKeys[j];
                keyframes.Rotations.emplace_back(key.mTime, glm::quat(key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z));
            }
            for (unsigned int j = 0; j < channel->mNumScalingKeys; ++j) {
                auto& key = channel->mScalingKeys[j];
                keyframes.Scales.emplace_back(key.mTime, glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z));
            }

            auto it = BoneMap.find(boneName);
            if (it != BoneMap.end()) {
                Tracks[it->second] = std::move(keyframes);
            } else {
                std::cerr << "Bone name not found in BoneMap: " << boneName << std::endl;
            }
            std::cout << "keyframes.Positions.size = " << keyframes.Positions.size() << std::endl;
            std::cout << "keyframes.Rotations.size = " << keyframes.Rotations.size() << std::endl;
        }
    }
};
