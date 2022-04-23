#pragma once

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Bullet.h"

using namespace glm;

Bullet::Bullet(vec3 Pos, vec3 Tar, float Speed) {
	Position = Pos;
	Direction = normalize(Tar - Pos);
	ShotSpeed = Speed;
}

void Bullet::UpdatePos(float deltatime) {
	Position = Position + deltatime * ShotSpeed * Direction;
}

vec3 Bullet::GetPos() {
	return Position;
}
