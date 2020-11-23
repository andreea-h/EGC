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

	Mesh* arrow = CreateArrow(5, 30);

	bowRadius = 100;
	Mesh* bow = CreateBow();

	ballonRadius = 30;
	Mesh* yellowBallon = CreateBallon('y', ballonRadius);

	Mesh* redBallon = CreateBallon('r', ballonRadius);
	//retine coordonate pentre viitoarele translatii de baloane si shurikene
	int i;
	for (i = 0; i < 10; i++) {
		translateY_values.push_back(0 - 200 * i);
	}
	Mesh* powerBar = CreateRectangle();

	GenerateBallonColor(); //genereaza aleator culorile pentru cele 80 baloane
	Mesh* ground = CreateGround();

	Mesh* gigel = CreateGigel();

	Mesh* cap_gigel = CreateHead();

	Mesh* eye = CreateEye();

	Mesh* diamond = CreateDiamond();

	Mesh* star = createStar();
}

Mesh* Tema1::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);

	// TODO: Send vertices data into the VBO buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//trimite date la vbo-ul activ
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	unsigned int IBO = 0;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(),
		&indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data
	// It will be learned later, when GLSL shaders will be introduced
	// For the moment just think that each property value from our vertex format needs to be send to a certain channel
	// in order to know how to receive it in the GLSL vertex shader

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// TODO: Unbind the VAO
	glBindVertexArray(0); //dezactiveaza legatura catre VAO-ul curent

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	return meshes[name];
}


Mesh* Tema1::createStar() {
	Mesh* star = new Mesh("star");
	vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(-0.5f, 0.5f, 0), glm::vec3(0.255, 0.412, 0.882)),
		VertexFormat(glm::vec3(0, 2, 0), glm::vec3(0.502, 0.000, 0.502)),
		VertexFormat(glm::vec3(0.5f, 0.5f, 0), glm::vec3(0.255, 0.412, 0.882)),
		VertexFormat(glm::vec3(2, 0, 0), glm::vec3(0.502, 0.000, 0.502)),
		VertexFormat(glm::vec3(0.5f, -0.5f, 0), glm::vec3(0.255, 0.412, 0.882)),
		VertexFormat(glm::vec3(0, -2, 0), glm::vec3(0.502, 0.000, 0.502)),
		VertexFormat(glm::vec3(-0.5f, -0.5f, 0), glm::vec3(0.255, 0.412, 0.882)),
		VertexFormat(glm::vec3(-2, 0, 0), glm::vec3(0.502, 0.000, 0.502)),
	};

	vector<unsigned short> indices =
	{
		0, 2, 1,
		7, 6, 0,
		2, 4, 3,
		6, 5, 4,
		0, 6, 4,
		2, 0, 4
	};
	star = CreateMesh("star", vertices, indices);
	return star;
}

Mesh* Tema1::CreateEye() {
	glm::vec3 color = glm::vec3(0.000, 0.000, 0.545);
	Mesh* eye = new Mesh("eye");
	vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(20, 0, 0), color),
		VertexFormat(glm::vec3(0, -20, 0), color),
		VertexFormat(glm::vec3(20, -20, 0), color)
	};
	vector<unsigned short> indices =
	{
		0, 2, 1,
		1, 2, 3
	};
	eye = CreateMesh("eye", vertices, indices);
	return eye;
}

Mesh* Tema1::CreateHead() {
	glm::vec3 color = glm::vec3(0.867, 0.627, 0.867);
	Mesh* cap_gigel = new Mesh("cap_gigel");
	vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(20, 0, 0), color),
		VertexFormat(glm::vec3(0, -20, 0), color),
		VertexFormat(glm::vec3(20, -20, 0), color)
	};
	
	vector<unsigned short> indices = 
	{
		0, 2, 1,
		1, 2, 3
	};

	cap_gigel = CreateMesh("cap_gigel", vertices, indices);
	return cap_gigel;
}

Mesh* Tema1::CreateGigel() {
	Mesh* gigel = new Mesh("gigel");
	glm::vec3 color = glm::vec3(0.867, 0.627, 0.867);
	vector<VertexFormat> vertices = {
		//incepe cu coltul din stanga sus al gatului
		VertexFormat(glm::vec3(10, 10, 0), color),
		VertexFormat(glm::vec3(20, 10, 0), color),
		VertexFormat(glm::vec3(10, 0, 0), color),
		VertexFormat(glm::vec3(20, 0, 0), color),

		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(30, 0, 0), color),
		//zona picioarelor
		VertexFormat(glm::vec3(0, -60, 0), color),
		VertexFormat(glm::vec3(30, -60, 0), color),
		VertexFormat(glm::vec3(0, -120, 0), color),
		VertexFormat(glm::vec3(10, -120, 0), color),
		VertexFormat(glm::vec3(20, -120, 0), color),
		VertexFormat(glm::vec3(30, -120, 0), color),

		VertexFormat(glm::vec3(10, -60, 0), color),
		VertexFormat(glm::vec3(20, -60, 0), color),
		VertexFormat(glm::vec3(-20, -65, 0), color), //14
		VertexFormat(glm::vec3(20, -30, 0), color), //15
		VertexFormat(glm::vec3(-10, -65, 0), color), //16
		VertexFormat(glm::vec3(30, -10, 0), color),
		VertexFormat(glm::vec3(30, -45, 0), color),
		VertexFormat(glm::vec3(20, -45, 0), color),
	};

	vector<unsigned short> indices =
	{
		0, 2, 1,
		1, 2, 3,
		4, 6, 5, 
		5, 6, 7, 
		6, 8, 12,
		12, 8, 9, 
		13, 10, 7,
		7, 10, 11, 
		14, 15, 4,
		4, 15, 16,
		5, 19, 17, 
		17, 19, 18
	};
	gigel = CreateMesh("gigel", vertices, indices);
	return gigel;
}

Mesh* Tema1::CreateDiamond() {
	Mesh* diamond = new Mesh("diamond");
	glm::vec3 color = glm::vec3(0.000, 0.749, 1.000);
	vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.878, 1.000, 1.000)), //0
		VertexFormat(glm::vec3(-20, 15, 0), glm::vec3(0.529, 0.808, 0.980)),//1
		VertexFormat(glm::vec3(20, 15, 0), glm::vec3(0.529, 0.808, 0.980)),//2
		VertexFormat(glm::vec3(-35, 5, 0), glm::vec3(0.878, 1.000, 1.000)),//3
		VertexFormat(glm::vec3(35, 5, 0), glm::vec3(0.878, 1.000, 1.000)),//4
		VertexFormat(glm::vec3(0, -40, 0), color)//5
	};
	vector<unsigned short> indices =
	{
		1, 0, 2,
		3, 0, 1,
		2, 0, 4,
		3, 5, 0, 
		0, 5, 4
	};
	diamond = CreateMesh("diamond", vertices, indices);
	return diamond;
}

Mesh* Tema1::CreateGround() {
	glm::vec3 color = glm::vec3(0.545, 0.271, 0.075);
	vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(0, 20, 0), color),
		VertexFormat(glm::vec3(window->GetResolution().x, 20, 0), color),
		VertexFormat(glm::vec3(window->GetResolution().x, 0, 0), color),
	};
	vector<unsigned short> indices =
	{
		1, 0, 2,
		0, 3, 2
	};
	Mesh* ground = new Mesh("ground");
	ground = CreateMesh("ground", vertices, indices);
	return ground;
}

Mesh* Tema1::CreateRectangle() {
	glm::vec3 color = glm::vec3(0.196, 0.804, 0.196);
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
	powerBar = CreateMesh("powerBar", vertices, indices);
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
	arrow = CreateMesh("arrow", vertices, indices);
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
	shuriken = CreateMesh("shuriken", vertices, indices);
	return shuriken;
}

Mesh* Tema1::CreateBow() {
	glm::vec3 color = glm::vec3(0.863, 0.078, 0.235);
	
	vector<VertexFormat> vertices;
	float i, j;
	int step1 = 0;
	for (i = 0.4; i <= M_PI - 0.4; i += 0.001) {
		vertices.push_back(VertexFormat(glm::vec3(bowRadius * sinf(i), bowRadius * cosf(i), 0), color));
		step1++;
	}
	vector<unsigned short> indices;
	int k;
	
	for (k = 0; k < step1; k++) {
		indices.push_back(k);
	}
	Mesh* bow = new Mesh("bow");
	bow = CreateMesh("bow", vertices, indices);
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
	maxBallonRadius = ballonRadius + 33;

	int j;
	for (j = 1; j <= step; j++) {
		indices.push_back(j);
		indices.push_back(j - 1);
		indices.push_back(0); //"uneste" de centru 
	}
	for (j = step + 1; j <= step + 27; j++) {
		indices.push_back(j);
	}
	
	if (color == 'y') {
		Mesh* yellowBallon = new Mesh("yellowBallon");
		yellowBallon = CreateMesh("yellowBallon", vertices, indices);
		return yellowBallon;
	}
	if (color == 'r') {
		Mesh* redBallon = new Mesh("redBallon");
		redBallon = CreateMesh("redBallon", vertices, indices);
		return redBallon;
	}
}

//verifica coliziuniea sagetii cu fiecare shuniker
void Tema1::checkShurikenColl() {
	int i;
	bool ok = false;
	for (i = 0; i < 130; i++) {
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

//verifica coliziunea sagetii cu baloanele 
void Tema1::checkBallonCollision() {
	//verifica daca varful sagetii este inclus in cercul incadrator al fiecarui balon
	int i;
	bool newScore = false;
	for (i = 0; i < 80; i++) {
		if (ballonPos[i].y <= window->GetResolution().y) {
			if (collisionMem[i] != 1) { //daca nu s-a mai produs coliziune pana la acest moment
				float xBallon = ballonPos[i].x;
				float yBallon = ballonPos[i].y;
				//(x - center_x)^2 + (y - center_y)^2 < radius^2 => coliziune
				if (powf(arrowTop.x - xBallon, 2.0f) + powf(arrowTop.y - yBallon, 2.0f) <
					powf(maxBallonRadius * 1.25f, 2.0)) { //se tine cont de factorul de scalare pe y (1.25) dat la render
					collisionMem[i] = 1;
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
	for (i = 0; i < 80; i++) {
		float posY = ballonPos[i].y;
		if ((posY >= window->GetResolution().y + 100) || collisionMem[i] == 1) { 
			count++;
		}
	}
	if (count == 80) {
		return true;
	}
	return false;
}

//intoarce true daca cercurile date prin parametri sunt in coliziune
bool Tema1::circleCollision(glm::vec2 center1, glm::vec2 center2, float radius1, float radius2)
{
	float diffX = center1.x - center2.x;
	float diffY = center1.y - center2.y;
	float dist = sqrt(diffX * diffX + diffY * diffY);
	if (dist < radius1 + radius2) { //coliziune
		return true;
	}
	return false;
}

//verifica coliziunea arcului cu shurikenele
void Tema1::checkBowCollision() {
	int i;
	for (i = 0; i < 130; i++) { //verifica pe rand coliziunea cu fiecare shuriken
		bool check = circleCollision(bowCircleCoord, shurikenPos[i],
			100 * 0.85f, shurikenSide * sqrt(2) * 0.75f);
		if (check == true && collisionMemBow[i] == 0) {
			livesCount -= 1;
			collisionMemBow[i] = 1;
			cout << "Ai pierdut o viata :( Mai ai " << livesCount << " vieti" << endl;
		}
	}
}

//recalculeaza pozitiile shurikenelor si le incarca in scena
void Tema1::LoadShuriken(float deltaTimeSeconds) {
	int i;
	for (i = 0; i < 30; i++) {
		if (collisionMemShuriken[i] != 1) {
			modelMatrixShuriken = glm::mat3(1);
			//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
			modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 200 * i, 100 + i * 20); //ok
			modelMatrixShuriken *= Transform2D::Translate(-20 * angularStep, 0); //translatia de-a lungul axei OX
			modelMatrixShuriken *= Transform2D::Rotate(-angularStep); // genereaza rotatia in jurul propriului centru
			modelMatrixShuriken *= Transform2D::Scale(0.75f, 0.75f);
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
			shurikenPos[i] = glm::vec2(modelMatrixShuriken[2][0] + modelMatrixShuriken[0][0],
				modelMatrixShuriken[2][1] + modelMatrixShuriken[1][1]);
		}
		else {
			scaleFactorsShuriken[i] += 0.5 * deltaTimeSeconds;
			modelMatrixShuriken = glm::mat3(1);
			//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
			modelMatrixShuriken *= Transform2D::Translate(-20 * angularStep, 0); //translatia de-a lungul axei OX
			modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 200 * i, 100 + i * 20); //ok
			modelMatrixShuriken *= Transform2D::Scale(0.75f - scaleFactorsShuriken[i], 0.75f - scaleFactorsShuriken[i]);
			if (0.75f - scaleFactorsShuriken[i] > 0) {
				RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
			}
		}
	}
	for (i = 30; i < 50; i++) {
		if (collisionMemShuriken[i] != 1) {
			modelMatrixShuriken = glm::mat3(1);
			//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
			modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 300 * (i - 30) + 200, 750 - i * 10); //ok
			modelMatrixShuriken *= Transform2D::Translate(-20 * angularStep, 0); //translatia de-a lungul axei OX
			modelMatrixShuriken *= Transform2D::Rotate(-angularStep); // genereaza rotatia in jurul propriului centru
			modelMatrixShuriken *= Transform2D::Scale(0.75f, 0.75f);
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
			shurikenPos[i] = glm::vec2(modelMatrixShuriken[2][0] + modelMatrixShuriken[0][0],
				modelMatrixShuriken[2][1] + modelMatrixShuriken[1][1]);
		}
		else {
			scaleFactorsShuriken[i] += 0.5 * deltaTimeSeconds;
			modelMatrixShuriken = glm::mat3(1);
			//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
			modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 300 * (i - 30) + 200, 750 - i * 10); //ok
			modelMatrixShuriken *= Transform2D::Translate(-20 * angularStep, 0); //translatia de-a lungul axei OX
			modelMatrixShuriken *= Transform2D::Scale(0.75f - scaleFactorsShuriken[i], 0.75f - scaleFactorsShuriken[i]);
			if (0.75f - scaleFactorsShuriken[i] > 0) {
				RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
			}
		}
		
	}
	for (i = 50; i < 60; i++) { //ultimul set de shurikene care devine vizibil in fereastra
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
		else {
			scaleFactorsShuriken[i] += 0.5 * deltaTimeSeconds;
			modelMatrixShuriken = glm::mat3(1);
			//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
			modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 300 * i + 1500, 50 + i * 5); //ok
			modelMatrixShuriken *= Transform2D::Translate(-20 * angularStep, 0); //translatia de-a lungul axei OX
			modelMatrixShuriken *= Transform2D::Scale(0.75f - scaleFactorsShuriken[i], 0.75f - scaleFactorsShuriken[i]);
			if (0.75f - scaleFactorsShuriken[i] > 0) {
				RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
			}
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
		else {
			scaleFactorsShuriken[i] += 0.5 * deltaTimeSeconds;
			modelMatrixShuriken = glm::mat3(1);
			//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
			modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 250 * (i - 60) + 50, 900 - i * 10); //ok
			modelMatrixShuriken *= Transform2D::Translate(-20 * angularStep, 0); //translatia de-a lungul axei OX
			modelMatrixShuriken *= Transform2D::Scale(0.75f - scaleFactorsShuriken[i], 0.75f - scaleFactorsShuriken[i]);
			if (0.75f - scaleFactorsShuriken[i] > 0) {
				RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
			}
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
		else {
			scaleFactorsShuriken[i] += 0.5 * deltaTimeSeconds;
			modelMatrixShuriken = glm::mat3(1);
			//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
			modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 250 * (i - 80) + 2000, 300 - i); //ok
			modelMatrixShuriken *= Transform2D::Translate(-20 * angularStep, 0); //translatia de-a lungul axei OX
			modelMatrixShuriken *= Transform2D::Scale(0.75f - scaleFactorsShuriken[i], 0.75f - scaleFactorsShuriken[i]);
			if (0.75f - scaleFactorsShuriken[i] > 0) {
				RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
			}
		}
	}
	for (i = 100; i < 130; i++) {
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
		else {
			scaleFactorsShuriken[i] += 0.5 * deltaTimeSeconds;
			modelMatrixShuriken = glm::mat3(1);
			//la fiecare apel de update() shuriken-ul se translateaza din dreapta scenei la o pozitie din [0, window->GetResolution().x + 200)
			modelMatrixShuriken *= Transform2D::Translate(window->GetResolution().x + 300 * (i - 100) + 2000, 700 - i); //ok
			modelMatrixShuriken *= Transform2D::Translate(-20 * angularStep, 0); //translatia de-a lungul axei OX
			modelMatrixShuriken *= Transform2D::Scale(0.75f - scaleFactorsShuriken[i], 0.75f - scaleFactorsShuriken[i]);
			if (0.75f- scaleFactorsShuriken[i] > 0) {
				RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
			}
		}
	}
}

//genereaza aleator culoarea baloanelor
void Tema1::GenerateBallonColor() {
	int i;
	srand(time(NULL));
	for (i = 0; i < 80; i++) {
		unsigned short colorRandom = rand() % 2;
		ballonColor[i] = colorRandom; // 1-rosu, 0-galben
	}
}

//recalculeaza pozitiile baloanelor si le incarca in scena
void Tema1::LoadBallons(float deltaTimeSeconds) {
	int i;
	for (i = 0; i < 30; i++) { 
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
	for (i = 30; i < 60; i++) {
		if (collisionMem[i] != 1) {
			modelMatrixBallonY = glm::mat3(1);
			modelMatrixBallonY *= Transform2D::Translate(1000, -250 * (i - 30) - 300);
			//a doua coloana de baloane se misca mai rapid in sus pe OY (factorul de translatie pe OY este mai mare)
			modelMatrixBallonY *= Transform2D::Translate(0, translateBallonStep * 1.25f);
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
			modelMatrixBallonY *= Transform2D::Translate(1000, -250 * (i - 30) - 300);
			modelMatrixBallonY *= Transform2D::Translate(0, translateBallonStep * 1.25f - 150 * deltaTimeSeconds);
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
	for (i = 60; i < 80; i++) {
		if (collisionMem[i] != 1) {
			modelMatrixBallonY = glm::mat3(1);
			modelMatrixBallonY *= Transform2D::Translate(1200, -300 * (i - 60) - 500);
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
			modelMatrixBallonY *= Transform2D::Translate(1200, -300 * (i - 60) - 500);
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

//incarca in scena forme sub forma de diamante care la coliziunea cu sageata ofera vieti in plus jucatorului
void Tema1::LoadDiamonds(float deltaTimeSeconds) {
	int i;
	for (i = 0; i < 15; i++) {
		if (collisionMemDiamond[i] != 1) { 
			modelMatrix = glm::mat3(1);
			if (i % 2 == 0) {
				modelMatrix *= Transform2D::Translate(700, -400 * i - 100);
			}
			else if (i % 3 == 0) {
				modelMatrix *= Transform2D::Translate(1050, -400 * i - 100);
			}
			else {
				modelMatrix *= Transform2D::Translate(950, -300 * i - 100);
			}
			modelMatrix *= Transform2D::Translate(0, translateBallonStep);
			RenderMesh2D(meshes["diamond"], shaders["VertexColor"], modelMatrix);
			diamondsPos[i] = glm::vec2(modelMatrix[2][0] + modelMatrix[1][0] + modelMatrix[0][0],
				modelMatrix[2][1] + modelMatrix[1][1] + modelMatrix[0][1]);
		}
		else {
			modelMatrix = glm::mat3(1);
			if (i % 2 == 0) {
				modelMatrix *= Transform2D::Translate(700, 0);
			}
			else if (i % 3 == 0) {
				modelMatrix *= Transform2D::Translate(1050, 0);
			}
			else {
				modelMatrix *= Transform2D::Translate(950, 0);
			}
			diamondTranslateY[i] -= 150 * deltaTimeSeconds;
			modelMatrix *= Transform2D::Translate(0, diamondTranslateY[i]);
			modelMatrix *= Transform2D::Rotate(0.785);
			RenderMesh2D(meshes["diamond"], shaders["VertexColor"], modelMatrix);
		}
	}
}

//verifica coliziunea sagetii cu diamantele
void Tema1::checkDiamondColl() {
	int i;
	bool newScore = false;
	for (i = 0; i < 15; i++) {
		if (diamondsPos[i].y <= window->GetResolution().y) {
			if (collisionMemDiamond[i] != 1) { //daca nu s-a mai produs coliziune pana la acest moment
				//(x - center_x)^2 + (y - center_y)^2 < radius^2
				float diamondX = diamondsPos[i].x;
				float diamondY = diamondsPos[i].y;
				if (powf(arrowTop.x - diamondX, 2.0f) + powf(arrowTop.y - diamondY, 2.0f) < powf(40.0f, 2.0)) {
					collisionMemDiamond[i] = 1;
					livesCount++;
					cout << "Yey! Ai o viata in plus! Numar de vieti: " << livesCount << endl;
					diamondTranslateY[i] = diamondY;
				}
			}
		}
	}
}

//incarca in scena un numar de stelute egal cu numarul de vieti al jucatorului
void Tema1::LoadStars() {
	int i;
	for (i = 0; i < livesCount; i++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(30 + 50 * i, 680);
		modelMatrix *= Transform2D::Scale(8, 8);
		RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
	}
}

//intoarce true daca toate shurikenele au fost facute vizibile la un moment dat in scena
bool Tema1::checkShurikenPos() {
	int i;
	int count = 0;
	for (i = 0; i < 130; i++) {
		if (collisionMemShuriken[i] != 1) {
			cout << shurikenPos[i].x << endl;
			if (shurikenPos[i].x < 0) {
				count++;
			}
		}
	}
	if (count == 130) {
		return true;
	}
	return false;
}

void Tema1::Update(float deltaTimeSeconds) {
	if (livesCount > 0) {
		glLineWidth(1.5f);
		RenderMesh2D(meshes["ground"], shaders["VertexColor"], glm::mat3(1));
		//ochii lui gigel
		modelMatrixArrow = glm::mat3(1);
		modelMatrixArrow *= Transform2D::Translate(65, 350 + translateY);
		modelMatrixArrow *= Transform2D::Scale(0.5f, 0.5f);
		RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrixArrow);

		modelMatrixArrow = glm::mat3(1);
		modelMatrixArrow *= Transform2D::Translate(85, 350 + translateY);
		modelMatrixArrow *= Transform2D::Scale(0.5f, 0.5f);
		RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrixArrow);

		//zambetul lui gigel
		modelMatrixArrow = glm::mat3(1);
		modelMatrixArrow *= Transform2D::Translate(81.5f, 335 + translateY);
		modelMatrixArrow *= Transform2D::Rotate(-1.57f);
		modelMatrixArrow *= Transform2D::Scale(0.1f, 0.085f);
		RenderMesh2D(meshes["bow"], shaders["VertexColor"], modelMatrixArrow);

		//corpul lui gigel
		modelMatrixArrow = glm::mat3(1);
		modelMatrixArrow *= Transform2D::Translate(50, 300 + translateY);
		modelMatrixArrow *= Transform2D::Scale(2.0f, 1.5f);
		RenderMesh2D(meshes["gigel"], shaders["VertexColor"], modelMatrixArrow);
		//capul lui gigel
		modelMatrixArrow = glm::mat3(1);
		modelMatrixArrow *= Transform2D::Translate(55, 360 + translateY);
		modelMatrixArrow *= Transform2D::Scale(2.5f, 2.5f);
		RenderMesh2D(meshes["cap_gigel"], shaders["VertexColor"], modelMatrixArrow);
		
		LoadDiamonds(deltaTimeSeconds); //diamantele
		LoadStars(); //stelutele care reprezinta numarul de vieti
		int i;
		translateBallonStep += 150 * deltaTimeSeconds; //cu cat se translateaza la fiecare cadru fiecare balon pe OY
		
		bool check = checkBallonPos();
		if (check == false) { //daca inca nu s-au facut toate baloanele vizibile in fereastra
			LoadBallons(deltaTimeSeconds);
		}
		else if (check == true) { //au fost afisate baloanele
			angularStep += 5 * deltaTimeSeconds; //pentru deplasarea respectiv rotirea shuriken-ului
			LoadShuriken(deltaTimeSeconds);
		}

		//aruncare sageata
		if (throwArrow == true) {
			arrowTranslateMove.x += (1000 * mouseClickTime) * deltaTimeSeconds;
			arrowTranslateMove.y += (1000 * mouseClickTime) * deltaTimeSeconds;
			modelMatrixArrow = glm::mat3(1);
			modelMatrixArrow *= Transform2D::Translate(110, 250 + translateY);
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
			modelMatrixArrow *= Transform2D::Translate(110, 250 + translateY);
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
		
		//randare arc
		modelMatrixBow = glm::mat3(1);
		modelMatrixBow *= Transform2D::Translate(90, 250 + translateY);
		modelMatrixBow *= Transform2D::Rotate(radiusArrow);
		modelMatrixBow *= Transform2D::Scale(0.85f, 0.85f);
		RenderMesh2D(meshes["bow"], shaders["VertexColor"], modelMatrixBow);

		//recalculare coordonatele cercului incadrator al arcului
		bowCircleCoord.x = modelMatrixBow[2][0] + modelMatrixBow[1][0] + modelMatrixBow[0][0];
		bowCircleCoord.y  = modelMatrixBow[2][1] + modelMatrixBow[1][1] + modelMatrixBow[0][1];
		
		//se afiseaza dreptunghiul care reprezinta puterea a apasare, scalat in functie de apasarea mouse-ului
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
		if (check == false) { //inca mai exista baloane vizibile in scena
			if (throwArrow == true) {
				checkBallonCollision();
				checkDiamondColl();
			}
		}
		if (check == true) { //au fost deja facute vizibile toate baloanele in scena
			if (throwArrow == true) { //coliziunile se verifica doar in momentul in care este aruncat arcul
				checkShurikenColl(); //verifica coliziunea sagetii cu shurikenele
			}
			checkBowCollision(); //verifica coliziunea sagetii cu shurikenele
		}
		//daca sageata nu mai este vizibila in fereastra, va fi translatata in pozitia initiala ca fiind atasata arcului
		if (sageata.x >= window->GetResolution().x) {
			throwArrow = false;
		}
	}
	else { //daca s-au pierdut toate vietile, se inchide fereastra
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
		if (250 + translateY - bowRadius <= 0) { //verifica daca arcul nu dispare din fereastra
			//daca arcul nu ar mai fi vizibil, ajusteaza pozitia
			translateY += deltaTime * 300;
		}
	}
	if (window->KeyHold(GLFW_KEY_W)) {
		translateY += deltaTime * 300;
		if (bowRadius + translateY + 250 >= window->GetResolution().y) { //verifica daca arcu nu dispare din fereastra
			//daca arcul nu ar mai fi vizibil, ajusteaza pozitia
			translateY -= deltaTime * 300;
		}
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
	float diffX = mouseX - modelMatrixArrow[2][0];
	float diffY = window->GetResolution().y - mouseY - modelMatrixArrow[2][1];
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
		if (aux / 1000.0f <= 4) { //se poate arunca o sageata o data la 3s
			cout << "Mai trebuie sa astepti " << 4 - aux/1000.0f << " secunde inainte de a arunca inca a sageata :)" << endl;
			mouseClick = false;
			powerFactor = 0;
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
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{

}