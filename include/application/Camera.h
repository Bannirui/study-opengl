//
// Created by rui ding on 2025/7/1.
//

#pragma once

#include <memory>
#include <GLAD/glad.h>
#include <glm/glm.hpp>

enum Camera_Movement {
    FORWARD, BACKWARD, LEFT, RIGHT,
};

// 默认值
const float YAW         = -90.0f;
const float PITCH       =   0.0f;
const float SPEED       =   2.5f;
const float SENSITIVITY =   0.1f;
const float ZOOM        =  45.0f;

class Camera
{
    public:
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;
        float Yaw;
        float Pitch;
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;
    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f,1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
        ~Camera();
        glm::mat4 GetViewMatrix();
        void ProcessKeyboard(Camera_Movement direction, float deltaTime);
        void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
        void ProcessMouseScroll(float yOffset);
    private:
        void updateCameraVectors();
};
