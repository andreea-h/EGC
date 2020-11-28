#include "Tema2.h"
#include "Transform3D.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Tema2::Tema2()
{
	FoV = 90.0f;
	thirdPersonCamPosition = glm::mat4(1);
	thirdPersonCamPosition = Transform3D::Translate(0, 1.75f, 2.5f);

	platforms = new Platform();
	platforms->generatePlatform();
}

Tema2::~Tema2()
{
	delete camera;
}


void Tema2::Init()
{
	renderCameraTarget = false;

	camera = new Tema_2::Camera();
	
	camera->Set(thirdPersonCamPosition * glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

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

	projectionMatrix = glm::perspective(RADIANS(FoV), window->props.aspectRatio, 0.01f, 200.0f);

	//adauga un shader in vectorul shaders
	Shader* shader = new Shader("colorShader");
	shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders["colorShader"] = shader;
}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

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

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}


void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	
	glClearColor(1.000, 0.855, 0.725, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::LoadPlatforms() {
	int i;
	for (i = 0; i < 3; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Scale(1.5f, 0.25f, platforms->getPlatformSize(i));
		modelMatrix *= Transform3D::Translate(1.5f * (i - 1), 0, -1 + translateZ);

		RenderSimpleMesh(meshes["box"], shaders["colorShader"], modelMatrix, platforms->getPlatformColor(i));
	}

	for (i = 0; i < 3; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Scale(1.5f, 0.25f, platforms->getPlatformSize(i));
		modelMatrix *= Transform3D::Translate(1.5f * (i - 1), 0, -3 + translateZ);

		RenderSimpleMesh(meshes["box"], shaders["colorShader"], modelMatrix, platforms->getPlatformColor(i));
	}
	for (i = 0; i < 3; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Scale(1.5f, 0.25f, platforms->getPlatformSize(i));
		modelMatrix *= Transform3D::Translate(1.5f * (i - 1), 0, -5 + translateZ);

		RenderSimpleMesh(meshes["box"], shaders["colorShader"], modelMatrix, platforms->getPlatformColor(i));
	}
}

void Tema2::Update(float deltaTimeSeconds)
{
	translateZ += 0.25 * deltaTimeSeconds; //deplasare pe Z a platformelor

	LoadPlatforms();

	//jucatorul
	glm::mat4 modelMatrix = glm::mat4(1);
	glm::vec3 pos = player.getPlayerCoords();
	modelMatrix *= Transform3D::Translate(pos.x, pos.y, pos.z);
	modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
	RenderSimpleMesh(meshes["sphere"], shaders["colorShader"], modelMatrix, glm::vec3(0.482, 0.408, 0.933));

	// Render the camera target. Useful for understanding where is the rotation point in Third-person camera movement
	if (renderCameraTarget)
	{
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
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
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
}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_T)
	{
		renderCameraTarget = !renderCameraTarget;
	}

	if (key == GLFW_KEY_C)
	{
		if (!thirdPersonCam)
		{
			camera->Set(thirdPersonCamPosition * glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
		}
		if (thirdPersonCam == true) {
			thirdPersonCam = false;
		}
		else {
			thirdPersonCam = true;
		}
		
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
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
			camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
			camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
		}

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			renderCameraTarget = true;
			// TODO : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
			camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
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