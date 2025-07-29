//
// Created by dingrui on 25-7-29.
//

#include "application/assimpLoader.h"

#include <iostream>
#include <ostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "glframework/tool/tool.h"
#include "glframework/Object.h"
#include "glframework/Mesh.h"
#include "glframework/Texture.h"
#include "glframework/geo/Model.h"
#include "glframework/material/PhoneMaterial.h"

Object* AssimpLoader::load(const std::string& path)
{
    Object*          root = new Object();
    Assimp::Importer importer;
    const aiScene*   scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);
    // 验证
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "模型读取失败" << std::endl;
        return nullptr;
    }
    processNode(scene->mRootNode, root, scene);
    return root;
}

void AssimpLoader::processNode(aiNode* node, Object* parent, const aiScene* scene)
{
    Object* myNode = new Object();
    parent->AddChild(myNode);
    auto localMatrix = getMat4f(node->mTransformation);
    // 解构位置 缩放 旋转
    glm::vec3 pos, eulerAngle, scale;
    Tool::decompose(localMatrix, pos, eulerAngle, scale);
    myNode->SetPosition(pos);
    myNode->SetAngleX(eulerAngle.x);
    myNode->SetAngleX(eulerAngle.y);
    myNode->SetAngleX(eulerAngle.z);
    myNode->SetScale(scale);
    // 检查mesh
    for (int i = 0, sz = node->mNumMeshes; i < sz; i++)
    {
        int     meshId = node->mMeshes[i];
        aiMesh* mesh   = scene->mMeshes[meshId];
        auto    myMesh = processMesh(mesh);
        myNode->AddChild(myMesh);
    }
    // 孩子
    for (int i = 0, sz = node->mNumChildren; i < sz; i++)
    {
        processNode(node->mChildren[i], myNode, scene);
    }
}

glm::mat4 AssimpLoader::getMat4f(aiMatrix4x4 val)
{
    // clang-format off
    glm::mat4 ret(
        val.a1, val.a2, val.a3, val.a4,
        val.b1, val.b2, val.b3, val.b4,
        val.c1, val.c2, val.c3, val.c4,
        val.d1, val.d2, val.d3, val.d4
        );
    // clang-format on
    return ret;
}
Mesh* AssimpLoader::processMesh(aiMesh* aiMesh)
{
    std::vector<float>    vertices;
    std::vector<uint32_t> indices;
    for (int i = 0, sz = aiMesh->mNumVertices; i < sz; i++)
    {
        // 顶点
        vertices.push_back(aiMesh->mVertices[i].x);
        vertices.push_back(aiMesh->mVertices[i].y);
        vertices.push_back(aiMesh->mVertices[i].z);
        // uv 第0套uv通常为贴图
        if (aiMesh->mTextureCoords[0])
        {
            vertices.push_back(aiMesh->mTextureCoords[0][i].x);
            vertices.push_back(aiMesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
        // 法线
        vertices.push_back(aiMesh->mNormals[i].x);
        vertices.push_back(aiMesh->mNormals[i].y);
        vertices.push_back(aiMesh->mNormals[i].z);
    }
    // 索引
    for (int i = 0, sz = aiMesh->mNumFaces; i < sz; i++)
    {
        aiFace face = aiMesh->mFaces[i];
        for (int j = 0, cnt = face.mNumIndices; j < cnt; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    auto geometry = new Model(
        vertices, indices, static_cast<VertexLayout>(VertexAttr::Position | VertexAttr::TexCoord | VertexAttr::Normal));
    auto material       = new PhoneMaterial();
    material->m_diffuse = new Texture("asset/texture/wall.jpg", 0);
    return new Mesh(geometry, material);
}
