#include "Tema3.h"

#include <Core/Engine.h>

#include <vector>

using namespace std;

Mesh* Tema3::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
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

void Tema3::InitMeshes() {
	// Load sphere mesh
	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create cube mesh for platforms
	{

		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), glm::vec3(0.3, 0.5, 0.4), glm::vec2(1.0f, 0.0f)),
			VertexFormat(glm::vec3(1, -1, -1), glm::vec3(0, 1, 1), glm::vec3(0.5, 0.2, 0.9), glm::vec2(1.0f, 1.0f)),
			VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 1, 0), glm::vec3(0.7, 0.0, 0.7), glm::vec2(0.0f, 1.0f)),
			VertexFormat(glm::vec3(1,  1, -1), glm::vec3(0, 0, 1), glm::vec3(0.1, 0.5, 0.8), glm::vec2(0.0f, 0.0f)),
			VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.2), glm::vec2(1.0f, 0.0f)),
			VertexFormat(glm::vec3(1, -1,  1), glm::vec3(1, 0, 1), glm::vec3(0.9, 0.4, 0.2), glm::vec2(1.0f, 1.0f)),
			VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1, 0, 0), glm::vec3(0.7, 0.7, 0.1), glm::vec2(0.0f, 1.0f)),
			VertexFormat(glm::vec3(1,  1,  1), glm::vec3(0, 1, 0), glm::vec3(0.7, 0.3, 0.7), glm::vec2(0.0f, 0.0f)),
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

		CreateMesh("cube_platform", vertices, indices);
	}

	// Create cube mesh for obstacles
	{

		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.2), glm::vec2(0.0f, 0.0f)),
			VertexFormat(glm::vec3(1, -1,  1), glm::vec3(1, 0, 1), glm::vec3(0.9, 0.4, 0.2), glm::vec2(1.0f, 0.0f)),
			VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1, 0, 0), glm::vec3(0.7, 0.7, 0.1), glm::vec2(0.0f, 1.0f)),
			VertexFormat(glm::vec3(1,  1,  1), glm::vec3(0, 1, 0), glm::vec3(0.7, 0.3, 0.7), glm::vec2(1.0f, 1.0f)),
			VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), glm::vec3(0.3, 0.5, 0.4), glm::vec2(1.0f, 0.0f)),
			VertexFormat(glm::vec3(1, -1, -1), glm::vec3(0, 1, 1), glm::vec3(0.5, 0.2, 0.9), glm::vec2(0.0f, 0.0f)),
			VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 1, 0), glm::vec3(0.7, 0.0, 0.7), glm::vec2(1.0f, 1.0f)),
			VertexFormat(glm::vec3(1,  1, -1), glm::vec3(0, 0, 1), glm::vec3(0.1, 0.5, 0.8), glm::vec2(0.0f, 1.0f)),
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

		CreateMesh("cube_obstacle", vertices, indices);
	}

	// Create a tetrahedron mesh
	{

		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(0, 0,  0), glm::vec3(0, 1, 0), glm::vec3(0.2, 0.8, 0.2), glm::vec2(1.0f, 0.0f)),
			VertexFormat(glm::vec3(-0.2, -DECORATIVE_TETRAHEDRON_HEIGHT,  1), glm::vec3(1, 0, 0), glm::vec3(0.9, 0.4, 0.2), glm::vec2(0.0f, 0.0f)),
			VertexFormat(glm::vec3(1, -DECORATIVE_TETRAHEDRON_HEIGHT,  0), glm::vec3(0, 0, 1), glm::vec3(0.7, 0.7, 0.1), glm::vec2(1.0f, 1.0f)),
			VertexFormat(glm::vec3(-0.3, -DECORATIVE_TETRAHEDRON_HEIGHT,  -1), glm::vec3(1, 1, 0), glm::vec3(0.7, 0.3, 0.7), glm::vec2(0.0f, 1.0f)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,	// indices for first triangle
			0, 2, 3,	// indices for second triangle
			0, 3, 1,
			1, 2, 3,
		};

		CreateMesh("tetrahedron", vertices, indices);
	}

	// Create a pyramid mesh
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(0, 0,  0), glm::vec3(0, 1, 0), glm::vec3(0.2, 0.8, 0.2), glm::vec2(1.0f, 0.0f)),
			VertexFormat(glm::vec3(0, -DECORATIVE_PYRAMID_HEIGHT,  1), glm::vec3(1, 0, 0), glm::vec3(0.9, 0.4, 0.2), glm::vec2(0.0f, 0.0f)),
			VertexFormat(glm::vec3(1, -DECORATIVE_PYRAMID_HEIGHT,  0), glm::vec3(0, 0, 1), glm::vec3(0.7, 0.7, 0.1), glm::vec2(1.0f, 1.0f)),
			VertexFormat(glm::vec3(0, -DECORATIVE_PYRAMID_HEIGHT,  -1), glm::vec3(1, 1, 0), glm::vec3(0.7, 0.3, 0.7), glm::vec2(0.0f, 1.0f)),
			VertexFormat(glm::vec3(-1, -DECORATIVE_PYRAMID_HEIGHT,  0), glm::vec3(1, 1, 0), glm::vec3(0.7, 0.3, 0.7), glm::vec2(1.0f, 1.0f))
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,	// indices for first triangle
			0, 2, 3,	// indices for second triangle
			0, 3, 4,
			0, 4, 1,
			1, 2, 3,
			3, 4, 1
		};

		CreateMesh("pyramid", vertices, indices);
	}

	// Create a cylinder mesh
	{
		std::vector<VertexFormat> vertices;
		std::vector<unsigned short> indices;
		// center of circle
		vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), BISQUE, glm::vec3(0.2, 0.8, 0.2), glm::vec2(0.0f, 0.0f)));
		for (int i = 0; i < CIRCLE_VERTICES + 1; i++) {
			// create vertex
			double x = cos(RADIANS(360.0f * i / CIRCLE_VERTICES));
			double y = sin(RADIANS(360.0f * i / CIRCLE_VERTICES));
			if (i % 2 == 0) {
				vertices.push_back(VertexFormat(glm::vec3(y, x, -1), BISQUE, glm::vec3(0.2, 0.8, 0.2), glm::vec2(y, x)));
			}
			else {
				vertices.push_back(VertexFormat(glm::vec3(y, x, -1), BISQUE, glm::vec3(0.2, 0.8, 0.2), glm::vec2(y, x)));
			}
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
		int offset = CIRCLE_VERTICES + 2;
		// center of circle
		vertices.push_back(VertexFormat(glm::vec3(0, 0, 1), BISQUE, glm::vec3(0.2, 0.8, 0.2), glm::vec2(0.0f, 0.0f)));
		for (int i = offset; i < offset + CIRCLE_VERTICES + 1; i++) {
			// create vertex
			double x = cos(RADIANS(360.0f * i / CIRCLE_VERTICES));
			double y = sin(RADIANS(360.0f * i / CIRCLE_VERTICES));
			if (i % 2 == 0) {
				vertices.push_back(VertexFormat(glm::vec3(y, x, 1), BISQUE, glm::vec3(0.2, 0.8, 0.2), glm::vec2(y, x)));
			}
			else {
				vertices.push_back(VertexFormat(glm::vec3(y, x, 1), BISQUE, glm::vec3(0.2, 0.8, 0.2), glm::vec2(y, x)));
			}
			// create triangle
			if (i < offset + CIRCLE_VERTICES - 1) {
				indices.push_back(offset);
				indices.push_back((unsigned short)(i + 1));
				indices.push_back((unsigned short)(i + 2));
			}
			else {
				indices.push_back(offset);
				indices.push_back(offset + CIRCLE_VERTICES);
				indices.push_back(offset + 1);
			}
		}
		for (int i = 0; i < CIRCLE_VERTICES + 1; i++) {
			// create triangles
			if (i < offset + CIRCLE_VERTICES - 1) {
				// first triangle
				indices.push_back((unsigned short)(i + 1));
				indices.push_back((unsigned short)(i + 2));
				indices.push_back((unsigned short)(offset + i + 2));
				// second triangle
				indices.push_back((unsigned short)(i + 1));
				indices.push_back((unsigned short)(offset + i + 2));
				indices.push_back((unsigned short)(offset + i + 1));
			}
			else {
				// first triangle
				indices.push_back(CIRCLE_VERTICES);
				indices.push_back(1);
				indices.push_back(offset + 1);
				// second triangle
				indices.push_back(CIRCLE_VERTICES);
				indices.push_back(offset + 1);
				indices.push_back(offset + CIRCLE_VERTICES);

			}
		}

		CreateMesh("cylinder", vertices, indices);
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

		std::vector<unsigned short> indices = { 0, 1, 2 };

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
}
