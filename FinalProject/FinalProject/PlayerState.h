#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PlayerState {
private:
	float shotSpeed;
	float FireDelay;
	float range;
	int life;
	glm::vec3 cursorPos;
public:
	PlayerState(float SS, float FD, float R);
	void GetHit();
	void updateCursor(float xPos, float yPos);
	glm::vec3 GetCursor();
	float GetFireDelay();
	float GetShotSpeed();
};