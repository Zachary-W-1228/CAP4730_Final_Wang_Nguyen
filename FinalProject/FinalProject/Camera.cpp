#pragma once

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"

using namespace glm;

Camera::Camera() {
    Position = vec3(0.0f, 0.0f, 0.0f);
    WorldUp = vec3(0.0f, 1.0f, 0.0f);
    updateCameraVectors();
}

mat4 Camera::GetViewMatrix()
{
    updateCameraVectors();
    return lookAt(Position, Position + Front, Up);
}

vec3 Camera::GetViewPos() {
    return Position;
}

void Camera::updateCameraVectors()
{
    vec3 front;
    front = normalize(vec3(0.0f, 0.0f, 1.0f) - Position);
    Front = normalize(front);
    Right = normalize(cross(WorldUp, Front));
    Up = normalize(cross(Front, Right));
}

