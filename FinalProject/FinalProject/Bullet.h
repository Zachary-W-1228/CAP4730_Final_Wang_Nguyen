#pragma once

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

class Bullet {
public:
	Bullet(vec3 Pos, vec3 Tar, float Speed); // Pos is initial position, Tar is target position
	void UpdatePos(float deltatime); // update enemy position
	vec3 GetPos();
private:
	vec3 Position;
	vec3 Direction;
	float ShotSpeed;
};