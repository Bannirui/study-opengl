//
// Created by dingrui on 25-7-26.
//

#include "glframework/light/SpotLight.h"
#include "glm/fwd.hpp"

SpotLight::SpotLight()
    : Object(kObject),
      m_targetDirection(glm::vec3(-1.0f, 0.0f, 0.0f)),
      m_innerAngle(45.0f),
      m_outerAngle(60.0f)
{
}
