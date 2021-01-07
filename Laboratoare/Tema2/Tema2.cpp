#include "Tema2.h"
#include "Transform3D.h"
#include "Transform2D.h"

#include <vector>
#include <string>
#include <iostream>
#include <time.h> 
#include <Core/Engine.h>

using namespace std;

Tema2::Tema2()
{
	Mesh* star = CreateStar();
	FoV = 45.0f;
	//defineste pozitia initiala a camerei
	thirdPersonCamPosition = glm::mat4(1);
	thirdPersonCamPosition = Transform3D::Translate(0, 1.75f, 3.5f);
	thirdPersonCam = true;

	//defineste pozitia camerei firstPerson
	firstPersonCamPosition = glm::mat3(1);
	glm::vec3 pos = player.getInitialPlayerCoords();
	firstPersonCamPosition *= Transform3D::Translate(pos.x, pos.y, pos.z - 0.75f);
	
	targetPosition = firstPersonCamPosition;
	targetPosition *= Transform3D::Translate(0.f, 0.f, -2.0f);

	renderCameraTarget = false;
	camera = new Tema_2::Camera();
	camera->Set(thirdPersonCamPosition * glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	projectionMatrix = glm::perspective(RADIANS(FoV), window->props.aspectRatio, 0.01f, 200.0f);

	platforms = new Platform();

	//defineste coordonatele la care urmeaza sa fie redate cuburile
	platforms->generatePlatform();
	setTranslatePoints();
}

Tema2::~Tema2()
{
	delete camera;
}

Mesh* Tema2::DefineSquare() {
	glm::vec3 color = glm::vec3(0.859, 0.439, 0.576);
	Mesh* square = new Mesh("square");
	vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(1, 0, 0), color),
		VertexFormat(glm::vec3(0, -1, 0), color),
		VertexFormat(glm::vec3(1, -1, 0), color)
	};
	vector<unsigned short> indices =
	{
		0, 2, 1,
		1, 2, 3
	};
	square->InitFromData(vertices, indices);
	meshes["square"] = square;
	return square;
}

Mesh* Tema2::DefineBlackSquare() {
	glm::vec3 color = glm::vec3(1.000, 0.980, 0.980);
	Mesh* square = new Mesh("black_square");
	vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(1, 0, 0), color),
		VertexFormat(glm::vec3(0, -1, 0), color),
		VertexFormat(glm::vec3(1, -1, 0), color)
	};
	vector<unsigned short> indices =
	{
		0, 2, 1,
		1, 2, 3
	};
	square->InitFromData(vertices, indices);
	meshes["black_square"] = square;
	return square;
}


void Tema2::Init()
{
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = DefineSquare(); //patrat folosit ulterior pentru fuel bar
	}

	{
		Mesh* mesh = DefineBlackSquare(); //patrat folosit ulterior pentru fuel bar
	}

	//adauga un shader pentru aplicarea culorii in vectorul shaders
	Shader* shader = new Shader("colorShader");
	shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders["colorShader"] = shader;

	//adauga un shader cu aplicarea unei functii de zgomot
	Shader* shader1 = new Shader("noiseShader");
	shader1->AddShader("Source/Laboratoare/Tema2/Shaders/NoiseShaderVS.glsl", GL_VERTEX_SHADER);
	shader1->AddShader("Source/Laboratoare/Tema2/Shaders/NoiseShaderFS.glsl", GL_FRAGMENT_SHADER);
	shader1->CreateAndLink();
	shaders["noiseShader"] = shader1;
}

Mesh* Tema2::CreateStar() {
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

Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
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

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	int timeLoc = glGetUniformLocation(shader->program, "time");
	glUniform1f(timeLoc, Engine::GetElapsedTime());

	srand(time(NULL));
	int randVal = glGetUniformLocation(shader->program, "rand");
	glUniform1f(randVal, rand() % 100 + 10);

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform
	GLint lightPosLoc = glGetUniformLocation(shader->program, "light_position");
	glUniform3f(lightPosLoc, lightPosition.x, lightPosition.y, lightPosition.z);

	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = camera->position;
	int eyePosLoc = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eyePosLoc, eyePosition.x, eyePosition.y, eyePosition.z);

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	//componeneta difuza=intensitatea de material difuza
	GLint varLoc = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(varLoc, materialKd);
	//componenta speculara=intensitatea de material speculara
	varLoc = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(varLoc, materialKs);

	varLoc = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(varLoc, materialShininess);

	varLoc = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(varLoc, color.x, color.y, color.z);

	// Bind model matrix
	GLint loc_modelMatrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_modelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_viewMatrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_viewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projectionMatrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.400, 0.804, 0.667, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::setTranslatePoints()
{
	int i;
	for (i = 0; i < 3; i++) {
		platforms->setTranslatePoint(i, -5.5f - platforms->getPlatformSize(i) / 2);
	}
	float maxCoord = -999;
	for (i = 0; i < 3; i++) {
		if (platforms->getPlatformSize(i) > maxCoord) {
			maxCoord = platforms->getPlatformSize(i);
		}
	}
	for (i = 3; i < 6; i++) {
		platforms->setTranslatePoint(i, -5.5f - maxCoord - 1 - platforms->getPlatformSize(i) / 2);
	}
	float maxCoord1 = -999;
	for (i = 3; i < 6; i++)
	{
		if (platforms->getPlatformSize(i) > maxCoord1) {
			maxCoord1 = platforms->getPlatformSize(i);
		}
	}
	for (i = 6; i < 9; i++) {
		platforms->setTranslatePoint(i, -5.5f - maxCoord - 2 - platforms->getPlatformSize(i) / 2 - maxCoord1);
	}

	float maxCoord2 = -999;
	for (i = 6; i < 9; i++)
	{
		if (platforms->getPlatformSize(i) > maxCoord2) {
			maxCoord2 = platforms->getPlatformSize(i);
		}
	}

	for (i = 9; i < 12; i++) {
		platforms->setTranslatePoint(i, -5.5f - maxCoord - 3 - platforms->getPlatformSize(i) / 2 - maxCoord1 - maxCoord2);
	}

	float maxCoord3 = -999;
	for (i = 9; i < 12; i++)
	{
		if (platforms->getPlatformSize(i) > maxCoord3) {
			maxCoord3 = platforms->getPlatformSize(i);
		}
	}

	for (i = 12; i < 15; i++) {
		platforms->setTranslatePoint(i, -5.5f - maxCoord - 4 - platforms->getPlatformSize(i) / 2 - maxCoord1 - maxCoord2 - maxCoord3);
	}
}

void Tema2::LoadStars() {
	int i;
	for (i = 0; i < lives; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(3 + i, 3.5f, -7.5f);
		modelMatrix *= Transform3D::Scale(0.1, 0.1, 0.1);
		RenderMesh(meshes["star"], shaders["VertexColor"], modelMatrix);
	}
}

//cuburile nou generate vor fi mereu afisate la aceeasi distanta de jucator pe axa OZ
void Tema2::LoadPlatforms() {
	int i;
	bool ok1 = false;
	int count = 0;

	for (i = 0; i < 3; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(2.5f * (i - 1), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
		modelMatrix *= Transform3D::Scale(1.95f, 0.25f, platforms->getPlatformSize(i));
		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		float zCoord = position.z;
		platforms->setPlatformXCoord(position.x, i);
		platforms->setPlatformYCoord(position.y, i);
		platforms->setPlatformZCoord(zCoord - platforms->getPlatformSize(i) / 2, i);

		//platformele care dispar in spatele camerei nu mai sunt redate
		if (platforms->getPlatformPos(i) <= 2.5f) {
			RenderSimpleMesh(meshes["box"], shaders["colorShader"], modelMatrix, platforms->getPlatformColor(i));
		}
		else {
			count++;
		}
	}
	
	if (count == 3) {
		ok1 = true;//platformele de la 0 la 2 nu vor mai aparea, trebuie sterse
	}
	count = 0;
	//1.25f este distanta dintre platforme
	for (i = 3; i < 6; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(2.5f * (i - 4), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
		modelMatrix *= Transform3D::Scale(1.95f, 0.25f, platforms->getPlatformSize(i));
		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		float zCoord = position.z;
		platforms->setPlatformXCoord(position.x, i);
		platforms->setPlatformYCoord(position.y, i);
		platforms->setPlatformZCoord(zCoord - platforms->getPlatformSize(i) / 2, i);
		RenderSimpleMesh(meshes["box"], shaders["colorShader"], modelMatrix, platforms->getPlatformColor(i));
	}
	for (i = 6; i < 9; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(2.5f * (i - 7), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
		modelMatrix *= Transform3D::Scale(1.95f, 0.25f, platforms->getPlatformSize(i));
		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		float zCoord = position.z;
		platforms->setPlatformXCoord(position.x, i);
		platforms->setPlatformYCoord(position.y, i);
		platforms->setPlatformZCoord(zCoord - platforms->getPlatformSize(i) / 2, i);
		RenderSimpleMesh(meshes["box"], shaders["colorShader"], modelMatrix, platforms->getPlatformColor(i));
	}
	for (i = 9; i < 12; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(2.5f * (i - 10), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
		modelMatrix *= Transform3D::Scale(1.95f, 0.25f, platforms->getPlatformSize(i));
		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		float zCoord = position.z;
		platforms->setPlatformXCoord(position.x, i);
		platforms->setPlatformYCoord(position.y, i);
		platforms->setPlatformZCoord(zCoord - platforms->getPlatformSize(i) / 2, i);
		RenderSimpleMesh(meshes["box"], shaders["colorShader"], modelMatrix, platforms->getPlatformColor(i));
	}
	for (i = 12; i < 15; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(2.5f * (i - 13), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
		modelMatrix *= Transform3D::Scale(1.95f, 0.25f, platforms->getPlatformSize(i));
		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		float zCoord = position.z;
		platforms->setPlatformXCoord(position.x, i);
		platforms->setPlatformYCoord(position.y, i);
		platforms->setPlatformZCoord(zCoord - platforms->getPlatformSize(i) / 2, i);
		RenderSimpleMesh(meshes["box"], shaders["colorShader"], modelMatrix, platforms->getPlatformColor(i));
	}

	//sterge platformele care nu mai sunt vizibile si genereaza altele in locul lor
	if (ok1 == true) {
		//calculeaza lungimea celei mai lungi platforme din cele care se sterg
		int i;
		float maxSize = -999;
		for (i = 0; i < 3; i++) {
			if (platforms->getPlatformSize(i) > maxSize) {
				maxSize = platforms->getPlatformSize(i);
			}
		}
		platforms->setLastMaxPlatform(maxSize);
		platforms->deletePlatform(0);
		platforms->deletePlatform(0);
		platforms->deletePlatform(0);
		platforms->generateNewPlatforms();
	}
}

// reda jucatorul in scena
void Tema2::LoadPlayer(float delta) {
	if (fallingPlayer == true) {
		glm::mat4 modelMatrix = glm::mat4(1);
		glm::vec3 pos = player.getActualPlayerCoords();
		modelMatrix *= Transform3D::Translate(pos.x, pos.y, pos.z);
		modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
		glm::vec4 coords = modelMatrix * glm::vec4(0, 0, 0, 1);
		//salveaza coordonatele actuale ale jucatorului
		float y = coords.y;
		y -= 2 * delta;
		
		player.setActualCoords(glm::vec4(coords.x, y, coords.z, 1));

		if (y < -5) { //jucatorul s-a aflat suficient timp in cadere, jocul se va incheia
			stopGame = true;
		}
		if (stopGame == false) {
			RenderSimpleMesh(meshes["sphere"], shaders["colorShader"], modelMatrix, glm::vec3(0.545, 0.000, 0.545));

			firstPersonCamPosition = glm::mat4(1);
			firstPersonCamPosition *= Transform3D::Translate(coords.x, coords.y + 0.15f, coords.z - 0.75f);

			targetPosition = firstPersonCamPosition;
			targetPosition *= Transform3D::Translate(0.f, 0.f, -2.0f);

			if (!thirdPersonCam)
			{
				camera->Set(firstPersonCamPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
					targetPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
					glm::vec3(0, 1, 0));
			}
		}
	}
	else if (jumping == false && moveLeft == false && moveRight == false) { //situatia in care pot avea loc coliziuni
		glm::mat4 modelMatrix = glm::mat4(1);
		glm::vec3 pos = player.getActualPlayerCoords();
		modelMatrix *= Transform3D::Translate(pos.x, pos.y, pos.z);
		modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
		glm::vec4 coords = modelMatrix * glm::vec4(0, 0, 0, 1);
		//salveaza coordonatele actuale ale jucatorului

		player.setActualCoords(coords);
		if (collideCheck == false) {
			RenderSimpleMesh(meshes["sphere"], shaders["colorShader"], modelMatrix, glm::vec3(0.545, 0.000, 0.545));
		}
		else {
			RenderSimpleMesh(meshes["sphere"], shaders["noiseShader"], modelMatrix, glm::vec3(1.000, 0.000, 1.000)); //magenta
		}

		firstPersonCamPosition = glm::mat4(1);
		firstPersonCamPosition *= Transform3D::Translate(coords.x, coords.y + 0.15f, coords.z - 0.75f);

		targetPosition = firstPersonCamPosition;
		targetPosition *= Transform3D::Translate(0.f, 0.f, -2.0f);

		if (!thirdPersonCam)
		{
			camera->Set(firstPersonCamPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
				targetPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
				glm::vec3(0, 1, 0));
		}
	}
	else if (jumping == true) {
		glm::mat4 modelMatrix = glm::mat4(1);
		glm::vec3 pos = player.getActualPlayerCoords();
		float y = pos.y;
		if (isUpJumping == true) {
			if (y >= 0.35f) {
				y -= 1.5 * delta;
			}
			else {
				isUpJumping = false;
				jumping = false;
			}
		}
		else {
			if (y <= 1) {
				y += 1.5 * delta;
			}
			else {
				isUpJumping = true;
			}
		}
		
		modelMatrix *= Transform3D::Translate(pos.x, y, pos.z);
		modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
		glm::vec4 coords = modelMatrix * glm::vec4(0, 0, 0, 1);
		//salveaza coordonatele actuale ale jucatorului

		player.setActualCoords(coords);

		RenderSimpleMesh(meshes["sphere"], shaders["colorShader"], modelMatrix, glm::vec3(0.545, 0.000, 0.545));

		firstPersonCamPosition = glm::mat4(1);
		firstPersonCamPosition *= Transform3D::Translate(coords.x, coords.y + 0.15f, coords.z - 0.75f);

		targetPosition = firstPersonCamPosition;
		targetPosition *= Transform3D::Translate(0.f, 0.f, -2.0f);

		if (!thirdPersonCam)
		{
			camera->Set(firstPersonCamPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
				targetPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
				glm::vec3(0, 1, 0));
		}
	}
	else if (moveLeft == true) {
		glm::mat4 modelMatrix = glm::mat4(1);
		glm::vec3 pos = player.getActualPlayerCoords();
		float y = pos.y;
		float x = pos.x;
		if (isUpLeft == true) {
			if (y >= 0.35f) {
				y -= 1.5 * delta;
			}
			else {
				isUpLeft = false;
				moveLeft = false;
			}
		}
		else {
			if (y <= 1) {
				y += 1.5 * delta;
			}
			else {
				isUpLeft = true;
			}
		}
		x -= 2.45f * delta;
		modelMatrix *= Transform3D::Translate(x, y, pos.z);
		modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
		glm::vec4 coords = modelMatrix * glm::vec4(0, 0, 0, 1);
		//salveaza coordonatele actuale ale jucatorului

		player.setActualCoords(coords);

		RenderSimpleMesh(meshes["sphere"], shaders["colorShader"], modelMatrix, glm::vec3(0.545, 0.000, 0.545));

		firstPersonCamPosition = glm::mat4(1);
		firstPersonCamPosition *= Transform3D::Translate(coords.x, coords.y + 0.15f, coords.z - 0.75f);

		targetPosition = firstPersonCamPosition;
		targetPosition *= Transform3D::Translate(0.f, 0.f, -2.0f);

		if (!thirdPersonCam) {
			camera->Set(firstPersonCamPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
				targetPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
				glm::vec3(0, 1, 0));
		}
	}

	else if (moveRight == true) {
		glm::mat4 modelMatrix = glm::mat4(1);
		glm::vec3 pos = player.getActualPlayerCoords();
		float y = pos.y;
		float x = pos.x;
		if (isUpRight == true) {
			if (y >= 0.35f) {
				y -= 1.5 * delta;
			}
			else {
				isUpRight = false;
				moveRight = false;
			}
		}
		else {
			if (y <= 1) {
				y += 1.5 * delta;
			}
			else {
				isUpRight = true;
			}
		}
		x += 2.45f * delta;
		modelMatrix *= Transform3D::Translate(x, y, pos.z);
		modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
		glm::vec4 coords = modelMatrix * glm::vec4(0, 0, 0, 1);
		//salveaza coordonatele actuale ale jucatorului

		player.setActualCoords(coords);
		RenderSimpleMesh(meshes["sphere"], shaders["colorShader"], modelMatrix, glm::vec3(0.545, 0.000, 0.545));

		firstPersonCamPosition = glm::mat4(1);
		firstPersonCamPosition *= Transform3D::Translate(coords.x, coords.y + 0.15f, coords.z - 0.75f);
		targetPosition = firstPersonCamPosition;
		targetPosition *= Transform3D::Translate(0.f, 0.f, -2.0f);

		if (!thirdPersonCam)
		{
			camera->Set(firstPersonCamPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
				targetPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
				glm::vec3(0, 1, 0));
		}
	}
}

//inainte de a incepe jocul este redata o platforma de start
void Tema2::LoadStartPlatform() 
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0, 0, -2.0f + translateZ);
	modelMatrix *= Transform3D::Scale(7, 0.25f, 7.0f);
	glm::vec3 position = modelMatrix * glm::vec4(0, 0, 0, 1);
	startPlatformZ = position.z;
	RenderSimpleMesh(meshes["box"], shaders["colorShader"], modelMatrix, glm::vec3(0.690, 0.878, 0.902));
}

// verifica coliziunea jucatorului cu platforma cu indicele index
bool Tema2::checkCollision(int index) {
	bool ok = false;
	glm::vec4 playerCoord = player.getActualPlayerCoords();
	//coordonata pe directia OZ a platformei cu indexul dat
	float i;
	//stabileste coordonatele suprafetei platformei
	float max_step = platforms->getPlatformPos(index) + platforms->getPlatformSize(index);
	float step = platforms->getPlatformPos(index);
	float zCoord;
	float xCoord;
	float yCoord;
	xCoord = platforms->getPlatformXCoord(index);
	yCoord = platforms->getPlatformYCoord(index);
	for (i = step; i < max_step; i += 0.02f) {
		float zCoord = i;
		
		//verifica daca punctul cubului cel mai apropiat de sfera este plasat in interiorul sferei
		float distance = sqrt(pow((xCoord - playerCoord.x), 2) + pow((yCoord - playerCoord.y), 2) + pow((zCoord - playerCoord.z), 2));

		if (distance < 0.5f) { //coliziune
			ok = true;
		}
	}
	return ok;
}

void Tema2::renderFuelInformation(float deltaTimeSeconds) {
	diffFactor += 0.03;
	if (fallingPlayer == false && start == true) {
		fuelValue -= (3.5f + diffFactor) * deltaTimeSeconds;
	}
	glm::vec3 fuelPos = glm::vec3(-3.f, 2.5f, -1.f);
	glm::mat4 modelMatrix = glm::mat4(1);
	
	if (!thirdPersonCam)
	{ //redare pentru modul firstPerson
		modelMatrix *= Transform3D::Translate(fuelPos.x + 2.0f, fuelPos.y, fuelPos.z - 6.5f);
		modelMatrix *= Transform3D::Scale(1.25f * fuelValue / initialFuelValue, 0.25f, 1.25f);
		glm::mat4 modelMatrix1 = glm::mat4(1);
		modelMatrix1 *= Transform3D::Translate(fuelPos.x + 2.0f, fuelPos.y, fuelPos.z - 6.5f);
		modelMatrix1 *= Transform3D::Scale(1.25f, 0.25f, 1.25f);
		RenderMesh(meshes["square"], shaders["VertexColor"], modelMatrix);
		RenderMesh(meshes["black_square"], shaders["VertexColor"], modelMatrix1);
	}
	else //redare pentru modul thirdPerson
	{ 
		glm::mat4 modelMatrix1 = glm::mat4(1);
		modelMatrix1 *= Transform3D::Translate(fuelPos.x, fuelPos.y, fuelPos.z);
		modelMatrix1 *= Transform3D::Scale(1.25f, 0.25f, 1.25f);
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(fuelPos.x, fuelPos.y, fuelPos.z);
		modelMatrix *= Transform3D::Scale(1.25f * fuelValue / initialFuelValue , 0.25f, 1.25f);
		RenderMesh(meshes["square"], shaders["VertexColor"], modelMatrix);
		RenderMesh(meshes["black_square"], shaders["VertexColor"], modelMatrix1);
	}
}

void Tema2::Update(float deltaTimeSeconds)
{	
	LoadStars();
	clock_t endMom = clock() - startMom;
	if (fallingPlayer == true) {
		glClearColor(1.000, 0.855, 0.725, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		LoadStars();
	}
	if (endMom >= 5000) {//5sec
		orangeAbility = false;
		prizeFactor = 0;
	}
	if (start == true && fallingPlayer == false) {
		int i;
		for (i = 0; i < 15; i++) {
			if (orangeAbility == true) {
				prizeFactor = 4;
			}
			platforms->setTranslateVal(platforms->getTranslateVal(i) + 7.5f * deltaTimeSeconds + prizeFactor * deltaTimeSeconds, i);
		}
		if (play == false) { //calculeaza translateZ doar daca platforma de start inca mai este redata in scena
			translateZ += 8.5f * deltaTimeSeconds;
		}
	}

	if (translateZ < 7) {
		LoadStartPlatform();
	}
	else { //daca s-a deplasat suficient cat sa nu mai fie vizibila in scena, platforma de start nu mai este redata
		play = true;
	}

	LoadPlatforms();
	LoadPlayer(deltaTimeSeconds);

	int i;
	bool checkPlayerPos = true;
	for (i = 0; i < 9; i++) {
		if (checkCollision(i) == true) {
			//rosu->final joc
			if (platforms->getPlatformColor(i) == glm::vec3(1, 0, 0)) {
				gameOver = true;
				start = false;
				Engine::GetWindow()->Close();
				cout << "Ai pierdut :(" << endl;
			}
			//acorda abilitati jucatorului pe baza culorii platformelor speciale
			//galben->pierde combustibil
			if (platforms->getPlatformColor(i) == glm::vec3(1, 1, 0)) {
				cout << "Ai pierdut o parte din combustibil :(" << endl;
				fuelValue -= 20;
			}
			//portocaliu->blocat la viteza mare
			if (platforms->getPlatformColor(i) == glm::vec3(1.000, 0.647, 0.000)) {
				orangeAbility = true;
				startMom = clock();
			}
			//verde->recupereaza combustibil
			if (platforms->getPlatformColor(i) == glm::vec3(0.000, 0.502, 0.000)) {
				fuelValue += 20;
				cout << "Ai recuperat o parte din combustibil :)" << endl;
			}
			//jucatorul poate sa ramana de countLives ori fara vieti
			//hotpink->primeste o viata in plus
			if (platforms->getPlatformColor(i) == glm::vec3(1.000, 0.412, 0.706)) {
				lives++;
				cout << "Ai primit o viata in plus :) Numar actual vieti: " << lives << endl;
			}
			//lavander->pierde o viata
			if (platforms->getPlatformColor(i) == glm::vec3(0.902, 0.902, 0.980)) {
				lives--;
				cout << "Ai pierdut o viata :(  Numar actual vieti: " << lives << endl;
			}
			platforms->setPlatformColor(i);
			//schimba culoarea platformei
			checkPlayerPos = false;
			collideCheck = true;
		}
	}

	if (fuelValue <= 0) { //verifica numarul de vieiti la terminarea combustibilului
		
		if (lives == 0) {
			gameOver = true;
			Engine::GetWindow()->Close();
			cout << "Combustibil epuizat :(" << endl;
		}
		else {
			fuelValue = initialFuelValue; //reinitializeaza combustibilul
			lives--;
		}
	}

	renderFuelInformation(deltaTimeSeconds);
	//daca jucatorul nu are coliziune cu nicio platforma iar jocul nu este in nicio situatie speciala
	//(inainte de start joc, saritura la apasarea space, deplasare stanga-dreapta), atunci este 'activata' animatia de cadere a mingii
	if (checkPlayerPos == true) {
		collideCheck = false; //nicio coliziune
		if (jumping == false && moveLeft == false && moveRight == false) {
			if (play == true) {
				fallingPlayer = true;
			}
		}
	}
	// Render the camera target. Useful for understanding where is the rotation point in Third-person camera movement
	if (renderCameraTarget) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
	}
}

void Tema2::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	mesh->Render();
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	// move the camera only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		float cameraSpeed = 2.0f;
		if (window->KeyHold(GLFW_KEY_W)) {
			// TODO : translate the camera forward
			camera->MoveForward(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// TODO : translate the camera to the left
			camera->TranslateRight(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO : translate the camera backwards
			camera->MoveForward(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO : translate the camera to the right
			camera->TranslateRight(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			// TODO : translate the camera down
			camera->TranslateUpword(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// TODO : translate the camera up
			camera->TranslateUpword(cameraSpeed * deltaTime);
		}
	}
	if (window->KeyHold(GLFW_KEY_W)) { //mareste viteza de deplasare a platformelor 
		if (orangeAbility == false) {
			int i;
			for (i = 0; i < 15; i++) {
				platforms->setTranslateVal(platforms->getTranslateVal(i) + 2.75 * deltaTime, i);
			}
		}
	}

	if (window->KeyHold(GLFW_KEY_S)) { //scade viteza de deplasare a platformelor
		if (orangeAbility == false) {
			int i;
			for (i = 0; i < 15; i++) {
				platforms->setTranslateVal(platforms->getTranslateVal(i) - 1.25f * deltaTime, i);
			}
		}
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_T){
		renderCameraTarget = !renderCameraTarget;
	}

	if (key == GLFW_KEY_C){
		if (!thirdPersonCam) {
			camera->Set(thirdPersonCamPosition * glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
		}
		thirdPersonCam = !thirdPersonCam;
	}

	if (key == GLFW_KEY_A) {//deplaseaza jucatorul
		moveLeft = true;
	}

	if (key == GLFW_KEY_D) {
		moveRight = true;
	}

	if (key == GLFW_KEY_SPACE){
		jumping = true;
	}

	if (key == GLFW_KEY_X) {
		start = true;
		startGame = true;
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			renderCameraTarget = false;
			// TODO : rotate the camera in First-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			if (!thirdPersonCam) {
				camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
				camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
			}
		}
		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			renderCameraTarget = true;
			// TODO : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			if (thirdPersonCam) {
				camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
				camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
			}
		}

	}
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}