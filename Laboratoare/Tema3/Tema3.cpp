#include "Tema3.h"
#include "Transform3D.h"
#include "Transform2D.h"

#include <vector>
#include <string>
#include <iostream>
#include <time.h> 
#include <Core/Engine.h>

using namespace std;

Tema3::Tema3()
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
	camera = new Tema_3::Camera();
	camera->Set(thirdPersonCamPosition * glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	projectionMatrix = glm::perspective(RADIANS(FoV), window->props.aspectRatio, 0.01f, 200.0f);

	platforms = new Platform();

	//defineste coordonatele la care urmeaza sa fie redate cuburile
	platforms->generatePlatform();
	setTranslatePoints();

	int i;
	for (i = 0; i < 10; i++) {
		translateRightDecorValues[i] = -i * 4.5f - (3 + i);
		translateLeftDecorValues[i] = -i * 2.5f - (2.6f + i);
	}
}

Tema3::~Tema3()
{
	delete camera;
}

Mesh* Tema3::DefineSquare() {
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

Mesh* Tema3::DefineBlackSquare() {
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

void Tema3::LoadMeshes() {
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

	{
		vector<VertexFormat> vertices_tetraedru
		{
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.125, 0.698, 0.667)),
			VertexFormat(glm::vec3(0.5f, 0, 0.86f), glm::vec3(0.275, 0.510, 0.706)),
			VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0.498, 1.000, 0.831)),
			VertexFormat(glm::vec3(0.5f, 1, 0.28f), glm::vec3(0.000, 0.000, 1.000))
		};

		vector<unsigned short> indices_tetraedru
		{
			0, 1, 3,
			1, 2, 3,
			0, 2, 1,
			0, 3, 2
		};
		Mesh* tetraedru = new Mesh("tetraedru");
		tetraedru->InitFromData(vertices_tetraedru, indices_tetraedru);
		meshes[tetraedru->GetMeshID()] = tetraedru;
	}

	Mesh* cube = CreateStylisedCube();
	meshes[cube->GetMeshID()] = cube;

	Mesh* cilindru = CreateCylinder();
	meshes[cilindru->GetMeshID()] = cilindru;
}

Mesh* Tema3::CreateStylisedCube() {

	std::vector<glm::vec3> vertices =
	{
		glm::vec3(0, 0, 1),
		glm::vec3(1, 0, 1),
		glm::vec3(0, 1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(1, 1, 0),

		glm::vec3(0, 0, 1),
		glm::vec3(1, 0, 1),
		glm::vec3(0, 1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(1, 1, 0),

		glm::vec3(0, 0, 1),
		glm::vec3(1, 0, 1),
		glm::vec3(0, 1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(1, 1, 0),
	};

	std::vector<unsigned short> indices = {
			0, 1, 2,
			1, 3, 2,

			10, 11, 15,
			10, 15, 14,

			17, 23, 19,
			17, 21, 23,

			6, 7, 4,
			7, 5, 4,

			8, 12, 9,
			9, 12, 13,

			18, 22, 20,
			16, 18, 20
	};

	std::vector<glm::vec3> normals
	{
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),

		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),

		glm::vec3(0, -1, 0),//8
		glm::vec3(0, -1, 0),//9
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),

		glm::vec3(0, -1, 0),//12
		glm::vec3(0, -1, 0),//13
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),

		glm::vec3(-1, 0, 0),//16
		glm::vec3(1, 0, 0),//17
		glm::vec3(-1, 0, 0),//18
		glm::vec3(1, 0, 0),//19

		glm::vec3(-1, 0, 0),//20
		glm::vec3(1, 0, 0),//21
		glm::vec3(-1, 0, 0),//22
		glm::vec3(1, 0, 0)//23
	};

	std::vector<glm::vec2> textureCoords
	{
		glm::vec2(0.f, 0.f),
		glm::vec2(1.f, 0.f),
		glm::vec2(0.f, 1.f),
		glm::vec2(1.f, 1.f),

		glm::vec2(0.f, 0.f),
		glm::vec2(1.f, 0.f),
		glm::vec2(0.f, 1.f),
		glm::vec2(1.f, 1.f),

		//8, 9, 12, 13 - fata de jos a cubului
		//10, 11, 14, 15 - fata de sus a cubului
		glm::vec2(0.f, 0.f),//8
		glm::vec2(1.f, 0.f),//9
		glm::vec2(0.f, 0.f),//10
		glm::vec2(1.f, 0.f),//11

		glm::vec2(0.f, 1.f),//12
		glm::vec2(1.f, 1.f),//13
		glm::vec2(0.f, 1.f),//14
		glm::vec2(1.f, 1.f),//15

		//fetele laterale ale cubului
		//fata laterala din stanga - 16, 20, 18, 22
		//fata laterala din dreapta - 17, 19, 21, 23
		glm::vec2(0.f, 0.f), //16
		glm::vec2(0.f, 0.f),
		glm::vec2(0.f, 1.f), //18
		glm::vec2(0.f, 1.f),

		glm::vec2(1.f, 0.f),//20
		glm::vec2(1.f, 0.f),
		glm::vec2(1.f, 1.f),//22
		glm::vec2(1.f, 1.f)
	};

	Mesh* cube = new Mesh("cub_stilizat");
	cube->InitFromData(vertices, normals, textureCoords, indices);
	return cube;
}

std::vector<float> generateCircleVertices() {

	std::vector<float> vertices;
	float angle;
	float step = 2 * 3.1415926f / 200;
	for (int i = 0; i < 200; i++) {
		angle = i * step;
		//coordonata x
		vertices.push_back(cos(angle)); 
		//coordoanata y
		vertices.push_back(sin(angle)); 
		//coordonata z
		vertices.push_back(0);                
	}
	return vertices;
}

Mesh* Tema3::CreateCylinder() {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned short> indices;

	Mesh* cylinder = new Mesh("cilindru");
	//cylinder->InitFromData(positions, normals, indices, textures);
	return cylinder;
}

void Tema3::LoadShaders() {
	//adauga un shader pentru aplicarea culorii in vectorul shaders
	{
		Shader* shader = new Shader("textureShader");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders["textureShader"] = shader;
	}
	{
		Shader* shader = new Shader("basicShader");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/BasicVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/BasicFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders["basicShader"] = shader;
	}
	{
		//adauga un shader cu aplicarea unei functii de zgomot
		Shader* shader1 = new Shader("noiseShader");
		shader1->AddShader("Source/Laboratoare/Tema3/Shaders/NoiseShaderVS.glsl", GL_VERTEX_SHADER);
		shader1->AddShader("Source/Laboratoare/Tema3/Shaders/NoiseShaderFS.glsl", GL_FRAGMENT_SHADER);
		shader1->CreateAndLink();
		shaders["noiseShader"] = shader1;
	}
	{
		Shader* shader = new Shader("waterShader");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/WaterVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/WaterFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders["waterShader"] = shader;
	}
}

void Tema3::LoadTextures() {
	//load textures
	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/grass1.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["grass"] = texture;
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/scoarta_copac.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["copac"] = texture;
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/piatra1.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["piatra"] = texture;
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/pietre3.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["piatra2"] = texture;
	}
}

void Tema3::Init()
{
	LoadMeshes();
	LoadShaders();
	LoadTextures();

	player.setYCoord(0.80f);
}


Mesh* Tema3::CreateStar() {
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

Mesh* Tema3::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
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

//render with textures
void Tema3::RenderMeshTex(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
	glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
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

void Tema3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.400, 0.804, 0.667, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::setTranslatePoints()
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
		platforms->setTranslatePoint(i, -5.5f - maxCoord - 1 * 2 - platforms->getPlatformSize(i) / 2 - maxCoord1);
	}
}

void Tema3::LoadStars() {
	int i;
	for (i = 0; i < lives; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(3 + i, 3.5f, -7.5f);
		modelMatrix *= Transform3D::Scale(0.1, 0.1, 0.1);
		RenderMesh(meshes["star"], shaders["VertexColor"], modelMatrix);
	}
}

//cuburile nou generate vor fi mereu afisate la aceeasi distanta de jucator pe axa OZ
void Tema3::LoadPlatforms() {
	int i;
	bool ok1 = false;
	int count = 0;

	for (i = 0; i < 3; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		if (i != 1) {
			modelMatrix *= Transform3D::Translate(2.85f * (i - 1), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
			modelMatrix *= Transform3D::Scale(2.85f, 0.25f, platforms->getPlatformSize(i));
		}
		else {
			modelMatrix *= Transform3D::Translate(2.5f * (i - 1), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
			modelMatrix *= Transform3D::Scale(1.40f, 0.25f, platforms->getPlatformSize(i));
		}
		
		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		float zCoord = position.z;
		platforms->setPlatformXCoord(position.x, i);
		platforms->setPlatformYCoord(position.y, i);
		platforms->setPlatformZCoord(zCoord - platforms->getPlatformSize(i) / 2, i);

		//platformele care dispar in spatele camerei nu mai sunt redate
		if (platforms->getPlatformPos(i) <= 2.5f) {
			RenderMeshTex(meshes["box"], shaders["textureShader"], modelMatrix, mapTextures["piatra2"]);
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
		if (i != 4) {
			modelMatrix *= Transform3D::Translate(2.85f * (i - 4), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
			modelMatrix *= Transform3D::Scale(2.85f, 0.25f, platforms->getPlatformSize(i));
		}
		else {
			modelMatrix *= Transform3D::Translate(2.5f * (i - 4), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
			modelMatrix *= Transform3D::Scale(1.40f, 0.25f, platforms->getPlatformSize(i));
		}
		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		float zCoord = position.z;
		platforms->setPlatformXCoord(position.x, i);
		platforms->setPlatformYCoord(position.y, i);
		platforms->setPlatformZCoord(zCoord - platforms->getPlatformSize(i) / 2, i);
		RenderMeshTex(meshes["box"], shaders["textureShader"], modelMatrix, mapTextures["piatra2"]);
	}
	for (i = 6; i < 9; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		if (i != 7) {
			modelMatrix *= Transform3D::Translate(2.85f * (i - 7), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
			modelMatrix *= Transform3D::Scale(2.85f, 0.25f, platforms->getPlatformSize(i));
		}
		else {
			modelMatrix *= Transform3D::Translate(2.5f * (i - 7), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
			modelMatrix *= Transform3D::Scale(1.40f, 0.25f, platforms->getPlatformSize(i));
		}
		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		float zCoord = position.z;
		platforms->setPlatformXCoord(position.x, i);
		platforms->setPlatformYCoord(position.y, i);
		platforms->setPlatformZCoord(zCoord - platforms->getPlatformSize(i) / 2, i);
		RenderMeshTex(meshes["box"], shaders["textureShader"], modelMatrix, mapTextures["piatra2"]);
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
void Tema3::LoadPlayer(float delta) {
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
			RenderSimpleMesh(meshes["sphere"], shaders["basicShader"], modelMatrix, glm::vec3(0.545, 0.000, 0.545));

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
			RenderSimpleMesh(meshes["sphere"], shaders["basicShader"], modelMatrix, glm::vec3(0.545, 0.000, 0.545));
		}
		else {
			RenderSimpleMesh(meshes["sphere"], shaders["basicShader"], modelMatrix, glm::vec3(0.824, 0.412, 0.118)); 
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
			if (y >= 0.8f) {
				y -= 1.5 * delta;
			}
			else {
				isUpJumping = false;
				jumping = false;
			}
		}
		else {
			if (y <= 1.5f) {
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

		RenderSimpleMesh(meshes["sphere"], shaders["basicShader"], modelMatrix, glm::vec3(0.545, 0.000, 0.545));

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
			if (y >= 0.8f) {
				y -= 1.5 * delta;
			}
			else {
				isUpLeft = false;
				moveLeft = false;
			}
		}
		else {
			if (y <= 1.5) {
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
	
		RenderSimpleMesh(meshes["sphere"], shaders["basicShader"], modelMatrix, glm::vec3(0.545, 0.000, 0.545));

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
			if (y >= 0.80) {
				y -= 1.5 * delta;
			}
			else {
				isUpRight = false;
				moveRight = false;
			}
		}
		else {
			if (y <= 1.5f) {
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
		RenderSimpleMesh(meshes["sphere"], shaders["basicShader"], modelMatrix, glm::vec3(0.545, 0.000, 0.545));

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

void Tema3::LoadDecorElements(float deltaTimeSeconds) {
	//recalculeaza valorile pentru translatii
	int i;
	for (i = 0; i < 10; i++) {
		if (fallingPlayer == false && start == true) {
			translateRightDecorValues[i] += 4.5f * deltaTimeSeconds;
		}

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(3.95f, 0, -5 + translateRightDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.5, 1.5f, 0.5);

		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		if (position.z < 2.5f) {
			RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["copac"]);
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(3.75f, 1.5f, -5.25f + translateRightDecorValues[i]);
			modelMatrix *= Transform3D::Scale(1, 1, 1);
			RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["grass"]);
		}
		else {
			translateRightDecorValues[i] = -10;
		}
	}

	for (i = 0; i < 10; i++) {
		if (fallingPlayer == false && start == true) {
			translateLeftDecorValues[i] += 4.5f * deltaTimeSeconds;
		}

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(-4.45f, 0, -5 + translateLeftDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.5, 1.5f, 0.5);

		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		if (position.z < 2.5f) {
			RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["copac"]);
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(-4.65f, 1.5f, -5.25f + translateLeftDecorValues[i]);
			modelMatrix *= Transform3D::Scale(1, 1, 1);
			RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["grass"]);
		}
		else {
			translateLeftDecorValues[i] = -10;
		}
	}
}


//inainte de a incepe jocul este redata o platforma de start
void Tema3::LoadStartPlatform() 
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0, 0, -2.0f + translateZ);
	modelMatrix *= Transform3D::Scale(8.5f, 0.25f, 7.0f);
	glm::vec3 position = modelMatrix * glm::vec4(0, 0, 0, 1);
	startPlatformZ = position.z;
	RenderMeshTex(meshes["box"], shaders["textureShader"], modelMatrix, mapTextures["grass"]);
}

// verifica coliziunea jucatorului cu platforma cu indicele index
bool Tema3::checkCollision(int index) {
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

	
	for (i = step; i < max_step; i += 0.01f) {
		float zCoord = i;
		//verifica daca punctul cubului cel mai apropiat de sfera este plasat in interiorul sferei
		xCoord = platforms->getPlatformXCoord(index);
		yCoord = platforms->getPlatformYCoord(index);
		float distance = sqrt(pow((xCoord - playerCoord.x), 2) + pow((yCoord - playerCoord.y), 2) + pow((zCoord - playerCoord.z), 2));

		if (distance <= 1) { //coliziune
			ok = true;
		}
	}
	return ok;
}

void Tema3::renderFuelInformation(float deltaTimeSeconds) {
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

void Tema3::Update(float deltaTimeSeconds)
{	
	LoadStars();
	LoadDecorElements(deltaTimeSeconds);
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-2, 2, -4);
	RenderMeshTex(meshes["cilindru"], shaders["textureShader"], modelMatrix, mapTextures["grass"]);

	if (fallingPlayer == true) {
		glClearColor(1.000, 0.855, 0.725, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		LoadDecorElements(deltaTimeSeconds);
	}
	
	if (start == true && fallingPlayer == false) {
		int i;
		for (i = 0; i < 9; i++) {
			platforms->setTranslateVal(platforms->getTranslateVal(i) + 4.5f * deltaTimeSeconds, i);
		}
		if (play == false) { //calculeaza translateZ doar daca platforma de start inca mai este redata in scena
			translateZ += 4.5f * deltaTimeSeconds;
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
				//cout << "Nicio coliziune detectata" << endl;
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

void Tema3::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema3::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
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


void Tema3::OnInputUpdate(float deltaTime, int mods)
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
			for (i = 0; i < 9; i++) {
				platforms->setTranslateVal(platforms->getTranslateVal(i) + 1.75 * deltaTime, i);
			}
		}
	}

	if (window->KeyHold(GLFW_KEY_S)) { //scade viteza de deplasare a platformelor
		if (orangeAbility == false) {
			int i;
			for (i = 0; i < 9; i++) {
				platforms->setTranslateVal(platforms->getTranslateVal(i) - 1.25f * deltaTime, i);
			}
		}
	}
}

void Tema3::OnKeyPress(int key, int mods)
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

void Tema3::OnKeyRelease(int key, int mods)
{
	
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
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

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema3::OnWindowResize(int width, int height)
{
}