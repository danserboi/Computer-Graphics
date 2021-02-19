#include "Objects2D.h"

#include <Core/Engine.h>
#define CIRCLE_VERTICES 100
#define PI 3.14159265

Mesh* Objects2D::CreateSq(std::string name, glm::vec3 color, bool fill)
{

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(-0.500000f, -0.500000f, 0), color),
		VertexFormat(glm::vec3(0.500000f, -0.500000f, 0), color),
		VertexFormat(glm::vec3(0.500000f, 0.500000f, 0), color),
		VertexFormat(glm::vec3(-0.500000f, 0.500000f, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Objects2D::CreateCircle(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	// center of circle
	vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color));
	for (int i = 0; i < CIRCLE_VERTICES + 1; i++) {
		// create vertex
		double x = cos((i * 360 / CIRCLE_VERTICES) * PI / 180);
		double y = sin((i * 360 / CIRCLE_VERTICES) * PI / 180);
		vertices.push_back(VertexFormat(glm::vec3(y, x, 0), color));
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

	Mesh* circle = new Mesh(name);
	circle->SetDrawMode(GL_TRIANGLE_FAN);
	circle->InitFromData(vertices, indices);

	return circle;
}

Mesh* Objects2D::CreateSemicircle(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	// center of circle
	vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color));
	for (int i = 0; i < CIRCLE_VERTICES / 2 + 2; i++) {
		// add vertex and index
		double x = cos((i * 360 / CIRCLE_VERTICES) * PI / 180);
		double y = sin((i * 360 / CIRCLE_VERTICES) * PI / 180);
		vertices.push_back(VertexFormat(glm::vec3(y, x, 0), color));
		indices.push_back((unsigned short)(i));
	}

	Mesh* semicircle = new Mesh(name);
	semicircle->SetDrawMode(GL_LINE_LOOP);
	semicircle->InitFromData(vertices, indices);

	return semicircle;
}

Mesh* Objects2D::CreateFilledSemicircle(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	// center of circle
	vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color));
	for (int i = 0; i < CIRCLE_VERTICES / 2 + 1; i++) {
		// create vertex
		double x = cos((i * 360 / CIRCLE_VERTICES) * PI / 180);
		double y = sin((i * 360 / CIRCLE_VERTICES) * PI / 180);
		vertices.push_back(VertexFormat(glm::vec3(y, x, 0), color));
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

	Mesh* semicircle = new Mesh(name);
	semicircle->SetDrawMode(GL_TRIANGLE_FAN);
	semicircle->InitFromData(vertices, indices);

	return semicircle;
}

Mesh* Objects2D::CreateLine(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(1, 0, 0), color),
	};

	Mesh* line = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1 };
	line->SetDrawMode(GL_LINES);


	line->InitFromData(vertices, indices);
	return line;
}

Mesh* Objects2D::CreateTriangle(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(-0.500000f, -1, 0), color),
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(0.500000f, -1, 0), color),
	};

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2 };
	triangle->SetDrawMode(GL_TRIANGLES);


	triangle->InitFromData(vertices, indices);
	return triangle;
}

Mesh* Objects2D::CreateShuriken(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(0.500000f, 0, 0), color),
		VertexFormat(glm::vec3(0.500000f, 0.500000f, 0), color),
		VertexFormat(glm::vec3(0, 0.500000f, 0), color),
		VertexFormat(glm::vec3(-0.500000f, 0.500000f, 0), color),
		VertexFormat(glm::vec3(-0.500000f, 0, 0), color),
		VertexFormat(glm::vec3(-0.500000f, -0.500000f, 0), color),
		VertexFormat(glm::vec3(0, -0.500000f, 0), color),
		VertexFormat(glm::vec3(0.500000f, -0.500000f, 0), color),
	};

	Mesh* shuriken = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 0, 3, 4, 0, 5, 6, 0, 7, 8 };
	shuriken->SetDrawMode(GL_TRIANGLE_FAN);


	shuriken->InitFromData(vertices, indices);
	return shuriken;
}

Mesh* Objects2D::CreateHitShuriken(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0.500000f, 0.500000f, 0), color),
		VertexFormat(glm::vec3(1, 0.500000f, 0), color),
		VertexFormat(glm::vec3(1, 1, 0), color),

		VertexFormat(glm::vec3(-0.500000f, 0.500000f, 0), color),
		VertexFormat(glm::vec3(-0.500000f, 1, 0), color),
		VertexFormat(glm::vec3(-1, 1, 0), color),

		VertexFormat(glm::vec3(-0.500000f, -0.500000f, 0), color),
		VertexFormat(glm::vec3(-1, -0.500000f, 0), color),
		VertexFormat(glm::vec3(-1, -1, 0), color),

		VertexFormat(glm::vec3(0.500000f, -0.500000f, 0), color),
		VertexFormat(glm::vec3(0.500000f, -1, 0), color),
		VertexFormat(glm::vec3(1, -1, 0), color),
	};

	Mesh* shuriken = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	shuriken->SetDrawMode(GL_TRIANGLES);


	shuriken->InitFromData(vertices, indices);
	return shuriken;
}

Mesh* Objects2D::CreateBalloonThread(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(0, -0.250000f, 0), color),
		VertexFormat(glm::vec3(0.200000f, -0.500000f, 0), color),
		VertexFormat(glm::vec3(-0.200000f, -1.000000f, 0), color),
		VertexFormat(glm::vec3(0.100000f, -1.500000f, 0), color),
	};

	Mesh* thread = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4 };
	thread->SetDrawMode(GL_LINE_STRIP);


	thread->InitFromData(vertices, indices);
	return thread;
}

Mesh* Objects2D::CreateVest(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0.25, 0.7, 0), color),
		VertexFormat(glm::vec3(0.55, 0.7, 0), color),
		VertexFormat(glm::vec3(0.6, 1.1, 0), color),
		VertexFormat(glm::vec3(0.7, 1.15, 0), color),
		VertexFormat(glm::vec3(0.7, 1.25, 0), color),
		VertexFormat(glm::vec3(0.45, 1.3, 0), color),
		VertexFormat(glm::vec3(0.45, 1.35, 0), color),
		VertexFormat(glm::vec3(0.35, 1.35, 0), color),
		VertexFormat(glm::vec3(0.35, 1.3, 0), color),
		VertexFormat(glm::vec3(0.1, 1.25, 0), color),
		VertexFormat(glm::vec3(0.1, 1.15, 0), color),
		VertexFormat(glm::vec3(0.2, 1.1, 0), color),
		VertexFormat(glm::vec3(0.25, 0.7, 0), color),
	};

	Mesh* vest = new Mesh(name);
	std::vector<unsigned short> indices = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	vest->SetDrawMode(GL_POLYGON);


	vest->InitFromData(vertices, indices);
	return vest;
}

Mesh* Objects2D::CreatePants(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0.25, 0.7, 0), color),
		VertexFormat(glm::vec3(0.55, 0.7, 0), color),
		VertexFormat(glm::vec3(0.55, 0.2, 0), color),
		VertexFormat(glm::vec3(0.45, 0.2, 0), color),
		VertexFormat(glm::vec3(0.4, 0.65, 0), color),
		VertexFormat(glm::vec3(0.35, 0.2, 0), color),
		VertexFormat(glm::vec3(0.25, 0.2, 0), color),
	};

	Mesh* pants = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4, 5, 6 };
	pants->SetDrawMode(GL_POLYGON);


	pants->InitFromData(vertices, indices);
	return pants;
}

Mesh* Objects2D::CreateRightHand(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0.7, 1.25, 0), color),
		VertexFormat(glm::vec3(1, 1.22, 0), color),
		VertexFormat(glm::vec3(1, 1.18, 0), color),
		VertexFormat(glm::vec3(0.7, 1.15, 0), color),
	};

	Mesh* pants = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	pants->SetDrawMode(GL_POLYGON);


	pants->InitFromData(vertices, indices);
	return pants;
}

Mesh* Objects2D::CreateLeftHand(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0.1, 1.25, 0), color),
		VertexFormat(glm::vec3(0.05, 1.22, 0), color),
		VertexFormat(glm::vec3(0.05, 1.18, 0), color),
		VertexFormat(glm::vec3(0.1, 1.15, 0), color),
		VertexFormat(glm::vec3(0.5, 1.18, 0), color),
		VertexFormat(glm::vec3(0.5, 1.22, 0), color),
	};

	Mesh* pants = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4, 5 };
	pants->SetDrawMode(GL_POLYGON);


	pants->InitFromData(vertices, indices);
	return pants;
}

Mesh* Objects2D::CreateRightShoe(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0.45, 0.15, 0), color),
		VertexFormat(glm::vec3(0.45, 0.2, 0), color),
		VertexFormat(glm::vec3(0.55, 0.2, 0), color),
		VertexFormat(glm::vec3(0.55, 0.17, 0), color),
		VertexFormat(glm::vec3(0.65, 0.17, 0), color),
		VertexFormat(glm::vec3(0.65, 0.15, 0), color),
		VertexFormat(glm::vec3(0.2, 0.15, 0), color),
	};

	Mesh* right_shoe = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4, 5, 6 };
	right_shoe->SetDrawMode(GL_POLYGON);


	right_shoe->InitFromData(vertices, indices);
	return right_shoe;
}

Mesh* Objects2D::CreateLeftShoe(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0.15, 0.15, 0), color),
		VertexFormat(glm::vec3(0.15, 0.17, 0), color),
		VertexFormat(glm::vec3(0.25, 0.17, 0), color),
		VertexFormat(glm::vec3(0.25, 0.2, 0), color),
		VertexFormat(glm::vec3(0.35, 0.2, 0), color),
		VertexFormat(glm::vec3(0.35, 0.15, 0), color),
		VertexFormat(glm::vec3(0.15, 0.15, 0), color),
	};

	Mesh* left_shoe = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4, 5, 6 };
	left_shoe->SetDrawMode(GL_POLYGON);


	left_shoe->InitFromData(vertices, indices);
	return left_shoe;
}

Mesh* Objects2D::CreateNose(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0.48, 1.47, 0), color),
		VertexFormat(glm::vec3(0.48, 1.45, 0), color),
		VertexFormat(glm::vec3(0.50, 1.41, 0), color),
		VertexFormat(glm::vec3(0.49, 1.40, 0), color),
	};

	Mesh* nose = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	nose->SetDrawMode(GL_LINE_STRIP);


	nose->InitFromData(vertices, indices);
	return nose;
}

Mesh* Objects2D::CreateMouth(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0.45, 1.35, 0), color),
		VertexFormat(glm::vec3(0.55, 1.38, 0), color),
	};

	Mesh* mouth = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1 };
	mouth->SetDrawMode(GL_LINES);


	mouth->InitFromData(vertices, indices);
	return mouth;
}

Mesh* Objects2D::CreateLeftEye(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0.42, 1.47, 0), color),
		VertexFormat(glm::vec3(0.42, 1.49, 0), color),
		VertexFormat(glm::vec3(0.43, 1.49, 0), color),
		VertexFormat(glm::vec3(0.44, 1.48, 0), color),
		VertexFormat(glm::vec3(0.43, 1.47, 0), color),
	};

	Mesh* leftEye = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4 };
	leftEye->SetDrawMode(GL_LINE_LOOP);


	leftEye->InitFromData(vertices, indices);
	return leftEye;
}

Mesh* Objects2D::CreateRightEye(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0.52, 1.47, 0), color),
		VertexFormat(glm::vec3(0.52, 1.49, 0), color),
		VertexFormat(glm::vec3(0.53, 1.49, 0), color),
		VertexFormat(glm::vec3(0.54, 1.48, 0), color),
		VertexFormat(glm::vec3(0.53, 1.47, 0), color),
	};

	Mesh* rightEye = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4 };
	rightEye->SetDrawMode(GL_LINE_LOOP);


	rightEye->InitFromData(vertices, indices);
	return rightEye;
}

Mesh* Objects2D::CreateLeftEar(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0.34, 1.43, 0), color),
		VertexFormat(glm::vec3(0.32, 1.46, 0), color),
		VertexFormat(glm::vec3(0.335, 1.48, 0), color),
		VertexFormat(glm::vec3(0.345, 1.465, 0), color),
		VertexFormat(glm::vec3(0.345, 1.445, 0), color),
		VertexFormat(glm::vec3(0.335, 1.435, 0), color),
	};

	Mesh* leftEar = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4, 5 };
	leftEar->SetDrawMode(GL_LINE_LOOP);


	leftEar->InitFromData(vertices, indices);
	return leftEar;
}

Mesh* Objects2D::CreateHat(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0.30, 1.52, 0), color),
		VertexFormat(glm::vec3(0.27, 1.7, 0), color),
		VertexFormat(glm::vec3(0.65, 1.55, 0), color),
	};

	Mesh* hat = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2 };
	hat->SetDrawMode(GL_POLYGON);


	hat->InitFromData(vertices, indices);
	return hat;
}

Mesh* Objects2D::CreateFeather(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0.33, 1.6, 0), color),
		VertexFormat(glm::vec3(0.10, 1.64, 0), color),
		VertexFormat(glm::vec3(0.05, 1.68, 0), color),
		VertexFormat(glm::vec3(0.17, 1.66, 0), color),
		VertexFormat(glm::vec3(0.32, 1.615, 0), color),
	};

	Mesh* feather = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4 };
	feather->SetDrawMode(GL_POLYGON);


	feather->InitFromData(vertices, indices);
	return feather;
}