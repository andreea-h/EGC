#pragma once

#include <string>
#include <Component/SimpleScene.h>
#include <Core/Engine.h>

class Player 
{
public:
	Player();
	~Player();

	glm::vec3 getInitialPlayerCoords();
	glm::vec4 getActualPlayerCoords();
	void setActualCoords(glm::vec4 coords);


	void setXCoord(float x); //modifica coordonata de x a jucatorului
	float getXCoord(); //returneaza coordonata pe x a jucatorului

	void setYCoord(float y); //modifica coordonata de y a jucatorului
	float getYCoord(); //returneaza coordonata pe y a jucatorului

private:
	glm::vec3 playerCoords = glm::vec3(0, 0.35f, -0.5f);
	glm::vec4 actualPlayerCoords = glm::vec4(0, 0.35f, -0.5f, 1);

};