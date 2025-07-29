//
// Created by dingrui on 25-7-29.
//

#pragma once

#include <string>

#include <glm/glm.hpp>
#include <assimp/scene.h>

class Object;
class Mesh;

class AssimpLoader
{
public:
    static Object* load(const std::string& path);

private:
    static void      processNode(aiNode* node, Object* parent, const aiScene* scene);
    static glm::mat4 getMat4f(aiMatrix4x4 val);
    static Mesh*     processMesh(aiMesh* aiMesh);
};
