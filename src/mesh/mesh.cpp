#include <execution>
#include <game/gameManager.hpp>
#include <mesh/mesh.hpp>
#include <object/cube.hpp>
#include <stb_image.h>

GLuint CreateTextureFromCompressedPng(const aiTexture* texture) {
    // Декодирование PNG данных, которые хранятся в памяти
    int width, height, channels;
    
    // stbi_load_from_memory: декодирует данные PNG, хранящиеся в буфере памяти
    unsigned char* data = stbi_load_from_memory(
        reinterpret_cast<const unsigned char*>(texture->pcData), 
        texture->mWidth, 
        &width, &height, &channels, 0
    );

    if (data == nullptr) {
        std::cerr << "Failed to load PNG from memory: " << stbi_failure_reason() << std::endl;
        return 0;
    }

    // Определение формата (RGB или RGBA)
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    // Генерация текстуры OpenGL
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Передача декодированных данных в OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    // Настройка параметров текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    // Освобождение декодированных данных
    stbi_image_free(data);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}


void Mesh::VertexBoneData::normalize ()
{
    float   sum = Weights [0];
    
    for ( int i = 1; i < size; i++ )
        sum += Weights [i];
        
    for ( int i = 0; i < size; i++ )
        Weights [i] /= sum;
}

void Mesh::VertexBoneData::AddBoneData(unsigned int BoneID, float Weight)
{
    for (unsigned int i = 0 ; i < size ; i++) {
        if (Weights[i] == 0.0) {
            IDs[i]     = BoneID;
            Weights[i] = Weight;
            return;
        }
    }
 
    std::cout << "Костей больше, чем мы рассчитывалось." << std::endl;
    assert(0);
}

stb_img_struct::stb_img_struct(const std::string& Filename) {
    img = stbi_load(Filename.c_str(), &x, &y, &n, 0);
    if (img == nullptr)
        std::cerr << "Failed to load img: " << Filename << std::endl;
    
}
stb_img_struct::~stb_img_struct() {
    if (img != nullptr)
        stbi_image_free(img);
}


Texture::Texture(const stb_img& img)
{
    if (img.img == nullptr)
        std::cerr << "Failed to create texture" << std::endl;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    GLenum format = (img.n == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, img.x, img.y, 0, format, GL_UNSIGNED_BYTE, img.img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
    }
}

Texture::Texture(const stb_img& img, const std::string &n)
    : Texture(img)
{
    name = n;
}


Mesh::MeshEntry::MeshEntry()
{
    NumIndices = 0;
    MaterialIndex = INVALID_MATERIAL;
    BaseVertex = 0;
    BaseIndex = 0;
}

Mesh::MeshEntry::~MeshEntry() {}

Mesh::Mesh(const std::string& Filename)
{
    LoadMesh(Filename);
    compileShaders("assets\\model\\shaders\\model_fs.glsl", "assets\\model\\shaders\\model_vs.glsl");
}

Mesh::~Mesh() {}

void Mesh::Clear()
{
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
}


unsigned int Mesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{    
    for (unsigned int i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }
    
    assert(0);

    return 0;
}

unsigned int Mesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (unsigned int i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }
    
    assert(0);

    return 0;
}

unsigned int Mesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);
    
    for (unsigned int i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }
    
    assert(0);

    return 0;
}


void Mesh::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
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

void Mesh::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
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

void Mesh::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
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


const aiNodeAnim* Mesh::FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
{
    for (unsigned int i = 0 ; i < pAnimation->mNumChannels ; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
        
        if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
            return pNodeAnim;
        }
    }
    
    return NULL;
}

void Mesh::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform)
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

void Mesh::BoneTransform(float TimeInSeconds, std::vector<aiMatrix4x4>& Transforms)
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

void Mesh::LoadBones(unsigned int MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones)
{
    for (unsigned int i = 0; i < pMesh->mNumBones; i++) {
        unsigned int BoneIndex = 0;
        std::string BoneName(pMesh->mBones[i]->mName.data);
 
        if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
            BoneIndex = m_NumBones;
            m_NumBones++;
            BoneInfo bi;
            m_BoneInfo.push_back(bi);
        } else {
            BoneIndex = m_BoneMapping[BoneName];
        }

        m_BoneMapping[BoneName] = BoneIndex;
        m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;

        for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
            unsigned int VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
            float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
            Bones[VertexID].AddBoneData(BoneIndex, Weight);
        }
    }
    for (unsigned int i = 0; i < pMesh->mNumBones; i++)
        Bones[i].normalize();
}

void recursive_search(aiNode *node, int deep) {
    for (int i = 0; i < deep; i++)
        std::cout << "  ";
    std::cout << node->mName.C_Str() << std::endl;
    for (int i = 0; i < node->mNumChildren; i++)
        recursive_search(node->mChildren[i], deep + 1);
}

bool Mesh::LoadMesh(const std::string& Filename)
{
    Clear();

    glGenVertexArrays(1, &m_VAO);   
    glBindVertexArray(m_VAO);

    glGenBuffers((sizeof(m_Buffers)/sizeof(m_Buffers[0])), m_Buffers);

    m_pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals| aiProcess_FlipUVs);

    if (m_pScene) {
        InitFromScene(m_pScene, Filename);
    } else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
        return false;
    }

    glBindVertexArray(0);	

    // std::cout << m_pScene->mRootNode->mName.C_Str();
    // for (int i = 0; i < m_pScene->mRootNode->mNumChildren; i++)
    //     recursive_search(m_pScene->mRootNode->mChildren[i], 1);

    assert(m_pScene->HasAnimations());

    return false;
}

GLuint Mesh::loadShader(const std::string &shaderPath, GLuint type)
{
    std::ifstream shaderFile(shaderPath);
    if (!shaderFile.is_open()) {
        std::cerr << "Error: Could not open shader file '" << shaderPath << "'" << std::endl;
        return 0;
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    std::string shaderCode = shaderStream.str();
    const GLchar* shaderCodePtr = shaderCode.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCodePtr, NULL);
    glCompileShader(shader);

    GLint ok;
    GLchar log[2000];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(shader, 2000, NULL, log);
        std::cout << "Shader(" << shaderPath << "): " << log << '\n' << shaderCode << std::endl;
    }

    return shader;
}

void Mesh::compileShaders(const std::string &FS, const std::string &VS)
{
    if (shaderProgram == 0) shaderProgram = glCreateProgram();

    GLuint fragmentShader;
    GLuint vertexShader;

    fragmentShader = loadShader(FS, GL_FRAGMENT_SHADER);
    vertexShader = loadShader(VS, GL_VERTEX_SHADER);
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, vertexShader);
    glLinkProgram(shaderProgram);

    GLint ok;
    GLchar log[2000];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &ok);
    if (!ok) {
        glGetProgramInfoLog(shaderProgram, 2000, NULL, log);
        std::cout << "Shader " << shaderProgram << " compilation Log:\n" << log << std::endl;
    
        GLint infoLogLength;
        GLchar *infoLog;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        infoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(fragmentShader, infoLogLength, NULL, infoLog);
        std::cout << "Shader fragmentShader Log:\n" << infoLog << std::endl;
        delete[] infoLog;

        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        infoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(vertexShader, infoLogLength, NULL, infoLog);
        std::cout << "Shader vertexShader Log:\n" << infoLog << std::endl;
        delete[] infoLog;

        std::cout << std::endl;
    }

    gWorldLocation                = glGetUniformLocation(shaderProgram, "gWorld");
    gTextureSamplerLocation       = glGetUniformLocation(shaderProgram, "gTextureSampler");
    gNormalMapLocation            = glGetUniformLocation(shaderProgram, "gNormalMap");
    gEyeWorldPosLocation          = glGetUniformLocation(shaderProgram, "gEyeWorldPos");
    gColorLocation                = glGetUniformLocation(shaderProgram, "gDirectionalLight.Color");
    gDirectionLocation            = glGetUniformLocation(shaderProgram, "gDirectionalLight.Direction");
    gAmbientIntensityLocation     = glGetUniformLocation(shaderProgram, "gDirectionalLight.AmbientIntensity");
    gDiffuseIntensityLocation     = glGetUniformLocation(shaderProgram, "gDirectionalLight.DiffuseIntensity");
    gMatSpecularIntensityLocation = glGetUniformLocation(shaderProgram, "gMatSpecularIntensity");
    gSpecularPowerLocation        = glGetUniformLocation(shaderProgram, "gSpecularPower");

    gBones          = glGetUniformLocation(shaderProgram, "gBones");
    gProjection     = glGetUniformLocation(shaderProgram, "Projection");
    gView           = glGetUniformLocation(shaderProgram, "View");
}


bool Mesh::InitFromScene(const aiScene* m_pScene, const std::string& Filename)
{
    m_Entries.resize(m_pScene->mNumMeshes);
    m_Textures.resize(m_pScene->mNumMaterials);

    std::vector<glm::vec3> Positions;
    std::vector<glm::vec3> Normals;
    std::vector<glm::vec2> TexCoords;
    std::vector<VertexBoneData> Bones;
    std::vector<unsigned int> Indices;
       
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

    Positions.reserve(NumVertices);
    Normals.reserve(NumVertices);
    TexCoords.reserve(NumVertices);
    Bones.resize(NumVertices);
    Indices.reserve(NumIndices);

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        const aiMesh* paiMesh = m_pScene->mMeshes[i];
        std::cout << m_pScene->mMeshes[i]->mName.C_Str() << std::endl;
        InitMesh(i, paiMesh, Positions, Normals, TexCoords, Bones, Indices);
    }

    if (!InitMaterials(m_pScene, Filename)) {
        return false;
    }

  	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);    

   	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);

    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);    
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> Total = end - start;
    std::cout << "Init model time: " << Total.count() << std::endl; 

    return true;
}

void Mesh::InitMesh(unsigned int MeshIndex, const aiMesh* paiMesh,
                    std::vector<glm::vec3>& Positions, std::vector<glm::vec3>& Normals,
                    std::vector<glm::vec2>& TexCoords, std::vector<VertexBoneData>& Bones,
                    std::vector<unsigned int>& Indices)
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
    
    LoadBones(MeshIndex, paiMesh, Bones);
    
    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }
}

bool Mesh::InitMaterials(const aiScene* m_pScene, const std::string& Filename)
{
    std::string::size_type SlashIndex = Filename.find_last_of("/");
    std::string Dir;
    bool Ret = true;

    if (SlashIndex == std::string::npos)
        Dir = ".";
    else if (SlashIndex == 0)
        Dir = "/";
    else
        Dir = Filename.substr(0, SlashIndex);

    std::vector<GLuint> textures(m_pScene->mNumTextures); // Загрузка текстур
    if (m_pScene->mNumTextures > 0) {
        for (unsigned int j = 0; j < m_pScene->mNumTextures; ++j) {
            aiTexture* texture = m_pScene->mTextures[j];

            if (texture->mHeight == 0) {
                std::cout << "Compressed texture found: " << texture->achFormatHint << std::endl;

                const char* data = reinterpret_cast<const char*>(texture->pcData);
                size_t dataSize = texture->mWidth;

                textures[j] = CreateTextureFromCompressedPng(texture);
            } else {
                std::cout << "Uncompressed texture found" << std::endl;

                unsigned char* pixels = reinterpret_cast<unsigned char*>(texture->pcData);
                int width = texture->mWidth;
                int height = texture->mHeight;

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            }
        }
    }

    for (unsigned int i = 0; i < m_pScene->mNumMaterials; i++) { // Загрузка текстур в материал
        const aiMaterial* pMaterial = m_pScene->mMaterials[i];

        aiString Path;
        if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path) == AI_SUCCESS) {
            // std::cout << "Diffuse texture path: " << Path.C_Str() << std::endl;
            GLuint textureID;
            if (Path.data[0] == '*') {
                int textureIndex = atoi(Path.C_Str() + 1);
                textureID = textures[textureIndex];
            } else {
                // textureID = LoadTextureFromFile(Path.C_Str());
            }

            m_Textures[i].dif = new Texture(textureID);
        }
    }

    return Ret;
}

void Mesh::push_uniforms() {
    GLfloat color[3] = { 0.8, 0.8, 0.9 };
    GLfloat direction[3] = { -0.4, -1.0, 0.2 };
    GLfloat AmbientIntensity = 0.9;
    GLfloat DiffuseIntensity = 0.35;
    GLfloat SpecularPower = 0.25;
    GLfloat MatSpecularIntensity = 0.25;
    glm::vec3 EyesPos = GameManager::callbackData.camera->GetPosition();

    glUniform3f(gColorLocation, color[0], color[1], color[2]);
    glUniform3f(gDirectionLocation, direction[0], direction[1], direction[2]);
    glUniform3f(gEyeWorldPosLocation, EyesPos[0], EyesPos[1], EyesPos[2]);
    glUniform1f(gAmbientIntensityLocation, AmbientIntensity);
    glUniform1f(gDiffuseIntensityLocation, DiffuseIntensity);
    glUniform1f(gSpecularPowerLocation, SpecularPower);
    glUniform1f(gMatSpecularIntensityLocation, MatSpecularIntensity);
}

void Mesh::set_transform(const objectTransform &transform) {
    if (!m_pScene || !m_pScene->mRootNode)
        return;

    aiMatrix4x4 TranslationMatrix;
    aiMatrix4x4 RotationMatrixX, RotationMatrixY, RotationMatrixZ;
    aiMatrix4x4 scaleMatrix;

    aiMatrix4x4::Translation(aiVector3D(transform.WorldPos.x, transform.WorldPos.y, transform.WorldPos.z), TranslationMatrix);

    aiMatrix4x4::RotationX(ToRadian(transform.Rotate.x), RotationMatrixX);
    aiMatrix4x4::RotationY(ToRadian(transform.Rotate.y), RotationMatrixY);
    aiMatrix4x4::RotationZ(ToRadian(transform.Rotate.z), RotationMatrixZ);

    aiMatrix4x4::Scaling(aiVector3D(transform.GetScale().x, transform.GetScale().y, transform.GetScale().z), scaleMatrix);

    m_pScene->mRootNode->mTransformation = TranslationMatrix * RotationMatrixZ * RotationMatrixY * RotationMatrixX * scaleMatrix;
}

void Mesh::Render(std::vector<aiMatrix4x4> *Transforms)
{
    glBindVertexArray(m_VAO);
    glUseProgram(shaderProgram);
    push_uniforms();

    glUniformMatrix4fv(gProjection, 1, GL_TRUE, &GameManager::render->PersProjTrans);
    glUniformMatrix4fv(gView, 1, GL_TRUE, &GameManager::render->View);

    for (unsigned int i = 0; i < m_NumBones; i++) 
        glUniformMatrix4fv(gBones + i, 1, GL_TRUE, &(*Transforms)[i][0][0]);

    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

        assert(MaterialIndex < m_Textures.size());
        
        if (m_Textures[MaterialIndex].dif) {
            glBindTexture(GL_TEXTURE_2D, m_Textures[MaterialIndex].dif->texture);
        }

		glDrawElementsBaseVertex(GL_TRIANGLES, 
                                 m_Entries[i].NumIndices, 
                                 GL_UNSIGNED_INT, 
                                 (void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex), 
                                 m_Entries[i].BaseVertex);
    }
    glBindVertexArray(0);
}
