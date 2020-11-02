#include "Tema1.h"
#include "Transform2D.h"

#include <vector>
#include <iostream>
#include <Core/Engine.h>

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

	Mesh* shuriken = CreateShuriken();
	AddMeshToList(shuriken);
}


Mesh* Tema1::CreateShuriken() {

	glm::vec3 color = glm::vec3(1.000, 0.078, 0.576);
	vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(-50, 50, 0), color),
		VertexFormat(glm::vec3(0, 50, 0), color),
		VertexFormat(glm::vec3(50, 50, 0), color),
		VertexFormat(glm::vec3(50, 0, 0), color),
		VertexFormat(glm::vec3(50, -50, 0), color),
		VertexFormat(glm::vec3(0, -50, 0), color),
		VertexFormat(glm::vec3(-50, -50, 0), color),
		VertexFormat(glm::vec3(-50, 0, 0), color),
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

void Tema1::Update(float deltaTimeSeconds) {
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(150, 250);
	RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);
}

void Tema1::FrameStart() {
	
	glClearColor(0.196, 0.804, 0.196, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::FrameEnd() {
	
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}

void Tema1::OnKeyPress(int key, int mods)
{
}

void Tema1::OnKeyRelease(int key, int mods)
{
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}