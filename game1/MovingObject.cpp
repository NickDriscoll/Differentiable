#include "head.h"

MovingObject::MovingObject(char* pathToTexture, Vector2 Position, SDL_Renderer* r)
{
	texture = loadTexture(pathToTexture, r);
	position = Position;
}

MovingObject::MovingObject(char* pathToTexture, Vector2 Position, AABB BoundingBox, SDL_Renderer* r)
{
	texture = loadTexture(pathToTexture, r);
	position = Position;
	boundingBox = BoundingBox;
}

SDL_Texture* MovingObject::getTexture()
{
	return texture;
}

void MovingObject::UpdatePhysics()
{
	//Store previous values in respective variables.
	lastPosition = position;
	lastVelocity = velocity;
	pushedRightWall = pushesRightWall;
	pushedLeftWall = pushesLeftWall;
	wasOnGround = onGround;
	wasAtCeiling = atCeiling;

	
}