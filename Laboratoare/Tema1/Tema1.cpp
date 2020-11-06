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

	Mesh* yellowBallon = CreateBallon('y', 30);
	AddMeshToList(yellowBallon);

	Mesh* redBallon = CreateBallon('r', 30);
	AddMeshToList(redBallon);

	//retine coordonate pentre viitoarele translatii de baloane si shurikene
	int i;
	for (i = 0; i < 10; i++) {
		translateY_values.push_back(0 - 200 * i);
	}

	Mesh* powerBar = CreateRectangle();
	AddMeshToList(powerBar);
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
		VertexFormat(glm::vec3(0, 0, 0), color),
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
	vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color1)); //centrul cercului care va reprezenta balonul
	for (i = 0; i <= 2 * M_PI; i += 0.001) {
		vertices.push_back(VertexFormat(glm::vec3(R * sinf(i), R * cosf(i), 0), color1));
		step++;
	}
	//adauga punctele din partea de jos a balonului (triunghiul mic)
	vertices.push_back(VertexFormat(glm::vec3(0, -R + 4, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(-10, -R - 10, 0), color1));
	vertices.push_back(VertexFormat(glm::vec3(10, -R - 10, 0), color1));

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

void Tema1::LoadBallons() {
	int i;
	for (i = 0; i < 10; i++) {
		modelMatrixBallonY = glm::mat3(1);
		modelMatrixBallonY *= Transform2D::Translate(1000, translateY_values[i]);
		modelMatrixBallonY *= Transform2D::Scale(1, 1.25f);
		if (i % 2 == 0) {
			RenderMesh2D(meshes["redBallon"], shaders["VertexColor"], modelMatrixBallonY);
		}
		else {
			RenderMesh2D(meshes["yellowBallon"], shaders["VertexColor"], modelMatrixBallonY);
		}

		modelMatrixBallonY = glm::mat3(1);
		modelMatrixBallonY *= Transform2D::Translate(800, translateY_values[i] - 225);
		modelMatrixBallonY *= Transform2D::Scale(1, 1.25f);
		if (i % 2 == 0) {
			RenderMesh2D(meshes["redBallon"], shaders["VertexColor"], modelMatrixBallonY);
		}
		else {
			RenderMesh2D(meshes["yellowBallon"], shaders["VertexColor"], modelMatrixBallonY);
		}

		modelMatrixBallonY = glm::mat3(1);
		modelMatrixBallonY *= Transform2D::Translate(1200, translateY_values[i] - 450);
		modelMatrixBallonY *= Transform2D::Scale(1, 1.25f);
		RenderMesh2D(meshes["redBallon"], shaders["VertexColor"], modelMatrixBallonY);
	}
}

void Tema1::LoadShuriken() {
	int i;
	for (i = 0; i < 3; i++) {
		modelMatrixShuriken = glm::mat3(1);
		//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
		modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 200 * i, 250);
		modelMatrixShuriken *= Transform2D::Translate(-50 * angularStep, 100 * i);
		modelMatrixShuriken *= Transform2D::Rotate(-angularStep); // genereaza rotatia in jurul propriului centru
		modelMatrixShuriken *= Transform2D::Scale(0.75f, 0.75f);
		RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
	}

	for (i = 0; i < 3; i++) {
		modelMatrixShuriken = glm::mat3(1);
		//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
		modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 400 * i, 250);
		modelMatrixShuriken *= Transform2D::Translate(-50 * angularStep, 100 * i);
		modelMatrixShuriken *= Transform2D::Rotate(-angularStep); // genereaza rotatia in jurul propriului centru
		modelMatrixShuriken *= Transform2D::Scale(0.75f, 0.75f);
		RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
	}

	for (i = 0; i < 3; i++) {
		modelMatrixShuriken = glm::mat3(1);
		//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
		modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 400 * i, 650);
		modelMatrixShuriken *= Transform2D::Translate(-50 * angularStep, 100 * i);
		modelMatrixShuriken *= Transform2D::Rotate(-angularStep); // genereaza rotatia in jurul propriului centru
		modelMatrixShuriken *= Transform2D::Scale(0.75f, 0.75f);
		RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
	}

}

void Tema1::Update(float deltaTimeSeconds) {
	arrowTranslateMove.x += 60 * deltaTimeSeconds;
	arrowTranslateMove.y += 60 * deltaTimeSeconds;

	int i;
	angularStep += 3 * deltaTimeSeconds;
	for (i = 0; i < 10; i++) {
		translateY_values[i] += 250 * deltaTimeSeconds;
		if (translateY_values[i] > window->GetResolution().y + 600) {
			translateY_values[i] = - 1200;
		}
	}

	//sageata este inca "atasata arcului"
	if (throwArrow == false) {
		modelMatrixArrow = glm::mat3(1);
		modelMatrixArrow *= Transform2D::Translate(100, 250 + translateY);
		modelMatrixArrow *= Transform2D::Rotate(radiusArrow);
		modelMatrixArrow *= Transform2D::Scale(0.85f, 0.85f);
	}
	else { //trebuie sa se faca translatia sagetii
		modelMatrixArrow = glm::mat3(1);
		modelMatrixArrow *= Transform2D::Translate(100, 250 + translateY);
		modelMatrixArrow *= Transform2D::Translate(arrowTranslateMove.x, arrowTranslateMove.y);
		modelMatrixArrow *= Transform2D::Rotate(radiusArrow);
		modelMatrixArrow *= Transform2D::Scale(0.85f, 0.85f);
	}
	RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrixArrow);

	//recalculeaza coordonatele varfului sagetii
	float arrowTopX = arrowLength + modelMatrixArrow[2][0];
	float arrowTopY = window->GetResolution().y - modelMatrixArrow[2][1];
	if (radiusArrow != 0) {
		arrowTopX *= cosf(radiusArrow);
		arrowTopY *= sinf(radiusArrow);
	}
	/*else { 
		//arrowTopY *= sinf(radiusArrow);
	}*/

	arrowTop = glm::vec3(arrowTopX, arrowTopY, 0);

	modelMatrixBow = glm::mat3(1);
	modelMatrixBow *= Transform2D::Translate(100, 250 + translateY);
	modelMatrixBow *= Transform2D::Rotate(radiusArrow);
	modelMatrixBow *= Transform2D::Scale(0.85f, 0.85f);
	RenderMesh2D(meshes["bow"], shaders["VertexColor"], modelMatrixBow);

	//////////////////////////////////////////////////////////////////////////////
	
	LoadBallons();
	LoadShuriken();
	if (50 * angularStep >= window->GetResolution().x + 800) {
		angularStep = 0;
	}

	if (mouseClick == true) {
		modelMatrixPower = glm::mat3(1);
		modelMatrixPower *= Transform2D::Translate(100, 10);
		powerFactor += 0.09f;
		modelMatrixPower *= Transform2D::Translate(30, 90 + translateY);
		modelMatrixPower *= Transform2D::Scale(1 + powerFactor, 1);
		RenderMesh2D(meshes["powerBar"], shaders["VertexColor"], modelMatrixPower);
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
	//cout<<glm::to_string(modelMatrixArrow)<<std::endl;
	//calculeaza distanta de la mouse la varful sagetii
	float diffX = mouseX - arrowTop[0];
	float diffY = mouseY - arrowTop[1];
	if (diffX > 0) {
		radiusArrow = -atan(diffY / diffX);
	}
	mousePos = glm::vec2(mouseX, window->GetResolution().y - mouseY);
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	mouseClick = true;
	mouseClickTime = clock();
	throwArrow = false;
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	mouseClick = false;
	powerFactor = 0; //daca nu se mai tine apasat pe mouse, dispare powerBar
	mouseClickTime = float(clock() - mouseClickTime);
	//cout << "timp: " << mouseClickTime << endl;
	//trebuie sa se lanseze sageata
	throwArrow = true;
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}