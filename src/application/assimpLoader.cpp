//
// Created by dingrui on 25-7-29.
//

#include <ostream>
#include <filesystem>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "glframework/tool/tool.h"
#include "glframework/Object.h"
#include "glframework/Mesh.h"
#include "glframework/Texture.h"
#include "glframework/geo/Model.h"
#include "glframework/material/PhongMaterial.h"
#include "application/assimpLoader.h"
#include "x_log.h"

std::shared_ptr<Object> AssimpLoader::load(const std::string &path) {
    // for example, the path is `asset/fbx/monkey.fbx`, the parent path is `asset/fbx/`
    std::size_t lastIndex = path.find_last_of("\\/");
    auto rootPath = path.substr(0, lastIndex + 1);
    std::shared_ptr<Object> root = std::make_shared<Object>();
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);
    // 验证
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        XLOG_ERROR("模型读取失败");
        return nullptr;
    }
    processNode(scene->mRootNode, root, scene, rootPath);
    return root;
}

void AssimpLoader::processNode(aiNode *aiNode, const std::shared_ptr<Object> &parent, const aiScene *aiScene,
                               const std::string &textureParentPath) {
    // 创建当前节点
    std::shared_ptr<Object> myNode = std::make_shared<Object>();
    // 加入父节点
    parent->AddChild(myNode);
    auto localMatrix = getMat4f(aiNode->mTransformation);
    // 解构位置 缩放 旋转
    glm::vec3 pos, eulerAngle, scale;
    Tool::decompose(localMatrix, pos, eulerAngle, scale);
    myNode->set_position(pos);
    myNode->SetAngleX(eulerAngle.x);
    myNode->SetAngleX(eulerAngle.y);
    myNode->SetAngleX(eulerAngle.z);
    myNode->set_scale(scale);
    // 检查mesh
    for (int i = 0, sz = aiNode->mNumMeshes; i < sz; i++) {
        int meshId = aiNode->mMeshes[i];
        aiMesh *aiMesh = aiScene->mMeshes[meshId];
        std::shared_ptr<Mesh> myMesh = processMesh(aiMesh, aiScene, textureParentPath);
        myNode->AddChild(myMesh);
    }
    // 孩子
    for (int i = 0, sz = aiNode->mNumChildren; i < sz; i++) {
        processNode(aiNode->mChildren[i], myNode, aiScene, textureParentPath);
    }
}

glm::mat4 AssimpLoader::getMat4f(aiMatrix4x4 val) {
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

std::shared_ptr<Mesh> AssimpLoader::processMesh(aiMesh *aiMesh, const aiScene *aiScene,
                                                const std::string &textureParentPath) {
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    for (int i = 0, sz = aiMesh->mNumVertices; i < sz; i++) {
        // 顶点
        vertices.push_back(aiMesh->mVertices[i].x);
        vertices.push_back(aiMesh->mVertices[i].y);
        vertices.push_back(aiMesh->mVertices[i].z);
        // uv 第0套uv通常为贴图
        if (aiMesh->mTextureCoords[0]) {
            vertices.push_back(aiMesh->mTextureCoords[0][i].x);
            vertices.push_back(aiMesh->mTextureCoords[0][i].y);
        } else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
        // 法线
        vertices.push_back(aiMesh->mNormals[i].x);
        vertices.push_back(aiMesh->mNormals[i].y);
        vertices.push_back(aiMesh->mNormals[i].z);
    }
    // 索引
    for (int i = 0, sz = aiMesh->mNumFaces; i < sz; i++) {
        aiFace face = aiMesh->mFaces[i];
        for (int j = 0, cnt = face.mNumIndices; j < cnt; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    std::shared_ptr<Model> geometry = std::make_shared<Model>(
        vertices, indices, static_cast<VertexLayout>(VertexAttr::Position | VertexAttr::TexCoord | VertexAttr::Normal));
    std::shared_ptr<PhongMaterial> material = std::make_shared<PhongMaterial>();
    if (aiMesh->mMaterialIndex >= 0) {
        aiMaterial *aiMaterial = aiScene->mMaterials[aiMesh->mMaterialIndex];
        Texture *diffuseTexture = processTexture(aiMaterial, aiTextureType_DIFFUSE, aiScene, textureParentPath);
        if (diffuseTexture) {
            diffuseTexture->SetUint(0);
            material->set_diffuse(diffuseTexture);
        }
        Texture *specularMask = processTexture(aiMaterial, aiTextureType_SPECULAR, aiScene, textureParentPath);
        if (specularMask) {
            specularMask->SetUint(1);
            material->set_specular_mask(specularMask);
        }
    }
    return std::make_shared<Mesh>(geometry, material);
}

Texture *AssimpLoader::processTexture(const aiMaterial *aiMaterial, const aiTextureType &type, const aiScene *aiScene,
                                      const std::string &textureParentPath) {
    // this path is relative to model
    aiString aiPath;
    aiMaterial->Get(AI_MATKEY_TEXTURE(type, 0), aiPath);
    if (!aiPath.length) {
        return nullptr;
    }
    std::string s = aiPath.C_Str();
    std::replace(s.begin(), s.end(), '\\', '/');
    // Boundary conversion
    std::filesystem::path texPath = std::filesystem::path(s);
    // Combine with model directory
    texPath = (textureParentPath / texPath).lexically_normal();
    const aiTexture *aiTexture = aiScene->GetEmbeddedTexture(texPath.string().c_str());
    Texture *texture = nullptr;
    if (aiTexture) {
        // the texture is in memory
        unsigned char *dataIn = reinterpret_cast<unsigned char *>(aiTexture->pcData);
        uint32_t width = aiTexture->mWidth;
        uint32_t height = aiTexture->mHeight;
        texture = Texture::CreateTexture(texPath.string(), dataIn, width, height, 0);
    } else {
        // the texture is in disk
        std::string fullPath = textureParentPath + aiPath.C_Str();
        texture = Texture::CreateTexture(fullPath, 0);
    }
    return texture;
}
