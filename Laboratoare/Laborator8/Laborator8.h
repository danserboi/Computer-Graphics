#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>

class Laborator8 : public SimpleScene
{
	public:
		Laborator8();
		~Laborator8();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void Laborator8::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 materialKe, glm::vec3 materialKa, glm::vec3 materialKd, glm::vec3 materialKs);


		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		glm::vec3 lightPosition;
		glm::vec3 lightDirection;
		unsigned int materialShininess;
		//glm::vec3 materialKe;
		//glm::vec3 materialKa;
		//glm::vec3 materialKd;
		//glm::vec3 materialKs;
		glm::vec3 lightSourceIntensity;
		glm::vec3 ambientLightIntensity;
		int light_mode;
		float cut_off_angle;
		float angle_OX;
		float angle_OY;
};
