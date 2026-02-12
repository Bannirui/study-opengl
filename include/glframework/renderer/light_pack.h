//
// Created by rui ding on 2026/1/6.
//

#pragma once

#include "glframework/light/DirectionalLight.h"
#include "glframework/light/PointLight.h"
#include "glframework/light/SpotLight.h"
#include "glframework/light/AmbientLight.h"

// 给renderer用 只需要给renderer1个相机和光源 让它渲染就行
// 至于光源具体有哪些 应该包装起来不用关注 也便于以后添加光源
struct LightPack
{
    // 平行光
    std::unique_ptr<DirectionalLight> directional;
    // 点光
    std::unique_ptr<PointLight> point;
    // 聚光
    std::unique_ptr<SpotLight> spot;
    // 环境光
    std::unique_ptr<AmbientLight> ambient;

    bool HasDirection() const { return directional != nullptr; }
    bool HasPoint() const { return point != nullptr; }
    bool HasSpot() const { return spot != nullptr; }
    bool HasAmbient() const { return ambient != nullptr; }
};