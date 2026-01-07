//
// Created by rui ding on 2026/1/6.
//

#pragma once

class DirectionalLight;
class PointLight;
class SpotLight;
class AmbientLight;

// 给renderer用 只需要给renderer1个相机和1给光源 让它渲染就行
struct LightPack
{
    // 平行光
    const DirectionalLight* directional = nullptr;
    // 点光
    const PointLight* point = nullptr;
    // 聚光
    const SpotLight* spot = nullptr;
    // 环境光
    const AmbientLight* ambient = nullptr;
};