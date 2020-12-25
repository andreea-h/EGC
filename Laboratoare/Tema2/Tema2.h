#pragma once
#include <Component/SimpleScene.h>
#include "Camera.h"
#include "Platform.h"
#include "Player.h"
#include <time.h>

class Tema2 : public SimpleScene
{
public:
	Tema2();
	~Tema2();

	void Init() override;

private:
	void LoadPlatforms(); //incarca in scena platformele
	void LoadPlayer(float delta);
	void setTranslatePoints();
	void LoadStartPlatform();
	bool checkCollision(int index);
	void renderFuelInformation(float deltaTimeSeconds);
	Mesh* DefineSquare();
	Mesh* DefineBlackSquare();
	Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);
	Mesh* Tema2::CreateStar();
	void LoadStars();


	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);

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
	float FoV;
	glm::vec3 lightPosition;
	unsigned int materialShininess;
	float materialKd;
	float materialKs;


	Platform* platforms; //obiectul care reprezinta platformele pe care merge jucatorul
	Player player;

	float translateZ;
	float startPlatformZ; //coordonata z a platformei de start 

	glm::mat4 thirdPersonCamPosition; //pozitiile camerelor third si first person
	glm::mat4 firstPersonCamPosition;

	glm::mat4 targetPosition;

	bool thirdPersonCam; //isi schimba valoarea la apasarea tastei C (se doreste modul firstPersonCamera)
	bool start;
	bool play; //este true daca platforma de start nu este mai redata in scena

	bool startGame = false; //devine true cand incepe jocul (la apasarea tastei x)
	bool jumping = false; //devine true la apasarea tastei Space
	bool down = false;

	bool isUpJumping = false; //devine true daca mingea a atins nivelul maxim in cadrul unei sarituri
	bool isUpLeft = false; //devine true daca mingea a atins nivelul maxim de inaltime la deplasarea pe coloana stanga
	bool isUpRight = false; //devine true daca mingea a atins nivelul maxim de inaltime la deplasarea pe coloana dreapta

	bool moveLeft = false; //jucatorul se deplaseaza pe coloana stanga
	bool moveRight = false; //jucatorul se deplaseaza pe coloana dreapta
	bool fallingPlayer = false; //jucatorul se afla in cadere de pe o platforma

	bool stopGame = false; //devine true atunci cand jucatorul a cazut pe de platforme si va disparea din scena, urmand ca jocul sa se incheie

	float initialFuelValue = 250.0f; //cantitate initiala de combustibil (si maxima)
	float fuelValue = initialFuelValue; //cantitatea curenta de combustibil de care dispune 

	bool collideCheck = false; //este true daca s-a produs coliziune jucator-platforma

	bool gameOver = false; //devine true la coliziunea cu o platforma rosie
	float prizeFactor; //factorul aditional de translatie adaugat la aterizarea pe o platforma portocalie
	float finalTime; //momentul la care trebuie sa inceteze efectul dat de platforma portocalie
	bool orangeAbility; //este true daca este activata abilitatea data de  platforma orange
	clock_t startMom; //momentul de start al aplicatii abilitatii
	float diffFactor; //combustibilul se termina mai repede o data cu cresterea dificulatii jocului
	int lives = 3; //numar initial de vieti
};