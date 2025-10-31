#include <object/mesh/loader/assimp_mesh_loader.hpp>
#include <object/material/material.hpp>
#include <object/material/texture.hpp>
#include <object/material/texture_unit.hpp>

std::string printVec3(const glm::vec3& v);
std::string printQuat(const glm::quat& q);
glm::vec3 quatToEuler(const glm::quat& q);

bool AssimpMeshLoader::LoadMesh(const std::string& fileName, MeshData& mesh) {
    Assimp::Importer Importer;
    const aiScene* m_pScene = Importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals| aiProcess_FlipUVs);

    if (m_pScene) {
        InitFromScene(mesh, m_pScene, fileName);
    } else {
        printf("Error parsing '%s': '%s'\n", fileName.c_str(), Importer.GetErrorString());
        return false;
    }

    return false;
}
bool AssimpMeshLoader::LoadMesh(const std::string& fileName, SkeletalMeshData& mesh) {
    Assimp::Importer Importer;
    const aiScene* m_pScene = Importer.ReadFile(fileName.c_str(),
    aiProcess_Triangulate | aiProcess_GenSmoothNormals| aiProcess_FlipUVs);

    if (m_pScene) {
        InitFromScene(mesh, m_pScene, fileName);
    } else {
        printf("Error parsing '%s': '%s'\n", fileName.c_str(), Importer.GetErrorString());
        return false;
    }

    return false;
}

std::vector<int> AssimpMeshLoader::InitVertexes(MeshData& mesh, const aiScene* m_pScene) {
    std::vector<int> MaterialIndex(m_pScene->mNumMeshes);
    mesh.m_Entries.resize(m_pScene->mNumMeshes);

    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;
    
    for (unsigned int i = 0 ; i < mesh.m_Entries.size() ; i++) {
        mesh.m_Entries[i].NumIndices = m_pScene->mMeshes[i]->mNumFaces * 3;
        mesh.m_Entries[i].BaseVertex = NumVertices;
        mesh.m_Entries[i].BaseIndex  = NumIndices;
        
        NumVertices += m_pScene->mMeshes[i]->mNumVertices;
        NumIndices  += mesh.m_Entries[i].NumIndices;
    
        MaterialIndex[i] = m_pScene->mMeshes[i]->mMaterialIndex;        
    }

    mesh.Indices.reserve(NumIndices);
    mesh.Positions.reserve(NumVertices);
    mesh.TexCoords.reserve(NumVertices);
    mesh.Normals.reserve(NumVertices);

    return MaterialIndex;
}
std::vector<int> AssimpMeshLoader::InitVertexes(SkeletalMeshData& mesh, const aiScene* m_pScene) {
    std::vector<int> MaterialIndex(m_pScene->mNumMeshes);
    mesh.m_Entries.resize(m_pScene->mNumMeshes);
    mesh.skeleton = std::make_shared<Skeleton>();

    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;
    unsigned int NumBones = 0;
    
    for (unsigned int i = 0 ; i < mesh.m_Entries.size() ; i++) {
        mesh.m_Entries[i].NumIndices = m_pScene->mMeshes[i]->mNumFaces * 3;
        mesh.m_Entries[i].BaseVertex = NumVertices;
        mesh.m_Entries[i].BaseIndex  = NumIndices;
        
        NumVertices += m_pScene->mMeshes[i]->mNumVertices;
        NumIndices  += mesh.m_Entries[i].NumIndices;
        NumBones    += m_pScene->mMeshes[i]->mNumBones;
    
        MaterialIndex[i] = m_pScene->mMeshes[i]->mMaterialIndex;        
    }

    mesh.Indices.reserve(NumIndices);
    mesh.Positions.reserve(NumVertices);
    mesh.TexCoords.reserve(NumVertices);
    mesh.Normals.reserve(NumVertices);
    mesh.Bones.resize(NumVertices);

    mesh.skeleton.get()->BoneMap.reserve(NumBones);
    mesh.skeleton.get()->inverseBindMat.resize(NumBones, glm::mat4(1.0f));
    mesh.skeleton.get()->inverseBindDQ.resize(NumBones, {glm::quat(1, 0, 0, 0), glm::quat(0, 0, 0, 0)});
    
std::cout << "NumBones: " << NumBones << std::endl; 

    return MaterialIndex;
}

bool AssimpMeshLoader::InitFromScene(MeshData& mesh, const aiScene* m_pScene, const std::string& fileName) {
    std::vector<int> MaterialIndex(InitVertexes(mesh, m_pScene));

    for (unsigned int i = 0 ; i < mesh.m_Entries.size() ; i++) {
        const aiMesh* paiMesh = m_pScene->mMeshes[i];
        InitMesh(mesh, i, paiMesh);
    }

    if (!InitMaterials(mesh, MaterialIndex, m_pScene, fileName)) {
        return false;
    }

    return true;
}

bool AssimpMeshLoader::InitFromScene(SkeletalMeshData& mesh, const aiScene* m_pScene, const std::string& fileName) {
    std::vector<int> MaterialIndex(InitVertexes(mesh, m_pScene));

    std::cout << "mesh.m_Entries.size(): " << mesh.m_Entries.size() << std::endl;
    for (unsigned int i = 0 ; i < mesh.m_Entries.size() ; i++) {
        const aiMesh* paiMesh = m_pScene->mMeshes[i];
        InitMesh(mesh, i, paiMesh);
        LoadBones(mesh, i, paiMesh);
    }

    if (!InitMaterials(mesh, MaterialIndex, m_pScene, fileName)) {
        return false;
    }

    mesh.skeleton.get()->BuildBoneTree(m_pScene->mRootNode);

    if (!LoadAnimations(mesh, m_pScene)) {
        return false;
    }

    return true;
}

void AssimpMeshLoader::InitMesh(MeshData& mesh,unsigned int MeshIndex, const aiMesh* paiMesh) {
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    auto& Positions  = mesh.Positions;
    auto& Normals    = mesh.Normals;
    auto& TexCoords  = mesh.TexCoords;
    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        Positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
        Normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
        TexCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));        
    }
    
    auto& Indices = mesh.Indices;
    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }
}

#include <locale>
#include <codecvt>
std::wstring utf8_to_wstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myConv;
    return myConv.from_bytes(str);
}

bool AssimpMeshLoader::InitMaterials(MeshData& mesh, std::vector<int>& MaterialIndex, const aiScene* scene, const std::string& fileName) {
    std::vector<std::shared_ptr<Material>> Materials;
    Materials.reserve(scene->mNumMaterials);
    aiString path;
    std::string texPath;

    auto createTexture = [&]() -> Texture* {
        if (texPath[0] == '*') { // Втроенная текстура
            int texIndex = std::stoi(texPath.substr(1));
            aiTexture* texture = scene->mTextures[texIndex];
            std::string uniqueName = fileName + "/embedded_" + std::to_string(texIndex);
            return Texture::Create(uniqueName, texture);
        }
        auto directory = fileName.substr(0, fileName.find_last_of("/\\")); // Внешняя текстура
        std::string fullPath = directory + "/" + texPath;
        return Texture::Create(fullPath, fullPath);
    };

    std::cout << "mNumMaterials: " << scene->mNumMaterials << std::endl;
    std::cout << "scene->mNumTextures: " << scene->mNumTextures << std::endl;
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
        Materials.push_back(std::make_shared<Material>());
        const aiMaterial* mat = scene->mMaterials[i];

        if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
                texPath = path.C_Str();
                std::cout << "Texture Path: " << texPath << "\n" << std::endl;
                Texture* t = createTexture();
                Materials[i]->textureUnits.emplace_back(t, TextureUnit::ALBEDO);
            }
        }

        if (mat->GetTextureCount(aiTextureType_EMISSIVE) > 0) {
            if (mat->GetTexture(aiTextureType_EMISSIVE, 0, &path) == AI_SUCCESS) {
                texPath = path.C_Str();
                std::cout << "Texture Path (EMISSIVE): " << texPath << "\n" << std::endl;
                Texture* t = createTexture();
                Materials[i]->textureUnits.emplace_back(t, TextureUnit::ALBEDO);
            }
        }

        // ...
    }

    for (size_t i = 0; i < mesh.m_Entries.size(); ++i) {
        mesh.m_Entries[i].material = Materials[MaterialIndex[i]];
    }
    
    return true;
}

void AssimpMeshLoader::LoadBones(SkeletalMeshData& mesh, unsigned int MeshIndex, const aiMesh* pMesh) {
    auto NumBones = pMesh->mNumBones;

    std::cout << "Found " << NumBones << " bones in mesh" << std::endl;

    for (unsigned int i = 0; i < NumBones; i++) {
        std::string BoneName(pMesh->mBones[i]->mName.data);
        std::string UniqueName = BoneName;

        int suffix = 1;
        while (mesh.skeleton.get()->BoneMap.find(UniqueName) != mesh.skeleton.get()->BoneMap.end()) {
            UniqueName = BoneName + "_" + std::to_string(suffix++);
        }

        mesh.skeleton.get()->BoneMap[UniqueName] = i;
        auto m = pMesh->mBones[i]->mOffsetMatrix;
        mesh.skeleton.get()->inverseBindMat[i] = glm::mat4(
            m.a1, m.b1, m.c1, m.d1,
            m.a2, m.b2, m.c2, m.d2,
            m.a3, m.b3, m.c3, m.d3,
            m.a4, m.b4, m.c4, m.d4
        );

        glm::quat q_real = glm::normalize(glm::quat_cast(mesh.skeleton.get()->inverseBindMat[i]));
        glm::vec3 t = glm::vec3(mesh.skeleton.get()->inverseBindMat[i][3]);
        glm::quat t_quat(0, t.x, t.y, t.z);
        glm::quat q_dual = 0.5f * t_quat * q_real;

        mesh.skeleton.get()->inverseBindDQ[i] = {q_real, q_dual};

        for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
            unsigned int VertexID = mesh.m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
            float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
            mesh.Bones[VertexID].AddBoneData(i, Weight);
        }
    }
}

bool AssimpMeshLoader::LoadAnimations(SkeletalMeshData& mesh, const aiScene* scene) {
    if (!scene) {
        return false;
    }

    mesh.skeleton.get()->AnimationMap.clear();

    for (unsigned int i = 0; i < scene->mNumAnimations; ++i) {
        const aiAnimation* anim = scene->mAnimations[i];
        std::string animName = anim->mName.C_Str();
        if (animName.empty()) {
            animName = "Animation_" + std::to_string(i);
        }

        mesh.skeleton.get()->AnimationMap.emplace(std::move(animName), SkeletalAnimation(anim, mesh.skeleton.get()->BoneMap));
    }

    return true;
}