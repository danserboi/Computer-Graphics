#include "Tema3.h"

#include <Core/Engine.h>

void Tema3::InitShaders() {
	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("NormalShaderTema3");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShaderNormal.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Create a shader program for drawing face polygon with the color specified by uniform variable
	{
		Shader* shader = new Shader("ColorShaderTema3");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShaderColor.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Create a shader program for drawing a 2D polygon with the color specified by uniform variable
	{
		Shader* shader = new Shader("Shader2DTema3");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShader2D.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/FragmentShader2D.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}