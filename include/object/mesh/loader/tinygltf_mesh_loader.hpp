//#pragma once
//#include "mesh_loader.hpp"
//
//namespace tinygltf {
//    class Model;
//};
//
//// НЕ РАБОТАЕТ ДЛЯ АНИМИРОВАННЫХ МОДЕЛЕЙ
//class TinygltfMeshLoader : public IMeshLoader {
//public:
//    bool LoadMesh(const std::string& fileName, Mesh& mesh) override;
//    bool LoadMesh(const std::string& fileName,SkeletalMesh& mesh) override;
//
//protected:
//    bool GetModel(const std::string& fileName, tinygltf::Model& model);
//
//    bool TinygltfMeshLoader::InitFromScene(Mesh& mesh, const tinygltf::Model& model, const std::string& fileName);
//};
