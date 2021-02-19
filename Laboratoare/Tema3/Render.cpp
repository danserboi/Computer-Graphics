#include "Tema3.h"

#include <Core/Engine.h>

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, Texture2D* texture)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	GLint modelLocation = glGetUniformLocation(shader->GetProgramID(), "Model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	if (strcmp(shader->GetName(), "NormalShaderTema3") == 0 || strcmp(shader->GetName(), "ColorShaderTema3") == 0) {
		GLint viewLocation = glGetUniformLocation(shader->GetProgramID(), "View");
		glm::mat4 viewMatrix = camera->GetViewMatrix();
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

		GLint projLocation = glGetUniformLocation(shader->GetProgramID(), "Projection");
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}

	if (strcmp(shader->GetName(), "NormalShaderTema3") == 0 || strcmp(shader->GetName(), "ColorShaderTema3") == 0 || strcmp(shader->GetName(), "ShaderSkybox") == 0) {
		if (texture)
		{
			// activate texture location 0
			glActiveTexture(GL_TEXTURE0);
			// bind the texture1 ID
			glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
			// send texture uniform value
			GLint texture_1_loc = glGetUniformLocation(shader->program, "texture_1");
			glUniform1i(texture_1_loc, 0);
		}

	}

	if (strcmp(shader->GetName(), "NormalShaderTema3") == 0) {
		GLint timeLocation = glGetUniformLocation(shader->GetProgramID(), "SpeedRunTime");
		glUniform1f(timeLocation, speedRunSeconds);

		GLint onPowerUpsLocation = glGetUniformLocation(shader->GetProgramID(), "OnPowerUps");
		glUniform1i(onPowerUpsLocation, onPowerUps);

		GLint fallingLocation = glGetUniformLocation(shader->GetProgramID(), "falling");
		glUniform1i(fallingLocation, falling);
	}

	if (strcmp(shader->GetName(), "ColorShaderTema3") == 0 || strcmp(shader->GetName(), "Shader2DTema3") == 0) {
		GLint colorLocation = glGetUniformLocation(shader->GetProgramID(), "Color");
		glUniform3fv(colorLocation, 1, glm::value_ptr(color));

	}

	if (strcmp(shader->GetName(), "ColorShaderTema3") == 0) {
		GLint TetrahedronLoc = glGetUniformLocation(shader->program, "ObjectType");
		if (mesh == meshes["cube_obstacle"]) {
			glUniform1i(TetrahedronLoc, 1);
		}
		else if (mesh == meshes["pyramid"]) {
			glUniform1i(TetrahedronLoc, 1);
		}
		else {
			glUniform1i(TetrahedronLoc, 0);
		}
		GLint locTime = glGetUniformLocation(shader->program, "Time");
		glUniform1f(locTime, (GLfloat)Engine::GetElapsedTime());
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema3::RenderDecoratives(int i) {
	// we render the decorative object on the left
	if (platformsGroups[i].leftDecorative == CYLINDER) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(FIRST_PLATFORM_OFFSET_X - 1 - DECORATIVE_OFFSET_X, DECORATIVE_CYLINDER_OFFSET_Y, platformsGroups[i].distance - platformsGroups[i].leftDecorativeZOffset));
		modelMatrix = glm::rotate(modelMatrix, (GLfloat)Engine::GetElapsedTime(), glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, DECORATIVE_CYLINDERS_ANG, glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, DECORATIVE_CYLINDER_SCALE_Z));
		RenderSimpleMesh(meshes["cylinder"], shaders["ColorShaderTema3"], modelMatrix, LAVENDER_BLUSH, mapTextures["decorative_cylinder_texture"]);
	}
	else if (platformsGroups[i].leftDecorative == PYRAMID) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(FIRST_PLATFORM_OFFSET_X - 1 - DECORATIVE_OFFSET_X, DECORATIVE_PYRAMID_HEIGHT, platformsGroups[i].distance - platformsGroups[i].leftDecorativeZOffset));
		RenderSimpleMesh(meshes["pyramid"], shaders["ColorShaderTema3"], modelMatrix, GRAY, mapTextures["decorative_pyramid_texture"]);
	}
	else if (platformsGroups[i].leftDecorative == SIMPLE_CYLINDER) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(FIRST_PLATFORM_OFFSET_X - 1 - DECORATIVE_OFFSET_X, DECORATIVE_SIMPLE_CYLINDER_HEIGHT, platformsGroups[i].distance - platformsGroups[i].leftDecorativeZOffset));
		modelMatrix = glm::rotate(modelMatrix, (GLfloat)Engine::GetElapsedTime(), glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(DECORATIVE_CYLINDERS_ANG), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, DECORATIVE_SIMPLE_CYLINDER_SCALE_Z));RenderSimpleMesh(meshes["cylinder"], shaders["ColorShaderTema3"], modelMatrix, PYNK, mapTextures["metal_core_texture"]);
	}
	else if (platformsGroups[i].leftDecorative == TETRAHEDRON) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(FIRST_PLATFORM_OFFSET_X - 1 - DECORATIVE_OFFSET_X, DECORATIVE_TETRAHEDRON_HEIGHT, platformsGroups[i].distance - platformsGroups[i].leftDecorativeZOffset));
		modelMatrix = glm::rotate(modelMatrix, (GLfloat)Engine::GetElapsedTime(), glm::vec3(0, 1, 0));
		RenderSimpleMesh(meshes["tetrahedron"], shaders["ColorShaderTema3"], modelMatrix, TEAL, mapTextures["tetrahedron_texture"]);
	}

	// we render the decorative object on the right
	if (platformsGroups[i].rightDecorative == CYLINDER) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(FIRST_PLATFORM_OFFSET_X + NO_COLUMNS + DECORATIVE_OFFSET_X, DECORATIVE_CYLINDER_OFFSET_Y, platformsGroups[i].distance - platformsGroups[i].rightDecorativeZOffset));
		modelMatrix = glm::rotate(modelMatrix, (GLfloat)Engine::GetElapsedTime(), glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, -DECORATIVE_CYLINDERS_ANG, glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, DECORATIVE_CYLINDER_SCALE_Z));
		RenderSimpleMesh(meshes["cylinder"], shaders["ColorShaderTema3"], modelMatrix, LAVENDER_BLUSH, mapTextures["decorative_cylinder_texture"]);
	}
	else if (platformsGroups[i].rightDecorative == PYRAMID) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(FIRST_PLATFORM_OFFSET_X + NO_COLUMNS + DECORATIVE_OFFSET_X, DECORATIVE_PYRAMID_HEIGHT, platformsGroups[i].distance - platformsGroups[i].rightDecorativeZOffset));
		RenderSimpleMesh(meshes["pyramid"], shaders["ColorShaderTema3"], modelMatrix, GRAY, mapTextures["decorative_pyramid_texture"]);
	}
	else if (platformsGroups[i].rightDecorative == SIMPLE_CYLINDER) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(FIRST_PLATFORM_OFFSET_X + NO_COLUMNS + DECORATIVE_OFFSET_X, DECORATIVE_SIMPLE_CYLINDER_HEIGHT, platformsGroups[i].distance - platformsGroups[i].rightDecorativeZOffset));
		modelMatrix = glm::rotate(modelMatrix, (GLfloat)Engine::GetElapsedTime(), glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(DECORATIVE_CYLINDERS_ANG), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, DECORATIVE_SIMPLE_CYLINDER_SCALE_Z));RenderSimpleMesh(meshes["cylinder"], shaders["ColorShaderTema3"], modelMatrix, PYNK, mapTextures["metal_core_texture"]);
	}
	else if (platformsGroups[i].rightDecorative == TETRAHEDRON) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(FIRST_PLATFORM_OFFSET_X + NO_COLUMNS + DECORATIVE_OFFSET_X, DECORATIVE_TETRAHEDRON_HEIGHT, platformsGroups[i].distance - platformsGroups[i].rightDecorativeZOffset));
		modelMatrix = glm::rotate(modelMatrix, (GLfloat)Engine::GetElapsedTime(), glm::vec3(0, 1, 0));
		RenderSimpleMesh(meshes["tetrahedron"], shaders["ColorShaderTema3"], modelMatrix, TEAL, mapTextures["tetrahedron_texture"]);
	}
}

void Tema3::RenderPlatformObjects(int i, int j) {
	// render collectables
	if (platformsGroups[i].collectablesZCoord[j] != -1) {
		for (int k = 0; k < platformsGroups[i].noCollectables[j]; k++) {
			if (platformsGroups[i].collectablesState[j][k] == 1) {
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(FIRST_PLATFORM_OFFSET_X + j, COLLECTABLE_SCALE / 2 + COLLECTABLE_OFFSET_Y, -COLLECTABLE_SCALE / 2 + platformsGroups[i].distance - platformsGroups[i].collectablesZCoord[j] - k));
				modelMatrix = glm::rotate(modelMatrix, (GLfloat)Engine::GetElapsedTime() * COLLECTABLES_ROTATION_SENSITIVTY, glm::vec3(0, 1, 0));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(COLLECTABLE_SCALE));
				RenderSimpleMesh(meshes["sphere"], shaders["ColorShaderTema3"], modelMatrix, GOLD, mapTextures["collectable_texture"]);
			}
		}
	}
	// render obstacle
	if (platformsGroups[i].obstacleZCoord[j] != -1) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(FIRST_PLATFORM_OFFSET_X + j, COLLECTABLE_SCALE, -PLATFORM_SCALE_Z + platformsGroups[i].distance - platformsGroups[i].obstacleZCoord[j]));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(PLATFORM_SCALE_X, COLLECTABLE_SCALE, PLATFORM_SCALE_Z));
		RenderSimpleMesh(meshes["cube_obstacle"], shaders["ColorShaderTema3"], modelMatrix, RED, mapTextures["concrete"]);
	}
}

void Tema3::RenderSphere() {
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(sphereX, SPHERE_SCALE * SPHERE_SCALE + sphereY, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(SPHERE_SCALE));
	RenderSimpleMesh(meshes["sphere"], shaders["NormalShaderTema3"], modelMatrix, BLUE, mapTextures["sphere_texture"]);
}

void Tema3::RenderScore() {
	// we render the quantity of fuel with green color
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-SCORE_OFFSET_X - SCORE_SCALE_X * (1 - noCollectablesGained / MAX_SCORE) / 2, SCORE_OFFSET_Y, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(SCORE_SCALE_X * noCollectablesGained / MAX_SCORE, SCORE_SCALE_Y, 0));
	RenderSimpleMesh(meshes["square"], shaders["Shader2DTema3"], modelMatrix, PURE_BLUE);
}

void Tema3::RenderFuelBar() {
	// we render the quantity of fuel with green color
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(SQUARE_OFFSET_X - SQUARE_SCALE_X * (1 - fuel / FUEL_MAX) / 2, SQUARE_OFFSET_Y, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(SQUARE_SCALE_X * fuel / FUEL_MAX, SQUARE_SCALE_Y, 0));
	RenderSimpleMesh(meshes["square"], shaders["Shader2DTema3"], modelMatrix, GREEN);

	// white background
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(SQUARE_OFFSET_X, SQUARE_OFFSET_Y, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(SQUARE_SCALE_X, SQUARE_SCALE_Y, 0));
	RenderSimpleMesh(meshes["square"], shaders["Shader2DTema3"], modelMatrix, WHITE);
}

void Tema3::RenderLifes() {
	for (int i = 0; i < lifes; i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-i * SPACE_BEETWEEN_CIRCLES + CIRCLES_OFFSET_X, CIRCLES_OFFSET_Y, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(CIRCLE_SCALE_X, CIRCLE_SCALE_Y, 0));
		RenderSimpleMesh(meshes["circle"], shaders["Shader2DTema3"], modelMatrix, BISQUE);
	}
}

void Tema3::RenderPlayButton() {
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(PLAY_BUTTON_TRIANGLE_SCALE));
	RenderSimpleMesh(meshes["triangle"], shaders["Shader2DTema3"], modelMatrix, SILVER);

	modelMatrix = glm::mat4(1);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(PLAY_BUTTON_CIRCLE_SCALE / window->props.aspectRatio, PLAY_BUTTON_CIRCLE_SCALE, 0));
	RenderSimpleMesh(meshes["circle"], shaders["Shader2DTema3"], modelMatrix, STEEL_BLUE);
}
