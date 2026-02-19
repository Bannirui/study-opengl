#type vertex
// 占位用 OpenGL的真正版本在编译的时候根据平台动态替换掉
#version xxx

// 本地xyz坐标
layout (location = 0) in vec3 a_pos;
// uv坐标
layout (location = 1) in vec2 a_uv;
// 法线
layout (location = 2) in vec3 a_normal;

out vec2 uv;
out vec3 color;
out vec3 normal;
// 世界坐标
out vec3 worldPos;

// 把顶点转换为世界坐标系
uniform mat4 u_model;
// 法线矩阵
uniform mat3 u_normalMatrix;
// 摄像机矩阵 转换成摄像机坐标系
uniform mat4 u_view;
// 透视投影矩阵 将摄像机坐标转换为剪裁坐标
uniform mat4 u_projection;
uniform mat4 u_matrices[100];

void main()
{
    // 世界坐标
    vec4 transformPos = vec4(a_pos, 1.0f);
    transformPos = u_model * u_matrices[gl_InstanceID] * transformPos;
    worldPos = transformPos.xyz;
    // 剪裁坐标
    gl_Position = u_projection * u_view * transformPos;
    uv = a_uv;
    // 法线不能直接传给fs 在发生变换的时候要把变换作用到法线上 根据model矩阵求出法线矩阵
    // 法线矩阵=(model矩阵的逆矩阵)转置
    // 经过法线矩阵作用之后的法线
    normal = u_normalMatrix * a_normal;
}

#type fragment
// 占位用 OpenGL的真正版本在编译的时候根据平台动态替换掉
#version xxx

// 平行光
struct DirectionalLight {
    // 照射方向
    vec3 direction;
    // 光源的颜色
    vec3 color;
    // 高光反射强度
    float specularIntensity;
};
// 点光源
struct PointLight {
    vec3 pos;
    vec3 color;
    float specularIntensity;
    // 衰减系数
    float k2;
    float k1;
    float kc;
};
// 探照光
struct SpotLight {
    // 光源位置
    vec3 pos;
    // 光照向的方向
    vec3 targetDirection;
    // 光源的颜色强度
    vec3 color;
    // 用两个可视角度范围控制过渡 避免边缘锐化
    float innerCos;
    float outerCos;
    // 高光反射强度
    float specularIntensity;
};

// uv坐标
in vec2 uv;
// 法线
in vec3 normal;
// 世界坐标
in vec3 worldPos;

// 采样器 diffuse贴图
uniform sampler2D u_diffuseSampler;
// 采样器 高光蒙版采样器
uniform sampler2D u_specularMaskSampler;
// 相机位置
uniform vec3 u_cameraPos;
uniform float u_shiness;
uniform bool u_activeSpotLight;
uniform SpotLight u_spotLight;
uniform bool u_activeDirectionalLight;
uniform DirectionalLight u_directionalLight;
uniform bool u_activePointLight;
uniform PointLight u_pointLight;
// 环境光
uniform vec3 u_ambientColor;

uniform float u_opacity;

out vec4 fragColor;

/**
 * 计算漫反射光照 光打到物体上反射的颜色
 * @param objColor 物体颜色
 * @param lightDir 光照方向 已经归一化好的
 * @param lightColor 光照颜色
 * @param normal 法线 归一化好的
 */
vec3 calDiffuse(vec3 objColor, vec3 lightDir, vec3 lightColor, vec3 normal) {
    // 计算漫反射 过滤负数 保证输出在0到1之间 得到的是平行光跟法线夹角的cos cos角度越大值越小 最终物体颜色越小
    float diffuse = clamp(dot(-lightDir, normal), 0.0f, 1.0f);
    vec3 diffuseColor = lightColor * diffuse * objColor;
    return diffuseColor;
}

/**
 * 计算镜面反射光照
 * @param lightDir 光源方向
 * @param color 光源颜色
 * @param normal 法线
 * @param viewDir 视线方向
 */
vec3 calSpecular(vec3 lightDir, vec3 color, vec3 normal, vec3 viewDir, float intensity) {
    // 防止背面光效果
    // 光照和法线夹角的cos值
    float cosVal = dot(-lightDir, normal);
    // step函数 cosVal小于0就用0 大于0就用1.0 下面把flag作用到反射高光上0就达到了过滤效果
    float flag = step(0.0, cosVal);
    // 镜面反射
    vec3 lightReflect = normalize(reflect(lightDir, normal));
    // 从光照背面看物体高光的剔除 光线照射和法线夹角钝角就是背面情况 此时cos<0 反射方向跟目光方向夹角的cos 夹角越大 看到的反射高光越弱
    float specular = max(dot(lightReflect, -viewDir) ,0.0f);
    // cos指数次方 让cos函数趋于高斯函数形状 让光斑变小变集中 控制高光反射的光班
    specular = pow(specular, u_shiness);
    // 对高光蒙版贴图采样 用R通道作为高光比例
    float specularMask = texture(u_specularMaskSampler, uv).r;
    vec3 specularColor = color * specular * flag * intensity * specularMask;
    return specularColor;
}

/**
 * 计算探照灯光源
 * @param light 探照灯
 * @param normal 法线归一向量
 * @param viewDir 视线归一向量
 */
vec3 calSpotLight(SpotLight light, vec3 normal, vec3 viewDir) {
    // 采样
    vec3 objectColor = texture(u_diffuseSampler, uv).rgb;
    // 光源照射方向
    vec3 lightDir = normalize(worldPos - light.pos);
    vec3 targetDir = normalize(light.targetDirection);
    // 探照灯的照射范围 spot light可视范围 gl中用的是弧度
    // cosTheta>cosVisible也就是theat<u_spotAngle时候才是可视的 这种方式太锐化 用两个角度来过渡
    float cosGamma = dot(lightDir, targetDir);
    float intensity = clamp((cosGamma - light.outerCos) / (light.innerCos - u_spotLight.outerCos), 0.0f, 1.0f);
    // diffuse
    vec3 diffuseColor = calDiffuse(objectColor, lightDir, light.color, normal);
    // specular
    vec3 specularColor = calSpecular(lightDir, light.color, normal, viewDir, light.specularIntensity);
    return (diffuseColor+specularColor)*intensity;
}

/**
 * 计算平行光源
 * @param light 平行灯
 * @param normal 法线归一向量
 * @param viewDir 视线归一向量
 */
vec3 calDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    // 采样
    vec3 objectColor = texture(u_diffuseSampler, uv).rgb;
    // 光源照射方向
    vec3 lightDir = normalize(light.direction);
    // diffuse
    vec3 diffuseColor = calDiffuse(objectColor, lightDir, light.color, normal);
    // specular
    vec3 specularColor = calSpecular(lightDir, light.color, normal, viewDir, light.specularIntensity);
    return diffuseColor+specularColor;
}

/**
 * 计算点光源
 * @param light 点光源
 * @param normal 法线归一向量
 * @param viewDir 视线归一向量
 */
vec3 calPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    // 采样
    vec3 objectColor = texture(u_diffuseSampler, uv).rgb;
    // 光源照射方向
    vec3 lightDir = normalize(worldPos - light.pos);
    // 光源和像素之间距离
    float dist = length(worldPos - light.pos);
    // 点光源衰减
    float attenuation = 1.0/(light.k2*dist*dist + light.k1*dist + light.kc);
    // diffuse
    vec3 diffuseColor = calDiffuse(objectColor, lightDir, light.color, normal);
    // specular
    vec3 specularColor = calSpecular(lightDir, light.color, normal, viewDir, light.specularIntensity);
    return (diffuseColor+specularColor)*attenuation;
}

void main()
{
    // 计算光照的通用数据
    // 采样
    vec3 objectColor = texture(u_diffuseSampler, uv).rgb;
    float alpha = texture(u_diffuseSampler, uv).a;
    // 法线向量归一
    vec3 normalN = normalize(normal);
    // 光源照射方向
    vec3 lightDirN = normalize(worldPos - u_spotLight.pos);
    // 视线向量
    vec3 viewDirN = normalize(worldPos - u_cameraPos);
    vec3 targetDirN = normalize(u_spotLight.targetDirection);
    vec3 ret = vec3(0.0f, 0.0f, 0.0f);
    if (u_activeSpotLight)
        ret += calSpotLight(u_spotLight, normalN, viewDirN);
    if (u_activeDirectionalLight)
        ret += calDirectionalLight(u_directionalLight, normalN, viewDirN);
    if (u_activePointLight)
        ret += calPointLight(u_pointLight, normalN, viewDirN);
    // 为了避免光照背面的死黑 添加环境光
    vec3 ambientColor = objectColor * u_ambientColor;
    vec3 finalColor = ret + ambientColor;
    fragColor = vec4(finalColor, alpha * u_opacity);
}