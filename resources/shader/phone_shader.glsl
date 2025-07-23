#type vertex
#version 330 core

// 本地xyz坐标
layout (location = 0) in vec3 a_pos;
// uv坐标
layout (location = 1) in vec2 a_uv;
// 法线
layout (location = 2) in vec3 a_normal;

// 把顶点转换为世界坐标系
uniform mat4 u_model;
// 法线矩阵
uniform mat3 u_normalMatrix;
// 摄像机矩阵 转换成摄像机坐标系
uniform mat4 u_view;
// 透视投影矩阵 将摄像机坐标转换为剪裁坐标
uniform mat4 u_projection;

out vec2 uv;
out vec3 color;
out vec3 normal;
// 世界坐标
out vec3 worldPos;

void main()
{
    // 世界坐标
    vec4 transformPos = u_model * vec4(a_pos, 1.0f);
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
#version 330 core

// uv坐标
in vec2 uv;
// 法线
in vec3 normal;
// 世界坐标
in vec3 worldPos;

// 采样器
uniform sampler2D u_sampler;
// 光源参数
uniform vec3 u_lightDirection;
uniform vec3 u_lightColor;
// 相机位置
uniform vec3 u_cameraPos;
// 控制高光反射强度
uniform float u_specularIntensity;
// 环境光
uniform vec3 u_ambientColor;
// 控制光斑大小
uniform float u_shiness;

out vec4 fragColor;

void main()
{
    // 采样
    vec3 objectColor = texture(u_sampler, uv).rgb;
    // 法线向量归一
    vec3 normalN = normalize(normal);
    vec3 lightDirN = normalize(u_lightDirection);
    // 计算漫反射 过滤负数 保证输出在0到1之间 得到的是平行光跟法线夹角的cos cos角度越大值越小 最终物体颜色越小
    float diffuse = clamp(dot(-lightDirN, normalN), 0.0f, 1.0f);
    vec3 diffuseColor = u_lightColor * diffuse * objectColor;
    // 镜面反射
    vec3 lightReflect = normalize(reflect(lightDirN, normalN));
    // 视线向量
    vec3 viewDir = normalize(worldPos - u_cameraPos);
    viewDir = clamp(-viewDir, 0.0f, 1.0f);
    // 光照和法线夹角的cos值
    float cosVal = dot(-lightDirN, normalN);
    // step函数 cosVal小于0就用0 大于0就用1.0 下面把flag作用到反射高光上0就达到了过滤效果
    float flag = step(0.0, cosVal);
    // 从光照北面看物体高光的剔除 光线照射和法线夹角钝角就是背面情况 此时cos<0
    // 反射方向跟目光方向夹角的cos 夹角越大 看到的反射高光越弱
    float specular = max(dot(lightReflect, -viewDir) ,0.0f);
    // cos指数次方 让cos函数趋于高斯函数形状 让光斑变小变集中 控制高光反射的光班
    specular = pow(specular, u_shiness);
    vec3 specularColor = u_lightColor * specular * flag * u_specularIntensity;
    // 为了避免光照背面的死黑 添加环境光
    vec3 ambientColor = objectColor * u_ambientColor;
    vec3 finalColor = diffuseColor + specularColor + ambientColor;
    fragColor = vec4(finalColor, 1.0f);
}