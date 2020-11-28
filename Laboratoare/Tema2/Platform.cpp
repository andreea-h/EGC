#pragma once
#include "Platform.h"

#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <chrono>

using namespace std;
Platform::Platform()
{
	zLengths = std::vector<float>();
	zCoords = std::vector<float>();
	colors = std::vector<glm::vec3>();
}

Platform::~Platform()
{

}

float randGenerator(float a, float b) {
	return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}

int genColor() {
	int color = rand() % 4 + 1; //numar din intervalul [1, 5]
	return color;
}

int Platform::countPlatforms() {
	return zLengths.size();
}

void Platform::setPlatformZCoord(float ZCoord, int index) {
	zCoords[index] = ZCoord;
}

// genereaza 3 lungimi aleatoare pentru trei platforme pe care le adauga in vectorul de lungimi 
// si 3 culori aleatoare asociate fiecarei platforme
void Platform::generatePlatform() {
	float length;
	int i;
	srand(time(NULL));
	for (i = 0; i < 3; i++) {
		length = randGenerator(2, 10);
		//cout << "generat: " << length << endl;
		zLengths.push_back(length);
		//genereaza aleator culoarea
		int color = genColor();
		if (color == 1) { //rosu
			colors.push_back(glm::vec3(1, 0, 0));
		}
		else if (color == 2) { //galben
			colors.push_back(glm::vec3(1, 1, 0));
		}
		else if (color == 3) { //portocaliu
			colors.push_back(glm::vec3(1.000, 0.647, 0.000));
		}
		else if (color == 4) { //verde
			colors.push_back(glm::vec3(0.000, 0.502, 0.000));
		}
	}
}

float Platform::getPlatformSize(int index) { //dimensiunea platformei de la un anumit index
	return zLengths[index];
}

float Platform::getPlatformPos(int index) { //intoarce coordoanata z a platformei de la un anumit index
	return zCoords[index];
}

glm::vec3 Platform::getPlatformColor(int index) {
	return colors[index];
}

//sterge platformele care nu mai sunt vizibile in spatiul de desenare
void Platform::deletePlatform(int i) {
	zLengths.erase(zLengths.begin() + i);
	zCoords.erase(zCoords.begin() + i);
	colors.erase(colors.begin() + i);
}