#include "head.h"
#include <math.h>

AABB::AABB()
{

}

AABB::AABB(Vector2 Center, Vector2 HalfSize)
{
	center = Center;
	halfSize = HalfSize;
}

bool AABB::overlaps(AABB other)
{
	if (abs(center.x - other.center.x) > halfSize.x + other.halfSize.x) return false;
	if (abs(center.y - other.center.y) > halfSize.y + other.halfSize.y) return false;
	return true;
}