#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>     // srand, rand
#include <time.h>       // time

#include <Core/Engine.h>

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{

	camera = new CameraTema2::Camera(glm::vec3(sphereX, THIRD_PERSON_CAMERA_Y, THIRD_PERSON_CAMERA_Z));
	camera->RotateThirdPerson_OX(RADIANS(INITIAL_ROTATION_ANGLE));
	projectionMatrix = glm::perspective(RADIANS(FOV), window->props.aspectRatio, Z_NEAR, Z_FAR);

	InitPlatforms();

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a simple cube mesh
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.2)),
			VertexFormat(glm::vec3(1, -1,  1), glm::vec3(1, 0, 1), glm::vec3(0.9, 0.4, 0.2)),
			VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1, 0, 0), glm::vec3(0.7, 0.7, 0.1)),
			VertexFormat(glm::vec3(1,  1,  1), glm::vec3(0, 1, 0), glm::vec3(0.7, 0.3, 0.7)),
			VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), glm::vec3(0.3, 0.5, 0.4)),
			VertexFormat(glm::vec3(1, -1, -1), glm::vec3(0, 1, 1), glm::vec3(0.5, 0.2, 0.9)),
			VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 1, 0), glm::vec3(0.7, 0.0, 0.7)),
			VertexFormat(glm::vec3(1,  1, -1), glm::vec3(0, 0, 1), glm::vec3(0.1, 0.5, 0.8)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
			2, 3, 7,		2, 7, 6,
			1, 7, 3,		1, 5, 7,
			6, 7, 4,		7, 5, 4,
			0, 4, 1,		1, 4, 5,
			2, 6, 4,		0, 2, 4,
		};

		CreateMesh("cube", vertices, indices);
	}

	// Create a simple square mesh
	{
		std::vector<VertexFormat> vertices =
		{
			VertexFormat(glm::vec3(-0.500000f, -0.500000f, 0)),
			VertexFormat(glm::vec3(0.500000f, -0.500000f, 0)),
			VertexFormat(glm::vec3(0.500000f, 0.500000f, 0)),
			VertexFormat(glm::vec3(-0.500000f, 0.500000f, 0))
		};

		std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

		CreateMesh("square", vertices, indices);
	}

	// Create a simple triangle mesh
	{
		std::vector<VertexFormat> vertices =
		{
			VertexFormat(glm::vec3(-.5f, -1, 0)),
			VertexFormat(glm::vec3(.5f, 0, 0)),
			VertexFormat(glm::vec3(-.5f, 1, 0)),
		};

		std::vector<unsigned short> indices = { 0, 1, 2};

		CreateMesh("triangle", vertices, indices);
	}

	// Create a simple circle mesh
	{
		std::vector<VertexFormat> vertices;
		std::vector<unsigned short> indices;
		// center of circle
		vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), BISQUE));
		for (int i = 0; i < CIRCLE_VERTICES + 1; i++) {
			// create vertex
			double x = cos(RADIANS(360.0f * i / CIRCLE_VERTICES));
			double y = sin(RADIANS(360.0f * i / CIRCLE_VERTICES));
			vertices.push_back(VertexFormat(glm::vec3(y, x, 0), BISQUE));
			// create triangle
			if (i < CIRCLE_VERTICES - 1) {
				indices.push_back(0);
				indices.push_back((unsigned short)(i + 1));
				indices.push_back((unsigned short)(i + 2));
			}
			else {
				indices.push_back(0);
				indices.push_back(CIRCLE_VERTICES);
				indices.push_back(1);
			}
		}
		CreateMesh("circle", vertices, indices);
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("NormalShaderTema2");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShaderNormal.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Create a shader program for drawing face polygon with the color specified by uniform variable
	{
		Shader* shader = new Shader("ColorShaderTema2");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShaderColor.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Create a shader program for drawing a 2D polygon with the color specified by uniform variable
	{
		Shader* shader = new Shader("Shader2DTema2");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader2D.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

void Tema2::InitPlatforms() {
	// initialize random seed
	srand(GetTickCount());
	for (int i = 0; i < NO_GROUPS; i++) {
		platformsGroups[i].distance = -i * PLATFORM_MAX_DIM_Z;
		GenerateRandomGroupOfPlatforms(i);
		if (i == 0) {
			platformsGroups[i].colors[NO_COLUMNS / 2 - 1] = PURPLE;
		}
	}
}

void Tema2::GenerateRandomGroupOfPlatforms(int i) {
	for (int j = 0; j < NO_COLUMNS; j += 2) {
		// we should always have at least one blue platform in a 2-group
		char offset = rand() % 2;
		char bluePlatIdx = j + offset;
		char otherPlatIdx = j + (1 - offset);
		platformsGroups[i].colors[bluePlatIdx] = BLUE;
		platformsGroups[i].lengths[bluePlatIdx] = (PLATFORM_MAX_DIM_Z - PLATFORM_MAX_FREE_SPACE_Z) + rand() % PLATFORM_MAX_FREE_SPACE_Z;
		// we generate the other platform 
		// which doesn't exist or it has a different color
		GeneratePlatformEasyLevel(i, otherPlatIdx);
		switch (difficulty)
		{
		case easy:
			GeneratePlatformEasyLevel(i, otherPlatIdx);
			break;
		case medium:
			GeneratePlatformMediumLevel(i, otherPlatIdx);
			break;
		case hard:
			GeneratePlatformHardLevel(i, otherPlatIdx);
			break;
		case pro:
			GeneratePlatformProLevel(i, otherPlatIdx);
			break;
		}
		platformsGroups[i].lengths[otherPlatIdx] = (PLATFORM_MAX_DIM_Z - PLATFORM_MAX_FREE_SPACE_Z) + rand() % PLATFORM_MAX_FREE_SPACE_Z;
	}
}

void Tema2::GeneratePlatformEasyLevel(int i, int otherPlatIdx) {
	bool exists = rand() % 2;
	if (exists) {
		char color = rand() % 10;
		if (color == 0)
			platformsGroups[i].colors[otherPlatIdx] = RED;
		if (1 <= color && color <= 3)
			platformsGroups[i].colors[otherPlatIdx] = YELLOW;
		if (4 <= color && color <= 6)
			platformsGroups[i].colors[otherPlatIdx] = ORANGE;
		if (7 <= color && color <= 8)
			platformsGroups[i].colors[otherPlatIdx] = GREEN;
		if (color == 9)
			platformsGroups[i].colors[otherPlatIdx] = BISQUE;
	}
	else {
		platformsGroups[i].colors[otherPlatIdx] = MISSING_PLATFORM;
	}
}

void Tema2::GeneratePlatformMediumLevel(int i, int otherPlatIdx) {
	bool exists = rand() % 2;
	if (exists) {
		char color = rand() % 20;
		if (0 <= color && color <= 7)
			platformsGroups[i].colors[otherPlatIdx] = RED;
		if (7 <= color && color <= 11)
			platformsGroups[i].colors[otherPlatIdx] = YELLOW;
		if (12 <= color && color <= 14)
			platformsGroups[i].colors[otherPlatIdx] = ORANGE;
		if (15 <= color && color <= 17)
			platformsGroups[i].colors[otherPlatIdx] = GREEN;
		if (18 <= color && color <= 19)
			platformsGroups[i].colors[otherPlatIdx] = BISQUE;
	}
	else {
		platformsGroups[i].colors[otherPlatIdx] = MISSING_PLATFORM;
	}
}

void Tema2::GeneratePlatformHardLevel(int i, int otherPlatIdx) {
	int exists = rand() % 4;
	if (exists == 0) {
		char color = rand() % 20;
		if (0 <= color && color <= 9)
			platformsGroups[i].colors[otherPlatIdx] = RED;
		if (10 <= color && color <= 13)
			platformsGroups[i].colors[otherPlatIdx] = YELLOW;
		if (14 <= color && color <= 15)
			platformsGroups[i].colors[otherPlatIdx] = ORANGE;
		if (16 <= color && color <= 17)
			platformsGroups[i].colors[otherPlatIdx] = GREEN;
		if (18 <= color && color <= 19)
			platformsGroups[i].colors[otherPlatIdx] = BISQUE;
	}
	else {
		platformsGroups[i].colors[otherPlatIdx] = MISSING_PLATFORM;
	}
}

void Tema2::GeneratePlatformProLevel(int i, int otherPlatIdx) {
	int exists = rand() % 4;
	if (exists == 0) {
		char color = rand() % 20;
		if (0 <= color && color <= 11)
			platformsGroups[i].colors[otherPlatIdx] = RED;
		if (12 <= color && color <= 13)
			platformsGroups[i].colors[otherPlatIdx] = YELLOW;
		if (14 <= color && color <= 15)
			platformsGroups[i].colors[otherPlatIdx] = ORANGE;
		if (16 <= color && color <= 17)
			platformsGroups[i].colors[otherPlatIdx] = GREEN;
		if (18 <= color && color <= 19)
			platformsGroups[i].colors[otherPlatIdx] = BISQUE;
	}
	else {
		platformsGroups[i].colors[otherPlatIdx] = MISSING_PLATFORM;
	}
}


void Tema2::ReinitializeValues() {
	platformsSpeed = minSpeed;
	fuel = FUEL_MAX;
	sphereX = -0.5f;
	sphereY = 0;
	onJump = false;
	onRise = false;
	falling = false;
	speedRun = false;
	speedRunSeconds = SPEED_RUN_SECS;
	onPowerUps = 0;
}

void Tema2::DifficultyUpdate() {
	double timeElapsed = Engine::GetElapsedTime() - timeElapsedBeforeStart;
	if (timeElapsed > PRO_DIFF_TIME) {
		difficulty = pro;
		fuelSensitivity = FUEL_SENSITIVITY_PRO;
		minSpeed = PLATFORMS_MIN_SPEED_PRO;
	}
	else if (timeElapsed > HARD_DIFF_TIME) {
		difficulty = hard;
		fuelSensitivity = FUEL_SENSITIVITY_HARD;
		minSpeed = PLATFORMS_MIN_SPEED_HARD;
	}
	else if (timeElapsed > MEDIUM_DIFF_TIME) {
		difficulty = medium;
		fuelSensitivity = FUEL_SENSITIVITY_MEDIUM;
		minSpeed = PLATFORMS_MIN_SPEED_MEDIUM;
	}
	if (platformsSpeed < minSpeed) {
		platformsSpeed = minSpeed;
	}
}

void Tema2::SpeedRunUpdate(float deltaTimeSeconds) {
	if (speedRun) {
		speedRunSeconds -= deltaTimeSeconds;
		if (speedRunSeconds < 0) {
			speedRun = false;
			onPowerUps = 0;
		}
	}
}

void Tema2::ScoreUpdate(float deltaTimeSeconds) {
	if (!speedRun) {
		score += (int)(SCORE_SENSITIVITY * platformsSpeed * deltaTimeSeconds);
	}
	else {
		score += (int)(SCORE_SENSITIVITY * PLATFORMS_SPEED_RUN_VALUE * deltaTimeSeconds);
	}
	cout << "Score: " << score << "\n";
}

void Tema2::FallingUpdate(float deltaTimeSeconds) {
	sphereY -= deltaTimeSeconds;
	if (sphereY < -ANIMATION_FALLING_TIME) {
		lifes--;
		ReinitializeValues();
		InitPlatforms();
	}
}

void Tema2::CameraUpdate() {
	if (cameraPerson) {
		camera->SetPosition(glm::vec3(sphereX, THIRD_PERSON_CAMERA_Y, THIRD_PERSON_CAMERA_Z));	
	}
	else {
		camera->SetPosition(glm::vec3(sphereX, FIRST_PERSON_CAMERA_Y + sphereY, FIRST_PERSON_CAMERA_Z));
	}
}

void Tema2::JumpUpdate(float deltaTimeSeconds) {
	if (onJump && onRise) {
		sphereY += JUMP_SENSITIVITY * deltaTimeSeconds;
		if (sphereY > RISE_TIME) {
			onRise = false;
			sphereY = RISE_TIME;
		}
	}
	else if (onJump && !onRise) {
		sphereY -= JUMP_SENSITIVITY * deltaTimeSeconds;
		if (sphereY <= 0) {
			onJump = false;
			sphereY = 0;
		}
	}
}

void Tema2::UpdateAndRenderPlatforms(float deltaTimeSeconds) {
	bool alive = false;
	// initialize random seed
	srand(GetTickCount());
	for (int i = 0; i < NO_GROUPS; i++) {
		if (!speedRun) {
			platformsGroups[i].distance += platformsSpeed * deltaTimeSeconds;
		}
		else {
			platformsGroups[i].distance += PLATFORMS_SPEED_RUN_VALUE * deltaTimeSeconds;
		}
		// if group of platforms becomes invisible, we reset the values for it
		if (platformsGroups[i].distance > MAX_Z + PLATFORM_MAX_DIM_Z) {
			platformsGroups[i].distance -= NO_GROUPS * PLATFORM_MAX_DIM_Z;
			GenerateRandomGroupOfPlatforms(i);
		}

		for (int j = 0; j < NO_COLUMNS; j++) {
			// we make sure that the sphere is on a platform and it has PURPLE color
			if (!onJump && !falling
				&& j - NO_COLUMNS / 2 + (1 - PLATFORM_SCALE_X * 2) / 2 <= sphereX && sphereX <= j - NO_COLUMNS / 2 + 1 - (1 - PLATFORM_SCALE_X * 2) / 2
				&& platformsGroups[i].colors[j] != MISSING_PLATFORM && platformsGroups[i].distance >= 0 && 0 >= platformsGroups[i].distance - platformsGroups[i].lengths[j]) {
				if (platformsGroups[i].colors[j] == RED) {
					lifes--;
				}
				else if (platformsGroups[i].colors[j] == ORANGE) {
					speedRun = true;
					speedRunSeconds = SPEED_RUN_SECS;
					onPowerUps = 1;
				}
				else if (platformsGroups[i].colors[j] == YELLOW) {
					fuel -= FUEL_LOSS;
				}
				else if (platformsGroups[i].colors[j] == GREEN) {
					fuel += FUEL_GAIN;
					if (fuel > FUEL_MAX) {
						fuel = FUEL_MAX;
					}
				}
				else if (platformsGroups[i].colors[j] == BISQUE) {
					if (lifes < LIFES_MAX_NO) {
						lifes++;
					}
				}
				alive = true;
				platformsGroups[i].colors[j] = PURPLE;
			}
			// we render the platforms
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(FIRST_PLATFORM_OFFSET_X + j, -PLATFORM_SCALE_Y, -PLATFORM_SCALE_Z * platformsGroups[i].lengths[j] + platformsGroups[i].distance));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(PLATFORM_SCALE_X, PLATFORM_SCALE_Y, PLATFORM_SCALE_Z * platformsGroups[i].lengths[j]));
			if (platformsGroups[i].colors[j] != MISSING_PLATFORM) {
				RenderSimpleMesh(meshes["cube"], shaders["ColorShaderTema2"], modelMatrix, platformsGroups[i].colors[j]);
			}
		}
	}

	if (!alive) {
		falling = true;
	}

	if (onJump) {
		falling = false;
	}
}

void Tema2::RenderSphere() {
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(sphereX, SPHERE_SCALE * SPHERE_SCALE + sphereY, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(SPHERE_SCALE));
	RenderSimpleMesh(meshes["sphere"], shaders["NormalShaderTema2"], modelMatrix, BLUE);
}

void Tema2::RenderFuelBar() {
	// we render the quantity of fuel with green color
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(SQUARE_OFFSET_X - SQUARE_SCALE_X * (1 - fuel / FUEL_MAX) / 2, SQUARE_OFFSET_Y, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(SQUARE_SCALE_X * fuel / FUEL_MAX, SQUARE_SCALE_Y, 0));
	RenderSimpleMesh(meshes["square"], shaders["Shader2DTema2"], modelMatrix, GREEN);

	// white background
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(SQUARE_OFFSET_X, SQUARE_OFFSET_Y, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(SQUARE_SCALE_X, SQUARE_SCALE_Y, 0));
	RenderSimpleMesh(meshes["square"], shaders["Shader2DTema2"], modelMatrix, WHITE);
}

void Tema2::RenderLifes() {
	for (int i = 0; i < lifes; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-i * SPACE_BEETWEEN_CIRCLES + CIRCLES_OFFSET_X, CIRCLES_OFFSET_Y, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(CIRCLE_SCALE_X, CIRCLE_SCALE_Y, 0));
		RenderSimpleMesh(meshes["circle"], shaders["Shader2DTema2"], modelMatrix, BISQUE);
	}
}

void Tema2::RenderPlayButton() {
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(PLAY_BUTTON_TRIANGLE_SCALE));
	RenderSimpleMesh(meshes["triangle"], shaders["Shader2DTema2"], modelMatrix, SILVER);

	modelMatrix = glm::mat4(1);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(PLAY_BUTTON_CIRCLE_SCALE / window->props.aspectRatio, PLAY_BUTTON_CIRCLE_SCALE, 0));
	RenderSimpleMesh(meshes["circle"], shaders["Shader2DTema2"], modelMatrix, STEEL_BLUE);
}

Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
{
	unsigned int VAO = 0;
	// Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// Create the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

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

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
	return meshes[name];
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
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
		UpdateAndRenderPlatforms(deltaTimeSeconds);
		RenderSphere();
	
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

void Tema2::FrameEnd()
{

}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	GLint modelLocation = glGetUniformLocation(shader->GetProgramID(), "Model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	if (strcmp(shader->GetName(), "NormalShaderTema2") == 0 || strcmp(shader->GetName(), "ColorShaderTema2") == 0) {
		GLint viewLocation = glGetUniformLocation(shader->GetProgramID(), "View");
		glm::mat4 viewMatrix = camera->GetViewMatrix();
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

		GLint projLocation = glGetUniformLocation(shader->GetProgramID(), "Projection");
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}

	if (strcmp(shader->GetName(), "NormalShaderTema2") == 0) {
		GLint timeLocation = glGetUniformLocation(shader->GetProgramID(), "SpeedRunTime");
		glUniform1f(timeLocation, speedRunSeconds);

		GLint onPowerUpsLocation = glGetUniformLocation(shader->GetProgramID(), "OnPowerUps");
		glUniform1i(onPowerUpsLocation, onPowerUps);

		GLint fallingLocation = glGetUniformLocation(shader->GetProgramID(), "falling");
		glUniform1i(fallingLocation, falling);
	}

	if (strcmp(shader->GetName(), "ColorShaderTema2") == 0 || strcmp(shader->GetName(), "Shader2DTema2") == 0) {
		GLint colorLocation = glGetUniformLocation(shader->GetProgramID(), "Color");
		glUniform3fv(colorLocation, 1, glm::value_ptr(color));
	
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

float Tema2::Distance(float xA, float yA, float xB, float yB)
{
	return sqrt((xA - xB) * (xA - xB) + (yA - yB) * (yA - yB));
}

bool Tema2::PointCircleCollision(float pointX, float pointY, float circleCenterX, float circleCenterY, float circleRadius)
{
	return Distance(pointX, pointY, circleCenterX, circleCenterY) < circleRadius;
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (started) {
		if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
		{
			if (!falling && !speedRun) {
				if (window->KeyHold(GLFW_KEY_W)) {
					platformsSpeed += PLATFORMS_SPEED_SENSITIVITY * deltaTime;
					if (platformsSpeed > PLATFORMS_MAX_SPEED) {
						platformsSpeed = PLATFORMS_MAX_SPEED;
					}
				}

				if (window->KeyHold(GLFW_KEY_S)) {
					platformsSpeed -= PLATFORMS_SPEED_SENSITIVITY * deltaTime;
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

void Tema2::OnKeyPress(int key, int mods)
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

void Tema2::OnKeyRelease(int key, int mods)
{
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	glm::ivec2 res = window->GetResolution();
	float mouseLogicX = -1 + 2 * 1.0f * mouseX / (res.x - 1);
	float mouseLogicY = (-1 + 2 * 1.0f * (res.y - 1 - mouseY) / (res.y - 1)) * 1 / window->props.aspectRatio;

	if (!started && PointCircleCollision(mouseLogicX, mouseLogicY, 0, 0, 0.1f)) {
		started = true;
		timeElapsedBeforeStart = Engine::GetElapsedTime();
	}
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
