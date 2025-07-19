//
// Created by rui ding on 2025/7/1.
//

#pragma once

#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

// 默认值
const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 2.5f;
const float SENSITIVITY = 0.1f;
// 透视投影时y轴视张角
const float ZOOM        = 45.0f;

class Camera
{
   public:
    // 摄像机位置
    glm::vec3 m_Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float     Yaw;
    float     Pitch;
    float     MovementSpeed;
    float     MouseSensitivity;
    float     m_Zoom;

   public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    ~Camera();
    glm::mat4 GetViewMatrix();
    void      ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void      ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
    void      ProcessMouseScroll(float yOffset);

   private:
    void updateCameraVectors();
};
