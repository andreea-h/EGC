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

float Player::getXCoord() {
	return playerCoords.x;
}

void Player::setXCoord(float x) {
	playerCoords.x = x;
}

float Player::getYCoord() {
	return playerCoords.y;
}

void Player::setYCoord(float y) {
	playerCoords.y = y;
}