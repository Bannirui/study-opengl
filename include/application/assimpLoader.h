//
// Created by dingrui on 25-7-29.
//

#pragma once

#include <string>

#include <glm/glm.hpp>
#include <assimp/scene.h>

class Object;
class Mesh;
class Texture;

class AssimpLoader {
public:
    static std::unique_ptr<Object> load(const std::string &path);

private:
    static void processNode(aiNode *aiNode, Object &parent, const aiScene *aiScene,
                            const std::string &textureParentPath);

    static glm::mat4 getMat4f(aiMatrix4x4 val);

    static std::unique_ptr<Mesh> processMesh(aiMesh *aiMesh, const aiScene *aiScene,
                                             const std::string &textureParentPath);

    static Texture *processTexture(const aiMaterial *aiMaterial, const aiTextureType &type, const aiScene *aiScene,
                                   const std::string &textureParentPath);
};
