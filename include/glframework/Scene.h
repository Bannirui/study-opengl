//
// Created by rui ding on 2025/7/28.
//

#pragma once

#include "glframework/Object.h"

// 基于场景的渲染 Object不用渲染 Mesh才渲染
class Scene : public Object
{
public:
    Scene();
    ~Scene();
};
