#pragma once

#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <assimp/scene.h>

class Object;
class Texture;
class InstanceMeshByAttribute;

class AssimpInstanceLoader
{
public:
    static std::unique_ptr<Object> Load(const std::string& path, const std::vector<glm::mat4>& instanceMatrices);

private:
    static void processNode(aiNode* aiNode, Object& parent, const aiScene* aiScene,
                            const std::string& textureParentPath, const std::vector<glm::mat4>& instanceMatrices);

    static glm::mat4 getMat4f(aiMatrix4x4 val);

    static std::unique_ptr<InstanceMeshByAttribute> processMesh(aiMesh* aiMesh, const aiScene* aiScene,
                                                                const std::string&            texturePath,
                                                                const std::vector<glm::mat4>& instanceMatrices);

    /**
     * @return 非owning指针
     */
    static std::shared_ptr<Texture> processTexture(const aiMaterial* aiMaterial, const aiTextureType& type,
                                                   const aiScene* aiScene, const std::string& textureParentPath);
};
