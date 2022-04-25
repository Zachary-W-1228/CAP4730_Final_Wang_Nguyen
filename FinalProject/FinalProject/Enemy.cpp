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
	if (EnemyID == 0)
		life = 10;
	else if (EnemyID == 1)
		life = 5;
	Pitch = Direction.y;
	Yaw = Direction.x;
	Roll = Direction.z;
}

void Enemy::UpdatePos(float deltatime) {
	float speed = 1.0f;
	if (EnemyID == 0)
		speed = 30.0f;
	if (EnemyID == 1)
		speed = 50.0f;
	Position = Position + deltatime * Direction * speed;
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

float Enemy::GetPitch() {
	return Pitch;
}

float Enemy::GetYaw() {
	return Yaw;
}

float Enemy::GetRoll() {
	return Roll;
}