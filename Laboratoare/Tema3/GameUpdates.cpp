#include "Tema3.h"

#include <iostream>

#include <Core/Engine.h>

using namespace std;

void Tema3::ReinitializeValues() {
	platformsSpeed = minSpeed;
	fuel = FUEL_MAX;
	sphereX = -0.5f;
	sphereY = 0;
	onJump = false;
	onRise = false;
	falling = false;
	speedRun = false;
	speedRunSeconds = SPEED_RUN_SECS;
	onPowerUps = 0;
}

void Tema3::DifficultyUpdate() {
	double timeElapsed = Engine::GetElapsedTime() - timeElapsedBeforeStart;
	if (timeElapsed > PRO_DIFF_TIME) {
		difficulty = pro;
		fuelSensitivity = FUEL_SENSITIVITY_PRO;
		minSpeed = PLATFORMS_MIN_SPEED_PRO;
	}
	else if (timeElapsed > HARD_DIFF_TIME) {
		difficulty = hard;
		fuelSensitivity = FUEL_SENSITIVITY_HARD;
		minSpeed = PLATFORMS_MIN_SPEED_HARD;
	}
	else if (timeElapsed > MEDIUM_DIFF_TIME) {
		difficulty = medium;
		fuelSensitivity = FUEL_SENSITIVITY_MEDIUM;
		minSpeed = PLATFORMS_MIN_SPEED_MEDIUM;
	}
	if (platformsSpeed < minSpeed) {
		platformsSpeed = minSpeed;
	}
}

void Tema3::SpeedRunUpdate(float deltaTimeSeconds) {
	if (speedRun) {
		speedRunSeconds -= deltaTimeSeconds;
		if (speedRunSeconds < 0) {
			speedRun = false;
			onPowerUps = 0;
		}
	}
}

void Tema3::ScoreUpdate(float deltaTimeSeconds) {
	cout << "Score: " << noCollectablesGained << "\n";
	if (noCollectablesGained == MAX_SCORE) {
		exit(0);
	}
}

void Tema3::FallingUpdate(float deltaTimeSeconds) {
	sphereY -= deltaTimeSeconds;
	if (sphereY < -ANIMATION_FALLING_TIME) {
		lifes--;
		ReinitializeValues();
		InitObjects();
	}
}

void Tema3::CameraUpdate() {
	if (cameraPerson) {
		camera->SetPosition(glm::vec3(sphereX, THIRD_PERSON_CAMERA_Y, THIRD_PERSON_CAMERA_Z));
	}
	else {
		camera->SetPosition(glm::vec3(sphereX, FIRST_PERSON_CAMERA_Y + sphereY, FIRST_PERSON_CAMERA_Z));
	}
}

void Tema3::JumpUpdate(float deltaTimeSeconds) {
	if (onJump && onRise) {
		sphereY += JUMP_SENSITIVITY * deltaTimeSeconds;
		if (sphereY > RISE_TIME) {
			onRise = false;
			sphereY = RISE_TIME;
		}
	}
	else if (onJump && !onRise) {
		sphereY -= JUMP_SENSITIVITY * deltaTimeSeconds;
		if (sphereY <= 0) {
			onJump = false;
			sphereY = 0;
		}
	}
}

void Tema3::UpdateAndRenderObjects(float deltaTimeSeconds) {
	bool obstacleCollision = false;
	bool alive = false;
	// initialize random seed
	srand(GetTickCount());
	for (int i = 0; i < NO_GROUPS; i++) {
		RenderDecoratives(i);
		if (!speedRun) {
			platformsGroups[i].distance += platformsSpeed * deltaTimeSeconds;
		}
		else {
			platformsGroups[i].distance += PLATFORMS_SPEED_RUN_VALUE * deltaTimeSeconds;
		}
		// if group of platforms becomes invisible, we reset the values for it
		if (platformsGroups[i].distance > MAX_Z + PLATFORM_MAX_DIM_Z) {
			platformsGroups[i].distance -= NO_GROUPS * PLATFORM_MAX_DIM_Z;
			GenerateRandomGroupOfObjects(i);
		}

		for (int j = 0; j < NO_COLUMNS; j++) {
			// we make sure that the sphere is on a platform and it has PURPLE color
			if (!onJump && !falling
				&& j - NO_COLUMNS / 2 + (1 - PLATFORM_SCALE_X * 2) / 2 <= sphereX && sphereX <= j - NO_COLUMNS / 2 + 1 - (1 - PLATFORM_SCALE_X * 2) / 2
				&& platformsGroups[i].colors[j] != MISSING_PLATFORM && platformsGroups[i].distance >= 0 && 0 >= platformsGroups[i].distance - platformsGroups[i].lengths[j]) {
				alive = true;
				if (platformsGroups[i].colors[j] == RED) {
					lifes--;
				}
				else if (platformsGroups[i].colors[j] == ORANGE) {
					speedRun = true;
					speedRunSeconds = SPEED_RUN_SECS;
					onPowerUps = 1;
				}
				else if (platformsGroups[i].colors[j] == YELLOW) {
					fuel -= FUEL_LOSS;
				}
				else if (platformsGroups[i].colors[j] == GREEN) {
					fuel += FUEL_GAIN;
					if (fuel > FUEL_MAX) {
						fuel = FUEL_MAX;
					}
				}
				else if (platformsGroups[i].colors[j] == BISQUE) {
					if (lifes < LIFES_MAX_NO) {
						lifes++;
					}
				}
				platformsGroups[i].colors[j] = PURPLE;
			}

			if (!falling) {
				// we verify collision with obstacle
				if (platformsGroups[i].obstacleZCoord[j] != -1 && platformsGroups[i].colors[j] != MISSING_PLATFORM) {
					if (SphereAABBCollision3D(FIRST_PLATFORM_OFFSET_X + j - PLATFORM_SCALE_X, 0, -2 * PLATFORM_SCALE_Z + platformsGroups[i].distance - platformsGroups[i].obstacleZCoord[j],
						FIRST_PLATFORM_OFFSET_X + j + PLATFORM_SCALE_X, 2 * COLLECTABLE_SCALE, platformsGroups[i].distance - platformsGroups[i].obstacleZCoord[j],
						sphereX, SPHERE_SCALE * SPHERE_SCALE + sphereY, 0, SPHERE_SCALE / 2)) {
						Sleep(500);
						obstacleCollision = true;
						goto collision;
					}
				}

				// we verify collision with collectables
				if (platformsGroups[i].collectablesZCoord[j] != -1) {
					for (int k = 0; k < platformsGroups[i].noCollectables[j]; k++) {
						if (platformsGroups[i].collectablesState[j][k] == 1) {
							if (SphereSphereCollision3D(FIRST_PLATFORM_OFFSET_X + j, COLLECTABLE_SCALE / 2 + COLLECTABLE_OFFSET_Y,
								-COLLECTABLE_SCALE / 2 + platformsGroups[i].distance - platformsGroups[i].collectablesZCoord[j] - k, COLLECTABLE_SCALE / 2,
								sphereX, SPHERE_SCALE * SPHERE_SCALE + sphereY, 0, SPHERE_SCALE / 2)) {
								platformsGroups[i].collectablesState[j][k] = 0;
								noCollectablesGained++;
							}
						}
					}
				}
			}

			// we render the platforms
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(FIRST_PLATFORM_OFFSET_X + j, -PLATFORM_SCALE_Y, -PLATFORM_SCALE_Z * platformsGroups[i].lengths[j] + platformsGroups[i].distance));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(PLATFORM_SCALE_X, PLATFORM_SCALE_Y, PLATFORM_SCALE_Z * platformsGroups[i].lengths[j]));
			if (platformsGroups[i].colors[j] != MISSING_PLATFORM) {
				RenderSimpleMesh(meshes["cube_platform"], shaders["ColorShaderTema3"], modelMatrix, platformsGroups[i].colors[j], mapTextures["crate"]);
				RenderPlatformObjects(i, j);
			}
		}
	}


	if (!alive) {
		falling = true;
	}

	if (onJump) {
		falling = false;
	}
collision:
	if (obstacleCollision) {
		lifes--;
		ReinitializeValues();
		InitObjects();
	}
}
