#include "head.h"

AABB::AABB()
{

}

AABB::AABB(const Vector2 &Origin, const Vector2 &Size)
{
	origin = Origin;
	size = Size;
	color = { 0x80, 0x80, 0x80, 0xFF };
}

AABB::AABB(const Vector2 &Origin, const Vector2 &Size, SDL_Color Color)
{
	origin = Origin;
	size = Size;
	color = Color;
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

bool AABB::overlaps(Vector2 other)
{
	return (other.x >= origin.x) && (other.x <= origin.x + size.x) && (other.y >= origin.y) && (other.y <= origin.y + size.y);
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
	SDL_SetRenderDrawColor(r, color.r, color.g, color.b, 0xFF);	
	SDL_RenderFillRect(r, &rect);

	if (debug)
	{
		SDL_Rect rect = newRect(origin - camera.getPosition(), size);
		SDL_SetRenderDrawColor(r, 0, 0xFF, 0, 0xFF);
		SDL_RenderDrawRect(r, &rect);
	}
}