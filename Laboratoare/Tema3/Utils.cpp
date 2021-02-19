#include "Tema3.h"

using namespace std;

float Tema3::Distance(float xA, float yA, float xB, float yB)
{
	return sqrt((xA - xB) * (xA - xB) + (yA - yB) * (yA - yB));
}

bool Tema3::PointCircleCollision(float pointX, float pointY, float circleCenterX, float circleCenterY, float circleRadius)
{
	return Distance(pointX, pointY, circleCenterX, circleCenterY) < circleRadius;
}

float Tema3::Distance3D(float firstX, float firstY, float firstZ, float otherX, float otherY, float otherZ)
{
	return sqrt((firstX - otherX) * (firstX - otherX) +
		(firstY - otherY) * (firstY - otherY) +
		(firstZ - otherZ) * (firstZ - otherZ));
}


bool Tema3::SphereSphereCollision3D(float sphereX, float sphereY, float sphereZ, float sphereRadius, float otherX, float otherY, float otherZ, float otherRadius) {
		return Distance3D(sphereX, sphereY, sphereZ, otherX, otherY, otherZ) < (sphereRadius + otherRadius);
}

bool Tema3::SphereAABBCollision3D(float boxMinX, float boxMinY, float boxMinZ, float boxMaxX, float boxMaxY, float boxMaxZ, float sphereX, float sphereY, float sphereZ, float sphereRadius) {
	// get box closest point to sphere center by clamping
	float x = max(boxMinX, min(sphereX, boxMaxX));
	float y = max(boxMinY, min(sphereY, boxMaxY));
	float z = max(boxMinZ, min(sphereZ, boxMaxZ));

	return Distance3D(x, y, z, sphereX, sphereY, sphereZ) < sphereRadius;
}
