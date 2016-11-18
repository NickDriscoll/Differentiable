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

	//Update bounding box's origin
	boundingBox.setOrigin(position);

	for (unsigned int boner = 0; boner < boxes.size(); boner++)
	{
		if (overlaps(boxes[boner]))
		{
			if (collidesFromTop(boundingBox, boxes[boner]) && !collidesFromTop(oldBoundingBox, boxes[boner]))
			{
				position.y = boxes[boner].getOrigin().y - textureHeight;
				onGround = true;
				//printf("colliding from top\n");
			}

			if (collidesFromLeft(boundingBox, boxes[boner]) && !collidesFromLeft(oldBoundingBox, boxes[boner]))
			{
				position.x = boxes[boner].getOrigin().x - textureWidth;
				pushesRightWall = true;
				//printf("colliding from left\n");
			}

			if (collidesFromRight(boundingBox, boxes[boner]) && !collidesFromRight(oldBoundingBox, boxes[boner]))
			{
				position.x = boxes[boner].getOrigin().x + boxes[boner].getSize().x;
				pushesLeftWall = true;
				//printf("colliding from right\n");
			}

			if (collidesFromBottom(boundingBox, boxes[boner]) && !collidesFromBottom(oldBoundingBox, boxes[boner]))
			{
				position.y = boxes[boner].getOrigin().y + boxes[boner].getSize().y;
				atCeiling = true;
				//printf("colliding from bottom\n");
			}
		}
	}

	//Apply physics based on object state
	if (onGround || atCeiling)
	{
		velocity.y = 0;
	}
	else
	{
		if (velocity.y < TERMINAL_VELOCITY)
		{
			velocity.y += ACCELERATION_DUE_TO_GRAVITY * timeDelta;
		} 
	}

	if (pushesRightWall || pushesLeftWall)
	{
		velocity.x = 0;
	}

	//Special case when ziplining is on
	if (ziplining)
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
		SDL_RenderDrawLine(r, 0  , position.y - camera.getPosition().y, SCREEN_WIDTH, position.y - camera.getPosition().y);
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

//The following functions are ONLY called when a collision has already occurred.
bool MovingObject::collidesFromLeft(AABB box, AABB other)
{
	return (box.getOrigin().x + box.getSize().x) > other.getOrigin().x;
}

bool MovingObject::collidesFromRight(AABB box, AABB other)
{
	return box.getOrigin().x < (other.getOrigin().x + other.getSize().x);
}

bool MovingObject::collidesFromTop(AABB box, AABB other)
{
	return (box.getOrigin().y + box.getSize().y) > other.getOrigin().y;
}

bool MovingObject::collidesFromBottom(AABB box, AABB other)
{
	return box.getOrigin().y < (other.getOrigin().y + other.getSize().y);
}