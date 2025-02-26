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

		Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);
		Mesh* CreateShuriken(float shurikenSide);
		Mesh* CreateArrow(float arrowSide, float lengthFactor);
		Mesh* CreateBow();
		Mesh* CreateBallon(char color, float radius); //culoare = y/r, radius = raza inainte de deformare balon
		Mesh* CreateRectangle(); //dreptunghi folosit pentru a afisa puterea de tragere 
		Mesh* CreateGround();
		Mesh* CreateGigel();
		Mesh* CreateHead();
		Mesh* CreateEye();
		Mesh* CreateDiamond();
		Mesh* createStar();

		void LoadShuriken(float deltaTimeSeconds);
		void LoadBallons(float deltaTimeSeconds);
		void checkBallonCollision(); //intoarce true daca exista coliziune balon-sageata
		void GenerateBallonColor();
		bool checkBallonPos(); //intoarce 'true' daca in fereastra nu mai sunt baloane vizibile
		void checkShurikenColl();
		bool circleCollision(glm::vec2 centre1, glm::vec2 centre2, float radius1, float radius2);
		void checkBowCollision();
		void LoadStars();
		bool checkShurikenPos();
		void LoadDiamonds(float deltaTimeSeconds);
		void checkDiamondColl();
		
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
		glm::mat3 modelMatrix;
		float shurikenSide;
		float arrowSide;
		float bowRadius;
			
		float ballonRadius;
		float radiusArrow = 0;
		float maxBallonRadius; //raza cercului incadrator al balonului considerand si polilinia atasata acestuia
		float arrowLength; //lungimea sagetii
		glm::vec3 arrowTop; //coordonatele varfului sagetii

		float translateY; //deplasarea arcului pe directie verticala
		bool w_pressed = false; //true daca s-a apasat w
		bool s_pressed = false;
		float moveBallon;
		std::vector<float> translateY_values = {}; //valorile translatiilor pe oY ale baloanelor
		
		float angularStep;
		float powerFactor = 1; //cu cat este scalat dreptunghiul powerBar pe oX
		bool mouseClick; //este true atata timp cat este apasat mouse-ul
		float mouseClickTime; //retine timpul cat este apasat mouse-ul
		float maxSpeedMoment; //retine momentul la care s-a atins viteza maxima a sagetii
		bool maxSpeed; //este true daca s-a atins limita maxima a vitezei

		bool throwArrow; //este true la mouseBtnRelease (cand trebuie lansata sageata)
		glm::vec2 mousePos;
		glm::vec2 arrowTranslateMove; //translatiile pe x si y ale sagetii

		float throwAngle; //unghiul aruncarii sagetii = unghiul pe care sageata il face cu axa Ox pe directia aruncarii

		std::vector<glm::vec2> ballonPos = std::vector<glm::vec2>(80); //pozitiile baloanelor actualizate la fiecare nou cadru
		std::vector<unsigned short> ballonColor = std::vector<unsigned short>(80); //culorile date prin 1-rosu, 0-galben, generate aleator
		std::vector<unsigned short> collisionMem = std::vector<unsigned short>(80); //retine -1 pe indicii asociati baloanelor are au avut coliziune cu sageata
		std::vector<float> scaleFactors = std::vector<float>(80); //factoriii de scalare pentru baloane in cazul coliziunilor
		float translateBallonStep;
		float removeX = 0;
		int score; //retine scorul jucatorului

		std::vector<glm::vec2> shurikenPos = std::vector<glm::vec2>(130); //pozitiile pentru shuriken
		std::vector<unsigned short> collisionMemShuriken = std::vector<unsigned short>(130);
		std::vector<unsigned short> collisionMemBow = std::vector<unsigned short>(130); //memoreaza coliziunile arcului cu shuriken-ul
		std::vector<float> scaleFactorsShuriken = std::vector<float>(130); //retine factorii de scalare pentru shurikene

		int livesCount = 3; //numarul de vieti al jucatorului
		float throwTimer; //o data la 4 s este permisa aruncarea sagetii

		glm::vec2 sageata; //coordonatele capatului din stanga al sagetii
		glm::vec2 bowCircleCoord; // coordonatele cercului incadrator al arcului

		std::vector<glm::vec2> diamondsPos = std::vector<glm::vec2>(15);
		std::vector<unsigned short> collisionMemDiamond = std::vector<unsigned short>(15); //retine daca s-a produs coliziune cu diamantele
		
		std::vector<float> diamondTranslateY = std::vector<float>(15); //retine deplasarea pe oy a diamantelor care au avut coliziune cu sageata
};