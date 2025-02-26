#include "Tema3.h"
#include "Transform3D.h"
#include "Transform2D.h"

#include <vector>
#include <string>
#include <iostream>
#include <time.h> 
#include <Core/Engine.h>

using namespace std;

Tema3::Tema3()
{
	window->SetSize(1500, 900);

	Mesh* star = CreateStar();

	FoV = 45.0f;
	//defineste pozitia initiala a camerei
	thirdPersonCamPosition = glm::mat4(1);
	thirdPersonCamPosition = Transform3D::Translate(0, 4.65f, 6.55f);
	thirdPersonCam = true;

	//defineste pozitia camerei firstPerson
	firstPersonCamPosition = glm::mat3(1);
	glm::vec3 pos = player.getActualPlayerCoords();
	firstPersonCamPosition *= Transform3D::Translate(pos.x, pos.y, pos.z - 0.25f);
	
	targetPosition = firstPersonCamPosition;
	targetPosition *= Transform3D::Translate(0.f, 0.f, -2.0f);

	renderCameraTarget = false;
	camera = new Tema_3::Camera();
	camera->Set(thirdPersonCamPosition * glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	

	projectionMatrix = glm::perspective(RADIANS(FoV), window->props.aspectRatio, 0.01f, 200.0f);
	
	platforms = new Platform();

	//defineste coordonatele la care urmeaza sa fie redate cuburile
	platforms->generatePlatform();
	setTranslatePoints();

	int i;
	for (i = 0; i < 6; i++) {
		translateRightDecorValues[i] = -i * 7.5f;
		translateLeftDecorValues[i] = -i * 7.5f;

		translateRightLampsValues[i] = -i * 19.5f - i;
		translateLeftLampsValues[i] = -i * 22.5f - i;

	}

	
	
	materialKd = 1.5;
	materialKs = 1.5;
	materialShininess = 70;
	cutOff = (float)M_PI / 3.5f;
}

Tema3::~Tema3()
{
	delete camera;
}

Mesh* Tema3::DefineSquare() {
	glm::vec3 color = glm::vec3(0.859, 0.439, 0.576);
	Mesh* square = new Mesh("square");
	vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(1, 0, 0), color),
		VertexFormat(glm::vec3(0, -1, 0), color),
		VertexFormat(glm::vec3(1, -1, 0), color)
	};
	vector<unsigned short> indices =
	{
		0, 2, 1,
		1, 2, 3
	};
	square->InitFromData(vertices, indices);
	meshes["square"] = square;
	return square;
}

Mesh* Tema3::DefineBlackSquare() {
	glm::vec3 color = glm::vec3(1.000, 0.980, 0.980);
	Mesh* square = new Mesh("black_square");
	vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(1, 0, 0), color),
		VertexFormat(glm::vec3(0, -1, 0), color),
		VertexFormat(glm::vec3(1, -1, 0), color)
	};
	vector<unsigned short> indices =
	{
		0, 2, 1,
		1, 2, 3
	};
	square->InitFromData(vertices, indices);
	meshes["black_square"] = square;
	return square;
}

Mesh* Tema3::CreateDiamond() {
	Mesh* diamond = new Mesh("diamant");
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 1), glm::vec3(1, 0, 0)),
		VertexFormat(glm::vec3(0, 0.25f, 0), glm::vec3(1, 1, 0)),
		VertexFormat(glm::vec3(0, 0, -0.25f), glm::vec3(1, 1, 1)),
		VertexFormat(glm::vec3(0.25f, -0.25, 0), glm::vec3(1, 1, 0)),
		VertexFormat(glm::vec3(-0.25f, -0.25, 0), glm::vec3(1, 1, 0))
	};
	std::vector<unsigned short> indices =
	{
		0, 1, 4,
		0, 4, 3,
		0, 3, 1,
		2, 1, 3,
		2, 4, 1,
		2, 3, 4,
	};
	diamond->InitFromData(vertices, indices);
	meshes["diamant"] = diamond;
	return diamond;
}

void Tema3::LoadMeshes() {
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

	{
		Mesh* mesh = DefineSquare(); //patrat folosit ulterior pentru fuel bar
	}

	{
		Mesh* mesh = DefineBlackSquare(); //patrat folosit ulterior pentru fuel bar
	}

	{
		Mesh* diamond = CreateDiamond();
	}

	{
		std::vector<glm::vec3> positions = 
		{
			glm::vec3(0, 0, 0),
			glm::vec3(0.5f, 0, 0.86f),
			glm::vec3(1, 0, 0),
			glm::vec3(0.5f, 1, 0.28f)
		};

		std::vector<glm::vec3> normals =
		{
			glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 1, 0)
		};

		std::vector<glm::vec2> texCoords =
		{
			glm::vec2(0, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 0),
			glm::vec2(1, 1)
		};

		vector<unsigned short> indices = 
		{
			0, 1, 3,
			1, 2, 3,
			0, 2, 1,
			0, 3, 2
		};

		Mesh* tetraedru = new Mesh("tetraedru");
		tetraedru->InitFromData(positions, normals, texCoords, indices);
		meshes[tetraedru->GetMeshID()] = tetraedru;
	}

	Mesh* cube = CreateStylisedCube();
	meshes[cube->GetMeshID()] = cube;

	Mesh* piramida = CreatePyramid();

	{
		{
			vector<glm::vec3> vertices
			{
				glm::vec3(0.5f,   0.5f, 0.0f),	// Top Right
				glm::vec3(0.5f,  -0.5f, 0.0f),	// Bottom Right
				glm::vec3(-0.5f, -0.5f, 0.0f),	// Bottom Left
				glm::vec3(-0.5f,  0.5f, 0.0f),	// Top Left
			};

			vector<glm::vec3> normals
			{
				glm::vec3(0, 1, 1),
				glm::vec3(1, 0, 1),
				glm::vec3(1, 0, 0),
				glm::vec3(0, 1, 0)
			};
			vector<glm::vec2> textureCoords
			{
				glm::vec2(0.0f, 0.0f),
				glm::vec2(1.0f, 0.0f),
				glm::vec2(1.0f, 1.0f),
				glm::vec2(0.0f, 1.0f)
			};

			vector<unsigned short> indices =
			{
				0, 1, 3,
				1, 2, 3
			};
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			Mesh* mesh = new Mesh("patrat");
			mesh->InitFromData(vertices, normals, textureCoords, indices);
			meshes[mesh->GetMeshID()] = mesh;
		}
	}

	Mesh* sfera = CreateSphere();
	meshes[sfera->GetMeshID()] = sfera;

}


Mesh* Tema3::CreatePyramid() {
	Mesh* piramida = new Mesh("piramida");
	std::vector<glm::vec3> positions =
	{
		glm::vec3(0, 0.5f, 0),
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, 0.5f),

		glm::vec3(0, 0.5f, 0),
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, 0.5f),

		glm::vec3(0, 0.5f, 0),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f),

		glm::vec3(0, 0.5f, 0),
		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f)
	};

	std::vector<glm::vec3> normals =
	{
		//triunghiul din fata
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		
		//trtiunghiul din stanga
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),
	
		//triunghiul din spate
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),

		//triunghiul din dreapta
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0)
	};

	std::vector<glm::vec2> texCoords =
	{
		glm::vec2(0.5f, 1),
		glm::vec2(0, 0),
		glm::vec2(1, 0),

		glm::vec2(0.5f, 1),
		glm::vec2(0, 0),
		glm::vec2(1, 0),

		glm::vec2(0.5f, 1),
		glm::vec2(0, 0),
		glm::vec2(1, 0),

		glm::vec2(0.5f, 1),
		glm::vec2(0, 0),
		glm::vec2(1, 0)
	};

	vector<unsigned short> indices =
	{
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11
	};
	piramida->InitFromData(positions, normals, texCoords, indices);

	meshes[piramida->GetMeshID()] = piramida;
	return piramida;
}

Mesh* Tema3::CreateSphere() {
	//creare sfera
	
		vector<glm::vec3> vertices_sfera;
		vector<unsigned short> indices_sfera;
		vector<glm::vec3> normals;
		vector<glm::vec2> texCoords;

		float pi = 2 * acos(0.0);
		float nr_latitudinal = 15;
		float nr_longitudinal = 50;

		float v, u;
		float x, y, z;		//(x, y, z) punct de pe sfera
		float r = 0.5f;		//raza sferei

		//coordonatele unui punct de pe sfera (x, y, z):
		//z = r * sin (u)
		//x = r * cos(v) * cos(u)
		//y = r * sin(v) * cos(u)

		//u - unghiul determinat de 2 puncte de pe sfera unite pe directie latitudinala
		//v - unghiul determinat de 2 puncte de pe sfera unite pe directie longitudinala
		float nx, ny, nz, lengthInv = 1.0f / r;    // vertex normal
		float s, t;                                     // vertex texCoord

		int i;
		for (i = 0; i <= nr_latitudinal; i++) {
			u = pi / 2 - (pi * i / nr_latitudinal);

			for (int j = 0; j <= nr_longitudinal; j++) {
				v = 2 * pi * j / nr_longitudinal;

				x = r * cosf(u) * cosf(v);
				y = r * cosf(u) * sinf(v);
				z = r * sinf(u);

				if (j % 2 == 0) {
					vertices_sfera.push_back(glm::vec3(x, y, z)); 
				}
				else {
					vertices_sfera.push_back(glm::vec3(x, y, z)); 
				}

				// normalized vertex normal (nx, ny, nz)
				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;
				normals.push_back(glm::vec3(nx, ny, nz));

				// vertex tex coord (s, t) range between [0, 1]
				s = (float)j / nr_longitudinal;
				t = (float)i / nr_latitudinal;
				texCoords.push_back(glm::vec2(s, t));
			}
		}

		int v1, v2, v3, v4;
		for (int i = 0; i < nr_latitudinal; ++i)
		{
			v1 = i * (nr_longitudinal + 1);
			v2 = v1 + nr_longitudinal + 1;
			v3 = v1 + 1;
			v4 = v2 + 1;
			//intr-o portiune latitudinala identificata prin indexul i, sunt incluse cate
			//2 triunghiuri pe sector londitudinal, cu exceptia celor de indice 0 si (nr_latitudinal - 1)
			//care au doar un triunghi pe sector

			for (int j = 0; j < nr_longitudinal; j++) {
				if (i != 0 && i != (nr_latitudinal - 1)) {
					indices_sfera.push_back(v1);
					indices_sfera.push_back(v2);
					indices_sfera.push_back(v3);

					indices_sfera.push_back(v3);
					indices_sfera.push_back(v2);
					indices_sfera.push_back(v4);
				}
				else if (i == 0) {
					indices_sfera.push_back(v3);
					indices_sfera.push_back(v2);
					indices_sfera.push_back(v4);
				}
				else {
					indices_sfera.push_back(v1);
					indices_sfera.push_back(v2);
					indices_sfera.push_back(v3);
				}
				v1++;
				v2++;
				v3++;
				v4++;
			}
		}

		Mesh* sfera = new Mesh("sfera");	
		sfera->InitFromData(vertices_sfera, normals, texCoords, indices_sfera);

		meshes[sfera->GetMeshID()] = sfera;
		return sfera;
}

Mesh* Tema3::CreateStylisedCube() {

	std::vector<glm::vec3> vertices =
	{
		glm::vec3(0, 0, 1),
		glm::vec3(1, 0, 1),
		glm::vec3(0, 1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(1, 1, 0),

		glm::vec3(0, 0, 1),
		glm::vec3(1, 0, 1),
		glm::vec3(0, 1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(1, 1, 0),

		glm::vec3(0, 0, 1),
		glm::vec3(1, 0, 1),
		glm::vec3(0, 1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(1, 1, 0),
	};

	std::vector<unsigned short> indices = {
			0, 1, 2,
			1, 3, 2,

			10, 11, 15,
			10, 15, 14,

			17, 23, 19,
			17, 21, 23,

			6, 7, 4,
			7, 5, 4,

			8, 12, 9,
			9, 12, 13,

			18, 22, 20,
			16, 18, 20
	};

	std::vector<glm::vec3> normals
	{
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),

		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),

		glm::vec3(0, -1, 0),//8
		glm::vec3(0, -1, 0),//9
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),

		glm::vec3(0, -1, 0),//12
		glm::vec3(0, -1, 0),//13
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),

		glm::vec3(-1, 0, 0),//16
		glm::vec3(1, 0, 0),//17
		glm::vec3(-1, 0, 0),//18
		glm::vec3(1, 0, 0),//19

		glm::vec3(-1, 0, 0),//20
		glm::vec3(1, 0, 0),//21
		glm::vec3(-1, 0, 0),//22
		glm::vec3(1, 0, 0)//23
	};

	std::vector<glm::vec2> textureCoords
	{
		glm::vec2(0.f, 0.f),
		glm::vec2(1.f, 0.f),
		glm::vec2(0.f, 1.f),
		glm::vec2(1.f, 1.f),

		glm::vec2(0.f, 0.f),
		glm::vec2(1.f, 0.f),
		glm::vec2(0.f, 1.f),
		glm::vec2(1.f, 1.f),

		//8, 9, 12, 13 - fata de jos a cubului
		//10, 11, 14, 15 - fata de sus a cubului
		glm::vec2(0.f, 0.f),//8
		glm::vec2(1.f, 0.f),//9
		glm::vec2(0.f, 0.f),//10
		glm::vec2(1.f, 0.f),//11

		glm::vec2(0.f, 1.f),//12
		glm::vec2(1.f, 1.f),//13
		glm::vec2(0.f, 1.f),//14
		glm::vec2(1.f, 1.f),//15

		//fetele laterale ale cubului
		//fata laterala din stanga - 16, 20, 18, 22
		//fata laterala din dreapta - 17, 19, 21, 23
		glm::vec2(0.f, 0.f), //16
		glm::vec2(0.f, 0.f),
		glm::vec2(0.f, 1.f), //18
		glm::vec2(0.f, 1.f),

		glm::vec2(1.f, 0.f),//20
		glm::vec2(1.f, 0.f),
		glm::vec2(1.f, 1.f),//22
		glm::vec2(1.f, 1.f)
	};

	Mesh* cube = new Mesh("cub_stilizat");
	cube->InitFromData(vertices, normals, textureCoords, indices);
	return cube;
}

std::vector<float> generateCircleVertices() {

	std::vector<float> vertices;
	float angle;
	float step = 2 * 3.1415926f / 200;
	for (int i = 0; i < 200; i++) {
		angle = i * step;
		//coordonata x
		vertices.push_back(cos(angle)); 
		//coordoanata y
		vertices.push_back(sin(angle)); 
		//coordonata z
		vertices.push_back(0);                
	}
	return vertices;
}

Mesh* Tema3::CreateCylinder() {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned short> indices;

	Mesh* cylinder = new Mesh("cilindru");
	//cylinder->InitFromData(positions, normals, indices, textures);
	return cylinder;
}

void Tema3::LoadShaders() {
	//adauga un shader pentru aplicarea culorii in vectorul shaders
	{
		Shader* shader = new Shader("textureShader");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders["textureShader"] = shader;
	}
	{
		Shader* shader = new Shader("basicShader");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/BasicVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/BasicFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders["basicShader"] = shader;
	}
	{
		//adauga un shader cu aplicarea unei functii de zgomot
		Shader* shader1 = new Shader("noiseShader");
		shader1->AddShader("Source/Laboratoare/Tema3/Shaders/NoiseShaderVS.glsl", GL_VERTEX_SHADER);
		shader1->AddShader("Source/Laboratoare/Tema3/Shaders/NoiseShaderFS.glsl", GL_FRAGMENT_SHADER);
		shader1->CreateAndLink();
		shaders["noiseShader"] = shader1;
	}
	{
		Shader* shader = new Shader("waterShader");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/WaterVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/WaterFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders["waterShader"] = shader;
	}
}

void Tema3::LoadTextures() {
	//load textures
	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/face.png";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["marmura"] = texture;
	}
	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/frunze_copac.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["grass"] = texture;
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/iarba.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["iarba"] = texture;
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/pietre3.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["pietre"] = texture;
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/piatra1.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["caramida"] = texture;
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/piatra2.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["pietreGri"] = texture;
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/scoarta_copac.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["copac"] = texture;
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/copac1.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["copac1"] = texture;
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/pietre3.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["piatra"] = texture;
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/lemn.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["lemn"] = texture;
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/rugina.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["tabla"] = texture;
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/metalic.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["metal"] = texture;
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/flori1.png";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["floare1"] = texture;
		texture->SetFiltering(GL_NEAREST, GL_NEAREST);
		//texture->SetFiltering(GL_TEXTURE_MIN_FILTER);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	
	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/flori2.png";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["floare2"] = texture;
		texture->SetFiltering(GL_NEAREST, GL_NEAREST);
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/flori3.png";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["floare3"] = texture;
		texture->SetFiltering(GL_NEAREST, GL_NEAREST);
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/flori4.png";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["floare4"] = texture;
		texture->SetFiltering(GL_NEAREST, GL_NEAREST);
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/grass.png";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["leaves"] = texture;
		texture->SetFiltering(GL_NEAREST, GL_NEAREST);
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/apa.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["apa"] = texture;
	}

	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/gold.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["aur"] = texture;
	}
	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/ice.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["gheata"] = texture;
	}
	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/culori.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["wool"] = texture;
	}
	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/cadou.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["cadou"] = texture;
	}
	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/cadou1.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["cadou1"] = texture;
	}
	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/gold1.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["gold1"] = texture;
	}
	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/silver.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["silver"] = texture;
	}
	{
		const string textureLoc = "Source/Laboratoare/Tema3/Textures/sun.jpg";
		Texture2D* texture = new Texture2D();
		texture->Load2D(textureLoc.c_str(), GL_REPEAT);
		mapTextures["sun"] = texture;
		texture->SetFiltering(GL_NEAREST, GL_NEAREST);
	}
}

void Tema3::Init()
{
	LoadMeshes();
	LoadShaders();
	LoadTextures();

	player.setYCoord(0.80f);
}


Mesh* Tema3::CreateStar() {
	Mesh* star = new Mesh("star");
	vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(-0.5f, 0.5f, 0), glm::vec3(0.255, 0.412, 0.882)),
		VertexFormat(glm::vec3(0, 2, 0), glm::vec3(0.502, 0.000, 0.502)),
		VertexFormat(glm::vec3(0.5f, 0.5f, 0), glm::vec3(0.255, 0.412, 0.882)),
		VertexFormat(glm::vec3(2, 0, 0), glm::vec3(0.502, 0.000, 0.502)),
		VertexFormat(glm::vec3(0.5f, -0.5f, 0), glm::vec3(0.255, 0.412, 0.882)),
		VertexFormat(glm::vec3(0, -2, 0), glm::vec3(0.502, 0.000, 0.502)),
		VertexFormat(glm::vec3(-0.5f, -0.5f, 0), glm::vec3(0.255, 0.412, 0.882)),
		VertexFormat(glm::vec3(-2, 0, 0), glm::vec3(0.502, 0.000, 0.502)),
	};

	vector<unsigned short> indices =
	{
		0, 2, 1,
		7, 6, 0,
		2, 4, 3,
		6, 5, 4,
		0, 6, 4,
		2, 0, 4
	};
	star = CreateMesh("star", vertices, indices);
	return star;
}

Mesh* Tema3::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);

	// TODO: Send vertices data into the VBO buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//trimite date la vbo-ul activ
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	unsigned int IBO = 0;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(),
		&indices[0], GL_STATIC_DRAW);
	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// TODO: Unbind the VAO
	glBindVertexArray(0); //dezactiveaza legatura catre VAO-ul curent

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	return meshes[name];
}

//render with textures
void Tema3::RenderMeshTex(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	GLint viewPosLoc = glGetUniformLocation(shader->program, "viewPos");
	glUniform3f(viewPosLoc, camera->position.x, camera->position.y + 2, camera->position.z);

	glUniform1i(glGetUniformLocation(shader->program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(shader->program, "material.specular"), 1);
	glUniform1f(glGetUniformLocation(shader->program, "material.shininess"), 36.0f);

	int var = glGetUniformLocation(shader->program, "grass");
	if (texture == mapTextures["leaves"] || texture == mapTextures["sun"]) {
		glUniform1i(var, 1);
	}
	else {
		glUniform1i(var, 0);
	}
	

	int i;
	for (i = 0; i < 1; i++) {
		string value = "pointLights[";
		string aux = value.append(std::to_string(i));
		string result = aux.append("].position");
		const GLchar* shader_code = result.c_str();
		glUniform3f(glGetUniformLocation(shader->program, shader_code),
			pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
		//cout << shader_code << endl;
		value = "pointLights[";
		aux = value.append(std::to_string(i));
		result = aux.append("].ambient");
		shader_code = result.c_str();
		glUniform3f(glGetUniformLocation(shader->program, shader_code), 0.5f, 0.5f, 0.5f);
		//cout << shader_code << endl;
		value = "pointLights[";
		aux = value.append(std::to_string(i));
		result = aux.append("].diffuse");
		shader_code = result.c_str();
		glUniform3f(glGetUniformLocation(shader->program, shader_code), 0.8f, 0.8f, 0.8f);
		//cout << shader_code << endl;
		value = "pointLights[";
		aux = value.append(std::to_string(i));
		result = aux.append("].specular");
		shader_code = result.c_str();
		glUniform3f(glGetUniformLocation(shader->program, shader_code), 1.5f, 1.5f, 1.5f);
		//cout << shader_code << endl;
		value = "pointLights[";
		aux = value.append(std::to_string(i));
		result = aux.append("].constant");
		shader_code = result.c_str();
		glUniform1f(glGetUniformLocation(shader->program, shader_code), 1.0f);//1
		//cout << shader_code << endl;
		value = "pointLights[";
		aux = value.append(std::to_string(i));
		result = aux.append("].linear");
		shader_code = result.c_str();
		glUniform1f(glGetUniformLocation(shader->program, shader_code), 0.045);//0.014
		//cout << shader_code << endl;
		value = "pointLights[";
		aux = value.append(std::to_string(i));
		result = aux.append("].quadratic");
		shader_code = result.c_str();
		glUniform1f(glGetUniformLocation(shader->program, shader_code), 0.0075);//0.0007
		value = "pointLights[";
		aux = value.append(std::to_string(i));
		result = aux.append("].color");
		shader_code = result.c_str();
		glUniform3f(glGetUniformLocation(shader->program, shader_code), lightColors[i].x, lightColors[i].y, lightColors[i].z);
		//cout << shader_code << endl;
	}

	for (i = 0; i < 4; i++) {
		string value = "spotLights[";
		string aux = value.append(std::to_string(i));
		string result = aux.append("].position");
		const GLchar* shader_code = result.c_str();
		glUniform3f(glGetUniformLocation(shader->program, shader_code),
			spotLightPositions[i].x, spotLightPositions[i].y, spotLightPositions[i].z);
		//cout << shader_code << endl;
		value = "spotLights[";
		aux = value.append(std::to_string(i));
		result = aux.append("].direction");
		shader_code = result.c_str();
		glUniform3f(glGetUniformLocation(shader->program, shader_code), 
			spotLightDirections[i].x, spotLightDirections[i].y, spotLightDirections[i].z);
		//cout << shader_code << endl;
		value = "spotLights[";
		aux = value.append(std::to_string(i));
		result = aux.append("].ambient");
		shader_code = result.c_str();
		glUniform3f(glGetUniformLocation(shader->program, shader_code), 0.2, 0.2, 0.2);
		//cout << shader_code << endl;
		value = "spotLights[";
		aux = value.append(std::to_string(i));
		result = aux.append("].diffuse");
		shader_code = result.c_str();
		glUniform3f(glGetUniformLocation(shader->program, shader_code), 1, 1, 1);
		//cout << shader_code << endl;
		value = "spotLights[";
		aux = value.append(std::to_string(i));
		result = aux.append("].specular");
		shader_code = result.c_str();
		glUniform3f(glGetUniformLocation(shader->program, shader_code), 1, 1, 1);
		//cout << shader_code << endl;
		value = "spotLights[";
		aux = value.append(std::to_string(i));
		result = aux.append("].linear");
		shader_code = result.c_str();
		glUniform1f(glGetUniformLocation(shader->program, shader_code), 0.9);
		//cout << shader_code << endl;
		value = "spotLights[";
		aux = value.append(std::to_string(i));
		result = aux.append("].quadratic");
		shader_code = result.c_str();
		glUniform1f(glGetUniformLocation(shader->program, shader_code), 0.032);
		value = "spotLights[";
		aux = value.append(std::to_string(i));
		result = aux.append("].constant");
		shader_code = result.c_str();
		glUniform1f(glGetUniformLocation(shader->program, shader_code), 1.0f);
		value = "spotLights[";
		aux = value.append(std::to_string(i));
		result = aux.append("].color");
		shader_code = result.c_str();
		glUniform3f(glGetUniformLocation(shader->program, shader_code), lightColors[i].x, lightColors[i].y, lightColors[i].z);
		value = "spotLights[";
		aux = value.append(std::to_string(i));
		result = aux.append("].cutOff");
		shader_code = result.c_str();
		glUniform1f(glGetUniformLocation(shader->program, shader_code), glm::cos(glm::radians(15.0f)));
		value = "spotLights[";
		aux = value.append(std::to_string(i));
		result = aux.append("].outerCutOff");
		shader_code = result.c_str();
		glUniform1f(glGetUniformLocation(shader->program, shader_code), glm::cos(glm::radians(0.0f)));
		//cout << shader_code << endl;
	}

	// Bind model matrix
	GLint loc_modelMatrix = glGetUniformLocation(shader->program, "model");
	glUniformMatrix4fv(loc_modelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_viewMatrix = glGetUniformLocation(shader->program, "view");
	glUniformMatrix4fv(loc_viewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projectionMatrix = glGetUniformLocation(shader->program, "projection");
	glUniformMatrix4fv(loc_projectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
	//glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, Texture2D* texture)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_modelMatrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_modelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_viewMatrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_viewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projectionMatrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int aux = glGetUniformLocation(shader->program, "usesTexture");
	if (texture != NULL) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "textures"), 0);
		glUniform1i(aux, 1);
	}
	else {
		glUniform1i(aux, 0);
	}

	int object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(object_color, 0.545, 0.000, 0.545);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	//glClearColor(0.529, 0.808, 0.980, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::setTranslatePoints()
{
	int i;
	for (i = 0; i < 3; i++) {
		platforms->setTranslatePoint(i, -5.5f - platforms->getPlatformSize(i) / 2);
	}
	float maxCoord = -999;
	for (i = 0; i < 3; i++) {
		if (platforms->getPlatformSize(i) > maxCoord) {
			maxCoord = platforms->getPlatformSize(i);
		}
	}
	for (i = 3; i < 6; i++) {
		platforms->setTranslatePoint(i, -5.5f - maxCoord - 1 - platforms->getPlatformSize(i) / 2);
	}
	float maxCoord1 = -999;
	for (i = 3; i < 6; i++)
	{
		if (platforms->getPlatformSize(i) > maxCoord1) {
			maxCoord1 = platforms->getPlatformSize(i);
		}
	}
	for (i = 6; i < 9; i++) {
		platforms->setTranslatePoint(i, -5.5f - maxCoord - 2 - platforms->getPlatformSize(i) / 2 - maxCoord1);
	}

	float maxCoord2 = -999;
	for (i = 6; i < 9; i++)
	{
		if (platforms->getPlatformSize(i) > maxCoord2) {
			maxCoord2 = platforms->getPlatformSize(i);
		}
	}
	for (i = 9; i < 12; i++) {
		platforms->setTranslatePoint(i, -5.5f - maxCoord - 3 - platforms->getPlatformSize(i) / 2 - maxCoord1 - maxCoord2);
	}

	float maxCoord3 = -999;
	for (i = 9; i < 12; i++)
	{
		if (platforms->getPlatformSize(i) > maxCoord3) {
			maxCoord3 = platforms->getPlatformSize(i);
		}
	}
	for (i = 12; i < 15; i++) {
		platforms->setTranslatePoint(i, -5.5f - maxCoord - 4 - platforms->getPlatformSize(i) / 2 - maxCoord1 - maxCoord2 - maxCoord3);
	}
}

void Tema3::LoadStars() {
	int i;
	for (i = 0; i < lives; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		//2.5f, 3.75f, 0.55f
		modelMatrix *= Transform3D::Translate(2.1f + i / 2.75f, 3.35f, 0.55f);
		modelMatrix *= Transform3D::Scale(0.05, 0.05, 0.05);
		RenderMesh(meshes["star"], shaders["VertexColor"], modelMatrix);
	}
}

//intoarce true daca obstacolul cu indexul index are coliziune cu sfera
bool Tema3::CheckObstacleCollision(int index) {
	bool ok = false;
	glm::vec4 playerCoord = player.getActualPlayerCoords();
	float i;
	//stabileste coordonatele suprafetei obstacolului
	float max_step = obstaclesPos[index].y + obstacleHeight / 2;
	float step = obstaclesPos[index].y - obstacleHeight / 2;
	float zCoord;
	float xCoord;
	float yCoord;
	for (i = step; i < max_step; i += 0.01f) {
		float yCoord = i;
		//verifica daca punctul obstacolului cel mai apropiat de sfera este plasat in interiorul sferei
		xCoord = obstaclesPos[index].x;
		zCoord = obstaclesPos[index].z;
		float distance = sqrt(pow((xCoord - playerCoord.x), 2) + pow((yCoord - playerCoord.y), 2) + pow((zCoord - playerCoord.z), 2));

		if (distance <= 1) { //coliziune
			ok = true;
		}
	}

	return ok;
}

void Tema3::CheckObstacleCollisions() {
	//verifica coliziunea cu cele 6 obstacole si actualizeaza numarul de vieti
	int i;
	for (i = 0; i < 6; i++) {
		if (collideCheckValues[i] == false && CheckObstacleCollision(i) == true) {
			cout << "Coliziune cu obstacolul " << i << endl;
			lives--;
			collideCheckValues[i] = true;
		}
	}
}

//cuburile nou generate vor fi mereu afisate la aceeasi distanta de jucator pe axa OZ
void Tema3::LoadPlatforms() {
	int i;
	bool ok1 = false;
	int count = 0;

	for (i = 0; i < 3; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		if (i != 1) {
			modelMatrix *= Transform3D::Translate(2.85f * (i - 1), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
			modelMatrix *= Transform3D::Scale(2.85f, 0.25f, platforms->getPlatformSize(i));
		}
		else {
			modelMatrix *= Transform3D::Translate(2.5f * (i - 1), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
			modelMatrix *= Transform3D::Scale(1.40f, 0.25f, platforms->getPlatformSize(i));
		}
		
		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);

		float zCoord = position.z;
		platforms->setPlatformXCoord(position.x, i);
		platforms->setPlatformYCoord(position.y, i);
		platforms->setPlatformZCoord(zCoord - platforms->getPlatformSize(i) / 2, i);

		//platformele care dispar in spatele camerei nu mai sunt redate
		if (platforms->getPlatformPos(i) <= 4.5f) {
			if (i == 1) {
				RenderMeshTex(meshes["box"], shaders["textureShader"], modelMatrix, mapTextures["piatra"]);
			}
			else {
				RenderMeshTex(meshes["box"], shaders["textureShader"], modelMatrix, mapTextures["iarba"]);
			}
			
		}
		else {
			count++;
		}
	}
	
	if (count == 3) {
		ok1 = true;//platformele de la 0 la 2 nu vor mai aparea, trebuie sterse
	}
	count = 0;
	//1.25f este distanta dintre platforme
	for (i = 3; i < 6; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		if (i != 4) {
			modelMatrix *= Transform3D::Translate(2.85f * (i - 4), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
			modelMatrix *= Transform3D::Scale(2.85f, 0.25f, platforms->getPlatformSize(i));
		}
		else {
			modelMatrix *= Transform3D::Translate(2.5f * (i - 4), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
			modelMatrix *= Transform3D::Scale(1.40f, 0.25f, platforms->getPlatformSize(i));
		}
		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		float zCoord = position.z;
		platforms->setPlatformXCoord(position.x, i);
		platforms->setPlatformYCoord(position.y, i);
		platforms->setPlatformZCoord(zCoord - platforms->getPlatformSize(i) / 2, i);
		if (i == 4) {
			RenderMeshTex(meshes["box"], shaders["textureShader"], modelMatrix, mapTextures["piatra"]);
		}
		else {
			RenderMeshTex(meshes["box"], shaders["textureShader"], modelMatrix, mapTextures["iarba"]);
		}
	}
	for (i = 6; i < 9; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		if (i != 7) {
			modelMatrix *= Transform3D::Translate(2.85f * (i - 7), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
			modelMatrix *= Transform3D::Scale(2.85f, 0.25f, platforms->getPlatformSize(i));
		}
		else {
			modelMatrix *= Transform3D::Translate(2.5f * (i - 7), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
			modelMatrix *= Transform3D::Scale(1.40f, 0.25f, platforms->getPlatformSize(i));
		}
		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		float zCoord = position.z;
		platforms->setPlatformXCoord(position.x, i);
		platforms->setPlatformYCoord(position.y, i);
		platforms->setPlatformZCoord(zCoord - platforms->getPlatformSize(i) / 2, i);
		if (i == 7) {
			RenderMeshTex(meshes["box"], shaders["textureShader"], modelMatrix, mapTextures["piatra"]);
		}
		else {
			RenderMeshTex(meshes["box"], shaders["textureShader"], modelMatrix, mapTextures["iarba"]);
		}
		
	}

	for (i = 9; i < 12; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		if (i != 10) {
			modelMatrix *= Transform3D::Translate(2.85f * (i - 10), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
			modelMatrix *= Transform3D::Scale(2.85f, 0.25f, platforms->getPlatformSize(i));
		}
		else {
			modelMatrix *= Transform3D::Translate(2.5f * (i - 10), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
			modelMatrix *= Transform3D::Scale(1.40f, 0.25f, platforms->getPlatformSize(i));
		}
		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		float zCoord = position.z;
		platforms->setPlatformXCoord(position.x, i);
		platforms->setPlatformYCoord(position.y, i);
		platforms->setPlatformZCoord(zCoord - platforms->getPlatformSize(i) / 2, i);
		if (i == 10) {
			RenderMeshTex(meshes["box"], shaders["textureShader"], modelMatrix, mapTextures["piatra"]);
		}
		else {
			RenderMeshTex(meshes["box"], shaders["textureShader"], modelMatrix, mapTextures["iarba"]);
		}
	}
	for (i = 12; i < 15; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		if (i != 13) {
			modelMatrix *= Transform3D::Translate(2.85f * (i - 13), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
			modelMatrix *= Transform3D::Scale(2.85f, 0.25f, platforms->getPlatformSize(i));
		}
		else {
			modelMatrix *= Transform3D::Translate(2.5f * (i - 13), 0, platforms->getTranslatePoint(i) + platforms->getTranslateVal(i));
			modelMatrix *= Transform3D::Scale(1.40f, 0.25f, platforms->getPlatformSize(i));
		}
		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		float zCoord = position.z;
		platforms->setPlatformXCoord(position.x, i);
		platforms->setPlatformYCoord(position.y, i);
		platforms->setPlatformZCoord(zCoord - platforms->getPlatformSize(i) / 2, i);
		if (i == 13) {
			RenderMeshTex(meshes["box"], shaders["textureShader"], modelMatrix, mapTextures["piatra"]);
		}
		else {
			RenderMeshTex(meshes["box"], shaders["textureShader"], modelMatrix, mapTextures["iarba"]);
		}
	}


	//sterge platformele care nu mai sunt vizibile si genereaza altele in locul lor
	if (ok1 == true) {
		//calculeaza lungimea celei mai lungi platforme din cele care se sterg
		int i;
		float maxSize = -999;
		for (i = 0; i < 3; i++) {
			if (platforms->getPlatformSize(i) > maxSize) {
				maxSize = platforms->getPlatformSize(i);
			}
		}
		platforms->setLastMaxPlatform(maxSize);
		platforms->deletePlatform(0);
		platforms->deletePlatform(0);
		platforms->deletePlatform(0);
		platforms->generateNewPlatforms();
	}
}


void Tema3::renderFuelInformation(float deltaTimeSeconds) {
	diffFactor += 0.03;
	if (fallingPlayer == false && start == true) {
		fuelValue -= (3.5f + diffFactor) * deltaTimeSeconds;
	}
	glm::vec3 fuelPos = glm::vec3(-3.f, 2.5f, -1.f);
	glm::mat4 modelMatrix = glm::mat4(1);

	if (!thirdPersonCam)
	{ //redare pentru modul firstPerson
		modelMatrix *= Transform3D::Translate(fuelPos.x + 2.0f, fuelPos.y, fuelPos.z - 6.5f);
		modelMatrix *= Transform3D::Scale(1.25f * fuelValue / initialFuelValue, 0.25f, 1.25f);
		glm::mat4 modelMatrix1 = glm::mat4(1);
		modelMatrix1 *= Transform3D::Translate(fuelPos.x + 2.0f, fuelPos.y, fuelPos.z - 6.5f);
		modelMatrix1 *= Transform3D::Scale(1.25f, 0.25f, 1.25f);
		RenderMesh(meshes["square"], shaders["VertexColor"], modelMatrix);
		RenderMesh(meshes["black_square"], shaders["VertexColor"], modelMatrix1);
	}
	else //redare pentru modul thirdPerson
	{
		glm::mat4 modelMatrix1 = glm::mat4(1);
		modelMatrix1 *= Transform3D::Translate(fuelPos.x, fuelPos.y, fuelPos.z);
		modelMatrix1 *= Transform3D::Scale(1.25f, 0.25f, 1.25f);
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(fuelPos.x, fuelPos.y, fuelPos.z);
		modelMatrix *= Transform3D::Scale(1.25f * fuelValue / initialFuelValue, 0.25f, 1.25f);
		RenderMesh(meshes["square"], shaders["VertexColor"], modelMatrix);
		RenderMesh(meshes["black_square"], shaders["VertexColor"], modelMatrix1);
	}
}

// reda jucatorul in scena
void Tema3::LoadPlayer(float delta) {

	if (fallingPlayer == true) {
		glm::mat4 modelMatrix = glm::mat4(1);
		glm::vec3 pos = player.getActualPlayerCoords();
		modelMatrix *= Transform3D::Translate(pos.x, pos.y, pos.z);
		modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
		glm::vec4 coords = modelMatrix * glm::vec4(0, 0, 0, 1);
		//salveaza coordonatele actuale ale jucatorului
		float y = coords.y;
		y -= 2 * delta;

		player.setActualCoords(glm::vec4(coords.x, y, coords.z, 1));

		if (y < -5) { //jucatorul s-a aflat suficient timp in cadere, jocul se va incheia
			stopGame = true;
		}
		if (stopGame == false) {
			RenderMeshTex(meshes["sphere"], shaders["textureShader"], modelMatrix, mapTextures["marmura"]);

			firstPersonCamPosition = glm::mat4(1);
			firstPersonCamPosition *= Transform3D::Translate(coords.x, coords.y + 0.15f, coords.z - 0.75f);

			targetPosition = firstPersonCamPosition;
			targetPosition *= Transform3D::Translate(0.f, 0.f, -2.0f);

			if (!thirdPersonCam)
			{
				camera->Set(firstPersonCamPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
					targetPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
					glm::vec3(0, 1, 0));
			}
		}
	}
	else if (jumping == false && moveLeft == false && moveRight == false) { //situatia in care pot avea loc coliziuni
		glm::mat4 modelMatrix = glm::mat4(1);
		glm::vec3 pos = player.getActualPlayerCoords();
		modelMatrix *= Transform3D::Translate(pos.x, pos.y, pos.z);
		modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
		glm::vec4 coords = modelMatrix * glm::vec4(0, 0, 0, 1);
		//salveaza coordonatele actuale ale jucatorului

		player.setActualCoords(coords);
		if (collideCheck == false) {
			//RenderSimpleMesh(meshes["sphere"], shaders["basicShader"], modelMatrix, glm::vec3(0.545, 0.000, 0.545), NULL);
			RenderMeshTex(meshes["sphere"], shaders["textureShader"], modelMatrix, mapTextures["marmura"]);
		}
		else {
			//RenderSimpleMesh(meshes["sphere"], shaders["basicShader"], modelMatrix, glm::vec3(0.824, 0.412, 0.118), NULL);
			RenderMeshTex(meshes["sphere"], shaders["textureShader"], modelMatrix, mapTextures["marmura"]);
		}

		firstPersonCamPosition = glm::mat4(1);
		firstPersonCamPosition *= Transform3D::Translate(coords.x, coords.y + 0.15f, coords.z - 0.75f);

		targetPosition = firstPersonCamPosition;
		targetPosition *= Transform3D::Translate(0.f, 0.f, -2.0f);

		if (!thirdPersonCam)
		{
			camera->Set(firstPersonCamPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
				targetPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
				glm::vec3(0, 1, 0));
		}
	}
	else if (jumping == true) {
		glm::mat4 modelMatrix = glm::mat4(1);
		glm::vec3 pos = player.getActualPlayerCoords();
		float y = pos.y;
		if (isUpJumping == true) {
			if (y >= 0.8f) {
				y -= 1.5 * delta;
			}
			else {
				isUpJumping = false;
				jumping = false;
			}
		}
		else {
			if (y <= 1.5f) {
				y += 1.5 * delta;
			}
			else {
				isUpJumping = true;
			}
		}

		modelMatrix *= Transform3D::Translate(pos.x, y, pos.z);
		modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
		glm::vec4 coords = modelMatrix * glm::vec4(0, 0, 0, 1);
		//salveaza coordonatele actuale ale jucatorului

		player.setActualCoords(coords);

		//RenderSimpleMesh(meshes["sphere"], shaders["basicShader"], modelMatrix, glm::vec3(0.545, 0.000, 0.545), NULL);
		RenderMeshTex(meshes["sphere"], shaders["textureShader"], modelMatrix, mapTextures["marmura"]);

		firstPersonCamPosition = glm::mat4(1);
		firstPersonCamPosition *= Transform3D::Translate(coords.x, coords.y + 0.15f, coords.z - 0.75f);

		targetPosition = firstPersonCamPosition;
		targetPosition *= Transform3D::Translate(0.f, 0.f, -2.0f);

		if (!thirdPersonCam)
		{
			camera->Set(firstPersonCamPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
				targetPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
				glm::vec3(0, 1, 0));
		}
	}
	else if (moveLeft == true) {
		glm::mat4 modelMatrix = glm::mat4(1);
		glm::vec3 pos = player.getActualPlayerCoords();
		float y = pos.y;
		float x = pos.x;
		if (isUpLeft == true) {
			if (y >= 0.8f) {
				y -= 1.5 * delta;
			}
			else {
				isUpLeft = false;
				moveLeft = false;
			}
		}
		else {
			if (y <= 1.5) {
				y += 1.5 * delta;
			}
			else {
				isUpLeft = true;
			}
		}
		x -= 2.45f * delta;
		modelMatrix *= Transform3D::Translate(x, y, pos.z);
		modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
		glm::vec4 coords = modelMatrix * glm::vec4(0, 0, 0, 1);
		//salveaza coordonatele actuale ale jucatorului

		player.setActualCoords(coords);

		//RenderSimpleMesh(meshes["sphere"], shaders["basicShader"], modelMatrix, glm::vec3(0.545, 0.000, 0.545), NULL);
		RenderMeshTex(meshes["sphere"], shaders["textureShader"], modelMatrix, mapTextures["marmura"]);

		firstPersonCamPosition = glm::mat4(1);
		firstPersonCamPosition *= Transform3D::Translate(coords.x, coords.y + 0.15f, coords.z - 0.75f);

		targetPosition = firstPersonCamPosition;
		targetPosition *= Transform3D::Translate(0.f, 0.f, -2.0f);

		if (!thirdPersonCam) {
			camera->Set(firstPersonCamPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
				targetPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
				glm::vec3(0, 1, 0));
		}
	}

	else if (moveRight == true) {
		glm::mat4 modelMatrix = glm::mat4(1);
		glm::vec3 pos = player.getActualPlayerCoords();
		float y = pos.y;
		float x = pos.x;
		if (isUpRight == true) {
			if (y >= 0.80) {
				y -= 1.5 * delta;
			}
			else {
				isUpRight = false;
				moveRight = false;
			}
		}
		else {
			if (y <= 1.5f) {
				y += 1.5 * delta;
			}
			else {
				isUpRight = true;
			}
		}
		x += 2.45f * delta;
		modelMatrix *= Transform3D::Translate(x, y, pos.z);
		modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
		glm::vec4 coords = modelMatrix * glm::vec4(0, 0, 0, 1);
		//salveaza coordonatele actuale ale jucatorului

		player.setActualCoords(coords);
		//RenderSimpleMesh(meshes["sphere"], shaders["basicShader"], modelMatrix, glm::vec3(0.545, 0.000, 0.545), NULL);
		RenderMeshTex(meshes["sphere"], shaders["textureShader"], modelMatrix, mapTextures["marmura"]);

		firstPersonCamPosition = glm::mat4(1);
		firstPersonCamPosition *= Transform3D::Translate(coords.x, coords.y + 0.15f, coords.z - 0.75f);
		targetPosition = firstPersonCamPosition;
		targetPosition *= Transform3D::Translate(0.f, 0.f, -2.0f);

		if (!thirdPersonCam)
		{
			camera->Set(firstPersonCamPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
				targetPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
				glm::vec3(0, 1, 0));
		}
	}
}

//inainte de a incepe jocul este redata o platforma de start
void Tema3::LoadStartPlatform() 
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0, 0, -2.0f + translateZ);
	modelMatrix *= Transform3D::Scale(8.5f, 0.25f, 7.0f);
	glm::vec3 position = modelMatrix * glm::vec4(0, 0, 0, 1);
	startPlatformZ = position.z;
	RenderMeshTex(meshes["box"], shaders["textureShader"], modelMatrix, mapTextures["lemn"]);
}

// verifica coliziunea jucatorului cu platforma cu indicele index
bool Tema3::checkCollision(int index) {
	bool ok = false;
	glm::vec4 playerCoord = player.getActualPlayerCoords();
	//coordonata pe directia OZ a platformei cu indexul dat
	float i;
	//stabileste coordonatele suprafetei platformei
	float max_step = platforms->getPlatformPos(index) + platforms->getPlatformSize(index);
	float step = platforms->getPlatformPos(index);
	float zCoord;
	float xCoord;
	float yCoord;

	
	for (i = step; i < max_step; i += 0.01f) {
		float zCoord = i;
		//verifica daca punctul cubului cel mai apropiat de sfera este plasat in interiorul sferei
		xCoord = platforms->getPlatformXCoord(index);
		yCoord = platforms->getPlatformYCoord(index);
		float distance = sqrt(pow((xCoord - playerCoord.x), 2) + pow((yCoord - playerCoord.y), 2) + pow((zCoord - playerCoord.z), 2));

		if (distance <= 1) { //coliziune
			ok = true;
		}
	}
	return ok;
}

void Tema3::LoadObstacles(float time) {
	if (start == true && fallingPlayer == false) {
		translateObstacles += 7.5f * time;
	}
	
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(1.65f, 0.25, -5 + translateObstacles);
	modelMatrix *= Transform3D::Scale(1.95f, 0.65f, 0.35f);
	obstaclesPos[0] = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
	RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["lemn"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-3.45f, 0.25, -20 + translateObstacles);
	modelMatrix *= Transform3D::Scale(1.95f, 0.65f, 0.35f);
	obstaclesPos[1] = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
	RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["pietreGri"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(1.65f, 0.25, -35 + translateObstacles);
	modelMatrix *= Transform3D::Scale(1.95f, 0.65f, 0.35f);
	obstaclesPos[2] = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
	RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["pietreGri"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-3.45f, 0.25, -35 + translateObstacles);
	modelMatrix *= Transform3D::Scale(1.95f, 0.65f, 0.35f);
	obstaclesPos[3] = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
	RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["lemn"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(1.65f, 0.25, -15 + translateObstacles);
	modelMatrix *= Transform3D::Scale(1.95f, 0.65f, 0.35f);
	obstaclesPos[4] = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
	RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["caramida"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-3.45f, 0.25, -45 + translateObstacles);
	modelMatrix *= Transform3D::Scale(1.95f, 0.65f, 0.35f);
	obstaclesPos[5] = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
	RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["metal"]);
	
	if (translateObstacles >= 55) {
		translateObstacles = -28;
		//reinitializeaza coliziunile
		int i;
		for (i = 0; i < 6; i++) {
			collideCheckValues[i] = false;
		}
	}
	
}

void Tema3::LoadDecorElements(float deltaTimeSeconds) {
	//recalculeaza valorile pentru translatii
	int i;
	for (i = 0; i <6; i++) {
		if (fallingPlayer == false && start == true) {
			translateRightDecorValues[i] += 7.5f * deltaTimeSeconds;
		}

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(3.85f, 0.5, -18 + translateRightDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.75, 0.75, 0.75);
		modelMatrix *= Transform3D::RotateOX(1.57);
		modelMatrix *= Transform3D::RotateOY(1.57);
		modelMatrix *= Transform3D::RotateOZ(1.57);
		RenderMeshTex(meshes["piramida"], shaders["textureShader"], modelMatrix, mapTextures["floare4"]);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(3.85f, 0.5, -13 + translateRightDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.75, 0.75, 0.75);
		modelMatrix *= Transform3D::RotateOX(1.57);
		modelMatrix *= Transform3D::RotateOY(1.57);
		modelMatrix *= Transform3D::RotateOZ(1.57);
		RenderMeshTex(meshes["piramida"], shaders["textureShader"], modelMatrix, mapTextures["floare1"]);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(3.85f, 0.5, -4 + translateRightDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.75, 0.75, 0.75);
		modelMatrix *= Transform3D::RotateOX(1.57);
		modelMatrix *= Transform3D::RotateOY(1.57);
		modelMatrix *= Transform3D::RotateOZ(1.57);
		RenderMeshTex(meshes["piramida"], shaders["textureShader"], modelMatrix, mapTextures["floare2"]);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(3.85f, 0.5, -1 + translateRightDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.75, 0.75, 0.75);
		modelMatrix *= Transform3D::RotateOX(1.57);
		modelMatrix *= Transform3D::RotateOY(1.57);
		modelMatrix *= Transform3D::RotateOZ(1.57);
		RenderMeshTex(meshes["piramida"], shaders["textureShader"], modelMatrix, mapTextures["floare3"]);


		//frunze pt fiecare floare
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(3.85f, 0.5, -17.95 + translateRightDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.75, 0.75, 0.75);
		modelMatrix *= Transform3D::RotateOY(1.57);
		modelMatrix *= Transform3D::RotateOZ(1.57);
		modelMatrix *= Transform3D::RotateOX(1.57);
		RenderMeshTex(meshes["patrat"], shaders["textureShader"], modelMatrix, mapTextures["leaves"]);


		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(3.85f, 0.5, -12.95 + translateRightDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.75, 0.75, 0.75);
		modelMatrix *= Transform3D::RotateOY(1.57);
		modelMatrix *= Transform3D::RotateOZ(1.57);
		modelMatrix *= Transform3D::RotateOX(1.57);
		RenderMeshTex(meshes["patrat"], shaders["textureShader"], modelMatrix, mapTextures["leaves"]);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(3.85f, 0.5, -3.95 + translateRightDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.75, 0.75, 0.75);
		modelMatrix *= Transform3D::RotateOY(1.57);
		modelMatrix *= Transform3D::RotateOZ(1.57);
		modelMatrix *= Transform3D::RotateOX(1.57);
		RenderMeshTex(meshes["patrat"], shaders["textureShader"], modelMatrix, mapTextures["leaves"]);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(3.85f, 0.5, -0.95 + translateRightDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.75, 0.75, 0.75);
		modelMatrix *= Transform3D::RotateOY(1.57);
		modelMatrix *= Transform3D::RotateOZ(1.57);
		modelMatrix *= Transform3D::RotateOX(1.57);
		RenderMeshTex(meshes["patrat"], shaders["textureShader"], modelMatrix, mapTextures["leaves"]);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(3.95f, 0, -5 + translateRightDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.25, 1.5f, 0.25);

		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		if (position.z - 9 < 6.55f) {
			RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["copac"]);
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(3.55f, 1.5f, -5.25f + translateRightDecorValues[i]);
			modelMatrix *= Transform3D::Scale(1, 1, 1);
			RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["grass"]);
		}
		else {
			translateRightDecorValues[i] = -29;
		}
	}

	for (i = 0; i < 6; i++) {
		if (fallingPlayer == false && start == true) {
			translateLeftDecorValues[i] += 7.5f * deltaTimeSeconds;
		}

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(-3.95f, 0.5, -19 + translateLeftDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.75, 0.75, 0.75);
		modelMatrix *= Transform3D::RotateOX(1.57);
		modelMatrix *= Transform3D::RotateOY(1.57);
		modelMatrix *= Transform3D::RotateOZ(1.57);
		RenderMeshTex(meshes["piramida"], shaders["textureShader"], modelMatrix, mapTextures["floare1"]);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(-3.95f, 0.5, -14 + translateLeftDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.75, 0.75, 0.75);
		modelMatrix *= Transform3D::RotateOX(1.57);
		modelMatrix *= Transform3D::RotateOY(1.57);
		modelMatrix *= Transform3D::RotateOZ(1.57);
		RenderMeshTex(meshes["piramida"], shaders["textureShader"], modelMatrix, mapTextures["floare2"]);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(-3.95f, 0.5, -8 + translateLeftDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.75, 0.75, 0.75);
		modelMatrix *= Transform3D::RotateOX(1.57);
		modelMatrix *= Transform3D::RotateOY(1.57);
		modelMatrix *= Transform3D::RotateOZ(1.57);
		RenderMeshTex(meshes["piramida"], shaders["textureShader"], modelMatrix, mapTextures["floare4"]);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(-3.95f, 0.5, -1 + translateLeftDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.75, 0.75, 0.75);
		modelMatrix *= Transform3D::RotateOX(1.57);
		modelMatrix *= Transform3D::RotateOY(1.57);
		modelMatrix *= Transform3D::RotateOZ(1.57);
		RenderMeshTex(meshes["piramida"], shaders["textureShader"], modelMatrix, mapTextures["floare3"]);


		//frunze pt fiecare floare
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(-3.95f, 0.5, -18.95 + translateLeftDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.75, 0.75, 0.75);
		modelMatrix *= Transform3D::RotateOY(1.57);
		modelMatrix *= Transform3D::RotateOZ(1.57);
		modelMatrix *= Transform3D::RotateOX(1.57);
		RenderMeshTex(meshes["patrat"], shaders["textureShader"], modelMatrix, mapTextures["leaves"]);


		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(-3.95f, 0.5, -13.95 + translateLeftDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.75, 0.75, 0.75);
		modelMatrix *= Transform3D::RotateOY(1.57);
		modelMatrix *= Transform3D::RotateOZ(1.57);
		modelMatrix *= Transform3D::RotateOX(1.57);
		RenderMeshTex(meshes["patrat"], shaders["textureShader"], modelMatrix, mapTextures["leaves"]);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(-3.95f, 0.5, -7.95 + translateLeftDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.75, 0.75, 0.75);
		modelMatrix *= Transform3D::RotateOY(1.57);
		modelMatrix *= Transform3D::RotateOZ(1.57);
		modelMatrix *= Transform3D::RotateOX(1.57);
		RenderMeshTex(meshes["patrat"], shaders["textureShader"], modelMatrix, mapTextures["leaves"]);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(-3.95f, 0.5, -0.95 + translateLeftDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.75, 0.75, 0.75);
		modelMatrix *= Transform3D::RotateOY(1.57);
		modelMatrix *= Transform3D::RotateOZ(1.57);
		modelMatrix *= Transform3D::RotateOX(1.57);
		RenderMeshTex(meshes["patrat"], shaders["textureShader"], modelMatrix, mapTextures["leaves"]);


		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(-4.25f, 0, -5 + translateLeftDecorValues[i]);
		modelMatrix *= Transform3D::Scale(0.25, 1.5f, 0.25);

		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		if (position.z - 18 < 6.55f) {
			RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["copac"]);
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(-4.65f, 1.5f, -5.25f + translateLeftDecorValues[i]);
			modelMatrix *= Transform3D::Scale(1, 1, 1);
			RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["grass"]);
		}
		else {
			translateLeftDecorValues[i] = -26;
		}
	}
}

void Tema3::LoadCollectionItem(float time) {
	if (giftIsJumping == true) {//deplasare in sus pe OY pt cuburi
		jumpingCube += 0.75f * time;
		if (jumpingCube >= jumpMaxHeight) {
			giftIsJumping = false;
		}
	}
	else {
		jumpingCube -= 0.75f * time;
		if (jumpingCube < 0.10f) {
			giftIsJumping = true;
		}
	}
	rotAngle += 0.75f * time;
	if (start == true && fallingPlayer == false) {
		int i;
		for (i = 0; i < 7; i++) {
			translateCollectionItem[i] += 7.5f * time;
			if (collectionItemsPos[i].z >= 10) {
				translateCollectionItem[i] = -20;
				collectionCheckValues[i] = false;
			}
		}
	}
	//el. colectabil 0
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-2.65f, 0.75f, -8.25f + translateCollectionItem[0]);
	modelMatrix *= Transform3D::Scale(0.75, 0.65, 0.65);
	modelMatrix *= Transform3D::RotateOY(rotAngle);
	glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
	collectionItemsPos[0] = position;
	itemBoundingSpheres[0] = collectionItemsPos[0];
	itemRadius[0] = 0.65f;
	if (collectionCheckValues[0] == false) {
		RenderMeshTex(meshes["piramida"], shaders["textureShader"], modelMatrix, mapTextures["gheata"]);
	}
	

	//el. colectabil 1
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(2.85f, 0.75f, -8.25f + translateCollectionItem[1]);
	modelMatrix *= Transform3D::Scale(0.75, 0.65, 0.65);
	modelMatrix *= Transform3D::RotateOY(-rotAngle);
	position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
	collectionItemsPos[1] = position;
	itemRadius[1] = 0.65f;
	itemBoundingSpheres[1] = collectionItemsPos[1];
	if (collectionCheckValues[1] == false) {
		RenderMeshTex(meshes["piramida"], shaders["textureShader"], modelMatrix, mapTextures["aur"]);
	}
	

	//el. colectabil 2
	//forma formata din 2 piramide unite
	//piramida de sus
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(2.85f, 0.75f, -18.25f + translateCollectionItem[2]);
	modelMatrix *= Transform3D::Scale(0.75, 0.65, 0.65);
	modelMatrix *= Transform3D::RotateOY(-rotAngle);
	position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
	collectionItemsPos[2] = position;
	itemRadius[2] = 0.65f;
	itemBoundingSpheres[2] = collectionItemsPos[2];
	if (collectionCheckValues[2] == false) {
		RenderMeshTex(meshes["piramida"], shaders["textureShader"], modelMatrix, mapTextures["aur"]);
	}
	//piramida de jos
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(2.85f, 0.10f, -18.25f + translateCollectionItem[2]);
	modelMatrix *= Transform3D::Scale(0.75, 0.65, 0.65);
	modelMatrix *= Transform3D::RotateOY(-rotAngle);

	modelMatrix *= Transform3D::RotateOX(1.57);
	modelMatrix *= Transform3D::RotateOY(1.57);
	modelMatrix *= Transform3D::RotateOZ(1.57);
	if (collectionCheckValues[2] == false) {
		RenderMeshTex(meshes["piramida"], shaders["textureShader"], modelMatrix, mapTextures["aur"]);
	}


	//el. colectabil 3
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-2.65f, jumpingCube, -18.25f + translateCollectionItem[3]);

	modelMatrix *= Transform3D::Scale(0.65, 0.65, 0.65);
	position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
	collectionItemsPos[3] = position;
	itemBoundingSpheres[3] = collectionItemsPos[3];
	itemRadius[3] = 0.65f;
	if (collectionCheckValues[3] == false) {
		RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["cadou"]);
	}

	//el. colectabil 4
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(2.65f, jumpingCube, -10.25f + translateCollectionItem[4]);
	modelMatrix *= Transform3D::Scale(0.65, 0.65, 0.65);
	position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
	collectionItemsPos[4] = position;
	itemBoundingSpheres[4] = collectionItemsPos[4];
	itemRadius[4] = 0.65f;
	if (collectionCheckValues[4] == false) {
		RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["cadou1"]);
	}

	//el. colectabil 5
	//format din 2 piramide
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.25f, 0.75f, -20.25f + translateCollectionItem[5]);
	modelMatrix *= Transform3D::Scale(0.75, 0.65, 0.65);
	modelMatrix *= Transform3D::RotateOY(-rotAngle);
	position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
	collectionItemsPos[5] = position;
	itemBoundingSpheres[5] = collectionItemsPos[5];
	itemRadius[5] = 0.65f;
	if (collectionCheckValues[5] == false) {
		RenderMeshTex(meshes["piramida"], shaders["textureShader"], modelMatrix, mapTextures["gheata"]);
	}
	//piramida de jos
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.25f, 0.10f, -20.25f + translateCollectionItem[5]);
	modelMatrix *= Transform3D::Scale(0.75, 0.65, 0.65);
	modelMatrix *= Transform3D::RotateOY(-rotAngle);

	modelMatrix *= Transform3D::RotateOX(1.57);
	modelMatrix *= Transform3D::RotateOY(1.57);
	modelMatrix *= Transform3D::RotateOZ(1.57);
	if (collectionCheckValues[5] == false) {
		RenderMeshTex(meshes["piramida"], shaders["textureShader"], modelMatrix, mapTextures["gheata"]);
	}

	//el. colectabil 6
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-0.5f, jumpingCube, -30.25f + translateCollectionItem[6]);
	modelMatrix *= Transform3D::Scale(0.65, 0.65, 0.65);
	position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
	collectionItemsPos[6] = position;
	itemBoundingSpheres[6] = collectionItemsPos[6];
	itemRadius[6] = 0.65f;
	if (collectionCheckValues[6] == false) {
		RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["cadou1"]);
	}
}

//verifica coliziunea jucatorul cu elementul colectabil de indice index
bool Tema3::CheckCollectionItemCollision(int index) {
	bool ok = false;
	glm::vec3 itemPos = itemBoundingSpheres[index];
	glm::vec3 playerPos = player.getActualPlayerCoords();
	float distance = sqrt(pow((playerPos.x - itemPos.x), 2) + pow((playerPos.y - itemPos.y), 2) + pow((playerPos.z - itemPos.z), 2));
	
	//distance < suma razelor
	if (distance < 0.5 + itemRadius[index]) {
		ok = true; //coliziune
	}
	return ok;
}

//verifica coliziunile cu elementele colectabile
void Tema3::CheckCollectionItems() {
	int i;
	for (i = 0; i < 7; i++) {
		if (collectionCheckValues[i] == false) {
			if (CheckCollectionItemCollision(i) == true) {
				cout << "Coliziune cu colectabilul de indice " << i << endl;
				collectionCheckValues[i] = true; //retine coliziunile cu elementul colectabil
				scorValue += 10;
			}
		}
	}
}

void Tema3::LoadScoreInfo() {
	glm::mat4 modelMatrix1 = glm::mat4(1);
	
	modelMatrix1 *= Transform3D::Translate(scoreInfoPos.x, scoreInfoPos.y, scoreInfoPos.z);
	modelMatrix1 *= Transform3D::Scale(1.25f, 0.25f, 1.25f);
	glm::mat4 modelMatrix = glm::mat4(1);
	
	modelMatrix *= Transform3D::Translate(scoreInfoPos.x + 1.25f * scorValue / maxScore / 2 - 1.25f / 2, scoreInfoPos.y, scoreInfoPos.z);
	modelMatrix *= Transform3D::Scale(1.25f * scorValue / maxScore, 0.25f, 1.25f);
	
	RenderMeshTex(meshes["patrat"], shaders["textureShader"], modelMatrix, mapTextures["gold1"]);
	RenderMeshTex(meshes["patrat"], shaders["textureShader"], modelMatrix1, mapTextures["silver"]);
}

void Tema3::LoadLamps(float time) {
	//right lamps
	int i;
	for (i = 0; i < 6; i++) {
		if (fallingPlayer == false && start == true) {
			translateRightLampsValues[i] += 7.5f * time;
		}
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(3.65f, 1.85f, -0.5f + translateRightLampsValues[i]);
		modelMatrix *= Transform3D::Scale(0.9, 0.5f, 0.9);
		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		if (position.z < 6.55f) {
			RenderMeshTex(meshes["tetraedru"], shaders["textureShader"], modelMatrix, mapTextures["tabla"]);
		}
		else {
			translateRightLampsValues[i] = -110;
		}

		RightLampsPos[i] = position.z;
		

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(3.95f, 0, -0.15f + translateRightLampsValues[i]);
		modelMatrix *= Transform3D::Scale(0.10, 1.85f, 0.10);
		position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		if (position.z < 2.5f) {
			RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["metal"]);
		}
	}
	//left lamps
	for (i = 0; i < 6; i++) {
		if (fallingPlayer == false && start == true) {
			translateLeftLampsValues[i] += 7.5f * time;
		}
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(-4.65f, 1.85f, -0.5f + translateLeftLampsValues[i]);
		modelMatrix *= Transform3D::Scale(0.9, 0.5f, 0.9);
		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		if (position.z < 6.55f) {
			RenderMeshTex(meshes["tetraedru"], shaders["textureShader"], modelMatrix, mapTextures["tabla"]);
		}
		LeftLampsPos[i] = position.z;

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(-4.05f, 0, -0.15f + translateLeftLampsValues[i]);
		modelMatrix *= Transform3D::Scale(0.10, 1.85f, 0.10);
		position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
		if (position.z < 6.55f) {
			RenderMeshTex(meshes["cub_stilizat"], shaders["textureShader"], modelMatrix, mapTextures["metal"]);
		}
		else {
			translateLeftLampsValues[i] = -120.5;
		}
	}
}

void Tema3::Update(float deltaTimeSeconds)
{
	spotLightPositions[0] = glm::vec3(3.65f, 1.85, RightLampsPos[1]);//0
	spotLightPositions[1] = glm::vec3(3.65f, 1.85, RightLampsPos[4]);//-1
	spotLightPositions[2] = glm::vec3(3.65f, 1.85, RightLampsPos[3]);//-2
	spotLightPositions[3] = glm::vec3(3.65f, 1.85, RightLampsPos[5]);



	spotLightDirections[0] = glm::vec3(3.25f, 0, RightLampsPos[1]);
	spotLightDirections[1] = glm::vec3(3.25f, 0, RightLampsPos[4]);
	spotLightDirections[2] = glm::vec3(3.25f, 0, RightLampsPos[3]);
	spotLightDirections[3] = glm::vec3(3.25f, 0, RightLampsPos[5]);


	LoadObstacles(deltaTimeSeconds);
	LoadDecorElements(deltaTimeSeconds);
	LoadLamps(deltaTimeSeconds);
	LoadCollectionItem(deltaTimeSeconds);
	LoadStars(); //reprezinta vietile jucatorului
	CheckObstacleCollisions();
	CheckCollectionItems();
	LoadScoreInfo();
	//pozitie "soare" -3.45f, 3.75f, -0.05f
	pointLightPositions[0] = glm::vec3(-3.55f, 3.25, -0.05f);
	

	//s-au irosit toate vietile
	if (start == true) {
		if (lives <= 0) {
			cout << "Ai pierdut toate vietile" << endl;
			start = false;
		}
	}
	

	if (fallingPlayer == true) {
		//glClearColor(1.000, 0.855, 0.725, 1);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		LoadDecorElements(deltaTimeSeconds);
		LoadLamps(deltaTimeSeconds);
		LoadObstacles(deltaTimeSeconds);
		LoadCollectionItem(deltaTimeSeconds);
		LoadStars(); //vietile jucatorului
	}
	
	if (start == true && fallingPlayer == false) {
		int i;
		for (i = 0; i < 15; i++) {
			platforms->setTranslateVal(platforms->getTranslateVal(i) + 7.5f * deltaTimeSeconds, i);
		}
		if (play == false) { //calculeaza translateZ doar daca platforma de start inca mai este redata in scena
			translateZ += 7.5f * deltaTimeSeconds;
		}
	}

	if (translateZ < 7) {
		LoadStartPlatform();
	}
	else { //daca s-a deplasat suficient cat sa nu mai fie vizibila in scena, platforma de start nu mai este redata
		play = true;
	}

	LoadPlatforms();
	LoadPlayer(deltaTimeSeconds);

	int i;
	bool checkPlayerPos = true;
	for (i = 0; i < 12; i++) {
		if (checkCollision(i) == true) {
			checkPlayerPos = false;
			collideCheck = true;
		}
	}
	/*
	if (fuelValue <= 0) { //verifica numarul de vieiti la terminarea combustibilului
		if (lives == 0) {
			gameOver = true;
			Engine::GetWindow()->Close();
			cout << "Combustibil epuizat :(" << endl;
		}
		else {
			fuelValue = initialFuelValue; //reinitializeaza combustibilul
			lives--;
		}
	}*/

	//renderFuelInformation(deltaTimeSeconds);
	//daca jucatorul nu are coliziune cu nicio platforma iar jocul nu este in nicio situatie speciala
	//(inainte de start joc, saritura la apasarea space, deplasare stanga-dreapta), atunci este 'activata' animatia de cadere a mingii
	if (checkPlayerPos == true) {
		collideCheck = false; //nicio coliziune
		if (jumping == false && moveLeft == false && moveRight == false) {
			if (play == true) {
				fallingPlayer = true;
				//cout << "Nicio coliziune detectata" << endl;
			}
		}
	}
	if (renderCameraTarget) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
	}
	sunAngle += 0.5f * deltaTimeSeconds;
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-3.55f, 3.45, -0.05f); //=point light position
	modelMatrix *= Transform3D::RotateOY(sunAngle);
	modelMatrix *= Transform3D::Scale(0.60, 0.60, 0.60);
	RenderSimpleMesh(meshes["sfera"], shaders["basicShader"], modelMatrix, glm::vec3(), mapTextures["sun"]);
}

void Tema3::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema3::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
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


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
	// move the camera only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
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
	if (window->KeyHold(GLFW_KEY_W)) { //mareste viteza de deplasare a platformelor 
		if (orangeAbility == false) {
			int i;
			for (i = 0; i < 9; i++) {
				platforms->setTranslateVal(platforms->getTranslateVal(i) + 1.75 * deltaTime, i);
			}
		}
	}

	if (window->KeyHold(GLFW_KEY_S)) { //scade viteza de deplasare a platformelor
		if (orangeAbility == false) {
			int i;
			for (i = 0; i < 9; i++) {
				platforms->setTranslateVal(platforms->getTranslateVal(i) - 1.25f * deltaTime, i);
			}
		}
	}
}

void Tema3::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_T){
		renderCameraTarget = !renderCameraTarget;
	}

	if (key == GLFW_KEY_C){
		if (!thirdPersonCam) {
			camera->Set(thirdPersonCamPosition * glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
		}
		thirdPersonCam = !thirdPersonCam;
	}

	if (key == GLFW_KEY_A) {//deplaseaza jucatorul
		moveLeft = true;
	}

	if (key == GLFW_KEY_D) {
		moveRight = true;
	}

	if (key == GLFW_KEY_SPACE){
		jumping = true;
	}

	if (key == GLFW_KEY_X) {
		start = true;
		startGame = true;
	}
}

void Tema3::OnKeyRelease(int key, int mods)
{
	
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
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
			if (!thirdPersonCam) {
				camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
				camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
			}
		}
		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			renderCameraTarget = true;
			// TODO : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			if (thirdPersonCam) {
				camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
				camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
			}
		}

	}
}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema3::OnWindowResize(int width, int height)
{
}