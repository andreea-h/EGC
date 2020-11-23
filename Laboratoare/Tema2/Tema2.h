#pragma once
#include <Component/SimpleScene.h>
#include "Camera.h"
#include "Platform.h"

class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;

	private:
		void LoadPlatforms(); //incarca in scena platformele
		
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		Tema_2::Camera* camera;
		glm::mat4 projectionMatrix;
		bool renderCameraTarget;

	private:
		Platform* platforms; //obiectul care reprezinta platformele pe care merge jucatorul

		float translateZ;
		glm::mat4 thirdPersonCamPosition;
		glm::mat4 firstPersonCamPosition;

		bool thirdPersonCam; //devine false daca se atasa tasta C (se doreste modul firstPersonCamera)
};