#include "application/assimp_instance_loader.h"

#include <filesystem>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "glframework/tool/tool.h"
#include "glframework/obj/Object.h"
#include "glframework/obj/mesh/Mesh.h"
#include "glframework/Texture.h"
#include "glframework/geo/Model.h"
#include "x_log.h"
#include "glframework/material/grass_instance_material.h"
#include "glframework/material/phong_instance_by_attribute_material.h"
#include "glframework/obj/mesh/instance_mesh_by_attribute.h"

std::unique_ptr<Object> AssimpInstanceLoader::Load(const std::string& path, const std::vector<glm::mat4>& instanceMatrices)
{
    // for example, the path is `asset/fbx/monkey.fbx`, the parent path is `asset/fbx/`
    std::size_t      lastIndex = path.find_last_of("\\/");
    auto             rootPath  = path.substr(0, lastIndex + 1);
    Assimp::Importer importer;
    const aiScene*   scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);
    // 验证
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        XLOG_ERROR("模型读取失败");
        return nullptr;
    }
    // create root, owned by caller
    std::unique_ptr<Object> root = std::make_unique<Object>();
    processNode(scene->mRootNode, *root, scene, rootPath, instanceMatrices);
    return root;
}

void AssimpInstanceLoader::processNode(aiNode* aiNode, Object& parent, const aiScene* aiScene,
                                       const std::string& textureParentPath, const std::vector<glm::mat4>& instanceMatrices)
{
    // 创建当前节点 加入父节点
    auto cur         = std::make_unique<Object>();
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
    for (int i = 0, sz = aiNode->mNumMeshes; i < sz; i++)
    {
        int                   meshId = aiNode->mMeshes[i];
        aiMesh*               aiMesh = aiScene->mMeshes[meshId];
        std::unique_ptr<Mesh> myMesh = processMesh(aiMesh, aiScene, textureParentPath, instanceMatrices);
        cur->AddChild(std::move(myMesh));
    }
    // 孩子
    for (int i = 0, sz = aiNode->mNumChildren; i < sz; i++)
    {
        processNode(aiNode->mChildren[i], *cur, aiScene, textureParentPath, instanceMatrices);
    }
    parent.AddChild(std::move(cur));
}

glm::mat4 AssimpInstanceLoader::getMat4f(aiMatrix4x4 val)
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

std::unique_ptr<InstanceMeshByAttribute> AssimpInstanceLoader::processMesh(aiMesh* aiMesh, const aiScene* aiScene, const std::string& texturePath,const std::vector<glm::mat4>& instanceMatrices)
{
    std::vector<float> vertices;
    // pos(3)+color(3)+uv(2)+normal(3)
    vertices.reserve(aiMesh->mNumVertices * 8);
    std::vector<uint32_t>  indices;
    Geometry::VertexLayout layout{};
    layout.posDim = Geometry::VertexPosDim::k3D;
    layout.attrs  = Geometry::VBOType::kNone;
    // vertices
    for (int i = 0, sz = aiMesh->mNumVertices; i < sz; i++)
    {
        // 顶点
        vertices.push_back(aiMesh->mVertices[i].x);
        vertices.push_back(aiMesh->mVertices[i].y);
        vertices.push_back(aiMesh->mVertices[i].z);
        if (aiMesh->HasVertexColors(0))
        {
            vertices.push_back(aiMesh->mColors[0][i].r);
            vertices.push_back(aiMesh->mColors[0][i].g);
            vertices.push_back(aiMesh->mColors[0][i].b);
            layout.attrs |= Geometry::VBOType::kColor;
        }
        // uv 第0套uv通常为贴图
        if (aiMesh->mTextureCoords[0])
        {
            vertices.push_back(aiMesh->mTextureCoords[0][i].x);
            vertices.push_back(aiMesh->mTextureCoords[0][i].y);
            layout.attrs |= Geometry::VBOType::kTexCoord;
        }
        // 法线
        vertices.push_back(aiMesh->mNormals[i].x);
        vertices.push_back(aiMesh->mNormals[i].y);
        vertices.push_back(aiMesh->mNormals[i].z);
        layout.attrs |= Geometry::VBOType::kNormal;
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
    std::unique_ptr<Model>         geometry = std::make_unique<Model>(vertices, indices, layout);
    auto material = std::make_unique<GrassInstanceMaterial>();
    if (aiMesh->mMaterialIndex >= 0)
    {
        aiMaterial* aiMaterial     = aiScene->mMaterials[aiMesh->mMaterialIndex];
        auto        diffuseTexture = processTexture(aiMaterial, aiTextureType_DIFFUSE, aiScene, texturePath);
        if (diffuseTexture)
        {
            diffuseTexture->set_uint(0);
            material->set_diffuse(diffuseTexture);
        }
        auto specularMask = processTexture(aiMaterial, aiTextureType_SPECULAR, aiScene, texturePath);
        if (specularMask)
        {
            specularMask->set_uint(1);
            material->set_specular_mask(specularMask);
        }
    }
    return std::make_unique<InstanceMeshByAttribute>(std::move(geometry), std::move(material), instanceMatrices);
}

std::shared_ptr<Texture> AssimpInstanceLoader::processTexture(const aiMaterial* aiMaterial, const aiTextureType& type,
                                                              const aiScene*     aiScene,
                                                              const std::string& textureParentPath)
{
    // this path is relative to model
    aiString aiPath;
    if (aiMaterial->Get(AI_MATKEY_TEXTURE(type, 0), aiPath) != AI_SUCCESS)
    {
        return nullptr;
    }
    // 相对
    std::string relative = aiPath.C_Str();
    std::replace(relative.begin(), relative.end(), '\\', '/');
    // 绝对
    std::filesystem::path fullPath = std::filesystem::path(textureParentPath) / relative;
    fullPath                       = fullPath.lexically_normal();
    std::string key                = fullPath.string();
    // 缓存
    if (Texture::s_TextureCache.count(key))
    {
        return Texture::s_TextureCache[key];
    }
    // 创建
    const aiTexture* embed = aiScene->GetEmbeddedTexture(aiPath.C_Str());
    if (embed)
    {
        // the texture is in memory
        unsigned char* dataIn = reinterpret_cast<unsigned char*>(embed->pcData);
        uint32_t       width  = embed->mWidth;
        uint32_t       height = embed->mHeight;
        return Texture::CreateTexture(key, dataIn, width, height, 0);
    }
    else
    {
        // the texture is in disk
        return Texture::CreateTexture(key, 0);
    }
}
