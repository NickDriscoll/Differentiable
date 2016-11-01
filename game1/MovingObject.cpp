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
	boundingBox = AABB(position, Vector2(textureWidth, textureHeight));
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
	oldBoundingBox = boundingBox;

	//Reset state vars
	onGround = false;
	atCeiling = false;
	pushesLeftWall = false;
	pushesRightWall = false;

	//Update position
	position = position + (velocity * timeDelta);

	//Ground collision placeholder
	if (position.y > SCREEN_HEIGHT - textureHeight)
	{
		onGround = true;
	}

	//Update bounding box's origin
	boundingBox.setOrigin(position);

	// TODO Check for collision
	// This loop sets flags and then later on the code decides what to actually do with them.
	for (unsigned int boner = 0; boner < boxes.size(); boner++)
	{
		if (overlaps(boxes[boner]))
		{
			printf("SIGLEMIIIIIIIIIIIC\n");
		}
	}

	if (pushesRightWall)
	{
		velocity.x = 0;
	}

	//Apply physics based on object state
	if (onGround)
	{
		velocity.y = 0;
	}
	else
	{
		velocity.y += ACCELERATION_DUE_TO_GRAVITY * timeDelta;
	}

	if (pushesRightWall || pushesLeftWall)
	{
		velocity.x = 0;
	}

}

void MovingObject::draw(SDL_Renderer* r, bool debug)
{
	SDL_Rect objectRect = newRect(position, Vector2(getTextureWidth(), getTextureHeight()));
	if (facingRight)
		SDL_RenderCopyEx(r, texture, NULL, &objectRect, 0, NULL, SDL_FLIP_NONE);
	else
		SDL_RenderCopyEx(r, texture, NULL, &objectRect, 0, NULL, SDL_FLIP_HORIZONTAL);

	if (debug)
	{
		SDL_Rect rect = newRect(boundingBox.getOrigin(), boundingBox.getSize());
		SDL_SetRenderDrawColor(r, 0xFF, 0, 0, 0xFF);
		SDL_RenderDrawRect(r, &rect);
	}
}

bool MovingObject::overlaps(AABB other)
{
	return boundingBox.overlaps(other);
}