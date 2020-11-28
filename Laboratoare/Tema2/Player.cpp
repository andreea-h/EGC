#include "Player.h"

#include <vector>
#include <iostream>
#include <Core/Engine.h>

Player::Player() {

}

Player::~Player()
{

}

glm::vec3 Player::getPlayerCoords() {
	return playerCoords;
}
