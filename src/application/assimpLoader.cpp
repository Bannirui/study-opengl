//
// Created by dingrui on 25-7-29.
//

#include "application/assimpLoader.h"

#include "glframework/tool/tool.h"

#include <iostream>
#include <ostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

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
    processNode(scene->mRootNode, root);
    return root;
}

void AssimpLoader::processNode(aiNode* node, Object* parent)
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
    // 孩子
    for (int i = 0, sz = node->mNumChildren; i < sz; i++)
    {
        processNode(node->mChildren[i], myNode);
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