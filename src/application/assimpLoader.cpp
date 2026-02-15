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

std::unique_ptr<Object> AssimpLoader::load(const std::string &path) {
    // for example, the path is `asset/fbx/monkey.fbx`, the parent path is `asset/fbx/`
    std::size_t lastIndex = path.find_last_of("\\/");
    auto rootPath = path.substr(0, lastIndex + 1);
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);
    // 验证
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        XLOG_ERROR("模型读取失败");
        return nullptr;
    }
    // create root, owned by caller
    std::unique_ptr<Object> root = std::make_unique<Object>();
    processNode(scene->mRootNode, *root, scene, rootPath);
    return root;
}

void AssimpLoader::processNode(aiNode *aiNode, Object &parent, const aiScene *aiScene,
                               const std::string &textureParentPath) {
    // 创建当前节点 加入父节点
    auto cur = std::make_unique<Object>();
    auto localMatrix = getMat4f(aiNode->mTransformation);
    // 解构位置 缩放 旋转
    glm::vec3 pos, eulerAngle, scale;
    Tool::decompose(localMatrix, pos, eulerAngle, scale);
    cur->set_position(pos);
    cur->SetAngleX(eulerAngle.x);
    cur->SetAngleY(eulerAngle.y);
    cur->SetAngleZ(eulerAngle.z);
    cur->set_scale(scale);
    // 检查mesh
    for (int i = 0, sz = aiNode->mNumMeshes; i < sz; i++) {
        int meshId = aiNode->mMeshes[i];
        aiMesh *aiMesh = aiScene->mMeshes[meshId];
        std::unique_ptr<Mesh> myMesh = processMesh(aiMesh, aiScene, textureParentPath);
        cur->AddChild(std::move(myMesh));
    }
    // 孩子
    for (int i = 0, sz = aiNode->mNumChildren; i < sz; i++) {
        processNode(aiNode->mChildren[i], *cur, aiScene, textureParentPath);
    }
    parent.AddChild(std::move(cur));
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

std::unique_ptr<Mesh> AssimpLoader::processMesh(aiMesh *aiMesh, const aiScene *aiScene,
                                                const std::string &textureParentPath) {
    std::vector<float> vertices;
    // pos(3)+uv(2)+normal(3)
    vertices.reserve(aiMesh->mNumVertices * 8);
    std::vector<uint32_t> indices;
    // vertices
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
    VertexLayout layout{};
    layout.posDim = VertexPosDim::k3D;
    layout.attrs = VertexAttr::kTexCoord | VertexAttr::kNormal;
    std::unique_ptr<Model> geometry = std::make_unique<Model>(vertices, indices, layout);
    std::unique_ptr<PhongMaterial> material = std::make_unique<PhongMaterial>();
    if (aiMesh->mMaterialIndex >= 0) {
        aiMaterial *aiMaterial = aiScene->mMaterials[aiMesh->mMaterialIndex];
        auto diffuseTexture = processTexture(aiMaterial, aiTextureType_DIFFUSE, aiScene, textureParentPath);
        if (diffuseTexture) {
            diffuseTexture->set_uint(0);
            material->set_diffuse(diffuseTexture);
        }
        auto specularMask = processTexture(aiMaterial, aiTextureType_SPECULAR, aiScene, textureParentPath);
        if (specularMask) {
            specularMask->set_uint(1);
            material->set_specular_mask(specularMask);
        }
    }
    return std::make_unique<Mesh>(std::move(geometry), std::move(material));
}

std::shared_ptr<Texture> AssimpLoader::processTexture(const aiMaterial *aiMaterial, const aiTextureType &type, const aiScene *aiScene,
                                      const std::string &textureParentPath) {
    // this path is relative to model
    aiString aiPath;
    if (aiMaterial->Get(AI_MATKEY_TEXTURE(type, 0), aiPath) != AI_SUCCESS) { return nullptr; }
    // 相对
    std::string relative = aiPath.C_Str();
    std::replace(relative.begin(), relative.end(), '\\', '/');
    // 绝对
    std::filesystem::path fullPath = std::filesystem::path(textureParentPath) / relative;
    fullPath = fullPath.lexically_normal();
    std::string key = fullPath.string();
    // 缓存
    if (Texture::s_TextureCache.count(key)) {
        return Texture::s_TextureCache[key];
    }
    // 创建
    const aiTexture *embed = aiScene->GetEmbeddedTexture(aiPath.C_Str());
    if (embed) {
        // the texture is in memory
        unsigned char *dataIn = reinterpret_cast<unsigned char *>(embed->pcData);
        uint32_t width = embed->mWidth;
        uint32_t height = embed->mHeight;
        return Texture::CreateTexture(key, dataIn, width, height, 0);
    } else {
        // the texture is in disk
        return Texture::CreateTexture(key, 0);
    }
}
