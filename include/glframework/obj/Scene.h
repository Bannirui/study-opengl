//
// Created by rui ding on 2025/7/28.
//

#pragma once

#include "Object.h"

// 场景就是游戏中所有物体的集合 基于场景的渲染 Object不用渲染 Mesh才渲染
// 场景的渲染就是dfs递归
class Scene : public Object {
public:
    Scene();
};
