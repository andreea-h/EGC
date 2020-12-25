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
	xCoords = std::vector<float>(9);
	yCoords = std::vector<float>(9);
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
	int color = rand() % 7 + 1; //numar din intervalul [1, 7]
	return color;
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
	for (i = 0; i < 3; i++) {
		length = randGenerator(5, 10);
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
		else if (color == 5) { //albastru
			colors.push_back(glm::vec3(0.118, 0.565, 1.000));
		}
		else if (color == 6) {//hotpink->recapatare viata
			colors.push_back(glm::vec3(1.000, 0.412, 0.706));
		}
		else if (color == 7) {//ladender->pierdere viata
			colors.push_back(glm::vec3(0.902, 0.902, 0.980));
		}
	}
	//verifica ca platforma cea mai lunga de pe fiecare rand sa nu fie rosie
	float maxLength = -999;
	int pos = 0;
	for (i = 0; i < 3; i++) {
		if (zLengths[i] > maxLength) {
			maxLength = zLengths[i];
			pos = i;
		}
	}
	if (colors[pos] == glm::vec3(1, 0, 0)) {//rosu
		//schimba culoarea
		while (colors[pos] == glm::vec3(1, 0, 0)) {
			int color = genColor();
			if (color == 1) { //rosu
				colors[pos] = glm::vec3(1, 0, 0);
			}
			else if (color == 2) { //galben
				colors[pos] = glm::vec3(1, 1, 0);
			}
			else if (color == 3) { //portocaliu
				colors[pos] = glm::vec3(1.000, 0.647, 0.000);
			}
			else if (color == 4) { //verde
				colors[pos] = glm::vec3(0.000, 0.502, 0.000);
			}
			else if (color == 5) { //albastru
				colors[pos] = (glm::vec3(0.118, 0.565, 1.000));
			}
			else if (color == 6) {//hotpink->recapatare viata
				colors.push_back(glm::vec3(1.000, 0.412, 0.706));
			}
			else if (color == 7) {//ladender->pierdere viata
				colors.push_back(glm::vec3(0.902, 0.902, 0.980));
			}
		}
	}


	//randul 2 de cuburi
	for (i = 3; i < 6; i++) {
		length = randGenerator(5, 10);
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
		else if (color == 5) { //albastru
			colors.push_back(glm::vec3(0.118, 0.565, 1.000));
		}
		else if (color == 6) {//hotpink->recapatare viata
			colors.push_back(glm::vec3(1.000, 0.412, 0.706));
		}
		else if (color == 7) {//ladender->pierdere viata
			colors.push_back(glm::vec3(0.902, 0.902, 0.980));
		}
	}
	//verifica ca platforma cea mai lunga de pe fiecare rand sa nu fie rosie
	maxLength = -999;
	pos = 0;
	for (i = 3; i < 6; i++) {
		if (zLengths[i] > maxLength) {
			maxLength = zLengths[i];
			pos = i;
		}
	}
	if (colors[pos] == glm::vec3(1, 0, 0)) {//rosu
		//schimba culoarea
		while (colors[pos] == glm::vec3(1, 0, 0)) {
			int color = genColor();
			if (color == 1) { //rosu
				colors[pos] = glm::vec3(1, 0, 0);
			}
			else if (color == 2) { //galben
				colors[pos] = glm::vec3(1, 1, 0);
			}
			else if (color == 3) { //portocaliu
				colors[pos] = glm::vec3(1.000, 0.647, 0.000);
			}
			else if (color == 4) { //verde
				colors[pos] = glm::vec3(0.000, 0.502, 0.000);
			}
			else if (color == 5) { //albastru
				colors[pos] = (glm::vec3(0.118, 0.565, 1.000));
			}
			else if (color == 6) {//hotpink->recapatare viata
				colors.push_back(glm::vec3(1.000, 0.412, 0.706));
			}
			else if (color == 7) {//ladender->pierdere viata
				colors.push_back(glm::vec3(0.902, 0.902, 0.980));
			}
		}
	}

	//randul 3 de cuburi
	for (i = 6; i < 9; i++) {
		length = randGenerator(5, 10);
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
		else if (color == 5) { //albastru
			colors.push_back(glm::vec3(0.118, 0.565, 1.000));
		}
		else if (color == 6) {//hotpink->recapatare viata
			colors.push_back(glm::vec3(1.000, 0.412, 0.706));
		}
		else if (color == 7) {//ladender->pierdere viata
			colors.push_back(glm::vec3(0.902, 0.902, 0.980));
		}
	}
	//verifica ca platforma cea mai lunga de pe fiecare rand sa nu fie rosie
	maxLength = -999;
	pos = 0;
	for (i = 6; i < 9; i++) {
		if (zLengths[i] > maxLength) {
			maxLength = zLengths[i];
			pos = i;
		}
	}
	if (colors[pos] == glm::vec3(1, 0, 0)) {//rosu
		//schimba culoarea
		while (colors[pos] != glm::vec3(1, 0, 0)) {
			int color = genColor();
			if (color == 1) { //rosu
				colors[pos] = glm::vec3(1, 0, 0);
			}
			else if (color == 2) { //galben
				colors[pos] = glm::vec3(1, 1, 0);
			}
			else if (color == 3) { //portocaliu
				colors[pos] = glm::vec3(1.000, 0.647, 0.000);
			}
			else if (color == 4) { //verde
				colors[pos] = glm::vec3(0.000, 0.502, 0.000);
			}
			else if (color == 5) { //albastru
				colors[pos] = (glm::vec3(0.118, 0.565, 1.000));
			}
			else if (color == 6) {//hotpink->recapatare viata
				colors.push_back(glm::vec3(1.000, 0.412, 0.706));
			}
			else if (color == 7) {//ladender->pierdere viata
				colors.push_back(glm::vec3(0.902, 0.902, 0.980));
			}
		}
	}
}

void Platform::generateNewPlatforms()
{
	float length;
	int i;
	srand(time(NULL));
	for (i = 6; i < 9; i++) {
		length = randGenerator(5, 10);
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
		else if (color == 5) { //albastru
			colors.push_back(glm::vec3(0.118, 0.565, 1.000));
		}
		else if (color == 6) {//hotpink->recapatare viata
			colors.push_back(glm::vec3(1.000, 0.412, 0.706)); 
		}
		else if (color == 7) {//ladender->pierdere viata
			colors.push_back(glm::vec3(0.902, 0.902, 0.980));
		}
		zCoords.push_back(0);
		xCoords.push_back(0);
		yCoords.push_back(0);
	}
	
	//verifica ca platforma cea mai lunga dintre cele generate sa nu fie de culoare rosie
	float maxLength = -999;
	int pos = 0;
	for (i = 6; i < 9; i++) {
		if (zLengths[i] > maxLength) {
			maxLength = zLengths[i];
			pos = i;
		}
	}
	
	if (colors[pos] == glm::vec3(1, 0, 0)) {//rosu
		//schimba culoarea
		while (colors[pos] == glm::vec3(1, 0, 0)) {
			int color = genColor();
			if (color == 1) { //rosu
				colors[pos] = glm::vec3(1, 0, 0);
			}
			else if (color == 2) { //galben
				colors[pos] = glm::vec3(1, 1, 0);
			}
			else if (color == 3) { //portocaliu
				colors[pos] = glm::vec3(1.000, 0.647, 0.000);
			}
			else if (color == 4) { //verde
				colors[pos] = glm::vec3(0.000, 0.502, 0.000);
			}
			else if (color == 5) { //albastru
				colors[pos] = glm::vec3(0.118, 0.565, 1.000);
			}
			else if (color == 6) {//hotpink->recapatare viata
				colors.push_back(glm::vec3(1.000, 0.412, 0.706));
			}
			else if (color == 7) {//ladender->pierdere viata
				colors.push_back(glm::vec3(0.902, 0.902, 0.980));
			}
		}
	}

	float max1 = lastMaxPlatform;
	float max2 = -999;
	pos = 0;
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
		translateZPoint.push_back(translateZPoint[pos] - max3/2 - zLengths[i] / 2 - 1);
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

void Platform::setPlatformXCoord(float x, int index) 
{
	xCoords[index] = x;
}

void Platform::setPlatformYCoord(float y, int index)
{
	yCoords[index] = y;
}

float Platform::getPlatformXCoord(int index) {
	return xCoords[index];
}
float Platform::getPlatformYCoord(int index) {
	return yCoords[index];
}

void Platform::setPlatformColor(int index) {
	colors[index] = glm::vec3(0.482, 0.408, 0.933); //coloarea mov
}

//sterge platformele care nu mai sunt vizibile in spatiul de desenare
void Platform::deletePlatform(int i) {
	zLengths.erase(zLengths.begin() + i);
	zCoords.erase(zCoords.begin() + i);
	xCoords.erase(xCoords.begin() + i);
	yCoords.erase(yCoords.begin() + i);
	colors.erase(colors.begin() + i);

	translateZPoint.erase(translateZPoint.begin() + i);
}