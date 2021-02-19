#include "Laborator8.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Laborator8::Laborator8()
{
}

Laborator8::~Laborator8()
{
}

void Laborator8::Init()
{
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
		Mesh* mesh = new Mesh("plane");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "plane50.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShaderLab8");
		shader->AddShader("Source/Laboratoare/Laborator8/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator8/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	//Light & material properties
	{
		lightPosition = glm::vec3(0, 1, 1);
		lightDirection = glm::vec3(0, -1, 0);
		materialShininess = 30;
		//materialKe = glm::vec3(0.15, 0.15, 0.15);
		//materialKa = glm::vec3(0.1, 0.1, 0.9);
		//materialKd = glm::vec3(0.8, 0.3, 0.5);
		//materialKs = glm::vec3(0.2, 0.5, 0.2);
		lightSourceIntensity = glm::vec3(0.9, 0.9, 0.9);
		ambientLightIntensity = glm::vec3(0.8, 0.8, 0.8);
		light_mode = 0;
		cut_off_angle = 45.0f;
		angle_OX = 0.0f;
		angle_OY = 0.0f;
	}
}

void Laborator8::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);	
}

void Laborator8::Update(float deltaTimeSeconds)
{
	{
		//materialKe = glm::vec3(0.15, 0.15, 0.15);
		//materialKa = glm::vec3(0.1, 0.1, 0.9);
		//materialKd = glm::vec3(0.8, 0.3, 0.5);
		//materialKs = glm::vec3(0.2, 0.5, 0.2);
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab8"], modelMatrix, glm::vec3(0.15, 0.15, 0.9), glm::vec3(0.9, 0.1, 0.1), glm::vec3(0.8, 0.3, 0.5), glm::vec3(0.2, 0.5, 0.2));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab8"], modelMatrix, glm::vec3(0.5), glm::vec3(0.1, 0.6, 0.2), glm::vec3(0.4, 0.05, 0.5), glm::vec3(0.9, 0.3, 0.6));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 1, 0));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab8"], modelMatrix, glm::vec3(0), glm::vec3(0.6, 0.1, 0.1), glm::vec3(0.1, 0.8, 0.2), glm::vec3(0.9, 0.9, 0.1));
	}

	// Render ground
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01f, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
		RenderSimpleMesh(meshes["plane"], shaders["ShaderLab8"], modelMatrix, glm::vec3(0.15), glm::vec3(0.1, 0.1, 0.9), glm::vec3(0.8, 0.1, 0.1), glm::vec3(0.2, 0.6, 0.2));
	}

	// Render the point light in the scene
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, lightPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
	}
}

void Laborator8::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator8::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, glm::vec3 materialKe, glm::vec3 materialKa, glm::vec3 materialKd, glm::vec3 materialKs)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform
	int light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

	int light_direction = glGetUniformLocation(shader->program, "light_direction");
	glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_ke = glGetUniformLocation(shader->program, "material_ke");
	glUniform3f(material_ke, materialKe.r, materialKe.g, materialKe.b);

	int material_ka = glGetUniformLocation(shader->program, "material_ka");
	glUniform3f(material_ka, materialKa.r, materialKa.g, materialKa.b);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform3f(material_kd, materialKd.r, materialKd.g, materialKd.b);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform3f(material_ks, materialKs.r, materialKs.g, materialKs.b);

	int light_source_intensity = glGetUniformLocation(shader->program, "light_source_intensity");
	glUniform3f(light_source_intensity, lightSourceIntensity.r, lightSourceIntensity.g, lightSourceIntensity.b);

	int ambient_light_intensity = glGetUniformLocation(shader->program, "ambient_light_intensity");
	glUniform3f(ambient_light_intensity, ambientLightIntensity.r, ambientLightIntensity.g, ambientLightIntensity.b);

	//int object_color = glGetUniformLocation(shader->program, "object_color");
	//glUniform3f(object_color, color.r, color.g, color.b);

	int light_mode_loc = glGetUniformLocation(shader->program, "light_mode");
	glUniform1i(light_mode_loc, light_mode);

	int cut_off_angle_loc = glGetUniformLocation(shader->program, "cut_off_angle");
	glUniform1f(cut_off_angle_loc, cut_off_angle);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator8::OnInputUpdate(float deltaTime, int mods)
{
	float speed = 2;

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

		glm::mat4 spot_rotation = glm::rotate(glm::mat4(1), angle_OY, glm::vec3(0, 1, 0));
		spot_rotation = glm::rotate(spot_rotation, angle_OX, glm::vec3(1, 0, 0));
		lightDirection = glm::vec3(spot_rotation * glm::vec4(glm::vec3(0, -1, 0), 0));

		// Control light position using on W, A, S, D, E, Q
		if (window->KeyHold(GLFW_KEY_W)) lightPosition -= forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_A)) lightPosition -= right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_S)) lightPosition += forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_D)) lightPosition += right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_E)) lightPosition += up * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_Q)) lightPosition -= up * deltaTime * speed;
		
		// BONUS
		// Control spotlight rotation angles
		if (window->KeyHold(GLFW_KEY_UP)) angle_OX += deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_DOWN)) angle_OX -= deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_LEFT)) angle_OY += deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_RIGHT)) angle_OY -= deltaTime * speed;
		// Control spotlight cutoff angle
		if (window->KeyHold(GLFW_KEY_1)) cut_off_angle += deltaTime * 10 * speed;
		if (window->KeyHold(GLFW_KEY_2)) cut_off_angle -= deltaTime * 10 * speed;
	}
}

void Laborator8::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_F)
	{
		light_mode = !light_mode;
	}
}

void Laborator8::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator8::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator8::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator8::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator8::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator8::OnWindowResize(int width, int height)
{
}
