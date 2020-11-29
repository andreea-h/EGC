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
	zCoords = std::vector<float>(9);
	colors = std::vector<glm::vec3>();
	translateZVal = std::vector<float>(9);
	translateZPoint = std::vector<float>(9);
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

void Platform::generate3NewPlat(int index) 
{
	int i, length;
	srand(time(NULL));
	for (i = index; i < index + 3; i++) {
		length = randGenerator(2, 10);

		zLengths[i] = length;
		//genereaza aleator culoarea
		int color = genColor();
		if (color == 1) { //rosu
			colors[i] = glm::vec3(1, 0, 0);
		}
		else if (color == 2) { //galben
			colors[i] = glm::vec3(1, 1, 0);
		}
		else if (color == 3) { //portocaliu
			colors[i] = glm::vec3(1.000, 0.647, 0.000);
		}
		else if (color == 4) { //verde
			colors[i] = glm::vec3(0.000, 0.502, 0.000);
		}
	}
}

float Platform::getTranslatePoint(int i) 
{
	return translateZPoint[i];
}

void Platform::setTranslatePoint(int i, float val)
{
	translateZPoint[i] = val;
}

int Platform::countPlatforms() {
	return zLengths.size();
}

void Platform::setTranslateVal(float z, int i) {
	translateZVal[i] = z;
}

float Platform::getTranslateVal(int i) {
	return translateZVal[i];
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
	for (i = 0; i < 9; i++) {
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

void Platform::generateNewPlatforms()
{
	float length;
	int i;
	srand(time(NULL));
	for (i = 6; i < 9; i++) {
		length = randGenerator(2, 10);
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
		//translateZVal.push_back(0);
		zCoords.push_back(0);
	}/*
	int j;
	float var = -999;
	for (j = 6; j < 9; j++) {
		if (zLengths[j] > var) {
			var = zLengths[j];
		}
	}
	
	for (j = 0; j < 3; j++) {
		translateZPoint.push_back(translateZPoint[3] - 1.25f - var);
	}
	
	*/
	
	float max1 = lastMaxPlatform;
	
	cout << max1 << endl;

	float max2 = -999;
	int pos = 0;
	for (i = 0; i < 3; i++)
	{
		if (zLengths[i] > max2) {
			max2 = zLengths[i];
			pos = i;
		}
	}
	
	float max3 = -999;
	
	for (i = 3; i < 6; i++)
	{
		if (zLengths[i] > max3) {
			max3 = zLengths[i];
			pos = i;
		}
	}
	
	for (i = 6; i < 9; i++) {
		//-2 - maxCoord - 1.25f - platforms->getPlatformSize(i) / 2 - maxCoord1 - 1.25f
		//translateZPoint.push_back(-2 -1.25f * 3 - zLengths[i]/2 - max1 - max2 - max3);
		translateZPoint.push_back(-2 + translateZPoint[pos] - zLengths[i] / 2 - 1.25f * 3);
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

void Platform::setLastMaxPlatform(float val) 
{
	lastMaxPlatform = val;
}

//sterge platformele care nu mai sunt vizibile in spatiul de desenare
void Platform::deletePlatform(int i) {
	zLengths.erase(zLengths.begin() + i);
	zCoords.erase(zCoords.begin() + i);
	colors.erase(colors.begin() + i);

	translateZPoint.erase(translateZPoint.begin() + i);
	
	//translateZVal.erase(translateZVal.begin() + i);
}