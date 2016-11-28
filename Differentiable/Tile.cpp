#include "head.h"

Tile::Tile(int TextureIndex, Vector2 Position, SDL_Renderer* r)
{
	texture = loadTexture("textures\\tile.png", r);
	textureIndex = TextureIndex;
	position = Position * 32;
}

Vector2 Tile::getPosition()
{
	return position;
}

void Tile::setPosition(Vector2 Position)
{
	position = Position * 32;
}

AABB Tile::aabb()
{
	return AABB(position, Vector2(TILE_WIDTH, TILE_WIDTH));
}

bool Tile::overlaps(Vector2 point)
{
	return (point.x >= position.x) && (point.x <= position.x + 32) && (point.y >= position.y) && (point.y <= position.y + 32);
}

void Tile::draw(SDL_Renderer* r, bool debug, Camera camera)
{
	SDL_Rect objectRect = newRect(position - camera.getPosition(), Vector2(TILE_WIDTH, TILE_WIDTH));
	SDL_Rect srcRect = newRect(Vector2(textureIndex * 32, 0), Vector2(TILE_WIDTH, TILE_WIDTH));
	SDL_RenderCopy(r, texture, &srcRect, &objectRect);

	if (debug)
	{
		SDL_Rect rect = newRect(position - camera.getPosition(), Vector2(TILE_WIDTH, TILE_WIDTH));
		SDL_SetRenderDrawColor(r, 0, 0xFF, 0, 0xFF);
		SDL_RenderDrawRect(r, &rect);
	}
}