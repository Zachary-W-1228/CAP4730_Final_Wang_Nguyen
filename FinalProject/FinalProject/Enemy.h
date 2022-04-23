#pragma once

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

class Enemy {
public:
	Enemy(vec3 Pos, vec3 Tar, int ID); // Pos is initial position, Tar is target position
	void UpdatePos(float deltatime); // update enemy position
	vec3 GetPos(); // get position
	int getID();
	void getHit();
	int getLife();
private:
	vec3 Position;
	vec3 Direction;
	int life;
	int EnemyID; // 0 is Meteorite, 1 is plane
};