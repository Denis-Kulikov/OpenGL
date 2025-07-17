#include <object/geometry/loader/assimp_mesh_loader.hpp>

std::string printVec3(const glm::vec3& v);
std::string printQuat(const glm::quat& q);
glm::vec3 quatToEuler(const glm::quat& q);

bool AssimpMeshLoader::LoadMesh(const std::string& Filename, GeometryMesh& mesh) {
    Assimp::Importer Importer;
    const aiScene* m_pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals| aiProcess_FlipUVs);

    InitBuffers(mesh);

    if (m_pScene) {
        InitFromScene(mesh, m_pScene, Filename);
    } else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
        return false;
    }

    return false;
}
bool AssimpMeshLoader::LoadMesh(const std::string& Filename, GeometrySkeletalMesh& mesh) {
    Assimp::Importer Importer;
    const aiScene* m_pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals| aiProcess_FlipUVs);

    InitBuffers(mesh);

    if (m_pScene) {
        InitFromScene(mesh, m_pScene, Filename);
    } else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
        return false;
    }

    return false;
}

void AssimpMeshLoader::InitBuffers(GeometryMesh& mesh) {
    auto& buffers = mesh.buffers;
    buffers.resize(GeometryMesh::NUM_VBs, 0);
    glGenVertexArrays(1, &buffers[0]);   
    glBindVertexArray(buffers[0]);
    glGenBuffers(buffers.size() - 1, buffers.data() + 1);
}
void AssimpMeshLoader::InitBuffers(GeometrySkeletalMesh& mesh) {
    auto& buffers = mesh.buffers;
    buffers.resize(GeometrySkeletalMesh::NUM_skeletal_VBs, 0);
    glGenVertexArrays(1, &buffers[0]);   
    glBindVertexArray(buffers[0]);
    glGenBuffers(buffers.size() - 1, buffers.data() + 1);
}

bool AssimpMeshLoader::InitFromScene(GeometryMesh& mesh, const aiScene* m_pScene, const std::string& Filename) {
    mesh.m_Entries.resize(m_pScene->mNumMeshes);
    mesh.m_Textures.resize(m_pScene->mNumMaterials);

    std::vector<unsigned int> Indices;
    std::vector<glm::vec3> Positions;
    std::vector<glm::vec2> TexCoords;
    std::vector<glm::vec3> Normals;
    
    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;
    
    for (unsigned int i = 0 ; i < mesh.m_Entries.size() ; i++) {
        mesh.m_Entries[i].MaterialIndex = m_pScene->mMeshes[i]->mMaterialIndex;        
        mesh.m_Entries[i].NumIndices    = m_pScene->mMeshes[i]->mNumFaces * 3;
        mesh.m_Entries[i].BaseVertex    = NumVertices;
        mesh.m_Entries[i].BaseIndex     = NumIndices;
        
        NumVertices += m_pScene->mMeshes[i]->mNumVertices;
        NumIndices  += mesh.m_Entries[i].NumIndices;
    }

    Indices.reserve(NumIndices);
    Positions.reserve(NumVertices);
    TexCoords.reserve(NumVertices);
    Normals.reserve(NumVertices);

    for (unsigned int i = 0 ; i < mesh.m_Entries.size() ; i++) {
        const aiMesh* paiMesh = m_pScene->mMeshes[i];
        std::cout << m_pScene->mMeshes[i]->mName.C_Str() << std::endl;
        InitMesh(mesh, i, paiMesh, Positions, Normals, TexCoords, Indices);
    }

    if (!InitMaterials(mesh, m_pScene, Filename.substr(0, Filename.find_last_of("/\\")))) {
        return false;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.buffers[GeometryMesh::EBO]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[GeometryMesh::POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);    

    glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[GeometryMesh::TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

   	glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[GeometryMesh::NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    return true;
}

void AssimpMeshLoader::InitMesh(GeometryMesh& mesh,unsigned int MeshIndex, const aiMesh* paiMesh,
                std::vector<glm::vec3>& Positions, std::vector<glm::vec3>& Normals,
                std::vector<glm::vec2>& TexCoords, std::vector<unsigned int>& Indices)
{
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    
    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        Positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
        Normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
        TexCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));        
    }
    
    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }
}

bool AssimpMeshLoader::InitMaterials(GeometryMesh& mesh, const aiScene* scene, const std::string& directory) {
    mesh.m_Textures.resize(scene->mNumMaterials);

    for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
        const aiMaterial* mat = scene->mMaterials[i];

        // Используем только первый диффузный слот
        if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString path;
            if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
                std::string texPath = path.C_Str();

                // Встроенная текстура (embedded) начинается с '*'
                if (texPath[0] == '*') {
                    int texIndex = std::stoi(texPath.substr(1));
                    aiTexture* texture = scene->mTextures[texIndex];

                    std::string uniqueName = directory + "/embedded_" + std::to_string(texIndex);
                    mesh.m_Textures[i] = Texture::Create(uniqueName, texture);
                } else {
                    // Внешняя текстура
                    std::string fullPath = directory + "/" + texPath;
                    mesh.m_Textures[i] = Texture::Create(fullPath, fullPath);
                }
            }
        } else {
            mesh.m_Textures[i] = Texture::Find("white");
        }
    }

    return true;
}


bool AssimpMeshLoader::InitFromScene(GeometrySkeletalMesh& mesh, const aiScene* m_pScene, const std::string& Filename) {
    mesh.m_Entries.resize(m_pScene->mNumMeshes);
    mesh.m_Textures.resize(m_pScene->mNumMaterials);

    std::vector<unsigned int> Indices;
    std::vector<glm::vec3> Positions;
    std::vector<glm::vec2> TexCoords;
    std::vector<glm::vec3> Normals;
    std::vector<VertexBoneData> Bones;
    
    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;
    
    for (unsigned int i = 0 ; i < mesh.m_Entries.size() ; i++) {
        mesh.m_Entries[i].MaterialIndex = m_pScene->mMeshes[i]->mMaterialIndex;        
        mesh.m_Entries[i].NumIndices    = m_pScene->mMeshes[i]->mNumFaces * 3;
        mesh.m_Entries[i].BaseVertex    = NumVertices;
        mesh.m_Entries[i].BaseIndex     = NumIndices;
        
        NumVertices += m_pScene->mMeshes[i]->mNumVertices;
        NumIndices  += mesh.m_Entries[i].NumIndices;
    }

    Indices.reserve(NumIndices);
    Positions.reserve(NumVertices);
    TexCoords.reserve(NumVertices);
    Normals.reserve(NumVertices);
    Bones.resize(NumVertices);

    for (unsigned int i = 0 ; i < mesh.m_Entries.size() ; i++) {
        const aiMesh* paiMesh = m_pScene->mMeshes[i];
        InitMesh(mesh, i, paiMesh, Positions, Normals, TexCoords, Indices);
        LoadBones(mesh, i, paiMesh, Bones);
    }

    if (!InitMaterials(mesh, m_pScene, Filename.substr(0, Filename.find_last_of("/\\")))) {
        return false;
    }

    mesh.skeleton.BuildBoneTree(m_pScene->mRootNode);

    if (!LoadAnimations(mesh, m_pScene)) {
        return false;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.buffers[GeometrySkeletalMesh::EBO]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[GeometrySkeletalMesh::POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);    

    glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[GeometrySkeletalMesh::TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[GeometrySkeletalMesh::NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[GeometrySkeletalMesh::BONE_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);    
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

    return true;
}

void AssimpMeshLoader::LoadBones(GeometrySkeletalMesh& mesh, unsigned int MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones) {
    auto NumBones = pMesh->mNumBones;
    mesh.skeleton.BoneMap.reserve(pMesh->mNumBones);
    mesh.skeleton.BoneLocal.resize(pMesh->mNumBones, glm::mat4(1.0f));
    mesh.skeleton.inverseBind.resize(pMesh->mNumBones, {glm::quat(1, 0, 0, 0), glm::quat(0, 0, 0, 0)});

    for (unsigned int i = 0; i < NumBones; i++) {
        std::string BoneName(pMesh->mBones[i]->mName.data);
        std::string UniqueName = BoneName;
        int Index;

        int suffix = 1;
        while (mesh.skeleton.BoneMap.find(UniqueName) != mesh.skeleton.BoneMap.end()) {
            UniqueName = BoneName + "_" + std::to_string(suffix++);
        }

        mesh.skeleton.BoneMap[UniqueName] = i;
        auto m = pMesh->mBones[i]->mOffsetMatrix;
        mesh.skeleton.BoneLocal[i] = glm::mat4(
            m.a1, m.b1, m.c1, m.d1,
            m.a2, m.b2, m.c2, m.d2,
            m.a3, m.b3, m.c3, m.d3,
            m.a4, m.b4, m.c4, m.d4
        );

        glm::quat q_real = glm::normalize(glm::quat_cast(mesh.skeleton.BoneLocal[i]));
        glm::vec3 t = glm::vec3(mesh.skeleton.BoneLocal[i][3]);
        glm::quat t_quat(0, t.x, t.y, t.z);
        glm::quat q_dual = 0.5f * t_quat * q_real;

        mesh.skeleton.inverseBind[i] = {q_real, q_dual};

        for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
            unsigned int VertexID = mesh.m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
            float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
            Bones[VertexID].AddBoneData(i, Weight);
        }

        for (auto& it : Bones) {
            it.NormalizeWeights();
        }
    }
}

bool AssimpMeshLoader::LoadAnimations(GeometrySkeletalMesh& mesh, const aiScene* scene) {
    if (!scene || !scene->HasAnimations()) {
        return false;
    }

    mesh.skeleton.AnimationMap.clear();

    for (unsigned int i = 0; i < scene->mNumAnimations; ++i) {
        const aiAnimation* anim = scene->mAnimations[i];
        std::string animName = anim->mName.C_Str();
        if (animName.empty()) {
            animName = "Animation_" + std::to_string(i);
        }
        // Вывод загруженных анимаций
        // std::cout << "GeometrySkeletalMesh::LoadAnimations:" << animName << std::endl; 

        mesh.skeleton.AnimationMap.emplace(std::move(animName), SkeletalAnimation(anim, mesh.skeleton.BoneMap));
    }

    return true;
}