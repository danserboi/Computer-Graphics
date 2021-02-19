#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Objects2D
{

	Mesh* CreateSq(std::string name, glm::vec3 color, bool fill = false);

	Mesh* CreateCircle(std::string name, glm::vec3 color);

	Mesh* CreateSemicircle(std::string name, glm::vec3 color);

	Mesh* CreateFilledSemicircle(std::string name, glm::vec3 color);

	Mesh* CreateLine(std::string name, glm::vec3 color);

	Mesh* CreateTriangle(std::string name, glm::vec3 color);

	Mesh* CreateShuriken(std::string name, glm::vec3 color);

	Mesh* CreateHitShuriken(std::string name, glm::vec3 color);

	Mesh* CreateBalloonThread(std::string name, glm::vec3 color);

	Mesh* CreateVest(std::string name, glm::vec3 color);

	Mesh* CreatePants(std::string name, glm::vec3 color);

	Mesh* CreateRightHand(std::string name, glm::vec3 color);

	Mesh* CreateLeftHand(std::string name, glm::vec3 color);

	Mesh* CreateRightShoe(std::string name, glm::vec3 color);

	Mesh* CreateLeftShoe(std::string name, glm::vec3 color);

	Mesh* CreateNose(std::string name, glm::vec3 color);

	Mesh* CreateMouth(std::string name, glm::vec3 color);

	Mesh* CreateLeftEye(std::string name, glm::vec3 color);

	Mesh* CreateRightEye(std::string name, glm::vec3 color);

	Mesh* CreateLeftEar(std::string name, glm::vec3 color);

	Mesh* CreateHat(std::string name, glm::vec3 color);

	Mesh* CreateFeather(std::string name, glm::vec3 color);
}
