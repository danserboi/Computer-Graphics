#include "Tema3.h"

void Tema3::InitObjects() {
	// initialize random seed
	srand(GetTickCount());
	for (int i = 0; i < NO_GROUPS; i++) {
		platformsGroups[i].distance = -i * PLATFORM_MAX_DIM_Z;
		GenerateRandomGroupOfObjects(i);
		// start platform
		if (i == 0) {
			platformsGroups[i].colors[NO_COLUMNS / 2 - 1] = PURPLE;
			platformsGroups[i].obstacleZCoord[NO_COLUMNS / 2 - 1] = -1;
			platformsGroups[i].collectablesZCoord[NO_COLUMNS / 2 - 1] = -1;
		}
	}
}

void Tema3::GenerateRandomGroupOfObjects(int i) {
	for (int j = 0; j < NO_COLUMNS; j += 2) {
		// we should always have at least one blue platform in a 2-group
		char offset = rand() % 2;
		char bluePlatIdx = j + offset;
		char otherPlatIdx = j + (1 - offset);
		platformsGroups[i].colors[bluePlatIdx] = BLUE;
		platformsGroups[i].lengths[bluePlatIdx] = (PLATFORM_MAX_DIM_Z - PLATFORM_MAX_FREE_SPACE_Z) + rand() % PLATFORM_MAX_FREE_SPACE_Z;
		// we generate the other platform 
		// which doesn't exist or it has a different color
		switch (difficulty)
		{
		case easy:
			GeneratePlatformEasyLevel(i, otherPlatIdx);
			break;
		case medium:
			GeneratePlatformMediumLevel(i, otherPlatIdx);
			break;
		case hard:
			GeneratePlatformHardLevel(i, otherPlatIdx);
			break;
		case pro:
			GeneratePlatformProLevel(i, otherPlatIdx);
			break;
		}
		platformsGroups[i].lengths[otherPlatIdx] = (PLATFORM_MAX_DIM_Z - PLATFORM_MAX_FREE_SPACE_Z) + rand() % PLATFORM_MAX_FREE_SPACE_Z;
		if (platformsGroups[i].colors[j] != MISSING_PLATFORM) {
			GenerateObjectsOnPlatform(i, j);
		}
		if (platformsGroups[i].colors[j + 1] != MISSING_PLATFORM) {
			GenerateObjectsOnPlatform(i, j + 1);
		}
	}
	GenerateDecoratives(i);
}

void Tema3::GeneratePlatformEasyLevel(int i, int otherPlatIdx) {
	bool exists = rand() % 2;
	if (exists) {
		char color = rand() % 10;
		if (color == 0)
			platformsGroups[i].colors[otherPlatIdx] = RED;
		if (1 <= color && color <= 3)
			platformsGroups[i].colors[otherPlatIdx] = YELLOW;
		if (4 <= color && color <= 6)
			platformsGroups[i].colors[otherPlatIdx] = ORANGE;
		if (7 <= color && color <= 8)
			platformsGroups[i].colors[otherPlatIdx] = GREEN;
		if (color == 9)
			platformsGroups[i].colors[otherPlatIdx] = BISQUE;
	}
	else {
		platformsGroups[i].colors[otherPlatIdx] = MISSING_PLATFORM;
	}
}

void Tema3::GeneratePlatformMediumLevel(int i, int otherPlatIdx) {
	bool exists = rand() % 2;
	if (exists) {
		char color = rand() % 20;
		if (0 <= color && color <= 7)
			platformsGroups[i].colors[otherPlatIdx] = RED;
		if (7 <= color && color <= 11)
			platformsGroups[i].colors[otherPlatIdx] = YELLOW;
		if (12 <= color && color <= 14)
			platformsGroups[i].colors[otherPlatIdx] = ORANGE;
		if (15 <= color && color <= 17)
			platformsGroups[i].colors[otherPlatIdx] = GREEN;
		if (18 <= color && color <= 19)
			platformsGroups[i].colors[otherPlatIdx] = BISQUE;
	}
	else {
		platformsGroups[i].colors[otherPlatIdx] = MISSING_PLATFORM;
	}
}

void Tema3::GeneratePlatformHardLevel(int i, int otherPlatIdx) {
	int exists = rand() % 4;
	if (exists == 0) {
		char color = rand() % 20;
		if (0 <= color && color <= 9)
			platformsGroups[i].colors[otherPlatIdx] = RED;
		if (10 <= color && color <= 13)
			platformsGroups[i].colors[otherPlatIdx] = YELLOW;
		if (14 <= color && color <= 15)
			platformsGroups[i].colors[otherPlatIdx] = ORANGE;
		if (16 <= color && color <= 17)
			platformsGroups[i].colors[otherPlatIdx] = GREEN;
		if (18 <= color && color <= 19)
			platformsGroups[i].colors[otherPlatIdx] = BISQUE;
	}
	else {
		platformsGroups[i].colors[otherPlatIdx] = MISSING_PLATFORM;
	}
}

void Tema3::GeneratePlatformProLevel(int i, int otherPlatIdx) {
	int exists = rand() % 4;
	if (exists == 0) {
		char color = rand() % 20;
		if (0 <= color && color <= 11)
			platformsGroups[i].colors[otherPlatIdx] = RED;
		if (12 <= color && color <= 13)
			platformsGroups[i].colors[otherPlatIdx] = YELLOW;
		if (14 <= color && color <= 15)
			platformsGroups[i].colors[otherPlatIdx] = ORANGE;
		if (16 <= color && color <= 17)
			platformsGroups[i].colors[otherPlatIdx] = GREEN;
		if (18 <= color && color <= 19)
			platformsGroups[i].colors[otherPlatIdx] = BISQUE;
	}
	else {
		platformsGroups[i].colors[otherPlatIdx] = MISSING_PLATFORM;
	}
}

void Tema3::GenerateObjectsOnPlatform(int i, int j) {
	int objectType = rand() % 3;
	// we have some collectables on platform
	if (objectType == COLLECTABLES) {
		platformsGroups[i].collectablesZCoord[j] = rand() % (platformsGroups[i].lengths[j] - MAX_NO_COLLECTABLES + 1);
		platformsGroups[i].noCollectables[j] = rand() % (MAX_NO_COLLECTABLES + 1);
		for (int k = 0; k < platformsGroups[i].noCollectables[j]; k++) {
			platformsGroups[i].collectablesState[j][k] = 1;
		}
		platformsGroups[i].obstacleZCoord[j] = -1;
	}
	// we have one obstacle on platform
	else if (objectType == OBSTACLE) {
		platformsGroups[i].obstacleZCoord[j] = rand() % platformsGroups[i].lengths[j];
		platformsGroups[i].collectablesZCoord[j] = -1;
	}
	// we don't have anything
	else {
		platformsGroups[i].obstacleZCoord[j] = -1;
		platformsGroups[i].collectablesZCoord[j] = -1;
	}
}

void Tema3::GenerateDecoratives(int i) {
	int leftObject = rand() % 5;
	if (leftObject == PYRAMID) {
		platformsGroups[i].leftDecorative = PYRAMID;
		platformsGroups[i].leftDecorativeZOffset = rand() % DECORATIVE_MAX_Z + 1;
	}
	else if (leftObject == CYLINDER) {
		platformsGroups[i].leftDecorative = CYLINDER;
		platformsGroups[i].leftDecorativeZOffset = rand() % DECORATIVE_MAX_Z + 1;
	}
	else if (leftObject == SIMPLE_CYLINDER) {
		platformsGroups[i].leftDecorative = SIMPLE_CYLINDER;
		platformsGroups[i].leftDecorativeZOffset = rand() % DECORATIVE_MAX_Z + 1;
	}
	else if (leftObject == TETRAHEDRON) {
		platformsGroups[i].leftDecorative = TETRAHEDRON;
		platformsGroups[i].leftDecorativeZOffset = rand() % DECORATIVE_MAX_Z + 1;
	}
	else {
		platformsGroups[i].leftDecorative = NO_OBJECT;
		platformsGroups[i].leftDecorativeZOffset = -1;
	}

	int rightObject = rand() % 5;
	if (rightObject == PYRAMID) {
		platformsGroups[i].rightDecorative = PYRAMID;
		platformsGroups[i].rightDecorativeZOffset = rand() % DECORATIVE_MAX_Z + 1;
	}
	else if (leftObject == CYLINDER) {
		platformsGroups[i].rightDecorative = CYLINDER;
		platformsGroups[i].rightDecorativeZOffset = rand() % DECORATIVE_MAX_Z + 2;
	}
	else if (leftObject == SIMPLE_CYLINDER) {
		platformsGroups[i].rightDecorative = SIMPLE_CYLINDER;
		platformsGroups[i].rightDecorativeZOffset = rand() % DECORATIVE_MAX_Z + 1;
	}
	else if (leftObject == TETRAHEDRON) {
		platformsGroups[i].rightDecorative = TETRAHEDRON;
		platformsGroups[i].rightDecorativeZOffset = rand() % DECORATIVE_MAX_Z + 1;
	}
	else {
		platformsGroups[i].rightDecorative = NO_OBJECT;
		platformsGroups[i].rightDecorativeZOffset = -1;
	}
}