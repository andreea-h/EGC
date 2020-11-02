#include "Laborator2.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Laborator2::Laborator2()
{
}

Laborator2::~Laborator2()
{

}

void Laborator2::Init()
{
	cullFace = GL_BACK;
	polygonMode = GL_FILL;

	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a mesh box using custom data
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1)),
			VertexFormat(glm::vec3(0, -1, 1), glm::vec3(0.541, 0.169, 0.886)),
			VertexFormat(glm::vec3(-1, 0, 1), glm::vec3(0.502, 0.000, 0.502)),
			VertexFormat(glm::vec3(0, 0, 1), glm::vec3(0.416, 0.353, 0.804)),
			VertexFormat(glm::vec3(-1, -1, 0), glm::vec3(0.545, 0.000, 0.545)),
			VertexFormat(glm::vec3(0, -1, 0), glm::vec3(0.933, 0.510, 0.93)),
			VertexFormat(glm::vec3(-1, 0, 0), glm::vec3(0.867, 0.627, 0.867)),
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.294, 0.000, 0.510)),
			// TODO: Complete the information for the cube
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,	// indices for first triangle
			1, 3, 2,	// indices for second triangle
			// TODO: Complete indices data
			2, 3, 7,
			2, 7, 6, 
			1, 7, 3,
			1, 5, 7,
			6, 7, 4,
			7, 5, 4,
			0, 4, 1,
			1, 4, 5,
			2, 6, 4,
			0, 2, 4
		};

		//"cube1" este mesh creat prin definirea vertices si indices
	/*	meshes["cube1"] = new Mesh("generated cube 1");
		meshes["cube1"]->InitFromData(vertices, indices);
		*/

		Mesh* cube1 = CreateMesh("cube1", vertices, indices);

		// Create a new mesh from buffer data
		Mesh *cube = CreateMesh("cube3", vertices, indices);
	}

	{
		vector<VertexFormat> vertices_tetraedru
		{
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.125, 0.698, 0.667)),
			VertexFormat(glm::vec3(0.5f, 0, 0.86f), glm::vec3(0.275, 0.510, 0.706)),
			VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0.498, 1.000, 0.831)),
			VertexFormat(glm::vec3(0.5f, 1, 0.28f), glm::vec3(0.000, 0.000, 1.000))
		};

		vector<unsigned short> indices_tetraedru
		{
			0, 1, 3,
			1, 2, 3,
			0, 2, 1,
			0, 3, 2
		};

		Mesh* tetraedru = CreateMesh("tetraedru", vertices_tetraedru, indices_tetraedru);
	}

	{
		vector<VertexFormat> vertices_patrat
		{
			
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0)),
			VertexFormat(glm::vec3(1, 0, 0), glm::vec3(1.000, 0.855, 0.725)),
			VertexFormat(glm::vec3(1, 1, 0), glm::vec3(1.000, 0.843, 0.000)),
			VertexFormat(glm::vec3(0, 1, 0), glm::vec3(1.000, 0.271, 0.000))
		};

		vector<unsigned short> indices_patrat
		{
			0, 3, 2, //clockwise
			0, 1, 2 //counter clockwise (from initial viewer's direction)
		};

		Mesh* patrat = CreateMesh("patrat", vertices_patrat, indices_patrat);	
	}

	{
		vector<VertexFormat> vertices_culori
		{
			VertexFormat(glm::vec3(0, 0, 1), glm::vec3(1, 0, 0)), //0
			VertexFormat(glm::vec3(1, 0, 1), glm::vec3(1, 0, 0)), //1
			VertexFormat(glm::vec3(0, 1, 1), glm::vec3(1, 0, 0)), //2
			VertexFormat(glm::vec3(1, 1, 1), glm::vec3(1, 0, 0)), //3
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.824, 0.412, 0.118)), //4
			VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0, 0, 1)), //5
			VertexFormat(glm::vec3(0, 1, 0), glm::vec3(1, 1, 0)), //6
			VertexFormat(glm::vec3(1, 1, 0), glm::vec3(1, 1, 0)), //7

			VertexFormat(glm::vec3(0, 0, 1), glm::vec3(1, 0, 1)), //8
			VertexFormat(glm::vec3(1, 0, 1), glm::vec3(0, 0, 1)), //9
			VertexFormat(glm::vec3(0, 1, 1), glm::vec3(1, 1, 0)), //10
			VertexFormat(glm::vec3(1, 1, 1), glm::vec3(1, 1, 0)), //11
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0, 1)), //12
			VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0.824, 0.412, 0.118)), //13
			VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0.824, 0.412, 0.118)), //14
			VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0, 0, 1)), //15

			VertexFormat(glm::vec3(0, 0, 1), glm::vec3(0.416, 0.353, 0.804)), //16
			VertexFormat(glm::vec3(1, 0, 1), glm::vec3(1, 0, 1)), //17
			VertexFormat(glm::vec3(0, 1, 1), glm::vec3(0.416, 0.353, 0.804)), //18
			VertexFormat(glm::vec3(1, 1, 1), glm::vec3(0, 0, 1)), //19
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.416, 0.353, 0.804)), //20
			VertexFormat(glm::vec3(1, 0, 0), glm::vec3(1, 0, 1)), //21
			VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0.416, 0.353, 0.804)), //22
			VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0.824, 0.412, 0.118)), //23
		};
		
		vector<unsigned short> indices_culori = 
		{
			0, 1, 2,	//culoarea (1, 0, 0)
			1, 3, 2,	
			
			10, 11, 7, //culoarea (1, 1, 0)
			10, 7, 6, 

			9, 15, 19, //culoarea (0, 0, 1)
			9, 5, 15,

			14, 23, 4, //culoarea (0.824, 0.412, 0.118)
			23, 13, 4,

			8, 12, 17, // culoarea (1, 1, 0)
			17, 12, 21,

			18, 22, 20,  //culoarea (0.416, 0.353, 0.804)
			16, 18, 20
		};

		Mesh* cub_culori = CreateMesh("cub_culori", vertices_culori, indices_culori);
	}

	//creare sfera
	{
		vector<VertexFormat> vertices_sfera;
		vector<unsigned short> indices_sfera;

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
		
		int i;
		for (i = 0; i <= nr_latitudinal; i++) {
			u = pi / 2 - (pi * i / nr_latitudinal);
			
			for (int j = 0; j <= nr_longitudinal; j++) {
				v = 2 * pi * j / nr_longitudinal;

				x = r * cosf(u) * cosf(v);
				y = r * cosf(u) * sinf(v);
				z = r * sinf(u);

				if (j % 2 == 0) {
					vertices_sfera.push_back(VertexFormat(glm::vec3(x, y, z), glm::vec3(0.000, 0.000, 0.545))); //culoarea darkblue
				}
				else {
					vertices_sfera.push_back(VertexFormat(glm::vec3(x, y, z), glm::vec3(1.000, 0.714, 0.757))); //culoare lightpink
				}
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

			for (int j = 0; j < nr_longitudinal; j++)	{
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
		Mesh* sfera = CreateMesh("sfera", vertices_sfera, indices_sfera);
	}
}

Mesh* Laborator2::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
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

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data
	// It will be learned later, when GLSL shaders will be introduced
	// For the moment just think that each property value from our vertex format needs to be send to a certain channel
	// in order to know how to receive it in the GLSL vertex shader

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

void Laborator2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator2::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	// TODO: Enable face culling
	glEnable(GL_CULL_FACE);

	// TODO: Set face custom culling. Use the "cullFace" variable
	glCullFace(cullFace);

	//glFrontFace(GL_CCW);

	// render an object using face normals for color
	RenderMesh(meshes["box"], shaders["VertexNormal"], glm::vec3(0, 0.5f, -1.5f), glm::vec3(0.75f));

	// render an object using colors from vertex
	RenderMesh(meshes["cube1"], shaders["VertexColor"], glm::vec3(-1.5f, 0, 3.5f), glm::vec3(1.25f));
	//cube1 este creat folosind "InitFromData()"
	
	RenderMesh(meshes["cube3"], shaders["VertexColor"], glm::vec3(3.5f, 0, 2.5f));

	//adauga tetraedru 
	RenderMesh(meshes["tetraedru"], shaders["VertexColor"], glm::vec3(4, 1, 1), glm::vec3(2));

	RenderMesh(meshes["patrat"], shaders["VertexColor"], glm::vec3(0, 0, 0));

	RenderMesh(meshes["cub_culori"], shaders["VertexColor"], glm::vec3(7, 0, 0));

	RenderMesh(meshes["sfera"], shaders["VertexColor"], glm::vec3(11, 0, 0), glm::vec3(1.5f));

	// TODO: Disable face culling
	glDisable(GL_CULL_FACE);
}

void Laborator2::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator2::OnInputUpdate(float deltaTime, int mods)
{

}

void Laborator2::OnKeyPress(int key, int mods)
{
	// TODO: switch between GL_FRONT and GL_BACK culling
	// Save the state in "cullFace" variable and apply it in the Update() method not here

	if (key == GLFW_KEY_F2)
	{
		if (cullFace == GL_BACK) {
			cullFace = GL_FRONT;
		}
		else {
			cullFace = GL_BACK;
		}
	}

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

void Laborator2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator2::OnWindowResize(int width, int height)
{
}
