#include "head.h"

AABB::AABB()
{

}

AABB::AABB(Vector2 Origin, Vector2 Size)
{
	origin = Origin;
	size = Size;
}

Vector2 AABB::getOrigin()
{
	return origin;
}

Vector2 AABB::getSize()
{
	return size;
}

void AABB::setOrigin(Vector2 newOrigin)
{
	origin = newOrigin;
}

void AABB::setSize(Vector2 newSize)
{
	size = newSize;
}

bool AABB::overlaps(AABB other)
{
	bool flag = true;
	Vector2* thisCenter = new Vector2(origin.x + (size.x / 2), origin.y + (size.y / 2));
	Vector2* otherCenter = new Vector2(other.getOrigin().x + (other.getSize().x / 2), other.getOrigin().y + (other.getSize().y / 2));
	if ((abs(thisCenter->x - otherCenter->x) > (size.x / 2) + (other.getSize().x / 2)) || (abs(thisCenter->y - otherCenter->y) > (size.y / 2) + (other.getSize().y / 2)))
	{
		flag = false;
	}
	delete thisCenter;
	delete otherCenter;
	return flag;
}

void AABB::draw(SDL_Renderer* r, bool debug, Camera camera)
{
	SDL_Rect rect = newRect(origin - camera.getPosition(), size);
	SDL_SetRenderDrawColor(r, 0x80, 0x80, 0x80, 0xFF);	
	SDL_RenderFillRect(r, &rect);

	if (debug)
	{
		SDL_Rect rect = newRect(origin - camera.getPosition(), size);
		SDL_SetRenderDrawColor(r, 0, 0xFF, 0, 0xFF);
		SDL_RenderDrawRect(r, &rect);
	}
}