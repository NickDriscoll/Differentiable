#include "head.h"

Tile::Tile(SDL_Texture* Texture, Vector2 Position)
{
	texture = Texture;
	position = Position;
}

Vector2 Tile::getPosition()
{
	return position;
}

void Tile::setPosition(Vector2 Position)
{
	position = Position;
}

bool Tile::overlaps(Vector2 point)
{
	Vector2 pixelPosition = position * 32;
	return (point.x >= pixelPosition.x) && (point.x <= pixelPosition.x + 32) && (point.y >= pixelPosition.y) && (point.y <= pixelPosition.y + 32);
}

void Tile::draw(SDL_Renderer* r, bool debug, Camera camera)
{
	SDL_Rect objectRect = newRect(position - camera.getPosition(), Vector2(TILE_WIDTH, TILE_WIDTH));
	SDL_RenderCopy(r, texture, NULL, &objectRect);
}