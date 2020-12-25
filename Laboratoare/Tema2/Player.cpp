#include "Player.h"

#include <vector>
#include <iostream>
#include <Core/Engine.h>

Player::Player() {

}

Player::~Player()
{

}

void Player::setActualCoords(glm::vec4 coords) {
	actualPlayerCoords = coords;
}

glm::vec4 Player::getActualPlayerCoords() {
	return actualPlayerCoords;
}

glm::vec3 Player::getInitialPlayerCoords() {
	return playerCoords;
}

float Player::getXCoord() {
	return actualPlayerCoords.x;
}

void Player::setXCoord(float x) {
	actualPlayerCoords.x = x;
}

float Player::getYCoord() {
	return actualPlayerCoords.y;
}

void Player::setYCoord(float y) {
	actualPlayerCoords.y = y;
}

