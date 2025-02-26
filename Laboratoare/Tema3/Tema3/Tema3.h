#pragma once
#include <Component/SimpleScene.h>
#include "Camera.h"
#include <time.h>
#include <Laboratoare/Tema2/Platform.h>
#include <Laboratoare/Tema2/Player.h>

class Tema3 : public SimpleScene
{
public:
	Tema3();
	~Tema3();

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
	Mesh* Tema3::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);
	Mesh* Tema3::CreateStar();
	void LoadStars();
	void LoadTextures();
	void LoadMeshes();
	void LoadShaders();
	Mesh* CreateStylisedCube();
	Mesh* CreateCylinder();
	Mesh* CreateDiamond();
	void LoadDecorElements(float deltaTimeSeconds);
	void LoadLamps(float deltaTimeSeconds);
	void LoadCollectionItem(float time);
	void LoadObstacles(float time);
	Mesh* CreatePyramid();
	bool CheckObstacleCollision(int index);
	bool CheckCollectionItemCollision(int index);
	void CheckCollectionItems();
	void LoadScoreInfo(); 


	void CheckObstacleCollisions();

	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);
	void RenderMeshTex(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	Tema_3::Camera* camera;
	glm::mat4 projectionMatrix;
	bool renderCameraTarget;

private:
	float FoV;
	glm::vec3 lightPosition;
	glm::vec3 lightDirection;

	unsigned int materialShininess;
	float materialKd;
	float materialKs;
	float cutOff;


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
	int lives = 5; //numar initial de vieti

	std::unordered_map<std::string, Texture2D*> mapTextures;

	float translateDecor;
	//memoreaza tranzitiile pentru trunchiurile de copaci de pe stanga si de pe dreapta
	std::vector<float> translateRightDecorValues = std::vector<float>(7);
	std::vector<float> translateLeftDecorValues = std::vector<float>(7);

	//memoreaza pozitiile 
	std::vector<float> RightDecorPos = std::vector<float>(7);
	std::vector<float> LeftDecorPos = std::vector<float>(7);

	//memoreaza tranzitiile pentru lampile de pe stanga si de pe dreapta
	std::vector<float> translateRightLampsValues = std::vector<float>(7);
	std::vector<float> translateLeftLampsValues = std::vector<float>(7);

	//memoreaza pozitiile 
	std::vector<float> RightLampsPos = std::vector<float>(7);
	std::vector<float> LeftLampsPos = std::vector<float>(7);

	std::vector<float> flowersRightPos1 = std::vector<float>(7);
	std::vector<float> flowersLeftPos1 = std::vector<float>(7);
	std::vector<float> flowersRightPos2 = std::vector<float>(7);
	std::vector<float> flowersLeftPos2 = std::vector<float>(7);
	std::vector<float> flowersRightPos3 = std::vector<float>(7);
	std::vector<float> flowersLeftPos3 = std::vector<float>(7);

	std::vector<glm::vec3> spotLightPosition = std::vector<glm::vec3>(7);
	std::vector<glm::vec3> spotLightDirection = std::vector<glm::vec3>(7);

	std::vector<float> translateWaterPlan = std::vector<float>(4);
	std::vector<float> waterPos = std::vector<float>(4);

	float translateObstacles; 
	
	//retine valorile tranzitiile pentru cele 8 elemente colectabile
	std::vector<float> translateCollectionItem = std::vector<float>(7);

	//retine pozitiile pentru cele 6 obstacole
	std::vector<glm::vec3> obstaclesPos = std::vector<glm::vec3>(7);
	float rotAngle;

	std::vector<glm::vec3> collectionItemsPos = std::vector<glm::vec3>(7);
	float jumpingCube; //elementele colectibile reprezentate prin cuuburi sunt translatate initial in sensul pozitiv al axei oY
	bool giftIsJumping = true;
	float jumpMaxHeight = 0.75f;

	float obstacleHeight = 0.65f;

	//retine daca s-a produc coliziune cu cele 6 obstacole
	std::vector<bool> collideCheckValues = std::vector<bool>(7);

	std::vector<bool> collectionCheckValues = std::vector<bool>(7);

	//retine coordonatele pentru bounding spheres asociate fiecarui element colectabil vizibil in scena la un moment dat
	std::vector<glm::vec3> itemBoundingSpheres = std::vector<glm::vec3>(7);
	std::vector<float> itemRadius = std::vector<float>(7); //retine valorile razelor boundingSpheres asociate elementelor decorative

	float scorValue; //retine scorul obtinut de jucator
	float maxScore = 150;


	glm::vec3 scoreInfoPos = glm::vec3(2.5f, 3.75f, 0.55f);
};