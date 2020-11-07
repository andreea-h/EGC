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

	GenerateBallonColor(); //genereaza aleator culorile pentru cele 240 baloane
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
	//primul vertex este in centrul balonului
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
	int count = 0;
	for (i = 0; i < 30; i++) {
		if (yellowBallonsPos[i] != glm::mat3(-1)) {
			count++;
			RenderMesh2D(meshes["yellowBallon"], shaders["VertexColor"], yellowBallonsPos[i]);
		}
	}
	//cout << count << " baloane ramase" << endl;
		//std::cout << glm::to_string(yellowBallonsPos[i]) << std::endl;
		/*if (checkYellBallonCollision(glm::vec2(modelMatrixBallonY[2][0], modelMatrixBallonY[2][1])) == true) {
			modelMatrixBallonY = glm::mat3(1);
			modelMatrixBallonY *= Transform2D::Translate(1000, translateY_values[i]);
			modelMatrixBallonY *= Transform2D::Scale(0.1f, 1.25f);
			RenderMesh2D(meshes["yellowBallon"], shaders["VertexColor"], modelMatrixBallonY);
			cout << "aici2" << endl;
		}*/
}

//verifica coliziunea sagetii cu baloanele galbene
void Tema1::checkBallonCollision() {
	//verifica daca varful sagetii este inclus in cercul incadrator al unui dintre baloanele 
	int i;
	for (i = 0; i < 240; i++) {
		if (collisionMem[i] != 1) { //daca nu s-a mai produs coliziune pana la acest moment
			float xBallon = ballonPos[i].x;
			float yBallon = ballonPos[i].y;
			//(x - center_x)^2 + (y - center_y)^2 < radius^2
			if (powf(arrowTop.x - xBallon, 2.0f) + powf(arrowTop.y - yBallon, 2.0f) <
				powf(ballonRadius * 1.25f, 2.0)) {
				//coliziune sageata balon
				//std::cout << glm::to_string() << std::endl;
				collisionMem[i] = 1;
				//cout << collisionMem[i] << "; coliziune: x = " << xBallon << "; y = " << yBallon << endl;
				cout << "sageata: " << arrowTop.x << "," << arrowTop.y << endl;
				std::cout << "balon: " << glm::to_string(ballonPos[i]) << std::endl;
				//cout << "*******" << endl;
			}
		}
	}
}

void Tema1::LoadShuriken() {
	int i;
	for (i = 0; i < 3; i++) {
		modelMatrixShuriken = glm::mat3(1);
		//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
		modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 200 * i, 250);
		modelMatrixShuriken *= Transform2D::Translate(-20 * angularStep, 100 * i);
		modelMatrixShuriken *= Transform2D::Rotate(-angularStep); // genereaza rotatia in jurul propriului centru
		modelMatrixShuriken *= Transform2D::Scale(0.75f, 0.75f);
		RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
	}

	for (i = 0; i < 3; i++) {
		modelMatrixShuriken = glm::mat3(1);
		//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
		modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 400 * i, 250);
		modelMatrixShuriken *= Transform2D::Translate(-20 * angularStep, 100 * i);
		modelMatrixShuriken *= Transform2D::Rotate(-angularStep); // genereaza rotatia in jurul propriului centru
		modelMatrixShuriken *= Transform2D::Scale(0.75f, 0.75f);
		RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
	}

	for (i = 0; i < 3; i++) {
		modelMatrixShuriken = glm::mat3(1);
		//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
		modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 400 * i, 650);
		modelMatrixShuriken *= Transform2D::Translate(-20 * angularStep, 100 * i);
		modelMatrixShuriken *= Transform2D::Rotate(-angularStep); // genereaza rotatia in jurul propriului centru
		modelMatrixShuriken *= Transform2D::Scale(0.75f, 0.75f);
		RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
	}
}

void Tema1::GenerateBallonColor() {
	int i;
	srand(time(NULL));
	for (i = 0; i < 240; i++) {
		unsigned short colorRandom = rand() % 2;
		ballonColor[i] = colorRandom; // 1-rosu, 0-galben
	}
}

void Tema1::Update(float deltaTimeSeconds) {
	int i;
	angularStep += 3 * deltaTimeSeconds;
	
	
	translateBallonStep += 150 * deltaTimeSeconds; //cu cat se translateaza la fiecare cadru fiecare balon pe OY

	for (i = 0; i < 80; i++) { //baloanele apar in 3 coloane => 240 baloane
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
			//std::cout << glm::to_string(ballonPos[i]) << std::endl;
			/*glBegin(GL_LINE_LOOP);
			for (int j = 0; j < 300; i++) {
				double angle = 2 * M_PI * j / 300;
				double x = cosf(angle);
				double y = sinf(angle);
				glVertex2d(x + ballonPos[i].x, y + ballonPos[i].y);
			}
			glEnd();*/
		}
		else { //efect de animatie la distrugerea balonului
			scaleFactors[i] += 0.01;
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

	for (i = 80; i < 160; i++) {
		if (collisionMem[i] != 1) {
			modelMatrixBallonY = glm::mat3(1);
			modelMatrixBallonY *= Transform2D::Translate(1000, -250 * (i - 80) - 300);
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
			scaleFactors[i] += 0.01;
			modelMatrixBallonY = glm::mat3(1);
			modelMatrixBallonY *= Transform2D::Translate(1000, -250 * (i - 80) - 300);
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

	for (i = 160; i < 240; i++) {
		if (collisionMem[i] != 1) {
			modelMatrixBallonY = glm::mat3(1);
			modelMatrixBallonY *= Transform2D::Translate(1200, -300 * (i - 160) - 500);
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
			scaleFactors[i] += 0.01;
			modelMatrixBallonY = glm::mat3(1);
			modelMatrixBallonY *= Transform2D::Translate(1200, -300 * (i - 160) - 500);
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
	if (radiusArrow != 0) {
		arrowTopX += arrowLength * cosf(radiusArrow);
		arrowTopY += arrowLength * sinf(radiusArrow);
	}
	else { 
		arrowTopX += arrowLength; //daca radiusArrow ==0 nu exista proiectie pe axa OY
	}
	arrowTop = glm::vec3(arrowTopX, arrowTopY, 0);
	///////////////////////////////////////////////////////////////
	modelMatrixBow = glm::mat3(1);
	modelMatrixBow *= Transform2D::Translate(100, 250 + translateY);
	modelMatrixBow *= Transform2D::Rotate(radiusArrow);
	modelMatrixBow *= Transform2D::Scale(0.85f, 0.85f);
	RenderMesh2D(meshes["bow"], shaders["VertexColor"], modelMatrixBow);

	//////////////////////////////////////////////////////////////////////////////
	//LoadShuriken();
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
	checkBallonCollision();
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
		radiusArrow = atan(diffY / diffX);
	}	
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	mouseClick = true;
	arrowTranslateMove.x = 0;
	arrowTranslateMove.y = 0;
	throwArrow = false;
	mouseClickTime = clock(); //se va retine timpul de apasare pe mouse
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	mouseClick = false;
	mouseClickTime = clock() - mouseClickTime;
	mouseClickTime = (float)mouseClickTime/1000.0f; //timpul in secunde cat s-a tinut apasat pe mouse
	powerFactor = 0; //daca nu se mai tine apasat pe mouse, dispare powerBar
	//trebuie sa se lanseze sageata
	throwArrow = true;
	throwAngle = radiusArrow;
	mousePos = glm::vec2(mouseX, window->GetResolution().y - mouseY); //pozitia mouse-ului in coordonate utilizator
	cout << "mouse: " << mousePos.x << " , " << mousePos.y << endl;
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{

}