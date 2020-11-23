#include "Laborator4.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"

using namespace std;

Laborator4::Laborator4()
{
}

Laborator4::~Laborator4()
{
}

void Laborator4::Init()
{
	polygonMode = GL_FILL;

	Mesh* mesh = new Mesh("box");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh->GetMeshID()] = mesh;

	// initialize tx, ty and tz (the translation steps)
	translateX = 0;
	translateY = 0;
	translateZ = 0;

	// initialize sx, sy and sz (the scale factors)
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;
	
	// initialize angularSteps
	angularStepOX = 0;
	angularStepOY = 0;
	angularStepOZ = 0;
}

void Laborator4::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator4::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-2.5f, 0.5f,-1.5f);
	modelMatrix *= Transform3D::Translate(translate.x, translate.y, translate.z);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, 0.5f, -1.5f);
	modelMatrix *= Transform3D::Scale(scaleX, scaleY, scaleZ);
	RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(2.5f, 0.5f, -1.5f);
	modelMatrix *= Transform3D::RotateOX(angularStepOX);
	modelMatrix *= Transform3D::RotateOY(angularStepOY);
	modelMatrix *= Transform3D::RotateOZ(angularStepOZ);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
	
	angle += deltaTimeSeconds;
	dist += deltaTimeSeconds / 3;
	//soare
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(8, 8, 0);
	modelMatrix *= Transform3D::Translate(dist, dist, 0);
	modelMatrix *= Transform3D::RotateOY(angle);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	//pamant
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(8, 8, 0);
	modelMatrix *= Transform3D::Translate(dist, dist, 0);
	modelMatrix *= Transform3D::RotateOY(angle);
	modelMatrix *= Transform3D::Translate(-8, -8, 0);
	modelMatrix *= Transform3D::Translate(14, 8, 0);
	modelMatrix *= Transform3D::Scale(0.75f, 0.75f, 0.75f);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	//satelit
	modelMatrix *= Transform3D::RotateOY(angle);
	modelMatrix *= Transform3D::Translate(4, 0, 0);
	
	modelMatrix *= Transform3D::Scale(0.45f, 0.45f, 0.45f);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}

void Laborator4::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator4::OnInputUpdate(float deltaTime, int mods)
{
	float speed = 1.5f;
	float move_offset = deltaTime * speed;

	glm::vec3 offset = glm::vec3(0);

	if (window->KeyHold(GLFW_KEY_A)) {
		offset -= glm::vec3(1, 0, 0);
	}
	if (window->KeyHold(GLFW_KEY_D)) {
		offset += glm::vec3(1, 0, 0);
	}
	if (window->KeyHold(GLFW_KEY_W)) {
		offset -= glm::vec3(0, 0, 1);
	}
	if (window->KeyHold(GLFW_KEY_S)) {
		offset += glm::vec3(0, 0, 1);
	}
	if (window->KeyHold(GLFW_KEY_R)) {
		offset += glm::vec3(0, 1, 0);
	}
	if (window->KeyHold(GLFW_KEY_F)) {
		offset -= glm::vec3(0, 1, 0);
	}

	if (glm::length(offset) > 0.f) {
		offset = move_offset * glm::normalize(offset);
	}

	translate += offset;

	if (window->KeyHold(GLFW_KEY_1)) {
		scaleX += move_offset;
		scaleY += move_offset;
		scaleZ += move_offset;
	}
	if (window->KeyHold(GLFW_KEY_2)) {
		scaleX -= move_offset;
		scaleY -= move_offset;
		scaleZ -= move_offset;
	}
	
	if (window->KeyHold(GLFW_KEY_3)) {
		angularStepOX += move_offset;
	}
	if (window->KeyHold(GLFW_KEY_4)) {
		angularStepOX -= move_offset;
	}

	if (window->KeyHold(GLFW_KEY_5)) {
		angularStepOY += move_offset;
	}
	if (window->KeyHold(GLFW_KEY_6)) {
		angularStepOY -= move_offset;
	}

	if (window->KeyHold(GLFW_KEY_7)) {
		angularStepOZ += move_offset;
	}
	if (window->KeyHold(GLFW_KEY_8)) {
		angularStepOZ -= move_offset;
	}
}

void Laborator4::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_SPACE)
	{
		switch (polygonMode)
		{
		case GL_POINT:
			polygonMode = GL_FILL;
			break;
		case GL_LINE:
			polygonMode = GL_POINT;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}
}

void Laborator4::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator4::OnWindowResize(int width, int height)
{
}
