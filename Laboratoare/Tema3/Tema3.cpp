#include "Tema3.h"

#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>     // srand, rand
#include <time.h>       // time

#include <Core/Engine.h>

using namespace std;

Tema3::Tema3()
{
}

Tema3::~Tema3()
{
}

void Tema3::Init()
{
	InitMeshes();
	InitTextures();
	InitShaders();
	InitObjects();

	camera = new CameraTema3::Camera(glm::vec3(sphereX, THIRD_PERSON_CAMERA_Y, THIRD_PERSON_CAMERA_Z));
	camera->RotateThirdPerson_OX(RADIANS(INITIAL_ROTATION_ANGLE));
	projectionMatrix = glm::perspective(RADIANS(FOV), window->props.aspectRatio, Z_NEAR, Z_FAR);

}

void Tema3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::Update(float deltaTimeSeconds)
{
	if (started) {
		DifficultyUpdate();
		SpeedRunUpdate(deltaTimeSeconds);
	
		if (falling) {
			FallingUpdate(deltaTimeSeconds);
		} else {
			fuel -= fuelSensitivity * deltaTimeSeconds;
			ScoreUpdate(deltaTimeSeconds);
		}
	
		CameraUpdate();
		JumpUpdate(deltaTimeSeconds);
		UpdateAndRenderObjects(deltaTimeSeconds);
		RenderSphere();
	
		RenderScore();
		if (fuel > 0) {
			RenderFuelBar();
		}
		else {
			lifes--;
			fuel = FUEL_MAX;
		}
	
		if (lifes > 0) {
			RenderLifes();
		}
		else {
			exit(0);
		}

	}
	else {
		RenderPlayButton();
	}
}

void Tema3::FrameEnd()
{

}

void Tema3::OnInputUpdate(float deltaTime, int mods)
{
	if (started) {
		if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
		{
			if (!falling && !speedRun) {
				if (window->KeyHold(GLFW_KEY_W)) {
					platformsSpeed += SPEED_SENSITIVITY * deltaTime;
					if (platformsSpeed > PLATFORMS_MAX_SPEED) {
						platformsSpeed = PLATFORMS_MAX_SPEED;
					}
				}

				if (window->KeyHold(GLFW_KEY_S)) {
					platformsSpeed -= SPEED_SENSITIVITY * deltaTime;
					if (platformsSpeed < minSpeed) {
						platformsSpeed = minSpeed;
					}
				}
			}

			if (window->KeyHold(GLFW_KEY_A)) {
				sphereX -= SPHERE_SENSITIVITY_X * deltaTime;
			}

			if (window->KeyHold(GLFW_KEY_D)) {
				sphereX += SPHERE_SENSITIVITY_X * deltaTime;
			}

		}
	}
}

void Tema3::OnKeyPress(int key, int mods)
{
	if (!started && key == GLFW_KEY_ENTER) {
		started = true;
		timeElapsedBeforeStart = Engine::GetElapsedTime();
	}

	if (started) {
		if (key == GLFW_KEY_C) {
			cameraPerson = !cameraPerson;
		}

		if (!falling && !onJump && key == GLFW_KEY_SPACE) {
			onJump = true;
			onRise = true;
		}

	}
}

void Tema3::OnKeyRelease(int key, int mods)
{
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	glm::ivec2 res = window->GetResolution();
	float mouseLogicX = -1 + 2 * 1.0f * mouseX / (res.x - 1);
	float mouseLogicY = (-1 + 2 * 1.0f * (res.y - 1 - mouseY) / (res.y - 1)) * 1 / window->props.aspectRatio;

	if (!started && PointCircleCollision(mouseLogicX, mouseLogicY, 0, 0, 0.1f)) {
		started = true;
		timeElapsedBeforeStart = Engine::GetElapsedTime();
	}
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema3::OnWindowResize(int width, int height)
{
}
