#pragma once
#include <object/mesh/loader/tinygltf_mesh_loader.hpp>
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


bool TinygltfMeshLoader::LoadMesh(const std::string& fileName, MeshData& mesh) {
    tinygltf::Model model;
    if (!GetModel(fileName, model))
        return false;

    return InitFromScene(mesh, model, fileName);
}

bool TinygltfMeshLoader::LoadMesh(const std::string& fileName, SkeletalMeshData& mesh) {
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


void debugMaterials(const tinygltf::Model& model) {
    std::cout << "\n=== MATERIALS ===" << std::endl;
    
    for (size_t i = 0; i < model.materials.size(); i++) {
        const tinygltf::Material& mat = model.materials[i];
        
        std::cout << "\nMaterial " << i << ": " << mat.name << std::endl;
        
        // PBR металичность-шероховатость
        if (mat.pbrMetallicRoughness.baseColorTexture.index >= 0) {
            std::cout << "  Base Color Texture: " << mat.pbrMetallicRoughness.baseColorTexture.index 
                      << " (texcoord: " << mat.pbrMetallicRoughness.baseColorTexture.texCoord << ")" << std::endl;
        }
        
        if (mat.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0) {
            std::cout << "  MetallicRoughness Texture: " << mat.pbrMetallicRoughness.metallicRoughnessTexture.index 
                      << " (texcoord: " << mat.pbrMetallicRoughness.metallicRoughnessTexture.texCoord << ")" << std::endl;
        }
        
        // Нормальные карты
        if (mat.normalTexture.index >= 0) {
            std::cout << "  Normal Texture: " << mat.normalTexture.index 
                      << " (scale: " << mat.normalTexture.scale 
                      << ", texcoord: " << mat.normalTexture.texCoord << ")" << std::endl;
        }
        
        // Карты окклюзии
        if (mat.occlusionTexture.index >= 0) {
            std::cout << "  Occlusion Texture: " << mat.occlusionTexture.index 
                      << " (strength: " << mat.occlusionTexture.strength 
                      << ", texcoord: " << mat.occlusionTexture.texCoord << ")" << std::endl;
        }
        
        // Эмиссия
        if (mat.emissiveTexture.index >= 0) {
            std::cout << "  Emissive Texture: " << mat.emissiveTexture.index 
                      << " (texcoord: " << mat.emissiveTexture.texCoord << ")" << std::endl;
        }
        
        // Цвета
        if (mat.pbrMetallicRoughness.baseColorFactor.size() == 4) {
            std::cout << "  Base Color Factor: [" 
                      << mat.pbrMetallicRoughness.baseColorFactor[0] << ", "
                      << mat.pbrMetallicRoughness.baseColorFactor[1] << ", "
                      << mat.pbrMetallicRoughness.baseColorFactor[2] << ", "
                      << mat.pbrMetallicRoughness.baseColorFactor[3] << "]" << std::endl;
        }
        
        std::cout << "  Metallic Factor: " << mat.pbrMetallicRoughness.metallicFactor << std::endl;
        std::cout << "  Roughness Factor: " << mat.pbrMetallicRoughness.roughnessFactor << std::endl;
        std::cout << "  Alpha Mode: " << mat.alphaMode << std::endl;
        std::cout << "  Double Sided: " << (mat.doubleSided ? "true" : "false") << std::endl;
    }
}

void debugTextures(const tinygltf::Model& model) {
    std::cout << "\n=== TEXTURES ===" << std::endl;
    
    for (size_t i = 0; i < model.textures.size(); i++) {
        const tinygltf::Texture& tex = model.textures[i];
        
        std::cout << "Texture " << i << ":" << std::endl;
        std::cout << "  Source: " << tex.source << std::endl;
        std::cout << "  Sampler: " << tex.sampler << std::endl;
        
        // Показываем информацию об изображении
        if (tex.source >= 0 && tex.source < model.images.size()) {
            const tinygltf::Image& image = model.images[tex.source];
            std::cout << "  Image: " << image.name << std::endl;
            std::cout << "  URI: " << image.uri << std::endl;
            std::cout << "  Size: " << image.width << "x" << image.height << std::endl;
            std::cout << "  Component: " << image.component << std::endl;
            std::cout << "  Bits: " << image.bits << std::endl;
            std::cout << "  Pixel Type: " << image.pixel_type << std::endl;
            if (!image.image.empty()) {
                std::cout << "  Image Data: " << image.image.size() << " bytes" << std::endl;
            }
        }
    }
}

void debugImages(const tinygltf::Model& model) {
    std::cout << "\n=== IMAGES ===" << std::endl;
    
    for (size_t i = 0; i < model.images.size(); i++) {
        const tinygltf::Image& image = model.images[i];
        
        std::cout << "Image " << i << ": " << image.name << std::endl;
        std::cout << "  URI: " << image.uri << std::endl;
        std::cout << "  Size: " << image.width << "x" << image.height << std::endl;
        std::cout << "  Components: " << image.component << std::endl;
        std::cout << "  Bits: " << image.bits << std::endl;
        std::cout << "  Pixel Type: " << image.pixel_type << std::endl;
        std::cout << "  Buffer View: " << image.bufferView << std::endl;
        std::cout << "  Mime Type: " << image.mimeType << std::endl;
        
        if (!image.image.empty()) {
            std::cout << "  Embedded: YES (" << image.image.size() << " bytes)" << std::endl;
        } else if (!image.uri.empty()) {
            std::cout << "  External: YES" << std::endl;
        } else if (image.bufferView >= 0) {
            std::cout << "  From Buffer: YES" << std::endl;
        }
    }
}

bool TinygltfMeshLoader::InitFromScene(MeshData& mesh, const tinygltf::Model& model, const std::string& fileName) {
    std::cout << "\n=== MODEL SUMMARY ===" << std::endl;
    std::cout << "Scenes: " << model.scenes.size() << std::endl;
    std::cout << "Nodes: " << model.nodes.size() << std::endl;
    std::cout << "Meshes: " << model.meshes.size() << std::endl;
    std::cout << "Materials: " << model.materials.size() << std::endl;
    std::cout << "Textures: " << model.textures.size() << std::endl;
    std::cout << "Images: " << model.images.size() << std::endl;
    std::cout << "Samplers: " << model.samplers.size() << std::endl;

    debugMaterials(model);
    debugTextures(model);

    // int NumIndices = 0;
    // int NumVertices = 0;
    // int numBones = 0;

    // for (const auto& mesh : model.meshes) {
    //     for (const auto& primitive : mesh.primitives) {
    //         if (primitive.indices >= 0 && primitive.indices < model.accessors.size()) {
    //             const auto& accessor = model.accessors[primitive.indices];
    //             NumIndices += accessor.count;
    //         }

    //         auto it = primitive.attributes.find("POSITION");
    //         if (it != primitive.attributes.end() && it->second < model.accessors.size()) {
    //             const auto& accessor = model.accessors[it->second];
    //             NumVertices += accessor.count;
    //         }
    //     }
    // }

    // if (skeletalMesh != nullptr && !model.skins.empty()) {
    //     for (const auto& jointIndex : model.skins[0].joints) {
    //         if (jointIndex >= 0 && jointIndex < model.nodes.size()) {
    //             ++numBones;
    //         }
    //     }

    //     BoneIndexes.reserve(numBones);
    //     BoneIndexesMap.reserve(numBones);
    //     for (const auto& jointIndex : model.skins[0].joints) {
    //         if (jointIndex >= 0 && jointIndex < model.nodes.size()) {
    //             BoneIndexes.emplace_back(jointIndex);
    //         }
    //     }

    //     int index = 0;
    //     for (auto i : BoneIndexes) {
    //         BoneIndexesMap.insert({i, index++});
    //     }
    // }

    // Indices.reserve(NumIndices);
    // Positions.reserve(NumVertices);
    // TexCoords.reserve(NumVertices);
    // Normals.reserve(NumVertices);

    // if (skeletalMesh != nullptr) {
    //     Bones.resize(NumVertices);
    //     skeletalMesh->skeleton.BoneMap.reserve(numBones);
    //     skeletalMesh->skeleton.inverseBindMat.resize(numBones, glm::mat4(1.0f));
    //     skeletalMesh->skeleton.inverseBindDQ.resize(numBones, {glm::quat(1, 0, 0, 0), glm::quat(0, 0, 0, 0)});
    // }

    // mesh.m_Entries.resize(model.meshes.size());
    // mesh.m_Textures.resize(model.textures.size());

    // size_t baseVertex = 0;
    // size_t baseIndex = 0;
    
    for (size_t meshIndex = 0; meshIndex < model.meshes.size(); ++meshIndex) {
        const auto& m = model.meshes[meshIndex];
        MeshData::MeshEntry& entry = mesh.m_Entries[meshIndex];

        for (const auto& primitive : m.primitives) {
            // size_t indicesStart = Indices.size();
            // size_t verticesStart = Positions.size();

            // // --- VERTEX ATTRIBUTES ---
            // const auto& attributes = primitive.attributes;

            // auto ReadAttribute = [&](const std::string& name, void* outVec, int componentCount) {
            //     if (attributes.find(name) == attributes.end()) return;
            //     const tinygltf::Accessor& accessor = model.accessors[attributes.at(name)];
            //     const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
            //     const tinygltf::Buffer& buffer = model.buffers[view.buffer];

            //     const unsigned char* data = buffer.data.data() + view.byteOffset + accessor.byteOffset;
            //     size_t count = accessor.count;
            //     size_t stride = view.byteStride ? view.byteStride : componentCount * sizeof(float);

            //     for (size_t i = 0; i < count; ++i) {
            //         float* ptr = (float*)(data + i * stride);
            //         if (componentCount == 3)
            //             static_cast<std::vector<glm::vec3>*>(outVec)->emplace_back(ptr[0], ptr[1], ptr[2]);
            //         else if (componentCount == 2)
            //             static_cast<std::vector<glm::vec2>*>(outVec)->emplace_back(ptr[0], ptr[1]);
            //     }
            // };

            // ReadAttribute("POSITION", &Positions, 3);
            // ReadAttribute("NORMAL", &Normals, 3);
            // ReadAttribute("TEXCOORD_0", &TexCoords, 2);

            // // --- INDICES ---
            // const tinygltf::Accessor& accessor = model.accessors[primitive.indices];
            // const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
            // const tinygltf::Buffer& buffer = model.buffers[view.buffer];
            // const unsigned char* data = buffer.data.data() + view.byteOffset + accessor.byteOffset;

            // for (size_t i = 0; i < accessor.count; ++i) {
            //     if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
            //         Indices.push_back(baseVertex + ((uint16_t*)data)[i]);
            //     else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
            //         Indices.push_back(baseVertex + ((uint32_t*)data)[i]);
            // }

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
                    // mesh.m_Textures[textureIndex] = 
                    Texture::Create(fileName + image.name, image);
                } else if (!image.uri.empty()) {
                    std::string dir = fileName.substr(0, fileName.find_last_of("/\\") + 1);
                    // mesh.m_Textures[textureIndex] = 
                    Texture::Create(image.uri, dir + image.uri);
                }
            }

            // --- MESH ENTRY ---
            // GeometryMesh::MeshEntry subEntry;
            // subEntry.BaseVertex = static_cast<unsigned int>(baseVertex);
            // subEntry.BaseIndex  = static_cast<unsigned int>(baseIndex);
            // subEntry.NumIndices = static_cast<unsigned int>(accessor.count);
            // subEntry.MaterialIndex = materialIndex;

            // mesh.m_Entries[meshIndex] = subEntry;

            // --- SKINNING ---
            // if (skeletalMesh != nullptr) {
            //     auto jointsIter = primitive.attributes.find("JOINTS_0");
            //     auto weightsIter = primitive.attributes.find("WEIGHTS_0");

            //     if (jointsIter != primitive.attributes.end() && weightsIter != primitive.attributes.end()) {
            //         const auto& jointsAccessor = model.accessors[jointsIter->second];
            //         const auto& weightsAccessor = model.accessors[weightsIter->second];

            //         const auto& jointsBufferView = model.bufferViews[jointsAccessor.bufferView];
            //         const auto& weightsBufferView = model.bufferViews[weightsAccessor.bufferView];

            //         const auto& weightsBuffer = model.buffers[weightsBufferView.buffer];
            //         const auto& jointsBuffer = model.buffers[jointsBufferView.buffer];

            //         const uint8_t* jointsData = jointsBuffer.data.data() + jointsBufferView.byteOffset + jointsAccessor.byteOffset;
            //         const float* weightsData = reinterpret_cast<const float*>(
            //             weightsBuffer.data.data() + weightsBufferView.byteOffset + weightsAccessor.byteOffset
            //         );

            //         const tinygltf::Skin& skin = model.skins[0];

            //         for (size_t i = 0; i < jointsAccessor.count; ++i) {
            //             glm::ivec4 boneIDs = {0, 0, 0, 0};

            //             if (jointsAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
            //                 const uint8_t* p = jointsData + i * 4;
            //                 for (int j = 0; j < 4; ++j)
            //                     boneIDs[j] = p[j];
            //             } else if (jointsAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
            //                 const uint16_t* p = reinterpret_cast<const uint16_t*>(jointsData + i * 8);
            //                 for (int j = 0; j < 4; ++j)
            //                     boneIDs[j] = p[j];
            //             } else {
            //                 std::cerr << "Unsupported JOINTS_0 component type!" << std::endl;
            //                 continue;
            //             }

            //             glm::vec4 boneWeights = glm::make_vec4(weightsData + i * 4);

            //             for (int j = 0; j < 4; ++j) {
            //                 int jointNodeIndex = skin.joints[boneIDs[j]];
            //                 auto it = BoneIndexesMap.find(jointNodeIndex);

            //                 size_t dstIndex = baseVertex + i;
            //                 if (dstIndex >= Bones.size()) {
            //                     std::cerr << "Index overflow: Bones[" << dstIndex << "] >= size=" << Bones.size() << std::endl;
            //                     continue;
            //                 }

            //                 if (it != BoneIndexesMap.end()) {
            //                     Bones[baseVertex + i].IDs[j] = it->second;
            //                     Bones[baseVertex + i].Weights[j] = boneWeights[j];
            //                 }
            //             }
            //         }
            //     }
            // }

            // --- UPDATE BASE INDEXES ---
            // baseVertex += Positions.size() - verticesStart;
            // baseIndex  += accessor.count;
        }
    }

    // if (skeletalMesh != nullptr) {
        // int start = 800;
        // for (int i = start; i < start + 100; ++i) {
        //     std::cout << "IDs: ";
        //     for (int j = 0; j < 4; ++j) {
        //         std::cout << Bones[i].IDs[j] << " ";
        //     }
        //     std::cout << std::endl;
        //     std::cout << "boneWeights: ";
        //     for (int j = 0; j < 4; ++j) {
        //         std::cout << Bones[i].Weights[j] << " ";
        //     }
        //     std::cout << std::endl;
        // }

        // === BONE TREE ===
        // int skin = FindSkeletonRoot(model);
        // if (skin >= 0)
        //     skeletalMesh->skeleton.BuildBoneTree(model, BoneIndexesMap, skin);


        // === ANIMATION ===
        // for (size_t i = 0; i < model.animations.size(); ++i) {
        //     const tinygltf::Animation& gltfAnim = model.animations[i];
        //     std::string animName = gltfAnim.name.empty() ? "Anim_" + std::to_string(i) : gltfAnim.name;
        //     skeletalMesh->skeleton.AnimationMap.insert({animName, SkeletalAnimation(model, gltfAnim, BoneIndexesMap)});
        //     // for (auto& it : skeletalMesh->skeleton.AnimationMap.begin()->second.Tracks) {
        //     //     std::cout << it.first << " " << it.second.Positions.size() << std::endl;
        //     // }
        // }

        // === INVERSE BIND ===
        // if (!model.skins.empty()) {
        //     const tinygltf::Skin& skin = model.skins[0];

        //     if (skin.inverseBindMatrices >= 0) {
        //         const auto& accessor = model.accessors[skin.inverseBindMatrices];
        //         const auto& bufferView = model.bufferViews[accessor.bufferView];
        //         const float* matrixData = reinterpret_cast<const float*>(
        //             model.buffers[bufferView.buffer].data.data() +
        //             accessor.byteOffset + bufferView.byteOffset
        //         );
        //         const auto& jointIndices = model.skins[0].joints;
        //         for (size_t i = 0; i < accessor.count; ++i) {
        //             int nodeIndex = jointIndices[i];

        //             auto it = BoneIndexesMap.find(nodeIndex);
        //             if (it == BoneIndexesMap.end()) {
        //                 continue;
        //             }
        //             glm::mat4 mat = glm::make_mat4(matrixData + i * 16);
        //             skeletalMesh->skeleton.inverseBindMat[it->second] = mat;
        //         }
        //     }
        // }
    // }

    return true;
}

