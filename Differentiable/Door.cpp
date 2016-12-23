#include "head.h"

Door::Door()
{

}

Door::Door(const Vector2 &Origin, const char* ConnectedRoom, SDL_Renderer* r)
{
	connectedRoom = ConnectedRoom;
	origin = Origin;
	boundingBox.setOrigin(origin);
	boundingBox = AABB(origin, Vector2(width, height));
}

Vector2 Door::getOrigin()
{
	return origin;
}

const char* Door::getConnectedRoom()
{
	return connectedRoom;
}

AABB Door::getBoundingBox()
{
	return boundingBox;
}

void Door::draw(SDL_Renderer* r, SDL_Texture* texture, Camera camera, bool debug)
{
	SDL_Rect rect = { origin.x - camera.getPosition().x, origin.y - camera.getPosition().y, Door::width, Door::height };
	SDL_RenderCopy(r, texture, NULL, &rect);

	if (debug)
	{
		SDL_SetRenderDrawColor(r, 0, 0, 0xFF, 0xFF);
		SDL_RenderDrawRect(r, &rect);
	}
}