#include <object/geometry/skeleton/animation/animator.hpp>

std::string printVec3(const glm::vec3& v);
std::string printQuat(const glm::quat& q);
glm::vec3 quatToEuler(const glm::quat& q);

static bool f = true;

Animator::Animator(const Skeleton& skeleton)
    : skeleton(skeleton)
{}

void Animator::SetAnimationAny() {
    if (!skeleton.AnimationMap.empty()) {
        animation = &skeleton.AnimationMap.begin()->second;
        std::cout << "Animation name: " << skeleton.AnimationMap.begin()->first << std::endl;
    }
    animationTime = 0.f;
}

void Animator::SetAnimation(const std::string& animationName) {
    auto it = skeleton.AnimationMap.find(animationName);
    if (it == skeleton.AnimationMap.end()) {
        std::cerr << "Animation not found: " << animationName << std::endl;
        animation = nullptr;
        return;
    }
    animationTime = 0.f;
}

void Animator::ApplyAnimation(std::vector<glm::mat4x3>& transforms, float deltaTime) {
    transforms.resize(skeleton.BoneLocal.size());
    if (animation == nullptr) {
        for (auto &t : transforms)
            t = glm::mat4x4(1.f);

        return;
    }

    animationTime += deltaTime;
    float TicksPerSecond = (float)(animation->TicksPerSecond != 0 ? animation->TicksPerSecond : 25.0f);
    float TimeInTicks = animationTime * TicksPerSecond;
    float AnimationTime = fmod(TimeInTicks, (float)animation->Duration);

    ReadNodeHierarchy(skeleton.BoneTree, glm::mat4(1.f), transforms, AnimationTime);
}

void Animator::ReadNodeHierarchy(const BoneNode& node, const glm::mat4& parentTransform,
                                std::vector<glm::mat4x3>& transforms, float AnimationTime) const
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
            glm::mat4 R = glm::mat4_cast(rotation);
            glm::mat4 S = glm::scale(glm::mat4(1.0f), scaling);

            localTransform = T * R * S;
        }

        glm::mat4 globalTransform = parentTransform * localTransform;
        glm::mat4 boneTransform = globalTransform * skeleton.BoneLocal[node.Index];
        transforms[node.Index] = glm::mat4x3(boneTransform);

        for (const auto& child : node.Children) {
            ReadNodeHierarchy(child, globalTransform, transforms, AnimationTime);
        }
    } else {
        for (const auto& child : node.Children) {
            ReadNodeHierarchy(child, parentTransform, transforms, AnimationTime);
        }
    }
}


void Animator::ApplyAnimationDQ(std::vector<glm::dualquat>& dualQuats, float deltaTime) {
    dualQuats.resize(skeleton.BoneLocal.size(), {glm::quat(1, 0, 0, 0), glm::quat(0, 0, 0, 0)});

    if (!animation) return;

    animationTime += deltaTime;
    float TicksPerSecond = animation->TicksPerSecond != 0 ? animation->TicksPerSecond : 25.0f;
    float TimeInTicks = animationTime * TicksPerSecond;
    float AnimationTime = fmod(TimeInTicks, animation->Duration);

    ReadNodeHierarchyDQ(skeleton.BoneTree, {glm::quat(1, 0, 0, 0), glm::quat(0, 0, 0, 0)}, 
                        glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(0.0f), dualQuats, 0);
    f = false;
}

void Animator::ReadNodeHierarchyDQ(const BoneNode& node, const glm::dualquat& parentTransform,
                                   const glm::quat& parentRot, const glm::vec3& parentTrans,
                                   std::vector<glm::dualquat>& dualQuats, float AnimationTime) const
{
    if (node.Index >= 0) {
        glm::vec3 translation(0.0f);
        glm::quat rotation(1.0f, 0.0f, 0.0f, 0.0f);

        auto it = animation->Tracks.find(node.Index);
        if (it != animation->Tracks.end()) {
            const auto& track = it->second;
            translation = track.CalcInterpolatedPosition(AnimationTime);
            rotation = track.CalcInterpolatedRotation(AnimationTime);
        }

        glm::quat globalRot = parentRot * rotation;
        glm::vec3 globalTrans = parentTrans + parentRot * translation;

        glm::dualquat dq_anim = glm::dualquat(globalRot, globalTrans);
        dq_anim = glm::normalize(dq_anim);
        dq_anim = dq_anim * skeleton.inverseBind[node.Index];

        dualQuats[node.Index] = dq_anim;

        // Вращение res относительно родителя корректно
        // Смещение global в глобальных координатах корректно
        if (f) {
            glm::vec3 angles = glm::degrees(glm::eulerAngles(globalRot));
            std::cout << "global: " << printVec3(globalTrans) << " | " << printVec3(angles) << std::endl;

            // glm::vec3 anglesInv = glm::degrees(glm::eulerAngles(skeleton.inverseBind[node.Index].real));
            // std::cout << "inv: " << printQuat(skeleton.inverseBind[node.Index].dual) << " | " << printVec3(anglesInv) << std::endl;

            glm::vec3 anglesRes = glm::degrees(glm::eulerAngles(dualQuats[node.Index].real));
            std::cout << "res: " << printQuat(dualQuats[node.Index].dual) << " | " << printVec3(anglesRes) << std::endl;
        }

        for (const auto& child : node.Children) {
            ReadNodeHierarchyDQ(child, dualQuats[node.Index], globalRot, globalTrans, dualQuats, AnimationTime);
        }
    } else {
        for (const auto& child : node.Children) {
            ReadNodeHierarchyDQ(child, parentTransform, parentRot, parentTrans, dualQuats, AnimationTime);
        }
    }
}
