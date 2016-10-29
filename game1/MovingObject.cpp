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

void MovingObject::UpdatePhysics(std::vector<AABB> boxes, double timeDelta)
{
	//Store previous values in respective variables.
	lastPosition = position;
	lastVelocity = velocity;
	pushedRightWall = pushesRightWall;
	pushedLeftWall = pushesLeftWall;
	wasOnGround = onGround;
	wasAtCeiling = atCeiling;

	//Update position
	position = position + (velocity * timeDelta);

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
	boundingBox.setCenter(position);

	// TODO Check for collision
	for (int i = 0; i < boxes.size(); i++)
	{
		if (!overlaps(boxes[i]))
		{
			velocity.y += ACCELERATION_DUE_TO_GRAVITY * timeDelta;
		}
	}

	/*
	//Apply gravity if not on ground
	if (!onGround && velocity.y < TERMINAL_VELOCITY)
	{
		velocity.y += ACCELERATION_DUE_TO_GRAVITY * timeDelta;
	}
	*/

	//Store boundingBox as oldBoundingBox
	oldBoundingBox = boundingBox;

}

void MovingObject::draw(SDL_Renderer* r, bool debug)
{
	SDL_Rect objectRect = { (int)round(position.x) , (int)round(position.y), this->getTextureWidth(), this->getTextureHeight() };
	if (facingRight)
		SDL_RenderCopyEx(r, texture, NULL, &objectRect, 0, NULL, SDL_FLIP_NONE);
	else
		SDL_RenderCopyEx(r, texture, NULL, &objectRect, 0, NULL, SDL_FLIP_HORIZONTAL);

	if (debug)
	{
		SDL_Rect rect = {boundingBox.getCenter().x, boundingBox.getCenter().y, boundingBox.getHalfSize().x * 2, boundingBox.getHalfSize().y * 2};
		SDL_SetRenderDrawColor(r, 0xFF, 0, 0, 0xFF);
		SDL_RenderDrawRect(r, &rect);
	}
}

bool MovingObject::overlaps(AABB other)
{
	return boundingBox.overlaps(other);
}