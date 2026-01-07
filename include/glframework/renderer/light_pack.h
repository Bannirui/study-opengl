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
    std::shared_ptr<DirectionalLight> directional;
    // 点光
    std::shared_ptr<PointLight> point;
    // 聚光
    std::shared_ptr<SpotLight> spot;
    // 环境光
    std::shared_ptr<AmbientLight> ambient;
};