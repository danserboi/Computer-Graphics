#include "Laborator3.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

// number of iterations for increasing/decreasing values
#define NO_ITER 40

using namespace std;

// 1, if we need to increase translateX, translateY, scaleX, scaleY
// 0, otherwise
int inc;
// current iteration
int iter;

// square center coordinates
float cx;
float cy;

// car coordinates
float carX = 0;
float carY = 0;
// car center coordinates
float carCenterX = 50;
float carCenterY = 50;
// car orientation
float carAng = 0;

Laborator3::Laborator3()
{
}

Laborator3::~Laborator3()
{
}

void Laborator3::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	float squareSide = 100;

	// compute coordinates of square center
	cx = corner.x + squareSide / 2;
	cy = corner.y + squareSide / 2;
	
	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;
	
	// initialize angularStep
	angularStep = 0;
	
	inc = 1;
	iter = 0;

	 Mesh* square1 = Object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
	AddMeshToList(square1);
	
	Mesh* square2 = Object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0), true);
	AddMeshToList(square2);

	Mesh* square3 = Object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1), true);
	AddMeshToList(square3);

}

void Laborator3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator3::Update(float deltaTimeSeconds)
{
	if (inc == 1)
		iter++;
	else
		iter--;
	
	// TODO: update steps for translation, rotation, scale, in order to create animations
	
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(10, 10);
	
	// TODO: create animations by multiplying current transform matrix with matrices from Transform 2D
	if (inc == 1) {
		translateX += deltaTimeSeconds * 100;
		translateY += deltaTimeSeconds * 100;
	}
	else {
		translateX -= deltaTimeSeconds * 100;
		translateY -= deltaTimeSeconds * 100;
	}
	modelMatrix *= Transform2D::Translate(translateX, translateY);


	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(400, 250);

	//TODO create animations by multiplying current transform matrix with matrices from Transform 2D
	if (inc == 1) {
		scaleX += deltaTimeSeconds;
		scaleY += deltaTimeSeconds;
	}
	else {
		scaleX -= deltaTimeSeconds;
		scaleY -= deltaTimeSeconds;
	}
	modelMatrix *= Transform2D::Translate(cx, cy) * Transform2D::Scale(scaleX, scaleY) * Transform2D::Translate(-cx, -cy);

	RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(650, 250);

	//TODO create animations by multiplying current transform matrix with matrices from Transform 2D
	angularStep += deltaTimeSeconds;
	modelMatrix *= Transform2D::Translate(cx, cy) * Transform2D::Rotate(angularStep) * Transform2D::Translate(-cx, -cy);

	RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);

	if (inc == 1 && iter == NO_ITER) {
		inc = 0;
	}

	if (inc == 0 && iter == 0) {
		inc = 1;
	}

	// car body
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(carCenterX, carCenterY) * Transform2D::Rotate(carAng) * Transform2D::Translate(-carCenterX, -carCenterY);
	modelMatrix *= Transform2D::Translate(carX, carY);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	// car front
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(carCenterX, carCenterY) * Transform2D::Rotate(carAng) * Transform2D::Translate(-carCenterX, -carCenterY);
	modelMatrix *= Transform2D::Translate(carX, carY);
	modelMatrix *= Transform2D::Translate(100, 0);
	modelMatrix *= Transform2D::Scale(0.3, 1);
	RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
}

void Laborator3::FrameEnd()
{

}

void Laborator3::OnInputUpdate(float deltaTime, int mods)
{
	int speed = 300;
	// rotate left
	if (window->KeyHold(GLFW_KEY_A)) {
		carAng += 2 * deltaTime;
	}
	// rotate right
	if (window->KeyHold(GLFW_KEY_D)) {
		carAng -= 2 * deltaTime;
	}
	// run
	if (window->KeyHold(GLFW_KEY_S)) {
		carX -= cos(carAng) * speed * deltaTime;
		carCenterX -= cos(carAng) * speed * deltaTime;
		carY -= sin(carAng) * speed * deltaTime;
		carCenterY -= sin(carAng) * speed * deltaTime;
	}
	// marche arriere
	if (window->KeyHold(GLFW_KEY_W)) {
		carX += cos(carAng) * speed * deltaTime;
		carCenterX += cos(carAng) * speed * deltaTime;
		carY += sin(carAng) * speed * deltaTime;
		carCenterY += sin(carAng) * speed * deltaTime;
	}
}

void Laborator3::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Laborator3::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator3::OnWindowResize(int width, int height)
{
}
