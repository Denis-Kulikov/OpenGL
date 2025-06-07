#include <object/geometry/mesh.hpp>


GeometryMesh::GeometryMesh(const std::string& Filename)
    : buffers(NUM_VBs, 0)
{
    InitBuffers();
    LoadMesh(Filename);
    SetType(MESH);

    glBindVertexArray(0);	
}

void GeometryMesh::Bind() const {
    glBindVertexArray(buffers[VAO]);
}

void GeometryMesh::Draw(void* data) const {
    int index = *static_cast<int*>(data);

    glDrawElementsBaseVertex(GL_TRIANGLES, 
                                m_Entries[index].NumIndices, 
                                GL_UNSIGNED_INT, 
                                (void*)(sizeof(unsigned int) * m_Entries[index].BaseIndex), 
                                m_Entries[index].BaseVertex);
}

void GeometryMesh::InitBuffers() {
    glGenVertexArrays(1, &buffers[0]);   
    glBindVertexArray(buffers[0]);
    glGenBuffers(buffers.size() - 1, buffers.data() + 1);
}

bool GeometryMesh::LoadMesh(const std::string& Filename) {
    Assimp::Importer Importer;
    const aiScene* m_pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals| aiProcess_FlipUVs);

    if (m_pScene) {
        InitFromScene(m_pScene, Filename);
    } else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
        return false;
    }

    return false;
}

bool GeometryMesh::InitFromScene(const aiScene* m_pScene, const std::string& Filename) {
    m_Entries.resize(m_pScene->mNumMeshes);
    m_Textures.resize(m_pScene->mNumMaterials);

    std::vector<unsigned int> Indices;
    std::vector<glm::vec3> Positions;
    std::vector<glm::vec2> TexCoords;
    std::vector<glm::vec3> Normals;
    
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

    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        const aiMesh* paiMesh = m_pScene->mMeshes[i];
        std::cout << m_pScene->mMeshes[i]->mName.C_Str() << std::endl;
        InitMesh(i, paiMesh, Positions, Normals, TexCoords, Indices);
    }

    if (!InitMaterials(m_pScene, Filename.substr(0, Filename.find_last_of("/\\")))) {
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

    return true;
}

void GeometryMesh::InitMesh(unsigned int MeshIndex, const aiMesh* paiMesh,
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

bool GeometryMesh::InitMaterials(const aiScene* scene, const std::string& directory) {
    m_Textures.resize(scene->mNumMaterials);

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
                    m_Textures[i] = Texture::Create(uniqueName, texture);
                } else {
                    // Внешняя текстура
                    std::string fullPath = directory + "/" + texPath;
                    m_Textures[i] = Texture::Create(fullPath, fullPath);
                }
            }
        } else {
            m_Textures[i] = Texture::Find("white");
        }
    }

    return true;
}
