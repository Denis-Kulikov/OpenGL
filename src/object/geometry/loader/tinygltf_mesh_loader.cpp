#pragma once
#include <object/geometry/loader/tinygltf_mesh_loader.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tiny_gltf.h>

void PrintMatrix(const glm::mat4& matrix);

int FindSkeletonRoot(const tinygltf::Model& model) {
    const tinygltf::Skin* skin;
    for (const auto& node : model.nodes) {
        if (node.skin >= 0) {
            skin = &model.skins[node.skin];
        }
    }

    std::unordered_set<int> jointSet(skin->joints.begin(), skin->joints.end());
    std::unordered_set<int> childSet;

    for (int jointIndex : skin->joints) {
        const auto& node = model.nodes[jointIndex];
        for (int child : node.children) {
            childSet.insert(child);
        }
    }

    for (int jointIndex : skin->joints) {
        if (childSet.find(jointIndex) == childSet.end()) {
            return jointIndex;
        }
    }

    return -1;
}


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
    std::vector<int> BoneIndexes;
    std::unordered_map<int, int> BoneIndexesMap;

    auto* skeletalMesh = dynamic_cast<GeometrySkeletalMesh*>(&mesh);
    
    int NumIndices = 0;
    int NumVertices = 0;
    int numBones = 0;

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

    if (skeletalMesh != nullptr && !model.skins.empty()) {
        for (const auto& jointIndex : model.skins[0].joints) {
            if (jointIndex >= 0 && jointIndex < model.nodes.size()) {
                ++numBones;
            }
        }

        BoneIndexes.reserve(numBones);
        BoneIndexesMap.reserve(numBones);
        for (const auto& jointIndex : model.skins[0].joints) {
            if (jointIndex >= 0 && jointIndex < model.nodes.size()) {
                BoneIndexes.emplace_back(jointIndex);
            }
        }

        int index = 0;
        for (auto i : BoneIndexes) {
            BoneIndexesMap.insert({i, index++});
        }
    }

    Indices.reserve(NumIndices);
    Positions.reserve(NumVertices);
    TexCoords.reserve(NumVertices);
    Normals.reserve(NumVertices);

    if (skeletalMesh != nullptr) {
        Bones.resize(NumVertices);
        skeletalMesh->skeleton.BoneMap.reserve(numBones);
        skeletalMesh->skeleton.BoneLocal.resize(numBones, glm::mat4(1.0f));
        skeletalMesh->skeleton.inverseBind.resize(numBones, {glm::quat(1, 0, 0, 0), glm::quat(0, 0, 0, 0)});
    }

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

            // --- SKINNING ---
            if (skeletalMesh != nullptr) {
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
                            auto it = BoneIndexesMap.find(boneIDs[j]);
                            if (it != BoneIndexesMap.end()) {
                                Bones[baseVertex + i].IDs[j] = it->second;
                                Bones[baseVertex + i].Weights[j] = boneWeights[j];
                            }
                        }
                    }
                }
            }

            // --- UPDATE BASE INDEXES ---
            baseVertex += Positions.size() - verticesStart;
            baseIndex  += accessor.count;
        }
    }

    if (skeletalMesh != nullptr) {
        // === ANIMATION ===
        for (size_t i = 0; i < model.animations.size(); ++i) {
            const tinygltf::Animation& gltfAnim = model.animations[i];
            std::string animName = gltfAnim.name.empty() ? "Anim_" + std::to_string(i) : gltfAnim.name;
            skeletalMesh->skeleton.AnimationMap.insert({animName, SkeletalAnimation(model, gltfAnim, BoneIndexesMap)});
        }

        // === INVERSE BIND ===
        if (!model.skins.empty()) {
            const tinygltf::Skin& skin = model.skins[0];

            if (skin.inverseBindMatrices >= 0) {
                const auto& accessor = model.accessors[skin.inverseBindMatrices];
                const auto& bufferView = model.bufferViews[accessor.bufferView];
                const float* matrixData = reinterpret_cast<const float*>(
                    model.buffers[bufferView.buffer].data.data() +
                    accessor.byteOffset + bufferView.byteOffset
                );
                const auto& jointIndices = model.skins[0].joints;
                for (size_t i = 0; i < accessor.count; ++i) {
                    int nodeIndex = jointIndices[i];

                    auto it = BoneIndexesMap.find(nodeIndex);
                    if (it == BoneIndexesMap.end()) {
                        continue;
                    }
                    glm::mat4 mat = glm::make_mat4(matrixData + i * 16);
                    skeletalMesh->skeleton.BoneLocal[it->second] = mat;
                }
            }
        }

        // === BONE TREE ===
        int skin = FindSkeletonRoot(model);
        if (skin >= 0)
            skeletalMesh->skeleton.BuildBoneTree(model, BoneIndexesMap, skin);
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

    if (skeletalMesh != nullptr) {
        glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[GeometrySkeletalMesh::BONE_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(BONE_ID_LOCATION);
        glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
        glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);    
        glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);
    }

    return true;
}

