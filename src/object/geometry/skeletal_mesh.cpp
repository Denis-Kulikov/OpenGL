#include <object/geometry/skeletal_mesh.hpp>

std::string printVec3(const glm::vec3& v);
std::string printQuat(const glm::quat& q);
glm::vec3 quatToEuler(const glm::quat& q);


GeometrySkeletalMesh::GeometrySkeletalMesh(const std::string& Filename) 
    : GeometryMesh()
{
    buffers.resize(NUM_skeletal_VBs, 0);
    InitBuffers();
    LoadMesh(Filename);
    SetType(SKELETAL_MESH);

    glBindVertexArray(0);	
}

const Skeleton& GeometrySkeletalMesh::GetSkeleton() const {
    return skeleton;
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

    skeleton.BuildBoneTree(m_pScene->mRootNode);

    if (!LoadAnimations(m_pScene)) {
        return false;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[EBO]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[POSITION_VB]);
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
    auto NumBones = pMesh->mNumBones;
    skeleton.BoneMap.reserve(pMesh->mNumBones);
    skeleton.BoneLocal.resize(pMesh->mNumBones, glm::mat4(1.0f));
    skeleton.inverseBind.resize(pMesh->mNumBones, {glm::quat(1, 0, 0, 0), glm::quat(0, 0, 0, 0)});

    for (unsigned int i = 0; i < NumBones; i++) {
        std::string BoneName(pMesh->mBones[i]->mName.data);
        std::string UniqueName = BoneName;
        int Index;

        int suffix = 1;
        while (skeleton.BoneMap.find(UniqueName) != skeleton.BoneMap.end()) {
            UniqueName = BoneName + "_" + std::to_string(suffix++);
        }

        skeleton.BoneMap[UniqueName] = i;
        auto m = pMesh->mBones[i]->mOffsetMatrix;
        skeleton.BoneLocal[i] = glm::mat4(
            m.a1, m.b1, m.c1, m.d1,
            m.a2, m.b2, m.c2, m.d2,
            m.a3, m.b3, m.c3, m.d3,
            m.a4, m.b4, m.c4, m.d4
        );

        glm::quat q_real = glm::normalize(glm::quat_cast(skeleton.BoneLocal[i]));
        glm::vec3 t = glm::vec3(skeleton.BoneLocal[i][3]);
        glm::quat t_quat(0, t.x, t.y, t.z);
        glm::quat q_dual = 0.5f * t_quat * q_real;

        skeleton.inverseBind[i] = {q_real, q_dual};

        // std::cout << printVec3(skeleton.BoneLocal[i][3]) << " | " << printQuat(q_dual) << std::endl;

        for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
            unsigned int VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
            float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
            Bones[VertexID].AddBoneData(i, Weight);
        }
    }
}

bool GeometrySkeletalMesh::LoadAnimations(const aiScene* scene) {
    if (!scene || !scene->HasAnimations()) {
        return false;
    }

    skeleton.AnimationMap.clear();

    for (unsigned int i = 0; i < scene->mNumAnimations; ++i) {
        const aiAnimation* anim = scene->mAnimations[i];
        std::string animName = anim->mName.C_Str();
        if (animName.empty()) {
            animName = "Animation_" + std::to_string(i);
        }
        std::cout << "GeometrySkeletalMesh::LoadAnimations:" << animName << std::endl; 

        skeleton.AnimationMap.emplace(std::move(animName), SkeletalAnimation(anim, skeleton.BoneMap));
    }

    return true;
}

GeometrySkeletalMesh* GeometrySkeletalMesh::Create(const std::string& name, const std::string& path) {
    auto [it, inserted] = cache.try_emplace(name, path);
    return &it->second;
}
GeometrySkeletalMesh* GeometrySkeletalMesh::Find(const std::string& name) {
    auto it = cache.find(name);
    return it != cache.end() ? &it->second : nullptr;
}

void GeometrySkeletalMesh::Delete(const std::string& path) {
    auto it = cache.find(path);
    if (it != cache.end()) {
        cache.erase(it); 
    }
}

void GeometrySkeletalMesh::ClearСache() {
    for (auto it = cache.begin(); it != cache.end(); ) {
        it = cache.erase(it);
    }
}
