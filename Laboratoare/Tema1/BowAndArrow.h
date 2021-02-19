#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

struct shuriken {
	float x;
	float y;
	bool hit;
	float hitAnimationTime;
};

struct balloon {
	float x;
	float y;
	// true = red, false = yelllow
	bool color;
	float scale;
	bool hit;
	bool hitAnimation;
};

#define LOGIC_SPACE_WIDTH 16.000000f
#define LOGIC_SPACE_HEIGHT 9.000000f

#define FOREST_GREEN glm::vec3(0.133f, 0.545f, 0.133f)
#define SPRING_GREEN glm::vec3(0.000f, 1.000f, 0.498f)
#define BROWN glm::vec3(0.647f, 0.165f, 0.165f)
#define BLACK glm::vec3(0, 0, 0)
#define BLUE glm::vec3(0, 0, 1)
#define GRAY glm::vec3(0.502f, 0.502f, 0.502f)
#define YELLOW glm::vec3(1, 1, 0)
#define WHITE glm::vec3(1, 1, 1)
#define RED glm::vec3(1, 0, 0)
#define SIENA glm::vec3(0.627f, 0.322f, 0.176f)
#define SILVER glm::vec3(0.753f, 0.753f, 0.753f)
#define CRIMSON glm::vec3(0.863f, 0.078f, 0.235f)
#define BISQUE glm::vec3(1.000f, 0.894f, 0.769f)

#define ARROW_MIN_SPEED 6
#define ARROW_MAX_SPEED 25
#define ARROW_SPEED_FACTOR 10
#define ARROW_TRIANGLE_SCALE 0.100000f
#define ARROW_LINE_LENGTH 1.000000f
#define BOW_SCALE 0.500000f
#define BOW_MAX_Y 8.000000f
#define BOW_MIN_Y 1.200000f
#define BOW_X 0.5000000f
#define EPS 0.010000f
#define BOW_MAX_ANG (RADIANS(180))/2
#define BOW_MIN_ANG -(RADIANS(180))/2
#define BOW_ROTATION_SPEED_FACTOR 2
#define MOUSE_LEFT_BUTTON 1
#define SECONDS_BETWEEN_SHOTS 3

#define SHOT_SEMAPHORE_SCALE 0.200000f
#define SHOT_SEMAPHORE_OFFSET_X 0.250000f
#define SHOT_SEMAPHORE_OFFSET_Y 0.800000f

#define CHARACTER_HEAD_OFFSET_X 0.450000f
#define CHARACTER_HEAD_OFFSET_Y 0.250000f
#define CHARACTER_HEAD_SCALE 0.150000f

#define SHEATH_FIRST_ARROW_ANG RADIANS(100)
#define SHEATH_FIRST_ARROW_X 0.370000f
#define SHEATH_SECOND_ARROW_X 0.255000f
#define SHEATH_OFFSET_Y 0.850000f
#define SHEATH_FIRST_ARROW_SCALE 0.100000f
#define FIRST_ARROW_TRIANGLE_OFFSET_Y 0.050000f
#define SHEATH_LINE_OFFSET_X 0.21
#define SHEATH_LINE_OFFSET_Y 1.28
#define SHEATH_LINE_SCALE_X 0.67

#define POWER_BAR_FACTOR_X 0.050000f
#define POWER_BAR_SCALE_Y 0.100000f
#define POWER_BAR_OFFSET_X 0.000000f
#define POWER_BAR_OFFSET_Y 1.150000f

#define BALLOON_SCALE_FACTOR_X 0.300000f
#define BALLOON_SCALE_FACTOR_Y 0.400000f
#define BALLOON_THREAD_SCALE_FACTOR 0.200000f
#define BALLOON_NECK_SCALE_FACTOR_X 0.150000f
#define BALLOON_NECK_SCALE_FACTOR_Y 0.100000f
#define NO_BALLOONS 14
#define BALLOONS_MIN_Y_OFFSET 9
#define RED_BALLOON_POINTS 5
#define YELLOW_BALLOON_POINTS -10

#define SHURIKEN_SPEED_FACTOR 8
#define NO_SHURIKENS 8
#define SHURIKENS_MAX_X_OFFSET 3
#define SHURIKENS_HIT_ANIMATION_TIME 0.100000f

#define ITEMS_RESET_TIME 21
#define ITEMS_SCALE 0.500000f

#define PANTS_CENTER_X 0.425
#define PANTS_OFFSET_Y 0.775
#define PANTS_RADIUS 0.275

class BowAndArrow : public SimpleScene
{
public:
	struct ViewportSpace
	{
		ViewportSpace() : x(0), y(0), width(1), height(1) {}
		ViewportSpace(int x, int y, int width, int height)
			: x(x), y(y), width(width), height(height) {}
		int x;
		int y;
		int width;
		int height;
	};

	struct LogicSpace
	{
		LogicSpace() : x(0), y(0), width(1), height(1) {}
		LogicSpace(float x, float y, float width, float height)
			: x(x), y(y), width(width), height(height) {}
		float x;
		float y;
		float width;
		float height;
	};

public:
	BowAndArrow();
	~BowAndArrow();

	void Init() override;

private:
	void initializeBalloons();
	void initializeShurikens();

	bool isVisible(float x, float y);
	float distance(float xA, float yA, float xB, float yB);
	bool pointCircleCollision(float pointX, float pointY, float circleCenterX, float circleCenterY, float circleRadius);
	bool circleCircleCollision(float	circle1CenterX, float circle1CenterY, float circle1Radius, float circle2CenterX, float circle2CenterY, float circle2Radius);

	void renderShuriken(glm::mat3 visMatrix, struct shuriken shk);
	void renderBalloon(glm::mat3 visMatrix, struct balloon balloon);
	void renderBow(glm::mat3 visMatrix);
	void renderBowArrow(glm::mat3 visMatrix);
	void renderThrownArrow(glm::mat3 visMatrix);
	void renderPowerBar(glm::mat3 visMatrix);
	void renderShotSemaphore(glm::mat3 visMatrix);
	void renderCharacterFace(glm::mat3 visMatrix);
	void renderCharacterBody(glm::mat3 visMatrix);

	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void DrawScene(glm::mat3 visMatrix);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;

	// Sets the logic space and view space
	// logicSpace: { x, y, width, height }
	// viewSpace: { x, y, width, height }
	glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
	glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

	void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

protected:
	float length;
	ViewportSpace viewSpace;
	LogicSpace logicSpace;
	glm::mat3 modelMatrix, visMatrix;

	int lifes = 3;

	int redBalloonsHits = 0;
	int yellowBalloonsHits = 0;

	float bowY = BOW_MIN_Y;
	float bowAng = 0.000000f;

	bool incArrowSpeed = false;
	float arrowSpeed = ARROW_MIN_SPEED;
	bool thrownAndVisible = false;
	float arrowAng;
	float arrowX;
	float arrowY;
	float arrowFinalSpeed;
	float thrownArrowDistance;

	bool shotUnallowed = false;
	float timeElapsedAfterShot = 0.000000f;

	float shurikensAng = 0;
	float balloonsOffsetY = 0;
	float shurikensOffsetX = LOGIC_SPACE_WIDTH + ITEMS_RESET_TIME;

	struct balloon balloons[NO_BALLOONS];

	struct shuriken shurikens[NO_SHURIKENS];
};
