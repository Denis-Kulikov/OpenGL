#include <object/mesh/skeleton/animation/animator.hpp>

void PrintMatrix(const glm::mat4& matrix);
void PrintMatrix(const glm::mat4x3& matrix);
std::string printVec3(const glm::vec3& v);
std::string printQuat(const glm::quat& q);
glm::vec3 quatToEuler(const glm::quat& q);
glm::vec3 ExtractTranslation(const glm::dualquat& dq);

static bool f = true;
// static int iMat = 0;
// static int iDQ = 0;

Animator::Animator(const Skeleton& skeleton)
    : skeleton(skeleton)
{}

void Animator::SetAnimationAny() {
    if (!skeleton.AnimationMap.empty()) {
        animation = &skeleton.AnimationMap.begin()->second;
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

void Animator::ApplyAnimation(std::vector<glm::mat4x4>& transforms, float deltaTime) {
    transforms.resize(skeleton.inverseBindMat.size());
    if (animation == nullptr) {
        for (auto &t : transforms)
            t = glm::mat4x4(1.f);

        return;
    }

    animationTime += deltaTime;
    // animationTime = 2;
    float TicksPerSecond = (float)(animation->TicksPerSecond != 0 ? animation->TicksPerSecond : 25.0f);
    float TimeInTicks = animationTime * TicksPerSecond;
    float AnimationTime = fmod(TimeInTicks, (float)animation->Duration);

    ReadNodeHierarchy(skeleton.BoneTree, glm::mat4(1.f), transforms, AnimationTime);
    f = false;
}

void Animator::ReadNodeHierarchy(const BoneNode& node, const glm::mat4& parentTransform,
                                std::vector<glm::mat4x4>& transforms, float AnimationTime) const
{
    if (node.Index >= 0) {
        glm::mat4 localTransform = glm::mat4(1.0f);
        
        auto it = animation->Tracks.find(node.Index);
        if (it != animation->Tracks.end()) {
            const auto& track = it->second;
            // glm::vec3 scaling = track.CalcInterpolatedScaling(AnimationTime);
            glm::quat rotation = track.CalcInterpolatedRotation(AnimationTime);
            glm::vec3 translation = track.CalcInterpolatedPosition(AnimationTime);

            glm::mat4 T = glm::translate(glm::mat4(1.0f), translation);
            glm::mat4 R = glm::mat4_cast(rotation);
            // glm::mat4 S = glm::scale(glm::mat4(1.0f), scaling);

            localTransform = T * R;
            // if (f) PrintMatrix(localTransform);
            // localTransform = T * R * S;
        }

        // Transform t;
        // t.SetMatrix(localTransform);
        // t.UpdateTransform();
        // std::cout << "Pos: " << printVec3(t.GetPosition()) << " | Rot: " << printVec3(quatToEuler(t.GetRotation())) << std::endl;

        glm::mat4 globalTransform = parentTransform * localTransform;
        transforms[node.Index] = glm::mat4x4(globalTransform * skeleton.inverseBindMat[node.Index]);

        // if (f) std::cout << "Bone index: " << node.Index << "\n";
        // if (f) PrintMatrix(transforms[node.Index]);

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
    dualQuats.resize(skeleton.inverseBindMat.size(), {glm::quat(1, 0, 0, 0), glm::quat(0, 0, 0, 0)});

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
    glm::quat globalRot = parentRot;
    glm::vec3 globalTrans = parentTrans;

    if (node.Index >= 0) {
        glm::vec3 translation(0.0f);
        glm::quat rotation(1.0f, 0.0f, 0.0f, 0.0f);

        auto it = animation->Tracks.find(node.Index);
        if (it != animation->Tracks.end()) {
            const auto& track = it->second;
            translation = track.CalcInterpolatedPosition(AnimationTime);
            rotation = track.CalcInterpolatedRotation(AnimationTime);
        }

        globalRot = parentRot * rotation;
        globalTrans = parentTrans + parentRot * translation;

        auto q_global = glm::normalize(globalRot);

        glm::quat t_quat(0, globalTrans.x, globalTrans.y, globalTrans.z);
        glm::quat d_global = 0.5f * t_quat * q_global;

        glm::dualquat dq_anim = glm::dualquat(globalRot, d_global);
        dq_anim = dq_anim * skeleton.inverseBindDQ[node.Index];

        dualQuats[node.Index] = glm::normalize(dq_anim);
    } 

    for (const auto& child : node.Children) {
        ReadNodeHierarchyDQ(child, globalRot, globalTrans, dualQuats, AnimationTime);
    }
}
