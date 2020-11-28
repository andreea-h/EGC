#pragma once

#include <string>
#include <Component/SimpleScene.h>
#include <Core/Engine.h>

class Player 
{
public:
	Player();
	~Player();

	glm::vec3 getPlayerCoords();

private:
	glm::vec3 playerCoords = glm::vec3(0, 0.35f, -0.5f);
};