#include <execution>
#include <game/gameManager.hpp>
#include <mesh/mesh.hpp>
#include <stb_image.h>

#include <object/sphere.hpp>
#include <object/cube.hpp>


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
    compileShaders("assets\\model\\2b\\shaders\\model_fs.glsl", "assets\\model\\2b\\shaders\\model_vs.glsl");
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
        // Interpolating position, rotation and scaling.
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
        m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
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
}


bool Mesh::LoadMesh(const std::string& Filename)
{
    Clear();

    glGenVertexArrays(1, &m_VAO);   
    glBindVertexArray(m_VAO);

    glGenBuffers((sizeof(m_Buffers)/sizeof(m_Buffers[0])), m_Buffers);

    m_pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals| aiProcess_FlipUVs);

    if (m_pScene) {
        m_GlobalInverseTransform = m_pScene->mRootNode->mTransformation;
        m_GlobalInverseTransform.Inverse();

        InitFromScene(m_pScene, Filename);
    } else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
        return false;
    }

    // if (m_pScene->HasAnimations()) {
    //     std::cout << "Found " << m_pScene->mNumAnimations << " animations:\n";
    //     for (unsigned int i = 0; i < m_pScene->mNumAnimations; i++) {
    //         const aiAnimation* animation = m_pScene->mAnimations[i];
    //         std::cout << " - Animation " << i + 1 << ": " << animation->mName.C_Str() << std::endl;
    //     }
    // } else 
    //     std::cout << "No animations found in the model.\n";

    glBindVertexArray(0);	

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

    m_material.push_back(new MaterialUniforms);

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
    gModel           = glGetUniformLocation(shaderProgram, "Model");


    for (unsigned int i = 0 ; i < 1 ; i++) {
        m_material[i]->Color               = glGetUniformLocation(shaderProgram, std::string(("gPointLights[") + std::string("0" + i) + std::string("].Color"           )).c_str());
        m_material[i]->AmbientIntensity    = glGetUniformLocation(shaderProgram, std::string(("gPointLights[") + std::string("0" + i) + std::string("].AmbientIntensity")).c_str());
        m_material[i]->DiffuseIntensity    = glGetUniformLocation(shaderProgram, std::string(("gPointLights[") + std::string("0" + i) + std::string("].DiffuseIntensity")).c_str());
        m_material[i]->Position            = glGetUniformLocation(shaderProgram, std::string(("gPointLights[") + std::string("0" + i) + std::string("].Position"        )).c_str());
        m_material[i]->Atten.Constant      = glGetUniformLocation(shaderProgram, std::string(("gPointLights[") + std::string("0" + i) + std::string("].Constant"  )).c_str());
        m_material[i]->Atten.Linear        = glGetUniformLocation(shaderProgram, std::string(("gPointLights[") + std::string("0" + i) + std::string("].Linear"    )).c_str());
        m_material[i]->Atten.Exp           = glGetUniformLocation(shaderProgram, std::string(("gPointLights[") + std::string("0" + i) + std::string("].Exp"       )).c_str());
    }

    assert(gBones                != 0xFFFFFFFF);
    assert(gProjection                != 0xFFFFFFFF);
    assert(gView                != 0xFFFFFFFF);
    // assert(gModel                != 0xFFFFFFFF);

    // assert(gWorldLocation                != 0xFFFFFFFF);
    assert(gTextureSamplerLocation       != 0xFFFFFFFF);
    assert(gNormalMapLocation            != 0xFFFFFFFF);
    assert(gEyeWorldPosLocation          != 0xFFFFFFFF);
    assert(gColorLocation                != 0xFFFFFFFF);
    assert(gDirectionLocation            != 0xFFFFFFFF);
    assert(gAmbientIntensityLocation     != 0xFFFFFFFF);
    assert(gDiffuseIntensityLocation     != 0xFFFFFFFF);
    assert(gMatSpecularIntensityLocation != 0xFFFFFFFF);
    assert(gSpecularPowerLocation        != 0xFFFFFFFF);

    for (unsigned int i = 0 ; i < 1 ; i++) {
        assert(m_material[i]->Color            != 0xFFFFFFFF);
        assert(m_material[i]->AmbientIntensity != 0xFFFFFFFF);
        assert(m_material[i]->DiffuseIntensity != 0xFFFFFFFF);
        assert(m_material[i]->Position         != 0xFFFFFFFF);
        assert(m_material[i]->Atten.Constant   != 0xFFFFFFFF);
        assert(m_material[i]->Atten.Linear     != 0xFFFFFFFF);
        assert(m_material[i]->Atten.Exp        != 0xFFFFFFFF);
    }
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
    
    // Count the number of vertices and indices
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
    // glEnableVertexAttribArray(BONE_ID_LOCATION);
    // glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    // glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);    
    // glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(BONE_ID_LOCATION + 1);
    glVertexAttribIPointer(BONE_ID_LOCATION + 1, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)(4 * sizeof(unsigned int)));

    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)(4 * sizeof(unsigned int) + 4 * sizeof(float)));
    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION + 1);  // Для остальных 4 Weights
    glVertexAttribPointer(BONE_WEIGHT_LOCATION + 1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)(4 * sizeof(unsigned int) + 8 * sizeof(float)));

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> Total = end - start;
    std::cout << "Init model time: " << Total.count() << std::endl; 

    return true;
}

void Mesh::InitMesh(unsigned int MeshIndex,
                    const aiMesh* paiMesh,
                    std::vector<glm::vec3>& Positions,
                    std::vector<glm::vec3>& Normals,
                    std::vector<glm::vec2>& TexCoords,
                    std::vector<VertexBoneData>& Bones,
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

    if (SlashIndex == std::string::npos)
        Dir = ".";
    else if (SlashIndex == 0)
        Dir = "/";
    else
        Dir = Filename.substr(0, SlashIndex);

    struct textures_names {
        textures_names() {};
        textures_names(std::string dif_, std::string nrm_, std::string spc_, std::string lmp_)
            : dif(dif_), nrm(nrm_), spc(spc_), lmp(lmp_) {};
        std::string dif;
        std::string nrm;
        std::string spc;
        std::string lmp;
    };
    std::unordered_map<std::string, textures_names> textureMap;
    std::ifstream file(Dir + "/texture.txt");

    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << Dir << "/texture.txt" << std::endl;
        return 1;
    }

    std::string key, value, dif, nrm, spc, lmp;
    while (file >> key >> dif >> nrm >> spc >> lmp) {
        textureMap[key] = textures_names(dif, nrm, spc, lmp);
    }

    file.close();
    

    bool Ret = true;

    for (unsigned int i = 0; i< m_pScene->mNumMaterials; i++){
        const aiMaterial* pMaterial = m_pScene->mMaterials[i];

        std::cout << m_pScene->mMeshes[i]->mName.C_Str() << " " << textureMap[m_pScene->mMeshes[i]->mName.C_Str()].dif << std::endl;
        stb_img dif(Dir + "/texture/" + textureMap[m_pScene->mMeshes[i]->mName.C_Str()].dif);
        if (dif.img != nullptr) {
            m_Textures[i].dif = new Texture(dif, textureMap[m_pScene->mMeshes[i]->mName.C_Str()].dif);
        } else {
            stb_img dif_white("assets/img/white.png");
            m_Textures[i].dif = new Texture(dif_white);
        }

        // if (dif.img != nullptr) {
        //     stb_img nrm(Dir + "/texture/" + textureMap[m_pScene->mMeshes[i]->mName.C_Str()].nrm);
        //     stb_img spc(Dir + "/texture/" + textureMap[m_pScene->mMeshes[i]->mName.C_Str()].spc);
        //     stb_img lmp(Dir + "/texture/" + textureMap[m_pScene->mMeshes[i]->mName.C_Str()].lmp);

        //     if (nrm.img != nullptr) m_Textures[i].nrm = new Texture(nrm);
        //     if (spc.img != nullptr) m_Textures[i].spc = new Texture(spc);
        //     if (lmp.img != nullptr) m_Textures[i].lmp = new Texture(lmp);
        // }

        // if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0){
        //     aiString Path;

        //     if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS){
        //         std::cout << "Path: " << Path.C_Str() << std::endl;
        //         std::string FullPath = Dir + "/" + Path.data;
        //         m_Textures[i] = new Texture(FullPath.c_str());
        //     } else {
        //         m_Textures[i] = new Texture("assets/img/white.png");
        //     }
        // }

        // for (unsigned int j = 0; j < pMaterial->GetTextureCount(aiTextureType_DIFFUSE); j++) {
        //     aiString path;
        //     pMaterial->GetTexture(aiTextureType_DIFFUSE, j, &path);
        //     std::cout << "Texture Path: " << path.C_Str() << std::endl;
        // }
    }

    return Ret;
}

void Mesh::push_uniforms(objectTransform &trans) {
    GLfloat color[3] = { 0.8, 0.8, 0.9 };
    GLfloat direction[3] = { -0.4, -1.0, 0.2 };
    GLfloat AmbientIntensity = 0.9;
    GLfloat DiffuseIntensity = 0.35;
    GLfloat SpecularPower = 0.25;
    GLfloat MatSpecularIntensity = 0.25;
    Vector3<GLfloat> EyesPos = GameManager::callbackData.camera->GetPosition();

    glUseProgram(shaderProgram);
    // glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &mtx_transform);
    glUniform3f(gColorLocation, color[0], color[1], color[2]);
    glUniform3f(gDirectionLocation, direction[0], direction[1], direction[2]);
    glUniform3f(gEyeWorldPosLocation, EyesPos[0], EyesPos[1], EyesPos[2]);
    glUniform1f(gAmbientIntensityLocation, AmbientIntensity);
    glUniform1f(gDiffuseIntensityLocation, DiffuseIntensity);
    glUniform1f(gSpecularPowerLocation, SpecularPower);
    glUniform1f(gMatSpecularIntensityLocation, MatSpecularIntensity);

    for (unsigned int i = 0 ; i < 1 ; i++) {
        struct {
            Vector3<GLfloat> Position = {-20.0, 0.0, 15.0};
            Vector3<GLfloat> Color = {1.0, 0.0, 0.9};
            GLfloat AmbientIntensity = 50.0;
            GLfloat DiffuseIntensity = 0.8;
            struct
            {
                GLfloat Constant = 1.0;
                GLfloat Linear = 0.09 * 25;
                GLfloat Exp = 0.032 * 15;
            } Atten;
        } pLights[1];

        objectTransform sphere_transform;
        pLights[i].Position = sphere_transform.GetWorldPos();
        pLights[i].Color = Vector3<GLfloat>(0.0, 0.0, 0.0); // !

        glUniform3f(m_material[i]->Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
        glUniform1f(m_material[i]->AmbientIntensity, pLights[i].AmbientIntensity);
        glUniform1f(m_material[i]->DiffuseIntensity, pLights[i].DiffuseIntensity);
        glUniform3f(m_material[i]->Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
        glUniform1f(m_material[i]->Atten.Constant, pLights[i].Atten.Constant);
        glUniform1f(m_material[i]->Atten.Linear, pLights[i].Atten.Linear);
        glUniform1f(m_material[i]->Atten.Exp, pLights[i].Atten.Exp);
    }
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

    m_pScene->mRootNode->mTransformation = TranslationMatrix * RotationMatrixZ * RotationMatrixY * RotationMatrixX * scaleMatrix * m_pScene->mRootNode->mTransformation;
}


void Mesh::Render(objectTransform &trans)
{
    glBindVertexArray(m_VAO);
    push_uniforms(trans);

    Matrix4f<GLfloat> CameraTranslationTrans, CameraRotateTrans, PersProjTrans;
    Matrix4f<GLfloat> ScaleTrans, RotateTrans, TranslationTrans;

    CameraTranslationTrans.InitTranslationTransform(-GameManager::render->pipeline.camera->GetPosition().x, -GameManager::render->pipeline.camera->GetPosition().y, -GameManager::render->pipeline.camera->GetPosition().z);
    CameraRotateTrans.InitCameraTransform(GameManager::render->pipeline.camera->Params.Target, GameManager::render->pipeline.camera->Params.Up);
    PersProjTrans.InitPersProjTransform(GameManager::render->pipeline.camera->PersProj.FOV, GameManager::render->pipeline.camera->PersProj.Width, GameManager::render->pipeline.camera->PersProj.Height, GameManager::render->pipeline.camera->PersProj.zNear, GameManager::render->pipeline.camera->PersProj.zFar);

    ScaleTrans.InitScaleTransform(trans.Scale.x, trans.Scale.y, trans.Scale.z);
    RotateTrans.InitRotateTransform(trans.Rotate.x, trans.Rotate.y, trans.Rotate.z);
    TranslationTrans.InitTranslationTransform(trans.WorldPos.x, trans.WorldPos.y, trans.WorldPos.z);

    Matrix4f<GLfloat> &Projection = PersProjTrans;
    Matrix4f<GLfloat> View = CameraRotateTrans * CameraTranslationTrans;
    Matrix4f<GLfloat> Model = TranslationTrans * RotateTrans * ScaleTrans;

    glUniformMatrix4fv(gModel, 1, GL_TRUE, &Model);

    glUniformMatrix4fv(gProjection, 1, GL_TRUE, &Projection);
    glUniformMatrix4fv(gView, 1, GL_TRUE, &View);

    std::vector<aiMatrix4x4> Transforms;
    BoneTransform(GameManager::Time.GetCurrentTime(), Transforms);

    for (unsigned int i = 0; i < m_NumBones; i++) 
        glUniformMatrix4fv(gBones + i, 1, GL_TRUE, &Transforms[i][0][0]);

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
