#pragma once

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Enemy.h"

using namespace glm;

Enemy::Enemy(vec3 Pos, vec3 Tar, int ID) {
	Position = Pos;
	Direction = normalize(Tar - Pos);
	EnemyID = ID;
}

void Enemy::UpdatePos(float deltatime) {
	Position = Position + deltatime * Direction;
}

vec3 Enemy::GetPos() {
	return Position;
}

int Enemy::getID() {
	return EnemyID;
}

void Enemy::getHit() {
	life--;
}

int Enemy::getLife() {
	return life;
}