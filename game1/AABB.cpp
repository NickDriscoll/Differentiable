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
	if (abs(center.x - other.center.x) > halfSize.x * 2 + other.halfSize.x) return false;
	if (abs(center.y - other.center.y) > halfSize.y * 2 + other.halfSize.y) return false;
	return true;
}

void AABB::draw(SDL_Renderer* r, bool debug)
{
	SDL_Rect rect = { (int)(center.x - halfSize.x), (int)(center.y - halfSize.y), (int)(halfSize.x * 2), (int)(halfSize.y * 2)};
	SDL_SetRenderDrawColor(r, 0x80, 0x80, 0x80, 0xFF);	
	SDL_RenderFillRect(r, &rect);

	if (debug)
	{
		SDL_Rect rect = { center.x - halfSize.x, center.y - halfSize.y, halfSize.x * 2, halfSize.y * 2 };
		SDL_SetRenderDrawColor(r, 0, 0xFF, 0, 0xFF);
		SDL_RenderDrawRect(r, &rect);
	}
}