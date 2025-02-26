#include "Laborator3.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

Laborator3::Laborator3()
{
}

Laborator3::~Laborator3()
{
}

void Laborator3::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	float squareSide = 100;

	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;
	
	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;
	
	// initialize angularStep
	angularStep = 0;
	

	Mesh* square1 = Object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
	AddMeshToList(square1);
	
	Mesh* square2 = Object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0), true);
	AddMeshToList(square2);

	Mesh* square3 = Object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
	AddMeshToList(square3);

	Mesh* soare = Object2D::CreateSquare("soare", corner, squareSide, glm::vec3(1, 1, 0), true);
	AddMeshToList(soare);

	Mesh* terra = Object2D::CreateSquare("terra", corner, squareSide, glm::vec3(0, 0, 1), true);
	AddMeshToList(terra);
}

void Laborator3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator3::Update(float deltaTimeSeconds)
{	
	// TODO: update steps for translation, rotation, scale, in order to create animations
	
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(150, 250);
	translateY = 0;

	if (moveRight == 1) {
		translateX += 100 * deltaTimeSeconds;
		if (translateX > xMax) {
			moveLeft = 1;
			moveRight = 0;
		}
	}
	else if (moveLeft == 1) {
		translateX -= 100 * deltaTimeSeconds;
		if (translateX < xMin) {
			moveLeft = 0;
			moveRight = 1;
		}
	}
	modelMatrix *= Transform2D::Translate(translateX, translateY);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
	
	// TODO: create animations by multiplying current transform matrix with matrices from Transform 2D
	modelMatrix = glm::mat3(1);

	modelMatrix *= Transform2D::Translate(400, 250);
	modelMatrix *= Transform2D::Scale(1.5f, 1.5f);

	angularStep += 0.5 * deltaTimeSeconds;
	modelMatrix *= Transform2D::Translate(50, 50);
	modelMatrix *= Transform2D::Rotate(angularStep);
	modelMatrix *= Transform2D::Translate(-50, -50);
	//square2 se roteste in varful centrului sau
	RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

	//TODO create animations by multiplying current transform matrix with matrices from Transform 2D
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(800, 250);

	if (zoomIn == 1) {
		scaleX += deltaTimeSeconds;
		if (scaleX > 2.5f) {
			zoomIn = 0;
			zoomOut = 1;
		}
	}	
	else if (zoomOut == 1) {
		scaleX -= deltaTimeSeconds;
		if (scaleX < 0.1f) {
			zoomIn = 1;
			zoomOut = 0;
		}
	}
		
	modelMatrix *= Transform2D::Translate(50, 50);
	modelMatrix *= Transform2D::Scale(scaleX, scaleX);
	modelMatrix *= Transform2D::Rotate(angularStep);
	modelMatrix *= Transform2D::Translate(-50, -50);

	RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);

	//adauga soarele si terra
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(300, 500);
	

	modelMatrix *= Transform2D::Translate(50, 50);
	modelMatrix *= Transform2D::Rotate(angularStep);
	modelMatrix *= Transform2D::Translate(-50, -50);

	RenderMesh2D(meshes["soare"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	
	modelMatrix *= Transform2D::Translate(325, 425);
	modelMatrix *= Transform2D::Scale(0.25f, 0.25f);

	modelMatrix *= Transform2D::Translate(50, 500);
	modelMatrix *= Transform2D::Rotate(angularStep);
	modelMatrix *= Transform2D::Translate(-50, -500);

	modelMatrix *= Transform2D::Translate(50, 50);
	modelMatrix *= Transform2D::Rotate(angularStep);
	modelMatrix *= Transform2D::Translate(-50, -50);

	RenderMesh2D(meshes["terra"], shaders["VertexColor"], modelMatrix);
}

void Laborator3::FrameEnd()
{

}

void Laborator3::OnInputUpdate(float deltaTime, int mods)
{
	
}

void Laborator3::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Laborator3::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator3::OnWindowResize(int width, int height)
{
}
