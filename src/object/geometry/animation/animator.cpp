#include <object/geometry/skeleton/animation/animator.hpp>

void PrintMatrix(const glm::mat4& matrix);
void PrintMatrix(const glm::mat4x3& matrix);
std::string printVec3(const glm::vec3& v);
std::string printQuat(const glm::quat& q);
glm::vec3 quatToEuler(const glm::quat& q);
glm::vec3 ExtractTranslation(const glm::dualquat& dq) {
    glm::quat t_quat = dq.dual * glm::conjugate(dq.real);
    return 2.0f * glm::vec3(t_quat.x, t_quat.y, t_quat.z);
}

static bool f = true;
static int iMat = 0;
static int iDQ = 0;

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

void Animator::ApplyAnimation(std::vector<glm::mat4x4>& transforms, float deltaTime) {
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

    if (iMat < 3) std::cout << "MATRIX" << std::endl;
    ReadNodeHierarchy(skeleton.BoneTree, glm::mat4(1.f), transforms, 0);
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
            // localTransform = T * R * S;
        }

        glm::mat4 globalTransform = parentTransform * localTransform;
        transforms[node.Index] = glm::mat4x4(globalTransform * skeleton.BoneLocal[node.Index]);

        if (iMat < 3) {
            Transform localT;
            localT.SetMatrix(localTransform);
            localT.UpdateTransform();
            glm::vec3 anglesAnim = glm::degrees(glm::eulerAngles(localT.GetRotation()));
            std::cout << "localT: " << printVec3(localT.GetPosition()) << " | " << printVec3(anglesAnim) << std::endl;

            Transform globalT;
            globalT.SetMatrix(globalTransform);
            globalT.UpdateTransform();
            glm::vec3 anglesGlobalAnim = glm::degrees(glm::eulerAngles(globalT.GetRotation()));
            std::cout << "globalT: " << printVec3(globalT.GetPosition()) << " | " << printVec3(anglesGlobalAnim) << std::endl;

            Transform invT;
            invT.SetMatrix(skeleton.BoneLocal[node.Index]);
            invT.UpdateTransform();
            glm::vec3 anglesInvAnim = glm::degrees(glm::eulerAngles(invT.GetRotation()));
            std::cout << "invT: " << printVec3(invT.GetPosition()) << " | " << printVec3(anglesInvAnim) << std::endl;

            Transform totalT;
            totalT.SetMatrix(transforms[node.Index]);
            totalT.UpdateTransform();
            glm::vec3 anglesTotalAnim = glm::degrees(glm::eulerAngles(totalT.GetRotation()));
            std::cout << "totalT: " << printVec3(totalT.GetPosition()) << " | " << printVec3(anglesTotalAnim) << std::endl;

            std::cout << std::endl;
            ++iMat;
        }

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

    if (iDQ < 3) std::cout << "DQS" << std::endl;
    ReadNodeHierarchyDQ(skeleton.BoneTree, glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(0.0f), dualQuats, AnimationTime);
    f = false;
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

        glm::dualquat dq_anim = glm::dualquat(globalRot, globalTrans);
        dq_anim = glm::normalize(dq_anim);
        // dq_anim = dq_anim * skeleton.inverseBind[node.Index];
        dq_anim = dq_anim * skeleton.inverseBind[node.Index];
        dq_anim = glm::normalize(dq_anim);

        dualQuats[node.Index] = dq_anim;

        if (iDQ < 3) {
            glm::vec3 anglesAnim = glm::degrees(glm::eulerAngles(rotation));
            std::cout << "animation: " << printVec3(translation) << " | " << printVec3(anglesAnim) << std::endl;

            glm::vec3 angles = glm::degrees(glm::eulerAngles(globalRot));
            std::cout << "global: " << printVec3(globalTrans) << " | " << printVec3(angles) << std::endl;

            glm::vec3 anglesInv = glm::degrees(glm::eulerAngles(skeleton.inverseBind[node.Index].real));
            glm::vec3 translationInv = ExtractTranslation(skeleton.inverseBind[node.Index]);
            std::cout << "inv: " << printVec3(translationInv) << " | " << printVec3(anglesInv) << std::endl;

            glm::vec3 anglesRes = glm::degrees(glm::eulerAngles(dualQuats[node.Index].real));
            glm::vec3 translationRes = ExtractTranslation(dualQuats[node.Index]);
            std::cout << "total: " << printVec3(translationRes) << " | " << printVec3(anglesRes) << std::endl;

            // glm::quat invRot = skeleton.inverseBind[node.Index].real;
            // glm::vec3 corrected = glm::inverse(invRot) * translationRes;
            // // std::cout << "corrected: " << printVec3(corrected) << std::endl;
            // dualQuats[node.Index].dual = 0.5f * glm::quat(0, corrected.x, corrected.y, corrected.z) * dualQuats[node.Index].real;
            // glm::vec3 translationRR = ExtractTranslation(dualQuats[node.Index]);
            // glm::vec3 anglesRR = glm::degrees(glm::eulerAngles(dualQuats[node.Index].real));
            // // std::cout << "corrected: " << printVec3(translationRR) << " | " << printVec3(anglesRR)<< std::endl;

            std::cout << std::endl;
            ++iDQ;
        }

        // Корректировка вращения из-за inverse bind pose
        // glm::vec3 translationRes = ExtractTranslation(dualQuats[node.Index]);
        // glm::quat invRot = skeleton.inverseBind[node.Index].real;
        // glm::vec3 corrected = glm::inverse(invRot) * translationRes;

        // dualQuats[node.Index].dual = 0.5f * glm::quat(0, corrected.x, corrected.y, corrected.z) * dualQuats[node.Index].real;
        // glm::vec3 translationRR = ExtractTranslation(dualQuats[node.Index]);
        // glm::vec3 anglesRR = glm::degrees(glm::eulerAngles(dualQuats[node.Index].real));
    } 

    for (const auto& child : node.Children) {
        ReadNodeHierarchyDQ(child, globalRot, globalTrans, dualQuats, AnimationTime);
    }
}
