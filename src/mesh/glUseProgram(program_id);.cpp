#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <game/gameManager.hpp>
#include <mesh/mesh.hpp>
#include <stb_image.h>

#include <object/sphere.hpp>
#include <object/cube.hpp>

stb_img_struct::stb_img_struct(const std::string& Filename) {
    img = stbi_load(Filename.c_str(), &x, &y, &n, 0);
    if (img == nullptr)
        std::cerr << "Failed to load img: " << Filename << std::endl;
    
}
stb_img_struct::~stb_img_struct() {
    if (img != nullptr)
        stbi_image_free(img);
}

Texture::Texture(const stb_img_struct& stb_img)
{
    if (stb_img.img == nullptr)
        std::cerr << "Failed to create texture" << std::endl;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    GLenum format = (stb_img.n == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, stb_img.x, stb_img.y, 0, format, GL_UNSIGNED_BYTE, stb_img.img);

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

Mesh::MeshEntry::MeshEntry()
{
    VB = INVALID_OGL_VALUE;
    IB = INVALID_OGL_VALUE;
    VAO = INVALID_OGL_VALUE;
    NumIndices = 0;
    MaterialIndex = INVALID_MATERIAL;
}

Mesh::MeshEntry::~MeshEntry()
{
    if (VB != INVALID_OGL_VALUE){
        glDeleteBuffers(1, &VB);
    }

    if (IB != INVALID_OGL_VALUE){
        glDeleteBuffers(1, &IB);
    }

    if (VAO != INVALID_OGL_VALUE){
        glDeleteBuffers(1, &VAO);
    }
}

Mesh::Mesh(const std::string& Filename)
{
    LoadMesh(Filename);
    compileShaders("assets\\model\\shaders\\model_fs.glsl", "assets\\model\\shaders\\model_vs.glsl");
}

Mesh::~Mesh()
{
}

void Mesh::MeshEntry::Init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices)
{
    NumIndices = Indices.size();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, m_pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, m_normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, m_tex));

    glBindVertexArray(0);
}


void Mesh::Clear()
{
    // for (unsigned int i = 0; i < m_Textures.size(); i++){
    //     SAFE_DELETE(m_Textures[i]);
    // }
}

bool Mesh::LoadMesh(const std::string& Filename)
{
    Clear();

    bool Ret = false;

    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals| aiProcess_FlipUVs);

    if (pScene) {
        InitFromScene(pScene, Filename);
    } else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
        return false;
    }

    return Ret;
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

    gTextureSamplerLocation       = glGetUniformLocation(shaderProgram, "gTextureSampler");
    gNormalMapLocation            = glGetUniformLocation(shaderProgram, "gNormalMap");
    gEyeWorldPosLocation          = glGetUniformLocation(shaderProgram, "gEyeWorldPos");
    gWorldLocation                = glGetUniformLocation(shaderProgram, "gWorld");
    gColorLocation                = glGetUniformLocation(shaderProgram, "gDirectionalLight.Color");
    gDirectionLocation            = glGetUniformLocation(shaderProgram, "gDirectionalLight.Direction");
    gAmbientIntensityLocation     = glGetUniformLocation(shaderProgram, "gDirectionalLight.AmbientIntensity");
    gDiffuseIntensityLocation     = glGetUniformLocation(shaderProgram, "gDirectionalLight.DiffuseIntensity");
    gMatSpecularIntensityLocation = glGetUniformLocation(shaderProgram, "gMatSpecularIntensity");
    gSpecularPowerLocation        = glGetUniformLocation(shaderProgram, "gSpecularPower");

    gModelLocation = glGetUniformLocation(shaderProgram, "model");
    gViewLocation = glGetUniformLocation(shaderProgram, "view");
    gProjectionLocation = glGetUniformLocation(shaderProgram, "projection");

    gDirLightColorLocation = glGetUniformLocation(shaderProgram, "gDirectionalLight.Color");
    gDirLightDirectionLocation = glGetUniformLocation(shaderProgram, "gDirectionalLight.Direction");
    gDirLightAmbientIntensityLocation = glGetUniformLocation(shaderProgram, "gDirectionalLight.AmbientIntensity");
    gDirLightDiffuseIntensityLocation = glGetUniformLocation(shaderProgram, "gDirectionalLight.DiffuseIntensity");

    gPointLightPosLocation = glGetUniformLocation(shaderProgram, "gPointLights[0].Position");
    gPointLightColorLocation = glGetUniformLocation(shaderProgram, "gPointLights[0].Color");
    gPointLightAmbientIntensityLocation = glGetUniformLocation(shaderProgram, "gPointLights[0].AmbientIntensity");
    gPointLightDiffuseIntensityLocation = glGetUniformLocation(shaderProgram, "gPointLights[0].DiffuseIntensity");
    gPointLightConstantLocation = glGetUniformLocation(shaderProgram, "gPointLights[0].Constant");
    gPointLightLinearLocation = glGetUniformLocation(shaderProgram, "gPointLights[0].Linear");
    gPointLightExpLocation = glGetUniformLocation(shaderProgram, "gPointLights[0].Exp");

    // for (unsigned int i = 0 ; i < 1 ; i++) {
    //     m_material[i]->Color               = glGetUniformLocation(shaderProgram, std::string(("gPointLights[") + std::string("0" + i) + std::string("].Color"           )).c_str());
    //     m_material[i]->AmbientIntensity    = glGetUniformLocation(shaderProgram, std::string(("gPointLights[") + std::string("0" + i) + std::string("].AmbientIntensity")).c_str());
    //     m_material[i]->DiffuseIntensity    = glGetUniformLocation(shaderProgram, std::string(("gPointLights[") + std::string("0" + i) + std::string("].DiffuseIntensity")).c_str());
    //     m_material[i]->Position            = glGetUniformLocation(shaderProgram, std::string(("gPointLights[") + std::string("0" + i) + std::string("].Position"        )).c_str());
    //     m_material[i]->Atten.Constant      = glGetUniformLocation(shaderProgram, std::string(("gPointLights[") + std::string("0" + i) + std::string("].Constant"  )).c_str());
    //     m_material[i]->Atten.Linear        = glGetUniformLocation(shaderProgram, std::string(("gPointLights[") + std::string("0" + i) + std::string("].Linear"    )).c_str());
    //     m_material[i]->Atten.Exp           = glGetUniformLocation(shaderProgram, std::string(("gPointLights[") + std::string("0" + i) + std::string("].Exp"       )).c_str());
    // }
    // assert(gTextureSamplerLocation       != 0xFFFFFFFF);
    // assert(gNormalMapLocation            != 0xFFFFFFFF);
    // assert(gEyeWorldPosLocation          != 0xFFFFFFFF);
    // assert(gWorldLocation                != 0xFFFFFFFF);
    // assert(gColorLocation                != 0xFFFFFFFF);
    // assert(gDirectionLocation            != 0xFFFFFFFF);
    // assert(gAmbientIntensityLocation     != 0xFFFFFFFF);
    // assert(gDiffuseIntensityLocation     != 0xFFFFFFFF);
    // assert(gMatSpecularIntensityLocation != 0xFFFFFFFF);
    // assert(gSpecularPowerLocation        != 0xFFFFFFFF);

    // for (unsigned int i = 0 ; i < 1 ; i++) {
    //     assert(m_material[i]->Color            != 0xFFFFFFFF);
    //     assert(m_material[i]->AmbientIntensity != 0xFFFFFFFF);
    //     assert(m_material[i]->DiffuseIntensity != 0xFFFFFFFF);
    //     assert(m_material[i]->Position         != 0xFFFFFFFF);
    //     assert(m_material[i]->Atten.Constant   != 0xFFFFFFFF);
    //     assert(m_material[i]->Atten.Linear     != 0xFFFFFFFF);
    //     assert(m_material[i]->Atten.Exp        != 0xFFFFFFFF);
    // }
}


bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    for (unsigned int i = 0; i < m_Entries.size(); i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        std::cout << pScene->mMeshes[i]->mName.C_Str() << std::endl;
        InitMesh(i, paiMesh);
    }
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> Total = end - start;
    std::cout << "Init model time: " << Total.count() << std::endl; 

    return InitMaterials(pScene, Filename);
}

void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
    m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    for (unsigned int i = 0; i< paiMesh->mNumVertices; i++){
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ?
                            &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        Vertex v(glm::vec3(pPos->x, pPos->y, pPos->z),
                 glm::vec3(pNormal->x, pNormal->y, pNormal->z),
                 glm::vec3(0, 0, 0),
                 glm::vec2(pTexCoord->x,pTexCoord->y));
        Vertices.push_back(v);
    }

    for (unsigned int i = 0; i < paiMesh->mNumFaces; i++){
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }

    m_Entries[Index].Init(Vertices, Indices);
}

bool Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
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
    std::ifstream file(Dir + "/texture.txt");  // Укажите путь к файлу

    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл!" << std::endl;
        return 1;
    }



    std::string key, value, dif, nrm, spc, lmp;
    while (file >> key >> dif >> nrm >> spc >> lmp) {
        textureMap[key] = textures_names(dif, nrm, spc, lmp);
    }

    file.close();
    

    bool Ret = true;

    for (unsigned int i = 0; i< pScene->mNumMaterials; i++){
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        std::cout << pScene->mMeshes[i]->mName.C_Str() << " " << textureMap[pScene->mMeshes[i]->mName.C_Str()].dif << std::endl;
        stb_img dif(Dir + "/texture/" + textureMap[pScene->mMeshes[i]->mName.C_Str()].dif);
        if (dif.img != nullptr) {
            m_Textures[i].dif = new Texture(dif);
        } else {
            stb_img dif_white("assets/img/white.png");
            m_Textures[i].dif = new Texture(dif_white);
        }

        if (dif.img != nullptr) {
            stb_img nrm(Dir + "/texture/" + textureMap[pScene->mMeshes[i]->mName.C_Str()].nrm);
            stb_img spc(Dir + "/texture/" + textureMap[pScene->mMeshes[i]->mName.C_Str()].spc);
            stb_img lmp(Dir + "/texture/" + textureMap[pScene->mMeshes[i]->mName.C_Str()].lmp);

            if (nrm.img != nullptr) m_Textures[i].nrm = new Texture(nrm);
            if (spc.img != nullptr) m_Textures[i].spc = new Texture(spc);
            if (lmp.img != nullptr) m_Textures[i].lmp = new Texture(lmp);
        }

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

void Mesh::Render(const objectTransform &model_transform)
{
    struct {
        glm::vec3 Color = { 0.8, 0.8, 0.9 };
        glm::vec3 Direction = { -0.4, -1.0, 0.2 };
        GLfloat AmbientIntensity = 0.9;
        GLfloat DiffuseIntensity = 0.35;
        GLfloat SpecularPower = 0.25;
        GLfloat MatSpecularIntensity = 0.25;
    } dirLight;
    Vector3<GLfloat> EyesPos = GameManager::callbackData.camera->GetPosition();

    static Sphere<10> sphere;
    objectTransform sphere_transform;
    sphere_transform.SetWorldPos(-25.0, 7.0, 10.0);
    sphere_transform.SetRotate(0.0, 0.0, 0.0);
    sphere_transform.SetScale(0.9);
    sphere.color = Vector3<GLfloat>(0.8, 0.0, 1.0);

    GameManager::render->PushGeometry(sphere.GetGeometry());
    GameManager::render->drawObject(GameManager::render->pipeline.GetTransform(sphere_transform), &sphere);


    glUseProgram(shaderProgram);
    // glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &mtx_transform);
    // glUniform3f(gColorLocation, color[0], color[1], color[2]);
    // glUniform3f(gDirectionLocation, direction[0], direction[1], direction[2]);
    // glUniform3f(gEyeWorldPosLocation, EyesPos[0], EyesPos[1], EyesPos[2]);
    // glUniform1f(gAmbientIntensityLocation, AmbientIntensity);
    // glUniform1f(gDiffuseIntensityLocation, DiffuseIntensity);
    // glUniform1f(gSpecularPowerLocation, SpecularPower);
    // glUniform1f(gMatSpecularIntensityLocation, MatSpecularIntensity);

    Matrix4f<GLfloat> ScaleTrans, RotateTrans, TranslationTrans, CameraTranslationTrans, CameraRotateTrans, PersProjTrans;

    ScaleTrans.InitScaleTransform(model_transform.Scale.x, model_transform.Scale.y, model_transform.Scale.z);
    RotateTrans.InitRotateTransform(model_transform.Rotate.x, model_transform.Rotate.y, model_transform.Rotate.z);
    TranslationTrans.InitTranslationTransform(model_transform.WorldPos.x, model_transform.WorldPos.y, model_transform.WorldPos.z);

    CameraTranslationTrans.InitTranslationTransform(-GameManager::render->pipeline.camera->GetPosition().x, -GameManager::render->pipeline.camera->GetPosition().y, -GameManager::render->pipeline.camera->GetPosition().z);
    CameraRotateTrans.InitCameraTransform(GameManager::render->pipeline.camera->Params.Target, GameManager::render->pipeline.camera->Params.Up);

    PersProjTrans.InitPersProjTransform(GameManager::render->pipeline.camera->PersProj.FOV, GameManager::render->pipeline.camera->PersProj.Width, GameManager::render->pipeline.camera->PersProj.Height, GameManager::render->pipeline.camera->PersProj.zNear, GameManager::render->pipeline.camera->PersProj.zFar);

    Matrix4f<GLfloat> model = ScaleTrans * RotateTrans * TranslationTrans;
    Matrix4f<GLfloat> view = CameraTranslationTrans * CameraRotateTrans;
    Matrix4f<GLfloat> &projection = PersProjTrans;

    glUniformMatrix4fv(gModelLocation, 1, GL_TRUE, &model);
    glUniformMatrix4fv(gViewLocation, 1, GL_TRUE, &view);
    glUniformMatrix4fv(gProjectionLocation, 1, GL_TRUE, &projection);

    glUniform1i(glGetUniformLocation(shaderProgram, "gTextureSampler"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "gNormalMap"), 1);

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


        pLights[i].Position = sphere_transform.GetWorldPos();
        pLights[i].Color = sphere.color;

        glUniform3f(m_material[i]->Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
        glUniform1f(m_material[i]->AmbientIntensity, pLights[i].AmbientIntensity);
        glUniform1f(m_material[i]->DiffuseIntensity, pLights[i].DiffuseIntensity);
        glUniform3f(m_material[i]->Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
        glUniform1f(m_material[i]->Atten.Constant, pLights[i].Atten.Constant);
        glUniform1f(m_material[i]->Atten.Linear, pLights[i].Atten.Linear);
        glUniform1f(m_material[i]->Atten.Exp, pLights[i].Atten.Exp);

        glUniform3f(gDirLightColorLocation, dirLight.Color.x, dirLight.Color.y, dirLight.Color.z);
        glUniform3f(gDirLightDirectionLocation, dirLight.Direction.x, dirLight.Direction.y, dirLight.Direction.z);
        glUniform1f(gDirLightAmbientIntensityLocation, dirLight.AmbientIntensity);
        glUniform1f(gDirLightDiffuseIntensityLocation, dirLight.DiffuseIntensity);

        glUniform3f(gPointLightPosLocation, pLights[0].Position.x, pLights[0].Position.y, pLights[0].Position.z);
        glUniform3f(gPointLightColorLocation, pLights[0].Color.x, pLights[0].Color.y, pLights[0].Color.z);
        glUniform1f(gPointLightAmbientIntensityLocation, pLights[0].AmbientIntensity);
        glUniform1f(gPointLightDiffuseIntensityLocation, pLights[0].DiffuseIntensity);
        glUniform1f(gPointLightConstantLocation, pLights[0].Atten.Constant);
        glUniform1f(gPointLightLinearLocation, pLights[0].Atten.Linear);
        glUniform1f(gPointLightExpLocation, pLights[0].Atten.Exp);
    }

    for (unsigned int i = 0; i < m_Entries.size(); i++) {
        glBindVertexArray(m_Entries[i].VAO);
        const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

        if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex].dif) {
            if (m_Textures[MaterialIndex].dif != nullptr) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_Textures[MaterialIndex].dif->texture);
            }
            if (m_Textures[MaterialIndex].nrm != nullptr) {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, m_Textures[MaterialIndex].nrm->texture);
            }

            // glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, m_Textures[MaterialIndex].dif->texture);
            glUniform1i(gTextureSamplerLocation, 0);
            glUniform1i(gNormalMapLocation, 0);
        }

        glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }
}

