#include "Laborator4.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"

using namespace std;

#define CUBE_MAX_Y 2.000000f
#define CUBE_MAX_ANG 180.000000f
#define PI 3.141592f

#define SUN_EARTH_DISTANCE_X 3.000000f
#define SUN_EARTH_DISTANCE_Z 1.000000f
#define MOON_EARTH_DISTANCE_X 1.500000f
#define MOON_EARTH_DISTANCE_Z 0.500000f

float cubeTransX = 0;
float cubeTransY = 0;
float cubeAng = 0;
int cubeInc = 1;

float solarSystemX = 4.000000f;
float solarSystemY = 4.000000f;
float solarSystemZ = 4.000000f;

float sunAng = 0;
float earthAng = 0;
float moonAng = 0;

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

	// Create a mesh square using custom data
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(0, 0,  0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(0.5f, 0,  0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(0, 0.5f,  0), glm::vec3(1, 1, 1))
		};

		vector<unsigned short> indices =
		{
			0, 2, 1,	// indices for first triangle
		};

		meshes["arrow"] = new Mesh("generated arrow");
		meshes["arrow"]->InitFromData(vertices, indices);

		// Create a new mesh from buffer data
		Mesh* square = CreateMesh("arrow", vertices, indices);
	}
}

Mesh* Laborator4::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// TODO: Create the VBO and bind it
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	// TODO: Create the IBO and bind it
	unsigned int IBO = 0;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

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
	glBindVertexArray(0);
	glDeleteBuffers(1, &IBO);
	glDeleteBuffers(1, &VBO);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	return meshes[name];
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

void Laborator4::Bonus1JumpAndFlip(float deltaTimeSeconds)
{
	cubeAng -= deltaTimeSeconds / 100.0 * CUBE_MAX_ANG;
	cubeTransX += deltaTimeSeconds;
	cubeTransY += deltaTimeSeconds / 100.0;

	if (cubeInc) {
		cubeTransY += deltaTimeSeconds;
		if (cubeTransY > CUBE_MAX_Y) {
			cubeTransY = CUBE_MAX_Y;
			cubeInc = 0;
		}
	}
	else {
		cubeTransY -= deltaTimeSeconds;
		if (cubeTransY < 0.000000f) {
			cubeTransY = 0.000000f;
			cubeInc = 1;
		}
	}

	// cube
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(2.500000f, 0.500000f, 1.500000f);
	modelMatrix *= Transform3D::Translate(cubeTransX, cubeTransY, 0);
	modelMatrix *= Transform3D::RotateOZ(cubeAng);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
	// first arrow
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(2.500000f, 0.500000f, 1.500000f);
	modelMatrix *= Transform3D::Translate(cubeTransX, cubeTransY, 0);
	modelMatrix *= Transform3D::Translate(0, 0, -0.51f);
	modelMatrix *= Transform3D::RotateOZ(PI / 4);
	modelMatrix *= Transform3D::RotateOZ(cubeAng);
	RenderMesh(meshes["arrow"], shaders["VertexColor"], modelMatrix);
	// second arrow
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(2.500000f, 0.500000f, 1.500000f);
	modelMatrix *= Transform3D::Translate(cubeTransX, cubeTransY, 0);
	modelMatrix *= Transform3D::Translate(0, 0, 0.51f);
	modelMatrix *= Transform3D::RotateOZ(PI / 4);
	modelMatrix *= Transform3D::RotateOZ(cubeAng);
	RenderMesh(meshes["arrow"], shaders["VertexColor"], modelMatrix);
}

void Laborator4::Bonus2SolarSys(float deltaTimeSeconds)
{
	sunAng += deltaTimeSeconds;
	earthAng += deltaTimeSeconds * 5;
	moonAng += deltaTimeSeconds * 10;
	solarSystemX += deltaTimeSeconds / 5;
	solarSystemY += deltaTimeSeconds / 5;
	solarSystemZ += deltaTimeSeconds / 5;

	// sun
	modelMatrix = glm::mat4(1);

	modelMatrix *= Transform3D::Translate(solarSystemX, solarSystemY, solarSystemZ);
	modelMatrix *= Transform3D::RotateOY(sunAng);

	modelMatrix *= Transform3D::Translate(0.000000f, 1.000000f, -1.000000f);
	modelMatrix *= Transform3D::Scale(2.000000f, 2.000000f, 2.000000f);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	// earth
	modelMatrix = glm::mat4(1);

	modelMatrix *= Transform3D::Translate(solarSystemX, solarSystemY, solarSystemZ);
	modelMatrix *= Transform3D::RotateOY(sunAng);

	modelMatrix *= Transform3D::Translate(SUN_EARTH_DISTANCE_X, 0, SUN_EARTH_DISTANCE_Z);
	modelMatrix *= Transform3D::RotateOY(earthAng);

	modelMatrix *= Transform3D::Translate(0, 0.500000f + 0.500000f, 0);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	// moon
	modelMatrix = glm::mat4(1);

	modelMatrix *= Transform3D::Translate(solarSystemX, solarSystemY, solarSystemZ);
	modelMatrix *= Transform3D::RotateOY(sunAng);

	modelMatrix *= Transform3D::Translate(SUN_EARTH_DISTANCE_X, 0, SUN_EARTH_DISTANCE_Z);
	modelMatrix *= Transform3D::RotateOY(earthAng);

	modelMatrix *= Transform3D::Translate(MOON_EARTH_DISTANCE_X, 0, MOON_EARTH_DISTANCE_Z);
	modelMatrix *= Transform3D::RotateOY(moonAng);

	modelMatrix *= Transform3D::Translate(0, 0.250000f + 0.500000f + 0.250000f, -1);
	modelMatrix *= Transform3D::Scale(0.500000f, 0.500000f, 0.500000f);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}

void Laborator4::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-2.5f, 0.5f,-1.5f);
	modelMatrix *= Transform3D::Translate(translateX, translateY, translateZ);
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

	Bonus1JumpAndFlip(deltaTimeSeconds);

	Bonus2SolarSys(deltaTimeSeconds);
	
}

void Laborator4::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator4::OnInputUpdate(float deltaTime, int mods)
{
	// TODO
	int speed = 10;
	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		if (window->KeyHold(GLFW_KEY_S)) {
			translateZ += speed * deltaTime;
		}
		if (window->KeyHold(GLFW_KEY_W)) {
			translateZ -= speed * deltaTime;
		}
		if (window->KeyHold(GLFW_KEY_R)) {
			translateY += speed * deltaTime;
		}
		if (window->KeyHold(GLFW_KEY_F)) {
			translateY -= speed * deltaTime;
		}
		if (window->KeyHold(GLFW_KEY_A)) {
			translateX -= speed * deltaTime;
		}
		if (window->KeyHold(GLFW_KEY_D)) {
			translateX += speed * deltaTime;
		}
	}

	if (window->KeyHold(GLFW_KEY_1)) {
		scaleX += deltaTime;
		scaleY += deltaTime;
		scaleZ += deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_2)) {
		if (scaleX > 0 && scaleY > 0 && scaleZ > 0) {
			scaleX -= deltaTime;
			scaleY -= deltaTime;
			scaleZ -= deltaTime;
		}
		if (scaleX < 0 && scaleY < 0 && scaleZ < 0) {
			scaleX = 0;
			scaleY = 0;
			scaleZ = 0;
		}
	}
	if (window->KeyHold(GLFW_KEY_3)) {
		angularStepOX += deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_4)) {
		angularStepOX -= deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_5)) {
		angularStepOY += deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_6)) {
		angularStepOY -= deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_7)) {
		angularStepOZ += deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_8)) {
		angularStepOZ -= deltaTime;
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
