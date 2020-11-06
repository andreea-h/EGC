#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include <vector>
#include <ctime>

class Tema1 : public SimpleScene {
	public:
		Tema1();
		~Tema1();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		Mesh* CreateShuriken(float shurikenSide);
		Mesh* CreateArrow(float arrowSide, float lengthFactor);
		Mesh* CreateBow();
		Mesh* CreateBallon(char color, float radius); //culoare = y/r, radius = raza inainte de deformare balon
		Mesh* CreateRectangle(); //dreptunghi folosit pentru a afisa puterea de tragere 
		void LoadBallons();
		void LoadShuriken();


		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;


	protected:
		glm::mat3 modelMatrixShuriken;
		glm::mat3 modelMatrixArrow;
		glm::mat3 modelMatrixBow;
		glm::mat3 modelMatrixBallonY;
		glm::mat3 modelMatrixBallonR;
		glm::mat3 modelMatrixPower;
		float shurikenSide;
		float arrowSide;
			
		float radiusArrow = 0;
		float arrowLength; //lungimea sagetii
		glm::vec3 arrowTop; //coordonatele varfului sagetii

		float translateY; //deplasarea arcului pe directie verticala
		bool w_pressed = false; //true daca s-a apasat
		bool s_pressed = false;
		float moveBallon;
		std::vector<float> translateY_values = {}; //valorile translatiilor pe oY ale baloanelor
		
		float angularStep;
		float powerFactor = 1; //cu cat este scalat dreptunghiul powerBar pe oX
		bool mouseClick; //este true atata timp cat este apasat mouse-ul
		float mouseClickTime; //retine timpul cat este apasat mouse-ul
		bool throwArrow; //este true la mouseBtnRelease (cand trebuie lansata sageata)
		glm::vec2 mousePos;
		glm::vec2 arrowTranslateMove; //translatiile pe x si y ale sagetii

		float throwAngle; //unghiul 
};