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