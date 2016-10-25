#include "head.h"

MovingObject::MovingObject()
{
	texture = NULL;
}

MovingObject::MovingObject(char* pathToTexture, Vector2 Position, SDL_Renderer* r, bool FacingRight)
{
	texture = loadTexture(pathToTexture, r);
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
	position = Position;
	facingRight = FacingRight;
}

SDL_Texture* MovingObject::getTexture()
{
	return texture;
}

Vector2 MovingObject::getPosition()
{
	return position;
}

int MovingObject::getTextureWidth()
{
	return textureWidth;
}

int MovingObject::getTextureHeight()
{
	return textureHeight;
}

bool MovingObject::getFacing()
{
	return facingRight;
}

void MovingObject::setFacing(bool facing)
{
	facingRight = facing;
}

void MovingObject::UpdatePhysics(double timeDelta)
{
	//Store previous values in respective variables.
	lastPosition = position;
	lastVelocity = velocity;
	pushedRightWall = pushesRightWall;
	pushedLeftWall = pushesLeftWall;
	wasOnGround = onGround;
	wasAtCeiling = atCeiling;

	//Update position
	position = position.add(velocity.multiply(timeDelta));

	//Ground collision placeholder
	if (position.y > SCREEN_HEIGHT - textureHeight)
	{
		position.y = SCREEN_HEIGHT - textureHeight;
		onGround = true;
	}
	else
	{
		onGround = false;
	}

	//Update bounding boxes position
	boundingBox.setCenter(position.add(aabbOffset));

	//Apply gravity if not on ground
	if (!onGround && velocity.y < TERMINAL_VELOCITY)
	{
		velocity.y += 0.0001;
	}
}