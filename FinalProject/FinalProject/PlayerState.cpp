#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "PlayerState.h"

PlayerState::PlayerState(float SS, float FD, float R) {
	shotSpeed = SS;
	FireDelay = FD;
	range = R;
	life = 3;
	cursorPos = glm::vec3(400.0f, 300.0f, range);
}

void PlayerState::GetHit() {
	life--;
}

glm::vec3 PlayerState::GetCursor() {
	return cursorPos;
}


void PlayerState::updateCursor(float xPos, float yPos) {
	cursorPos.x = xPos;
	cursorPos.y = yPos;
}

float PlayerState::GetFireDelay() {
	return FireDelay;
}

float PlayerState::GetShotSpeed() {
	return shotSpeed;
}