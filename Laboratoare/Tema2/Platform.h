#pragma once

#include <vector>

#include <Core/Engine.h>
#include <Component/SimpleScene.h>

class Platform
{
public:
	Platform();
	~Platform();

	void generatePlatform();
	void deletePlatform(int index);
	float getPlatformSize(int index);
	void setPlatformZCoord(float ZCoord, int index);
	float getPlatformPos(int index);
	glm::vec3 getPlatformColor(int index);

	int countPlatforms(); //intoarce numarul de elemente din zLengths

private:
	std::vector<float> zLengths; //lungimea platformei (pe directia axei OZ)
	std::vector<float> zCoords; //coordonatele platformelor pe directia z
	std::vector<glm::vec3> colors; //culorile initiale ale platformelor
};