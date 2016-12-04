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

Vector2 MovingObject::getVelocity()
{
	return velocity;
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

void MovingObject::setTexture(char* texturePath, SDL_Renderer* r)
{
	texture = loadTexture(texturePath, r);
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
}

void MovingObject::setFacing(bool facing)
{
	facingRight = facing;
}

void MovingObject::setPosition(Vector2 newPos)
{
	position = newPos;
}

// TODO: Seriously fix this shit
void MovingObject::UpdatePhysics(std::vector<Tile> tiles, double timeDelta)
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

	//Update bounding box's position
	boundingBox.setOrigin(position);

	//Check collision
	for (unsigned int i = 0; i < tiles.size() && !onGround; i++)
	{
		onGround = tiles[i].aabb().overlaps(boundingBox);
	}

	//Fall if the player isn't on the ground
	if (!onGround)
	{
		if (velocity.y > TERMINAL_VELOCITY)
		{
			velocity.y = TERMINAL_VELOCITY;
		}
		else
		{
			velocity.y += ACCELERATION_DUE_TO_GRAVITY * timeDelta;
		}
	}
	else
	{
		velocity.y = 0;
	}


}

void MovingObject::draw(SDL_Renderer* r, bool debug, Camera camera)
{
	SDL_Rect objectRect = newRect(position - camera.getPosition(), Vector2(getTextureWidth(), getTextureHeight()));
	if (facingRight)
		SDL_RenderCopyEx(r, texture, NULL, &objectRect, 0, NULL, SDL_FLIP_NONE);
	else
		SDL_RenderCopyEx(r, texture, NULL, &objectRect, 0, NULL, SDL_FLIP_HORIZONTAL);

	if (ziplining)
	{
		SDL_SetRenderDrawColor(r, 0xFF, 0, 0, 0xFF);
		SDL_RenderDrawLine(r, 0, position.y - camera.getPosition().y, SCREEN_WIDTH, position.y - camera.getPosition().y);
	}

	if (debug)
	{
		SDL_Rect rect = newRect(boundingBox.getOrigin()  - camera.getPosition(), boundingBox.getSize());
		SDL_SetRenderDrawColor(r, 0xFF, 0, 0, 0xFF);
		SDL_RenderDrawRect(r, &rect);
	}
}

bool MovingObject::overlaps(AABB other)
{
	return boundingBox.overlaps(other);
}