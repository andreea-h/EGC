#pragma once
#include "Platform.h"

#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;
Platform::Platform()
{
	zLengths = std::vector<float>();
	zCoords = std::vector<float>();
}

Platform::~Platform()
{

}

int Platform::countPlatforms() {
	return zLengths.size();
}

float randGenerator(float a, float b) {
	return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}


void Platform::setPlatformZCoord(float ZCoord, int index) {
	
	zCoords[index] = ZCoord;
}



//genereaza 3 lungimi aleatoare pentru trei platforme pe care le adauga in vectorul de lungimi
void Platform::generatePlatform() {
	float length;
	int i;
	for (i = 0; i < 3; i++) {
		length = randGenerator(2, 10);
		//cout << "generat: " << length << endl;
		zLengths.push_back(length);
	}
	zCoords = std::vector<float>(zLengths.size());
}

float Platform::getPlatformSize(int index) { //dimensiunea platformei de le un anume index
	return zLengths[index];
}

float Platform::getPlatformPos(int index) { //intoarce coordoanata z a platformei de la un anumit index
	return zCoords[index];
}

//sterge platformele care nu mai sunt vizibile in spatiul de desenare
void Platform::deletePlatform(int i) {
	zLengths.erase(zLengths.begin() + i);
	zCoords.erase(zCoords.begin() + i);
}