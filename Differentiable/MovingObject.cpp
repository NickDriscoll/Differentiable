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
	rotation = 0;
	rotationalVelocity = 0;
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
	lastRotation = rotation;
	lastRotationalVelocity = rotationalVelocity;
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

	//Update rotation
	rotation = rotation + (rotationalVelocity * timeDelta);

	//Update bounding box's position
	boundingBox.setOrigin(position);

	//Check collision
	for (unsigned int i = 0; i < tiles.size(); i++)
	{
	   /*
		* For each tile, check if there is any overlap between this
		* bounding box and the tile's 
		*/
		if (boundingBox.overlaps(tiles[i].aabb()))
		{
		   /*
			* Computes the Minkowski sum of this and tiles[i] (a new rectangle),
			* then checks the distance from the center of this to the new rectangle and t
			* it's diagonals.
			* Still not entirely sure how this one works, I found this algorithm here
			* http://gamedev.stackexchange.com/questions/29786/a-simple-2d-rectangle-collision-algorithm-that-also-determines-which-sides-that
			*/

			//Get Minkowski sum
			float w = 0.5 * (textureWidth + tiles[i].aabb().getSize().x);
			float h = 0.5 * (textureHeight + tiles[i].aabb().getSize().y);
			float dx = boundingBox.getOrigin().x - tiles[i].aabb().getOrigin().x;
			float dy = boundingBox.getOrigin().y - tiles[i].aabb().getOrigin().y;

			float wy = w * dy;
			float hx = h * dx;

			if (wy > hx)
				if (wy > -hx)
				{
					/* collision at the top */
					//printf("Collision at top\n");
					//position.y = tiles[i].getPosition().y + TILE_WIDTH;
					atCeiling = true;
				}
				else
				{
					/* on the right */
					position.x = tiles[i].getPosition().x - textureWidth;
					pushesRightWall = true;
				}
			else
			{
				if (wy > -hx)
				{
					/* on the left */
					position.x = tiles[i].getPosition().x + TILE_WIDTH;
					pushesLeftWall = true;
				}
				else
				{
					/* at the bottom */
					position.y = tiles[i].getPosition().y - textureHeight;
					if (position.y < -textureHeight)
						position.y--;
					onGround = true;
				}
			}

		}
	}

	//Fall if the player isn't on the ground
	if (!onGround)
	{
		rotationalVelocity = 180;
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
		rotationalVelocity = 0;
		rotation = 0;
		velocity.y = 0;
	}

	//Stop player if they've bumped their head.
	if (atCeiling)
	{
		velocity.y *= -1;
	}

	//Stop player if they're pushing against a wall
	if (pushesLeftWall || pushesRightWall)
	{
		velocity.x = 0;
	}

	//Ziplining
	if (ziplining)
	{
		velocity.y = 0;
	}
}

void MovingObject::draw(SDL_Renderer* r, bool debug, Camera camera)
{
	SDL_Rect objectRect = newRect(position - camera.getPosition(), Vector2(getTextureWidth(), getTextureHeight()));
	if (facingRight)
		SDL_RenderCopyEx(r, texture, NULL, &objectRect, rotation, NULL, SDL_FLIP_NONE);
	else
		SDL_RenderCopyEx(r, texture, NULL, &objectRect, -rotation, NULL, SDL_FLIP_HORIZONTAL);

	if (ziplining)
	{
		SDL_SetRenderDrawColor(r, 0xFF, 0, 0, 0xFF);
		SDL_RenderDrawLine(r, 0, position.y + textureHeight / 2 - camera.getPosition().y, SCREEN_WIDTH, position.y + textureHeight / 2 - camera.getPosition().y);
	}

	if (debug)
	{
		SDL_Rect rect = newRect(boundingBox.getOrigin()  - camera.getPosition(), boundingBox.getSize());
		SDL_SetRenderDrawColor(r, 0xFF, 0, 0, 0xFF);
		SDL_RenderDrawRect(r, &rect);
	}
}