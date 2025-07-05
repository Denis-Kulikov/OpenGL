#include <object/geometry/skeleton/animation/animator.hpp>

Animator::Animator(const Skeleton& skeleton)
    : skeleton(skeleton)
{}

void Animator::SetAnimation(const std::string& animationName) {
    auto it = skeleton.AnimationMap.find(animationName);
    if (it == skeleton.AnimationMap.end()) {
        std::cerr << "Animation not found: " << animationName << std::endl;
        animation = nullptr;
        return;
    }

    animation = &it->second;
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

    ReadNodeHierarchyDQ(skeleton.BoneTree, glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(0.0f), dualQuats, AnimationTime);
}

void Animator::ReadNodeHierarchyDQ(const BoneNode& node, 
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

        const glm::dualquat& inv_bind = skeleton.inverseBind[node.Index];
        dualQuats[node.Index] = dq_anim * inv_bind;

        for (const auto& child : node.Children) {
            ReadNodeHierarchyDQ(child, globalRot, globalTrans, dualQuats, AnimationTime);
        }
    } else {
        for (const auto& child : node.Children) {
            ReadNodeHierarchyDQ(child, parentRot, parentTrans, dualQuats, AnimationTime);
        }
    }
}
