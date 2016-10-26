#include "head.h"

AABB::AABB()
{

}

AABB::AABB(Vector2 Center, Vector2 HalfSize)
{
	center = Center;
	halfSize = HalfSize;
}

Vector2 AABB::getCenter()
{
	return center;
}

Vector2 AABB::getHalfSize()
{
	return halfSize;
}

void AABB::setCenter(Vector2 newCenter)
{
	center = newCenter;
}

void AABB::setHalfSize(Vector2 newHalfSize)
{
	halfSize = newHalfSize;
}

bool AABB::overlaps(AABB other)
{
	if (abs(center.x - other.center.x) > halfSize.x + other.halfSize.x) return false;
	if (abs(center.y - other.center.y) > halfSize.y + other.halfSize.y) return false;
	return true;
}

void AABB::draw(SDL_Renderer* r)
{
	SDL_Rect rect = { center.x - halfSize.x, center.y - halfSize.y, halfSize.x * 2, halfSize.y * 2 };
	SDL_SetRenderDrawColor(r, 0x80, 0x80, 0x80, 0xFF);
	
	SDL_RenderFillRect(r, &rect);
}