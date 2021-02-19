#pragma once
#include <Component/SimpleScene.h>
#include "Camera.h"
#include <Core/GPU/Mesh.h>

#define BLUE glm::vec3(0, 0, 0.5f)
#define RED glm::vec3(0.863f, 0.078f, 0.235f)
#define YELLOW glm::vec3(1, 1, 0)
#define ORANGE glm::vec3(1.000f, 0.647f, 0.000f)
#define GREEN glm::vec3(0.000f, 1.000f, 0.498f)
#define PURPLE glm::vec3(0.502f, 0.000f, 0.502f)
#define WHITE glm::vec3(1, 1, 1)
#define BISQUE glm::vec3(1.000f, 0.894f, 0.769f)
#define GOLD glm::vec3(1.000f, 0.843f, 0.000f)
#define GRAY glm::vec3(0.502f, 0.502f, 0.502f)
#define SILVER glm::vec3(0.753f, 0.753f, 0.753f)
#define STEEL_BLUE glm::vec3(0.275f, 0.510f, 0.706f)
#define PURE_BLUE glm::vec3(0, 0, 1)
#define TEAL glm::vec3(0.000f, 0.502f, 0.502f)
#define LAVENDER_BLUSH glm::vec3(1.000f, 0.941f, 0.961f)
#define PYNK glm::vec3(1.000f, 0.078f, 0.576f)
#define MISSING_PLATFORM glm::vec3(-1, -1, -1)

#define FIRST_PERSON_CAMERA_Y 0.5f
#define FIRST_PERSON_CAMERA_Z -0.2f
#define THIRD_PERSON_CAMERA_Y 2.5f
#define THIRD_PERSON_CAMERA_Z 2.8f

#define SPHERE_SCALE 0.5f
#define SPHERE_SENSITIVITY_X 4
#define JUMP_SENSITIVITY 3
#define RISE_TIME 1.5
#define ANIMATION_FALLING_TIME 2

#define PLATFORM_SCALE_X 0.4f
#define PLATFORM_SCALE_Y 0.05f
#define PLATFORM_SCALE_Z 0.5f
#define FIRST_PLATFORM_OFFSET_X -5.5f
#define PLATFORM_MAX_DIM_Z 10
#define PLATFORM_MAX_FREE_SPACE_Z 3
#define NO_GROUPS 24
#define NO_COLUMNS 12
#define MAX_Z 1
#define PLATFORMS_MAX_SPEED 15
#define PLATFORMS_MIN_SPEED_EASY 4
#define PLATFORMS_MIN_SPEED_MEDIUM 4.5
#define PLATFORMS_MIN_SPEED_HARD 5.5
#define PLATFORMS_MIN_SPEED_PRO 6
#define PLATFORMS_SPEED_RUN_VALUE 8
#define SPEED_RUN_SECS 10
#define SPEED_SENSITIVITY 20

#define MAX_SCORE 100.0f
#define SCORE_SCALE_X 0.3f
#define SCORE_SCALE_Y 0.10f
#define SCORE_OFFSET_X -0.80
#define SCORE_OFFSET_Y 0.90

#define SQUARE_SCALE_X 0.3f
#define SQUARE_SCALE_Y 0.15f
#define SQUARE_OFFSET_X -0.8
#define SQUARE_OFFSET_Y 0.8
#define FUEL_MAX 10
#define FUEL_SENSITIVITY_EASY 0.3
#define FUEL_SENSITIVITY_MEDIUM 0.4
#define FUEL_SENSITIVITY_HARD 0.5
#define FUEL_SENSITIVITY_PRO 0.6
#define FUEL_GAIN 5
#define FUEL_LOSS 5

#define LIFES_MAX_NO 3
#define CIRCLE_VERTICES 100
#define CIRCLE_SCALE_X 0.03f
#define CIRCLE_SCALE_Y 0.05f
#define CIRCLES_OFFSET_X 0.94f
#define CIRCLES_OFFSET_Y 0.75
#define SPACE_BEETWEEN_CIRCLES 0.08f

#define SCORE_SENSITIVITY 15

#define MEDIUM_DIFF_TIME 60
#define PRO_DIFF_TIME 120
#define HARD_DIFF_TIME 180

#define INITIAL_ROTATION_ANGLE -25
#define FOV 60
#define Z_NEAR 0.01f
#define Z_FAR 200.0f

#define PLAY_BUTTON_TRIANGLE_SCALE 0.1f
#define PLAY_BUTTON_CIRCLE_SCALE 0.18f

#define COLLECTABLE_SCALE 0.4f
#define COLLECTABLE_OFFSET_Y 0.1f
#define MAX_NO_COLLECTABLES 5
#define COLLECTABLES_ROTATION_SENSITIVTY 4

#define DECORATIVE_SIMPLE_CYLINDER_SCALE_Z 0.5f
#define DECORATIVE_SIMPLE_CYLINDER_HEIGHT 2
#define DECORATIVE_TETRAHEDRON_HEIGHT 2

#define DECORATIVE_PYRAMID_HEIGHT 4
#define DECORATIVE_PYRAMID_OFFSET_X 1
#define DECORATIVE_PYRAMID_SCALE 0.5f
#define DECORATIVE_MAX_Z 7
#define DECORATIVE_OFFSET_X 2

#define NO_OBJECT 0
#define PYRAMID 1
#define CYLINDER 2
#define SIMPLE_CYLINDER 3
#define TETRAHEDRON 4

#define COLLECTABLES 0
#define OBSTACLE 1

#define DECORATIVE_CYLINDER_OFFSET_Y 1.0f
#define DECORATIVE_CYLINDER_OFFSET_X 2
#define DECORATIVE_CYLINDER_SCALE_Z 0.1f
#define DECORATIVE_CYLINDERS_ANG 45.0f

enum Difficulty { easy, medium, hard, pro };

struct PlatformsGroup {
	glm::vec3 colors[NO_COLUMNS];
	char lengths[NO_COLUMNS];
	float distance;
	int leftDecorative;
	int rightDecorative;
	int leftDecorativeZOffset;
	int rightDecorativeZOffset;
	int obstacleZCoord[NO_COLUMNS];
	int noCollectables[NO_COLUMNS];
	int collectablesState[NO_COLUMNS][MAX_NO_COLLECTABLES];
	int collectablesZCoord[NO_COLUMNS];
};

class Tema3 : public SimpleScene
{
public:
	Tema3();
	~Tema3();
	

	void Init() override;

private:
	void InitMeshes();
	void InitTextures();
	void InitShaders();
	void InitObjects();

	void GenerateRandomGroupOfObjects(int i);

	void GenerateDecoratives(int i);
	void GenerateObjectsOnPlatform(int i, int j);

	void GeneratePlatformEasyLevel(int i, int otherPlatIdx);
	void GeneratePlatformMediumLevel(int i, int otherPlatIdx);
	void GeneratePlatformHardLevel(int i, int otherPlatIdx);
	void GeneratePlatformProLevel(int i, int otherPlatIdx);

	void ReinitializeValues();

	void DifficultyUpdate();
	void SpeedRunUpdate(float deltaTimeSeconds);
	void ScoreUpdate(float deltaTimeSeconds);
	void FallingUpdate(float deltaTimeSeconds);
	void CameraUpdate();
	void JumpUpdate(float deltaTimeSeconds);

	void UpdateAndRenderObjects(float deltaTimeSeconds);
	void RenderDecoratives(int i);
	void RenderPlatformObjects(int i, int j);
	void RenderSphere();
	void RenderScore();
	void RenderFuelBar();
	void RenderLifes();
	void RenderPlayButton();

	Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);
	
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, Texture2D* texture = nullptr);

	float Distance(float xA, float yA, float xB, float yB);
	bool PointCircleCollision(float pointX, float pointY, float circleCenterX, float circleCenterY, float circleRadius);
	float Distance3D(float firstX, float firstY, float firstZ, float otherX, float otherY, float otherZ);
	bool SphereSphereCollision3D(float sphereX, float sphereY, float sphereZ, float sphereRadius, float otherX, float otherY, float otherZ, float otherRadius);
	bool SphereAABBCollision3D(float boxMinX, float boxMinY, float boxMinZ, float boxMaxX, float boxMaxY, float boxMaxZ, float sphereX, float sphereY, float sphereZ, float sphereRadius);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	std::unordered_map<std::string, Texture2D*> mapTextures;

	bool started = false;
	float timeElapsedBeforeStart = 0;
	
	float platformsSpeed = PLATFORMS_MIN_SPEED_EASY;
	float fuelSensitivity = FUEL_SENSITIVITY_EASY;
	float minSpeed = PLATFORMS_MIN_SPEED_EASY;
	float fuel = FUEL_MAX;
	// false for first person camera, true for third person camera
	bool cameraPerson = true;
	float sphereX = -0.5f;
	float sphereY = 0;

	bool onJump = false;
	bool onRise = false;
	bool falling = false;
	bool speedRun = false;
	float speedRunSeconds = SPEED_RUN_SECS;
	int onPowerUps = 0;
	
	int lifes = 1;

	int noCollectablesGained = 0;

	struct PlatformsGroup platformsGroups[NO_GROUPS];

	enum Difficulty difficulty = easy;

protected:
	CameraTema3::Camera* camera;
	glm::mat4 projectionMatrix;

};
