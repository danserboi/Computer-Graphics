#include "BowAndArrow.h"

#include <vector>
#include <iostream>
#include <stdlib.h>     // srand, rand
#include <time.h>       // time

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Objects2D.h"

using namespace std;


BowAndArrow::BowAndArrow()
{
}

BowAndArrow::~BowAndArrow()
{
}

void BowAndArrow::Init()
{
	auto camera = GetSceneCamera();
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	logicSpace.x = 0;		// logic x
	logicSpace.y = 0;		// logic y
	logicSpace.width = LOGIC_SPACE_WIDTH;	// logic width
	logicSpace.height = LOGIC_SPACE_HEIGHT;	// logic height

	Mesh* shuriken = Objects2D::CreateShuriken("shuriken", SILVER);
	AddMeshToList(shuriken);

	Mesh* hitShuriken = Objects2D::CreateHitShuriken("hitShuriken", SILVER);
	AddMeshToList(hitShuriken);

	Mesh* square = Objects2D::CreateSq("square", BLUE, true);
	AddMeshToList(square);

	Mesh* greenCircle = Objects2D::CreateCircle("greenCircle", SPRING_GREEN);
	AddMeshToList(greenCircle);

	Mesh* redCircle = Objects2D::CreateCircle("redCircle", RED);
	AddMeshToList(redCircle);

	Mesh* redThread = Objects2D::CreateBalloonThread("redThread", RED);
	AddMeshToList(redThread);

	Mesh* redTriangle = Objects2D::CreateTriangle("redTriangle", RED);
	AddMeshToList(redTriangle);

	Mesh* yellowCircle = Objects2D::CreateCircle("yellowCircle", YELLOW);
	AddMeshToList(yellowCircle);

	Mesh* yellowThread = Objects2D::CreateBalloonThread("yellowThread", YELLOW);
	AddMeshToList(yellowThread);

	Mesh* yellowTriangle = Objects2D::CreateTriangle("yellowTriangle", YELLOW);
	AddMeshToList(yellowTriangle);

	Mesh* bow = Objects2D::CreateSemicircle("bow", SIENA);
	AddMeshToList(bow);

	Mesh* arrowLine = Objects2D::CreateLine("arrowLine", BROWN);
	AddMeshToList(arrowLine);

	Mesh* arrowTriangle = Objects2D::CreateTriangle("arrowTriangle", BROWN);
	AddMeshToList(arrowTriangle);

	// character meshes

	Mesh* bisqueCircle = Objects2D::CreateCircle("bisqueCircle", BISQUE);
	AddMeshToList(bisqueCircle);

	Mesh* characterVest = Objects2D::CreateVest("vest", FOREST_GREEN);
	AddMeshToList(characterVest);

	Mesh* characterPants = Objects2D::CreatePants("pants", CRIMSON);
	AddMeshToList(characterPants);

	Mesh* characterRightHand = Objects2D::CreateRightHand("rightHand", BISQUE);
	AddMeshToList(characterRightHand);

	Mesh* characterLeftHand = Objects2D::CreateLeftHand("leftHand", BISQUE);
	AddMeshToList(characterLeftHand);

	Mesh* characterRightShoe = Objects2D::CreateRightShoe("rightShoe", GRAY);
	AddMeshToList(characterRightShoe);

	Mesh* characterLeftShoe = Objects2D::CreateLeftShoe("leftShoe", GRAY);
	AddMeshToList(characterLeftShoe);

	Mesh* characterNose = Objects2D::CreateNose("nose", BLACK);
	AddMeshToList(characterNose);

	Mesh* characterMouth = Objects2D::CreateMouth("mouth", BLACK);
	AddMeshToList(characterMouth);

	Mesh* characterLeftEye = Objects2D::CreateLeftEye("leftEye", BLACK);
	AddMeshToList(characterLeftEye);

	Mesh* characterRightEye = Objects2D::CreateRightEye("rightEye", BLACK);
	AddMeshToList(characterRightEye);

	Mesh* characterLeftEar = Objects2D::CreateLeftEar("leftEar", BLACK);
	AddMeshToList(characterLeftEar);

	Mesh* characterHat = Objects2D::CreateHat("hat", FOREST_GREEN);
	AddMeshToList(characterHat);

	Mesh* characterFeather = Objects2D::CreateFeather("feather", CRIMSON);
	AddMeshToList(characterFeather);

	initializeBalloons();

	initializeShurikens();
}

void BowAndArrow::initializeBalloons()
{
	// initialize random seed
	srand(time(NULL));

	for (int i = 0; i < NO_BALLOONS; i++) {
		balloons[i].x = i + 2;
		// generate random number between -BALLOONS_MIN_Y_OFFSET - 1 and -1 for y coord
		balloons[i].y = -rand() % (BALLOONS_MIN_Y_OFFSET + 1) - 1;
		// generate random color
		balloons[i].color = (bool)(rand() % 2);
		balloons[i].scale = ITEMS_SCALE;
		balloons[i].hit = false;
		balloons[i].hitAnimation = false;
	}

}

void BowAndArrow::initializeShurikens()
{
	// initialize random seed
	srand(time(NULL));

	for (int i = 0; i < NO_SHURIKENS; i++) {
		// generate random number between 1 and SHURIKENS_MAX_X_OFFSET + 1 for x coord
		shurikens[i].x = rand() % (SHURIKENS_MAX_X_OFFSET + 1) + 1;
		shurikens[i].y = i + 1;
		shurikens[i].hit = false;
		shurikens[i].hitAnimationTime = SHURIKENS_HIT_ANIMATION_TIME;
	}

}

bool BowAndArrow::isVisible(float x, float y)
{
	return (x >= 0 && x < LOGIC_SPACE_WIDTH) && (y >= 0 && y < LOGIC_SPACE_HEIGHT);
}

float BowAndArrow::distance(float xA, float yA, float xB, float yB)
{
	return sqrt((xA - xB) * (xA - xB) + (yA - yB) * (yA - yB));
}

bool BowAndArrow::pointCircleCollision(float pointX, float pointY, float circleCenterX, float circleCenterY, float circleRadius)
{
	return isVisible(pointX, pointY) && isVisible(circleCenterX, circleCenterY) && distance(pointX, pointY, circleCenterX, circleCenterY) < circleRadius;
}

bool BowAndArrow::circleCircleCollision(float	circle1CenterX, float circle1CenterY, float circle1Radius, float circle2CenterX, float circle2CenterY, float circle2Radius)
{
	return isVisible(circle1CenterX, circle1CenterY) && isVisible(circle2CenterX, circle2CenterY) &&
		distance(circle1CenterX, circle1CenterY, circle2CenterX, circle2CenterY) < circle1Radius + circle2Radius;
}

void BowAndArrow::renderShuriken(glm::mat3 visMatrix, struct shuriken shk)
{
	glm::mat3 modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(shk.x + shurikensOffsetX, shk.y);
	modelMatrix *= Transform2D::Rotate(shurikensAng);
	modelMatrix *= Transform2D::Scale(ITEMS_SCALE, ITEMS_SCALE);
	if (!shk.hit) {
		RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);
	}
	else {
		if (shk.hitAnimationTime > EPS) {
			RenderMesh2D(meshes["hitShuriken"], shaders["VertexColor"], modelMatrix);
		}
	}
}

void BowAndArrow::renderBalloon(glm::mat3 visMatrix, struct balloon balloon)
{
	modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(balloon.x, balloon.y + balloonsOffsetY);
	modelMatrix *= Transform2D::Scale(balloon.scale, balloon.scale);
	modelMatrix *= Transform2D::Scale(BALLOON_THREAD_SCALE_FACTOR, BALLOON_THREAD_SCALE_FACTOR);
	if (balloon.color) {
		RenderMesh2D(meshes["redThread"], shaders["VertexColor"], modelMatrix);
	}
	else {
		RenderMesh2D(meshes["yellowThread"], shaders["VertexColor"], modelMatrix);
	}

	modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(balloon.x, balloon.y + balloon.scale * BALLOON_SCALE_FACTOR_Y + balloonsOffsetY);
	modelMatrix *= Transform2D::Scale(balloon.scale, balloon.scale);
	modelMatrix *= Transform2D::Scale(BALLOON_SCALE_FACTOR_X, BALLOON_SCALE_FACTOR_Y);
	if (balloon.color) {
		RenderMesh2D(meshes["redCircle"], shaders["VertexColor"], modelMatrix);
	}
	else {
		RenderMesh2D(meshes["yellowCircle"], shaders["VertexColor"], modelMatrix);
	}

	modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(balloon.x, balloon.y + balloon.scale * BALLOON_NECK_SCALE_FACTOR_Y / 2 + balloonsOffsetY);
	modelMatrix *= Transform2D::Scale(balloon.scale, balloon.scale);
	modelMatrix *= Transform2D::Scale(BALLOON_NECK_SCALE_FACTOR_X, BALLOON_NECK_SCALE_FACTOR_Y);
	if (balloon.color) {
		RenderMesh2D(meshes["redTriangle"], shaders["VertexColor"], modelMatrix);
	}
	else {
		RenderMesh2D(meshes["yellowTriangle"], shaders["VertexColor"], modelMatrix);
	}
}

void BowAndArrow::renderBow(glm::mat3 visMatrix)
{
	glm::mat3 modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(BOW_X, bowY);
	modelMatrix *= Transform2D::Rotate(bowAng);
	modelMatrix *= Transform2D::Scale(BOW_SCALE, BOW_SCALE);
	RenderMesh2D(meshes["bow"], shaders["VertexColor"], modelMatrix);
}

void BowAndArrow::renderBowArrow(glm::mat3 visMatrix)
{
	glm::mat3 modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(BOW_X, bowY);
	modelMatrix *= Transform2D::Rotate(bowAng);
	RenderMesh2D(meshes["arrowLine"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(BOW_X + ARROW_LINE_LENGTH + ARROW_TRIANGLE_SCALE, bowY);
	modelMatrix *= Transform2D::Translate(-(ARROW_LINE_LENGTH + ARROW_TRIANGLE_SCALE), 0);
	modelMatrix *= Transform2D::Rotate(bowAng);
	modelMatrix *= Transform2D::Translate(+(ARROW_LINE_LENGTH + ARROW_TRIANGLE_SCALE), 0);
	modelMatrix *= Transform2D::Rotate(-(RADIANS(180)) / 2);
	modelMatrix *= Transform2D::Scale(ARROW_TRIANGLE_SCALE, ARROW_TRIANGLE_SCALE);
	RenderMesh2D(meshes["arrowTriangle"], shaders["VertexColor"], modelMatrix);
}

void BowAndArrow::renderThrownArrow(glm::mat3 visMatrix)
{
	glm::mat3 modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(arrowX, arrowY);
	modelMatrix *= Transform2D::Rotate(arrowAng);
	RenderMesh2D(meshes["arrowLine"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(arrowX + ARROW_LINE_LENGTH + ARROW_TRIANGLE_SCALE, arrowY);
	modelMatrix *= Transform2D::Translate(-(ARROW_LINE_LENGTH + ARROW_TRIANGLE_SCALE), 0);
	modelMatrix *= Transform2D::Rotate(arrowAng);
	modelMatrix *= Transform2D::Translate(+(ARROW_LINE_LENGTH + ARROW_TRIANGLE_SCALE), 0);
	modelMatrix *= Transform2D::Rotate(-(RADIANS(180)) / 2);
	modelMatrix *= Transform2D::Scale(ARROW_TRIANGLE_SCALE, ARROW_TRIANGLE_SCALE);
	RenderMesh2D(meshes["arrowTriangle"], shaders["VertexColor"], modelMatrix);
}

void BowAndArrow::renderPowerBar(glm::mat3 visMatrix)
{
	glm::mat3 modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate((arrowSpeed - ARROW_MIN_SPEED) * POWER_BAR_FACTOR_X / 2 + POWER_BAR_OFFSET_X, bowY - POWER_BAR_OFFSET_Y);
	modelMatrix *= Transform2D::Scale((arrowSpeed - ARROW_MIN_SPEED) * POWER_BAR_FACTOR_X, POWER_BAR_SCALE_Y);
	RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
}

void BowAndArrow::renderShotSemaphore(glm::mat3 visMatrix)
{
	glm::mat3 modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(BOW_X + SHOT_SEMAPHORE_OFFSET_X, bowY + SHOT_SEMAPHORE_OFFSET_Y);
	modelMatrix *= Transform2D::Scale(SHOT_SEMAPHORE_SCALE, SHOT_SEMAPHORE_SCALE);
	if (shotUnallowed) {
		RenderMesh2D(meshes["redCircle"], shaders["VertexColor"], modelMatrix);
	}
	else {
		RenderMesh2D(meshes["greenCircle"], shaders["VertexColor"], modelMatrix);
	}
}

void BowAndArrow::renderCharacterFace(glm::mat3 visMatrix)
{
	glm::mat3 modelMatrix = modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(0, bowY - BOW_MIN_Y);
	RenderMesh2D(meshes["nose"], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes["mouth"], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes["leftEye"], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes["rightEye"], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes["leftEar"], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes["feather"], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes["hat"], shaders["VertexColor"], modelMatrix);
	modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(CHARACTER_HEAD_OFFSET_X, bowY + CHARACTER_HEAD_OFFSET_Y);
	modelMatrix *= Transform2D::Scale(CHARACTER_HEAD_SCALE, CHARACTER_HEAD_SCALE);
	RenderMesh2D(meshes["bisqueCircle"], shaders["VertexColor"], modelMatrix);
}


void BowAndArrow::renderCharacterBody(glm::mat3 visMatrix)
{
	// hands
	glm::mat3 modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(0, bowY - BOW_MIN_Y);
	RenderMesh2D(meshes["rightHand"], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes["leftHand"], shaders["VertexColor"], modelMatrix);

	// sheath line
	// we multiply by 4 the number of pixels in a line
	glLineWidth(4);
	modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(SHEATH_LINE_OFFSET_X, bowY - BOW_MIN_Y + SHEATH_LINE_OFFSET_Y);
	modelMatrix *= Transform2D::Rotate(-(RADIANS(180)) / 3);
	modelMatrix *= Transform2D::Scale(SHEATH_LINE_SCALE_X, 1);
	RenderMesh2D(meshes["arrowLine"], shaders["VertexColor"], modelMatrix);
	// we set the number of pixels in a line to the default value
	glLineWidth(1);

	// clothes
	modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(0, bowY - BOW_MIN_Y);
	RenderMesh2D(meshes["vest"], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes["pants"], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes["leftShoe"], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes["rightShoe"], shaders["VertexColor"], modelMatrix);

	// first arrow from sheath
	modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(SHEATH_FIRST_ARROW_X, bowY - SHEATH_OFFSET_Y + FIRST_ARROW_TRIANGLE_OFFSET_Y);
	modelMatrix *= Transform2D::Rotate(SHEATH_FIRST_ARROW_ANG);
	RenderMesh2D(meshes["arrowLine"], shaders["VertexColor"], modelMatrix);
	modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(SHEATH_FIRST_ARROW_X + ARROW_LINE_LENGTH + ARROW_TRIANGLE_SCALE, bowY - SHEATH_OFFSET_Y);
	modelMatrix *= Transform2D::Translate(-(ARROW_LINE_LENGTH + ARROW_TRIANGLE_SCALE), 0);
	modelMatrix *= Transform2D::Rotate(SHEATH_FIRST_ARROW_ANG);
	modelMatrix *= Transform2D::Translate(+(ARROW_LINE_LENGTH + ARROW_TRIANGLE_SCALE), 0);
	modelMatrix *= Transform2D::Rotate(-(RADIANS(180)) / 2);
	modelMatrix *= Transform2D::Scale(ARROW_TRIANGLE_SCALE, ARROW_TRIANGLE_SCALE);
	RenderMesh2D(meshes["arrowTriangle"], shaders["VertexColor"], modelMatrix);

	// second arrow from sheath
	modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(SHEATH_SECOND_ARROW_X, bowY - SHEATH_OFFSET_Y);
	modelMatrix *= Transform2D::Rotate((RADIANS(180)) / 2);
	RenderMesh2D(meshes["arrowLine"], shaders["VertexColor"], modelMatrix);
	modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(SHEATH_SECOND_ARROW_X + ARROW_LINE_LENGTH + ARROW_TRIANGLE_SCALE, bowY - SHEATH_OFFSET_Y);
	modelMatrix *= Transform2D::Translate(-(ARROW_LINE_LENGTH + ARROW_TRIANGLE_SCALE), 0);
	modelMatrix *= Transform2D::Rotate((RADIANS(180)) / 2);
	modelMatrix *= Transform2D::Translate(+(ARROW_LINE_LENGTH + ARROW_TRIANGLE_SCALE), 0);
	modelMatrix *= Transform2D::Rotate(-(RADIANS(180)) / 2);
	modelMatrix *= Transform2D::Scale(ARROW_TRIANGLE_SCALE, ARROW_TRIANGLE_SCALE);
	RenderMesh2D(meshes["arrowTriangle"], shaders["VertexColor"], modelMatrix);
}

// 2D visualization matrix
glm::mat3 BowAndArrow::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
	float sx, sy, tx, ty;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	tx = viewSpace.x - sx * logicSpace.x;
	ty = viewSpace.y - sy * logicSpace.y;

	return glm::transpose(glm::mat3(
		sx, 0.0f, tx,
		0.0f, sy, ty,
		0.0f, 0.0f, 1.0f));
}

// uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 BowAndArrow::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
	float sx, sy, tx, ty, smin;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	if (sx < sy)
		smin = sx;
	else
		smin = sy;
	tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
	ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

	return glm::transpose(glm::mat3(
		smin, 0.0f, tx,
		0.0f, smin, ty,
		0.0f, 0.0f, 1.0f));
}

void BowAndArrow::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
	glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	glEnable(GL_SCISSOR_TEST);
	glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
	GetSceneCamera()->Update();
}

void BowAndArrow::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void BowAndArrow::Update(float deltaTimeSeconds)
{
	int score = redBalloonsHits * RED_BALLOON_POINTS + yellowBalloonsHits * YELLOW_BALLOON_POINTS;

	cout << "Score: " << score << endl;

	cout << "Lifes: " << lifes << endl;

	for (int i = 0; i < NO_BALLOONS; i++) {
		if (!balloons[i].hit && pointCircleCollision(
			arrowX + (ARROW_LINE_LENGTH + ARROW_TRIANGLE_SCALE) * cos(arrowAng),
			arrowY + (ARROW_LINE_LENGTH + ARROW_TRIANGLE_SCALE) * sin(arrowAng),
			balloons[i].x, balloons[i].y + balloons[i].scale * BALLOON_SCALE_FACTOR_Y + balloonsOffsetY,
			balloons[i].scale * BALLOON_SCALE_FACTOR_Y)) {
			balloons[i].hit = true;
			balloons[i].hitAnimation = true;
			if (balloons[i].color) {
				redBalloonsHits++;
			}
			else {
				yellowBalloonsHits++;
			}
		}
		if (balloons[i].hit && balloons[i].hitAnimation) {
			balloons[i].scale -= deltaTimeSeconds;
			if (balloons[i].scale < 0) {
				balloons[i].scale = 0;
				// the animation is finished
				balloons[i].hitAnimation = false;
			}
		}
	}

	for (int i = 0; i < NO_SHURIKENS; i++) {
		if (!shurikens[i].hit && pointCircleCollision(arrowX + (ARROW_LINE_LENGTH + ARROW_TRIANGLE_SCALE) * cos(arrowAng),
			arrowY + (ARROW_LINE_LENGTH + ARROW_TRIANGLE_SCALE) * sin(arrowAng),
			shurikens[i].x + shurikensOffsetX, shurikens[i].y, ITEMS_SCALE)) {
			shurikens[i].hit = true;
		}
		if (!shurikens[i].hit && circleCircleCollision(BOW_X, bowY, BOW_X, shurikens[i].x + shurikensOffsetX, shurikens[i].y, ITEMS_SCALE)) {
			shurikens[i].hit = true;
			lifes--;
		}
		if (!shurikens[i].hit && circleCircleCollision(PANTS_CENTER_X, bowY - PANTS_OFFSET_Y, PANTS_RADIUS, shurikens[i].x + shurikensOffsetX, shurikens[i].y, ITEMS_SCALE)) {
			shurikens[i].hit = true;
			lifes--;
		}
		if (lifes < 1) {
			exit(0);
		}
		if (shurikens[i].hit && shurikens[i].hitAnimationTime > 0) {
			shurikens[i].hitAnimationTime -= deltaTimeSeconds;
			if (shurikens[i].hitAnimationTime < 0) {
				shurikens[i].hitAnimationTime = 0;
			}
		}
	}

	balloonsOffsetY += deltaTimeSeconds;

	shurikensOffsetX -= deltaTimeSeconds;
	shurikensAng += SHURIKEN_SPEED_FACTOR * deltaTimeSeconds;

	if (balloonsOffsetY > 2 * ITEMS_RESET_TIME) {
		initializeBalloons();
		balloonsOffsetY = 0;
	}

	if (shurikensOffsetX < LOGIC_SPACE_WIDTH - ITEMS_RESET_TIME) {
		initializeShurikens();
		shurikensOffsetX = LOGIC_SPACE_WIDTH + ITEMS_RESET_TIME;
	}

	if (incArrowSpeed && arrowSpeed < ARROW_MAX_SPEED - EPS) {
		arrowSpeed += ARROW_SPEED_FACTOR * deltaTimeSeconds;
		if (arrowSpeed > ARROW_MAX_SPEED) {
			arrowSpeed = ARROW_MAX_SPEED;
		}
	}

	if (thrownAndVisible) {
		thrownArrowDistance = arrowFinalSpeed * deltaTimeSeconds;
		arrowX += cos(arrowAng) * thrownArrowDistance;
		arrowY += sin(arrowAng) * thrownArrowDistance;
	}

	if (shotUnallowed) {
		timeElapsedAfterShot += deltaTimeSeconds;
		if (timeElapsedAfterShot > SECONDS_BETWEEN_SHOTS) {
			timeElapsedAfterShot = 0;
			shotUnallowed = false;
		}
	}

	glm::ivec2 resolution = window->GetResolution();

	// Sets the screen area where to draw - the left half of the window
	viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
	SetViewportArea(viewSpace, glm::vec3(0), true);

	// Compute uniform 2D visualization matrix
	visMatrix = glm::mat3(1);
	visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
	DrawScene(visMatrix);
}

void BowAndArrow::FrameEnd()
{

}

void BowAndArrow::DrawScene(glm::mat3 visMatrix)
{
	if (lifes >= 1) {
		// we double the number of pixels in a line
		glLineWidth(2);

		// bow
		renderBow(visMatrix);
		// arrow
		if (!shotUnallowed) {
			renderBowArrow(visMatrix);
		}
		else {
			if (isVisible(arrowX, arrowY)) {
				renderThrownArrow(visMatrix);
			}
			else {
				// the arrow is out of display
				thrownAndVisible = false;
				thrownArrowDistance = 0.000000f;
			}
		}

		// now we reset the value to the default one
		glLineWidth(1);

		// power bar
		if (arrowSpeed > ARROW_MIN_SPEED + EPS) {
			renderPowerBar(visMatrix);
		}

		// shot semaphore
		renderShotSemaphore(visMatrix);

		// character face
		renderCharacterFace(visMatrix);

		// character body
		renderCharacterBody(visMatrix);

		for (int i = 0; i < NO_BALLOONS; i++) {
			// render current balloon
			renderBalloon(visMatrix, balloons[i]);
		}

		for (int i = 0; i < NO_SHURIKENS; i++) {
			// shuriken
			renderShuriken(visMatrix, shurikens[i]);
		}
	}
}

void BowAndArrow::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_S)) {
		if (bowY > BOW_MIN_Y + EPS) {
			bowY -= BOW_ROTATION_SPEED_FACTOR * deltaTime;
			if (bowY < BOW_MIN_Y) {
				bowY = BOW_MIN_Y;
			}
		}
	}
	if (window->KeyHold(GLFW_KEY_W)) {
		if (bowY < BOW_MAX_Y - EPS) {
			bowY += BOW_ROTATION_SPEED_FACTOR * deltaTime;
			if (bowY > BOW_MAX_Y) {
				bowY = BOW_MAX_Y;
			}
		}
	}
}

void BowAndArrow::OnKeyPress(int key, int mods)
{

}

void BowAndArrow::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void BowAndArrow::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// display coordinates => logic coordinates
	// on OX: 0 - (res.x-1) => 0.0 - LOGIC_SPACE_WIDTH
	// on OY: (res.y-1) - 0 => 0.0 - LOGIC_SPACE_HEIGHT
	glm::ivec2 res = window->GetResolution();
	float mouseLogicX = mouseX * LOGIC_SPACE_WIDTH / (res.x - 1);
	if (mouseLogicX < BOW_X) {
		mouseLogicX = BOW_X;
	}
	float mouseLogicY = (res.y - 1 - mouseY) * LOGIC_SPACE_HEIGHT / (res.y - 1);
	bowAng = atan((mouseLogicY - bowY) / (mouseLogicX - BOW_X));
}

void BowAndArrow::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (button == MOUSE_LEFT_BUTTON) {
		if (!thrownAndVisible && !shotUnallowed) {
			incArrowSpeed = true;
		}
	}
}

void BowAndArrow::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	if (button == MOUSE_LEFT_BUTTON) {
		if (!thrownAndVisible && timeElapsedAfterShot < EPS) {
			shotUnallowed = true;
			timeElapsedAfterShot = 0.000000f;

			incArrowSpeed = false;
			arrowFinalSpeed = arrowSpeed;

			thrownArrowDistance = 0;
			arrowSpeed = ARROW_MIN_SPEED;
			thrownAndVisible = true;
			arrowX = BOW_X;
			arrowY = bowY;
			glm::ivec2 res = window->GetResolution();
			float mouseLogicX = mouseX * LOGIC_SPACE_WIDTH / (res.x - 1);
			if (mouseLogicX < BOW_X) {
				mouseLogicX = BOW_X;
			}
			float mouseLogicY = (res.y - 1 - mouseY) * LOGIC_SPACE_HEIGHT / (res.y - 1);
			arrowAng = atan((mouseLogicY - bowY) / (mouseLogicX - BOW_X));
		}

	}
}

void BowAndArrow::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}