#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>
#include <assimp/cimport.h>
#include <directxmath.h>
#include <SimpleMath.h>

#include <iostream>
#include <vector>

class ModelManager
{
public:
#pragma pack(push, 4)
    struct Vertex
    {
        DirectX::SimpleMath::Vector4 position;
        DirectX::SimpleMath::Vector4 normal;
    };
#pragma pack(pop)

    struct Mesh
    {
        std::vector<Vertex> vertices{};
        std::vector<int> indices{};
    };

public:
    const std::vector<Mesh>& GetMeshes() 
    {
        return meshes;
    }

    bool Load(const std::string& path, DirectX::SimpleMath::Vector3 _color)
    {
        meshes.clear();
        Assimp::Importer importer;
        const auto scene = importer.ReadFile(path,
            aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_FlipWindingOrder | aiProcess_CalcTangentSpace);

        if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) 
        {
            std::cout << importer.GetErrorString() << std::endl;
            return false;
        }

        ProcessNode(scene, scene->mRootNode, _color);
        return true;
    }

private:
    std::vector<Mesh> meshes;

private:
    void ProcessNode(const aiScene* scene, aiNode* node, DirectX::SimpleMath::Vector3 _color)
    {
        const auto numMeshes = node->mNumMeshes;
        const auto numChildren = node->mNumChildren;

        for (size_t i = 0; i < numMeshes; i++)
            meshes.push_back(ProcessMesh(scene, scene->mMeshes[node->mMeshes[i]], _color));

        for (size_t i = 0; i < numChildren; i++) ProcessNode(scene, node->mChildren[i], _color);
    }

    Mesh ProcessMesh(const aiScene* scene, aiMesh* mesh, DirectX::SimpleMath::Vector3 _color)
    {
        std::vector<Vertex> vertices;
        std::vector<int> indices;

        for (size_t i = 0; i < mesh->mNumVertices; ++i) {
            Vertex vertex;
            vertex.position = ToVector4(mesh->mVertices[i], 1);

            if (mesh->HasNormals()) vertex.normal = ToVector4(mesh->mNormals[i], 0);


            vertices.push_back(vertex);
        }

        for (size_t i = 0; i < mesh->mNumFaces; i++) 
        {
            const auto face = mesh->mFaces[i];

            for (size_t j = 0; j < face.mNumIndices; ++j) indices.push_back(face.mIndices[j]);
        }

        return Mesh(vertices, indices);
    }

    static DirectX::SimpleMath::Vector2 ToVector2(aiVector3D vector) {
        return { vector.x, vector.y };
    }

    static DirectX::SimpleMath::Vector3 ToVector3(aiVector3D vector) 
    {
        return { vector.x, vector.y, vector.z };
    }

    static DirectX::SimpleMath::Vector4 ToVector4(aiVector3D vector, float _w)
    {
        return { vector.x, vector.y, vector.z, _w };
    }

    static DirectX::SimpleMath::Vector4 ToVector4(DirectX::SimpleMath::Vector3 vector, float _w)
    {
        return { vector.x, vector.y, vector.z, _w };
    }
};

