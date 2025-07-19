#pragma once
#include <object/geometry/loader/tinygltf_mesh_loader.hpp>
#include <glm/gtc/type_ptr.hpp>

bool TinygltfMeshLoader::LoadMesh(const std::string& fileName, GeometryMesh& mesh) {
    tinygltf::Model model;
    if (!GetModel(fileName, model))
        return false;

    return InitFromScene(mesh, model, fileName);
}

bool TinygltfMeshLoader::LoadMesh(const std::string& fileName, GeometrySkeletalMesh& mesh) {
    tinygltf::Model model;
    if (!GetModel(fileName, model))
        return false;

    return InitFromScene(mesh, model, fileName);
}

bool TinygltfMeshLoader::GetModel(const std::string& fileName, tinygltf::Model& model) {
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    std::string ext = fileName.substr(fileName.find_last_of('.') + 1);
    bool success = false;

    if (ext == "glb") {
        success = loader.LoadBinaryFromFile(&model, &err, &warn, fileName);
    } else if (ext == "gltf") {
        success = loader.LoadASCIIFromFile(&model, &err, &warn, fileName);
    } else {
        printf("Unsupported file type: '%s'\n", ext.c_str());
        return false;
    }

    if (!err.empty() || !warn.empty()) 
        throw std::runtime_error(err + '\n' + warn);

    if (!success)
        return false;
    
    return true;
}


bool TinygltfMeshLoader::InitFromScene(GeometryMesh& mesh, const tinygltf::Model& model, const std::string& fileName) {
    std::vector<unsigned int> Indices;
    std::vector<glm::vec3> Positions;
    std::vector<glm::vec2> TexCoords;
    std::vector<glm::vec3> Normals;
    std::vector<VertexBoneData> Bones;
    
    int NumIndices = 0;
    int NumVertices = 0;

    for (const auto& mesh : model.meshes) {
        for (const auto& primitive : mesh.primitives) {
            if (primitive.indices >= 0 && primitive.indices < model.accessors.size()) {
                const auto& accessor = model.accessors[primitive.indices];
                NumIndices += accessor.count;
            }

            auto it = primitive.attributes.find("POSITION");
            if (it != primitive.attributes.end() && it->second < model.accessors.size()) {
                const auto& accessor = model.accessors[it->second];
                NumVertices += accessor.count;
            }
        }
    }

    Indices.reserve(NumIndices);
    Positions.reserve(NumVertices);
    TexCoords.reserve(NumVertices);
    Normals.reserve(NumVertices);

    if (auto* skeletalMesh = dynamic_cast<GeometrySkeletalMesh*>(&mesh))
        Bones.resize(NumVertices);

    mesh.m_Entries.resize(model.meshes.size());
    mesh.m_Textures.resize(model.textures.size());

    size_t baseVertex = 0;
    size_t baseIndex = 0;
    
    for (size_t meshIndex = 0; meshIndex < model.meshes.size(); ++meshIndex) {
        const auto& m = model.meshes[meshIndex];
        GeometryMesh::MeshEntry& entry = mesh.m_Entries[meshIndex];

        for (const auto& primitive : m.primitives) {
            size_t indicesStart = Indices.size();
            size_t verticesStart = Positions.size();

            // --- VERTEX ATTRIBUTES ---
            const auto& attributes = primitive.attributes;

            auto ReadAttribute = [&](const std::string& name, void* outVec, int componentCount) {
                if (attributes.find(name) == attributes.end()) return;
                const tinygltf::Accessor& accessor = model.accessors[attributes.at(name)];
                const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[view.buffer];

                const unsigned char* data = buffer.data.data() + view.byteOffset + accessor.byteOffset;
                size_t count = accessor.count;
                size_t stride = view.byteStride ? view.byteStride : componentCount * sizeof(float);

                for (size_t i = 0; i < count; ++i) {
                    float* ptr = (float*)(data + i * stride);
                    if (componentCount == 3)
                        static_cast<std::vector<glm::vec3>*>(outVec)->emplace_back(ptr[0], ptr[1], ptr[2]);
                    else if (componentCount == 2)
                        static_cast<std::vector<glm::vec2>*>(outVec)->emplace_back(ptr[0], ptr[1]);
                }
            };

            ReadAttribute("POSITION", &Positions, 3);
            ReadAttribute("NORMAL", &Normals, 3);
            ReadAttribute("TEXCOORD_0", &TexCoords, 2);

            // // --- INDICES ---
            const tinygltf::Accessor& accessor = model.accessors[primitive.indices];
            const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
            const tinygltf::Buffer& buffer = model.buffers[view.buffer];
            const unsigned char* data = buffer.data.data() + view.byteOffset + accessor.byteOffset;

            for (size_t i = 0; i < accessor.count; ++i) {
                if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
                    Indices.push_back(baseVertex + ((uint16_t*)data)[i]);
                else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
                    Indices.push_back(baseVertex + ((uint32_t*)data)[i]);
            }

            // // --- TEXTURES ---
            int materialIndex = primitive.material;
            int textureIndex = -1;

            if (materialIndex >= 0 && materialIndex < model.materials.size()) {
                const auto& material = model.materials[materialIndex];
                textureIndex = material.pbrMetallicRoughness.baseColorTexture.index;
            }

            if (textureIndex >= 0 && textureIndex < model.textures.size()) {
                const tinygltf::Texture& texture = model.textures[textureIndex];
                const tinygltf::Image& image = model.images[texture.source];

                if (!image.image.empty()) {
                    mesh.m_Textures[textureIndex] = Texture::Create(fileName + image.name, image);
                } else if (!image.uri.empty()) {
                    std::string dir = fileName.substr(0, fileName.find_last_of("/\\") + 1);
                    mesh.m_Textures[textureIndex] = Texture::Create(image.uri, dir + image.uri);
                }
            }

            // --- MESH ENTRY ---
            GeometryMesh::MeshEntry subEntry;
            subEntry.BaseVertex = static_cast<unsigned int>(baseVertex);
            subEntry.BaseIndex  = static_cast<unsigned int>(baseIndex);
            subEntry.NumIndices = static_cast<unsigned int>(accessor.count);
            subEntry.MaterialIndex = materialIndex;

            mesh.m_Entries[meshIndex] = subEntry;

            baseVertex += Positions.size() - verticesStart;
            baseIndex  += accessor.count;


            // // --- SKINNING ---
            if (auto* skeletalMesh = dynamic_cast<GeometrySkeletalMesh*>(&mesh)) {
                auto jointsIter = primitive.attributes.find("JOINTS_0");
                auto weightsIter = primitive.attributes.find("WEIGHTS_0");

                if (jointsIter != primitive.attributes.end() && weightsIter != primitive.attributes.end()) {
                    const auto& jointsAccessor = model.accessors[jointsIter->second];
                    const auto& weightsAccessor = model.accessors[weightsIter->second];
                    const auto& jointsBufferView = model.bufferViews[jointsAccessor.bufferView];
                    const auto& weightsBufferView = model.bufferViews[weightsAccessor.bufferView];

                    const uint8_t* jointsData = model.buffers[jointsBufferView.buffer].data.data() +
                                                jointsAccessor.byteOffset + jointsBufferView.byteOffset;
                    const float* weightsData = reinterpret_cast<const float*>(
                        model.buffers[weightsBufferView.buffer].data.data() +
                        weightsAccessor.byteOffset + weightsBufferView.byteOffset
                    );

                    for (size_t i = 0; i < jointsAccessor.count; ++i) {
                        glm::ivec4 boneIDs = {0, 0, 0, 0};

                        if (jointsAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
                            const uint8_t* p = jointsData + i * 4;
                            for (int j = 0; j < 4; ++j)
                                boneIDs[j] = p[j];
                        } else if (jointsAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                            const uint16_t* p = reinterpret_cast<const uint16_t*>(jointsData + i * 8);
                            for (int j = 0; j < 4; ++j)
                                boneIDs[j] = p[j];
                        } else {
                            std::cerr << "Unsupported JOINTS_0 component type!" << std::endl;
                            continue;
                        }

                        glm::vec4 boneWeights = glm::make_vec4(weightsData + i * 4);

                        for (int j = 0; j < 4; ++j) {
                            Bones[baseVertex + i].IDs[j] = boneIDs[j];
                            Bones[baseVertex + i].Weights[j] = boneWeights[j];
                        }
                    }
                }
            }
        }
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

    if (auto* skeletalMesh = dynamic_cast<GeometrySkeletalMesh*>(&mesh)) {
        glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[GeometrySkeletalMesh::BONE_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(BONE_ID_LOCATION);
        glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
        glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);    
        glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);
    }

    return true;
}

bool TinygltfMeshLoader::InitFromScene(GeometrySkeletalMesh& mesh, const tinygltf::Model& model, const std::string& fileName) {
    // const auto& skins = model.skins;
    // if (skins.empty()) return false;



    // for (size_t meshIndex = 0; meshIndex < model.meshes.size(); ++meshIndex) {
    //     const auto& m = model.meshes[meshIndex];
    //     GeometryMesh::MeshEntry& entry = mesh.m_Entries[meshIndex];

    //     for (const auto& primitive : m.primitives) {

    //     }
    // }


    return true;
}
