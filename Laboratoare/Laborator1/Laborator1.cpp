#include "Laborator1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

	bool f_key_pressed = false; //pentru schimbarea culorii de stergere a ecranului
	bool p_key_pressed = false; //pentru a schimba un obiect afisat
	int counter = 0; //contorizeaza numarul de apasari al tastei P
	

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

Laborator1::Laborator1()
{
}

Laborator1::~Laborator1()
{
}

void Laborator1::Init()
{
	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;

		Mesh* sphere = new Mesh("sphere");
		sphere->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[sphere->GetMeshID()] = sphere;

		Mesh* teapot = new Mesh("teapot");
		teapot->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
		meshes[teapot->GetMeshID()] = teapot;

		Mesh* bamboo = new Mesh("bamboo");
		bamboo->LoadMesh(RESOURCE_PATH::MODELS + "Vegetation/Bamboo", "bamboo.obj");
		meshes[bamboo->GetMeshID()] = bamboo;
	}
}

void Laborator1::FrameStart()
{

}

void Laborator1::Update(float deltaTimeSeconds) //aici se dau comenzile de desenare
{
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer
	if (f_key_pressed == true) { 
		//la apasarea tastei F se schimba culoarea cu una generata aleator la fiecare apasare
		glClearColor(bgR, bgG, bgB, 1);
	}
	else {
		glClearColor(0, 0, 0, 1); //r_g_b_opacitate
	}
	
	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw 
	// seteaza dimensiunile spatiului de desenare
	glViewport(0, 0, resolution.x, resolution.y); //(se deseneaza pe toata fereastra)

	//se dau comenzile de desenare (cu pozitie si factor de scalare)
	// render the object
	if (p_key_pressed == false || counter % 3 == 0) {
		RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));
	}
	else {
		if (counter % 3 == 1) {
			RenderMesh(meshes["teapot"], glm::vec3(1, 0.5f, 0), glm::vec3(1.5f));
		}
		else if (counter % 3 == 2) {
			RenderMesh(meshes["sphere"], glm::vec3(1, 0.5f, 0), glm::vec3(1.5f));
		}
	}
	
	// render the object again but with different properties
	RenderMesh(meshes["box"], /*glm::vec3(-1, 0.5f, 0)*/ jumpingBoxPos); //factor de scalare implicit 1

	RenderMesh(meshes["sphere"], glm::vec3(-2, 2, 0), glm::vec3(1.5f));

	RenderMesh(meshes["bamboo"], glm::vec3(2, 1, 1), glm::vec3(0.1f));

	//obiectul care se deplaseaza la apasarea tastelor W, A, S, D, E, Q
	RenderMesh(meshes["sphere"], objPos, glm::vec3(0.75f));

	
}

void Laborator1::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator1::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input
	if (window->KeyHold(GLFW_KEY_W)) { //inainte
		objPos += glm::vec3(1, 0, 0) * deltaTime;
		//deplasamentul trebuie sa fie proportional cu timpul scurs intre frame-uri (deltaTime)
	}
	if (window->KeyHold(GLFW_KEY_S)) { //inapoi
		objPos -= glm::vec3(1, 0, 0) * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_A)) { //stanga
		objPos += glm::vec3(0, 1, 0) * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_D)) { //dreapta
		objPos -= glm::vec3(0, 1, 0) * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_E)) { //sus
		objPos += glm::vec3(0, 0, 1) * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_Q)) { //jos
		objPos -= glm::vec3(0, 0, 1) * deltaTime;
	}
};

void Laborator1::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_F) {
		// do something
		f_key_pressed = true;
		bgR = (rand() % 11) / 10.f;
		bgG = (rand() % 11) / 11.f;
		bgB = (rand() % 11) / 12.f;
	}
	if (key == GLFW_KEY_P) {
		p_key_pressed = true;
		counter++;
	}
	//apasarea tastei Space face ca unul din cuburi sa topaie
	if (key == GLFW_KEY_SPACE) { 
		jumpingBoxPos.y += 1.5f;
	}
};

void Laborator1::OnKeyRelease(int key, int mods)
{
	// add key release event
	if (key == GLFW_KEY_SPACE) { 
		jumpingBoxPos.y = 0;
	}
};

void Laborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Laborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Laborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator1::OnWindowResize(int width, int height)
{
	// treat window resize event
}
