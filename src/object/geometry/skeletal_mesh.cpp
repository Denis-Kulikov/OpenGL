#include <object/geometry/skeletal_mesh.hpp>


GeometrySkeletalMesh::GeometrySkeletalMesh(const std::string& Filename) 
    : GeometryMesh()
{
    buffers.resize(NUM_skeletal_VBs, 0);
    InitBuffers();

    if (LoadMesh(Filename)) {

    }

    glBindVertexArray(0);	
    SetType(MESH);
}

bool GeometrySkeletalMesh::InitFromScene(const aiScene* m_pScene, const std::string& Filename) {
    m_Entries.resize(m_pScene->mNumMeshes);
    m_Textures.resize(m_pScene->mNumMaterials);

    std::vector<unsigned int> Indices;
    std::vector<glm::vec3> Positions;
    std::vector<glm::vec2> TexCoords;
    std::vector<glm::vec3> Normals;
    std::vector<VertexBoneData> Bones;
    
    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;
    
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        m_Entries[i].MaterialIndex = m_pScene->mMeshes[i]->mMaterialIndex;        
        m_Entries[i].NumIndices    = m_pScene->mMeshes[i]->mNumFaces * 3;
        m_Entries[i].BaseVertex    = NumVertices;
        m_Entries[i].BaseIndex     = NumIndices;
        
        NumVertices += m_pScene->mMeshes[i]->mNumVertices;
        NumIndices  += m_Entries[i].NumIndices;
    }

    Indices.reserve(NumIndices);
    Positions.reserve(NumVertices);
    TexCoords.reserve(NumVertices);
    Normals.reserve(NumVertices);
    Bones.resize(NumVertices);

    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        const aiMesh* paiMesh = m_pScene->mMeshes[i];
        InitMesh(i, paiMesh, Positions, Normals, TexCoords, Indices);
        LoadBones(i, paiMesh, Bones);
    }

    if (!InitMaterials(m_pScene, Filename)) {
        return false;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[EBO]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[VBO]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);    

    glBindBuffer(GL_ARRAY_BUFFER, buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[BONE_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);    
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

    return true;
}

void GeometrySkeletalMesh::LoadBones(unsigned int MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones) {
    for (unsigned int i = 0; i < pMesh->mNumBones; i++) {
        unsigned int BoneIndex = 0;
        std::string BoneName(pMesh->mBones[i]->mName.data);

        if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
            BoneIndex = m_NumBones;
            m_NumBones++;
            m_BoneInfo.emplace_back(BoneInfo());
        } else {
            BoneIndex = m_BoneMapping[BoneName];
        }

        m_BoneMapping[BoneName] = BoneIndex;
        auto m = pMesh->mBones[i]->mOffsetMatrix;
        m_BoneInfo[BoneIndex].BoneOffset = glm::mat4(
            m.a1, m.b1, m.c1, m.d1,
            m.a2, m.b2, m.c2, m.d2,
            m.a3, m.b3, m.c3, m.d3,
            m.a4, m.b4, m.c4, m.d4
        );

        for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
            unsigned int VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
            float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
            Bones[VertexID].AddBoneData(BoneIndex, Weight);
        }
    }
}

unsigned int GeometrySkeletalMesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{    
    for (unsigned int i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }
    
    return 0;
}

unsigned int GeometrySkeletalMesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    for (unsigned int i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }

    return 0;
}

unsigned int GeometrySkeletalMesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    for (unsigned int i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }

    return 0;
}


void GeometrySkeletalMesh::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumPositionKeys == 1) {
        Out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }
            
    unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
    unsigned int NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
    float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}

void GeometrySkeletalMesh::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
    if (pNodeAnim->mNumRotationKeys == 1) {
        Out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }
    
    unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
    unsigned int NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
    float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& EndRotationQ   = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;    
    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
    Out = Out.Normalize();
}

void GeometrySkeletalMesh::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumScalingKeys == 1) {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
    unsigned int NextScalingIndex = (ScalingIndex + 1);
    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}


const aiNodeAnim* GeometrySkeletalMesh::FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
{
    for (unsigned int i = 0 ; i < pAnimation->mNumChannels ; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
        
        if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
            return pNodeAnim;
        }
    }
    
    return NULL;
}

void GeometrySkeletalMesh::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform)
{
    std::string NodeName(pNode->mName.data);

    const aiAnimation* pAnimation = m_pScene->mAnimations[0];

    aiMatrix4x4 NodeTransformation = pNode->mTransformation;

    const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

    if (pNodeAnim) {
        aiVector3D Scaling;
        CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
        aiMatrix4x4 ScalingM;
        aiMatrix4x4::Scaling(Scaling, ScalingM);

        aiQuaternion RotationQ;
        CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
        aiMatrix4x4 RotationM = aiMatrix4x4(RotationQ.GetMatrix());

        aiVector3D Translation;
        CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
        aiMatrix4x4 TranslationM;
        aiMatrix4x4::Translation(Translation, TranslationM);

        NodeTransformation = TranslationM * RotationM * ScalingM;
    }

    aiMatrix4x4 GlobalTransformation = ParentTransform * NodeTransformation;

    if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
        unsigned int BoneIndex = m_BoneMapping[NodeName];
        m_BoneInfo[BoneIndex].FinalTransformation = GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
    }

    for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
        ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
    }
}

void GeometrySkeletalMesh::BoneTransform(float TimeInSeconds, std::vector<aiMatrix4x4>& Transforms)
{
    aiMatrix4x4 Identity;

    float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
    float TimeInTicks = TimeInSeconds * TicksPerSecond;
    float AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);

    ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);

    Transforms.resize(m_NumBones);

    for (unsigned int i = 0; i < m_NumBones; i++) {
        Transforms[i] = m_BoneInfo[i].FinalTransformation;
    }
}
