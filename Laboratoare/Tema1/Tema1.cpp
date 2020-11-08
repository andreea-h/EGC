#include "Tema1.h"
#include "Transform2D.h"

#include <vector>
#include <iostream>
#include <Core/Engine.h>
#include <cmath>
#include <glm/gtx/string_cast.hpp> //afisare matrice

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init() {
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	shurikenSide = 30;
	Mesh* shuriken = CreateShuriken(shurikenSide);
	AddMeshToList(shuriken);

	Mesh* arrow = CreateArrow(5, 30);
	AddMeshToList(arrow);

	Mesh* bow = CreateBow();
	AddMeshToList(bow);

	ballonRadius = 30;
	Mesh* yellowBallon = CreateBallon('y', ballonRadius);
	AddMeshToList(yellowBallon);

	Mesh* redBallon = CreateBallon('r', ballonRadius);
	AddMeshToList(redBallon);

	//retine coordonate pentre viitoarele translatii de baloane si shurikene
	int i;
	for (i = 0; i < 10; i++) {
		translateY_values.push_back(0 - 200 * i);
	}

	Mesh* powerBar = CreateRectangle();
	AddMeshToList(powerBar);

	GenerateBallonColor(); //genereaza aleator culorile pentru cele 180 baloane
}

Mesh* Tema1::CreateRectangle() {
	glm::vec3 color = glm::vec3(0.180, 0.545, 0.341);
	vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(0, 18, 0), color),
		VertexFormat(glm::vec3(10, 18, 0), color),
		VertexFormat(glm::vec3(10, 0, 0), color),
	};

	vector<unsigned short> indices =
	{
		0, 2, 1,
		3, 2, 0
	};
	Mesh* powerBar = new Mesh("powerBar");
	powerBar->InitFromData(vertices, indices);
	return powerBar;
}

Mesh* Tema1::CreateArrow(float arrowSide, float lengthFactor) {

	glm::vec3 color = glm::vec3(1.000, 0.078, 0.576);
	vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(0, -arrowSide, 0), color), 
		VertexFormat(glm::vec3(lengthFactor * arrowSide, 0, 0), color),
		VertexFormat(glm::vec3(lengthFactor * arrowSide, -arrowSide, 0), color),
		VertexFormat(glm::vec3(lengthFactor * arrowSide, 2 * arrowSide, 0), color),
		VertexFormat(glm::vec3((lengthFactor + 3) * arrowSide, -arrowSide / 2, 0), color),
		VertexFormat(glm::vec3(lengthFactor * arrowSide, - 3 * arrowSide, 0), color)
	};

	vector<unsigned short> indices = {
		0, 1, 2,
		1, 3, 2,
		4, 6, 5
	};

	arrowLength = lengthFactor * arrowSide + 3 * arrowSide;
	Mesh* arrow = new Mesh("arrow");
	arrow->InitFromData(vertices, indices);
	return arrow;
}

Mesh* Tema1::CreateShuriken(float shurikenSide) {

	glm::vec3 color = glm::vec3(0.600, 0.196, 0.800);
	vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.878, 1.000, 1.000)),
		VertexFormat(glm::vec3(-1, 1, 0) * shurikenSide, color),
		VertexFormat(glm::vec3(0, 1, 0) * shurikenSide, color),
		VertexFormat(glm::vec3(1, 1, 0) * shurikenSide, color),
		VertexFormat(glm::vec3(1, 0, 0) * shurikenSide, color),
		VertexFormat(glm::vec3(1, -1, 0) * shurikenSide, color),
		VertexFormat(glm::vec3(0, -1, 0) * shurikenSide, color),
		VertexFormat(glm::vec3(-1, -1, 0) * shurikenSide, color),
		VertexFormat(glm::vec3(-1, 0, 0) * shurikenSide, color),
	};

	vector<unsigned short> indices = {
		1, 0, 2,
		0, 4, 3,
		0, 6, 5,
		8, 7, 0
	};

	Mesh* shuriken = new Mesh("shuriken");
	shuriken->InitFromData(vertices, indices);
	return shuriken;
}

Mesh* Tema1::CreateBow() {
	glm::vec3 color = glm::vec3(0.000, 0.000, 0.804);
	float radius = 100;
	vector<VertexFormat> vertices;
	float i, j;
	int step1 = 0;
	for (i = 0.4; i <= M_PI - 0.4; i += 0.001) {
		vertices.push_back(VertexFormat(glm::vec3(radius * sinf(i), radius * cosf(i), 0), color));
		step1++;
	}
	vector<unsigned short> indices;
	int k;
	
	for (k = 0; k < step1; k++) {
		indices.push_back(k);
	}

	Mesh* bow = new Mesh("bow");
	bow->InitFromData(vertices, indices);
	bow->SetDrawMode(GL_LINE_LOOP);
	return bow;
}

Mesh* Tema1::CreateBallon(char color, float R) {
	glm::vec3 color1 = glm::vec3(0, 0, 0);
	if (color == 'r') {
		color1 = glm::vec3(1, 0, 0); //rosu
	}
	if (color == 'y') {
		color1 = glm::vec3(1, 1, 0); //galben
	}
	vector<VertexFormat> vertices;
	vector<unsigned short> indices;
	float i;
	int step = 0;
	//primul vertex este in centrul balonului
	vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.902, 0.902, 0.980))); //centrul cercului care va reprezenta balonul
	for (i = 0; i <= 2 * M_PI; i += 0.001) {
		vertices.push_back(VertexFormat(glm::vec3(R * sinf(i), R * cosf(i), 0), color1));
		step++;
	}
	//adauga punctele din partea de jos a balonului (triunghiul mic)
	vertices.push_back(VertexFormat(glm::vec3(0, -R + 4, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(-10, -R - 10, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(10, -R - 10, 0), color1));

	color1 = glm::vec3(0.753, 0.753, 0.753); //silver
	//polilinia
	vertices.push_back(VertexFormat(glm::vec3(2, -R -10, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(0, -R -9, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(-5, -R -17, 0), color1));
	
	vertices.push_back(VertexFormat(glm::vec3(-5, -R - 17, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(-3, -R - 19, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(2, -R - 10, 0), color1));

	vertices.push_back(VertexFormat(glm::vec3(-5, -R - 17, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(3, -R -21, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(2, -R - 23, 0), color1));

	vertices.push_back(VertexFormat(glm::vec3(-5, -R - 17, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(3, -R - 21, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(-3, -R - 15, 0), color1));

	vertices.push_back(VertexFormat(glm::vec3(3, -R - 21, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(2, -R - 19, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(-5, -R - 26, 0), color1));

	vertices.push_back(VertexFormat(glm::vec3(-5, -R - 26, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(-3, -R - 29, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(3, -R - 21, 0), color1));

	vertices.push_back(VertexFormat(glm::vec3(-5, -R - 26, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(3, -R - 33, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(5, -R - 30, 0), color1));

	vertices.push_back(VertexFormat(glm::vec3(-5, -R - 26, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(5, -R - 30, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(-3, -R - 24, 0), color1));

	int j;
	for (j = 1; j < step; j++) {
		indices.push_back(j);
		indices.push_back(j - 1);
		indices.push_back(0); //"uneste" de centru 
	}
	for (j = step + 1; j <= step + 27; j++) {
		indices.push_back(j);
	}
	
	if (color == 'y') {
		Mesh* yellowBallon = new Mesh("yellowBallon");
		yellowBallon->InitFromData(vertices, indices);
		return yellowBallon;
	}
	if (color == 'r') {
		Mesh* redBallon = new Mesh("redBallon");
		redBallon->InitFromData(vertices, indices);
		return redBallon;
	}
}

//verifica coliziuniea sagetii cu fiecare shuniker
void Tema1::checkShurikenColl() {
	int i;
	bool ok = false;
	for (i = 0; i < 120; i++) {
		//verifica coliziunea doar pentru shiriken-ele vizibile in fereastra
		if (shurikenPos[i].x >= 0 && shurikenPos[i].x <= window->GetResolution().x) {
			if (collisionMemShuriken[i] != 1) { //daca nu s-a produs deja coliziune
				float posX = shurikenPos[i].x;
				float posY = shurikenPos[i].y;
				if (powf(arrowTop.x - posX, 2.0f) + powf(arrowTop.y - posY, 2.0f) <
					powf(shurikenSide * sqrt(2) * 0.75f, 2.0)) {
					collisionMemShuriken[i] = 1;
					score += 15;
					ok = true;
				}
			}
		}
	}
	if (ok == true) {
		cout << "Scorul este: " << score << " puncte :)" << endl;
	}
}

//verifica coliziunea sagetii cu baloanele galbene
void Tema1::checkBallonCollision() {
	//verifica daca varful sagetii este inclus in cercul incadrator al unui dintre baloanele 
	int i;
	bool newScore = false;
	for (i = 0; i < 180; i++) {
		if (ballonPos[i].y <= window->GetResolution().y) {
			if (collisionMem[i] != 1) { //daca nu s-a mai produs coliziune pana la acest moment
				float xBallon = ballonPos[i].x;
				float yBallon = ballonPos[i].y;
				//(x - center_x)^2 + (y - center_y)^2 < radius^2
				if (powf(arrowTop.x - xBallon, 2.0f) + powf(arrowTop.y - yBallon, 2.0f) <
					powf((ballonRadius  + 10)* 1.25f, 2.0)) {
					//coliziune sageata balon
					//std::cout << glm::to_string() << std::endl;
					collisionMem[i] = 1;
					//cout << collisionMem[i] << "; coliziune: x = " << xBallon << "; y = " << yBallon << endl;
					//cout << "sageata: " << arrowTop.x << "," << arrowTop.y << endl;
					//cout << "balon: " << glm::to_string(ballonPos[i]) << std::endl;
					//cout << "mouse: " << mousePos.x << ", " << mousePos.y << endl;
					//cout << "*******" << endl;
					if (ballonColor[i] == 1) { //rosu
						//creste scorul
						score += 10;
					}
					else {
						score -= 5; //scade scorul
					}
					newScore = true;
				}
			}
		}
	}
	if (newScore == true) { //s-a actualizat scorul
		cout << "Scorul este: " << score << " puncte :)" << endl;
	}
}

//verifica daca mai exista baloane vizibile in fereasta
//cand baloanele nu mai sunt vizibile in fereastra, apar shurikene
bool Tema1::checkBallonPos() {
	int i;
	int count = 0;
	for (i = 0; i < 180; i++) {
		float posY = ballonPos[i].y;
		if ((posY >= window->GetResolution().y + 100) || collisionMem[i] == 1) { 
			count++;
		}
	}
	if (count == 180) {
		return true;
	}
	return false;
}

void Tema1::LoadShuriken() {
	int i;
	for (i = 0; i < 20; i++) {
		if (collisionMemShuriken[i] != 1) {
			modelMatrixShuriken = glm::mat3(1);
			//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
			
			modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 200 * i, 200 + i * 20); //ok
			modelMatrixShuriken *= Transform2D::Translate(-20 * angularStep, 0); //translatia de-a lungul axei OX
			modelMatrixShuriken *= Transform2D::Rotate(-angularStep); // genereaza rotatia in jurul propriului centru
			modelMatrixShuriken *= Transform2D::Scale(0.75f, 0.75f);
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
			shurikenPos[i] = glm::vec2(modelMatrixShuriken[2][0] + modelMatrixShuriken[0][0],
				modelMatrixShuriken[2][1] + modelMatrixShuriken[1][1]);
		}
		
	}
	for (i = 20; i < 40; i++) {
		if (collisionMemShuriken[i] != 1) {
			modelMatrixShuriken = glm::mat3(1);
			//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
			modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 300 * (i - 20) + 200, 750 - i * 10); //ok
			modelMatrixShuriken *= Transform2D::Translate(-20 * angularStep, 0); //translatia de-a lungul axei OX
			modelMatrixShuriken *= Transform2D::Rotate(-angularStep); // genereaza rotatia in jurul propriului centru
			modelMatrixShuriken *= Transform2D::Scale(0.75f, 0.75f);
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
			shurikenPos[i] = glm::vec2(modelMatrixShuriken[2][0] + modelMatrixShuriken[0][0],
				modelMatrixShuriken[2][1] + modelMatrixShuriken[1][1]);
		}
		
	}
	for (i = 40; i < 60; i++) { //ultimul set de shurikene care devine vizibil in fereastra
		if (collisionMemShuriken[i] != 1) {
			modelMatrixShuriken = glm::mat3(1);
			//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
			modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 300 * i + 1500,  50 + i * 5); //ok
			modelMatrixShuriken *= Transform2D::Translate(-20 * angularStep, 0); //translatia de-a lungul axei OX
			modelMatrixShuriken *= Transform2D::Rotate(-angularStep); // genereaza rotatia in jurul propriului centru
			modelMatrixShuriken *= Transform2D::Scale(0.75f, 0.75f);
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
			shurikenPos[i] = glm::vec2(modelMatrixShuriken[2][0] + modelMatrixShuriken[0][0],
				modelMatrixShuriken[2][1] + modelMatrixShuriken[1][1]);
		}
		
	}
	for (i = 60; i < 80; i++) {
		if (collisionMemShuriken[i] != 1) {
			modelMatrixShuriken = glm::mat3(1);
			//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
			modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 250 * (i - 60) + 50, 900 - i * 10);
			modelMatrixShuriken *= Transform2D::Translate(-20 * angularStep, 0); //translatia de-a lungul axei OX
			modelMatrixShuriken *= Transform2D::Rotate(-angularStep); // genereaza rotatia in jurul propriului centru
			modelMatrixShuriken *= Transform2D::Scale(0.75f, 0.75f);
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
			shurikenPos[i] = glm::vec2(modelMatrixShuriken[2][0] + modelMatrixShuriken[0][0],
				modelMatrixShuriken[2][1] + modelMatrixShuriken[1][1]);
		}
	}

	for (i = 80; i < 100; i++) {
		if (collisionMemShuriken[i] != 1) {
			modelMatrixShuriken = glm::mat3(1);
			//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
			modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 250 * (i - 80) + 2000, 300 - i);
			modelMatrixShuriken *= Transform2D::Translate(-20 * angularStep, 0); //translatia de-a lungul axei OX
			modelMatrixShuriken *= Transform2D::Rotate(-angularStep); // genereaza rotatia in jurul propriului centru
			modelMatrixShuriken *= Transform2D::Scale(0.75f, 0.75f);
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
			shurikenPos[i] = glm::vec2(modelMatrixShuriken[2][0] + modelMatrixShuriken[0][0],
				modelMatrixShuriken[2][1] + modelMatrixShuriken[1][1]);
		}
	}

	for (i = 100; i < 120; i++) {
		if (collisionMemShuriken[i] != 1) {
			modelMatrixShuriken = glm::mat3(1);
			//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
			modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 300 * (i - 100) + 2000, 700 - i);
			modelMatrixShuriken *= Transform2D::Translate(-20 * angularStep, 0); //translatia de-a lungul axei OX
			modelMatrixShuriken *= Transform2D::Rotate(-angularStep); // genereaza rotatia in jurul propriului centru
			modelMatrixShuriken *= Transform2D::Scale(0.75f, 0.75f);
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
			shurikenPos[i] = glm::vec2(modelMatrixShuriken[2][0] + modelMatrixShuriken[0][0],
				modelMatrixShuriken[2][1] + modelMatrixShuriken[1][1]);
		}
	}
}

void Tema1::GenerateBallonColor() {
	int i;
	srand(time(NULL));
	for (i = 0; i < 180; i++) {
		unsigned short colorRandom = rand() % 2;
		ballonColor[i] = colorRandom; // 1-rosu, 0-galben
	}
}

void Tema1::LoadBallons(float deltaTimeSeconds) {
	int i;
	for (i = 0; i < 90; i++) { //baloanele apar in 3 coloane => 90 baloane total
		if (collisionMem[i] != 1) {
			modelMatrixBallonY = glm::mat3(1);
			modelMatrixBallonY *= Transform2D::Translate(800, -200 * i - 100);
			modelMatrixBallonY *= Transform2D::Translate(0, translateBallonStep);
			modelMatrixBallonY *= Transform2D::Scale(1, 1.25f);
			if (ballonColor[i] == 1) { //rosu
				RenderMesh2D(meshes["redBallon"], shaders["VertexColor"], modelMatrixBallonY);
			}
			else {
				RenderMesh2D(meshes["yellowBallon"], shaders["VertexColor"], modelMatrixBallonY);
			}
			//memoreaza pozitiile baloanelor
			ballonPos[i] = glm::vec2(modelMatrixBallonY[2][0] + modelMatrixBallonY[0][0],
				modelMatrixBallonY[2][1] + modelMatrixBallonY[1][1]);
		}
		else { //efect de animatie la distrugerea balonului
			scaleFactors[i] += 0.5 * deltaTimeSeconds;
			modelMatrixBallonY = glm::mat3(1);
			modelMatrixBallonY *= Transform2D::Translate(800, -200 * i - 100);
			modelMatrixBallonY *= Transform2D::Translate(0, translateBallonStep - 150 * deltaTimeSeconds);
			modelMatrixBallonY *= Transform2D::Scale(1 - scaleFactors[i], 1.25f - scaleFactors[i]);
			if (1 - scaleFactors[i] > 0) {
				if (ballonColor[i] == 1) { //rosu
					RenderMesh2D(meshes["redBallon"], shaders["VertexColor"], modelMatrixBallonY);
				}
				else {
					RenderMesh2D(meshes["yellowBallon"], shaders["VertexColor"], modelMatrixBallonY);
				}
			}
		}
	}

	for (i = 90; i < 150; i++) {
		if (collisionMem[i] != 1) {
			modelMatrixBallonY = glm::mat3(1);
			modelMatrixBallonY *= Transform2D::Translate(1000, -250 * (i - 90) - 300);
			modelMatrixBallonY *= Transform2D::Translate(0, translateBallonStep);
			modelMatrixBallonY *= Transform2D::Scale(1, 1.25f);
			if (ballonColor[i] == 1) {
				RenderMesh2D(meshes["redBallon"], shaders["VertexColor"], modelMatrixBallonY);
			}
			else {
				RenderMesh2D(meshes["yellowBallon"], shaders["VertexColor"], modelMatrixBallonY);
			}
			ballonPos[i] = glm::vec2(modelMatrixBallonY[2][0] + modelMatrixBallonY[0][0],
				modelMatrixBallonY[2][1] + modelMatrixBallonY[1][1]);
		}
		else { //efect de animatie la distrugerea balonului
			scaleFactors[i] += 0.5 * deltaTimeSeconds;
			modelMatrixBallonY = glm::mat3(1);
			modelMatrixBallonY *= Transform2D::Translate(1000, -250 * (i - 90) - 300);
			modelMatrixBallonY *= Transform2D::Translate(0, translateBallonStep - 150 * deltaTimeSeconds);
			modelMatrixBallonY *= Transform2D::Scale(1 - scaleFactors[i], 1.25f - scaleFactors[i]);
			if (1 - scaleFactors[i] > 0) {
				if (ballonColor[i] == 1) { //rosu
					RenderMesh2D(meshes["redBallon"], shaders["VertexColor"], modelMatrixBallonY);
				}
				else {
					RenderMesh2D(meshes["yellowBallon"], shaders["VertexColor"], modelMatrixBallonY);
				}
			}
		}
	}

	for (i = 150; i < 180; i++) {
		if (collisionMem[i] != 1) {
			modelMatrixBallonY = glm::mat3(1);
			modelMatrixBallonY *= Transform2D::Translate(1200, -300 * (i - 150) - 500);
			modelMatrixBallonY *= Transform2D::Translate(0, translateBallonStep);
			modelMatrixBallonY *= Transform2D::Scale(1, 1.25f);
			if (ballonColor[i] == 1) {
				RenderMesh2D(meshes["redBallon"], shaders["VertexColor"], modelMatrixBallonY);
			}
			else {
				RenderMesh2D(meshes["yellowBallon"], shaders["VertexColor"], modelMatrixBallonY);
			}
			ballonPos[i] = glm::vec2(modelMatrixBallonY[2][0] + modelMatrixBallonY[0][0],
				modelMatrixBallonY[2][1] + modelMatrixBallonY[1][1]);
			//std::cout << glm::to_string(ballonPos[i]) << std::endl;
		}
		else { //efect de animatie la distrugerea balonului
			scaleFactors[i] += 0.5 * deltaTimeSeconds;
			modelMatrixBallonY = glm::mat3(1);
			modelMatrixBallonY *= Transform2D::Translate(1200, -300 * (i - 150) - 500);
			modelMatrixBallonY *= Transform2D::Translate(0, translateBallonStep - 150 * deltaTimeSeconds);
			modelMatrixBallonY *= Transform2D::Scale(1 - scaleFactors[i], 1.25f - scaleFactors[i]);
			if (1 - scaleFactors[i] > 0) {
				if (ballonColor[i] == 1) { //rosu
					RenderMesh2D(meshes["redBallon"], shaders["VertexColor"], modelMatrixBallonY);
				}
				else {
					RenderMesh2D(meshes["yellowBallon"], shaders["VertexColor"], modelMatrixBallonY);
				}
			}
		}
	}
}

void Tema1::Update(float deltaTimeSeconds) {
	if (livesCount > 0) {
		int i;
		translateBallonStep += 100 * deltaTimeSeconds; //cu cat se translateaza la fiecare cadru fiecare balon pe OY

		bool check = checkBallonPos();
		if (check == false) {
			LoadBallons(deltaTimeSeconds);
		}
		else {
			angularStep += 5 * deltaTimeSeconds; //pentru deplasarea respectiv rotirea shuriken-ului
			LoadShuriken();
		}
		//aruncare sageata
		if (throwArrow == true) {
			arrowTranslateMove.x += (1000 * mouseClickTime) * deltaTimeSeconds;
			arrowTranslateMove.y += (1000 * mouseClickTime) * deltaTimeSeconds;
			modelMatrixArrow = glm::mat3(1);
			modelMatrixArrow *= Transform2D::Translate(100, 250 + translateY);
			//translatare pe directia pozitiei mouse-ului la momentul mouseBtnRelease

			modelMatrixArrow *= Transform2D::Translate(arrowTranslateMove.x * cosf(throwAngle),
				arrowTranslateMove.x * sinf(throwAngle));
			modelMatrixArrow *= Transform2D::Rotate(throwAngle); //unghiul sub care se arunca sageata este unghiul translatiei la momentul mouseBtnRelease

			modelMatrixArrow *= Transform2D::Scale(0.85f, 0.85f);
			RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrixArrow);

		}
		else {
			//sageata este afisata ca atasata arcului si orientata mereu dupa directia mouse-ului
			modelMatrixArrow = glm::mat3(1);
			modelMatrixArrow *= Transform2D::Translate(100, 250 + translateY);
			modelMatrixArrow *= Transform2D::Rotate(radiusArrow);
			modelMatrixArrow *= Transform2D::Scale(0.85f, 0.85f);
			RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrixArrow);
		}
		//recalculeaza coordonatele varfului sagetii
		float arrowTopX = modelMatrixArrow[2][0] + modelMatrixArrow[1][0] + modelMatrixArrow[0][0];
		float arrowTopY = modelMatrixArrow[2][1] + modelMatrixArrow[1][1] + modelMatrixArrow[0][1];
		sageata.x = arrowTopX;
		sageata.y = arrowTopY;
		
		if (radiusArrow != 0) {
			arrowTopX += arrowLength * cosf(radiusArrow) * 0.85f;
			arrowTopY += arrowLength * sinf(radiusArrow) * 0.85f;
		}
		else {
			arrowTopX += arrowLength * 0.85f; //daca radiusArrow == 0 nu exista proiectie pe axa OY
		}
		arrowTop = glm::vec3(arrowTopX, arrowTopY, 0);
		
		//////////////////////////////////////////////////////////////////////////////
		modelMatrixBow = glm::mat3(1);
		modelMatrixBow *= Transform2D::Translate(100, 250 + translateY);
		modelMatrixBow *= Transform2D::Rotate(radiusArrow);
		modelMatrixBow *= Transform2D::Scale(0.85f, 0.85f);
		RenderMesh2D(meshes["bow"], shaders["VertexColor"], modelMatrixBow);

		//////////////////////////////////////////////////////////////////////////////
		//se afiseaza dreptunghiul scalat in functie de apasarea mouse-ului
		if (mouseClick == true) {
			modelMatrixPower = glm::mat3(1);
			modelMatrixPower *= Transform2D::Translate(100, 10);
			powerFactor += 0.09f;
			modelMatrixPower *= Transform2D::Translate(30, 90 + translateY);
			if (powerFactor >= 0.09 * 150) { //s-a atins limita maxima a vitezei
				//se opreste si cresterea vitezei de aruncare a sagetii
				modelMatrixPower *= Transform2D::Scale(0.09 * 150 + 1, 1);
				maxSpeedMoment = clock();
				maxSpeed = true;
			}
			else {
				modelMatrixPower *= Transform2D::Scale(1 + powerFactor, 1);
			}
			RenderMesh2D(meshes["powerBar"], shaders["VertexColor"], modelMatrixPower);
		}
		//verifica coliziunile baloanelor cu sageata
		if (check == false) {
			checkBallonCollision();
		}
		if (check == true) {
			checkShurikenColl();
		}
		//daca sageata a iesit din fereastra, va fi translatata in pozitia initiala ca fiind atasata arcului
		if (sageata.x >= window->GetResolution().x) {
			throwArrow = false;
		}


	}
	else {
		Engine::GetWindow()->Close();
		cout << "Ai pierdut cele 3 vieti :(" << endl;
		cout << "Scorul tau final este: " << score  <<" puncte." << endl;
	}
}

void Tema1::FrameStart() {
	glClearColor(1.000, 0.855, 0.725, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::FrameEnd() {
	
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_S)) {
		translateY -= deltaTime * 300;
	}

	if (window->KeyHold(GLFW_KEY_W)) {
		translateY += deltaTime * 300;
	}
}

void Tema1::OnKeyPress(int key, int mods)
{
}

void Tema1::OnKeyRelease(int key, int mods)
{
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	float diffX = mouseX - modelMatrixArrow[2][0] - arrowSide * cosf(radiusArrow);
	float diffY = window->GetResolution().y - mouseY - modelMatrixArrow[2][1] - arrowSide * sinf(radiusArrow);
	if (diffX > 0 && diffY != 0) {
		float temp = atan(diffY / diffX);
		//limitam rotirea arcului si a sagetii dupa pozitia mouse-ului in intervalul [-45 gr, 45 gr]
		if (temp >= -0.785 && temp <= 0.785) {
			radiusArrow = temp;
		}
	}	
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	mouseClick = true;
	maxSpeed = false;
	arrowTranslateMove.x = 0;
	arrowTranslateMove.y = 0;
	throwArrow = false;
	mouseClickTime = clock(); //se va retine timpul de apasare pe mouse
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	if (throwTimer == 0) { //primul MousebtnRelease
		throwTimer = clock();
	}
	else {
		float aux;
		aux = clock() - throwTimer;
		if (aux / 1000.0f <= 4) { //se poate arunca un arc o data la 4s
			cout << "Mai trebuie sa astepti " << 4 - aux/1000.0f << " secunde inainte de a arunca inca a sageata :)" << endl;
			return;
		}
	}
	throwTimer = clock();
	mouseClick = false;
	if (maxSpeed == false) { //s-a facut release inainte de a atinge limita maxima a vitezei
		mouseClickTime = (float)(clock() - mouseClickTime); //timpul in cicli de ceas
	}
	else {
		mouseClickTime = maxSpeedMoment - mouseClickTime;
	}
	
	mouseClickTime = (float)mouseClickTime/1000.0f; //timpul in secunde cat s-a tinut apasat pe mouse
	powerFactor = 0; //daca nu se mai tine apasat pe mouse, dispare powerBar
	//trebuie sa se lanseze sageata
	throwArrow = true;
	throwAngle = radiusArrow;
	mousePos = glm::vec2(mouseX, window->GetResolution().y - mouseY); //pozitia mouse-ului in coordonate utilizator
	//cout << "mouse: " << mousePos.x << " , " << mousePos.y << endl;
	//cout << "varf sageata: " << arrowTop.x << ", " << arrowTop.y << endl;
	//cout << "sageata stanga: " << sageata.x << " , " << sageata.y << endl;
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{

}