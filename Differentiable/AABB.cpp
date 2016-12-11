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
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = origin.x;
	rightA = origin.x + size.x;
	topA = origin.y;
	bottomA = origin.y + size.y;

	//Calculate the sides of rect B
	leftB = other.origin.x;
	rightB = other.origin.x + other.size.x;
	topB = other.origin.y;
	bottomB = other.origin.y + other.size.y;

	//If any of the sides from A are outside of B
	if (bottomA < topB)
	{
		return false;
	}

	if (topA > bottomB)
	{
		return false;
	}

	if (rightA < leftB)
	{
		return false;
	}

	if (leftA > rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

bool* AABB::collisionSide(AABB other)
{
	bool* sides = new bool[NUMBER_OF_SIDES];

	//Check collision side.
	if (origin.y + size.y > other.origin.y)
	{
		sides[Bottom] = true;
	}
	else if (other.origin.y + other.size.y > origin.y)
	{
		sides[Top] = true;
	}
	else if (origin.x + size.x > other.origin.x)
	{
		sides[Right] = true;
	}
	else if (other.origin.x + other.size.x > origin.x)
	{
		sides[Left] = true;
	}

	return sides;
	
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