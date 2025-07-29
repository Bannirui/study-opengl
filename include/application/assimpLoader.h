//
// Created by dingrui on 25-7-29.
//

#pragma once

#include <assimp/scene.h>

#include "glframework/Object.h"
class AssimpLoader
{
public:
    static Object* load(const std::string& path);

private:
    static void      processNode(aiNode* node, Object* parent);
    static glm::mat4 getMat4f(aiMatrix4x4 val);
};
